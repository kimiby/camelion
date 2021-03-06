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
/* This file was automatically generated by generate_h.sh, 06/02/16 16:57:51 */
#ifndef LIBCAMELION_H
#define LIBCAMELION_H

#include <stdint.h>

/* Exported from ./defines/xmacro.h */
#define CML_X_ERROR_CODES \
    /* No error                                                             */ \
    CML(SUCCESS             , (0x00),        "Successfull                   ") \
    /* System general errors                                                */ \
    CML(SYSTEM_GENERALERROR , (0x01),        "Generic error                 ") \
    CML(SYSTEM_MISUSEOFBLTNS, (0x02),        "Misuse of shell built-ins     ") \
    /* User general errors                                                  */ \
    CML(USER_BADALLOC       , (0x10 + 0x01), "Memory allocation failed      ") \
    CML(USER_BADPOINTER     , (0x10 + 0x02), "Bad pointer provided          ") \
    CML(USER_BADTYPE        , (0x10 + 0x03), "Bad type provided             ") \
    CML(USER_BADVALUE       , (0x10 + 0x04), "Bad value provided            ") \
    CML(USER_BADSTRING      , (0x10 + 0x05), "Bad string provided           ") \
    /* User file errors                                                     */ \
    CML(USER_CANTOPENFILE   , (0x20 + 0x01), "Can't open file               ") \
    CML(USER_CANTREADFILE   , (0x20 + 0x02), "Can't read file               ") \
    CML(USER_CANTSEEKFILE   , (0x20 + 0x03), "Can't seek file               ") \
    CML(USER_CANTWRITFILE   , (0x20 + 0x04), "Can't write to file           ") \
    /* User storable parsing errors                                         */ \
    CML(USER_BADSTART       , (0x30 + 0x01), "No storable signature found   ") \
    CML(USER_BADNAME        , (0x30 + 0x02), "Bad entity name               ") \
    CML(USER_BADDATA        , (0x30 + 0x03), "Bad nfreezed data             ") \
    CML(USER_BADVERSION     , (0x30 + 0x04), "Bad version                   ") \
    /* System errors                                                        */ \
    CML(SYSTEM_CANNOTEXECUTE, (0x7E),        "Command invoked cannot execute") \
    CML(SYSTEM_NOTFOUND     , (0x7F),        "Command not found             ") \
    CML(SYSTEM_INVALIDEXIT  , (0x80),        "Invalid argument to exit      ") \
    /* System signals                                                       */ \
    CML(SYSTEM_SIGHUP       , (0x80 + 0x01), "Hangup                        ") \
    CML(SYSTEM_SIGINT       , (0x80 + 0x02), "Interrupt                     ") \
    CML(SYSTEM_SIGQUIT      , (0x80 + 0x03), "Quit                          ") \
    CML(SYSTEM_SIGILL       , (0x80 + 0x04), "Illegal Instruction           ") \
    CML(SYSTEM_SIGTRAP      , (0x80 + 0x05), "Trace/Breakpoint Trap         ") \
    CML(SYSTEM_SIGABRT      , (0x80 + 0x06), "Abort                         ") \
    CML(SYSTEM_SIGEMT       , (0x80 + 0x07), "Emulation Trap                ") \
    CML(SYSTEM_SIGFPE       , (0x80 + 0x08), "Arithmetic Exception          ") \
    CML(SYSTEM_SIGKILL      , (0x80 + 0x09), "Killed                        ") \
    CML(SYSTEM_SIGBUS       , (0x80 + 0x0A), "Bus Error                     ") \
    CML(SYSTEM_SIGSEGV      , (0x80 + 0x0B), "Segmentation Fault            ") \
    CML(SYSTEM_SIGSYS       , (0x80 + 0x0C), "Bad System Call               ") \
    CML(SYSTEM_SIGPIPE      , (0x80 + 0x0D), "Broken Pipe                   ") \
    CML(SYSTEM_SIGALRM      , (0x80 + 0x0E), "Alarm Clock                   ") \
    CML(SYSTEM_SIGTERM      , (0x80 + 0x0F), "Terminated                    ") \
    CML(SYSTEM_SIGUSR1      , (0x80 + 0x10), "User Signal 1                 ") \
    CML(SYSTEM_SIGUSR2      , (0x80 + 0x11), "User Signal 2                 ") \
    CML(SYSTEM_SIGCHLD      , (0x80 + 0x12), "Child Status                  ") \
    CML(SYSTEM_SIGPWR       , (0x80 + 0x13), "Power Fail/Restart            ") \
    CML(SYSTEM_SIGWINCH     , (0x80 + 0x14), "Window Size Change            ") \
    CML(SYSTEM_SIGURG       , (0x80 + 0x15), "Urgent Socket Condition       ") \
    CML(SYSTEM_SIGPOLL      , (0x80 + 0x16), "Socket I/O Possible           ") \
    CML(SYSTEM_SIGSTOP      , (0x80 + 0x17), "Stopped (signal)              ") \
    CML(SYSTEM_SIGTSTP      , (0x80 + 0x18), "Stopped (user)                ") \
    CML(SYSTEM_SIGCONT      , (0x80 + 0x19), "Continued                     ") \
    CML(SYSTEM_SIGTTIN      , (0x80 + 0x1A), "Stopped (tty input)           ") \
    CML(SYSTEM_SIGTTOU      , (0x80 + 0x1B), "Stopped (tty output)          ") \
    CML(SYSTEM_SIGVTALRM    , (0x80 + 0x1C), "Virtual Timer Expired         ") \
    CML(SYSTEM_SIGPROF      , (0x80 + 0x1D), "Profiling Timer Expired       ") \
    CML(SYSTEM_SIGXCPU      , (0x80 + 0x1E), "CPU time limit exceeded       ") \
    CML(SYSTEM_SIGXFSZ      , (0x80 + 0x1F), "File size limit exceeded      ") \
    CML(SYSTEM_SIGWAITING   , (0x80 + 0x20), "All LWPs blocked              ") \
    CML(SYSTEM_SIGLWP       , (0x80 + 0x21), "VI Interrupt for T. Library   ") \
    CML(SYSTEM_SIGAIO       , (0x80 + 0x22), "Asynchronous I/O              ") \
                                                                               \
    CML(SYSTEM_OUTOFRANGE   , (0xFF),        "Exit status unknown           ") \
    /* Maximum is 255, as this index will be sent to exit() function           */


