
/*
 *
 * --- Numerical Resolution of PDE 3D ---
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
#include "MESCHACH_EF/INCLUDES/assemblage_objects_3D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_3D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_3D.h"

/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

/* ------------------------------------------------------------------------ */


int  main()
{
   
   ELT_3D   *MyElt , *elt_P1 , *elt_P1b , *elt_P2 ;

   GEOM_3D  *MyGeom;

   BC_3D    *MyBC;
   
   RHS_3D   *MyRHS;

   ADV_3D   *MyADV;
   
   VEC      *SOL = NULL;

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
   //err_list_attach(MY_LIST8,NB_ERRS8,my_err_mesg8);

   warn_list_attach(MY_LIST2, NB_WARNS2, my_warn_mesg2);
   warn_list_attach(MY_LIST3, NB_WARNS3, my_warn_mesg3);
   warn_list_attach(MY_LIST4, NB_WARNS4, my_warn_mesg4);
   warn_list_attach(MY_LIST5, NB_WARNS5, my_warn_mesg5);
   warn_list_attach(MY_LIST6, NB_WARNS6, my_warn_mesg6);
   warn_list_attach(MY_LIST7, NB_WARNS7, my_warn_mesg7);
	//warn_list_attach(MY_LIST8,NB_WARNS8,my_warn_mesg8);
   
   /*  LECTURE INPUT FILE "PDE_EXAMPLE_3D.json" */
   MyParams = Params_setup_from_file("/Users/xavier/DEVELOPMENT/MESCHACH_WORK/DRIVERS_C/3D/PDE_EXAMPLE_3D.json");

   Params_set_staticparam(MyParams, 0);
   
   elt_P1  = elt3D_get("P1");
   elt_P1b = elt3D_get("P1b");
   elt_P2  = elt3D_get("P2");
   
   MyElt  = elt3D_get(MyParams->ef_params.name_ef);  /* "P1" */
                                                     /* "P1b" */
                                                     /* "P2" */
   
   MyGeom = Geom3D_get(MyElt, 
			MyParams->geom_params.meshfile,
			MyParams->geom_params.meshname,
			MyParams->geom_params.meshtype);

   Geom3D_foutput(MyParams->io_files.fp2, MyGeom);

   MyBC = Bc3D_setup_from_params(MyParams);
   
   Geom3D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X);
   
   
   MyRHS = Rhs3D_setup_from_params(MyParams);


   /* ---------------------------------------------------------------------- */
   
   if ( strcmp(MyParams->problem, "LAPLACIAN") == 0 )
   {
      SOL = solve3D_laplacian(MyElt, MyGeom, MyBC, MyRHS);
   }
   else
   if ( strcmp(MyParams->problem, "CONVECTION-DIFFUSION") == 0 )
   {
      SOL = solve3D_convection_diffusion(MyElt, MyGeom, MyBC, MyRHS, NULL);
   }
   else
   if ( strcmp(MyParams->problem, "STOKES") == 0 )
   {
      (void)solve3D_stokes(MyElt, elt_P1, MyGeom, MyBC, MyRHS);
   }
   else
   {
      printf("Problem \"%s\" not yet implemented", MyParams->problem);
      exit(EXIT_FAILURE);
   }
   
   /* ----------------------------------------------------------------------------- */
   
   if ( strcmp(MyParams->problem, "LAPLACIAN") == 0 )
   {
      if (MyParams->graph_params.SILO) graphics3D( "silo", MyElt, MyGeom, SOL, "SolApproch");
      if (MyParams->graph_params.VTK ) graphics3D( "vtk" , MyElt, MyGeom, SOL, "SolApproch");
   }

   if ( MyParams->exactsol_params.exact_sol[0] >= 0 )
   {
      double norm_diff = 0.0;
      FUN_3D  *fun3D = Fun3D_get();
      Fun3D_setCFunction(fun3D, ExSol3D[MyParams->exactsol_params.exact_sol[0]]);
      
      graphics3D_fun("vtk", MyElt , MyGeom , fun3D, "Sol_Exacte");
      
      VEC *sol_exacte = v_get(MyGeom->NBSOMM);
      sol_exacte = build_vec_from_function3D(MyElt, MyGeom, fun3D, sol_exacte);
      
      VEC *v_diff = v_get(MyGeom->NBSOMM);
      double diff = v_norm2( v_sub(SOL, sol_exacte, v_diff) );
      
      printf("diff solexacte-solapprochee = %le \n", norm_diff);
      
      graphics3D("vtk",  MyElt, MyGeom, SOL, "Diff_Approch");
      
      FUN_3D_FREE(fun3D);
   }
   /* ----------------------------------------------------------------------------- */
   
   
   ELT_3D_FREE(elt_P1);
   ELT_3D_FREE(elt_P2);
   ELT_3D_FREE(elt_P1b);
      
   ELT_3D_FREE(MyElt);
   GEOM_3D_FREE(MyGeom);
   BC_3D_FREE(MyBC);
   RHS_3D_FREE(MyRHS);

   if (SOL != NULL) {
      V_FREE(SOL);
   }

   /*------------------------------------------------------------------------*/

   mem_info_file(stdout, 0);
   
   mem_info_file(stdout, MY_LIST1);
   mem_info_file(stdout, MY_LIST2);
   mem_info_file(stdout, MY_LIST3);
   mem_info_file(stdout, MY_LIST4);
   mem_info_file(stdout, MY_LIST5);
   mem_info_file(stdout, MY_LIST6);
   mem_info_file(stdout, MY_LIST7);
   
   /*------------------------------------------------------------------------*/

   return EXIT_SUCCESS;
}
