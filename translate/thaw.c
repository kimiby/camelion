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

#include <stdio.h>
#include <stdlib.h>

#include "../translate/thaw.h"
#include "../nodes/basis.h"
#include "../defines/tools.h"
#include "../serials/sread.h"
#include "../defines/consts.h"

#define CML_PERL_MAJOR_VERSION (0x05)

static CML_Error process_element(CML_Bytes * bytes, uint32_t * bpos,
                                 CML_Node * root, CML_Bool hasname);

static CML_Error process_name(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * target)
{
    uint32_t name_len;
    CHECKERR(CML_SerialsReadUINT32(bytes, bpos, &name_len));
    if (name_len > 256) return CML_ERROR_USER_BADNAME;

    char name[256];
    name[name_len] = '\0';
    CHECKERR(CML_SerialsReadDATA(bytes, bpos, (uint8_t *)name, name_len));

    CHECKERR(CML_NodeSetName(target, name));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_int8(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * root, CML_Bool hasname)
{
    int8_t value;
    CHECKERR(CML_SerialsReadINT8(bytes, bpos, &value));

    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_INTEGER, &child));
    CHECKERC(CML_NodeSetInteger(child, value),
             CML_NodeFree(child));
    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_int32(CML_Bytes * bytes, uint32_t * bpos,
                                CML_Node * root, CML_Bool hasname)
{
    int32_t value;
    CHECKERR(CML_SerialsReadINT32(bytes, bpos, &value));

    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_INTEGER, &child));
    CHECKERC(CML_NodeSetInteger(child, value),
             CML_NodeFree(child));
    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_string(CML_Bytes * bytes, uint32_t * bpos,
                                 CML_Node * root, CML_Bool hasname)
{
    uint8_t str_len;
    CHECKERR(CML_SerialsReadUINT8(bytes, bpos, &str_len));
    char string[257];
    CHECKERR(CML_SerialsReadDATA(bytes, bpos, (uint8_t *)string, str_len));
    string[str_len] = '\0';

    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_STRING, &child));
    CHECKERC(CML_NodeSetString(child, string),
             CML_NodeFree(child));
    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_data(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * root, CML_Bool hasname)
{
    uint32_t str_len;
    CHECKERR(CML_SerialsReadUINT32(bytes, bpos, &str_len));
    char * string = calloc(str_len + 1, 1);
    if (!string) return CML_ERROR_USER_BADALLOC;
    CHECKERR(CML_SerialsReadDATA(bytes, bpos, (uint8_t *)string, str_len));

    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_STRING, &child));
    CHECKERC(CML_NodeSetString(child, string),
             CML_NodeFree(child);
             free(string));

    free(string);

    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_undef(CML_Bytes * bytes, uint32_t * bpos,
                                CML_Node * root, CML_Bool hasname)
{
    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_UNDEF, &child));

    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_array(CML_Bytes * bytes, uint32_t * bpos,
                                CML_Node * root, CML_Bool hasname)
{
    uint32_t size;
    CHECKERR(CML_SerialsReadUINT32(bytes, bpos, &size));

    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_ARRAY, &child));

    while (size)
    {
        CHECKERC(process_element(bytes, bpos, child, CML_FALSE),
                 CML_NodeFree(child));
        size--;
    }

    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_hash(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * root, CML_Bool hasname)
{
    uint32_t size;
    CHECKERR(CML_SerialsReadUINT32(bytes, bpos, &size));

    CML_Node * child;
    CHECKERR(CML_NodeCreate(CML_TYPE_HASH, &child));

    while (size)
    {
        CHECKERC(process_element(bytes, bpos, child, CML_TRUE),
                 CML_NodeFree(child));
        size--;
    }

    if (hasname)
        CHECKERC(process_name(bytes, bpos, child),
                 CML_NodeFree(child));
    CHECKERC(CML_NodeAppend(root, child),
             CML_NodeFree(child));

    return CML_ERROR_SUCCESS;
}