/* Exported from ./defines/types.h */
# define CML(NAME, CODE, DESC) CML_ERROR_ ## NAME = CODE,
typedef enum
{
    CML_X_ERROR_CODES
    CML_ERROR_LAST
} CML_Error;
#undef CML

typedef enum
{
    CML_TYPE_UNDEF,
    CML_TYPE_INTEGER,
    CML_TYPE_STRING,
    CML_TYPE_ARRAY,
    CML_TYPE_HASH,

    CML_TYPE_FIRST = CML_TYPE_UNDEF,
    CML_TYPE_LAST  = CML_TYPE_HASH
} CML_Type;

typedef struct CML_Node
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

typedef struct
{
    uint8_t * data;
    uint32_t  size;
} CML_Bytes;

typedef enum
{
    CML_FALSE,
    CML_TRUE
} CML_Bool;


/* Exported from ./helpers/info.h */
char * CML_HelpError(CML_Error code);

/* Exported from ./helpers/string.h */
CML_Error dec2int(char * line, int32_t * value);

/* Exported from ./defines/tools.h */
#define CHECKPTR(X)                         \
    if (!X) return CML_ERROR_USER_BADPOINTER
#define CHECKSTR(X)                         \
    if (!X) return CML_ERROR_USER_BADSTRING
#define CHECKJAR(X)                         \
    if (((X)->type != CML_TYPE_HASH) &&     \
        ((X)->type != CML_TYPE_ARRAY))      \
        return CML_ERROR_USER_BADTYPE;
#define CHECKTYP(X)                         \
    if (((X) < CML_TYPE_FIRST) ||           \
        ((X) > CML_TYPE_LAST ))             \
        return CML_ERROR_USER_BADTYPE
