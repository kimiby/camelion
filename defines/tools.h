#ifndef TOOLS_H
#define TOOLS_H

#define CHECKPTR(X)                         \
    if (!X) return CML_ERROR_USER_BADPOINTER
#define CHECKSTR(X)                         \
    if (!X) return CML_ERROR_USER_BADSTRING
#define CHECKJAR(X)                         \
    if (((X)->type != CML_TYPE_HASH) &&     \
        ((X)->type != CML_TYPE_ARRAY))      \
        return CML_ERROR_USER_BADTYPE;
#define CHECKTYP(X)                         \
    if (((X) < CML_TYPE_FIRST) ||           \
        ((X) > CML_TYPE_LAST ))             \
        return CML_ERROR_USER_BADTYPE
#define CHECKERR(X)                         \
{                                           \
    CML_Error _res = X;                     \
    if (_res != CML_ERROR_SUCCESS)          \
        return _res;                        \
}
#define EXTENDNODE(NODE, SIZE)              \
{                                           \
    CML_Node ** oldptr = (NODE)->nodes;     \
    (NODE)->nodes =                         \
        realloc((NODE)->nodes,              \
                sizeof(CML_Node *) *        \
                ((NODE)->ncount + SIZE));   \
    if (!(NODE)->nodes)                     \
    {                                       \
        (NODE)->nodes = oldptr;             \
        return CML_ERROR_USER_BADALLOC;     \
    }                                       \
}
#define CHECKMEM(X)                         \
    if (!X) return CML_ERROR_USER_BADALLOC

#endif // TOOLS_H
