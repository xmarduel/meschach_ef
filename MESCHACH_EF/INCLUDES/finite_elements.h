#ifndef MEF_FINITE_ELEMENTS_H
#define MEF_FINITE_ELEMENTS_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file finite_elements.h
 *
 * Contains the finite elements memory and errors routines handling
 *
 * ------------------------------------------------------------------------ */

#define MY_LIST2     2

#define TYPE_ELT_1D    0
#define TYPE_ELT_2D    1
#define TYPE_ELT_3D    2

#define NB_STRUCTS2    3

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"


/**********************************************************************/
static char *my_names2[NB_STRUCTS2] = { "ELT_1D" ,
                                        "ELT_2D" ,
                                        "ELT_3D" };

static int (*my_frees2[NB_STRUCTS2])(void*) = { (int(*)(void*)) elt1D_free ,
                                                (int(*)(void*)) elt2D_free ,
                                                (int(*)(void*)) elt3D_free };

static MEM_ARRAY my_tnums2[NB_STRUCTS2]; /* initialised to zeros */


/* ------------------------------------------------------------------- */

#define  NB_ERRS2         2
#define  NB_WARNS2        1

static  char *my_err_mesg2[NB_ERRS2] =
{
   "unknown fe error",                /* 0 */
   "finite element not implemented"   /* 1 */
};

static char *my_warn_mesg2[NB_WARNS2] =
{
   "unknown warning"    	     /* 0 */
};

#define	E_EF_UNKNOWN	  0
#define	E_EF_NOTIMPL	  1

#define W_EF_UNKNOWN      0

/***********************************************************************/

/* static memory routines handling */
int mem_stat_reg2 (void **var, int type);
int mem_stat_free2(int mark);
#define MEM_STAT_REG2(var,type) mem_stat_reg_list((void **)&(var),type,MY_LIST2) /**< */

/* dynamic memory routine handling */
int mem_attach_list2(void);

/* error and warnings routine handling */
int err_attach_list2(void);
int warn_attach_list2(void);
int err_warn_attach_lists2(void);

/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	 error2(err_num, fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,MY_LIST2)
/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define warning2(warn_num, fn_name) ev_warn(__FILE__,warn_num,__LINE__,fn_name,MY_LIST2)

#ifdef __cplusplus
}
#endif

#endif

/*! \fn mem_attach_list2          ( void )
*
* Set "on" the statistics about the memory used for all the ELT_xD structures
*/

/*! \fn err_attach_list2          ( void )
*
* Set "on" the setjmp-longjmp error handling
*/

/*! \fn warn_attach_list2          ( void )
*
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn err_warn_attach_lists2     ( void )
*
* Set "on" the setjmp-longjmp error handling and \n
* Set "on" the setjmp-longjmp warning handling
*/


/*! \fn mem_stat_reg2(void **var, int type)
*
* \param var  : adress of a pointer to a ELT_nD structure
* \param type : TYPE_ELT_1D, TYPE_ELT_2D or TYPE_ELT_3D
*
* Register the static variables in a workspace
*
* usage :\n
*
* mem_stat_mark(1);
*
* static ELT_1D *MyEltP1 = Elt1D_get("P1");\n
* mem_stat_reg2(&MyEltP1, TYPE_ELT_1D); // or MEM_STAT_REG2(MyEltP1, TYPE_ELT_1D);\n
*
* mem_stat_free2(1);
*
*/

/*! \fn mem_stat_free2(int mark)
*
* \param mark  : the index of the "workspace" where the static variables are registered
*
* Free the static variables created in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"
*
* static ELT_1D *MyEltP1 = Elt1D_get("P1");\n
* mem_stat_reg2(&MyEltP1, TYPE_ELT_1D); // or MEM_STAT_REG2(MyEltP1, TYPE_ELT_1D);\n
*
* mem_stat_free2(1);
*
*/