#define CHECKERR(X)                         \
{                                           \
    CML_Error _res = X;                     \
    if (_res != CML_ERROR_SUCCESS)          \
        return _res;                        \
}
#define CHECKERC(X, ACTIONS)                \
{                                           \
    CML_Error _res = X;                     \
    if (_res != CML_ERROR_SUCCESS)          \
    {                                       \
        ACTIONS;                            \
        return _res;                        \
    }                                       \
}
#define CHECKMEM(X)                         \
    if (!X) return CML_ERROR_USER_BADALLOC


/* Exported from ./defines/consts.h */

/* Exported from ./memory/alloc.h */
#define CML_Malloc( PTR, SIZE) CML_Malloc ((void **)PTR, SIZE)
#define CML_Calloc( PTR, SIZE) CML_Calloc ((void **)PTR, SIZE)
#define CML_Realloc(PTR, SIZE) CML_Realloc((void **)PTR, SIZE)
#define CML_Free(   PTR      ) CML_Free   ((void **)PTR)

CML_Error (CML_Malloc) (void ** ptr, uint32_t size);
CML_Error (CML_Calloc) (void ** ptr, uint32_t size);
CML_Error (CML_Realloc)(void ** ptr, uint32_t size);
CML_Error (CML_Free)   (void ** ptr);

/* Exported from ./serials/sread.h */

/* Exported from ./serials/swrite.h */

/* Exported from ./translate/thaw.h */
CML_Error CML_ThawData (char * data, uint32_t size, CML_Node ** result);
CML_Error CML_ThawBytes(CML_Bytes * bytes,          CML_Node ** result);
CML_Error CML_ThawFile (char * filename,            CML_Node ** result);

/* Exported from ./translate/nfreeze.h */
CML_Error CML_NfreezeNode    (CML_Node * node, CML_Bytes ** result);
CML_Error CML_NfreezeStorable(char * storable, CML_Bytes ** result);

/* Exported from ./nodes/find.h */
#define CML_FIND_DELIMETER ('.')

CML_Error CML_NodeFind         (CML_Node * node, char * path, CML_Node ** result, CML_Type type);
CML_Error CML_NodeFindUndef    (CML_Node * node, char * path, CML_Node ** result);
CML_Error CML_NodeFindHash     (CML_Node * node, char * path, CML_Node ** result);
CML_Error CML_NodeFindArray    (CML_Node * node, char * path, CML_Node ** result);
CML_Error CML_NodeFindString   (CML_Node * node, char * path, CML_Node ** result);
CML_Error CML_NodeFindInteger  (CML_Node * node, char * path, CML_Node ** result);
CML_Error CML_NodeFindContainer(CML_Node * node, char * path, CML_Node ** result);
CML_Error CML_NodeFindIndex    (CML_Node * node, char * name, uint32_t *  index );

/* Exported from ./nodes/basis.h */
CML_Error CML_NodeCreate    (CML_Type    type, CML_Node ** result);
CML_Error CML_NodeSetName   (CML_Node *  node, char     *  value);
CML_Error CML_NodeSetString (CML_Node *  node, char     *  value);
CML_Error CML_NodeSetInteger(CML_Node *  node, int32_t     value);
CML_Error CML_NodeFree      (CML_Node ** node);
CML_Error CML_NodeCopy      (CML_Node *  node, CML_Node **  dest, CML_Bool recursive);
CML_Error CML_NodeRemove    (CML_Node *  node, uint32_t    index);
CML_Error CML_NodeRemoveAll (CML_Node *  node);
CML_Error CML_NodeAppend    (CML_Node *  node, CML_Node *  child);
CML_Error CML_NodeInsert    (CML_Node *  node, CML_Node *  child, uint32_t pos);

/* Exported from ./reader/read.h */
CML_Error CML_FromFile          (char * filename, char ** result);
CML_Error CML_StorableFromFile  (char * filename, CML_Node ** result);
CML_Error CML_StorableFromString(char * storable, CML_Node ** result);

/* Exported from ./reader/write.h */
CML_Error CML_DataFree(CML_Bytes ** bytes);
CML_Error CML_DataToFile(uint8_t * data, uint32_t length, char * filename);
CML_Error CML_StorableToFile  (CML_Node * node, char *  filename);
CML_Error CML_StorableToString(CML_Node * node, char ** storable);

#endif //LIBCAMELION_H
