
#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse2.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_factor.h"

#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_1D.h"

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

VEC* build_vec_ef_from_function1D(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D* dfun, VEC *vec)
{
   int s;

   if ( MyElt   == ELT_1D_NULL   ) error(E_NULL, "build_vec_ef_from_function1D");
   if ( MyGeom  == GEOM_1D_NULL  ) error(E_NULL, "build_vec_ef_from_function1D");
   if ( fun     == FUN_1D_NULL   ) error(E_NULL, "build_vec_ef_from_function1D");
   //if ( dfun   == FUN_1D_NULL   ) error(E_NULL, "build_vec_ef_from_function1D");
   if ( vec     == VNULL         ) error(E_NULL, "build_vec_ef_from_function1D");

   if ( (strcmp(MyElt->name_ef,"P1") == 0) ||
        (strcmp(MyElt->name_ef,"P2") == 0) ||
        (strcmp(MyElt->name_ef,"P3") == 0)  )
   {  
      for (s=0; s<MyGeom->NBSOMM; s++)
      {
         vec->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s]);
      }
   }
   else
   if (strcmp(MyElt->name_ef,"H3") == 0) 
   {
      if ( dfun == FUN_1D_NULL ) error(E_NULL, "build_vec_from_function1D");

      for (s=0; s<MyGeom->NBSOMM/2; s++)
      {
    	     vec->ve[2*s  ] = fun->eval(fun  , MyGeom->XSOMM->ve[2*s+0]);
    	     vec->ve[2*s+1] = dfun->eval(dfun, MyGeom->XSOMM->ve[2*s+1]);
      }
   }
   else
   if ( (strcmp(MyElt->name_ef,"S2") == 0) ||
        (strcmp(MyElt->name_ef,"S3") == 0) ||
        (strcmp(MyElt->name_ef,"S4") == 0) ||
        (strcmp(MyElt->name_ef,"S5") == 0)  )
   {
      VEC *vec_world = v_get(MyGeom->NBSOMM);
      
      /* give vec in the world space */
      for (s=0; s<MyGeom->NBSOMM; s++)
      {
         vec_world->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s]);
      }
      
      // inverse of EF_to_WORLD: LU error !!
      int nb_steps = 0;
      iter_xspbicgstab(MyGeom->EF_to_WORLD, NULL, vec_world, 1.0e-12, vec, 100, &nb_steps, NULL);
      printf(" sol exact eval ... bicgstab: # of iter. = %d \n\n", nb_steps);
   }
   else
   {
      error(E_UNKNOWN, "build_vec_ef_from_function1D");
   }
   
   return vec;
}

VEC* build_vec_world_from_function1D(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D* dfun, VEC *vec)
{
   int s;
   
   if ( MyElt   == ELT_1D_NULL   ) error(E_NULL, "build_vec_world_from_function1D");
   if ( MyGeom  == GEOM_1D_NULL  ) error(E_NULL, "build_vec_world_from_function1D");
   if ( fun     == FUN_1D_NULL   ) error(E_NULL, "build_vec_world_from_function1D");
   //if ( dfun   == FUN_1D_NULL   ) error(E_NULL, "build_vec_world_from_function1D");
   //if ( vec     == VNULL         ) error(E_NULL, "build_vec_world_from_function1D");
   
   if (vec == VNULL)
   {
      if (strcmp(MyElt->name_ef,"H3") == 0)
      {
         vec = v_get(MyGeom->NBSOMM/2);
      }
      else
      {
         vec = v_get(MyGeom->NBSOMM);
      }
   }
   
   if ( (strcmp(MyElt->name_ef,"P1") == 0) ||
        (strcmp(MyElt->name_ef,"P2") == 0) ||
        (strcmp(MyElt->name_ef,"P3") == 0) ||
        (strcmp(MyElt->name_ef,"S2") == 0) ||
        (strcmp(MyElt->name_ef,"S3") == 0) ||
        (strcmp(MyElt->name_ef,"S4") == 0) ||
        (strcmp(MyElt->name_ef,"S5") == 0) )
   {
      for (s=0; s<MyGeom->NBSOMM; s++)
      {
         vec->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s]);
      }
   }
   else
   if (strcmp(MyElt->name_ef,"H3") == 0)
   {
      for (s=0; s<MyGeom->NBSOMM/2; s++)
      {
         vec->ve[s] = fun->eval(fun  , MyGeom->XSOMM->ve[2*s]);
      }
   }
   else
   {
      error(E_UNKNOWN, "build_vec_world_from_function1D");
   }
   
   return vec;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

