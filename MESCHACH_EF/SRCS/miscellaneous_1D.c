
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

VEC* build_vec_from_function1D(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D* dfun, VEC *vec)
{
   int s;

   if ( MyElt   == ELT_1D_NULL   ) error(E_NULL, "build_vec_from_function1D");
   if ( MyGeom  == GEOM_1D_NULL  ) error(E_NULL, "build_vec_from_function1D");
   if ( fun     == FUN_1D_NULL   ) error(E_NULL, "build_vec_from_function1D");
   //if ( dfun   == FUN_1D_NULL   ) error(E_NULL, "build_vec_from_function1D");
   if ( vec     == VNULL         ) error(E_NULL, "build_vec_from_function1D");

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
   if ( (strcmp(MyElt->name_ef,"S3") == 0)||
        (strcmp(MyElt->name_ef,"S5") == 0) )
   {
      VEC *vec_world = v_get(MyGeom->NBSOMM);
	   
	   /* give vec in the world space */
	   for (s=0; s<MyGeom->NBSOMM; s++)
	   {
         vec_world->ve[s] = fun->eval(fun, MyGeom->XSOMM->ve[s]);
	   }
	      
	   /* give vec in the EF space */
      vec = spLUresolution_spooles(MyGeom->EF_to_WORLD, vec_world, vec);

      /* clean */
      V_FREE(vec_world);
   }
   else
   if ( (strcmp(MyElt->name_ef,"S2") == 0) ||
        (strcmp(MyElt->name_ef,"S4") == 0) )
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
      error(E_UNKNOWN, "build_vec_from_function1D");
   }
   
   return vec;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

VEC* build_vec_from_function1Dtransient(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D* dfun, VEC *vec, Real tps)
{
   int s;

   if ( MyElt   == ELT_1D_NULL   ) error(E_NULL, "build_vec_from_function1D");
   if ( MyGeom  == GEOM_1D_NULL  ) error(E_NULL, "build_vec_from_function1D");
   if ( fun     == FUN_1D_NULL   ) error(E_NULL, "build_vec_from_function1D");
   //if ( dfun   == FUN_1D_NULL   ) error(E_NULL, "build_vec_from_function1D");
   if ( vec     == VNULL         ) error(E_NULL, "build_vec_from_function1D");

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
	      
	   /* give vec in the EF space */
      vec = spLUresolution_spooles(MyGeom->EF_to_WORLD, vec_world, vec);

      /* clean */
      V_FREE(vec_world);
   }
   else
   {
      error(E_UNKNOWN, "build_vec_from_function1D");
   }
   
   return vec;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

VEC* get_vector_for_plotting_1D( const ELT_1D *MyElt, const GEOM_1D *MyGeom, const VEC *SOL )
{
   VEC* GRAPH;

   /* check */
   if ( MyElt      == ELT_1D_NULL   ) error(E_NULL, "get_vector_for_plotting_1D");
   if ( MyGeom     == GEOM_1D_NULL  ) error(E_NULL, "get_vector_for_plotting_1D");
   if ( SOL        == VNULL         ) error(E_NULL, "get_vector_for_plotting_1D");

   /* check */
   if ( SOL->dim   != MyGeom->EF_to_WORLD->n ) error(E_SIZES, "get_vector_for_plotting_1D");

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
      VEC *TMP = sp_mv_mlt(MyGeom->EF_to_WORLD, SOL, NULL );

      int delta_nbsomm = 0;

      if (strcmp(MyElt->name_ef,"S2") == 0) delta_nbsomm = 1;
      if (strcmp(MyElt->name_ef,"S3") == 0) delta_nbsomm = 2;
      if (strcmp(MyElt->name_ef,"S4") == 0) delta_nbsomm = 3;
      if (strcmp(MyElt->name_ef,"S5") == 0) delta_nbsomm = 4;
      
      if ( MyGeom->periodicity == NON_PERIODIC_MESHe )
      {
         if (strcmp(MyElt->name_ef,"S2") == 0  || strcmp(MyElt->name_ef,"S4") == 0)
         {
            GRAPH = v_copy(TMP, NULL);
            //GRAPH = v_get(SOL->dim - delta_nbsomm);
            //v_move(TMP,(delta_nbsomm+1)/2, GRAPH->dim-1, GRAPH, 0);
         }
         else
         {
            GRAPH = v_get(SOL->dim - delta_nbsomm);
            v_move(TMP,(delta_nbsomm+1)/2, GRAPH->dim, GRAPH, 0);
         }
      }
      else
      if ( MyGeom->periodicity == PERIODIC_MESHe )
      {
         GRAPH = v_copy(TMP, NULL);
      }
      else
      {
         error(E_UNKNOWN, "get_vector_for_plotting_1D");
      }

      V_FREE(TMP);
   }
   else
   {
      error(E_UNKNOWN, "get_vector_for_plotting_1D");
   }

   return GRAPH;
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/
