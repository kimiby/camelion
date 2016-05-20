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

#include <errno.h>

enum CML_Symbols
{
    CML_SMB_CBO =  '{', // Curly Bracket Opened
    CML_SMB_CBC =  '}', // Curly Bracket Closed
    CML_SMB_SBO =  '[', // Square Bracket Opened
    CML_SMB_SBC =  ']', // Square Bracket Closed
    CML_SMB_CMM =  ',', // Comma
    CML_SMB_EQU =  '=', // Quality
    CML_SMB_GRT =  '>', // Greater Than Sign
    CML_SMB_HSH =  '#', // Hash (Pound) Sign
    CML_SMB_SPC =  ' ', // Space Sign
    CML_SMB_NUL = '\0', // Null
    CML_SMB_BSL = '\\', // Backslash
    CML_SMB_DQU = '\"', // Double Quotes
    CML_SMB_SQU = '\'', // Single Quotes
    CML_SMB_NLN = '\n', // New Line

    CML_SMB_AR1 = CML_SMB_EQU, // Perl Arrow Part 1
    CML_SMB_AR2 = CML_SMB_GRT, // Perl Arrow Part 2

    CML_SMB_CM1 = CML_SMB_HSH, // Perl Comment Started
    CML_SMB_CM2 = CML_SMB_NLN, // Perl Comment Ended
    CML_SMB_CM3 = CML_SMB_SPC, // Perl Comment Filler

    CML_SMB_HS1 = CML_SMB_CBO, // Perl Hash Opened
    CML_SMB_HS2 = CML_SMB_CBC, // Perl Hash Closed

    CML_SMB_RR1 = CML_SMB_SBO, // Perl Array Opened
    CML_SMB_RR2 = CML_SMB_SBC, // Perl Array Closed

    CML_SMB_QT1 = CML_SMB_DQU, // Perl Quotes Type 1
    CML_SMB_QT2 = CML_SMB_SQU, // Perl Quotes Type 2

    CML_SMB_ENS = CML_SMB_NUL, // Perl End-Of-String
    CML_SMB_ENV = CML_SMB_CMM, // Perl End-Of-Value

    CML_SMB_ESC = CML_SMB_BSL  // Perl Escape Symbol
};

static CML_Error string_symbol(char ** string, char * result)
{
    if (!string) return CML_ERROR_USER_BADSTRING;

    *result = **string;
    *string += 1;

    return CML_ERROR_SUCCESS;
}

static CML_Error string_peek(char ** string, char * result)
{
    if (!string) return CML_ERROR_USER_BADSTRING;

    *result = **string;

    return CML_ERROR_SUCCESS;
}

static CML_Error string_skip(char ** string)
{
    for (;;)
    {
        char symbol;
        CHECKERR(string_symbol(string, &symbol));
        if (!isspace(symbol)) break;
    }

    *string -= 1;

    return CML_ERROR_SUCCESS;
}

static CML_Error string_arrow(char ** storable)
{
    char symbol;
    CHECKERR(string_symbol(storable, &symbol));
    if (symbol != CML_SMB_AR1) return CML_ERROR_USER_BADSTRING;
    CHECKERR(string_symbol(storable, &symbol));
    if (symbol != CML_SMB_AR2) return CML_ERROR_USER_BADSTRING;

    return CML_ERROR_SUCCESS;
}

static CML_Error CML_NodeParse(CML_Node * root, char ** storable);

static CML_Error CML_FromFile(char * filename, char ** result)
{
    CHECKPTR(filename);
    CHECKPTR(result);

    FILE * file = fopen(filename, "rb");
    if (!file)
        return CML_ERROR_USER_CANTOPENFILE;

    if (fseek(file, 0, SEEK_END))
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

    if (fseek(file, 0, SEEK_SET))
    {
        fclose(file);
        return CML_ERROR_USER_CANTSEEKFILE;
    }

    *result = calloc(fsize + 1, 1);
    if (!(*result))
    {
        fclose(file);
        return CML_ERROR_USER_BADALLOC;
    }

    if (fread(*result, 1, fsize, file) != (uint32_t)fsize)
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
            if (string[i] == CML_SMB_CM2)
                flag = CML_FALSE;
            else
                string[i] = CML_SMB_CM3;
        }
        else
        {
            if (string[i] == CML_SMB_CM1)
            {
                string[i] = CML_SMB_CM3;
                flag = CML_TRUE;
            }
        }
    }
}

