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

#ifndef CONSTS_H
#define CONSTS_H

#include <stdint.h>

#define CML_PERL_DATALONG (0x01)
#define CML_PERL_ARRAY    (0x02)
#define CML_PERL_HASH     (0x03)
#define CML_PERL_EXTENDED (0x04)
#define CML_PERL_UNDEF    (0x05)
#define CML_PERL_INT8     (0x08)
#define CML_PERL_INT32    (0x09)
#define CML_PERL_STRING   (0x0A)
#define CML_PERL_UTF8     (0x17)
#define CML_PERL_LUTF8    (0x18)

extern const uint8_t signature[2];
extern const uint8_t sign_hash;
extern const uint8_t sign_arry;

#endif // CONSTS_H
