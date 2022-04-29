
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
 *     Convection-Diffusion :
 *        -e.LAPLACIEN(u) + b.GRADIENT(u) = F 
 *        B.C. Dirichlet Type
 *
 */

#include <time.h>
#include <pthread.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_1D.h"
#include "MESCHACH_EF/INCLUDES/rhs_1D.h"
#include "MESCHACH_EF/INCLUDES/adv_1D.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_1D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_1D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_1D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_1D.h"

/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"

#ifdef HAVE_CPGPLOT
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_stationnary.h"
#endif

#ifdef HAVE_LIBSCIPLOT
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_stationnary.h"
#endif

#ifdef HAVE_VOGLE
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_stationnary.h"
#endif

#include "MESCHACH_EF/INCLUDES/all_params.h"

/* ------------------------------------------------------------------------ */

int main()
{
    ELT_1D   *MyElt, *elt_P1, *elt_P2, *elt_P3, *elt_H3, *elt_S3, *elt_S5;

    GEOM_1D  *MyGeom;

    BC_1D    *MyBC;

    RHS_1D   *MyRhsFun;
    ADV_1D   *MyAdvFun;

    VEC      *SOL;

    PARAMS   *MyParams;

    clock_t  t1;
    clock_t  t2;

    /* memory informations */

    mem_info_on(TRUE);

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

    /* LECTURE  IN  INPUT FILE "PDE_EXAMPLE_1D.json" */

    MyParams = Params_setup_from_file("/Users/xavier/DEVELOPMENT/MESCHACH_WORK/DRIVERS_C/1D/PDE_EXAMPLE_1D.json");

    Params_set_staticparam(MyParams, 0);

    elt_P1 = elt1D_get("P1");
    elt_P2 = elt1D_get("P2");
    elt_P3 = elt1D_get("P3");
    elt_H3 = elt1D_get("H3");
    elt_S3 = elt1D_get("S3");
    elt_S5 = elt1D_get("S5");

    /* Element type */
    MyElt = elt1D_get(MyParams->ef_params.name_ef);

    /* Geometry */
    MyGeom = Geom1D_setup_from_params(MyElt, MyParams);

    /* Boundary conditions */
    MyBC = Bc1D_setup_from_params(MyParams);

    Geom1D_check_with_boundaryconditions(MyGeom, MyBC, AXEe_X);
    Geom1D_foutput(stdout, MyGeom);

    /* Right Hand Side */
    MyRhsFun = Rhs1D_setup_from_params(MyParams);

    /* Advection */
    MyAdvFun = Adv1D_setup_from_params(MyParams);


    t1 = clock();
    t2 = clock();

    printf("total time = %f \n", 1.0 * (t2 - t1) / CLOCKS_PER_SEC);
    /* ---------------------------------------------------------------------- */

    if (strcmp(MyParams->problem, "LAPLACIAN") == 0)
    {
        SOL = solve1D_laplacian(MyElt, MyGeom, MyBC, MyRhsFun);
    }
    else
    if (strcmp(MyParams->problem, "BILAPLACIAN") == 0)
    {
        SOL = solve1D_bilaplacian(MyElt, MyGeom, MyBC, MyRhsFun);
    }
    else
    if (strcmp(MyParams->problem, "CONVECTION-DIFFUSION") == 0)
    {
        SOL = solve1D_convection_diffusion(MyElt, MyGeom, MyBC, MyRhsFun, MyAdvFun);
    }
    else
    if (strcmp(MyParams->problem, "HELMHOLZ") == 0)
    {
        SOL = solve1D_helmholz(MyElt, MyGeom, MyBC, MyRhsFun);
    }
    else
    if (strcmp(MyParams->problem, "BURGERS") == 0)
    {
        SOL = solve1D_burgers(MyElt, MyGeom, MyBC, MyRhsFun);
    }
    else
    {
        printf("Problem \"%s\" not yet implemented", MyParams->problem);
        exit(EXIT_FAILURE);
    }

    /* ---------------------------------------------------------------------- */
    t2 = clock();

    printf("total time = %f \n", 1.0 * (t2 - t1) / CLOCKS_PER_SEC);

    /* ----------------------- graphics output ------------------------------ */

    {
        VEC            *DATA_PLOT = get_vector_for_plotting_1D(MyElt, MyGeom, SOL);
        GEOM_1D        *GeomP1 = Geom1D_getP1geom_from(MyElt, MyGeom);

        graphics1D("gnuplot", MyElt, MyGeom, SOL, "SolApproch1D.dat");
        //graphics1D_gnuplot_script("SolApproch1D.dat", NULL, NULL, MyParams->resol_params.nb_steps, MyParams->misc_params.iter_file);

        v_output(DATA_PLOT);
        /*
         * graphics1D("silo", MyElt, MyGeom, SOL, "SolApproch");
         */

#ifdef HAVE_VOGLE
        if (MyParams->graph_params.VOGLE == 1)
        {
            graphics1D_vopl_initialize(MyParams->graph_interactiv1Dplots_params.DRIVER,
                MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_X,
                MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_Y, 1, 1, VOPL_CURVE);
            graphics1D_vopl_title("SOLUTION");
            graphics1D_vopl_legend(1, 1, "SOLUTION");
            graphics1D_vopl_labelsize(1, 1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
            graphics1D_vopl_window(1, 1, MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].X_MIN,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].X_MAX,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].Y_MIN,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].Y_MAX);
            graphics1D_vopl_setbox(1, 1, 1);
            graphics1D_vopl_curvedata(1, 1, VOGLEe_COLOR_BLUE, GeomP1->XSOMM, DATA_PLOT);

            graphics1D_vopl_plotcurve();
        }
