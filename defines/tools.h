#ifndef TOOLS_H
#define TOOLS_H

#define CHECKPTR(X)                       \
    if (!X) return CML_ERROR_USER_BADALLOC;
#define CHECKERR(X)                       \
{                                         \
    CML_Error _res = X;                   \
    if (_res != CML_ERROR_SUCCESS)        \
        return _res;                      \
}

#endif // TOOLS_H
