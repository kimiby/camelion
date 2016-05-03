#include "../helpers/info.h"

char * CML_HelpError(CML_Error code)
{
#   define CML(NAME, CODE, DESC) \
    if (code == CODE) return DESC; else

    CML_X_ERROR_CODES
        return "Unknown code                  ";

#   undef CML
}
