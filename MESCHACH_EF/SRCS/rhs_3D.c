
/** \file rhs_3D.c
 *
 * Contains the structure of rhs in 3 dimensions
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/rhs.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D* Rhs3D_get(void)
{
   int i,j;
   RHS_3D *MyRhs;

   if ( (MyRhs=NEW(RHS_3D)) == (RHS_3D *)NULL )
   {
      error(E_MEM, "RHS3D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_RHS_3D, 0, sizeof(RHS_3D), MY_LIST6);
      mem_numvar_list(TYPE_RHS_3D, 1, MY_LIST6);
   }

   MyRhs->current_selected_axe = AXEe_X;

   for (i=0; i<3; i++)
   for (j=0; j<NBMAX_RHS_3D_FUNCTIONS; j++)
   {
      Fun3D_init(&(MyRhs->Fun[i][j]));
   }

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D* Rhs3D_setup_from_params(const PARAMS* MyParams)
{
	RHS_3D* MyRhs = Rhs3D_get();

   Rhs3D_setLUAFunction(MyRhs, /*ref_e*/0, AXEe_X, MyParams->rhs_params.rhs[AXEe_X].fundef   );
   Rhs3D_setLUAFunction(MyRhs, /*ref_e*/0, AXEe_X, MyParams->rhs_params.rhs[AXEe_Y].fundef   );
   Rhs3D_setLUAFunction(MyRhs, /*ref_e*/0, AXEe_Z, MyParams->rhs_params.rhs[AXEe_Z].fundef   );

	//Rhs3D_setCFunction(MyRhs, /*ref_e*/0, AXEe_X, sources3D[MyParams->rhs_params.rhs[AXEe_X]] );
	//Rhs3D_setCFunction(MyRhs, /*ref_e*/0, AXEe_Y, sources3D[MyParams->rhs_params.rhs[AXEe_Y]] );
	//Rhs3D_setCFunction(MyRhs, /*ref_e*/0, AXEe_Z, sources3D[MyParams->rhs_params.rhs[AXEe_Z]] );

	return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Rhs3D_free(RHS_3D *MyRhs)
{
   if ( MyRhs == (RHS_3D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_RHS_3D, sizeof(RHS_3D), 0, MY_LIST6);
         mem_numvar_list(TYPE_RHS_3D, -1, MY_LIST6);
      }

      free(MyRhs);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D* Rhs3D_setFunction      (RHS_3D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyRhs == NULL )                               error (E_NULL, "Rhs3D_setFunction");

   if ( ref_e > NBMAX_RHS_3D_FUNCTIONS )              error6(E_RHS_WRONGIDXNUMBER, "Rhs3D_setFunction");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error6(E_RHS_WRONGAXENUMBER, "Rhs3D_setFunction");

   /* set the function */
   Fun3D_setFunction(&(MyRhs->Fun[axe][ref_e]), type, phi, clientdata);

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D *Rhs3D_setCFunction      (RHS_3D* MyRhs, int ref_e, int axe, FUNC_3D phi)
{
   return Rhs3D_setFunction(MyRhs, ref_e, axe, FUN_C_STATIONNARY, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D* Rhs3D_setLUAFunction    ( RHS_3D* MyRhs, int ref_e, int axe, const char *def)
{
   void *L = make_lua_interpreter(def, "3D");

   return Rhs3D_setFunction(MyRhs, ref_e, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction3D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D *Rhs3D_setCFunctionTransient(RHS_3D* MyRhs, int ref_e, int axe, FUNC_4D phi)
{
   return Rhs3D_setFunction(MyRhs, ref_e, axe, FUN_C_TRANSIENT, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D* Rhs3D_setLUAFunctionTransient( RHS_3D* MyRhs, int ref_e, int axe, const char *def)
{
   void *L = make_lua_interpreter(def, "3D_TR");

   return Rhs3D_setFunction(MyRhs, ref_e, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction4D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real    Rhs3D_evalFunction        (const RHS_3D* MyRhs, int ref_e, int axe , Real x, Real y, Real z)
{
   Real val;

   if ( MyRhs == NULL )    error (E_NULL, "Rhs3D_evalFunction");

   if ( ref_e > NBMAX_RHS_3D_FUNCTIONS )              error6(E_RHS_WRONGIDXNUMBER, "Rhs3D_evalFunction");
   if ( (axe!=AXEe_X)&&(axe!=AXEe_Y)&&(axe!=AXEe_Z) ) error6(E_RHS_WRONGAXENUMBER, "Rhs3D_evalFunction");

   val = MyRhs->Fun[axe][ref_e].eval(&MyRhs->Fun[axe][ref_e], x, y, z, MyRhs->tps);

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D *Rhs3D_setTps (RHS_3D* MyRhs, Real tps)
{
   if ( MyRhs == NULL )  error (E_NULL, "Rhs3D_setTps");

   MyRhs->tps = tps;

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_3D *Rhs3D_setCurrentSelectedAxe (RHS_3D* MyRhs, int axe)
{
   /* check */
   if ( MyRhs == NULL )                                         error (E_NULL, "Rhs3D_setCurrentSelectedAxe");
   if ( (axe != AXEe_X) && (axe != AXEe_Y) && (axe != AXEe_Z) ) error6(E_RHS_WRONGAXENUMBER, "Rhs3D_setCurrentSelectedAxe");

   MyRhs->current_selected_axe = axe;

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
