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

#ifndef BASIS_H
#define BASIS_H

#include "../defines/types.h"

/* E */ CML_Error CML_NodeCreate    (CML_Type    type, CML_Node ** result);
/* E */ CML_Error CML_NodeSetName   (CML_Node *  node, char     *  value);
/* E */ CML_Error CML_NodeSetString (CML_Node *  node, char     *  value);
/* E */ CML_Error CML_NodeSetInteger(CML_Node *  node, int32_t     value);
/* E */ CML_Error CML_NodeFree      (CML_Node ** node);
/* E */ CML_Error CML_NodeCopy      (CML_Node *  node, CML_Node **  dest, CML_Bool recursive);
/* E */ CML_Error CML_NodeRemove    (CML_Node *  node, uint32_t    index);
/* E */ CML_Error CML_NodeAppend    (CML_Node *  node, CML_Node *  child);
/* E */ CML_Error CML_NodeInsert    (CML_Node *  node, CML_Node *  child, uint32_t pos);

#endif // BASIS_H
