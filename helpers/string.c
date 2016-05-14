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

#include <limits.h>
#include <string.h>

#include "../helpers/string.h"

CML_Error dec2int(char * line, int32_t * value)
{
    /* -2.147.483.648
        2.147.483.647 */

    if (!line || line[0] == 0)  return CML_ERROR_USER_BADVALUE;
    if (!value) return CML_ERROR_USER_BADVALUE;

    if ((strlen(line) == 1) && (*line == '-'))
        return CML_ERROR_USER_BADVALUE;

    int32_t minus = 1;

    if ((strlen(line) > 1) && (*line == '-'))
    {
        minus = -1;
        line++;
    }

    if (strlen(line) > 10)
        return CML_ERROR_USER_BADVALUE;

    int64_t value64;
    value64 = 0;

    while (*line)
    {
        value64 *= 10;

        if ((*line >= '0') && (*line <= '9'))
            value64 += *line - '0';
        else
            return CML_ERROR_USER_BADVALUE;

        if (value64 > INT_MAX)
            return CML_ERROR_USER_BADVALUE;

        line++;
    }

    value64 *= minus;

    if (value64 < INT_MIN)
        return CML_ERROR_USER_BADVALUE;

    *value = (int32_t)value64;

    return CML_ERROR_SUCCESS;
}
