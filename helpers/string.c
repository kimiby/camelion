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
