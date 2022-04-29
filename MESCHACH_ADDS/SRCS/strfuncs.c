
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "MESCHACH_ADDS/INCLUDES/strfuncs.h"

char *str_build(const char* format, ...)
{
	char *res = (char*)malloc(256);
	
	va_list ap;
	va_start(ap, format);
   
	vsnprintf(res, 256, format, ap);
   
	va_end(ap);
   
   return res;
}