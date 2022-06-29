
/*
 *
 * --- Numerical Resolution of PDE 1D ---
 *
 *     examples :
 *
 *     Laplacian :
 *        -e.LAPLACIEN(u) = F
 *        B.C. Dirichlet Type
 *
 *     BiLaplacian :
 *        -e.BI-LAPLACIEN(u) = F
 *        B.C. Dirichlet Type
 *
 *
 */


#include <time.h>

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
#include "MESCHACH_EF/INCLUDES/assemblage_objects_1D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_1D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"

#include "MESCHACH_EF/INCLUDES/miscellaneous_1D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#ifdef HAVE_CPGPLOT
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_transient.h"
#endif

#ifdef HAVE_VOGLE
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_transient.h"
#endif

#ifdef HAVE_LIBSCIPLOT
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_stationnary.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_transient.h"
#endif

/* ------------------------------------------------------------------------ */

int  main(void)
{

   ELT_1D   *MyElt ;

   GEOM_1D  *MyGeom;

   BC_1D    *MyBC;

   RHS_1D   *MyRhsFun;

   FUN_1D   *MyCiFun;
   FUN_1D   *MyDCiFun; /* for Hermite ef */

   VEC      *U_o , *U_nm1, *U_nm2, *U_n ;

   VEC      *ABSCISSAS1, *ABSCISSAS2, *ABSCISSAS3, *ABSCISSAS4;
   VEC      *U_graph1  , *U_graph2,   *U_graph3  , *U_graph4;

   PARAMS   *MyParams;

   double TPS;
   int    IT_TPS;
   int    IT_TPS_MAX;
   int    i,k;
   double DT;
   double DX;
   int    NX;

   SPMAT *MASS;
   SPMAT *RIGID1;
   SPMAT *RIGID2;

   SPMAT *A;
   SPMAT *A_REF;

   SPMAT *LLT;

   VEC *RHS;
   VEC *RHS_FUN;
   VEC *RHS_UUx;
   VEC *RHS_TIME_DISCR;
   VEC *RHS_LIN;
   VEC *RHS_LIN1;
   VEC *RHS_LIN2;

   VEC *U_tmp1;
   VEC *U_tmp2;
   VEC *U_tmp3;

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

   mem_info_file(stdout, 0);
   mem_info_file(stdout, MY_LIST1);
   mem_info_file(stdout, MY_LIST2);
   mem_info_file(stdout, MY_LIST3);
   mem_info_file(stdout, MY_LIST4);
   mem_info_file(stdout, MY_LIST5);
   mem_info_file(stdout, MY_LIST6);
   mem_info_file(stdout, MY_LIST7);

   /* LECTURE FROM INPUT FILE  */

   MyParams = Params_setup_from_file("DRIVERS_C/1D_TRANSIENT/PDE_EXAMPLE_TR_1D.json");

   Params_set_staticparam(MyParams, 0);


   MyElt  = elt1D_get(MyParams->ef_params.name_ef);

   /* Geometry */
   MyGeom = Geom1D_setup_from_params(MyElt, MyParams);

   /* Boundary conditions */
   MyBC = Bc1D_setup_from_params(MyParams);

   Geom1D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X);
   Geom1D_foutput(stdout, MyGeom);

   /* Right Hand Side */
   MyRhsFun = Rhs1D_setup_from_params(MyParams);

   /* Initial Condition */
   MyCiFun = Fun1D_get();
   Fun1D_setLUAFunctionTransient(MyCiFun, MyParams->ini_params.ini[0].fundef);
   /* Initial Condition for Hermite EF */
   MyDCiFun = Fun1D_get();
   Fun1D_setLUAFunctionTransient(MyDCiFun, MyParams->ini_params.dini[0].fundef);


   /* ---------------------------------------------------------------------- */

   TPS = 0.0;
   IT_TPS_MAX = floor(MyParams->time_params.TPS_FIN/MyParams->time_params.DT) ;
   if ((MyParams->time_params.TPS_FIN/MyParams->time_params.DT - IT_TPS_MAX)>0.5) IT_TPS_MAX += 1;
   printf("IT_TPS_MAX = %d \n",IT_TPS_MAX);

   DT = MyParams->time_params.DT;
   NX = MyParams->geom_params.nx;
   DX = (MyGeom->X_RIGHT- MyGeom->X_LEFT) / NX ;

   /* --------------------------------------------------------------------- */

   ABSCISSAS1 = v_get( MyGeom->NBSOMM );
   U_graph1   = v_get( MyGeom->NBSOMM );

   ABSCISSAS2 = v_get( IT_TPS_MAX+1 );
   U_graph2   = v_get( IT_TPS_MAX+1 );

   ABSCISSAS3 = v_get( IT_TPS_MAX+1 );
   U_graph3   = v_get( IT_TPS_MAX+1 );

   ABSCISSAS4 = v_get( IT_TPS_MAX+1 );
   U_graph4   = v_get( IT_TPS_MAX+1 );


   U_o   = v_get( MyGeom->NBSOMM );
   U_nm2 = v_get( MyGeom->NBSOMM );
   U_nm1 = v_get( MyGeom->NBSOMM );
   U_n   = v_get( MyGeom->NBSOMM );

   U_tmp1 = v_get( MyGeom->NBSOMM );
   U_tmp2 = v_get( MyGeom->NBSOMM );
   U_tmp3 = v_get( MyGeom->NBSOMM );

   ABSCISSAS1 = Geom1D_getBaseMesh( MyElt ,  MyGeom );

   for (k=0; k<=IT_TPS_MAX; k++)
   {
      ABSCISSAS2->ve[k] = k*DT;
   }

   U_o = build_vec_ef_from_function1Dtransient(MyElt, MyGeom, MyCiFun, MyDCiFun, U_o, 0.0);

