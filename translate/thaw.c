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

static CML_Error process_int8(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * root, CML_Bool hasname)
{
    ///@todo
}

static CML_Error process_int32(CML_Bytes * bytes, uint32_t * bpos,
                                CML_Node * root, CML_Bool hasname)
{
    ///@todo
}

static CML_Error process_string(CML_Bytes * bytes, uint32_t * bpos,
                                 CML_Node * root, CML_Bool hasname)
{
    ///@todo
}

static CML_Error process_data(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * root, CML_Bool hasname)
{
    ///@todo
}

static CML_Error process_undef(CML_Bytes * bytes, uint32_t * bpos,
                                CML_Node * root, CML_Bool hasname)
{
    ///@todo
}

static CML_Error process_array(CML_Bytes * bytes, uint32_t * bpos,
                                CML_Node * root, CML_Bool hasname)
{
    ///@todo
}

static CML_Error process_hash(CML_Bytes * bytes, uint32_t * bpos,
                               CML_Node * root, CML_Bool hasname)
{
    ///@todo
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

    CHECKERR(CML_NodeCreate(CML_TYPE_HASH, result));
    CHECKERC(CML_SerialsReadUINT32(bytes, &bpos, &count), ///@todo increase bpos in Read
             CML_NodeFree(*result));

    for (i = 0; i < count; i++)
        CHECKERC(process_element(bytes, &bpos, *result, CML_TRUE),
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

    if (!fseek(file, 0, SEEK_END))
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

    if (!fseek(file, 0, SEEK_SET))
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

    if (fread((*result)->data, fsize, 1, file) != (uint32_t)fsize)
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
    CHECKERR(CML_FromFile(filename, &bytes));
    CHECKERC(CML_ThawBytes(bytes, result),
             free(bytes->data);
             free(bytes));

    return CML_ERROR_SUCCESS;
}
