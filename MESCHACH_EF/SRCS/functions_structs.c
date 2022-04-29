
/*
 * easy administration for the structures "functions"
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"   /* for axes enumerations */
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"


/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int mem_attach_list5(void)
{
   return mem_attach_list(MY_LIST5, NB_STRUCTS5, my_names5, my_frees5, my_tnums5);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int err_attach_list5(void)
{
   return err_list_attach(MY_LIST5, NB_ERRS5, my_err_mesg5 );
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int warn_attach_list5(void)
{
   return warn_list_attach(MY_LIST5, NB_WARNS5, my_warn_mesg5 );
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int err_warn_attach_lists5(void)
{
   err_attach_list5();
   warn_attach_list5();

   return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

FUNC_1D function1D_by_name(const char *funcname, FUN_CLASS function_class)
{
	FUNC_1D fun;
	
	FUNC_1D_NAMEMAP *name_to_func = NULL;
	
	
	switch(function_class)
	{
		case FUNCTION_RHSe:
			name_to_func = RHS_FUNC_1D_NAMEMAP;
			break;
			
		case FUNCTION_BCe:
			name_to_func = BC_FUNC_1D_NAMEMAP;
			break;
         
      case FUNCTION_ADVe:
			name_to_func = ADV_FUNC_1D_NAMEMAP;
			break;
			
		case FUNCTION_SOLe:
			name_to_func = SOL_FUNC_1D_NAMEMAP;
			break;
			
		case FUNCTION_ICe:
			name_to_func = RHS_FUNC_1D_NAMEMAP;
			break;
			
		default:
			break;
	}
	
	if ( name_to_func == NULL )
	{
		error(E_NULL, "function1D_by_name");
	}
	
	while ( name_to_func->func != NULL )
	{
		if ( strcmp(name_to_func->name, funcname) == 0 )
		{
			return name_to_func->func;
		}
		
		++name_to_func;
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

FUNC_2D function2D_by_name(const char *funcname, FUN_CLASS function_class)
{
	FUNC_2D fun;
	
	FUNC_2D_NAMEMAP *name_to_func = NULL;
	
	
	switch(function_class)
	{
		case FUNCTION_RHSe:
			name_to_func = RHS_FUNC_2D_NAMEMAP;
			break;
			
		case FUNCTION_BCe:
			name_to_func = BC_FUNC_2D_NAMEMAP;
			break;
         
      case FUNCTION_ADVe:
			name_to_func = ADV_FUNC_2D_NAMEMAP;
			break;
			
		case FUNCTION_SOLe:
			name_to_func = SOL_FUNC_2D_NAMEMAP;
			break;
			
		case FUNCTION_ICe:
			name_to_func = RHS_FUNC_2D_NAMEMAP;
			break;
			
		default:
			break;
	}
	
	if ( name_to_func == NULL )
	{
		error(E_NULL, "function2D_by_name");
	}
	
	while ( name_to_func->func != NULL )
	{
		if ( strcmp(name_to_func->name, funcname) == 0 )
		{
			return name_to_func->func;
		}
		
		++name_to_func;
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

FUNC_3D function3D_by_name(const char *funcname, FUN_CLASS function_class)
{
	FUNC_3D fun;
	
	FUNC_3D_NAMEMAP *name_to_func = NULL;
	
	
	switch(function_class)
	{
		case FUNCTION_RHSe:
			name_to_func = RHS_FUNC_3D_NAMEMAP;
			break;
			
		case FUNCTION_BCe:
			name_to_func = BC_FUNC_3D_NAMEMAP;
			break;
         
      case FUNCTION_ADVe:
			name_to_func = ADV_FUNC_3D_NAMEMAP;
			break;
			
		case FUNCTION_SOLe:
			name_to_func = SOL_FUNC_3D_NAMEMAP;
			break;
			
		case FUNCTION_ICe:
			name_to_func = RHS_FUNC_3D_NAMEMAP;
			break;
			
		default:
			break;
	}
	
	if ( name_to_func == NULL )
	{
		error(E_NULL, "function3D_by_name");
	}
	
	while ( name_to_func->func != NULL )
	{
		if ( strcmp(name_to_func->name, funcname) == 0 )
		{
			return name_to_func->func;
		}
		
		++name_to_func;
	}
	
	return NULL;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int function1Dindex_by_name(const char *funcname, FUN_CLASS function_class)
{	
	FUNC_1D_NAMEMAP *name_to_func = NULL;
	
	
	switch(function_class)
	{
		case FUNCTION_RHSe:
			name_to_func = RHS_FUNC_1D_NAMEMAP;
			break;
			
		case FUNCTION_BCe:
			name_to_func = BC_FUNC_1D_NAMEMAP;
			break;
         
      case FUNCTION_ADVe:
			name_to_func = ADV_FUNC_1D_NAMEMAP;
			break;
			
		case FUNCTION_SOLe:
			name_to_func = SOL_FUNC_1D_NAMEMAP;
			break;
			
		case FUNCTION_ICe:
			name_to_func = RHS_FUNC_1D_NAMEMAP;
			break;
			
		default:
			break;
	}
	
	if ( name_to_func == NULL )
	{
		error(E_NULL, "function1Dindex_by_name");
	}
	
	int idx = 0;
	
	while ( name_to_func->func != NULL )
	{
		if ( strcmp(name_to_func->name, funcname) == 0 )
		{
			return idx;
		}
		
		++idx;
		
		++name_to_func;
	}

	return -1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int function2Dindex_by_name(const char *funcname, FUN_CLASS function_class)
{	
	FUNC_2D_NAMEMAP *name_to_func = NULL;
	
	
	switch(function_class)
	{
		case FUNCTION_RHSe:
			name_to_func = RHS_FUNC_2D_NAMEMAP;
			break;
			
		case FUNCTION_BCe:
			name_to_func = BC_FUNC_2D_NAMEMAP;
			break;
         
      case FUNCTION_ADVe:
			name_to_func = ADV_FUNC_2D_NAMEMAP;
			break;
			
		case FUNCTION_SOLe:
			name_to_func = SOL_FUNC_2D_NAMEMAP;
			break;
			
		case FUNCTION_ICe:
			name_to_func = RHS_FUNC_2D_NAMEMAP;
			break;
			
		default:
			break;
	}
	
	if ( name_to_func == NULL )
	{
		error(E_NULL, "function2Dindex_by_name");
	}
	
	int idx = 0;
	
	while (name_to_func->func != NULL)
	{
		if ( strcmp(name_to_func->name, funcname) == 0 )
		{
			return idx;
		}
		
		++idx;
		
		++name_to_func;
	}
	
	return -1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int function3Dindex_by_name(const char *funcname, FUN_CLASS function_class)
{	
	FUNC_3D_NAMEMAP *name_to_func = NULL;
	
	
	switch(function_class)
	{
		case FUNCTION_RHSe:
			name_to_func = RHS_FUNC_3D_NAMEMAP;
			break;
			
		case FUNCTION_BCe:
			name_to_func = BC_FUNC_3D_NAMEMAP;
			break;
         
      case FUNCTION_ADVe:
			name_to_func = ADV_FUNC_3D_NAMEMAP;
			break;
			
		case FUNCTION_SOLe:
			name_to_func = SOL_FUNC_3D_NAMEMAP;
			break;
			
		case FUNCTION_ICe:
			name_to_func = RHS_FUNC_3D_NAMEMAP;
			break;
			
		default:
			break;
	}
	
	if ( name_to_func == NULL )
	{
		error(E_NULL, "function3Dindex_by_name");
	}
	
	int idx = 0;
	
	while (name_to_func->func != NULL)
	{
		if ( strcmp(name_to_func->name, funcname) == 0 )
		{
			return idx;
		}
		
		++idx;
		
		++name_to_func;
	}
	
	return -1;
}