#ifdef HAVE_CPGPLOT
   if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "PGPLOT") == 0)
   {
      PARAMS_graphics_interactiv1D_plots *PlotParams = &(MyParams->graph_interactiv1Dplots_params);

      int NX = MyParams->graph_interactiv1Dplots_params.NX;
      int NY = MyParams->graph_interactiv1Dplots_params.NY;

      char *driver        = MyParams->graph_interactiv1Dplots_params.DRIVER;

      int window_size_x = MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_X;
      int window_size_y = MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_Y;


      graphics1D_cpgplot_initialize(driver, window_size_x, window_size_y, NX,NY);

      U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_o, U_graph1);
      U_graph2->ve[0] = v_norm2(U_graph1)*sqrt(DX);
      U_graph3->ve[0] = 0.0;

      graphics1D_cpgplot_title("Initial Condition");

      graphics1D_cpgplot_labelsize(1,1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_cpgplot_labelsize(1,2, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_cpgplot_labelsize(2,1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_cpgplot_labelsize(2,2, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);

      graphics1D_cpgplot_legend(1,1, "Initial Condition");
      graphics1D_cpgplot_legend(1,2, PlotParams->LABEL[1][2].LEGEND);
      graphics1D_cpgplot_legend(2,1, PlotParams->LABEL[2][1].LEGEND);
      graphics1D_cpgplot_legend(2,2, PlotParams->LABEL[2][2].LEGEND);

      graphics1D_cpgplot_window(1,1,  PlotParams->WINDOW[1][1].X_MIN, PlotParams->WINDOW[1][1].X_MAX, PlotParams->WINDOW[1][1].Y_MIN, PlotParams->WINDOW[1][1].Y_MAX);
      graphics1D_cpgplot_window(1,2,  PlotParams->WINDOW[1][2].X_MIN, PlotParams->WINDOW[1][2].X_MAX, PlotParams->WINDOW[1][2].Y_MIN, PlotParams->WINDOW[1][2].Y_MAX);
      graphics1D_cpgplot_window(2,1,  PlotParams->WINDOW[2][1].X_MIN, PlotParams->WINDOW[2][1].X_MAX, PlotParams->WINDOW[2][1].Y_MIN, PlotParams->WINDOW[2][1].Y_MAX);
      graphics1D_cpgplot_window(2,2,  PlotParams->WINDOW[2][2].X_MIN, PlotParams->WINDOW[2][2].X_MAX, PlotParams->WINDOW[2][2].Y_MIN, PlotParams->WINDOW[2][2].Y_MAX);

      graphics1D_cpgplot_setbox(1,1, 1);
      graphics1D_cpgplot_setbox(1,2, 1);
      graphics1D_cpgplot_setbox(2,1, 1);
      graphics1D_cpgplot_setbox(2,2, 1);

      graphics1D_cpgplot_endmarker(1,1, PGPLOTe_MARKER_UNDEFINED);
      graphics1D_cpgplot_endmarker(1,2, PGPLOTe_MARKER_UNDEFINED);
      graphics1D_cpgplot_endmarker(2,1, PGPLOTe_MARKER_UNDEFINED);
      graphics1D_cpgplot_endmarker(2,2, PGPLOTe_MARKER_MULTIPLICATION);

      graphics1D_cpgplot_curvedata1(1,1, PGPLOTe_COLOR_GREEN , ABSCISSAS1, U_graph1, -1,-1 );
      graphics1D_cpgplot_curvedata1(2,1, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, 1 );
      graphics1D_cpgplot_curvedata1(1,2, PGPLOTe_COLOR_RED   , ABSCISSAS2, U_graph2,  0, 1 );
      graphics1D_cpgplot_curvedata1(2,2, PGPLOTe_COLOR_RED   , U_graph2  , U_graph3,  0, 1 );

      graphics1D_cpgplot_plotcurves_start();
   }
#endif

#ifdef HAVE_VOGLE
   if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "VOGLE") == 0)
   {
      PARAMS_graphics_interactiv1D_plots *params = &(MyParams->graph_interactiv1Dplots_params);

      int NX = MyParams->graph_interactiv1Dplots_params.NX;
      int NY = MyParams->graph_interactiv1Dplots_params.NY;

      char *driver        = MyParams->graph_interactiv1Dplots_params.DRIVER;

      int window_size_x = MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_X;
      int window_size_y = MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_Y;

      graphics1D_vopl_initialize(driver, window_size_x, window_size_y, NX,NY, VOPL_CURVE);

      U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_o, U_graph1);
      U_graph2->ve[0] = v_norm2(U_graph1)*sqrt(DX);
      U_graph3->ve[0] = 0.0;

      graphics1D_vopl_title("Initial Condition");

      graphics1D_vopl_labelsize(1,1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_vopl_labelsize(1,2, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_vopl_labelsize(2,1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_vopl_labelsize(2,2, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);

      graphics1D_vopl_legend(1,1, "Initial Condition");
      graphics1D_vopl_legend(1,2, params->LABEL[1][2].LEGEND);
      graphics1D_vopl_legend(2,1, params->LABEL[2][1].LEGEND);
      graphics1D_vopl_legend(2,2, params->LABEL[2][2].LEGEND);

      graphics1D_vopl_window(1,1,  params->WINDOW[1][1].X_MIN, params->WINDOW[1][1].X_MAX, params->WINDOW[1][1].Y_MIN, params->WINDOW[1][1].Y_MAX);
      graphics1D_vopl_window(1,2,  params->WINDOW[1][2].X_MIN, params->WINDOW[1][2].X_MAX, params->WINDOW[1][2].Y_MIN, params->WINDOW[1][2].Y_MAX);
      graphics1D_vopl_window(2,1,  params->WINDOW[2][1].X_MIN, params->WINDOW[2][1].X_MAX, params->WINDOW[2][1].Y_MIN, params->WINDOW[2][1].Y_MAX);
      graphics1D_vopl_window(2,2,  params->WINDOW[2][2].X_MIN, params->WINDOW[2][2].X_MAX, params->WINDOW[2][2].Y_MIN, params->WINDOW[2][2].Y_MAX);


      graphics1D_vopl_setbox( 1,1, 1);
      graphics1D_vopl_setbox( 1,2, 1);
      graphics1D_vopl_setbox( 2,1, 1);
      graphics1D_vopl_setbox( 2,2, 1);

      graphics1D_vopl_endmarker(1,1, VOGLEe_NO_MARKER);
      graphics1D_vopl_endmarker(1,2, VOGLEe_NO_MARKER);
      graphics1D_vopl_endmarker(2,1, VOGLEe_NO_MARKER);
      graphics1D_vopl_endmarker(2,2, VOGLEe_MARKER_CHRISMAS);

      graphics1D_vopl_curvedata1(1,1, VOGLEe_COLOR_GREEN, ABSCISSAS1, U_graph1, -1,-1);
      graphics1D_vopl_curvedata1(2,1, VOGLEe_COLOR_RED  , ABSCISSAS2, U_graph2,  0, 1);
      graphics1D_vopl_curvedata1(1,2, VOGLEe_COLOR_RED  , ABSCISSAS2, U_graph2,  0, 1);
      graphics1D_vopl_curvedata1(2,2, VOGLEe_COLOR_RED  , U_graph2  , U_graph3,  0, 1);

      graphics1D_vopl_plotcurves_start();
   }
#endif

#ifdef HAVE_LIBSCIPLOT
   if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "LIBSCIPLOT") == 0)
   {
      PARAMS_graphics_interactiv1D_plots *params = &(MyParams->graph_interactiv1Dplots_params);

      int NX = MyParams->graph_interactiv1Dplots_params.NX;
      int NY = MyParams->graph_interactiv1Dplots_params.NY;

      char *driver        = MyParams->graph_interactiv1Dplots_params.DRIVER;

      int window_size_x = MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_X;
      int window_size_y = MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_Y;

      graphics1D_libsciplot_initialize(driver, window_size_x, window_size_y, NX, NY);

      graphics1D_libsciplot_bgcolor(LIBSCIPLOTe_WHITE);
      graphics1D_libsciplot_title("Initial Condition");

      U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_o, U_graph1);
      U_graph2->ve[0] = v_norm2(U_graph1)*sqrt(DX);
      U_graph3->ve[0] = 0.0;

      graphics1D_libsciplot_labelsize(1,1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_libsciplot_labelsize(1,2, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_libsciplot_labelsize(2,1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
      graphics1D_libsciplot_labelsize(2,2, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);

      graphics1D_libsciplot_legendcolor(1,1, LIBSCIPLOTe_COLOR_BLUE);
      graphics1D_libsciplot_legendcolor(1,2, LIBSCIPLOTe_COLOR_BLUE);
      graphics1D_libsciplot_legendcolor(2,1, LIBSCIPLOTe_COLOR_BLUE);
      graphics1D_libsciplot_legendcolor(2,2, LIBSCIPLOTe_COLOR_BLUE);

      graphics1D_libsciplot_framecolor(1,1, LIBSCIPLOTe_COLOR_BLUE);
      graphics1D_libsciplot_framecolor(1,2, LIBSCIPLOTe_COLOR_BLUE);
      graphics1D_libsciplot_framecolor(2,1, LIBSCIPLOTe_COLOR_BLUE);
      graphics1D_libsciplot_framecolor(2,2, LIBSCIPLOTe_COLOR_BLUE);

      graphics1D_libsciplot_legend(1,1, "Initial Condition");
      graphics1D_libsciplot_legend(1,2, params->LABEL[1][2].LEGEND);
      graphics1D_libsciplot_legend(2,1, params->LABEL[2][1].LEGEND);
      graphics1D_libsciplot_legend(2,2, params->LABEL[2][2].LEGEND);

      graphics1D_libsciplot_window(1,1,  params->WINDOW[1][1].X_MIN, params->WINDOW[1][1].X_MAX, params->WINDOW[1][1].Y_MIN, params->WINDOW[1][1].Y_MAX);
      graphics1D_libsciplot_window(1,2,  params->WINDOW[1][2].X_MIN, params->WINDOW[1][2].X_MAX, params->WINDOW[1][2].Y_MIN, params->WINDOW[1][2].Y_MAX);
      graphics1D_libsciplot_window(2,1,  params->WINDOW[2][1].X_MIN, params->WINDOW[2][1].X_MAX, params->WINDOW[2][1].Y_MIN, params->WINDOW[2][1].Y_MAX);
      graphics1D_libsciplot_window(2,2,  params->WINDOW[2][2].X_MIN, params->WINDOW[2][2].X_MAX, params->WINDOW[2][2].Y_MIN, params->WINDOW[2][2].Y_MAX);


      graphics1D_libsciplot_setbox( 1,1, 1);
      graphics1D_libsciplot_setbox( 1,2, 1);
      graphics1D_libsciplot_setbox( 2,1, 1);
      graphics1D_libsciplot_setbox( 2,2, 1);

      graphics1D_libsciplot_endmarker(1,1, LIBSCIPLOTe_SYMBOL_VOID);
      graphics1D_libsciplot_endmarker(1,2, LIBSCIPLOTe_SYMBOL_VOID);
      graphics1D_libsciplot_endmarker(2,1, LIBSCIPLOTe_SYMBOL_VOID);
      graphics1D_libsciplot_endmarker(2,2, LIBSCIPLOTe_SYMBOL_SQUARE);

      graphics1D_libsciplot_curvedata1(1,1, LIBSCIPLOTe_COLOR_BLUE , ABSCISSAS1, U_graph1, -1,-1);
      graphics1D_libsciplot_curvedata1(2,1, LIBSCIPLOTe_COLOR_RED  , ABSCISSAS2, U_graph2,  0, 1);
      graphics1D_libsciplot_curvedata1(1,2, LIBSCIPLOTe_COLOR_RED  , ABSCISSAS2, U_graph2,  0, 1);
      graphics1D_libsciplot_curvedata1(2,2, LIBSCIPLOTe_COLOR_RED  , U_graph2  , U_graph3,  0, 1);

      graphics1D_libsciplot_plotcurves_start();
   }
#endif


   /* Assemblage Matrices before the time stepping */
   MASS   = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
   MASS   = assemblage1D_matrix_Mass  (MyElt, MyGeom, MASS  );

   RIGID1 = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
   RIGID1 = assemblage1D_matrix_Stiff1(MyElt, MyGeom, RIGID1);

   RIGID2 = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
   RIGID2 = assemblage1D_matrix_Stiff2(MyElt, MyGeom, RIGID2);


   /* HEAT EQUATION : A = MASSE + v.DT.RIGID1   */
   /* K-S  EQUATION : A = MASSE + v.DT.RIGID2   */
   A   = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
   sp_zero(A);

   sp_mltadd(A, MASS  , 1.0                             , A);
   sp_mltadd(A, RIGID1, DT*MyParams->phys_params.kappa  , A);
   sp_mltadd(A, RIGID2, DT*MyParams->phys_params.epsilon, A);

   /* On compactifie A */
   sp_compact(A, 0.0);

   /* On sauvegarde A dans A_OLD pour les CL sur le 2nd membre */
   A_REF = sp_copy(A);

   /* on prend on compte les CL pour transformer A */
   transform1D_matrix_with_bc(MyElt, MyGeom, MyBC, A);

   /* et on factorise la matrice A dans LLT */
   LLT = sp_copy(A); spCHfactor(LLT);

   /* -------------------- TIME SCHEME ------------------------ */
   U_nm2   = v_get(MyGeom->NBSOMM);
   U_nm1   = v_get(MyGeom->NBSOMM);
   U_n     = v_get(MyGeom->NBSOMM);

   U_nm1   = v_copy(U_o, U_nm1);

   RHS            = v_get(MyGeom->NBSOMM);
   RHS_FUN        = v_get(MyGeom->NBSOMM);
   RHS_TIME_DISCR = v_get(MyGeom->NBSOMM);
   RHS_UUx        = v_get(MyGeom->NBSOMM);
   RHS_LIN        = v_get(MyGeom->NBSOMM);
   RHS_LIN1       = v_get(MyGeom->NBSOMM);
   RHS_LIN2       = v_get(MyGeom->NBSOMM);

   /* ---- We have to compute the RHS at the first time step -- */

   for (k=1; k<=1; k++)
   {
      IT_TPS = k;
      TPS    = k*MyParams->time_params.DT;

      /* -- 2nd membre at time TPS ---------- */
      Rhs1D_setTps(MyRhsFun , TPS);
      Bc1D_setTps (MyBC, TPS);

      /* construct the right hand side */
      RHS_FUN  = assemblage1D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN);

      RHS_TIME_DISCR = sp_mv_mlt( MASS, U_nm1, RHS_TIME_DISCR );

      RHS_UUx  = assemblage1D_vector_abx(MyElt, MyGeom, U_nm1, U_nm1, RHS_UUx);

      v_zero(RHS);
      RHS = v_mltadd(RHS, RHS_TIME_DISCR, 1.0, RHS);
      RHS = v_mltadd(RHS, RHS_UUx       , -DT, RHS);
      RHS = v_mltadd(RHS, RHS_FUN       ,  DT, RHS);


      /* -- prise en compte des CL ---------- */
      transform1D_vector_with_bc (MyElt, MyGeom, MyBC, A_REF, RHS);

      /* -- Calcul de U_np1 ----------------- */
      U_n =  spCHsolve(LLT, RHS, U_n);

      /* plots */
      U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_n, U_graph1);
      U_graph2->ve[k] = v_norm2(U_graph1)*sqrt(DX);
      U_graph3->ve[k] = ( U_graph2->ve[k] - U_graph2->ve[k-1] ) / DT;

      if ( (IT_TPS % MyParams->misc_params.itgraph) == 0 )
      {
         char title[64];
         char legend[64];

#ifdef HAVE_CPGPLOT
         if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "PGPLOT") == 0) /* -- Graphics with CPGPLOT library ---- */
         {
            snprintf(title, 63, "SOL_PGPLOT_%010.5lf", k*DT ); title[63] = '\0';
            graphics1D_cpgplot_title(title);

            snprintf(legend, 63, "U(x,t)  ( t = %010.5lf )", k*DT ); legend[63] = '\0';
            graphics1D_cpgplot_legend(1,1, legend);

            graphics1D_cpgplot_curvedata1(1,1, PGPLOTe_COLOR_RED   , ABSCISSAS1, U_graph1, -1,-1 );
            graphics1D_cpgplot_curvedata1(2,1, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2,  0, k );
            graphics1D_cpgplot_curvedata1(1,2, PGPLOTe_COLOR_BLUE1 , ABSCISSAS2, U_graph2,  0, k );
            graphics1D_cpgplot_curvedata1(2,2, PGPLOTe_COLOR_BLUE1 , U_graph2  , U_graph3,  0, k );

            graphics1D_cpgplot_plotcurves_flush();
         }
