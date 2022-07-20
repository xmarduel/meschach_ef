#ifndef MEF_BOUNDARY_CONDITIONS_H
#define MEF_BOUNDARY_CONDITIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file boundary_conditions.h
 *
 * Contains the boundary conditions memory and errors routines handling
 *
 * -------------------------------------------------------------------- */

#define MY_LIST4   4

#define TYPE_BC_1D    0
#define TYPE_BC_2D    1
#define TYPE_BC_3D    2

#define NB_STRUCTS4   3



#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/boundary_conditions_1D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_2D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_3D.h"

/*****************************************************************************/
static char *my_names4[NB_STRUCTS4] = { "BC_1D"  ,
                                        "BC_2D"  ,
                                        "BC_3D"  };

static int (*my_frees4[NB_STRUCTS4])(void*) = { (int(*)(void*))Bc1D_free ,
                                                (int(*)(void*))Bc2D_free ,
                                                (int(*)(void*))Bc3D_free };


static MEM_ARRAY my_tnums4[NB_STRUCTS4]; /* initialised to zeros */
/*****************************************************************************/

#define  NB_ERRS4                   10
#define  NB_WARNS4                   7

static  char *my_err_mesg4[NB_ERRS4] =
{
   "unknown bc error",                                /* 0 */
   "boundary condition incompatible with geometry",   /* 1 */
   "too many boundary conditions in lecture",         /* 2 */
   "index in boundary conditions too big",            /* 3 */
   "axe in boundary conditions wrong",                /* 4 */
   "boundary condition not set on reference index",   /* 5 */
   "dirichlet function not set on a ref'ed somm",     /* 6 */
   "neumann function not set on a ref'ed somm",       /* 7 */
   "cauchy function not set on a ref'ed somm",        /* 8 */
   "robin function not set on a ref'ed somm"          /* 9 */
};

static char *my_warn_mesg4[NB_WARNS4] =
{
   "unknown bc warning"	, 			      /* 0 */
   "more boundary functions than refs on somms",      /* 1 */
   "mask of bc set not initialized on a reference",   /* 2 */
   "mask of bc set on a reference but no function set on this mask",  /* 3 */
   "second function for dirichlet BC is never used",  /* 4 */
   "second function for neumann BC is never used",    /* 5 */
   "second function for cauchy BC is never used"      /* 6 */
};

#define	 E_BC_UNKNOWN	            0
#define	 E_BC_INCOMPATIBLE          1
#define  E_BC_NBBOUNDARYCONDITIONS  2
#define  E_BC_WRONGIDXNUMBER        3
#define  E_BC_WRONGAXENUMBER        4
#define  E_BC_WRONGBCTYPE           5
#define  E_BC_DIRICHLETFUNNOTSET    6
#define  E_BC_NEUMANNFUNNOTSET      7
#define  E_BC_CAUCHYFUNNOTSET       8
#define  E_BC_ROBINFUNNOTSET        9

#define  W_BC_UNKNOWN               0
#define  W_BC_TOOMANYFUNCTIONS      1
#define  W_BC_TYPENOTSET            2
#define  W_BC_MASKSETBUTNOFUNCTION  3
#define  W_BC_SECONDFUNCDIRICHLET   4
#define  W_BC_SECONDFUNCNEUMANN     5
#define  W_BC_SECONDFUNCCAUCHY      6

/******************************************************************************/

/* static memory routines handling */
int mem_stat_reg4 (void **var, int type);
int mem_stat_free4(int mark);
#define MEM_STAT_REG4(var,type) mem_stat_reg_list((void **)&(var),type,MY_LIST4) /**<  */

/* dynamic memory routine handling */
int mem_attach_list4(void);

/* error and warnings routine handling */
int err_attach_list4(void);
int warn_attach_list4(void);
int err_warn_attach_lists4(void);

/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	 error4(err_num, fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,MY_LIST4)
/* warn (W_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define  warning4(warn_num, fn_name) ev_warn(__FILE__,warn_num,__LINE__,fn_name,MY_LIST4)

#ifdef __cplusplus
}
#endif

#endif

/*! \fn mem_attach_list4          ( void )
*
* Set "on" the statistics about the memory used for all the BC_xD structures
*/

/*! \fn err_attach_list4          ( void )
*
* Set "on" the setjmp-longjmp error handling
*/

/*! \fn warn_attach_list4          ( void )
*
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn err_warn_attach_lists4     ( void )
*
* Set "on" the setjmp-longjmp error handling and \n
* Set "on" the setjmp-longjmp warning handling
*/


/*! \fn mem_stat_reg4(void **var, int type)
*
* \param var  : adress of a pointer to a BC_nD structure
* \param type : TYPE_BC_1D, TYPE_BC_2D or TYPE_BC_3D
*
* Register the static variables in a workspace
*
* usage :\n
*
* mem_stat_mark(1);
*
* static BC_1D *MyBc = Bc1D_get();\n
* mem_stat_reg4(&MyBc, TYPE_BC_1D); // or MEM_STAT_REG4(MyBc, TYPE_BC_1D);\n
*
* mem_stat_free4(1);
*
*/

/*! \fn mem_stat_free4(int mark)
*
* \param mark  : the index of the "workspace" where the static variables are registered
*
* Free the static variables created in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"
*
* static BC_1D *MyBc = Bc1D_get();\n
* mem_stat_reg4(&MyBc, TYPE_BC_1D); // or MEM_STAT_REG4(MyBc, TYPE_BC_1D);\n
*
* mem_stat_free4(1);
*
*/

