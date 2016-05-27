from ctypes import *

camelion = CDLL ("../[bin]/libcamelion.dylib")


class Data(Union):
    pass


class CMLNode(Structure):
    pass


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
        # todo: err 2 string
        raise Exception("Bad data :", err)

    res = __thaw(node)

    camelion.CML_NodeFree(node)

    return res


def freeze():
    """Not yet implemented"""
    pass

