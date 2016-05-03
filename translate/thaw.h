#ifndef THAW_H
#define THAW_H

#include "../defines/types.h"

/* E */ CML_Error CML_ThawNode    (CML_Bytes * bytes, CML_Node * result);
/* E */ CML_Error CML_ThawStorable(CML_Bytes * bytes, char    ** result);

#endif // THAW_H
