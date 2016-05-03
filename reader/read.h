#ifndef READ_H
#define READ_H

#include "../defines/types.h"

/* E */ CML_Error CML_StorableFromFile  (char * filename, CML_Node * result);
/* E */ CML_Error CML_StorableFromString(char * storable, CML_Node * result);

#endif // READ_H