#endif

#ifdef HAVE_VOGLE
         if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "VOGLE") == 0) /* -- Graphics with VOGLE library ---- */
         {
            snprintf(title, 63, "SOL_VOGLE_%010.5lf", k*DT ); title[63] = '\0';
            graphics1D_vopl_title(title);

            snprintf(legend, 63, "U(x,t)  ( t = %010.5lf )", k*DT ); legend[63] = '\0';
            graphics1D_vopl_legend(1,1, legend);

            graphics1D_vopl_curvedata1(1,1, VOGLEe_COLOR_RED  , ABSCISSAS1, U_graph1, -1,-1);
            graphics1D_vopl_curvedata1(2,1, VOGLEe_COLOR_BLUE , ABSCISSAS2, U_graph2,  0, k);
            graphics1D_vopl_curvedata1(1,2, VOGLEe_COLOR_BLUE , ABSCISSAS2, U_graph2,  0, k);
            graphics1D_vopl_curvedata1(2,2, VOGLEe_COLOR_BLUE , U_graph2  , U_graph3,  0, k);

            graphics1D_vopl_plotcurves_flush();
         }
#endif

#ifdef HAVE_LIBSCIPLOT
         if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "LIBSCIPLOT") == 0) /* -- Graphics with LIBSCIPLOT library ---- */
         {
            snprintf(title, 63, "SOL_LIBSCIPLOT_%010.5lf", k*DT ); title[63] = '\0';
            graphics1D_libsciplot_title(title);

            snprintf(legend, 63, "U(x,t)  ( t = %010.5lf )", k*DT ); legend[63] = '\0';
            graphics1D_libsciplot_legend(1,1, legend);

            graphics1D_libsciplot_curvedata1(1,1, LIBSCIPLOTe_COLOR_RED  , ABSCISSAS1, U_graph1, -1,-1);
            graphics1D_libsciplot_curvedata1(2,1, LIBSCIPLOTe_COLOR_BLUE , ABSCISSAS2, U_graph2,  0, k);
            graphics1D_libsciplot_curvedata1(1,2, LIBSCIPLOTe_COLOR_BLUE , ABSCISSAS2, U_graph2,  0, k);
            graphics1D_libsciplot_curvedata1(2,2, LIBSCIPLOTe_COLOR_BLUE , U_graph2  , U_graph3,  0, k);

            graphics1D_libsciplot_plotcurves_flush();
         }
