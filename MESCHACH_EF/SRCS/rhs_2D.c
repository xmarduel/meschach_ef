
/** \file rhs_2D.c
 *
 * Contains the rhs functions for dim 2
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/rhs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D*  Rhs2D_get(void)
{
   RHS_2D *MyRhs;
   int i,j;

   if ( (MyRhs=NEW(RHS_2D)) == (RHS_2D *)NULL )
   {
      error(E_MEM, "RHS2D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_RHS_2D, 0, sizeof(RHS_2D), MY_LIST6);
      mem_numvar_list(TYPE_RHS_2D, 1, MY_LIST6);
   }

   MyRhs->current_selected_axe = AXEe_X;
   
   for (i=0; i<2; i++)
   for (j=0; j<NBMAX_RHS_2D_FUNCTIONS; j++)
   {
      Fun2D_init(&(MyRhs->Fun[i][j]));
   }

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setup_from_params(const PARAMS* MyParams)
{
	RHS_2D* MyRhs = Rhs2D_get();
   
   Rhs2D_setLUAFunction(MyRhs, /*ref_e*/0, AXEe_X, MyParams->rhs_params.rhs[AXEe_X].fundef   );
   Rhs2D_setLUAFunction(MyRhs, /*ref_e*/0, AXEe_Y, MyParams->rhs_params.rhs[AXEe_X].fundef   );

	//Rhs2D_setCFunction(MyRhs, /*ref_e*/0, AXEe_X, sources2D[MyParams->rhs_params.rhs[AXEe_X]] );
	//Rhs2D_setCFunction(MyRhs, /*ref_e*/0, AXEe_Y, sources2D[MyParams->rhs_params.rhs[AXEe_Y]] );
   
	return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Rhs2D_free( RHS_2D *MyRhs )
{
   if ( MyRhs == (RHS_2D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_RHS_2D, sizeof(RHS_2D), 0, MY_LIST6);
         mem_numvar_list(TYPE_RHS_2D, -1, MY_LIST6);
      }

      free(MyRhs);
      
      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setFunction      (RHS_2D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyRhs == NULL )                  error (E_NULL, "Rhs2D_setFunction");
   
   if ( ref_e > NBMAX_RHS_2D_FUNCTIONS )   error6(E_RHS_WRONGIDXNUMBER, "Rhs2D_setFunction");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y ) ) error6(E_RHS_WRONGAXENUMBER, "Rhs2D_setFunction"); 
   
   /* set the function */
   Fun2D_setFunction(&(MyRhs->Fun[axe][ref_e]), type, phi, clientdata);

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setCFunction      (RHS_2D* MyRhs, int ref_e, int axe, FUNC_2D phi)
{
   return  Rhs2D_setFunction(MyRhs, ref_e, axe, FUN_C_STATIONNARY, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setLUAFunction    ( RHS_2D* MyRhs, int ref_e, int axe, const char *def)
{
   void *L = make_lua_interpreter(def, "2D");
   
   return Rhs2D_setFunction(MyRhs, ref_e, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction2D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setCFunctionTransient( RHS_2D* MyRhs, int ref_e, int axe, FUNC_3D phi)
{
   return  Rhs2D_setFunction(MyRhs, ref_e, axe, FUN_C_TRANSIENT, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setLUAFunctionTransient( RHS_2D* MyRhs, int ref_e, int axe, const char *def)
{
   void *L = make_lua_interpreter(def, "2D_TR");
   
   return Rhs2D_setFunction(MyRhs, ref_e, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction3D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real    Rhs2D_evalFunction        (const RHS_2D* MyRhs, int ref_e, int axe , Real x, Real y)
{
   Real val;

   if ( MyRhs == NULL )    error (E_NULL, "Rhs2D_evalFunction");
   
   if ( ref_e > NBMAX_RHS_2D_FUNCTIONS )   error6(E_RHS_WRONGIDXNUMBER, "Rhs2D_evalFunction");
   if ( (axe != AXEe_X)&&(axe !=AXEe_Y ) ) error6(E_RHS_WRONGAXENUMBER, "Rhs2D_evalFunction");

   val = MyRhs->Fun[axe][ref_e].eval(&MyRhs->Fun[axe][ref_e], x, y, MyRhs->tps);
   
   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setTps(RHS_2D* MyRhs, Real tps)
{
   if ( MyRhs == NULL ) error (E_NULL, "Rhs2D_setTps");
   
   MyRhs->tps = tps;
 
   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_2D* Rhs2D_setCurrentSelectedAxe (RHS_2D* MyRhs, int axe)
{
   /* check */
   if ( MyRhs == NULL )                      error (E_NULL, "Rhs2D_setCurrentSelectedAxe");
   if ( (axe != AXEe_X) && (axe != AXEe_Y) ) error6(E_RHS_WRONGAXENUMBER, "Rhs2D_setCurrentSelectedAxe");

   MyRhs->current_selected_axe = axe;

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
