from ctypes import *
import json
import platform

camelion = CDLL("libcamelion.{}".format("so" if platform.uname()[0] != "Darwin" else "dylib"))


class Data(Union):
    pass


class CMLNode(Structure):
    pass


class CMLBytes(Structure):
    pass


CMLBytes._fields_ = [
    ('data', POINTER(c_uint8)),
    ('size', c_int32),
]

Data._fields_ = [
    ('string', c_char_p),
    ('integer', c_int32),
]

CMLNode._fields_ = [
    ('name', c_char_p),
    ('type', c_int),
    ('data', Data),
    ('nodes', POINTER(POINTER(CMLNode))),
    ('ncount', c_uint32),
]


camelion.CML_StorableFromString.argtypes = [c_char_p, POINTER(POINTER(CMLNode))]
camelion.CML_StorableFromString.restype = c_uint
camelion.CML_ThawData.argtypes = [c_char_p, c_uint32, POINTER(POINTER(CMLNode))]
camelion.CML_ThawData.restype = c_uint
camelion.CML_NfreezeNode.argtypes = [POINTER(CMLNode), POINTER(POINTER(CMLBytes))]
camelion.CML_NfreezeNode.restype = c_uint
camelion.CML_HelpError.restype = c_char_p


def __thaw(c, o=None):
    if c.contents.ncount == 0:
        return

    if o is None:
        o = __make_type(c.contents)

    for n in xrange(0,c.contents.ncount):
        node = c.contents.nodes[n]
        temp = __make_type(node.contents)

        if type(o) is dict:
            name = node.contents.name
            o[name] = temp
            __thaw(node, o[name])
        elif type(o) is list:
            o.append(temp)
            __thaw(node,temp)

    return o


def __make_type(d):
    return {
        0: lambda: None,
        1: lambda: d.data.integer,
        2: lambda: d.data.string,
        3: lambda: [],
        4: lambda: {},
    }[d.type]()


def thaw(data):
    node = POINTER(CMLNode)()

    err = camelion.CML_ThawData(data, len(data), byref(node))
    if err > 0:
        raise Exception("Error :", help_error(err))

    res = __thaw(node)

    camelion.CML_NodeFree(byref(node))

    return res


def help_error(code):
    return camelion.CML_HelpError(code)


def nfreeze(data):
    data = json.dumps(data, separators=(", ", "=>"))
    res = ""
    node = POINTER(CMLNode)()
    b = POINTER(CMLBytes)()

    err = camelion.CML_StorableFromString(data, byref(node))
    if err > 0:
        raise Exception("Error :", help_error(err))

    err = camelion.CML_NfreezeNode(node, byref(b))
    if err > 0:
        raise Exception("Error :", help_error(err))

    for n in xrange(0, b.contents.size):
        res += format(b.contents.data[n], '02x')

    camelion.CML_DataFree(byref(b))
    camelion.CML_NodeFree(byref(node))

    return res.decode("hex")
