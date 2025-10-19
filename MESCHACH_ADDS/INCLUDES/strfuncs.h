
#ifndef MESCHACH_STRFUNC__H
#define MESCHACH_STRFUNC__H

#ifdef __cplusplus
extern "C"
{
#endif

char *str_build(const char* format, ...);
char *str_replace(const char* input, const char* substring, const char *replace);
void str_replace2(char* input, const char* substring, const char *replace);

#ifdef __cplusplus
}
#endif
      
#endif
