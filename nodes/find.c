#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "../nodes/find.h"
#include "../defines/tools.h"

static CML_Error path_curr(char * path, char ** result)
{
    CHECKPTR(path);
    CHECKPTR(result);

    char   * delpos = strchr(path, CML_FIND_DELIMETER);
    uint32_t intpos;
    intpos = delpos ? strlen(path) : delpos - path;
    *result = malloc(++intpos);
    CHECKMEM(*result);
    strcpy(*result, path + pos);

    return CML_ERROR_SUCCESS;
}

static CML_Error path_next(char * path, char ** result)
{
    char * delpos = strchr(path, CML_FIND_DELIMETER);
    return delpos ? delpos + 1 : NULL;
}

CML_Error CML_NodeFindUndef(char * path, CML_Node * result)
{
    ///@todo
}

CML_Error CML_NodeFindHash(char * path, CML_Node * result)
{
    ///@todo
}

CML_Error CML_NodeFindArray(char * path, CML_Node * result)
{
    ///@todo
}

CML_Error CML_NodeFindString(char * path, CML_Node * result)
{
    ///@todo
}

CML_Error CML_NodeFindInteger(char * path, CML_Node * result)
{
    ///@todo
}
