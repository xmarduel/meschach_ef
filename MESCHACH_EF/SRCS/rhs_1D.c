
/** \file rhs_1D.c
 *
 * Contains the rhs functions in 1 dimension
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/rhs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D*  Rhs1D_get(void)
{
   RHS_1D *MyRhs;
   int i,j;

   if ( (MyRhs=NEW(RHS_1D)) == (RHS_1D *)NULL )
   {
      error(E_MEM, "Rhs1D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_RHS_1D, 0, sizeof(RHS_1D), MY_LIST6);
      mem_numvar_list(TYPE_RHS_1D, 1, MY_LIST6);
   }

   for (i=0; i<1; i++)
   for (j=0; j<NBMAX_RHS_1D_FUNCTIONS; j++)
   {
      Fun1D_init(&(MyRhs->Fun[i][j]));
   }

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D* Rhs1D_setup_from_params(const PARAMS* MyParams)
{
	RHS_1D* MyRhs = Rhs1D_get();

   Rhs1D_setLUAFunction(MyRhs, /*ref_e*/0, AXEe_X, MyParams->rhs_params.rhs[AXEe_X].fundef );

	return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/


int Rhs1D_free(RHS_1D *MyRhs)
{
   if ( MyRhs == (RHS_1D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_RHS_1D, sizeof(RHS_1D), 0, MY_LIST6);
         mem_numvar_list(TYPE_RHS_1D, -1, MY_LIST6);
      }

      free(MyRhs);

      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D *Rhs1D_setFunction      (RHS_1D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyRhs == NULL )                  error (E_NULL, "Rhs1D_setFunction");

   if ( ref_e > NBMAX_RHS_1D_FUNCTIONS ) error6(E_RHS_WRONGIDXNUMBER, "Rhs1D_setFunction");
   if ( axe != AXEe_X )                  error6(E_RHS_WRONGAXENUMBER, "Rhs1D_setFunction");

   /* set the function */
   Fun1D_setFunction(&(MyRhs->Fun[axe][ref_e]), type, phi, clientdata);

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D *Rhs1D_setCFunction      (RHS_1D* MyRhs, int ref_e, int axe, FUNC_1D phi)
{
   return Rhs1D_setFunction(MyRhs, ref_e, axe, FUN_C_STATIONNARY, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D* Rhs1D_setLUAFunction         ( RHS_1D* MyRhs, int ref_e, int axe, const char *def)
{
   void *L = make_lua_interpreter(def, "1D");

   return Rhs1D_setFunction(MyRhs, ref_e, axe, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction1D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D *Rhs1D_setCFunctionTransient(RHS_1D* MyRhs, int ref_e, int axe, FUNC_2D phi)
{
   return Rhs1D_setFunction(MyRhs, ref_e, axe, FUN_C_TRANSIENT, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D* Rhs1D_setLUAFunctionTransient( RHS_1D* MyRhs, int ref_e, int axe, const char *def)
{
   void *L = make_lua_interpreter(def, "1D_TR");

   return Rhs1D_setFunction(MyRhs, ref_e, axe, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction2D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real    Rhs1D_evalFunction        (const RHS_1D* MyRhs, int ref_e, int axe , Real x)
{
   Real val;

   if ( MyRhs == NULL )    error (E_NULL, "Rhs1D_evalFunction");

   if ( ref_e > NBMAX_RHS_1D_FUNCTIONS ) error6(E_RHS_WRONGIDXNUMBER, "Rhs1D_evalFunction");
   if ( axe != AXEe_X )                  error6(E_RHS_WRONGAXENUMBER, "Rhs1D_evalFunction");

   val = MyRhs->Fun[axe][ref_e].eval(&MyRhs->Fun[axe][ref_e], x, MyRhs->tps);

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

RHS_1D *Rhs1D_setTps (RHS_1D* MyRhs, Real tps)
{
   if ( MyRhs == NULL )    error (E_NULL, "Rhs1D_setTps");

   MyRhs->tps = tps;

   return MyRhs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