VEC* build_vec_ef_from_function1Dtransient(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D* dfun, VEC *vec, Real tps)
{
   int s;

   if ( MyElt   == ELT_1D_NULL   ) error(E_NULL, "build_vec_ef_from_function1Dtransient");
   if ( MyGeom  == GEOM_1D_NULL  ) error(E_NULL, "build_vec_ef_from_function1Dtransient");
   if ( fun     == FUN_1D_NULL   ) error(E_NULL, "build_vec_ef_from_function1Dtransient");
   //if ( dfun   == FUN_1D_NULL   ) error(E_NULL, "build_vec_ef_from_function1Dtransient");
   if ( vec     == VNULL         ) error(E_NULL, "build_vec_ef_from_function1Dtransient");

   if ( (strcmp(MyElt->name_ef,"P1") == 0) ||
        (strcmp(MyElt->name_ef,"P2") == 0) ||
        (strcmp(MyElt->name_ef,"P3") == 0)  )
   {  
      for (s=0; s<MyGeom->NBSOMM; s++)
      {
         vec->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s], tps);
      }
   }
   else
   if (strcmp(MyElt->name_ef,"H3") == 0) 
   {
      if ( dfun == FUN_1D_NULL ) error(E_NULL, "build_vec_from_function1D");

      for (s=0; s<MyGeom->NBSOMM/2; s++)
      {
    	     vec->ve[2*s  ] = fun->eval(fun  , MyGeom->XSOMM->ve[2*s+0], tps);
    	     vec->ve[2*s+1] = dfun->eval(dfun, MyGeom->XSOMM->ve[2*s+1], tps);
      }
   }
   else
   if ( (strcmp(MyElt->name_ef,"S2") == 0)||
        (strcmp(MyElt->name_ef,"S3") == 0)||
        (strcmp(MyElt->name_ef,"S4") == 0)||
        (strcmp(MyElt->name_ef,"S5") == 0) )
   {
      VEC *vec_world = v_get(MyGeom->NBSOMM);
	   
	   /* give vec in the world space */
	   for (s=0; s<MyGeom->NBSOMM; s++)
	   {
         vec_world->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s], tps);
	   }
	      
      // inverse of EF_to_WORLD: LU error !!
      int nb_steps = 0;
      sp_foutput(stdout,  MyGeom->EF_to_WORLD);
      iter_xspcg(MyGeom->EF_to_WORLD, NULL, vec_world, 1.0e-12, vec, 100, &nb_steps, NULL);
      //iter_xspbicgstab(MyGeom->EF_to_WORLD, NULL, vec_world, 1.0e-12, vec, 100, &nb_steps, NULL);
      printf(" sol exact eval ... bicgstab: # of iter. = %d \n\n", nb_steps);

      /* clean */
      V_FREE(vec_world);
   }
   else
   {
      error(E_UNKNOWN, "build_vec_ef_from_function1Dtransient");
   }
   
   return vec;
}


VEC* build_vec_world_from_function1Dtransient(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D* dfun, VEC *vec, Real tps)
{
   int s;
   
   if ( MyElt   == ELT_1D_NULL   ) error(E_NULL, "build_vec_world_from_function1Dtransient");
   if ( MyGeom  == GEOM_1D_NULL  ) error(E_NULL, "build_vec_world_from_function1Dtransient");
   if ( fun     == FUN_1D_NULL   ) error(E_NULL, "build_vec_world_from_function1Dtransient");
   //if ( dfun   == FUN_1D_NULL   ) error(E_NULL, "build_vec_world_from_function1Dtransient");
   if ( vec     == VNULL         ) error(E_NULL, "build_vec_world_from_function1Dtransient");
   
   if ( (strcmp(MyElt->name_ef,"P1") == 0) ||
        (strcmp(MyElt->name_ef,"P2") == 0) ||
        (strcmp(MyElt->name_ef,"P3") == 0)  ||
        (strcmp(MyElt->name_ef,"S2") == 0)||
        (strcmp(MyElt->name_ef,"S3") == 0)||
        (strcmp(MyElt->name_ef,"S4") == 0)||
        (strcmp(MyElt->name_ef,"S5") == 0) )
   {
      for (s=0; s<MyGeom->NBSOMM; s++)
      {
         vec->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s], tps);
      }
   }
   else
   if (strcmp(MyElt->name_ef,"H3") == 0)
   {
      if ( dfun == FUN_1D_NULL ) error(E_NULL, "build_vec_world_from_function1Dtransient");
         
      for (s=0; s<MyGeom->NBSOMM/2; s++)
      {
         vec->ve[2*s  ] = fun->eval(fun  , MyGeom->XSOMM->ve[2*s+0], tps);
         vec->ve[2*s+1] = dfun->eval(dfun, MyGeom->XSOMM->ve[2*s+1], tps);
      }
   }
   else
   {
      error(E_UNKNOWN, "build_vec_world_from_function1Dtransient");
   }
   
   return vec;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

VEC* build_vec_world_from_vec_ef_1D( const ELT_1D *MyElt, const GEOM_1D *MyGeom, const VEC *SOL )
{
   VEC* GRAPH;

   /* check */
   if ( MyElt      == ELT_1D_NULL   ) error(E_NULL, "build_vec_world_from_vec_ef_1D");
   if ( MyGeom     == GEOM_1D_NULL  ) error(E_NULL, "build_vec_world_from_vec_ef_1D");
   if ( SOL        == VNULL         ) error(E_NULL, "build_vec_world_from_vec_ef_1D");

   /* check */
   if ( SOL->dim   != MyGeom->EF_to_WORLD->n ) error(E_SIZES, "build_vec_world_from_vec_ef_1D");

   /**/
   if ( (strcmp(MyElt->name_ef,"P1") == 0) ||
        (strcmp(MyElt->name_ef,"P2") == 0) ||
        (strcmp(MyElt->name_ef,"P3") == 0)  )
   {
      GRAPH = v_copy(SOL, NULL);
   }
   else
   if ( (strcmp(MyElt->name_ef,"H3") == 0)  )
   {
      GRAPH = sp_mv_mlt(MyGeom->EF_to_WORLD, SOL, NULL );
   }
   else
   if ( (strcmp(MyElt->name_ef,"S2") == 0) ||
        (strcmp(MyElt->name_ef,"S3") == 0) ||
        (strcmp(MyElt->name_ef,"S4") == 0) ||
        (strcmp(MyElt->name_ef,"S5") == 0)  ) 
   {
      GRAPH = sp_mv_mlt(MyGeom->EF_to_WORLD, SOL, NULL );
   }
   else
   {
      error(E_UNKNOWN, "build_vec_world_from_vec_ef_1D");
   }

   return GRAPH;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
