#ifndef MEF_JSON_CONFIG_TO_PARAMETERS_H
#define MEF_JSON_CONFIG_TO_PARAMETERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file all_params_json_config.h 
 *
 * fill the PARAMS structure from a json config
 *
 * -------------------------------------------------------------------- */

#include <jansson.h>
	
#include "MESCHACH_EF/INCLUDES/all_params.h"

PARAMS* json_config_to_params(const json_t *config);
PARAMS *json_config_append_to_params(PARAMS* p, const json_t* config);
	
	
#define MY_LIST8       8
	
#define  NB_ERRS8      3
#define  NB_WARNS8     1
	
static  char *my_err_mesg8[NB_ERRS8] =
{
	"unknown json error",                  /* 0 */
	"json missing object error",           /* 1 */
	"json object without value error",     /* 2 */
};
	
static char *my_warn_mesg8[NB_WARNS8] = 
{
	"unknown json warning"	, 			      /* 0 */
};
	
#define	E_JSON_UNKNOWN           0
#define	E_JSON_MISSING_OBJECT    1
#define  E_JSON_NULL_OBJECT_VALUE 2
	
#define  W_JSON_UNKNOWN           0

	
/******************************************************************************/
	

/* dynamic memory routine handling */
int mem_attach_list8(void);
	
/* error and warnings routine handling */
int err_attach_list8(void);
int warn_attach_list8(void);
int err_warn_attach_lists8(void);
	
/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	error8(err_num, fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,MY_LIST8)
/* warn (W_TYPE, "myfunc") raises warn type E_TYPE for function my_func() */
#define  warning8(warn_num, fn_name) ev_warn(__FILE__,warn_num,__LINE__,fn_name,MY_LIST8)
	

	

#ifdef __cplusplus
}
#endif


#endif

