#ifndef MEF_ADV_H
#define MEF_ADV_H

#ifdef __cplusplus
extern "C"
{
#endif


/** \file adv.h
 *
 * Contains the advection terms memory and errors routines handling
 *
 * -------------------------------------------------------------------- */

#define MY_LIST7   7

#define TYPE_ADV_1D    0
#define TYPE_ADV_2D    1
#define TYPE_ADV_3D    2

#define NB_STRUCTS7   3


#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/adv_1D.h"
#include "MESCHACH_EF/INCLUDES/adv_2D.h"
#include "MESCHACH_EF/INCLUDES/adv_3D.h"

/*****************************************************************************/
static char *my_names7[NB_STRUCTS7] = { "ADV_1D"  ,
                                        "ADV_2D"  ,
                                        "ADV_3D"  };

static int (*my_frees7[NB_STRUCTS7])(void*) = { (int(*)(void*))Adv1D_free ,
                                                (int(*)(void*))Adv2D_free ,
                                                (int(*)(void*))Adv3D_free };


static MEM_ARRAY my_tnums7[NB_STRUCTS7]; /* initialised to zeros */
/*****************************************************************************/

#define  NB_ERRS7                    5
#define  NB_WARNS7                   1

static  char *my_err_mesg7[NB_ERRS7] =
{
   "unknown adv error",                /* 0 */
   "adv incompatible with geometry" ,  /* 1 */
   "too many adv  in lecture",         /* 2 */
   "adv index too big",                /* 3 */
   "adv axe too big"                   /* 4 */
};

static char *my_warn_mesg7[NB_WARNS7] =
{
   "unknown rhs warning"               /* 0 */
};

#define  E_ADV_UNKNOWN	            0
#define  E_ADV_INCOMPATIBLE         1
#define  E_ADV_TOOMANYADV           2
#define  E_ADV_WRONGIDXNUMBER       3
#define  E_ADV_WRONGAXENUMBER       4

#define  W_ADV_UNKNOWN              0

/******************************************************************************/

/* static memory routines handling */
int mem_stat_reg7 (void **var, int type);
int mem_stat_free7(int mark);
#define MEM_STAT_REG7(var,type) mem_stat_reg_list((void **)&(var),type,MY_LIST7) /**< similar to mem_stat_reg7 */

/* dynamic memory routine handling */
int mem_attach_list7(void);

/* error and warnings routine handling */
int err_attach_list7(void);
int warn_attach_list7(void);
int err_warn_attach_lists7(void);

/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	 error7(err_num, fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,MY_LIST7)
/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	 warning7(warn_num, fn_name) ev_warn(__FILE__,warn_num,__LINE__,fn_name,MY_LIST7)

#ifdef __cplusplus
}
#endif

#endif

/*! \fn mem_attach_list7          ( void )
*
* Set "on" the statistics about the memory used for all the ADV_xD structures
*/

/*! \fn err_attach_list7          ( void )
*
* Set "on" the setjmp-longjmp error handling
*/

/*! \fn warn_attach_list7          ( void )
*
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn err_warn_attach_lists7     ( void )
*
* Set "on" the setjmp-longjmp error handling and \n
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn mem_stat_reg7(void **var, int type)
*
* \param var  : adress of a pointer to a ADV_nD structure
* \param type : TYPE_ADV_1D, TYPE_ADV_2D or TYPE_ADV_3D
*
* Register the static variables in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"
*
* static ADV_2D *MyStaticAdv = Adv2D_get();\n
* mem_stat_reg7(&MyStaticAdv, TYPE_ADV_2D); // or MEM_STAT_REG7(MyStaticAdv, TYPE_ADV_2D);\n
*
* mem_stat_free7(1);
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
* static ADV_2D *MyStaticAdv = Adv2D_get();\n
* mem_stat_reg7(&MyStaticAdv, TYPE_ADV_2D); // or MEM_STAT_REG7(MyStaticAdv, TYPE_ADV_2D);\n
*
* mem_stat_free7(1);
*
*/

