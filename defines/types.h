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

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include "../defines/xmacro.h"

/* E */ typedef enum
{
    CML_TYPE_UNDEF,
    CML_TYPE_INTEGER,
    CML_TYPE_STRING,
    CML_TYPE_ARRAY,
    CML_TYPE_HASH,

    CML_TYPE_FIRST = CML_TYPE_UNDEF,
    CML_TYPE_LAST  = CML_TYPE_HASH
} CML_Type;

/* E */ typedef struct CML_Node
{
    char   * name;
    CML_Type type;

    union
    {
        char  * string;
        int32_t integer;
    } data;

    struct CML_Node ** nodes;
           uint32_t    ncount;

} CML_Node;

/* E */ typedef struct
{
    uint8_t * data;
    uint32_t  size;
} CML_Bytes;

typedef enum
{
    CML_FALSE,
    CML_TRUE
} CML_Bool;

/* E+ */
# define CML(NAME, CODE, DESC) CML_ERROR_ ## NAME = CODE,
typedef enum
{
    CML_X_ERROR_CODES
    CML_ERROR_LAST
} CML_Error;
#undef CML
/* E- */

#endif // TYPES_H
