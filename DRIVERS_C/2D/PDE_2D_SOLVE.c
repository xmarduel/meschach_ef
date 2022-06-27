
/*
 *
 * --- Numerical Resolution of PDE 2D ---
 *
 *     examples :
 *
 *     Laplacien :
 *        -e.LAPLACIEN(u) = F 
 *        B.C. Dirichlet Type
 *
 *     Convection-Diffusion :
 *        -e.LAPLACIEN(u) + b.GRADIENT(u) = F 
 *        B.C. Dirichlet Type
 *
 *     Stokes :
 *        -1./Re*LAPLACIEN(U) + GRADIENT(p) = F
 *        DIVERGENCE(U)                     = 0
 *
 *     Navier-Stokes :
 *        -1./Re*LAPLACIEN(U) + U*GRADIENT(U) + GRADIENT(p) = F
 *        DIVERGENCE(U)                                     = 0
 *
 */

#include <pthread.h>

#include <time.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/rhs.h"
#include "MESCHACH_EF/INCLUDES/adv.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_2D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_2D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_2D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_geoms.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_stationnary.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_stationnary.h"

/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

/* ------------------------------------------------------------------------ */


int  main()
{
   
   ELT_2D   *MyElt , *MyEltM1 , *elt_P1 , *elt_P2 , *elt_P3 ;
   
   GEOM_2D  *MyGeom;

   BC_2D    *MyBC;
   
   RHS_2D   *MyRHS;
   
   ADV_2D   *MyADV;

   VEC      *SOL;

   PARAMS   *MyParams ;
   
   /* memory informations */
   mem_attach_list(MY_LIST1, NB_STRUCTS1, my_names1, my_frees1, my_tnums1);
   mem_attach_list(MY_LIST2, NB_STRUCTS2, my_names2, my_frees2, my_tnums2);
   mem_attach_list(MY_LIST3, NB_STRUCTS3, my_names3, my_frees3, my_tnums3);
   mem_attach_list(MY_LIST4, NB_STRUCTS4, my_names4, my_frees4, my_tnums4);
   mem_attach_list(MY_LIST5, NB_STRUCTS5, my_names5, my_frees5, my_tnums5);
   mem_attach_list(MY_LIST6, NB_STRUCTS6, my_names6, my_frees6, my_tnums6);
   mem_attach_list(MY_LIST7, NB_STRUCTS7, my_names7, my_frees7, my_tnums7);
   
   
   err_list_attach(MY_LIST2, NB_ERRS2, my_err_mesg2);
   err_list_attach(MY_LIST3, NB_ERRS3, my_err_mesg3);
   err_list_attach(MY_LIST4, NB_ERRS4, my_err_mesg4);
   err_list_attach(MY_LIST5, NB_ERRS5, my_err_mesg5);
   err_list_attach(MY_LIST6, NB_ERRS6, my_err_mesg6);
   err_list_attach(MY_LIST7, NB_ERRS7, my_err_mesg7);
   
   warn_list_attach(MY_LIST2, NB_WARNS2, my_warn_mesg2);
   warn_list_attach(MY_LIST3, NB_WARNS3, my_warn_mesg3);
   warn_list_attach(MY_LIST4, NB_WARNS4, my_warn_mesg4);
   warn_list_attach(MY_LIST5, NB_WARNS5, my_warn_mesg5);
   warn_list_attach(MY_LIST6, NB_WARNS6, my_warn_mesg6);
   warn_list_attach(MY_LIST7, NB_WARNS7, my_warn_mesg7);
    
   /*  LECTURE  IN  INPUT FILE "PDE_EXAMPLE_2D.json"    */
   
   MyParams = Params_setup_from_file("/Users/xavier/DEVELOPMENT/MESCHACH_WORK/DRIVERS_C/2D/PDE_EXAMPLE_2D.json");

   Params_set_staticparam(MyParams,0);
   
   elt_P1 = elt2D_get("P1");
   elt_P2 = elt2D_get("P2");
   elt_P3 = elt2D_get("P3");
   
   
   MyElt    = elt2D_get(MyParams->ef_params.name_ef);  /* "P1"  "P2" */
   
   if ( strcmp("P1", MyParams->ef_params.name_ef) == 0 )
   {
      MyEltM1  = elt_P1;
   }
   else
   if ( strcmp("P1b", MyParams->ef_params.name_ef ) == 0 )
   {
      MyEltM1  = elt_P1;
   }
   else
   if ( strcmp("P2", MyParams->ef_params.name_ef ) == 0 )
   {
      MyEltM1  = elt_P1;
   }
   else
   if ( strcmp("P3", MyParams->ef_params.name_ef ) == 0 )
   {
      MyEltM1  = elt_P2;
   }
   else
   {
      error(E_UNKNOWN, "main");
   }
   
   MyGeom = Geom2D_get(MyElt,
					MyParams->geom_params.meshfile,
					MyParams->geom_params.meshname,
					MyParams->geom_params.meshtype);  /* ".quad1" */  /* ".quad2" */  /* ".emc2"  */  /* ".gmsh"  */
   
   if (MyParams->misc_graphics.igeomview)
   {
      graphics_geom2D_view("X11", MyGeom, "dummy1" );
   }

   if (MyParams->misc_graphics.igeomtotalview)
   {
      GEOM_2D *MyGeomTotal = Geom2D_getP1geom_from(MyElt, MyGeom);

      graphics_geom2D_view("X11", MyGeomTotal, "dummy2");

      GEOM_2D_FREE(MyGeomTotal);
   }
   

   
   MyBC = Bc2D_setup_from_params(MyParams);
	
   Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X);
   
   
   MyRHS = Rhs2D_setup_from_params(MyParams);

   MyADV = Adv2D_setup_from_params(MyParams);


   /* ---------------------------------------------------------------------- */
   
   if ( strcmp(MyParams->problem,"LAPLACIAN") == 0 )
   {
      SOL = solve2D_laplacian(MyElt, MyGeom, MyBC, MyRHS);
   }
   else if ( strcmp(MyParams->problem,"CONVECTION-DIFFUSION") == 0 )
   {
      SOL = solve2D_convection_diffusion(MyElt, MyGeom, MyBC, MyRHS, MyADV);
   }
   else if ( strcmp(MyParams->problem,"STOKES") == 0 )
   {
      (void)solve2D_stokes(MyElt, MyEltM1, MyGeom, MyBC, MyRHS);
   }
   else if ( strcmp(MyParams->problem,"NAVIER-STOKES") == 0 )
   {
      (void)solve2D_navier_stokes(MyElt, MyEltM1, MyGeom, MyBC, MyRHS);
   }
   else
   {
      printf("Problem \"%s\" not yet implemented", MyParams->problem);
      exit(EXIT_FAILURE);
   }
   
   
   /* ------ graphics output ----------------------------------------------- */ 

   if ( (strcmp(MyParams->problem, "LAPLACIAN") == 0) || (strcmp(MyParams->problem, "CONVECTION-DIFFUSION") == 0) )
   {
      if (MyParams->graph_params.GNUPLOT) graphics2D("gnuplot", MyElt, MyGeom, SOL, "SolApproch" );
      if (MyParams->graph_params.MATLAB)  graphics2D("matlab" , MyElt, MyGeom, SOL, "SolApproch" );
   
   
      if (MyParams->graph_params.SILO)    graphics2D("silo", MyElt, MyGeom, SOL, "SolApproch");
      if (MyParams->graph_params.VTK )    graphics2D("vtk" , MyElt, MyGeom, SOL, "SolApproch");
   }
   
   if ( (strcmp(MyParams->problem, "LAPLACIAN") == 0) || (strcmp(MyParams->problem, "CONVECTION-DIFFUSION") == 0) )
   {
      if (strcmp(MyParams->graph_interactiv2Dplots_params.ENGINE , "VOGLE") == 0)
      {
         GEOM_2D *geomP1 = Geom2D_getP1geom_from(MyElt, MyGeom);

         if (strcmp(MyParams->graph_interactiv2Dplots_params.GRAPHICS_TYPE[1][1], "SURFACE") == 0)
         {
            graphics2D_vogle_initialize(MyParams->graph_interactiv2Dplots_params.DRIVER,
                                        MyParams->graph_interactiv2Dplots_params.SIZE_WINDOW_X,
                                        MyParams->graph_interactiv2Dplots_params.SIZE_WINDOW_Y);
				
            graphics2D_vogle_title("SOLUTION");
            graphics2D_vogle_store_surfacedata(MyGeom->NSELMT, MyGeom->XYSOMM);
            graphics2D_vogle_store_plotdata(SOL);
				
            graphics2D_vogle_plotsurface();
         }
         else
         {
            graphics1D_vopl_initialize(MyParams->graph_interactiv2Dplots_params.DRIVER,
                                       MyParams->graph_interactiv2Dplots_params.SIZE_WINDOW_X,
                                       MyParams->graph_interactiv2Dplots_params.SIZE_WINDOW_Y, 1, 1, VOPL_CONTOURS);
            graphics1D_vopl_title("SOLUTION");
            graphics1D_vopl_window(1, 1, MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].X_MIN,
                                         MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].X_MAX,
                                         MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].Y_MIN,
                                         MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].Y_MAX);
				
            graphics1D_vopl_nblevels(1, 1, MyParams->graph_interactiv2Dplots_params.NB_LEVELS[1][1]);
            graphics1D_vopl_levels(1, 1, MyParams->graph_interactiv2Dplots_params.LEVELS[1][1]);
            graphics1D_vopl_contourmeshdata(1, 1, geomP1->NSELMT, geomP1->XYSOMM);
            graphics1D_vopl_contourplotdata(1, 1, SOL);
				
            graphics1D_vopl_plotcontour();
         }

         GEOM_2D_FREE(geomP1);
      }
   }
 
   /* ------ graphics output ----------------------------------------------- */    
   
   ELT_2D_FREE(elt_P1);
   ELT_2D_FREE(elt_P2);
   ELT_2D_FREE(elt_P3);
      
   ELT_2D_FREE(MyElt);
   GEOM_2D_FREE(MyGeom);
   BC_2D_FREE(MyBC);
   RHS_2D_FREE(MyRHS);
   ADV_2D_FREE(MyADV);
   
   /*V_FREE(SOL);*/

   Params_free(MyParams);

   /* -----------------------------------------------------------------------*/

   mem_info_file(stdout, 0);  /* = mem_info(); */
   
   mem_info_file(stdout, MY_LIST1);
   mem_info_file(stdout, MY_LIST2);
   mem_info_file(stdout, MY_LIST3);
   mem_info_file(stdout, MY_LIST4);
   mem_info_file(stdout, MY_LIST5);
   mem_info_file(stdout, MY_LIST6);
   mem_info_file(stdout, MY_LIST7);

   pthread_exit(NULL);
   
   return EXIT_SUCCESS;
   
}
