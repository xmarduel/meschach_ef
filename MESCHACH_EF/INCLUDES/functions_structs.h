#ifndef MEF_FUNCTIONS_STRUCTS__H
#define MEF_FUNCTIONS_STRUCTS__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file functions_structs.h
 *
 * Contains the structures describing a "C" or a "Python" function in 1-D, 2-D, 3-D
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/machine.h"
#include "MESCHACH/INCLUDES/matrix.h"


#define MY_LIST5   5

#define TYPE_FUN_1D   0
#define TYPE_FUN_2D   1
#define TYPE_FUN_3D   2

#define NB_STRUCTS5   3

typedef Real (*FUNC_1D)(Real);
typedef Real (*FUNC_2D)(Real,Real);
typedef Real (*FUNC_3D)(Real,Real,Real);
typedef Real (*FUNC_4D)(Real,Real,Real,Real);

typedef Real (*FUNC_1D_PLUS_VOID)(Real,                void*);
typedef Real (*FUNC_2D_PLUS_VOID)(Real,Real,           void*);
typedef Real (*FUNC_3D_PLUS_VOID)(Real,Real,Real,      void*);
typedef Real (*FUNC_4D_PLUS_VOID)(Real,Real,Real,Real, void*);


typedef enum
{
   FUN_UNDEFINED       = -1,  /**< flag indicates that the function is not yet set */

   FUN_C_STATIONNARY   =  0,  /**< function depends only on space variables */
   FUN_C_TRANSIENT     =  1,  /**< function depends on t  + space variables */
   FUN_PY_STATIONNARY  =  2,  /**< function depends only on space variables */
   FUN_PY_TRANSIENT    =  3,  /**< function depends on t  + space variables */
   FUN_LUA_STATIONNARY =  4,  /**< function depends only on space variables */
   FUN_LUA_TRANSIENT   =  5   /**< function depends on t  + space variables */
} FUN_TYPE;


typedef enum
{
   FUNCTION_RHSe      = 0,
	FUNCTION_BCe       = 1,
	FUNCTION_SOLe      = 2,
	FUNCTION_ICe       = 3,
   FUNCTION_ADVe      = 4,

} FUN_CLASS;


typedef struct FUNC_1D_NAMEMAP_ {

	char name[64];
	FUNC_1D func;

} FUNC_1D_NAMEMAP;

typedef struct FUNC_2D_NAMEMAP_ {

	char name[64];
	FUNC_2D func;

} FUNC_2D_NAMEMAP;

typedef struct FUNC_3D_NAMEMAP_ {

	char name[64];
	FUNC_3D func;

} FUNC_3D_NAMEMAP;

typedef struct FUNC_4D_NAMEMAP_ {

	char name[64];
	FUNC_4D func;

} FUNC_4D_NAMEMAP;


FUNC_1D function1D_by_name(const char *funcname, FUN_CLASS function_class);
FUNC_2D function2D_by_name(const char *funcname, FUN_CLASS function_class);
FUNC_3D function3D_by_name(const char *funcname, FUN_CLASS function_class);
FUNC_4D function4D_by_name(const char *funcname, FUN_CLASS function_class);

int function1Dindex_by_name(const char *funcname, FUN_CLASS function_class);
int function2Dindex_by_name(const char *funcname, FUN_CLASS function_class);
int function3Dindex_by_name(const char *funcname, FUN_CLASS function_class);
int function4Dindex_by_name(const char *funcname, FUN_CLASS function_class);

Real FunctionForEvaluatingLuaFunction1D(Real x, void *L);
Real FunctionForEvaluatingLuaFunction2D(Real x, Real y, void *L);
Real FunctionForEvaluatingLuaFunction3D(Real x, Real y, Real z, void *L);
Real FunctionForEvaluatingLuaFunction4D(Real x, Real y, Real z, Real t, void *L);

void * make_lua_interpreter(const char* def, const char *fun_type);

#include "functions_structs_1D.h"
#include "functions_structs_2D.h"
#include "functions_structs_3D.h"


/*****************************************************************************/
static char *my_names5[NB_STRUCTS5] = { "FUN_1D" ,
                                        "FUN_2D" ,
                                        "FUN_3D" };

static int (*my_frees5[NB_STRUCTS5])(void*) = { (int(*)(void*))Fun1D_free ,
                                                (int(*)(void*))Fun2D_free ,
                                                (int(*)(void*))Fun3D_free };


static MEM_ARRAY my_tnums5[NB_STRUCTS5]; /* initialised to zeros */

/* ------------------------------------------------------------------- */

#define  NB_ERRS5                  4
#define  NB_WARNS5                 1

static  char *my_err_mesg5[NB_ERRS5] =
{
   "fun type is not set (FUN_UNDEFINED)",              /* 0 */
   "fun_struct does not contain a transient func",     /* 1 */
   "fun_struct does not contain a stationnary func",   /* 2 */
   "fun_struct has a undefined type"                   /* 3 */
};

static char *my_warn_mesg5[NB_WARNS5] =
{
   "unknown fun_struc warning"            /* 0 */
};


#define	 E_FUN_UNKNOWN	           0
#define	 E_FUN_NOTATRANSIENTFUNC   1
#define  E_FUN_NOTASTATIONNARYFUNC 2
#define  E_FUN_WRONGTYPE           3

#define  W_FUN_UNKNOWN             0

/******************************************************************************/

int mem_attach_list5(void);
int err_attach_list5(void);
int warn_attach_list5(void);
int err_warn_attach_lists5(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error5(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,MY_LIST5)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning5(warn_num,fn_name) ev_warn(__FILE__,warn_num,__LINE__,fn_name,MY_LIST5)

#ifdef __cplusplus
}
#endif

#endif


/*! \enum FUN_TYPE
* \brief List the different types functions
*/

