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

#ifndef THAW_H
#define THAW_H

#include "../defines/types.h"

/* E */ CML_Error CML_ThawData (char * data, uint32_t size, CML_Node ** result);
/* E */ CML_Error CML_ThawBytes(CML_Bytes * bytes,          CML_Node ** result);
/* E */ CML_Error CML_ThawFile (char * filename,            CML_Node ** result);

#endif // THAW_H
