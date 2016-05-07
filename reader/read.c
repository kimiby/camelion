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
#include <ctype.h>

#include "../reader/read.h"
#include "../defines/types.h"
#include "../nodes/basis.h"
#include "../defines/tools.h"
#include "../helpers/string.h"

static CML_Error CML_NodeParse(CML_Node * root, char * storable, uint32_t * caret);

static CML_Error CML_FromFile(char * filename, char ** result)
{
    CHECKPTR(filename);
    CHECKPTR(result);

    FILE * file = fopen(filename, "rb");
    if (!file)
        return CML_ERROR_USER_CANTOPENFILE;

    if (!fseek(file, 0, SEEK_END))
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    int32_t fsize = ftell(file);
    if (fsize < 0)
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    if (!fseek(file, 0, SEEK_SET))
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    *result = malloc(fsize + 1);
    if (!(*result))
    {
        fclose(file);
        return CML_ERROR_USER_BADALLOC;
    }

    if (fread(*result, fsize, 1, file) != (uint32_t)fsize)
    {
        fclose(file);
        return CML_ERROR_USER_CANTREADFILE;
    }

    fclose(file);

    return CML_ERROR_SUCCESS;
}

static void comments_remove(char * string)
{
    uint32_t i;
    CML_Bool flag = CML_FALSE;
    for (i = 0; i < strlen(string); i++)
    {
        if (flag)
        {
            if (string[i] == '\n')
                flag = CML_FALSE;
            else
                string[i] = ' ';
        }
        else
        {
            if (string[i] == '#')
            {
                string[i] = ' ';
                flag = CML_TRUE;
            }
        }
    }
}

static void space_skip(char * string, uint32_t * caret)
{
    while (isspace(string[*caret])) *caret += 1;
}

static CML_Error CML_NodeReadValue(CML_Node * root, char * storable, uint32_t * caret)
{
    space_skip(storable, caret);

    if (storable[*caret] == '{')
    {
        root->type = CML_TYPE_HASH;
        *caret += 1;
        CHECKERR(CML_NodeParse(root, storable, caret));
    }
    else if (storable[*caret] == '[')
    {
        root->type = CML_TYPE_ARRAY;
        *caret += 1;
        CHECKERR(CML_NodeParse(root, storable, caret));
    }
    else
    {
        ///@fixme implement stepped realloc
        char val[0x1000];
        uint32_t valpos = 0;

            char stopper = ',';
        CML_Bool escaped = CML_FALSE;

        if (storable[*caret] == '\'') { stopper = '\''; *caret += 1; } else
        if (storable[*caret] == '\"') { stopper = '\"'; *caret += 1; }

        while ((storable[*caret] != stopper) &&
               (!escaped))
        {
            if (!escaped)
            {
                if (storable[*caret] != '\\')
                    val[valpos] = storable[*caret];
                else
                    escaped = CML_TRUE;
            }
            else
            {
                escaped = CML_FALSE;
                val[valpos] = storable[*caret];
            }
            valpos++;

            *caret += 1;
        }
        if (stopper != ',')
            *caret += 1;

        *caret += 1;
        val[valpos++] = '\0';

        int32_t value;
        if (dec2int(val, &value) == CML_ERROR_SUCCESS)
        {
            root->type = CML_TYPE_INTEGER;
            CHECKERR(CML_NodeSetInteger(root, value));
        }
        else
        {
            root->type = CML_TYPE_STRING;
            CHECKERR(CML_NodeSetString(root, val));
        }
    }

    return CML_ERROR_SUCCESS;
}

static CML_Error CML_NodeReadName(CML_Node * root, char * storable, uint32_t * caret)
{
    space_skip(storable, caret);

    if ((storable[*caret] != '{') &&
        (storable[*caret] != '['))
    {
        char name[0x100];
        uint32_t namecaret = 0;

        while (!isspace(storable[*caret]))
        {
            name[namecaret++] = storable[*caret];
            *caret += 1;
            if (namecaret > 255)
                return CML_ERROR_USER_BADNAME;
        }
        name[namecaret++] = '\0';

        root->name = malloc(namecaret);
        if (!root->name)
            return CML_ERROR_USER_BADALLOC;

        memcpy(root->name, name, namecaret);

        space_skip(storable, caret);

        if ((strlen(storable + *caret) >    2) &&
            (       storable [ *caret] == '=') &&
            (       storable [ *caret] == '>'))
            *caret += 2;
    }

    return CML_ERROR_SUCCESS;
}

static CML_Error CML_NodeParse(CML_Node * root, char * storable, uint32_t * caret)
{
    space_skip(storable, caret);

    while ((storable[*caret] != ']') &&
           (storable[*caret] != '}'))
    {
        CML_Node * child;
        CHECKERR(CML_NodeCreate(CML_TYPE_UNDEF, &child));

        if (root->type != CML_TYPE_ARRAY)
            CHECKERC(CML_NodeReadName(child, storable, caret),
                     CML_NodeFree(child));
        CHECKERC(CML_NodeReadValue(child, storable, caret),
                 CML_NodeFree(child));

        CHECKERR(CML_NodeAppend(root, child));

        space_skip(storable, caret);
    }

    /* Skip '}' or ']' */
    *caret += 1;

    space_skip(storable, caret);
    if (storable[*caret] == ',')
        *caret += 1;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_StorableFromString(char * storable, CML_Node ** result)
{
    CHECKPTR(storable);
    CHECKPTR(result);

    char * data = malloc(strlen(storable) + 1);
    if (!data)
        return CML_ERROR_USER_BADALLOC;

    comments_remove(data);

    uint32_t caret = 0;
    space_skip(data, &caret);

    if (data[caret++] != '{')
        return CML_ERROR_USER_BADSTART;

    ///@fixme currentlyit's unsafe. Need func/macro to check borders for
    ///       every character accessing.

    CHECKERR(CML_NodeCreate(CML_TYPE_HASH, result));
    CHECKERR(CML_NodeParse (*result, data, &caret));

    return CML_ERROR_SUCCESS;
}

CML_Error CML_StorableFromFile(char * filename, CML_Node ** result)
{
    CHECKPTR(filename);
    CHECKPTR(result);

    char * buffer;
    CHECKERR(CML_FromFile(filename, &buffer));

    CHECKERC(CML_StorableFromString(buffer, result), free(buffer));

    free(buffer);
    return CML_ERROR_SUCCESS;
}