#endif
      }

      v_copy (U_nm1, U_nm2 );
      v_copy (U_n  , U_nm1 );
      /* We can do a new time iteration now */
   }

   /* TIME SCHEME 2nd ORDER :                       */
   /*                                               */
   /* dU/dt    -> 1.5*U_n - 2.0*U_nm1 + 0.5*U_nm2   */
   /* OpLIN(U) -> OpLIN(U_n + U_nm1)/2              */
   /* OpNLI(U) -> OpNLI( 2U_nm1 - U_nm2)            */

   sp_zero(A);

   sp_mltadd(A, MASS  , 1.5                                 , A);
   sp_mltadd(A, RIGID1, 0.5*DT*MyParams->phys_params.kappa  , A);
   sp_mltadd(A, RIGID2, 0.5*DT*MyParams->phys_params.epsilon, A);

   /* On compactifie A */
   sp_compact(A, 0.0);

   /* On sauvegarde A dans A_OLD pour les CL sur le 2nd membre */
   A_REF = sp_copy2(A, A_REF);

   /* on prend on compte les CL pour transformer A */
   transform1D_matrix_with_bc(MyElt, MyGeom, MyBC, A);

   /* et on factorise la matrice A dans LLT */
   LLT = sp_copy2(A, LLT); spCHfactor(LLT);

   for (k=2; k<=IT_TPS_MAX; k++)
   {
      IT_TPS = k;
      TPS    = k*MyParams->time_params.DT;

      /* -- 2nd membre at time TPS ---------- */
      Rhs1D_setTps(MyRhsFun, TPS);
      Bc1D_setTps (MyBC, TPS);

      /* construct the right hand side */
      RHS_FUN  = assemblage1D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN);

      U_tmp2 = v_zero(U_tmp2);
      U_tmp2 = v_mltadd(U_tmp2, U_nm1,  2.0, U_tmp2);
      U_tmp2 = v_mltadd(U_tmp2, U_nm2, -0.5, U_tmp2);
      RHS_TIME_DISCR = sp_mv_mlt(MASS, U_tmp2, RHS_TIME_DISCR);

      U_tmp1 = v_zero(U_tmp1);
      U_tmp1 = v_mltadd(U_tmp1, U_nm1,  2.0, U_tmp1);
      U_tmp1 = v_mltadd(U_tmp1, U_nm2, -1.0, U_tmp1);
      RHS_UUx  = assemblage1D_vector_abx(MyElt, MyGeom, U_tmp1, U_tmp1, RHS_UUx);

      U_tmp3 = v_zero(U_tmp3);
      U_tmp3 = v_mltadd( U_tmp3, U_nm1 ,  0.5 , U_tmp3 );
      RHS_LIN1 = sp_mv_mlt( RIGID1, U_tmp3, RHS_LIN1);
      sv_mlt( MyParams->phys_params.kappa,   RHS_LIN1 , RHS_LIN1 );
      RHS_LIN2 = sp_mv_mlt( RIGID2, U_tmp3, RHS_LIN2);
      sv_mlt( MyParams->phys_params.epsilon, RHS_LIN2 , RHS_LIN2 );
      /* finally */
      RHS_LIN  = v_add( RHS_LIN1 , RHS_LIN2 , RHS_LIN );



      v_zero(RHS);
      RHS = v_mltadd(RHS, RHS_TIME_DISCR, 1.0 , RHS);
      RHS = v_mltadd(RHS, RHS_FUN       ,  DT , RHS);
      RHS = v_mltadd(RHS, RHS_UUx       , -DT , RHS);
      RHS = v_mltadd(RHS, RHS_LIN       , -DT , RHS);

      /* -- prise en compte des CL ---------- */
      transform1D_vector_with_bc (MyElt, MyGeom, MyBC, A_REF, RHS);

      /* -- Calcul de U_np1 ----------------- */
      U_n =  spCHsolve(LLT, RHS, U_n);


      /* plots */
      U_graph1 = sp_mv_mlt(MyGeom->EF_to_WORLD, U_n, U_graph1);
      U_graph2->ve[k] = v_norm2(U_graph1)*sqrt(DX);
      U_graph3->ve[k] = ( U_graph2->ve[k] - U_graph2->ve[k-1] ) / DT;

      if ( (IT_TPS % MyParams->misc_params.itgraph) == 0 )
      {
         char title[64];
         char legend[64];

#ifdef HAVE_CPGPLOT
         if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "PGPLOT") == 0) /* -- Graphics with CPGPLOT library ---- */
         {
            snprintf(title, 63, "SOL_PGPLOT_%010.5lf", k*DT ); title[63] = '\0';
            graphics1D_cpgplot_title(title);

            snprintf(legend, 63, "U(x,t)  ( t = %010.5lf )", k*DT ); legend[63] = '\0';
            graphics1D_cpgplot_legend(1,1, legend);

            graphics1D_cpgplot_curvedata1(1, 1, PGPLOTe_COLOR_RED  , ABSCISSAS1, U_graph1, -1,-1);
            graphics1D_cpgplot_curvedata1(2, 1, PGPLOTe_COLOR_BLUE1, ABSCISSAS2, U_graph2,  0, k);
            graphics1D_cpgplot_curvedata1(1, 2, PGPLOTe_COLOR_BLUE1, ABSCISSAS2, U_graph2,  0, k);
            graphics1D_cpgplot_curvedata1(2, 2, PGPLOTe_COLOR_BLUE1, U_graph2  , U_graph3,  0, k);

            graphics1D_cpgplot_plotcurves_flush();
        }
