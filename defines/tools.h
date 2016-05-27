/* «Camelion» - Perl storable/C struct back-and-forth translator
*
*  Copyright (C) Alexey Shishkin 2016
*
*  This file is part of Project «Camelion».
*
*  Project «Camelion» is free software: you can redistribute it and/or modify
*  it under the terms of the GNU Lesser General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Project «Camelion» is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public License
*  along with Project «Camelion». If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TOOLS_H
#define TOOLS_H

#define UNUSED(X) (void)(X)

/* Extended library tools                  */
/* E+ */
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
#define CHECKERC(X, ACTIONS)                \
{                                           \
    CML_Error _res = X;                     \
    if (_res != CML_ERROR_SUCCESS)          \
    {                                       \
        ACTIONS;                            \
        return _res;                        \
    }                                       \
}
#define CHECKMEM(X)                         \
    if (!X) return CML_ERROR_USER_BADALLOC
/* E- */

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

#endif // TOOLS_H
