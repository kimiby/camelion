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

#ifndef FIND_H
#define FIND_H

#include "../defines/types.h"

/* E+ */
#define CML_FIND_DELIMETER ('.')
/* E- */

/* E */ CML_Error CML_NodeFind         (CML_Node * node, char * path, CML_Node ** result, CML_Type type);
/* E */ CML_Error CML_NodeFindUndef    (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindHash     (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindArray    (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindString   (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindInteger  (CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindContainer(CML_Node * node, char * path, CML_Node ** result);
/* E */ CML_Error CML_NodeFindIndex    (CML_Node * node, char * name, uint32_t *  index );

#endif // FIND_H
