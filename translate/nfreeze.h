#ifndef NFREEZE_H
#define NFREEZE_H

#include "../defines/types.h"

/* E */ CML_Error CML_NfreezeNode    (CML_Node * node, CML_Bytes * result);
/* E */ CML_Error CML_NfreezeStorable(char * storable, CML_Bytes * result);

#endif // NFREEZE_H
