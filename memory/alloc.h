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

#ifndef ALLOC_H
#define ALLOC_H

#include <stdint.h>

#include "../defines/types.h"

/* E */ CML_Error CML_Malloc (void ** ptr, uint32_t size);
/* E */ CML_Error CML_Calloc (void ** ptr, uint32_t size);
/* E */ CML_Error CML_Realloc(void ** ptr, uint32_t size);
/* E */ CML_Error CML_Free   (void ** ptr);

#endif // ALLOC_H
