#include "vogle.h"

void
verrorhandler_(int (*f)(int en, char *m, int len))
{
        vdevice.verrorhandlerfunc = f;
}
