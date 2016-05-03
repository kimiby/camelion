#ifndef FIND_H
#define FIND_H

#include "../defines/types.h"

#define CML_FIND_DELIMETER ('.')

/* E */ CML_Error CML_NodeFindUndef  (char * path, CML_Node * result);
/* E */ CML_Error CML_NodeFindHash   (char * path, CML_Node * result);
/* E */ CML_Error CML_NodeFindArray  (char * path, CML_Node * result);
/* E */ CML_Error CML_NodeFindString (char * path, CML_Node * result);
/* E */ CML_Error CML_NodeFindInteger(char * path, CML_Node * result);

#endif // FIND_H
