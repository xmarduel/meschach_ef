/** \file all_params.c
 *
 * Contains the routines for setting-getting values of all parameters
 *
 */

#include "MESCHACH/INCLUDES/err.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"

#include "MESCHACH_EF/INCLUDES/all_params_datas.h"

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_physical_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_time_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_finite_elements_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_geometry_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_boundary_conditions_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_convective_terms_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_exact_solution_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_eigen_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_stokes_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_navierstokes_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_matrix_solver_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_graphics_output_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_graphics_interactiv1Dplots_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_graphics_interactiv2Dplots_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_time_scheme_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_miscellaneous_params(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_miscellaneous_graphics(const PARAMS* p, const char* field);
static void * Params_get_oneparam0_cat_main_problem(const PARAMS* p, const char* field);

static void * Params_get_oneparam1_cat_right_hand_side_params(const PARAMS* p, const char* field, int idx1);
static void * Params_get_oneparam1_cat_initial_condition_params(const PARAMS* p, const char* field, int idx1);
static void * Params_get_oneparam1_cat_exact_solution_params(const PARAMS* p, const char* field, int idx1);
static void * Params_get_oneparam1_cat_graphics_interactiv1Dplots_params(const PARAMS* p, const char* field, int idx1);
static void * Params_get_oneparam1_cat_graphics_interactiv2Dplots_params(const PARAMS* p, const char* field, int idx1);

static void * Params_get_oneparam2_cat_convective_terms_params(const PARAMS* p, const char* field, int idx1, int idx2);
static void * Params_get_oneparam2_cat_boundary_conditions_params(const PARAMS* p, const char* field, int idx1, int idx2);
static void * Params_get_oneparam2_cat_graphics_interactiv1Dplots_params(const PARAMS* p, const char* field, int idx1, int idx2);
static void * Params_get_oneparam2_cat_graphics_interactiv2Dplots_params(const PARAMS* p, const char* field, int idx1, int idx2);

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void * Params_get_oneparam(const PARAMS* p, const char* category, const char* field)
{
   if ( !p        ) error(E_NULL, "Params_get_oneparam");
   if ( !category ) error(E_NULL, "Params_get_oneparam");
   if ( !field    ) error(E_NULL, "Params_get_oneparam");

   /* check */
   if ( !Params_check_oneparam0_arguments(category, field) )
   {
      return NULL;
   }

   /* get */
   if ( strcmp(category,"main_problem") == 0 )
   {
      return Params_get_oneparam0_cat_main_problem(p, field);
   }
   else
   if ( strcmp(category,"physical_params") == 0 )
   {
      return Params_get_oneparam0_cat_physical_params(p, field);
   }
   else
   if ( strcmp(category,"time_params") == 0 )
   {
      return Params_get_oneparam0_cat_time_params(p, field);
   }
   else
   if ( strcmp(category,"time_scheme_params") == 0 )
   {
      return Params_get_oneparam0_cat_time_scheme_params(p, field);
   }
   else
   if ( strcmp(category,"finite_elements_params") == 0 )
   {
      return Params_get_oneparam0_cat_finite_elements_params(p, field);
   }
   else
   if ( strcmp(category,"geometry_params") == 0 )
   {
      return Params_get_oneparam0_cat_geometry_params(p, field);
   }
   else
   if ( strcmp(category,"boundary_conditions_params") == 0 )
   {
      return Params_get_oneparam0_cat_boundary_conditions_params(p, field);
   }
   else
   if ( strcmp(category,"eigen_params") == 0 )
   {
      return Params_get_oneparam0_cat_eigen_params(p, field);
   }
   else
   if ( strcmp(category,"stokes_params") == 0 )
   {
      return Params_get_oneparam0_cat_stokes_params(p, field);
   }
   else
   if ( strcmp(category,"navierstokes_params") == 0 )
   {
      return Params_get_oneparam0_cat_navierstokes_params(p, field);
   }
   else
   if ( strcmp(category,"convective_terms_params") == 0 )
   {
      return Params_get_oneparam0_cat_convective_terms_params(p, field);
   }
   else
   if ( strcmp(category,"matrix_solver_params") == 0 )
   {
      return Params_get_oneparam0_cat_matrix_solver_params(p, field);
   }
   else
   if ( strcmp(category,"graphics_output_params") == 0 )
   {
      return Params_get_oneparam0_cat_graphics_output_params(p, field);
   }
   else
   if ( strcmp(category,"miscellaneous_params") == 0 )
   {
      return Params_get_oneparam0_cat_miscellaneous_params(p, field);
   }
   else
   if ( strcmp(category,"miscellaneous_graphics") == 0 )
   {
      return Params_get_oneparam0_cat_miscellaneous_graphics(p, field);
   }
   else
   if ( strcmp(category,"graphics_interactiv1D_params") == 0 )
   {
      return Params_get_oneparam0_cat_graphics_interactiv1Dplots_params(p, field);
   }
   else
   if ( strcmp(category,"graphics_interactiv2D_params") == 0 )
   {
      return Params_get_oneparam0_cat_graphics_interactiv2Dplots_params(p, field);
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam");
   }

   return (void*)NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_physical_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"epsilon") == 0 )
   {
      return (void*)(&(p->phys_params.epsilon));
   }
   else
   if ( strcmp(field,"sigma") == 0 )
   {
      return (void*)(&(p->phys_params.sigma));
   }
   else
   if ( strcmp(field,"kappa") == 0 )
   {
      return (void*)(&(p->phys_params.kappa));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_physical_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_time_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"DT") == 0 )
   {
      return (void*)(&(p->time_params.DT));
   }
   else
   if ( strcmp(field,"TPS_INI") == 0 )
   {
      return (void*)(&(p->time_params.TPS_INI));
   }
   else
   if ( strcmp(field,"TPS_FIN") == 0 )
   {
      return (void*)(&(p->time_params.TPS_FIN));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_time_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
static void * Params_get_oneparam0_cat_finite_elements_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"name_ef") == 0 )
   {
      return (void*)(p->ef_params.name_ef);
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_finite_elements_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_geometry_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"meshfile") == 0 )
   {
      return (void*)(p->geom_params.meshfile);
   }
   else
   if ( strcmp(field,"meshname") == 0 )
   {
      return (void*)(p->geom_params.meshname);
   }
   else
   if ( strcmp(field,"meshtype") == 0 )
   {
      return (void*)(p->geom_params.meshtype);
   }
   else
   if ( strcmp(field,"nx") == 0 )
   {
      return (void*)(&(p->geom_params.nx));
   }
   else
   if ( strcmp(field,"xmin") == 0 )
   {
      return (void*)(&(p->geom_params.xmin));
   }
   else
   if ( strcmp(field,"xmax") == 0 )
   {
      return (void*)(&(p->geom_params.xmax));
   }
   else
   if ( strcmp(field,"distribution") == 0 )
   {
      return (void*)(p->geom_params.distribution);
   }
   else
   if ( strcmp(field,"periodicity") == 0 )
   {
      return (void*)(&(p->geom_params.periodicity));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_geometry_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_boundary_conditions_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"nb_BCtotal") == 0 )
   {
      return (void*)(&(p->bc_params.nb_BC));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_boundary_conditions_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_convective_terms_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"method") == 0 )
   {
      return (void*)(p->adv_params.method);
   }
   else
   if ( strcmp(field,"delta") == 0 )
   {
      return (void*)(&(p->adv_params.delta));
   }
   else
   if ( strcmp(field,"rho_stab") == 0 )
   {
      return (void*)(&(p->adv_params.rho_stab));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_convective_terms_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_eigen_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"problem") == 0 )
   {
      return (void*)(&(p->eigen_params.eigen_problem));
   }
   else
   if ( strcmp(field,"option_eigen") == 0 )
   {
      return (void*)(&(p->eigen_params.eigen_option));
   }
   else
   if ( strcmp(field,"nb_eig") == 0 )
   {
      return (void*)(&(p->eigen_params.nb_eig));
   }
   else
   if ( strcmp(field,"lower_ibound") == 0 )
   {
      return (void*)(&(p->eigen_params.lower_ibound));
   }
   else
   if ( strcmp(field,"upper_ibound") == 0 )
   {
      return(void*)( &(p->eigen_params.upper_ibound));
   }
   else
   if ( strcmp(field,"lower_rbound") == 0 )
   {
      return (void*)(&(p->eigen_params.lower_rbound));
   }
   else
   if ( strcmp(field,"upper_rbound") == 0 )
   {
      return (void*)(&(p->eigen_params.upper_rbound));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_eigen_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_stokes_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field, "method") == 0 )
   {
      return (void*)(p->stokes_params.method);
   }
   else
   if ( strcmp(field, "pressurematrix.max_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.pressurematrix.max_steps));
   }
   else
   if ( strcmp(field, "pressurematrix.eps_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.pressurematrix.eps_steps));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_method") == 0 )
   {
      return (void*)(p->stokes_params.pressurematrix.innerloop_method);
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.bandwidth_option") == 0 )
   {
      return (void*)(&(p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.bandwidth_method") == 0 )
   {
      return (void*)(&(p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.eps_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.pressurematrix.innerloop_solver.eps_steps));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.max_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.pressurematrix.innerloop_solver.max_steps));
   }

   else
   if ( strcmp(field, "uzawa.rho") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.rho));
   }
   else
   if ( strcmp(field, "uzawa.max_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.max_steps));
   }
   else
   if ( strcmp(field, "uzawa.eps_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.eps_steps));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.bandwidth_option") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.innerloop_solver.bandwidth_option));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.bandwidth_method") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.innerloop_solver.bandwidth_method));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.eps_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.innerloop_solver.eps_steps));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.max_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.uzawa.innerloop_solver.max_steps));
   }

   else
   if ( strcmp(field, "augmentedlagrangian.rho") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.rho));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.max_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.max_steps));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.eps_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.eps_steps));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.innerloop_method") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.innerloop_method));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.innerloop_solver.bandwidth_option") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_option));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.innerloop_solver.bandwidth_method") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_method));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.innerloop_solver.eps_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps));
   }
   else
   if ( strcmp(field, "augmentedlagrangian.innerloop_solver.max_steps") == 0 )
   {
      return (void*)(&(p->stokes_params.augmentedlagrangian.innerloop_solver.max_steps));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_stokes_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_navierstokes_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"method") == 0 )
   {
      return (void*)(p->navierstokes_params.method);
   }
   else
   if ( strcmp(field,"eps_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.eps_steps));
   }
   else
   if ( strcmp(field,"max_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.max_steps));
   }
   else
   if ( strcmp(field,"outer_method") == 0 )
   {
      return (void*)(p->navierstokes_params.outer_method);
   }
   /* PRESSURE MATRIX */
   else
   if ( strcmp(field, "pressurematrix.max_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.pressurematrix.max_steps));
   }
   else
   if ( strcmp(field, "pressurematrix.eps_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.pressurematrix.eps_steps));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_method") == 0 )
   {
      return (void*)(p->navierstokes_params.pressurematrix.innerloop_method);
   }
   if ( strcmp(field, "pressurematrix.innerloop_solver.bandwidth_option") == 0 )
   {
      return (void*)(&(p->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_option));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.bandwidth_method") == 0 )
   {
      return (void*)(&(p->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_method));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.eps_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.pressurematrix.innerloop_solver.eps_steps));
   }
   else
   if ( strcmp(field, "pressurematrix.innerloop_solver.max_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.pressurematrix.innerloop_solver.max_steps));
   }
   /* UZAWA */
   else
   if ( strcmp(field, "uzawa.rho") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.rho));
   }
   else
   if ( strcmp(field, "uzawa.max_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.max_steps));
   }
   else
   if ( strcmp(field, "uzawa.eps_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.eps_steps));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.bandwidth_option") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.innerloop_solver.bandwidth_option));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.bandwidth_method") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.innerloop_solver.bandwidth_method));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.eps_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.innerloop_solver.eps_steps));
   }
   else
   if ( strcmp(field, "uzawa.innerloop_solver.max_steps") == 0 )
   {
      return (void*)(&(p->navierstokes_params.uzawa.innerloop_solver.max_steps));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_navierstokes_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_matrix_solver_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"eps_steps") == 0 )
   {
      return (void*)(&(p->resol_params.eps_steps));
   }
   else
   if ( strcmp(field,"max_steps") == 0 )
   {
      return (void*)(&(p->resol_params.max_steps));
   }
   else
   if ( strcmp(field,"nb_steps") == 0 )
   {
      return (void*)(&(p->resol_params.nb_steps));
   }
   else
   if ( strcmp(field,"preconditionning") == 0 )
   {
      return (void*)(p->resol_params.preconditionning);
   }
   else
   if ( strcmp(field,"resolution_method") == 0 )
   {
      return (void*)(p->resol_params.resolution_method);
   }
   else
   if ( strcmp(field,"bandwidth_method") == 0 )
   {
      return (void*)(&(p->resol_params.bandwidth_method));
   }
   else
   if ( strcmp(field,"bandwidth_option") == 0 )
   {
      return (void*)(&(p->resol_params.bandwidth_option));
   }
   else
   if ( strcmp(field,"iter_file") == 0 )
   {
      return (void*)(p->resol_params.iter_file);
   }
   else
   if ( strcmp(field,"iter_info") == 0 )
   {
      return (void*)(&(p->resol_params.iter_info));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_matrix_solver_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_graphics_output_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"GNUPLOT") == 0 )
   {
      return (void*)(&(p->graph_params.GNUPLOT));
   }
   else
   if ( strcmp(field,"MATLAB") == 0 )
   {
      return (void*)(&(p->graph_params.MATLAB));
   }
   else
   if ( strcmp(field,"SILO") == 0 )
   {
      return (void*)(&(p->graph_params.SILO));
   }
   else
   if ( strcmp(field,"VTK") == 0 )
   {
      return (void*)(&(p->graph_params.VTK));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_graphics_output_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_graphics_interactiv1Dplots_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"ENGINE") == 0 )
   {
      return (void*)(p->graph_interactiv1Dplots_params.ENGINE) ;
   }
   else
   if ( strcmp(field,"DRIVER") == 0 )
   {
      return (void*)(p->graph_interactiv1Dplots_params.DRIVER) ;
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_X") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.SIZE_WINDOW_X));
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_Y") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.SIZE_WINDOW_Y));
   }
   else
   if ( strcmp(field,"SIZE_LABELS") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.SIZE_LABELS));
   }
   else
   if ( strcmp(field,"NX") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.NX));
   }
   else
   if ( strcmp(field,"NY") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.NY));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_graphics_interactiv1Dplots_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_graphics_interactiv2Dplots_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"DRIVER") == 0 )
   {
      return (void*)(p->graph_interactiv2Dplots_params.DRIVER) ;
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_X") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.SIZE_WINDOW_X));
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_Y") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.SIZE_WINDOW_Y));
   }
   else
   if ( strcmp(field,"SIZE_LABELS") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.SIZE_LABELS));
   }
   else
   if ( strcmp(field,"NX") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.NX));
   }
   else
   if ( strcmp(field,"NY") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.NY));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_graphics_interactiv2Dplots_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_time_scheme_params(const PARAMS* p, const char* field)
{
   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_miscellaneous_graphics(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"ispsmatpattern") == 0 )
   {
      return (void*)(&(p->misc_graphics.ispmatpattern));
   }
   else
   if ( strcmp(field,"igeomview") == 0 )
   {
      return (void*)(&(p->misc_graphics.igeomview));
   }
   else
   if ( strcmp(field,"igeomcontourview") == 0 )
   {
      return (void*)(&(p->misc_graphics.igeomcontourview));
   }
   else
   if ( strcmp(field,"igeomtotalview") == 0 )
   {
      return (void*)(&(p->misc_graphics.igeomtotalview));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_miscellaneous_graphics");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_exact_solution_params(const PARAMS* p, const char* field)
{
   error(E_UNKNOWN, "Params_get_oneparam0_cat_exact_solution_params");

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_miscellaneous_params(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"itprint") == 0 )
   {
      return (void*)(&(p->misc_params.itprint));
   }
   else
   if ( strcmp(field,"itsave") == 0 )
   {
      return (void*)(&(p->misc_params.itsave));
   }
   else
   if ( strcmp(field,"itstat") == 0 )
   {
      return (void*)(&(p->misc_params.itstat));
   }
   else
   if ( strcmp(field,"itgraph") == 0 )
   {
      return (void*)(&(p->misc_params.itgraph));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam0_cat_miscellaneous_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam0_cat_main_problem(const PARAMS* p, const char* field)
{
   if ( strcmp(field,"NULL") == 0 )
   {
      return (void*)(p->problem);
   }
   else
   {
      printf("field = %s \n", field);
      error(E_UNKNOWN, "Params_get_oneparam0_cat_main_problem");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void * Params_get_oneparam1(const PARAMS* p, const char* category, const char* field, int idx1 )
{
   if ( !p        ) error(E_NULL, "Params_get_oneparam1");
   if ( !category ) error(E_NULL, "Params_get_oneparam1");
   if ( !field    ) error(E_NULL, "Params_get_oneparam1");


   /* check */
   if ( !Params_check_oneparam1_arguments(category, field, idx1) )
   {
      return NULL;
   }

   /* get */
   if ( strcmp(category,"right_hand_side_params") == 0 )
   {
      return Params_get_oneparam1_cat_right_hand_side_params(p, field, idx1);
   }
   else
   if ( strcmp(category,"initial_condition_params") == 0 )
   {
      return Params_get_oneparam1_cat_initial_condition_params(p, field, idx1);
   }
   else
   if ( strcmp(category,"exact_solution_params") == 0 )
   {
      return Params_get_oneparam1_cat_exact_solution_params(p, field, idx1);
   }
   else
   if ( strcmp(category,"graphics_interactiv1D_params") == 0 )
   {
      return Params_get_oneparam1_cat_graphics_interactiv1Dplots_params(p, field, idx1);
   }
   else
   if ( strcmp(category,"graphics_interactiv2D_params") == 0 )
   {
      return Params_get_oneparam1_cat_graphics_interactiv2Dplots_params(p, field, idx1);
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam1");
   }

   return (void*)NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam1_cat_right_hand_side_params(const PARAMS* p, const char* field, int idx1)
{
   /*
   if ( strcmp(field,"rhs") == 0 )
   {
      return (void*)(&(p->rhs_params.rhs[idx1]));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam1_cat_right_hand_side_params");
   }
   */

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam1_cat_initial_condition_params(const PARAMS* p, const char* field, int idx1)
{
   /*
   if ( strcmp(field,"ic") == 0 )
   {
      return (void*)(&(p->ic_params.ini[idx1]));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam1_cat_initial_condition_params");
   }
   */

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam1_cat_exact_solution_params(const PARAMS* p, const char* field, int idx1)
{
   /*
   if ( strcmp(field,"exact_sol") == 0 )
   {
      return (void*)(&(p->exactsol_params.exact_sol[idx1]));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam1_cat_exact_solution_params");
   }
   */

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam1_cat_graphics_interactiv1Dplots_params(const PARAMS* p, const char* field, int idx1)
{
   error(E_UNKNOWN, "Params_get_oneparam1_cat_graphics_interactiv1Dplots_params");

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam1_cat_graphics_interactiv2Dplots_params(const PARAMS* p, const char* field, int idx1)
{
   error(E_UNKNOWN, "Params_get_oneparam1_cat_graphics_interactiv2Dplots_params");

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void * Params_get_oneparam2(const PARAMS* p, const char* category, const char* field, int idx1, int idx2  )
{
   if ( !p        ) error(E_NULL, "Params_get_oneparam2");
   if ( !category ) error(E_NULL, "Params_get_oneparam2");
   if ( !field    ) error(E_NULL, "Params_get_oneparam2");

   /* check */
   if ( !Params_check_oneparam2_arguments(category, field, idx1, idx2) )
   {
      return NULL;
   }

   /* get */
   if ( strcmp(category,"convective_terms_params") == 0 )
   {
      return Params_get_oneparam2_cat_convective_terms_params(p, field, idx1, idx2);
   }
   else
   if ( strcmp(category,"boundary_conditions_params") == 0 )
   {
      return Params_get_oneparam2_cat_boundary_conditions_params(p, field, idx1, idx2);
   }
   else
   if ( strcmp(category,"graphics_interactiv1D_params") == 0 )
   {
      return Params_get_oneparam2_cat_graphics_interactiv1Dplots_params(p, field, idx1, idx2);
   }
   else
   if ( strcmp(category,"graphics_interactiv2D_params") == 0 )
   {
      return Params_get_oneparam2_cat_graphics_interactiv2Dplots_params(p, field, idx1, idx2);
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam2");
   }

   return (void*)NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam2_cat_convective_terms_params(const PARAMS* p, const char* field, int idx1, int idx2)
{
   if ( strcmp(field,"adv1") == 0 )
   {
      return (void*)(&(p->adv_params.adv1[idx1][idx2]));
   }
   else
   if ( strcmp(field,"adv2") == 0 )
   {
      return (void*)(&(p->adv_params.adv2[idx1][idx2]));
   }
   else
   if ( strcmp(field,"adv3") == 0 )
   {
      return (void*)(&(p->adv_params.adv3[idx1][idx2]));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam2_cat_convective_terms_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam2_cat_boundary_conditions_params(const PARAMS* p, const char* field, int idx1, int idx2)
{
   if ( strcmp(field,"BcType") == 0 )
   {
      return (void*)(&(p->bc_params.BcType[idx1][idx2]));
   }
   else
   if ( strcmp(field,"BcDirichlet") == 0 )
   {
      return (void*)(&(p->bc_params.BcDirichlet[idx1][idx2]));
   }
   else
   if ( strcmp(field,"BCNeumann") == 0 )
   {
      return (void*)(&(p->bc_params.BCNeumann[idx1][idx2]));
   }
   else
   if ( strcmp(field,"BcCauchy") == 0 )
   {
      return (void*)(&(p->bc_params.BcCauchy[idx1][idx2]));
   }
   else
   if ( strcmp(field,"BcRobin1") == 0 )
   {
      return (void*)(&(p->bc_params.BcRobin1[idx1][idx2]));
   }
   else
   if ( strcmp(field,"BcRobin2") == 0 )
   {
      return (void*)(&(p->bc_params.BcRobin2[idx1][idx2]));
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam2_cat_boundary_conditions_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam2_cat_graphics_interactiv1Dplots_params(const PARAMS* p, const char* field, int idx1, int idx2)
{
   if ( strcmp(field,"LABEL_LEGEND") == 0 )
   {
      return (void*)(p->graph_interactiv1Dplots_params.LABEL[idx1][idx2].LEGEND);
   }
   else
   if ( strcmp(field,"LABEL_AXE_X") == 0 )
   {
      return (void*)(p->graph_interactiv1Dplots_params.LABEL[idx1][idx2].AXE_X);
   }
   else
   if ( strcmp(field,"LABEL_AXE_Y") == 0 )
   {
      return (void*)(p->graph_interactiv1Dplots_params.LABEL[idx1][idx2].AXE_Y);
   }
   else
   if ( strcmp(field,"WINDOW_X_MAX") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].X_MAX)) ;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MAX") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].Y_MAX)) ;
   }
   else
   if ( strcmp(field,"WINDOW_X_MIN") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].X_MIN)) ;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MIN") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].Y_MIN)) ;
   }
   else
   if ( strcmp(field,"MARKER") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.MARKER[idx1][idx2])) ;
   }
   else
   if ( strcmp(field,"MARKER_END") == 0 )
   {
      return (void*)(&(p->graph_interactiv1Dplots_params.MARKER_END[idx1][idx2])) ;
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam2_cat_graphics_interactiv1Dplots_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void * Params_get_oneparam2_cat_graphics_interactiv2Dplots_params(const PARAMS* p, const char* field, int idx1, int idx2)
{
   if ( strcmp(field,"LABEL_LEGEND") == 0 )
   {
      return (void*)(p->graph_interactiv2Dplots_params.LABEL[idx1][idx2].LEGEND);
   }
   else
   if ( strcmp(field,"WINDOW_X_MAX") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].X_MAX)) ;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MAX") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].Y_MAX)) ;
   }
   else
   if ( strcmp(field,"WINDOW_X_MIN") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].X_MIN)) ;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MIN") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].Y_MIN)) ;
   }
   else
   if ( strcmp(field,"NB_LEVELS") == 0 )
   {
      return (void*)(&(p->graph_interactiv2Dplots_params.NB_LEVELS[idx1][idx2])) ;
   }
   else
   if ( strcmp(field,"LEVELS") == 0 )
   {
      return (void*)(p->graph_interactiv2Dplots_params.LEVELS[idx1][idx2]) ;
   }
   else
   if ( strcmp(field,"GRAPHICS_TYPE") == 0 )
   {
      return (void*)(p->graph_interactiv2Dplots_params.GRAPHICS_TYPE[idx1][idx2]);
   }
   else
   {
      error(E_UNKNOWN, "Params_get_oneparam2_cat_graphics_interactiv2Dplots_params");
   }

   return NULL;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/
