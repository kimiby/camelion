#include <string.h>
#include <stdlib.h>
#include "../nodes/basis.h"
#include "../defines/tools.h"

CML_Error CML_NodeCreate(CML_Type type, CML_Node ** result)
{
    CHECKPTR(result);

    if (type > CML_TYPE_LAST)
        return CML_ERROR_USER_BADTYPE;

    CML_Node * node = malloc(sizeof(CML_Node *));
    if (!node)
        return CML_ERROR_USER_BADALLOC;

    memset(node, 0, sizeof(CML_Node));
    node->type         = type;
    *result = node;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeSetName(CML_Node * node, char * value)
{
    CHECKPTR(node);

    if (node->name)
        free(node->name);

    if (value)
    {
        node->name = malloc(strlen(value) + 1);
        if (!node->name)
            return CML_ERROR_USER_BADALLOC;
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
        free(node->data.string);

    if (value)
    {
        node->data.string = malloc(strlen(value) + 1);
        if (!node->data.string)
            return CML_ERROR_USER_BADALLOC;

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

CML_Error CML_NodeFree(CML_Node * node)
{
    CHECKPTR(node);

    if (node->name)
        free(node->name);
    if ((node->type == CML_TYPE_STRING) &&
        (node->data.string))
        free(node->data.string);

    if (node->ncount)
    {
        uint32_t i;
        for (i = 0; i < node->ncount; i++)
            CHECKERR(CML_NodeFree(node->nodes[i]));
        /* There might be memory loss if 1st of 3
         * elements will return non-success. 2nd
         * & 3rd elements will not be freed    */
    }

    free(node);

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeCopy(CML_Node * node, CML_Node ** dest)
{
    ///@todo
}

CML_Error CML_NodeRemove(CML_Node * node, uint32_t index)
{
    CHECKPTR(node);

    if ((node->type != CML_TYPE_ARRAY) &&
        (node->type != CML_TYPE_HASH))
        return CML_ERROR_USER_BADTYPE;

    if (node->ncount <= index)
        return CML_ERROR_USER_BADVALUE;

    CHECKERR(CML_NodeFree(node->nodes[index]));

    uint32_t i;
    for (i = index; i < (node->ncount - 1); i++)
        node->nodes[i] = node->nodes[i + 1];

    return CML_ERROR_SUCCESS;
}

CML_Error CML_NodeAppend(CML_Node * node, CML_Node * child)
{
    CHECKPTR(node);
    CHECKPTR(child);

    if ((node->type != CML_TYPE_ARRAY) ||
        (node->type != CML_TYPE_HASH))
        return CML_ERROR_USER_BADTYPE;

    CML_Node ** oldptr = node->nodes;
    node->nodes = realloc(node->nodes,
                          sizeof(CML_Node *) * (node->ncount + 1));
    if (!node->nodes)
    {
        node->nodes = oldptr;
        return CML_ERROR_USER_BADALLOC;
    }

    node->nodes[node->ncount++] = child;

    return CML_ERROR_SUCCESS;
}


CML_Error CML_NodeInsert(CML_Node * node, CML_Node * child, uint32_t pos)
{
    ///@todo
}