#endif

#ifdef HAVE_VOGLE
         if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "VOGLE") == 0) /* -- Graphics with VOGLE library ---- */
         {
            snprintf(title, 63, "SOL_VOGLE_%010.5lf", k*DT ); title[63] = '\0';
            graphics1D_vopl_title(title);

            snprintf(legend, 64, "U(x,t)  ( t = %010.5lf )", k*DT ); legend[63] = '\0';
            graphics1D_vopl_legend(1,1, legend);

            graphics1D_vopl_curvedata1(1, 1, VOGLEe_COLOR_RED , ABSCISSAS1, U_graph1, -1,-1);
            graphics1D_vopl_curvedata1(2, 1, VOGLEe_COLOR_BLUE, ABSCISSAS2, U_graph2,  0, k);
            graphics1D_vopl_curvedata1(1, 2, VOGLEe_COLOR_BLUE, ABSCISSAS2, U_graph2,  0, k);
            graphics1D_vopl_curvedata1(2, 2, VOGLEe_COLOR_BLUE, U_graph2  , U_graph3,  0, k);

            graphics1D_vopl_plotcurves_flush();
         }
#endif

#ifdef HAVE_LIBSCIPLOT
         if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "LIBSCIPLOT") == 0) /* -- Graphics with LIBSCIPLOT library ---- */
         {
            snprintf(title, 63, "SOL_LIBSCIPLOT_%010.5lf", k*DT ); title[63] = '\0';
            graphics1D_libsciplot_title(title);

            snprintf(legend, 63, "U(x,t)  ( t = %010.5lf )", k*DT ); legend[63] = '\0';
            graphics1D_libsciplot_legend(1,1, legend);

            graphics1D_libsciplot_curvedata1(1, 1, LIBSCIPLOTe_COLOR_RED , ABSCISSAS1, U_graph1, -1,-1);
            graphics1D_libsciplot_curvedata1(2, 1, LIBSCIPLOTe_COLOR_BLUE, ABSCISSAS2, U_graph2,  0, k);
            graphics1D_libsciplot_curvedata1(1, 2, LIBSCIPLOTe_COLOR_BLUE, ABSCISSAS2, U_graph2,  0, k);
            graphics1D_libsciplot_curvedata1(2, 2, LIBSCIPLOTe_COLOR_BLUE, U_graph2  , U_graph3,  0, k);

            graphics1D_libsciplot_plotcurves_flush();
         }
