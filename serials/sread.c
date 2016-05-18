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

#include "../serials/sread.h"

CML_Error CML_SerialsReadINT8(CML_Bytes * bytes, uint32_t * bpos, int8_t * result)
{
    if (*bpos + sizeof(int8_t) > bytes->size)
        return CML_ERROR_USER_BADDATA;

    *result = bytes->data[*bpos];
    *bpos += 1;

    /* Need to reverse signed char */
    /* It's a perl thing           */
    *result ^= 1 << 7;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_SerialsReadUINT8(CML_Bytes * bytes, uint32_t * bpos, uint8_t * result)
{
    if (*bpos + sizeof(uint8_t) > bytes->size)
        return CML_ERROR_USER_BADDATA;

    *result = bytes->data[*bpos];
    *bpos += 1;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_SerialsReadINT32(CML_Bytes * bytes, uint32_t * bpos, int32_t * result)
{
    if (*bpos + sizeof(int32_t) > bytes->size)
        return CML_ERROR_USER_BADDATA;

    union
    {
        int32_t res;
        uint8_t bytes[sizeof(int32_t)];
    } conv;

    uint32_t i;
    for (i = 0; i < sizeof(int32_t); i++, *bpos += 1)
        conv.bytes[3 - i] = bytes->data[*bpos];

    *result = conv.res;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_SerialsReadUINT32(CML_Bytes * bytes, uint32_t * bpos, uint32_t * result)
{
    if (*bpos + sizeof(uint32_t) > bytes->size)
        return CML_ERROR_USER_BADDATA;

    union
    {
        uint32_t res;
        uint8_t bytes[sizeof(uint32_t)];
    } conv;

    uint32_t i;
    for (i = 0; i < sizeof(uint32_t); i++, *bpos += 1)
        conv.bytes[3 - i] = bytes->data[*bpos];

    *result = conv.res;

    return CML_ERROR_SUCCESS;
}


CML_Error CML_SerialsReadDATA(CML_Bytes * bytes, uint32_t * bpos, uint8_t * result, uint32_t length)
{
    if (*bpos + length > bytes->size)
        return CML_ERROR_USER_BADDATA;

    uint32_t i;
    for (i = 0; i < length; i++, *bpos += 1)
        result[i] = bytes->data[*bpos];
    result[i] = '\0';

    return CML_ERROR_SUCCESS;
}
