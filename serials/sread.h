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

#ifndef SREAD_H
#define SREAD_H

#include <stdint.h>

#include "../defines/types.h"

CML_Error CML_SerialsReadINT8  (CML_Bytes * bytes, uint32_t * bpos,  int8_t  * result);
CML_Error CML_SerialsReadUINT8 (CML_Bytes * bytes, uint32_t * bpos, uint8_t  * result);
CML_Error CML_SerialsReadINT32 (CML_Bytes * bytes, uint32_t * bpos,  int32_t * result);
CML_Error CML_SerialsReadUINT32(CML_Bytes * bytes, uint32_t * bpos, uint32_t * result);


#endif // SREAD_H