static CML_Error process_element(CML_Bytes * bytes, uint32_t * bpos,
                                 CML_Node * root, CML_Bool hasname)
{
    uint8_t type;
    CHECKERR(CML_SerialsReadUINT8(bytes, bpos, &type));

    switch (type)
    {
    case CML_PERL_INT8    :
        return process_int8  (bytes, bpos, root, hasname);
    case CML_PERL_INT32   :
        return process_int32 (bytes, bpos, root, hasname);
    case CML_PERL_STRING  :
        return process_string(bytes, bpos, root, hasname);
    case CML_PERL_DATALONG:
        return process_data  (bytes, bpos, root, hasname);
    case CML_PERL_UNDEF   :
        return process_undef (bytes, bpos, root, hasname);
    case CML_PERL_EXTENDED :
        CHECKERR(CML_SerialsReadUINT8(bytes, bpos, &type));
        switch (type)
        {
        case CML_PERL_ARRAY :
            return process_array(bytes, bpos, root, hasname);
        case CML_PERL_HASH :
            return process_hash (bytes, bpos, root, hasname);
        default:
            return CML_ERROR_USER_BADTYPE;
        }
    default:
        return CML_ERROR_USER_BADTYPE;
    }

    return CML_ERROR_SUCCESS;
}

CML_Error CML_ThawBytes(CML_Bytes * bytes, CML_Node ** result)
{
    CHECKPTR(bytes);
    CHECKPTR(result);

    uint32_t count;
    uint32_t i;
    uint32_t bpos = 0;

    uint8_t sign;
    CHECKERR(CML_SerialsReadUINT8(bytes, &bpos, &sign));
    if (sign != CML_PERL_MAJOR_VERSION)
        return CML_ERROR_USER_BADVERSION;
    /* Ignoring minor version */
    CHECKERR(CML_SerialsReadUINT8(bytes, &bpos, &sign));
    /* Reading main type */
    CHECKERR(CML_SerialsReadUINT8(bytes, &bpos, &sign));

    CML_Bool named;
    CML_Type typed;

    if (sign == sign_hash)
    {
        typed = CML_TYPE_HASH;
        named = CML_TRUE;
    }
    else if (sign == sign_arry)
    {
        typed = CML_TYPE_ARRAY;
        named = CML_FALSE;
    }
    else
        return CML_ERROR_USER_BADTYPE;

    CHECKERR(CML_NodeCreate(typed, result));
    CHECKERC(CML_SerialsReadUINT32(bytes, &bpos, &count),
             CML_NodeFree(*result));

    for (i = 0; i < count; i++)
        CHECKERC(process_element(bytes, &bpos, *result, named),
                 CML_NodeFree(*result));

    return CML_ERROR_SUCCESS;
}

static CML_Error CML_FromFile(char * filename, CML_Bytes ** result)
{
    CHECKPTR(filename);
    CHECKPTR(result);

    FILE * file = fopen(filename, "rb");
    if (!file)
        return CML_ERROR_USER_CANTOPENFILE;

    if (fseek(file, 0, SEEK_END))
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    int32_t fsize = ftell(file);
    if (fsize < 0)
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    (*result)->size = fsize;

    if (fseek(file, 0, SEEK_SET))
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    (*result)->data = malloc(fsize + 1);
    if (!(*result)->data)
    {
        fclose(file);
        return CML_ERROR_USER_BADALLOC;
    }

    if (fread((*result)->data, 1, fsize, file) != (uint32_t)fsize)
    {
        fclose(file);
        return CML_ERROR_USER_CANTREADFILE;
    }

    fclose(file);

    return CML_ERROR_SUCCESS;
}

CML_Error CML_ThawFile(char * filename, CML_Node ** result)
{
    CHECKPTR(filename);
    CHECKPTR(result);

    CML_Bytes * bytes;
    bytes = calloc(sizeof(CML_Bytes), 1);
    if (!bytes)
        return CML_ERROR_USER_BADALLOC;

    CHECKERC(CML_FromFile(filename, &bytes),
             free(bytes->data);
             free(bytes));
    CHECKERC(CML_ThawBytes(bytes, result),
             free(bytes->data);
             free(bytes));

    free(bytes->data);
    free(bytes);

    return CML_ERROR_SUCCESS;
}


CML_Error CML_ThawData(char * data, uint32_t size, CML_Node ** result)
{
    CML_Bytes bytes;
    bytes.data = (uint8_t *)data;
    bytes.size = size;

    CHECKERR(CML_ThawBytes(&bytes, result));

    return CML_ERROR_SUCCESS;
}
