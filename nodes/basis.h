#ifndef BASIS_H
#define BASIS_H

#include "../defines/types.h"

/* E */ CML_Error CML_NodeCreate    (CML_Type   type, CML_Node ** result);
/* E */ CML_Error CML_NodeSetName   (CML_Node * node, char     *  value);
/* E */ CML_Error CML_NodeSetString (CML_Node * node, char     *  value);
/* E */ CML_Error CML_NodeSetInteger(CML_Node * node, int32_t     value);
/* E */ CML_Error CML_NodeFree      (CML_Node * node);
/* E */ CML_Error CML_NodeCopy      (CML_Node * node, CML_Node **  dest, CML_Bool recursive);
/* E */ CML_Error CML_NodeRemove    (CML_Node * node, uint32_t    index);
/* E */ CML_Error CML_NodeAppend    (CML_Node * node, CML_Node *  child);
/* E */ CML_Error CML_NodeInsert    (CML_Node * node, CML_Node *  child, uint32_t pos);

#endif // BASIS_H
