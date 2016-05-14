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
#include <string.h>

#include "../reader/write.h"
#include "../defines/tools.h"

#define CML_PERL_PADDING (4)

CML_Error CML_StorableToFile(CML_Node * node, char * filename)
{
    char * buffer;
    CHECKERR(CML_StorableToString(node, &buffer));

    FILE * file = fopen(filename, "wb");
    if (!file)
    {
        free(buffer);
        return CML_ERROR_USER_CANTOPENFILE;
    }

    if (fwrite(buffer, strlen(buffer), 1, file))
    {
        free(buffer);
        return CML_ERROR_USER_CANTWRITFILE;
    }

    free(buffer);
    return CML_ERROR_SUCCESS;
}

static CML_Error string_append_pdd(char ** string, uint32_t level)
{
    char   * oldptr = *string;
    uint32_t oldpos = (*string) ? strlen(*string) : 0;
    *string = realloc(*string, strlen(*string) + level * CML_PERL_PADDING + 1);
    if (!(*string))
    {
        *string = oldptr;
        return CML_ERROR_USER_BADALLOC;
    }

    uint32_t i;
    for (i = 0; i < level * CML_PERL_PADDING; i++)
        *string[oldpos + i] = ' ';

    return CML_ERROR_SUCCESS;
}

static CML_Error string_append_int(char ** string, uint32_t val)
{
    char   * oldptr = *string;
    uint32_t oldpos = (*string) ? strlen(*string) : 0;
    *string = realloc(*string, strlen(*string) + strlen("-4294967296") + 1);
    if (!(*string))
    {
        *string = oldptr;
        return CML_ERROR_USER_BADALLOC;
    }

    sprintf(*string + oldpos, "%d", val);

    return CML_ERROR_SUCCESS;
}

static CML_Error string_append_str(char ** string, char * str)
{
    char   * oldptr = *string;
    uint32_t oldpos = (*string) ? strlen(*string) : 0;
    *string = realloc(*string, strlen(*string) + strlen(str) + 1);
    if (!(*string))
    {
        *string = oldptr;
        return CML_ERROR_USER_BADALLOC;
    }

    sprintf(*string + oldpos, "%s", str);

    return CML_ERROR_SUCCESS;
}

static CML_Error node_print(CML_Node * root, uint32_t level, char ** output)
{
    uint32_t i;

    CHECKERR(string_append_pdd(output, level));

    if (root->name)
    {
        CHECKERR(string_append_str(output, root->name));
        CHECKERR(string_append_str(output, " => "));
    }

    switch (root->type)
    {
    case CML_TYPE_UNDEF :
        CHECKERR(string_append_str(output, "undef,\n"));
        break;
    case CML_TYPE_INTEGER :
        CHECKERR(string_append_int(output, root->data.integer));
        CHECKERR(string_append_str(output, ",\n"));
        break;
    case CML_TYPE_STRING :
        CHECKERR(string_append_str(output, root->data.string));
        CHECKERR(string_append_str(output, ",\n"));
        break;
    case CML_TYPE_ARRAY :
        CHECKERR(string_append_str(output, "[\n"));
        for (i = 0; i < root->ncount; i++)
            CHECKERR(node_print(root->nodes[i], level + 1, output));
        CHECKERR(string_append_pdd(output, level));
        CHECKERR(string_append_str(output, "],\n"));
        break;
    case CML_TYPE_HASH :
        CHECKERR(string_append_str(output, "{\n"));
        for (i = 0; i < root->ncount; i++)
            CHECKERR(node_print(root->nodes[i], level + 1, output));
        CHECKERR(string_append_pdd(output, level));
        CHECKERR(string_append_str(output, "},\n"));
        break;
    default:
        return CML_ERROR_USER_BADTYPE;
    }

    return CML_ERROR_SUCCESS;
}

CML_Error CML_StorableToString(CML_Node * node, char ** storable)
{
    CHECKPTR(node);
    CHECKPTR(storable);

    *storable = NULL;
    CHECKERC(node_print(node, 0, storable),
             free(*storable); *storable = NULL);

    return CML_ERROR_SUCCESS;
}
