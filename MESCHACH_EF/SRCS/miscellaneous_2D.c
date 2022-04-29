

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_2D.h"

/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

VEC  *build_vec_from_function2D(ELT_2D *MyElt, GEOM_2D *MyGeom, FUN_2D* fun, VEC *vec)
{
   int s;
   
   /* check */
   if ( MyElt      == ELT_2D_NULL   ) error(E_NULL, "build_vec_from_function2D");
   if ( MyGeom     == GEOM_2D_NULL  ) error(E_NULL, "build_vec_from_function2D");
   if ( fun        == FUN_2D_NULL   ) error(E_NULL, "build_vec_from_function2D");
   if ( vec        == VNULL         ) error(E_NULL, "build_vec_from_function2D");

   for (s=0; s<MyGeom->NBSOMM ; s++)
   {
      vec->ve[s] = fun->eval(fun, MyGeom->XYSOMM->me[s][0] ,
                                  MyGeom->XYSOMM->me[s][1] );
   }

   return vec;
}

/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

VEC  *build_vec_from_function2Dtransient(ELT_2D *MyElt, GEOM_2D *MyGeom, FUN_2D* fun, VEC *vec, Real tps)
{
   int s;
   
   /* check */
   if ( MyElt      == ELT_2D_NULL   ) error(E_NULL, "build_vec_from_function2D");
   if ( MyGeom     == GEOM_2D_NULL  ) error(E_NULL, "build_vec_from_function2D");
   if ( fun        == FUN_2D_NULL   ) error(E_NULL, "build_vec_from_function2D");
   if ( vec        == VNULL         ) error(E_NULL, "build_vec_from_function2D");

   for (s=0; s<MyGeom->NBSOMM ; s++)
   {
      vec->ve[s] = fun->eval(fun, MyGeom->XYSOMM->me[s][0],
                                  MyGeom->XYSOMM->me[s][1],
								 tps);
   }

   return vec;
}

/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
