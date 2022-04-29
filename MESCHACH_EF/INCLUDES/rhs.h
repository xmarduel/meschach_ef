#ifndef MEF_RHS_H
#define MEF_RHS_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file rhs.h
 *
 * Contains the rhs terms memory and errors routines handling
 *
 * -------------------------------------------------------------------- */

#define MY_LIST6   6

#define TYPE_RHS_1D    0
#define TYPE_RHS_2D    1
#define TYPE_RHS_3D    2

#define NB_STRUCTS6   3


#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/rhs_1D.h"
#include "MESCHACH_EF/INCLUDES/rhs_2D.h"
#include "MESCHACH_EF/INCLUDES/rhs_3D.h"

/*****************************************************************************/
static char *my_names6[NB_STRUCTS6] = { "RHS_1D"  ,
                                        "RHS_2D"  ,
                                        "RHS_3D"  };

static int (*my_frees6[NB_STRUCTS6])(void*) = { (int(*)(void*))Rhs1D_free ,
                                                (int(*)(void*))Rhs2D_free ,
                                                (int(*)(void*))Rhs3D_free };


static MEM_ARRAY my_tnums6[NB_STRUCTS6]; /* initialised to zeros */
/*****************************************************************************/

#define  NB_ERRS6                    5
#define  NB_WARNS6                   1

static  char *my_err_mesg6[NB_ERRS6] =
{
   "unknown rhs error",                /* 0 */
   "rhs incompatible with geometry" ,  /* 1 */
   "too many rhs  in lecture",         /* 2 */
   "rhs index too big",                /* 3 */
   "rhs axe wrong"                     /* 4 */
};

static char *my_warn_mesg6[NB_WARNS6] = 
{
   "unknown rhs warning"               /* 0 */
};

#define	 E_RHS_UNKNOWN	            0
#define	 E_RHS_INCOMPATIBLE         1
#define  E_RHS_TOOMANYRHS           2
#define  E_RHS_WRONGIDXNUMBER       3
#define  E_RHS_WRONGAXENUMBER       4

#define  W_RHS_UNKNOWN              0

/******************************************************************************/

/* static memory routines handling */
int mem_stat_reg6 (void **var, int type);
int mem_stat_free6(int mark);
#define MEM_STAT_REG6(var,type) mem_stat_reg_list((void **)&(var),type,MY_LIST6) /**<  */

/* dynamic memory routine handling */
int mem_attach_list6(void);

/* error and warnings routine handling */
int err_attach_list6(void);
int warn_attach_list6(void);
int err_warn_attach_lists6(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error6(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,MY_LIST6)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning6(warn_num,fn_name) ev_warn(__FILE__,warn_num,__LINE__,fn_name,MY_LIST6)

#ifdef __cplusplus
}
#endif

#endif

/*! \fn mem_attach_list6          ( void )
*
* Set "on" the statistics about the memory used for all the ADV_xD structures
*/

/*! \fn err_attach_list6          ( void )
*
* Set "on" the setjmp-longjmp error handling
*/

/*! \fn warn_attach_list6          ( void )
*
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn err_warn_attach_lists6     ( void )
*
* Set "on" the setjmp-longjmp error handling and \n
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn mem_stat_reg6(void **var, int type)
* 
* \param var  : adress of a pointer to a RHS_nD structure
* \param type : TYPE_RHS_1D, TYPE_RHS_2D or TYPE_RHS_3D
*
* Register the static variables in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"
*
* static RHS_2D *MyStaticRhs = Rhs2D_get();\n
* mem_stat_reg6(&MyStaticRhs, TYPE_RHS_2D); // or MEM_STAT_REG6(MyStaticRhs, TYPE_RHS_2D);\n
*
* mem_stat_free6(1);
*
*/

/*! \fn mem_stat_free7(int mark)
*
* \param mark  : the index of the "workspace" whre the static variables are registered
*
* Free the static variables created in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"\n
*
* static RHS_2D *MyStaticRhs = Rhs2D_get();\n
* mem_stat_reg7(&MyStaticRhs, TYPE_RHS_2D); // or MEM_STAT_REG7(MyStaticRhs, TYPE_RHS_2D);\n
*
* mem_stat_free6(1);
*
*/

