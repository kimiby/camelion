#ifndef FIND_H
#define FIND_H

#include "../defines/types.h"

#define CML_FIND_DELIMETER ('.')

/* E */ CML_Error CML_NodeFind       (CML_Node * node, char * path, CML_Node ** result, CML_Type type);
/* E */ CML_Error CML_NodeFindUndef  (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindHash   (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindArray  (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindString (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindInteger(CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindIndex  (CML_Node * node, char * name, uint32_t *  index );

#endif // FIND_H
