

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_3D.h"

/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

VEC* build_vec_ef_from_function3D(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const FUN_3D *FUN, VEC* vec)
{
   Real diff = 0.0;
   int s;

   /* check */
   if ( MyElt      == ELT_3D_NULL   ) error(E_NULL, "build_vec_ef_from_function3D");
   if ( MyGeom     == GEOM_3D_NULL  ) error(E_NULL, "build_vec_ef_from_function3D");
   if ( FUN        == FUN_3D_NULL   ) error(E_NULL, "build_vec_ef_from_function3D");
   if ( vec        == VNULL         ) error(E_NULL, "build_vec_ef_from_function3D");


   for (s=0; s<MyGeom->NBSOMM ; s++)
   {
      vec->ve[s] = FUN->eval(FUN, MyGeom->XYSOMM->me[s][0] ,
                                  MyGeom->XYSOMM->me[s][1] ,
                                  MyGeom->XYSOMM->me[s][2] );
   }

   return vec;
}

/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/

VEC* build_vec_ef_from_function3Dtransient(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const FUN_3D *FUN, VEC* vec, Real tps)
{
   Real diff = 0.0;
   int s;

   /* check */
   if ( MyElt      == ELT_3D_NULL   ) error(E_NULL, "build_vec_ef_from_function3Dtransient");
   if ( MyGeom     == GEOM_3D_NULL  ) error(E_NULL, "build_vec_ef_from_function3Dtransient");
   if ( FUN        == FUN_3D_NULL   ) error(E_NULL, "build_vec_ef_from_function3Dtransient");
   if ( vec        == VNULL         ) error(E_NULL, "build_vec_ef_from_function3Dtransient");


   for (s=0; s<MyGeom->NBSOMM ; s++)
   {
      vec->ve[s] = FUN->eval(FUN, MyGeom->XYSOMM->me[s][0],
                                  MyGeom->XYSOMM->me[s][1],
                                  MyGeom->XYSOMM->me[s][2],
                                  tps);
   }

   return vec;
}

/*--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------*/
