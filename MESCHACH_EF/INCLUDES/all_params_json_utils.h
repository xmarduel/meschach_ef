/** \file all_params_json_utils.h
 *
 * misc functions to read a json config file containing comments
 *
 */

#ifndef MEF_JSON_UTILS_H
#define MEF_JSON_UTILS_H

#ifdef __cplusplus
extern "C"
{
#endif
	
#include <jansson.h>

json_t *json_load_xfile(const char *path);

/* shorcuts to retrieve values */
const char *json_object_get_string_value(const json_t *p, const char *objname);
int json_object_get_integer_value(const json_t *p, const char *objname);
double json_object_get_real_value(const json_t *p, const char *objname);

int json_object_get_boolean_value(const json_t *p, const char *objname);
	
int json_array_get_integer_value(const json_t *p, int idx);
double json_array_get_real_value(const json_t *p, int idx);
const char* json_array_get_string_value(const json_t *p, int idx);

int json_object_get_arrayitem_integer_value(const json_t *p, const char *objname, int idx);
double json_object_get_arrayitem_real_value(const json_t *p, const char *objname, int idx);
const char* json_object_get_arrayitem_string_value(const json_t *p, const char *objname, int idx);
	
/* validation */
int     json_check_data(const char* data, const char* schema, char* output);
	
	
#ifdef __cplusplus
}
#endif

#endif
