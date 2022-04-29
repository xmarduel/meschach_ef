
/** \file adv_3D.c
 *
 * Contains the adv functions for dim 3
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/adv.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D* Adv3D_get(void)
{
   int i,j,k;
   ADV_3D *MyAdv;

   if ( (MyAdv=NEW(ADV_3D)) == (ADV_3D *)NULL )
   {
      error(E_MEM, "Adv3D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_ADV_3D, 0, sizeof(ADV_3D), MY_LIST7);
      mem_numvar_list(TYPE_ADV_3D, 1, MY_LIST7);
   }
   

   MyAdv->current_selected_axe1 = AXEe_X;
   MyAdv->current_selected_axe2 = AXEe_X;
   MyAdv->current_selected_axe3 = AXEe_X;
   
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   for (k=0; k<NBMAX_ADV_3D_FUNCTIONS; k++)
   {
      Fun3D_init(&(MyAdv->Fun1[i][j][k]));
      Fun3D_init(&(MyAdv->Fun2[i][j][k]));
      Fun3D_init(&(MyAdv->Fun3[i][j][k]));
   }
     
   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D* Adv3D_setup_from_params(const PARAMS *MyParams)
{
	
   ADV_3D *MyADV = Adv3D_get();
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_X, sources3D[MyParams->adv_params.adv1[AXEe_X][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_Y, sources3D[MyParams->adv_params.adv1[AXEe_X][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_X, AXEe_Z, sources3D[MyParams->adv_params.adv1[AXEe_X][AXEe_Z]] );
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Y, AXEe_X, sources3D[MyParams->adv_params.adv1[AXEe_Y][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Y, AXEe_Y, sources3D[MyParams->adv_params.adv1[AXEe_Y][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Y, AXEe_Z, sources3D[MyParams->adv_params.adv1[AXEe_Y][AXEe_Z]] );
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Z, AXEe_X, sources3D[MyParams->adv_params.adv1[AXEe_Z][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Z, AXEe_Y, sources3D[MyParams->adv_params.adv1[AXEe_Z][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_X, AXEe_Z, AXEe_Z, sources3D[MyParams->adv_params.adv1[AXEe_Z][AXEe_Z]] );
	
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_X, AXEe_X, sources3D[MyParams->adv_params.adv2[AXEe_X][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_X, AXEe_Y, sources3D[MyParams->adv_params.adv2[AXEe_X][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_X, AXEe_Z, sources3D[MyParams->adv_params.adv2[AXEe_X][AXEe_Z]] );
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Y, AXEe_X, sources3D[MyParams->adv_params.adv2[AXEe_Y][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Y, AXEe_Y, sources3D[MyParams->adv_params.adv2[AXEe_Y][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Y, AXEe_Z, sources3D[MyParams->adv_params.adv2[AXEe_Y][AXEe_Z]] );
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Z, AXEe_X, sources3D[MyParams->adv_params.adv2[AXEe_Z][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Z, AXEe_Y, sources3D[MyParams->adv_params.adv2[AXEe_Z][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Y, AXEe_Z, AXEe_Z, sources3D[MyParams->adv_params.adv2[AXEe_Z][AXEe_Z]] );
	
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_X, AXEe_X, sources3D[MyParams->adv_params.adv3[AXEe_X][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_X, AXEe_Y, sources3D[MyParams->adv_params.adv3[AXEe_X][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_X, AXEe_Z, sources3D[MyParams->adv_params.adv3[AXEe_X][AXEe_Z]] );
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_Y, AXEe_X, sources3D[MyParams->adv_params.adv3[AXEe_Y][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_Y, AXEe_Y, sources3D[MyParams->adv_params.adv3[AXEe_Y][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_Y, AXEe_Z, sources3D[MyParams->adv_params.adv3[AXEe_Y][AXEe_Z]] );
	
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_Z, AXEe_X, sources3D[MyParams->adv_params.adv3[AXEe_Z][AXEe_X]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_Z, AXEe_Y, sources3D[MyParams->adv_params.adv3[AXEe_Z][AXEe_Y]] );
	Adv3D_setCFunction(MyADV, /*ref_e*/0, AXEe_Z, AXEe_Z, AXEe_Z, sources3D[MyParams->adv_params.adv3[AXEe_Z][AXEe_Z]] );
	
	return MyADV;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Adv3D_free (ADV_3D *MyAdv)
{
   if ( MyAdv == (ADV_3D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_ADV_3D, sizeof(ADV_3D), 0, MY_LIST7);
         mem_numvar_list(TYPE_ADV_3D, -1, MY_LIST7);
      }

      free(MyAdv);
      
      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D* Adv3D_setFunction      (ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyAdv == NULL )                  error (E_NULL, "Adv3D_setFunction");
   
   if ( ref_e > NBMAX_ADV_3D_FUNCTIONS )                                error7(E_ADV_WRONGIDXNUMBER, "Adv3D_setFunction");
 
   if ( !( (axe1 == AXEe_X) || (axe1 == AXEe_Y) || (axe1 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_setFunction");
   if ( !( (axe2 == AXEe_X) || (axe2 == AXEe_Y) || (axe2 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_setFunction");
   if ( !( (axe3 == AXEe_X) || (axe3 == AXEe_Y) || (axe3 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_setFunction");
   
   /* set the function */
   switch(axe1)
   {
      case AXEe_X: Fun3D_setFunction(&(MyAdv->Fun1[axe2][axe3][ref_e]), type, phi, clientdata); break;
      case AXEe_Y: Fun3D_setFunction(&(MyAdv->Fun2[axe2][axe3][ref_e]), type, phi, clientdata); break;
      case AXEe_Z: Fun3D_setFunction(&(MyAdv->Fun3[axe2][axe3][ref_e]), type, phi, clientdata); break;
   }

   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D *Adv3D_setCFunction      (ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_3D phi)
{
   return Adv3D_setFunction( MyAdv, ref_e, axe1, axe2, axe3, FUN_C_STATIONNARY, phi, NULL);   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D *Adv3D_setCFunctionTransient(ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_4D phi)
{
   return Adv3D_setFunction( MyAdv, ref_e, axe1, axe2, axe3, FUN_C_TRANSIENT, phi, NULL);  
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

typedef FUN_3D    FUN_3D_ARRAY[3][3][NBMAX_ADV_3D_FUNCTIONS];

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real    Adv3D_evalFunction        (const ADV_3D* MyAdv, int ref_e, int axe1 , int axe2 , int axe3 , Real x, Real y, Real z)
{
   Real val = 0.0;
   
   FUN_3D_ARRAY * axe_fun;

   /* check arguments */
   if ( MyAdv == NULL )    error (E_NULL, "Adv3D_evalFunction");
   
   /* check arguments */
   if ( ref_e > NBMAX_ADV_3D_FUNCTIONS )                                error7(E_ADV_WRONGIDXNUMBER, "Adv3D_evalFunction");

   /* check arguments */
   if ( !( (axe1 == AXEe_X) || (axe1 == AXEe_Y) || (axe1 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_evalFunction");
   if ( !( (axe2 == AXEe_X) || (axe2 == AXEe_Y) || (axe2 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_evalFunction");
   if ( !( (axe3 == AXEe_X) || (axe3 == AXEe_Y) || (axe3 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_evalFunction");

   /* the evaluation depends on the type of function stores in the ADV_2D function at the given axes & reference */
   switch(axe1)
   {
      case AXEe_X: val = MyAdv->Fun1[axe2][axe3][ref_e].eval(&MyAdv->Fun1[axe2][axe3][ref_e], x, y, z, MyAdv->tps); break;
      case AXEe_Y: val = MyAdv->Fun2[axe2][axe3][ref_e].eval(&MyAdv->Fun2[axe2][axe3][ref_e], x, y, z, MyAdv->tps); break;
      case AXEe_Z: val = MyAdv->Fun3[axe2][axe3][ref_e].eval(&MyAdv->Fun3[axe2][axe3][ref_e], x, y, z, MyAdv->tps); break;
   }

   return val;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D*  Adv3D_setTps              (ADV_3D* MyAdv, Real tps)
{
   if ( MyAdv == NULL )  error (E_NULL, "Adv3D_setTps");
   
   MyAdv->tps = tps;
 
   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

ADV_3D*  Adv3D_setCurrentSelectedAxe (ADV_3D* MyAdv, int axe1, int axe2, int axe3)
{
   /* check */
   if ( MyAdv == NULL )  error (E_NULL, "Adv3D_setCurrentSelectedAxe");

   if ( !( (axe1 == AXEe_X) || (axe1 == AXEe_Y) || (axe1 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_setCurrentSelectedAxe");
   if ( !( (axe2 == AXEe_X) || (axe2 == AXEe_Y) || (axe2 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_setCurrentSelectedAxe");
   if ( !( (axe3 == AXEe_X) || (axe3 == AXEe_Y) || (axe3 == AXEe_Z) ) ) error7(E_ADV_WRONGAXENUMBER, "Adv3D_setCurrentSelectedAxe");
   
   MyAdv->current_selected_axe1 = axe1;
   MyAdv->current_selected_axe2 = axe2;
   MyAdv->current_selected_axe3 = axe3;
   
   return MyAdv;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/
