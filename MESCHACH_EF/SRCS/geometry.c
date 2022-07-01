
/** \file geometry.c
 *
 *  Contains the geometry memory and errors routines handling
 *
 */

#include "MESCHACH_EF/INCLUDES/geometry.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int mem_attach_list3(void)
{
   return mem_attach_list(MY_LIST3, NB_STRUCTS3, my_names3, my_frees3, my_tnums3);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int err_attach_list3(void)
{
   return err_list_attach(MY_LIST3, NB_ERRS3, my_err_mesg3);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int warn_attach_list3(void)
{
   return warn_list_attach(MY_LIST3, NB_WARNS3, my_warn_mesg3);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int err_warn_attach_lists3(void)
{
   err_attach_list3();
   warn_attach_list3();

   return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int mem_stat_reg3(void **var, int type)
{
   return mem_stat_reg_list(var,type, MY_LIST3);
}

int mem_stat_free3(int mark)
{
   return mem_stat_free_list(mark, MY_LIST3);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

