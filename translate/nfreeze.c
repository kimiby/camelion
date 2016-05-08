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

#include <string.h>

#include "../translate/nfreeze.h"
#include "../reader/read.h"
#include "../defines/tools.h"
#include "../nodes/basis.h"
#include "../defines/consts.h"
#include "../serials/swrite.h"

static CML_Error encode_node(CML_Node * node, CML_Bytes * result);

static CML_Error encode_undef(CML_Node * node, CML_Bytes * result)
{
    UNUSED(node);

    CHECKERR(CML_SerialWriteUINT8(result, CML_PERL_UNDEF));

    return CML_ERROR_SUCCESS;
}

static CML_Error encode_integer(CML_Node * node, CML_Bytes * result)
{
    CHECKERR(CML_SerialWriteUINT8(result, CML_PERL_INT32));
    CHECKERR(CML_SerialWriteINT32(result, node->data.integer));

    return CML_ERROR_SUCCESS;
}

static CML_Error encode_string(CML_Node * node, CML_Bytes * result)
{
    CHECKERR(CML_SerialWriteUINT8( result, CML_PERL_DATALONG));
    CHECKERR(CML_SerialWriteUINT32(result, strlen(node->data.string)));
    CHECKERR(CML_SerialWriteString(result, node->data.string));

    return CML_ERROR_SUCCESS;
}

static CML_Error encode_array(CML_Node * node, CML_Bytes * result)
{
    CHECKERR(CML_SerialWriteUINT8 (result, CML_PERL_EXTENDED));
    CHECKERR(CML_SerialWriteUINT8 (result, CML_PERL_ARRAY));
    CHECKERR(CML_SerialWriteUINT32(result, node->ncount));
    uint32_t i;
    for (i = 0; node->ncount; i++)
        CHECKERR(encode_node(node, result));

    return CML_ERROR_SUCCESS;
}

static CML_Error encode_hash(CML_Node * node, CML_Bytes * result)
{
    CHECKERR(CML_SerialWriteUINT8 (result, CML_PERL_EXTENDED));
    CHECKERR(CML_SerialWriteUINT8 (result, CML_PERL_HASH));
    CHECKERR(CML_SerialWriteUINT32(result, node->ncount));
    uint32_t i;
    for (i = 0; node->ncount; i++)
        CHECKERR(encode_node(node, result));

    return CML_ERROR_SUCCESS;
}

static CML_Error encode_name(CML_Node * node, CML_Bytes * result)
{
    if (node->name)
    {
        CHECKERR(CML_SerialWriteUINT32(result, strlen(node->name)));
        CHECKERR(CML_SerialWriteString(result, node->name));
    }

    return CML_ERROR_SUCCESS;
}

static CML_Error encode_node(CML_Node * node, CML_Bytes * result)
{
    switch (node->type)
    {
    case CML_TYPE_UNDEF   : CHECKERR(encode_undef  (node, result)); break;
    case CML_TYPE_INTEGER : CHECKERR(encode_integer(node, result)); break;
    case CML_TYPE_STRING  : CHECKERR(encode_string (node, result)); break;
    case CML_TYPE_ARRAY   : CHECKERR(encode_array  (node, result)); break;
    case CML_TYPE_HASH    : CHECKERR(encode_hash   (node, result)); break;
    default :
        return CML_ERROR_USER_BADTYPE;
    }

    CHECKERR(encode_name(node, result));

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NfreezeNode(CML_Node * node, CML_Bytes ** result)
{
    CHECKPTR(node);
    CHECKPTR(result);

    *result = malloc(sizeof(CML_Bytes));
    CHECKMEM(*result);
    (*result)->data = NULL;
    (*result)->size = 0;

    uint32_t i;
    for (i = 0; i < sizeof(signature); i++)
        CHECKERC(CML_SerialWriteUINT8(*result, signature[i]),
                 free(*result));

    CHECKERC(CML_SerialWriteUINT32(*result, node->ncount),
             free(*result));

    for (i = 0; i < node->ncount; i++)
        CHECKERC(encode_node(node->nodes[i], *result),
                 free(*result));

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NfreezeStorable(char * storable, CML_Bytes ** result)
{
    CHECKPTR(storable);
    CHECKPTR(result);

    CML_Node * temp;
    CHECKERR(CML_StorableFromString(storable, &temp));
    CHECKERC(CML_NfreezeNode(temp, result),
             CML_NodeFree(temp));

    return CML_ERROR_SUCCESS;
}
