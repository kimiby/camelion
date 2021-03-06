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

#ifndef WRITE_H
#define WRITE_H

#include "../defines/types.h"

/* E */ CML_Error CML_DataFree(CML_Bytes ** bytes);
/* E */ CML_Error CML_DataToFile(uint8_t * data, uint32_t length, char * filename);
/* E */ CML_Error CML_StorableToFile  (CML_Node * node, char *  filename);
/* E */ CML_Error CML_StorableToString(CML_Node * node, char ** storable);

#endif // WRITE_H
