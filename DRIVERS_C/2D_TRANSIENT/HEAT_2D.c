
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
 */

#include <time.h>
#include <pthread.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"
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

#ifdef HAVE_VOGLE
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_transient.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_transient.h"
#endif

/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

/* ------------------------------------------------------------------------ */

int  main()
{

   ELT_2D   *MyElt , *MyEltM1 , *elt_P1 , *elt_P2 , *elt_P3 ;

   GEOM_2D  *MyGeom;

   BC_2D    *MyBC;

   RHS_2D   *MyRhsFun;

   FUN_2D   *MyCiFun;

   VEC      *U_o , *U_nm1, *U_n ;

   SPMAT *MASS;
   SPMAT *RIGID1;

   SPMAT *A;
   SPMAT *A_REF;

   SPMAT *LLT;

   VEC *RHS;
   VEC *RHS_FUN;
   VEC *RHS_UUx;
   VEC *RHS_TIME_DISCR;

   PARAMS   *MyParams ;

   double TPS;
   int    IT_TPS;
   int    IT_TPS_MAX;
   int    i,k;
   double DT;

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

   /*  LECTURE INPUT FILE "PDE_EXAMPLE_TR_2D.json" */

   MyParams = Params_setup_from_file("DRIVERS_C/2D_TRANSIENT/PDE_EXAMPLE_TR_2D.json");

   Params_set_staticparam(MyParams,0);

   elt_P1 = elt2D_get("P1");
   elt_P2 = elt2D_get("P2");
   elt_P3 = elt2D_get("P3");


   MyElt    = elt2D_get(MyParams->ef_params.name_ef);  /* "P1" */
                                                       /* "P2" */
   if ( strcmp("P1", MyParams->ef_params.name_ef) == 0 )
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
               MyParams->geom_params.meshtype);  /* ".quad1" ".quad2" ".emc2" ".gmsh"  */
   /*
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
   */

   MyBC = Bc2D_setup_from_params(MyParams);

   Geom2D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X);


   MyRhsFun = Rhs2D_setup_from_params(MyParams);


   /* ---------------------------------------------------------------------- */
   TPS = 0.0;
   IT_TPS_MAX = floor(MyParams->time_params.TPS_FIN / MyParams->time_params.DT) ;
   if ((MyParams->time_params.TPS_FIN/MyParams->time_params.DT - IT_TPS_MAX)>0.5) IT_TPS_MAX += 1;
   printf("IT_TPS_MAX = %d \n",IT_TPS_MAX);

   DT = MyParams->time_params.DT;
   /* --------------------------------------------------------------------- */

   /* ---------------------------------------------------------------------- */

   /*U_o = build_vec_from_function2D(MyElt, MyGeom, MyCiFun, U_o);*/
   U_o = v_get(MyGeom->NBSOMM);
   U_o = v_zero(U_o);
   /* ---------------------------------------------------------------------- */

#ifdef HAVE_VOGLE
   if (strcmp(MyParams->graph_interactiv2Dplots_params.ENGINE , "VOGLE") == 0)
   {
      int NX = MyParams->graph_interactiv2Dplots_params.NX;
      int NY = MyParams->graph_interactiv2Dplots_params.NY;

      char *driver        = MyParams->graph_interactiv2Dplots_params.DRIVER;

      int window_size_x = MyParams->graph_interactiv2Dplots_params.SIZE_WINDOW_X;
      int window_size_y = MyParams->graph_interactiv2Dplots_params.SIZE_WINDOW_Y;

      if (strcmp(MyParams->graph_interactiv2Dplots_params.GRAPHICS_TYPE[1][1], "SURFACE") == 0)
      {
          graphics2D_vogle_initialize(driver, window_size_x, window_size_y);

          graphics2D_vogle_store_surfacedata(MyGeom->NSELMT, MyGeom->XYSOMM);
          graphics2D_vogle_store_plotdata(U_o);

          graphics2D_vogle_plotsurface_start();
      }
      else
      {
         graphics1D_vopl_initialize(driver, window_size_x, window_size_y, 1, 1, VOPL_CONTOURS);
         graphics1D_vopl_title("SOLUTION");
         graphics1D_vopl_window(1, 1, MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].X_MIN,
                                    MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].X_MAX,
                                    MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].Y_MIN,
                                    MyParams->graph_interactiv2Dplots_params.WINDOW[1][1].Y_MAX);

         graphics1D_vopl_nblevels(1, 1, MyParams->graph_interactiv2Dplots_params.NB_LEVELS[1][1]);
         graphics1D_vopl_levels(1, 1, MyParams->graph_interactiv2Dplots_params.LEVELS[1][1]);
         graphics1D_vopl_contourmeshdata(1, 1, MyGeom->NSELMT, MyGeom->XYSOMM);
         graphics1D_vopl_contourplotdata(1, 1, U_o);

         graphics1D_vopl_plotcontours_start();
      }
   }