static CML_Error CML_NodeReadValue(CML_Node * root, char ** storable)
{
    CHECKERR(string_skip(storable));

    char symbol;
    CHECKERR(string_symbol(storable, &symbol));

    if (symbol == CML_SMB_HS1)
    {
        root->type = CML_TYPE_HASH;
        CHECKERR(CML_NodeParse(root, storable));
    }
    else if (symbol == CML_SMB_RR1)
    {
        root->type = CML_TYPE_ARRAY;
        CHECKERR(CML_NodeParse(root, storable));
    }
    else
    {
        ///@fixme implement stepped realloc
        char val[0x1000];
        uint32_t valpos = 0;

            char stopper = CML_SMB_ENV;
        CML_Bool escaped = CML_FALSE;

        if (symbol == CML_SMB_QT2)
        {
            stopper = CML_SMB_QT2;
            CHECKERR(string_symbol(storable, &symbol));
        }
        else if (symbol == CML_SMB_QT1)
        {
            stopper = CML_SMB_QT1;
            CHECKERR(string_symbol(storable, &symbol));
        }

        while ((symbol != stopper) || (escaped))
        {
            if (!escaped)
            {
                if (symbol != CML_SMB_ESC)
                    val[valpos] = symbol;
                else
                {
                    escaped = CML_TRUE;
                    valpos--;
                }
            }
            else
            {
                escaped = CML_FALSE;
                val[valpos] = symbol;
            }
            valpos++;

            CHECKERR(string_symbol(storable, &symbol));
        }
        if (stopper != CML_SMB_ENV)
            CHECKERR(string_symbol(storable, &symbol));

        CHECKERR(string_symbol(storable, &symbol));
        val[valpos++] = CML_SMB_ENS;

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

static CML_Error string_realloc(char ** string, uint32_t * size, char symbol)
{
    char * oldptr = *string;
    *string = realloc(*string, *size + 1);
    if (!*string)
    {
        free(oldptr);
        return CML_ERROR_USER_BADALLOC;
    }

    (*string)[*size] = symbol;
    *size += 1;

    return CML_ERROR_SUCCESS;
}

static CML_Error CML_NodeReadName(CML_Node * root, char ** storable)
{
    CHECKERR(string_skip(storable));

    char symbol;
    CHECKERR(string_symbol(storable, &symbol));

    if ((symbol != CML_SMB_HS1) &&
        (symbol != CML_SMB_RR1))
    {
        uint32_t namecaret = 0;
        root->name = NULL;

        while (!isspace(symbol))
        {
            CHECKERR(string_realloc(&root->name, &namecaret, symbol));
            CHECKERR(string_symbol(storable, &symbol));
            if (namecaret > 255)
            {
                free(root->name);
                return CML_ERROR_USER_BADNAME;
            }
        }
        CHECKERR(string_realloc(&root->name, &namecaret, CML_SMB_ENS));
        CHECKERR(string_skip(storable));
        CHECKERR(string_arrow(storable));
    }

    return CML_ERROR_SUCCESS;
}

static CML_Error CML_NodeParse(CML_Node * root, char ** storable)
{
    CHECKERR(string_skip(storable));

    char symbol;
    CHECKERR(string_peek(storable, &symbol));

    while ((symbol != CML_SMB_RR2) &&
           (symbol != CML_SMB_HS2))
    {
        CML_Node * child;
        CHECKERR(CML_NodeCreate(CML_TYPE_UNDEF, &child));

        if (root->type != CML_TYPE_ARRAY)
        {
            CHECKERC(CML_NodeReadName(child, storable),
                     CML_NodeFree(child));
        }
        CHECKERR(string_skip(storable));

        CHECKERC(CML_NodeReadValue(child, storable),
                 CML_NodeFree(child));

        CHECKERR(CML_NodeAppend(root, child));

        CHECKERR(string_skip(storable));
        CHECKERR(string_peek(storable, &symbol));
    }

    /* Skip '}' or ']' */
    CHECKERR(string_symbol(storable, &symbol));

    CHECKERR(string_skip(storable));

    /* Skip last ',' */
    if ((**storable) && (**storable == CML_SMB_ENV))
        *storable += 1;

    return CML_ERROR_SUCCESS;
}

CML_Error CML_StorableFromString(char * storable, CML_Node ** result)
{
    CHECKPTR(storable);
    CHECKPTR(result);

    char * data = malloc(strlen(storable) + 1);
    if (!data)
        return CML_ERROR_USER_BADALLOC;

    char * olddata = data;
    strcpy(data, storable);

    comments_remove(data);

    CHECKERR(string_skip(&data));

    char symbol;
    CHECKERR(string_symbol(&data, &symbol));

    if (symbol != CML_SMB_HS1)
        return CML_ERROR_USER_BADSTART;

    CHECKERC(CML_NodeCreate(CML_TYPE_HASH, result),
             free(olddata));
    CHECKERC(CML_NodeParse (*result, &data),
             free(olddata));
    free(olddata);

    return CML_ERROR_SUCCESS;
}

CML_Error CML_StorableFromFile(char * filename, CML_Node ** result)
{
    CHECKPTR(filename);
    CHECKPTR(result);

    char * buffer = NULL;
    CHECKERR(CML_FromFile(filename, &buffer));

    if (!buffer)
        return CML_ERROR_USER_BADSTRING;

    CHECKERC(CML_StorableFromString(buffer, result), free(buffer));

    free(buffer);
    return CML_ERROR_SUCCESS;
}