#endif
      }

      v_copy (U_nm1, U_nm2 );
      v_copy (U_n  , U_nm1 );
      /* We can do a new time iteration now */
   }


   ELT_1D_FREE(MyElt);

   GEOM_1D_FREE(MyGeom);
   BC_1D_FREE(MyBC);

   RHS_1D_FREE(MyRhsFun);

   FUN_1D_FREE(MyCiFun);
   FUN_1D_FREE(MyDCiFun);

   Params_free(MyParams);

   SP_FREE(MASS);
   SP_FREE(RIGID1);
   SP_FREE(RIGID2);

   SP_FREE(A);
   SP_FREE(A_REF);
   SP_FREE(LLT);

   V_FREE(U_o);
   V_FREE(U_nm2);
   V_FREE(U_nm1);
   V_FREE(U_n);

   V_FREE(ABSCISSAS1);
   V_FREE(U_graph1);

   V_FREE(ABSCISSAS2);
   V_FREE(U_graph2);

   V_FREE(ABSCISSAS3);
   V_FREE(U_graph3);

   V_FREE(ABSCISSAS4);
   V_FREE(U_graph4);

   V_FREE(RHS);
   V_FREE(RHS_FUN);
   V_FREE(RHS_TIME_DISCR);
   V_FREE(RHS_UUx);
   V_FREE(RHS_LIN);
   V_FREE(RHS_LIN1);
   V_FREE(RHS_LIN2);

   V_FREE(U_tmp1);
   V_FREE(U_tmp2);
   V_FREE(U_tmp3);

   /* -----------------------------------------------------------------------*/

   mem_info_file(stdout, 0);  /* = mem_info(); */

   mem_info_file(stdout, MY_LIST1);
   mem_info_file(stdout, MY_LIST2);
   mem_info_file(stdout, MY_LIST3);
   mem_info_file(stdout, MY_LIST4);
   mem_info_file(stdout, MY_LIST5);
   mem_info_file(stdout, MY_LIST6);
   mem_info_file(stdout, MY_LIST7);

#ifdef HAVE_CPGPLOT
   if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "PGPLOT") == 0)
   {
      graphics1D_cpgplot_finalize_transient();
   }
#endif

#ifdef HAVE_VOGLE
   if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "VOGLE") == 0)
   {
      graphics1D_vopl_finalize_transient();
   }
#endif

#ifdef HAVE_LIBSCIPLOT
   if (strcmp(MyParams->graph_interactiv1Dplots_params.ENGINE , "LIBSCIPLOT") == 0)
   {
      graphics1D_libsciplot_finalize_transient();
   }
#endif

   pthread_exit(NULL);

   return EXIT_SUCCESS;
}
