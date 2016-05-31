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

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../memory/alloc.h"
#include "../serials/swrite.h"
#include "../defines/tools.h"

CML_Error CML_SerialWriteUINT8(CML_Bytes * bytes, uint8_t value)
{
    CHECKERR(CML_Realloc((void **)&bytes->data, bytes->size + sizeof(uint8_t)));

    bytes->data[bytes->size++] = value;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_SerialWriteUINT32(CML_Bytes * bytes, uint32_t value)
{
    CHECKERR(CML_Realloc((void **)&bytes->data, bytes->size + sizeof(uint32_t)));

    bytes->data[bytes->size++] = (value >> 0x18) & 0xFF;
    bytes->data[bytes->size++] = (value >> 0x10) & 0xFF;
    bytes->data[bytes->size++] = (value >> 0x08) & 0xFF;
    bytes->data[bytes->size++] = (value >> 0x00) & 0xFF;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_SerialWriteINT32 (CML_Bytes * bytes, int32_t  value)
{    
    CHECKERR(CML_Realloc((void **)&bytes->data, bytes->size + sizeof(uint32_t)));

    bytes->data[bytes->size++] = (value >> 0x18) & 0xFF;
    bytes->data[bytes->size++] = (value >> 0x10) & 0xFF;
    bytes->data[bytes->size++] = (value >> 0x08) & 0xFF;
    bytes->data[bytes->size++] = (value >> 0x00) & 0xFF;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_SerialWriteString(CML_Bytes * bytes, char * value)
{
    uint32_t len = strlen(value);

    CHECKERR(CML_Realloc((void **)&bytes->data, bytes->size + len));

    memcpy(&bytes->data[bytes->size], value, len);

    bytes->size += len;

    return CML_ERROR_SUCCESS;
}
