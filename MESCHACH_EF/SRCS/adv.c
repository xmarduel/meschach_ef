
/** \file adv.c
 *
 * Contains the advection terms memory and errors routines handling
 *
 */


#include "MESCHACH/INCLUDES/meminfo.h"

#include "MESCHACH_EF/INCLUDES/adv.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int mem_attach_list7(void)
{
   return mem_attach_list(MY_LIST7, NB_STRUCTS7, my_names7, my_frees7, my_tnums7);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int err_attach_list7(void)
{
   return err_list_attach(MY_LIST7, NB_ERRS7, my_err_mesg7 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int warn_attach_list7(void)
{
   return warn_list_attach(MY_LIST7, NB_ERRS7, my_warn_mesg7 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int err_warn_attach_lists7(void)
{
   err_attach_list7();
   warn_attach_list7();

   return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int mem_stat_reg7(void **var, int type)
{
   return mem_stat_reg_list(var,type, MY_LIST7);
}

int mem_stat_free7(int mark)
{
   return mem_stat_free_list(mark, MY_LIST7);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

