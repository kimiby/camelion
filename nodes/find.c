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
#include <stdlib.h>
#include <stdint.h>

#include "../nodes/find.h"
#include "../defines/tools.h"

static char * path_curr(char * path)
{
    char   * delpos = strchr(path, CML_FIND_DELIMETER);
    uint32_t intpos;
    intpos = delpos ? strlen(path) : (uint32_t)(delpos - path);
    return path + intpos;
}

static char * path_next(char * path)
{
    char * delpos = strchr(path, CML_FIND_DELIMETER);
    return delpos ? delpos + 1 : NULL;
}

CML_Error CML_NodeFindIndex(CML_Node * node, char * name, uint32_t * index)
{
    CHECKPTR(node );
    CHECKSTR(name );
    CHECKPTR(index);
    CHECKJAR(node );

    uint32_t i;
    for (i = 0; i < node->ncount; i++)
    if (!strcmp(node->nodes[i]->name, name))
    {
        *index = i;
        return CML_ERROR_SUCCESS;
    }

    return CML_ERROR_USER_BADSTRING;
}

CML_Error CML_NodeFind(CML_Node * node, char * path, CML_Node ** result, CML_Type type)
{
    CHECKPTR(node  );
    CHECKTYP(type  );
    CHECKPTR(result);

    char * next = path_next(path);
    char * curr = path_curr(path);
    uint32_t index;

    CHECKERR(CML_NodeFindIndex(node, curr, &index));

    if (next) /* go deeper */
        CHECKERR(CML_NodeFind(node->nodes[index], next, result, type))
    else /* stay here */
    {
        if (node->nodes[index]->type == type)
            *result = node->nodes[index];
        else
            return CML_ERROR_USER_BADTYPE;
    }


    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeFindUndef(CML_Node * node, char * path, CML_Node ** result)
{
    return CML_NodeFind(node, path, result, CML_TYPE_UNDEF);
}

CML_Error CML_NodeFindHash(CML_Node * node, char * path, CML_Node ** result)
{
    return CML_NodeFind(node, path, result, CML_TYPE_HASH);
}

CML_Error CML_NodeFindArray(CML_Node * node, char * path, CML_Node ** result)
{
    return CML_NodeFind(node, path, result, CML_TYPE_ARRAY);
}

CML_Error CML_NodeFindString(CML_Node * node, char * path, CML_Node ** result)
{
    return CML_NodeFind(node, path, result, CML_TYPE_STRING);
}

CML_Error CML_NodeFindInteger(CML_Node * node, char * path, CML_Node ** result)
{
    return CML_NodeFind(node, path, result, CML_TYPE_INTEGER);
}
