
/** \file adv_2D.c
 *
 * Contains the adv functions for dim 2
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/adv.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D* Adv2D_get(void)
{
   ADV_2D *MyAdv;
   int i,j,k;

   if ( (MyAdv=NEW(ADV_2D)) == (ADV_2D *)NULL )
   {
      error(E_MEM, "Adv2D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_ADV_2D, 0, sizeof(ADV_2D), MY_LIST7);
      mem_numvar_list(TYPE_ADV_2D, 1, MY_LIST7);
   }

   MyAdv->current_selected_axe1 = AXEe_X;
   MyAdv->current_selected_axe2 = AXEe_X;
   MyAdv->current_selected_axe3 = AXEe_X;
   
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   for (k=0; k<NBMAX_ADV_2D_FUNCTIONS; k++)
   {
      Fun2D_init(&(MyAdv->Fun1[i][j][k]));
      Fun2D_init(&(MyAdv->Fun2[i][j][k]));
   }

   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D* Adv2D_setup_from_params(const PARAMS *MyParams)
{
	ADV_2D* MyADV = Adv2D_get();

	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_X, sources2D[MyParams->adv_params.adv1[AXEe_X][AXEe_X]] );
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_Y, sources2D[MyParams->adv_params.adv1[AXEe_X][AXEe_Y]] );
	
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Y, AXEe_X, sources2D[MyParams->adv_params.adv1[AXEe_Y][AXEe_X]] );
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Y, AXEe_X, sources2D[MyParams->adv_params.adv1[AXEe_Y][AXEe_Y]] );

	
		
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_X, AXEe_X, sources2D[MyParams->adv_params.adv2[AXEe_Y][AXEe_X]] );
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_X, AXEe_Y, sources2D[MyParams->adv_params.adv2[AXEe_Y][AXEe_Y]] );
	
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Y, AXEe_X, sources2D[MyParams->adv_params.adv2[AXEe_Y][AXEe_X]] );
	Adv2D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Y, AXEe_Y, sources2D[MyParams->adv_params.adv2[AXEe_Y][AXEe_Y]] );
   
	return MyADV;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Adv2D_free (ADV_2D *MyAdv)
{
   if ( MyAdv == (ADV_2D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_ADV_2D, sizeof(ADV_2D), 0, MY_LIST7);
         mem_numvar_list(TYPE_ADV_2D, -1, MY_LIST7);
      }

      free(MyAdv);
      
      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D* Adv2D_setFunction      (ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyAdv == NULL )                  error (E_NULL, "Adv2D_setFunction");
   
   if ( ref_e > NBMAX_ADV_2D_FUNCTIONS )            error7(E_ADV_WRONGIDXNUMBER, "Adv2D_setFunction");

   if ( !( (axe1 == AXEe_X) || (axe1 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_setFunction");
   if ( !( (axe2 == AXEe_X) || (axe2 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_setFunction");
   if ( !( (axe3 == AXEe_X) || (axe3 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_setFunction");
   
   /* set the function */
   switch(axe1)
   {
      case AXEe_X: Fun2D_setFunction(&(MyAdv->Fun1[axe2][axe3][ref_e]), type, phi, clientdata); break;
      case AXEe_Y: Fun2D_setFunction(&(MyAdv->Fun2[axe2][axe3][ref_e]), type, phi, clientdata); break;
   }

   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D* Adv2D_setCFunction      (ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_2D phi)
{
   return  Adv2D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_C_STATIONNARY, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D* Adv2D_setCFunctionTransient (ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_3D phi)
{
   return  Adv2D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_C_TRANSIENT, phi, NULL);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

typedef FUN_2D    FUN_2D_ARRAY[2][2][NBMAX_ADV_2D_FUNCTIONS];

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real    Adv2D_evalFunction        (const ADV_2D* MyAdv, int ref_e, int axe1 , int axe2 , int axe3 , Real x, Real y)
{
   Real val = 0.0;

   FUN_2D_ARRAY * axe_fun;

   /* check arguments */
   if ( MyAdv == NULL )    error (E_NULL, "Adv2D_evalFunction");
   
   /* check arguments */
   if ( ref_e > NBMAX_ADV_2D_FUNCTIONS )            error7(E_ADV_WRONGIDXNUMBER, "Adv2D_evalFunction");

   /* check arguments */
   if ( !( (axe1 == AXEe_X) || (axe1 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_evalFunction");
   if ( !( (axe2 == AXEe_X) || (axe2 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_evalFunction");
   if ( !( (axe3 == AXEe_X) || (axe3 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_evalFunction");


   /* the evaluation depends on the type of function stores in the ADV_2D function at the given axes & reference */

   switch(axe1)
   {
      case AXEe_X: val = MyAdv->Fun1[axe2][axe3][ref_e].eval(&MyAdv->Fun1[axe2][axe3][ref_e], x, y, MyAdv->tps); break;
      case AXEe_Y: val = MyAdv->Fun2[axe2][axe3][ref_e].eval(&MyAdv->Fun2[axe2][axe3][ref_e], x, y, MyAdv->tps); break;
   }
   
   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D*  Adv2D_setTps (ADV_2D* MyAdv, Real tps)
{
   if ( MyAdv == NULL )  error (E_NULL, "Adv2D_setTps");
   
   MyAdv->tps = tps;
 
   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_2D*  Adv2D_setCurrentSelectedAxe (ADV_2D* MyAdv, int axe1, int axe2, int axe3)
{
   /* check */
   if ( MyAdv == NULL )                      error (E_NULL, "Adv2D_setCurrentSelectedAxe");
   
   if ( !( (axe1 == AXEe_X) || (axe1 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_setCurrentSelectedAxe");
   if ( !( (axe2 == AXEe_X) || (axe2 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_setCurrentSelectedAxe");
   if ( !( (axe3 == AXEe_X) || (axe3 == AXEe_Y) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv2D_setCurrentSelectedAxe");

   MyAdv->current_selected_axe1 = axe1;
   MyAdv->current_selected_axe2 = axe2;
   MyAdv->current_selected_axe3 = axe3;

   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
