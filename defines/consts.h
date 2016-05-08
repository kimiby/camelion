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

const uint8 signature[] = {0x05,
                           0x09,
                           0x03};

#endif // CONSTS_H
