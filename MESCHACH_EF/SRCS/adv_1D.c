
/** \file adv_1D.c
 *
 * Contains the adv functions for dim 1
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/adv.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D* Adv1D_get(void)
{
   ADV_1D *MyAdv;
   int i,j,k;
   
   if ( (MyAdv=NEW(ADV_1D)) == (ADV_1D *)NULL )
   {
      error(E_MEM, "Adv1D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_ADV_1D, 0, sizeof(ADV_1D), MY_LIST7);
      mem_numvar_list(TYPE_ADV_1D, 1, MY_LIST7);
   }  

   for (i=0; i<1; i++)
   for (j=0; j<1; j++)
   for (k=0; k<NBMAX_ADV_1D_FUNCTIONS; k++)
   {
      Fun1D_init(&(MyAdv->Fun[i][j][k]));
   }
   
   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D* Adv1D_setup_from_params(const PARAMS *MyParams)
{
   ADV_1D* MyAdv = Adv1D_get();
   
   int is_lua_def = 1;
   
   if (is_lua_def)
   {
      Adv1D_setLUAFunction(MyAdv, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_X, MyParams->adv_params.adv1[AXEe_X][AXEe_X].fundef );
   }
   else
   {
      // int idx = 2; // MyParams->adv_params.adv1[AXEe_X][AXEe_X] // TODO
      //Adv1D_setCFunction  (MyAdv, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_X, sources1D[idx]);
   }
   
   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Adv1D_free (ADV_1D *MyAdv)
{
   if ( MyAdv == (ADV_1D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_ADV_1D, sizeof(ADV_1D), 0, MY_LIST7);
         mem_numvar_list(TYPE_ADV_1D, -1, MY_LIST7);
      }

      free(MyAdv);
      
      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D *Adv1D_setFunction      (ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyAdv == NULL )                  error (E_NULL, "Adv1D_setFunction");
   
   if ( ref_e > NBMAX_ADV_1D_FUNCTIONS ) error7(E_ADV_WRONGIDXNUMBER, "Adv1D_setFunction");

   if ( axe1 != AXEe_X )                 error7(E_ADV_WRONGAXENUMBER, "Adv1D_setFunction");
   if ( axe2 != AXEe_X )                 error7(E_ADV_WRONGAXENUMBER, "Adv1D_setFunction");
   if ( axe3 != AXEe_X )                 error7(E_ADV_WRONGAXENUMBER, "Adv1D_setFunction");
   
   /* set the function */
   Fun1D_setFunction(&(MyAdv->Fun[axe2][axe3][ref_e]), type, phi, clientdata);
   
   return MyAdv;   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D *Adv1D_setCFunction      (ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_1D phi)
{
   return Adv1D_setFunction( MyAdv, ref_e, axe1, axe2, axe3, FUN_C_STATIONNARY, phi, NULL);   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D *Adv1D_setLUAFunction    (ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, const char *def)
{
   void *L = make_lua_interpreter(def, "1D");
   
   return Adv1D_setFunction( MyAdv, ref_e, axe1, axe2, axe3, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction1D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D *Adv1D_setCFunctionTransient (ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_2D phi)
{
   return Adv1D_setFunction( MyAdv, ref_e, axe1, axe2, axe3, FUN_C_TRANSIENT, phi, NULL);  
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D *Adv1D_setLUAFunctionTransient (ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, const char *def)
{
   void *L = make_lua_interpreter(def, "1D_TR");
   
   return Adv1D_setFunction( MyAdv, ref_e, axe1, axe2, axe3, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction2D, L);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real    Adv1D_evalFunction        (const ADV_1D* MyAdv, int ref_e, int axe1 , int axe2 , int axe3 , Real x)
{
   Real val;

   /* check arguments */
   if ( MyAdv == NULL )    error (E_NULL, "Adv1D_evalFunction");
   
   /* check arguments */
   if ( ref_e > NBMAX_ADV_1D_FUNCTIONS ) error7(E_ADV_WRONGIDXNUMBER, "Adv1D_setFunction");

   /* check arguments */
   if ( axe1 != AXEe_X )                 error7(E_ADV_WRONGAXENUMBER, "Adv1D_evalFunction");
   if ( axe2 != AXEe_X )                 error7(E_ADV_WRONGAXENUMBER, "Adv1D_evalFunction");
   if ( axe3 != AXEe_X )                 error7(E_ADV_WRONGAXENUMBER, "Adv1D_evalFunction");

   /* the evaluation depends on the type of function stores in the ADV_1D function at the given axes & reference */
   val = MyAdv->Fun[axe2][axe3][ref_e].eval( &MyAdv->Fun[axe2][axe3][ref_e], x, MyAdv->tps);
   
   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_1D* Adv1D_setTps (ADV_1D* MyAdv, Real tps)
{
   if ( MyAdv == NULL )    error (E_NULL, "Adv1D_setTps");
   
   MyAdv->tps = tps;

   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
