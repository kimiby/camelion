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

#include "../nodes/basis.h"
#include "../defines/tools.h"
#include "../memory/alloc.h"

CML_Error CML_NodeCreate(CML_Type type, CML_Node ** result)
{
    CHECKPTR(result);
    CHECKTYP(type);

    CML_Node * node;
    CHECKERR(CML_Malloc(&node, sizeof(CML_Node)));

    memset(node, 0, sizeof(CML_Node));
    node->type         = type;
    *result = node;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeSetName(CML_Node * node, char * value)
{
    CHECKPTR(node);

    if ((value) && (strlen(value) > 255))
        return CML_ERROR_USER_BADNAME;

    if (node->name)
        CHECKERR(CML_Free(&node->name));

    if (value)
    {
        CHECKERR(CML_Malloc(&node->name, strlen(value) + 1));
        strcpy(node->name, value);
    }

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeSetString(CML_Node * node, char * value)
{
    CHECKPTR(node);

    if (node->type != CML_TYPE_STRING)
        return CML_ERROR_USER_BADTYPE;

    if (node->data.string)
        CHECKERR(CML_Free(&node->data.string));

    if (value)
    {
        CHECKERR(CML_Malloc(&node->data.string, strlen(value) + 1));
        strcpy(node->data.string, value);
    }

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeSetInteger(CML_Node * node, int32_t value)
{
    CHECKPTR(node);

    if (node->type != CML_TYPE_INTEGER)
        return CML_ERROR_USER_BADTYPE;

    node->data.integer = value;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeFree(CML_Node ** node)
{
    CHECKPTR(node);

    if ((*node)->name)
        CHECKERR(CML_Free(&(*node)->name));
    if (((*node)->type == CML_TYPE_STRING) &&
        ((*node)->data.string))
        CHECKERR(CML_Free(&(*node)->data.string));

    if ((*node)->ncount)
    {
        uint32_t i;
        for (i = 0; i < (*node)->ncount; i++)
            CHECKERR(CML_NodeFree(&(*node)->nodes[i]));
        /* There might be memory loss if 1st of 3
         * elements will return non-success. 2nd
         * & 3rd elements will not be freed    */
    }

    CHECKERR(CML_Free(&(*node)->nodes));
    CHECKERR(CML_Free(&(*node)));

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeCopy(CML_Node * node, CML_Node ** dest, CML_Bool recursive)
{
    CHECKPTR(node);
    CHECKPTR(dest);

    CHECKERR(CML_NodeCreate(node->type, dest));
    CHECKERR(CML_NodeSetName(*dest, node->name));

    switch (node->type)
    {
    case CML_TYPE_UNDEF  : break;
    case CML_TYPE_INTEGER:
        CHECKERR(CML_NodeSetInteger(*dest, node->data.integer));
        break;
    case CML_TYPE_STRING :
        CHECKERR(CML_NodeSetString(*dest, node->data.string));
        break;
    case CML_TYPE_ARRAY  :
    case CML_TYPE_HASH   :
        if (recursive)
        {
            uint32_t i;
            for (i = 0; i < node->ncount; i++)
            {
                CML_Node * child;
                CHECKERR(CML_NodeCopy(node->nodes[i], &child, CML_TRUE));
                CHECKERR(CML_NodeAppend(*dest, child));
            }
        }
    }

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeRemove(CML_Node * node, uint32_t index)
{
    CHECKPTR(node);
    CHECKJAR(node);

    if (node->ncount <= index)
        return CML_ERROR_USER_BADVALUE;

    CHECKERR(CML_NodeFree(&node->nodes[index]));

    uint32_t i;
    for (i = index; i < (node->ncount - 1); i++)
        node->nodes[i] = node->nodes[i + 1];

    node->ncount--;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeAppend(CML_Node * node, CML_Node * child)
{
    CHECKPTR(node);
    CHECKPTR(child);
    CHECKJAR(node);

    CHECKERR(CML_Realloc(&node->nodes, (node->ncount + 1) * sizeof(CML_Node *)));

    node->nodes[node->ncount++] = child;

    return CML_ERROR_SUCCESS;
}


CML_Error CML_NodeInsert(CML_Node * node, CML_Node * child, uint32_t pos)
{
    CHECKPTR(node);
    CHECKPTR(child);
    CHECKJAR(node);

    if (node->ncount < pos)
        return CML_ERROR_USER_BADVALUE;

    CHECKERR(CML_Realloc(&node->nodes, (node->ncount + 1) * sizeof(CML_Node *)));

    uint32_t i;
    for (i = node->ncount; i > pos; i--)
        node->nodes[i] = node->nodes[i - 1];

    node->ncount++;
    node->nodes[pos] = child;

    return CML_ERROR_SUCCESS;
}


CML_Error CML_NodeRemoveAll(CML_Node * node)
{
    CHECKPTR(node);
    CHECKJAR(node);

    while (node->ncount)
        CHECKERR(CML_NodeRemove(node, 0));

    return CML_ERROR_SUCCESS;
}
