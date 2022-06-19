#ifndef MEF_PARAMETERS_DATAS__H
#define MEF_PARAMETERS_DATAS__H

/** \file all_params_datas.h
 *
 * Contains the data-structures for setting-getting values of all parameters
 * + checking
 * 
 * -------------------------------------------------------------------- */

#include "MESCHACH_EF/INCLUDES/all_params.h"


#ifdef __cplusplus
extern "C"
{
#endif

   
typedef struct
{
   const char * p1;         /* the category of a parameter */
   const char * p2;         /* its field's name */

   int  type;               /* its type */
   
} TYPE_PARAMS0;

typedef struct
{
   const char * p1;         /* the category of a parameter */
   const char * p2;         /* its field's name */

   int  type;               /* its type */

   int  idx_min, idx_max;   /* to check array bounds */

} TYPE_PARAMS1;

typedef struct
{
   const char * p1;          /* the category of a parameter */
   const char * p2;          /* its field's name */

   int  type;                /* its type */
   
   int  idx1_min, idx1_max;  /* to check array bounds */
   int  idx2_min, idx2_max;  /* to check array bounds */
    
} TYPE_PARAMS2;


#define TYPEd_UNDEFINED    0 /* for errors */

#define TYPEd_STRING       1
#define TYPEd_INT          2
#define TYPEd_REAL         3
#define TYPEd_CHAR         6
#define TYPEd_FUN_1D       7
#define TYPEd_FUN_2D       8
#define TYPEd_FUN_3D       9
#define TYPEd_REAL_VECTOR 10
#define TYPEd_INT_VECTOR  11

static TYPE_PARAMS0 params_list0[] =
{
   { "main_problem"   , "NULL"    , TYPEd_STRING },
	
	{ "finite_elements_params"     , "name_ef"   , TYPEd_STRING },

   { "physical_params", "epsilon" , TYPEd_REAL },
   { "physical_params", "sigma"   , TYPEd_REAL },
   { "physical_params", "kappa"   , TYPEd_REAL },

   { "time_params"    , "DT"      , TYPEd_REAL },
   { "time_params"    , "TPS_INI" , TYPEd_REAL },
   { "time_params"    , "TPS_FIN" , TYPEd_REAL },
	
   { "time_scheme_params"    , "QSTEP"           , TYPEd_INT },
   { "time_scheme_params"    , "ALPHA_STEP"      , TYPEd_INT },
   { "time_scheme_params"    , "BETA_STEPS"      , TYPEd_INT },
   { "time_scheme_params"    , "GAMMA_STEP"      , TYPEd_INT },

   { "geometry_params", "meshfile", TYPEd_STRING },
   { "geometry_params", "meshtype", TYPEd_STRING },
	{ "geometry_params", "meshname", TYPEd_STRING },
   { "geometry_params", "nx"      , TYPEd_INT    },
   { "geometry_params", "xmin"    , TYPEd_REAL   },
   { "geometry_params", "xmax"    , TYPEd_REAL   },
   { "geometry_params", "distribution" , TYPEd_STRING },
	{ "geometry_params", "periodicity"  , TYPEd_INT },

   { "boundary_conditions_params" , "nb_BCtotal", TYPEd_INT },

   { "convective_terms_params"    , "method"    , TYPEd_STRING },
   { "convective_terms_params"    , "delta"     , TYPEd_REAL },
   { "convective_terms_params"    , "rho_stab"  , TYPEd_REAL },

   { "eigen_params"          , "problem"      , TYPEd_INT  },
   { "eigen_params"          , "option_eigen" , TYPEd_INT  },
   { "eigen_params"          , "nb_eig"       , TYPEd_INT  },
   { "eigen_params"          , "lower_ibound" , TYPEd_INT  },
   { "eigen_params"          , "upper_ibound" , TYPEd_INT  },
   { "eigen_params"          , "lower_rbound" , TYPEd_REAL },
   { "eigen_params"          , "upper_rbound" , TYPEd_REAL },

   { "stokes_params"         , "method"       , TYPEd_STRING },
	/* PRESSUREMATRIX */
	{ "stokes_params"         , "pressurematrix.eps_steps"                          , TYPEd_REAL },
	{ "stokes_params"         , "pressurematrix.max_steps"                          , TYPEd_INT },
	{ "stokes_params"         , "pressurematrix.innerloop_method"                   , TYPEd_STRING },
	{ "stokes_params"         , "pressurematrix.innerloop_solver.bandwidth_option"  , TYPEd_INT },
	{ "stokes_params"         , "pressurematrix.innerloop_solver.bandwidth_method"  , TYPEd_INT },
	{ "stokes_params"         , "pressurematrix.innerloop_solver.eps_steps"         , TYPEd_REAL },
	{ "stokes_params"         , "pressurematrix.innerloop_solver.max_steps"         , TYPEd_INT },
	/* UZAWA */
	{ "stokes_params"         , "uzawa.rho"                                         , TYPEd_REAL },
	{ "stokes_params"         , "uzawa.eps_steps"                                   , TYPEd_REAL },
	{ "stokes_params"         , "uzawa.max_steps"                                   , TYPEd_INT },
	{ "stokes_params"         , "uzawa.innerloop_method"                            , TYPEd_STRING },
	{ "stokes_params"         , "uzawa.innerloop_solver.bandwidth_option"           , TYPEd_INT },
	{ "stokes_params"         , "uzawa.innerloop_solver.bandwidth_method"           , TYPEd_INT },
	{ "stokes_params"         , "uzawa.innerloop_solver.eps_steps"                  , TYPEd_REAL },
	{ "stokes_params"         , "uzawa.innerloop_solver.max_steps"                  , TYPEd_INT },
	/* AUGMENTEDLAGR */
	{ "stokes_params"         , "augmentedlagrangian.rho"                                 , TYPEd_REAL },
	{ "stokes_params"         , "augmentedlagrangian.eps_steps"                           , TYPEd_REAL },
	{ "stokes_params"         , "augmentedlagrangian.max_steps"                           , TYPEd_INT },
	{ "stokes_params"         , "augmentedlagrangian.innerloop_method"                    , TYPEd_STRING },
	{ "stokes_params"         , "augmentedlagrangian.innerloop_solver.bandwidth_option"   , TYPEd_INT },
	{ "stokes_params"         , "augmentedlagrangian.innerloop_solver.bandwidth_method"   , TYPEd_INT },
	{ "stokes_params"         , "augmentedlagrangian.innerloop_solver.eps_steps"          , TYPEd_REAL },
	{ "stokes_params"         , "augmentedlagrangian.innerloop_solver.max_steps"          , TYPEd_INT },
	

    { "navierstokes_params"   , "method"           , TYPEd_STRING },
	{ "navierstokes_params"   , "eps_steps"        , TYPEd_REAL },
	{ "navierstokes_params"   , "max_steps"        , TYPEd_INT },
    { "navierstokes_params"   , "outer_method"     , TYPEd_STRING },
	/* PRESSUREMATRIX */
	{ "navierstokes_params"   , "pressurematrix.eps_steps"                          , TYPEd_REAL },
	{ "navierstokes_params"   , "pressurematrix.max_steps"                          , TYPEd_INT },
	{ "navierstokes_params"   , "pressurematrix.innerloop_method"                   , TYPEd_STRING },
	{ "navierstokes_params"   , "pressurematrix.innerloop_solver.bandwidth_option"  , TYPEd_INT },
	{ "navierstokes_params"   , "pressurematrix.innerloop_solver.bandwidth_method"  , TYPEd_INT },
	{ "navierstokes_params"   , "pressurematrix.innerloop_solver.eps_steps"         , TYPEd_REAL },
	{ "navierstokes_params"   , "pressurematrix.innerloop_solver.max_steps"         , TYPEd_INT },
	/* UZAWA */
	{ "navierstokes_params"   , "uzawa.rho"                                         , TYPEd_REAL },
	{ "navierstokes_params"   , "uzawa.eps_steps"                                   , TYPEd_REAL },
	{ "navierstokes_params"   , "uzawa.max_steps"                                   , TYPEd_INT },
	{ "navierstokes_params"   , "uzawa.innerloop_method"                            , TYPEd_STRING },
	{ "navierstokes_params"   , "uzawa.innerloop_solver.bandwidth_option"           , TYPEd_INT },
	{ "navierstokes_params"   , "uzawa.innerloop_solver.bandwidth_method"           , TYPEd_INT },
	{ "navierstokes_params"   , "uzawa.innerloop_solver.eps_steps"                  , TYPEd_REAL },
	{ "navierstokes_params"   , "uzawa.innerloop_solver.max_steps"                  , TYPEd_INT },
   
	
	
   { "matrix_solver_params"  , "resolution_method"      , TYPEd_STRING },
   { "matrix_solver_params"  , "preconditionning"       , TYPEd_STRING },
   { "matrix_solver_params"  , "eps_steps"              , TYPEd_REAL },
   { "matrix_solver_params"  , "max_steps"              , TYPEd_INT  },
   { "matrix_solver_params"  , "bandwidth_method"       , TYPEd_INT },
   { "matrix_solver_params"  , "bandwidth_option"       , TYPEd_INT },
   { "matrix_solver_params"  , "iter_file"              , TYPEd_STRING },
   { "matrix_solver_params"  , "iter_info"              , TYPEd_INT },
	
	
   { "graphics_output_params", "GNUPLOT"         , TYPEd_INT },
   { "graphics_output_params", "GRAPH"           , TYPEd_INT },
   { "graphics_output_params", "MATLAB"          , TYPEd_INT },
   { "graphics_output_params", "MATPLOTLIB"      , TYPEd_INT },
   { "graphics_output_params", "PGPLOT"          , TYPEd_INT },
   { "graphics_output_params", "VOGLE"           , TYPEd_INT },
   { "graphics_output_params", "LIBSCIPLOT"      , TYPEd_INT },
   { "graphics_output_params", "SILO"            , TYPEd_INT },
   { "graphics_output_params", "VTK"             , TYPEd_INT },

   { "graphics_interactiv1D_params" , "DRIVER"          , TYPEd_STRING },
   { "graphics_interactiv1D_params" , "SIZE_WINDOW_X"   , TYPEd_INT  },
   { "graphics_interactiv1D_params" , "SIZE_WINDOW_Y"   , TYPEd_INT  },
   { "graphics_interactiv1D_params" , "SIZE_LABELS"     , TYPEd_REAL },
   { "graphics_interactiv1D_params" , "NX"              , TYPEd_INT  },
   { "graphics_interactiv1D_params" , "NY"              , TYPEd_INT  },
	
   { "graphics_interactiv2D_params" , "DRIVER"          , TYPEd_STRING },
   { "graphics_interactiv2D_params" , "SIZE_WINDOW_X"   , TYPEd_INT  },
   { "graphics_interactiv2D_params" , "SIZE_WINDOW_Y"   , TYPEd_INT  },
   { "graphics_interactiv2D_params" , "SIZE_LABELS"     , TYPEd_REAL },
   { "graphics_interactiv2D_params" , "NX"              , TYPEd_INT  },
   { "graphics_interactiv2D_params" , "NY"              , TYPEd_INT  },
	
   { "miscellaneous_params"  , "itprint"         , TYPEd_INT },
   { "miscellaneous_params"  , "itsave"          , TYPEd_INT },
   { "miscellaneous_params"  , "itstat"          , TYPEd_INT },
   { "miscellaneous_params"  , "itgraph"         , TYPEd_INT },
	
   { "miscellaneous_graphics", "ispmatpattern"   , TYPEd_INT },
   { "miscellaneous_graphics", "igeomview"       , TYPEd_INT },
   { "miscellaneous_graphics", "igeomcontourview", TYPEd_INT },
   { "miscellaneous_graphics", "igeomtotalview"  , TYPEd_INT },
   
   { NULL                    , NULL              , TYPEd_INT }
};   


static TYPE_PARAMS1 params_list1[] =
{
   { "right_hand_side_params"     , "rhs"                 , TYPEd_INT      , AXEe_X , AXEe_Z },
   { "initial_condition_params"   , "ic"                  , TYPEd_INT      , AXEe_X , AXEe_Z },
   { "exact_solution_params"      , "exact_sol"           , TYPEd_INT      , AXEe_X , AXEe_Z },
	
   { NULL                         , NULL                  , TYPEd_INT      , 0 , 0 }

};


static TYPE_PARAMS2 params_list2[] =
{
   { "convective_terms_params"    , "adv1"                , TYPEd_INT  , AXEe_X , AXEe_Z , AXEe_X , AXEe_Z },
   { "convective_terms_params"    , "adv2"                , TYPEd_INT  , AXEe_X , AXEe_Z , AXEe_X , AXEe_Z },
   { "convective_terms_params"    , "adv3"                , TYPEd_INT  , AXEe_X , AXEe_Z , AXEe_X , AXEe_Z },

   { "boundary_conditions_params" , "TabBCMask"           , TYPEd_INT  , 0 , NB_BOUNDARYCONDITIONS-1, AXEe_X , AXEe_Z },
   { "boundary_conditions_params" , "TabPhi_BCdirichlet"  , TYPEd_INT  , 0 , NB_BOUNDARYCONDITIONS-1, AXEe_X , AXEe_Z },
   { "boundary_conditions_params" , "TabPhi_BCneumann"    , TYPEd_INT  , 0 , NB_BOUNDARYCONDITIONS-1, AXEe_X , AXEe_Z },
   { "boundary_conditions_params" , "TabPhi_BCcauchy"     , TYPEd_INT  , 0 , NB_BOUNDARYCONDITIONS-1, AXEe_X , AXEe_Z },
   { "boundary_conditions_params" , "TabPhi_BCrobin1"     , TYPEd_INT  , 0 , NB_BOUNDARYCONDITIONS-1, AXEe_X , AXEe_Z },
   { "boundary_conditions_params" , "TabPhi_BCrobin2"     , TYPEd_INT  , 0 , NB_BOUNDARYCONDITIONS-1, AXEe_X , AXEe_Z },

   { "graphics_interactiv1D_params"  , "LABEL_LEGEND"        , TYPEd_STRING , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "LABEL_AXE_X"         , TYPEd_STRING , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "LABEL_AXE_Y"         , TYPEd_STRING , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "WINDOW_X_MAX"        , TYPEd_REAL   , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "WINDOW_Y_MAX"        , TYPEd_REAL   , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "WINDOW_X_MIN"        , TYPEd_REAL   , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "WINDOW_Y_MIN"        , TYPEd_REAL   , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "MARKER"              , TYPEd_INT    , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv1D_params"  , "MARKER_END"          , TYPEd_INT    , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },

   { "graphics_interactiv2D_params"  , "LABEL_LEGEND"      , TYPEd_STRING      , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "WINDOW_X_MAX"      , TYPEd_REAL        , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "WINDOW_Y_MAX"      , TYPEd_REAL        , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "WINDOW_X_MIN"      , TYPEd_REAL        , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "WINDOW_Y_MIN"      , TYPEd_REAL        , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "NB_LEVELS"         , TYPEd_INT         , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "LEVELS"            , TYPEd_REAL_VECTOR , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
   { "graphics_interactiv2D_params"  , "GRAPHICS_TYPE"     , TYPEd_STRING      , 0, GRAPHIC_NB_SUB_WINDOWS, 0, GRAPHIC_NB_SUB_WINDOWS },
	
   { NULL                            , NULL                , TYPEd_INT    , 0, 0, 0, 0 },
   
};


int Params_check_oneparam0_arguments(const char* category, const char* field);
int Params_check_oneparam1_arguments(const char* category, const char* field, int idx1 );
int Params_check_oneparam2_arguments(const char* category, const char* field, int idx1, int idx2 );

int Params_get_valuetype(const char* category, const char* field);

#ifdef __cplusplus
}
#endif

#endif