#endif

   /* Assemblage Matrices before the time stepping */
   MASS   = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
   MASS   = assemblage2D_matrix_Mass  (MyElt, MyGeom, MASS);

   RIGID1 = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM, 10);
   RIGID1 = assemblage2D_matrix_Stiff1(MyElt, MyGeom, RIGID1);

   /* HEAT EQUATION : A = MASSE + v.DT.RIGID1   */
   A   = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM, 10);
   sp_zero(A);

   sp_mltadd(A, MASS  , 1.0                           , A);
   sp_mltadd(A, RIGID1, DT*MyParams->phys_params.kappa, A);

   /* On compactifie A */
   sp_compact(A, 0.0);

   /* On sauvegarde A dans A_OLD pour les CL sur le 2nd membre */
   A_REF = sp_copy(A);

   /* on prend on compte les CL pour transformer A */
   transform2D_matrix_with_bc(MyElt, MyGeom, MyBC, A);

   /* et on factorise la matrice A dans LLT */
   LLT = sp_copy(A);
   spCHfactor(LLT);

   /* -------------------- TIME SCHEME ------------------------ */
   U_nm1   = v_get(MyGeom->NBSOMM);
   U_n     = v_get(MyGeom->NBSOMM);

   U_nm1   = v_copy(U_o, U_nm1);

   RHS            = v_get(MyGeom->NBSOMM);
   RHS_FUN        = v_get(MyGeom->NBSOMM);
   RHS_TIME_DISCR = v_get(MyGeom->NBSOMM);
   RHS_UUx        = v_get(MyGeom->NBSOMM);

   /* ---- We have to compute the RHS at the first time step -- */

   for (k=1; k<=IT_TPS_MAX; k++)
   {
      printf("k = %d\n", k);

      IT_TPS = k;
      TPS    = k*MyParams->time_params.DT;

      /* -- 2nd membre at time TPS ---------- */
      Rhs2D_setTps(MyRhsFun , TPS);
      Bc2D_setTps (MyBC, TPS);

      /* construct the right hand side */
      RHS_FUN  = assemblage2D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN);

      RHS_TIME_DISCR = sp_mv_mlt(MASS, U_nm1, RHS_TIME_DISCR);

      /* RHS_UUx  = assemblage2D_vector_abx(MyElt, MyGeom, U_nm1, U_nm1, RHS_UUx); */

      v_zero(RHS);
      RHS = v_mltadd(RHS, RHS_TIME_DISCR, 1.0, RHS);
      RHS = v_mltadd(RHS, RHS_FUN       ,  DT, RHS);
      /* RHS = v_mltadd(RHS, RHS_UUx       , -DT, RHS);*/

      /* -- prise en compte des CL ---------- */
      transform2D_vector_with_bc ( MyElt, MyGeom, MyBC, A_REF, RHS);

      /* -- Calcul de U_np1 ----------------- */
      U_n =  spCHsolve(LLT, RHS, U_n);


#ifdef HAVE_VOGLE
      if (strcmp(MyParams->graph_interactiv2Dplots_params.ENGINE , "VOGLE") == 0) /* -- Graphics with VOGLE library ---- */
      {
         if (strcmp(MyParams->graph_interactiv2Dplots_params.GRAPHICS_TYPE[1][1], "SURFACE") == 0)
         {
            char title[64];
            char legend[64];

            snprintf(title, 63, "SOL_VOGLE_%010.5lf", k*DT ); title[63] = '\0';
            graphics2D_vogle_title(title);

            graphics2D_vogle_store_plotdata(U_n);

            graphics2D_vogle_plotsurface_flush();
         }
         else
         {
            char title[64];

            snprintf(title, 63, "SOL_VOGLE_%010.5lf", k*DT ); title[63] = '\0';

            graphics1D_vopl_contourtransientbuildinfo(1,1);
            graphics1D_vopl_contourplotdata(1, 1, U_n);
            graphics1D_vopl_title(title);

            graphics1D_vopl_plotcontours_flush();
         }
      }
#endif

      v_copy (U_n, U_nm1);
      /* We can do a new time iteration now */
   }

   ELT_2D_FREE(elt_P1);
   ELT_2D_FREE(elt_P2);
   ELT_2D_FREE(elt_P3);

   ELT_2D_FREE(MyElt);
   GEOM_2D_FREE(MyGeom);
   BC_2D_FREE(MyBC);
   RHS_2D_FREE(MyRhsFun);

   V_FREE(U_n);
   V_FREE(U_nm1);
   V_FREE(U_o);

   Params_free(MyParams);

   /* -----------------------------------------------------------------------*/

   mem_info_file(stdout,0);  /* = mem_info(); */

   mem_info_file(stdout,MY_LIST1);
   mem_info_file(stdout,MY_LIST2);
   mem_info_file(stdout,MY_LIST3);
   mem_info_file(stdout,MY_LIST4);
   mem_info_file(stdout,MY_LIST5);
   mem_info_file(stdout,MY_LIST6);
   mem_info_file(stdout,MY_LIST7);

   pthread_exit(NULL);

   return EXIT_SUCCESS;

}