#endif

#ifdef HAVE_CPGPLOT
        if (MyParams->graph_params.PGPLOT == 1)
        {
            graphics1D_cpgplot_initialize(MyParams->graph_interactiv1Dplots_params.DRIVER,
                MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_X,
                MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_Y, 1, 1);
            graphics1D_cpgplot_title("SOLUTION");
            graphics1D_cpgplot_legend(1, 1, "SOLUTION");
            graphics1D_cpgplot_labelsize(1, 1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
            graphics1D_cpgplot_window(1, 1,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].X_MIN,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].X_MAX,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].Y_MIN,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].Y_MAX);
            graphics1D_cpgplot_setbox(1, 1, 1);
            graphics1D_cpgplot_curvedata(1, 1, PGPLOTe_COLOR_BLUE1, GeomP1->XSOMM, DATA_PLOT);

            graphics1D_cpgplot_plotcurve();
        }
#endif

#ifdef HAVE_LIBSCIPLOT
        if (MyParams->graph_params.LIBSCIPLOT == 1)
        {
            graphics1D_libsciplot_initialize(MyParams->graph_interactiv1Dplots_params.DRIVER,
                MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_X,
                MyParams->graph_interactiv1Dplots_params.SIZE_WINDOW_Y, 1, 1);
            graphics1D_libsciplot_title("SOLUTION");
            graphics1D_libsciplot_bgcolor(LIBSCIPLOTe_GREEN);
            graphics1D_libsciplot_legend(1, 1, "SOLUTION");
            graphics1D_libsciplot_framecolor(1, 1, LIBSCIPLOTe_COLOR_RED);
            graphics1D_libsciplot_labelsize(1, 1, MyParams->graph_interactiv1Dplots_params.SIZE_LABELS);
            graphics1D_libsciplot_window(1, 1,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].X_MIN,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].X_MAX,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].Y_MIN,
                MyParams->graph_interactiv1Dplots_params.WINDOW[1][1].Y_MAX);
            graphics1D_libsciplot_setbox(1, 1, 1);
            graphics1D_libsciplot_curvedata(1, 1, LIBSCIPLOTe_COLOR_RED, GeomP1->XSOMM, DATA_PLOT);

            graphics1D_libsciplot_plotcurve();

            graphics1D_libsciplot_finalize();
        }
#endif

        V_FREE(DATA_PLOT);
        GEOM_1D_FREE(GeomP1);
    }

	 /* ----------------------- check-------- ------------------------------ */

    if (MyParams->exactsol_params.exact_sol[0] >= 0)
    {
        VEC *vec_tmp   = v_get(SOL->dim);
        VEC* vec_exact = v_get(SOL->dim);
        FUN_1D *fun_exact = Fun1D_get();
        Fun1D_setCFunction(fun_exact,  ExSol1D[MyParams->exactsol_params.exact_sol[0]]);
       
        vec_exact = build_vec_from_function1D(MyElt, MyGeom, fun_exact, NULL, vec_exact);
       
        double diff2 = v_norm2(v_sub(SOL, vec_exact, vec_tmp));
        fprintf(stdout, "\ndiff solexacte-solapprochee = %le\n", diff2);
       
       v_foutput(stdout, SOL);
       v_foutput(stdout, vec_exact);
       
    }

    /* --------------------------------------------------------------------- */

    ELT_1D_FREE(elt_P1);
    ELT_1D_FREE(elt_P2);
    ELT_1D_FREE(elt_P3);
    ELT_1D_FREE(elt_H3);
    ELT_1D_FREE(elt_S3);
    ELT_1D_FREE(elt_S5);

    ELT_1D_FREE(MyElt);

    GEOM_1D_FREE(MyGeom);
    BC_1D_FREE(MyBC);
    RHS_1D_FREE(MyRhsFun);
    ADV_1D_FREE(MyAdvFun);

    V_FREE(SOL);

    PARAMS_FREE(MyParams);

    /* ----------------------------------------------------------------------- */

    mem_info_file(stdout, 0);    /* = mem_info(); */

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
