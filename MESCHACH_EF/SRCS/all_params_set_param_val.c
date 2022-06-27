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

static void Params_set_oneparam0_cat_physical_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_time_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_finite_elements_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_geometry_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_boundary_conditions_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_convective_terms_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_eigen_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_stokes_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_navierstokes_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_matrix_solver_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_graphics_output_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_graphics_interactiv1Dplots_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_graphics_interactiv2Dplots_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_time_scheme_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_miscellaneous_params(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_miscellaneous_graphics(PARAMS* p, const char* field, void*  val);
static void Params_set_oneparam0_cat_main_problem(PARAMS* p, const char* field, void*  val);

static void Params_set_oneparam1_cat_right_hand_side_params(PARAMS* p, const char* field, int idx1, void*  val);
static void Params_set_oneparam1_cat_initial_condition_params(PARAMS* p, const char* field, int idx1, void*  val);
static void Params_set_oneparam1_cat_exact_solution_params(PARAMS* p, const char* field, int idx1, void*  val);
static void Params_set_oneparam1_cat_graphics_interactiv1Dplots_params(PARAMS* p, const char* field, int idx1, void*  val);
static void Params_set_oneparam1_cat_graphics_interactiv2Dplots_params(PARAMS* p, const char* field, int idx1, void*  val);

static void Params_set_oneparam2_cat_convective_terms_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val);
static void Params_set_oneparam2_cat_boundary_conditions_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val);
static void Params_set_oneparam2_cat_graphics_interactiv1Dplots_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val);
static void Params_set_oneparam2_cat_graphics_interactiv2Dplots_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val);

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void Params_set_oneparam(PARAMS* p, const char* category, const char* field, void*  val)
{
   if ( !p        ) error(E_NULL, "Params_set_oneparam");
   if ( !category ) error(E_NULL, "Params_set_oneparam");
   if ( !field    ) error(E_NULL, "Params_set_oneparam");
   if ( !val      ) error(E_NULL, "Params_set_oneparam");

   /* check */
   if ( !Params_check_oneparam0_arguments(category, field) )
   {
      return;
   }

   /* set */
   if ( strcmp(category,"main_problem") == 0 )
   {
      Params_set_oneparam0_cat_main_problem(p, field, val);
   }
   else
   if ( strcmp(category,"physical_params") == 0 )
   {
      Params_set_oneparam0_cat_physical_params(p, field, val);
   }
   else
   if ( strcmp(category,"time_params") == 0 )
   {
      Params_set_oneparam0_cat_time_params(p, field, val);
   }
   else
   if ( strcmp(category,"finite_elements_params") == 0 )
   {
      Params_set_oneparam0_cat_finite_elements_params(p, field, val);
   }
   else
   if ( strcmp(category,"geometry_params") == 0 )
   {
      Params_set_oneparam0_cat_geometry_params(p, field, val);
   }
   else
   if ( strcmp(category,"boundary_conditions_params") == 0 )
   {
      Params_set_oneparam0_cat_boundary_conditions_params(p, field, val);
   }
   else
   if ( strcmp(category,"convective_terms_params") == 0 )
   {
      Params_set_oneparam0_cat_convective_terms_params(p, field, val);
   }
   else
   if ( strcmp(category,"eigen_params") == 0 )
   {
      Params_set_oneparam0_cat_eigen_params(p, field, val);
   }
   else
   if ( strcmp(category,"stokes_params") == 0 )
   {
      Params_set_oneparam0_cat_stokes_params(p, field, val);
   }
   else
   if ( strcmp(category,"navierstokes_params") == 0 )
   {
      Params_set_oneparam0_cat_navierstokes_params(p, field, val);
   }
   else
   if ( strcmp(category,"matrix_solver_params") == 0 )
   {
      Params_set_oneparam0_cat_matrix_solver_params(p, field, val);
   }
   else
   if ( strcmp(category,"graphics_output_params") == 0 )
   {
      Params_set_oneparam0_cat_graphics_output_params(p, field, val);
   }
   else
   if ( strcmp(category,"graphics_interactiv1D_params") == 0 )
   {
      Params_set_oneparam0_cat_graphics_interactiv1Dplots_params(p, field, val);
   }
   else
   if ( strcmp(category,"graphics_interactiv2D_params") == 0 )
   {
      Params_set_oneparam0_cat_graphics_interactiv2Dplots_params(p, field, val);
   }
   else
   if ( strcmp(category,"time_scheme_params") == 0 )
   {
      Params_set_oneparam0_cat_time_scheme_params(p, field, val);
   }
   else
   if ( strcmp(category,"miscellaneous_params") == 0 )
   {
      Params_set_oneparam0_cat_miscellaneous_params(p, field, val);
   }
   else
   if ( strcmp(category,"miscellaneous_graphics") == 0 )
   {
      Params_set_oneparam0_cat_miscellaneous_graphics(p, field, val);
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam");
   }

   return;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_main_problem(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"NULL") == 0 )
   {
      strncpy(p->problem, (char*)val, 128 );
      p->problem[127]='\0';

      if ( TYPEd_STRING != Params_get_valuetype("main_problem", "NULL") )
      {
         error(E_UNKNOWN, "Params_set_oneparam0_cat_main_problem");         
      }
           
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_main_problem");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_physical_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"epsilon") == 0 )
   {
      p->phys_params.epsilon = *(Real*)val;
   }
   else
   if ( strcmp(field,"sigma") == 0 )
   {
      p->phys_params.sigma   = *(Real*)val;
   }
   else
   if ( strcmp(field,"kappa") == 0 )
   {
      p->phys_params.kappa   = *(Real*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_physical_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_time_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"DT") == 0 )
   {
      p->time_params.DT        = *(Real*)val;
   }
   else
   if ( strcmp(field,"TPS_INI") == 0 )
   {
      p->time_params.TPS_INI   = *(Real*)val;
   }
   else
   if ( strcmp(field,"TPS_FIN") == 0 )
   {
      p->time_params.TPS_FIN   = *(Real*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_time_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_finite_elements_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"name_ef") == 0 )
   {
      strncpy(p->ef_params.name_ef, (char*)val, 16);
      p->ef_params.name_ef[15] = '\0';
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_finite_elements_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_geometry_params(PARAMS* p, const char* field, void*  val)
{   
   if ( strcmp(field,"meshfile") == 0 )
   {
      strncpy(p->geom_params.meshfile, (char*)val, 256);
      p->geom_params.meshfile[255] = '\0';
   }
   else
   if ( strcmp(field,"meshname") == 0 )
   {
      strncpy(p->geom_params.meshname, (char*)val, 64 );
      p->geom_params.meshname[63] = '\0';
   }
   else
   if ( strcmp(field,"meshtype") == 0 )
   {
      strncpy(p->geom_params.meshtype, (char*)val, 64 );
      p->geom_params.meshtype[63] = '\0';
   }
   else
   if ( strcmp(field,"nx") == 0 )
   {
      p->geom_params.nx = *(int*)val;
   }
   else
   if ( strcmp(field,"xmin") == 0 )
   {
      p->geom_params.xmin = *(Real*)val;
   }
   else
   if ( strcmp(field,"xmax") == 0 )
   {
      p->geom_params.xmax = *(Real*)val;
   }
   else
   if ( strcmp(field,"distribution") == 0 )
   {
      strncpy(p->geom_params.distribution, (char*)val, 64 );
      p->geom_params.distribution[63] = '\0';
   }
   else
   if ( strcmp(field,"periodicity") == 0 )
   {
      p->geom_params.periodicity = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_geometry_params");
   }   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_boundary_conditions_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"nb_BCtotal") == 0 )
   {
      p->bc_params.nb_BC = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_boundary_conditions_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_convective_terms_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"method") == 0 )
   {
      strncpy(p->adv_params.method, (char*)val, 7 );
      p->adv_params.method[7] = '\0';
   }
   else
   if ( strcmp(field,"delta") == 0 )
   {
      p->adv_params.delta = *(Real*)val;
   }
   else
   if ( strcmp(field,"rho_stab") == 0 )
   {
      p->adv_params.rho_stab = *(Real*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_convective_terms_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_eigen_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"problem") == 0 )
   {
      p->eigen_params.eigen_problem = *(int*)val;
   }
   else
   if ( strcmp(field,"option_eigen") == 0 )
   {
      p->eigen_params.eigen_option = *(int*)val;
   }
   else
   if ( strcmp(field,"nb_eig") == 0 )
   {
      p->eigen_params.nb_eig = *(int*)val;
   }
   else
   if ( strcmp(field,"lower_ibound") == 0 )
   {
      p->eigen_params.lower_ibound = *(int*)val;
   }
   else
   if ( strcmp(field,"upper_ibound") == 0 )
   {
      p->eigen_params.upper_ibound = *(int*)val;
   }
   else
   if ( strcmp(field,"lower_rbound") == 0 )
   {
      p->eigen_params.lower_rbound = *(Real*)val;
   }
   else
   if ( strcmp(field,"upper_rbound") == 0 )
   {
      p->eigen_params.upper_rbound = *(Real*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_eigen_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_stokes_params(PARAMS* p, const char* field, void* val)
{
   if ( strcmp(field,"method") == 0 )
   {
      strncpy(p->stokes_params.method, (char*)val, 64 );
      p->stokes_params.method[63] = '\0';
   }

   /* PRESSUREMATRIX */
   else
   if ( strcmp(field,"pressurematrix.eps_steps") == 0 )
   {
      p->stokes_params.pressurematrix.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.max_steps") == 0 )
   {
      p->stokes_params.pressurematrix.max_steps = *(int*)val;
   }   
   else
   if ( strcmp(field,"pressurematrix.innerloop_method") == 0 )
   {
      strncpy(p->stokes_params.pressurematrix.innerloop_method, (char*)val, 64 );
      p->stokes_params.pressurematrix.innerloop_method[63] = '\0';
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.bandwidth_option") == 0 )
   {
      p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option = *(int*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.bandwidth_method") == 0 )
   {
      p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method = *(int*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.eps_steps") == 0 )
   {
      p->stokes_params.pressurematrix.innerloop_solver.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.max_steps") == 0 )
   {
      p->stokes_params.pressurematrix.innerloop_solver.max_steps = *(int*)val;
   }   
   
   /* UZAWA */
   else
   if ( strcmp(field,"uzawa.rho") == 0 )
   {
      p->stokes_params.uzawa.rho = *(Real*)val;
   }
   else
   if ( strcmp(field,"uzawa.eps_steps") == 0 )
   {
      p->stokes_params.uzawa.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"uzawa.max_steps") == 0 )
   {
      p->stokes_params.uzawa.max_steps = *(int*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_method") == 0 )
   {
      strncpy(p->stokes_params.uzawa.innerloop_method, (char*)val, 64 );
      p->stokes_params.uzawa.innerloop_method[63] = '\0';
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.bandwidth_option") == 0 )
   {
      p->stokes_params.uzawa.innerloop_solver.bandwidth_option = *(int*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.bandwidth_method") == 0 )
   {
      p->stokes_params.uzawa.innerloop_solver.bandwidth_method = *(int*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.eps_steps") == 0 )
   {
      p->stokes_params.uzawa.innerloop_solver.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.max_steps") == 0 )
   {
      p->stokes_params.uzawa.innerloop_solver.max_steps = *(int*)val;
   }
   
   /* AUGMENTEDLAGR */
   else
   if ( strcmp(field,"augmentedlagrangian.rho") == 0 )
   {
      p->stokes_params.augmentedlagrangian.rho = *(Real*)val;
   }
   else
   if ( strcmp(field,"augmentedlagrangian.eps_steps") == 0 )
   {
      p->stokes_params.augmentedlagrangian.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"augmentedlagrangian.max_steps") == 0 )
   {
      p->stokes_params.augmentedlagrangian.max_steps = *(int*)val;
   }
   else
   if ( strcmp(field,"augmentedlagrangian.innerloop_method") == 0 )
   {
      strncpy(p->stokes_params.augmentedlagrangian.innerloop_method, (char*)val, 64 );
      p->stokes_params.augmentedlagrangian.innerloop_method[63] = '\0';
   }
   else
   if ( strcmp(field,"augmentedlagrangian.innerloop_solver.bandwidth_option") == 0 )
   {
      p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_option = *(int*)val;
   }
   else
   if ( strcmp(field,"augmentedlagrangian.innerloop_solver.bandwidth_method") == 0 )
   {
      p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_method = *(int*)val;
   }
   else
   if ( strcmp(field,"augmentedlagrangian.innerloop_solver.eps_steps") == 0 )
   {
      p->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"augmentedlagrangian.innerloop_solver.max_steps") == 0 )
   {
      p->stokes_params.augmentedlagrangian.innerloop_solver.max_steps = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_stokes_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_navierstokes_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"method") == 0 )
   {
      strncpy(p->navierstokes_params.method, (char*)val, 64 );
      p->navierstokes_params.method[63] = '\0';
   }
   else
   if ( strcmp(field,"eps_steps") == 0 )
   {
      p->navierstokes_params.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"max_steps") == 0 )
   {
      p->navierstokes_params.max_steps = *(int*)val;
   }
   else
   if ( strcmp(field,"outer_method") == 0 )
   {
      strncpy(p->navierstokes_params.outer_method, (char*)val, 64 );
      p->navierstokes_params.outer_method[63] = '\0';
   }
   
   /* PRESSUREMATRIX */
   else
   if ( strcmp(field,"pressurematrix.innerloop_method") == 0 )
   {
      strncpy(p->navierstokes_params.pressurematrix.innerloop_method, (char*)val, 64 );
      p->navierstokes_params.pressurematrix.innerloop_method[63] = '\0';
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.bandwidth_option") == 0 )
   {
      p->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_option = *(int*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.bandwidth_method") == 0 )
   {
      p->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_method = *(int*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.eps_steps") == 0 )
   {
      p->navierstokes_params.pressurematrix.innerloop_solver.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"pressurematrix.innerloop_solver.max_steps") == 0 )
   {
      p->navierstokes_params.pressurematrix.innerloop_solver.max_steps = *(int*)val;
   }   
   
   /* UZAWA */
   else
   if ( strcmp(field,"uzawa.rho") == 0 )
   {
      p->navierstokes_params.uzawa.rho = *(Real*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_method") == 0 )
   {
      strncpy(p->navierstokes_params.uzawa.innerloop_method, (char*)val, 64 );
      p->navierstokes_params.uzawa.innerloop_method[63] = '\0';
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.bandwidth_option") == 0 )
   {
      p->navierstokes_params.uzawa.innerloop_solver.bandwidth_option = *(int*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.bandwidth_method") == 0 )
   {
      p->navierstokes_params.uzawa.innerloop_solver.bandwidth_method = *(int*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.eps_steps") == 0 )
   {
      p->navierstokes_params.uzawa.innerloop_solver.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"uzawa.innerloop_solver.max_steps") == 0 )
   {
      p->navierstokes_params.uzawa.innerloop_solver.max_steps = *(int*)val;
   }
   
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_navierstokes_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_matrix_solver_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"eps_steps") == 0 )
   {
      p->resol_params.eps_steps = *(Real*)val;
   }
   else
   if ( strcmp(field,"max_steps") == 0 )
   {
      p->resol_params.max_steps = *(int*)val;
   }
   else
   if ( strcmp(field,"nb_steps") == 0 )
   {
      p->resol_params.nb_steps = *(int*)val;
   }
   else
   if ( strcmp(field,"bandwidth_method") == 0 )
   {
      p->resol_params.bandwidth_method = *(BANDWRt_METHOD*)val;
   }
   else
   if ( strcmp(field,"bandwidth_option") == 0 )
   {
      p->resol_params.bandwidth_option = *(BANDWRt_OPTION*)val;
   }
   else
   if ( strcmp(field,"preconditionning") == 0 )
   {
      strncpy(p->resol_params.preconditionning,(char*)val, 16);
      p->resol_params.preconditionning[15] = '\0';
   }
   else
   if ( strcmp(field,"resolution_method") == 0 )
   {
      strncpy(p->resol_params.resolution_method,(char*)val, 16 );
      p->resol_params.resolution_method[15] = '\0';
   }
   else
   if ( strcmp(field,"iter_file") == 0 )
   {
      strncpy(p->resol_params.iter_file, (char*)val, 128 );
      p->resol_params.iter_file[127] = '\0';
   }
   else
   if ( strcmp(field,"iter_info") == 0 )
   {
      p->resol_params.iter_info = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_matrix_solver_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_graphics_output_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"GNUPLOT") == 0 )
   {
      p->graph_params.GNUPLOT = *(int*)val;
   }
   else
   if ( strcmp(field,"GRAPH") == 0 )
   {
      p->graph_params.GRAPH = *(int*)val;
   }
   else
   if ( strcmp(field,"LIBSCIPLOT") == 0 )
   {
      p->graph_params.LIBSCIPLOT = *(int*)val;
   }
   else
   if ( strcmp(field,"MATLAB") == 0 )
   {
      p->graph_params.MATLAB = *(int*)val;
   }
   else
   if ( strcmp(field,"SILO") == 0 )
   {
      p->graph_params.SILO = *(int*)val ;
   }
   else
   if ( strcmp(field,"VTK") == 0 )
   {
      p->graph_params.VTK = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_graphics_output_params");
   }
   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_graphics_interactiv1Dplots_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"ENGINE") == 0 )
   {
      strncpy(p->graph_interactiv1Dplots_params.ENGINE , (char*)val, 16-1 );
   }
   else
   if ( strcmp(field,"DRIVER") == 0 )
   {
      strncpy(p->graph_interactiv1Dplots_params.DRIVER , (char*)val, 16-1 );
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_X") == 0 )
   {
      p->graph_interactiv1Dplots_params.SIZE_WINDOW_X = *(int*)val;
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_Y") == 0 )
   {
      p->graph_interactiv1Dplots_params.SIZE_WINDOW_Y = *(int*)val;
   }
   else
   if ( strcmp(field,"SIZE_LABELS") == 0 )
   {
      p->graph_interactiv1Dplots_params.SIZE_LABELS = *(Real*)val;
   }
   else
   if ( strcmp(field,"NX") == 0 )
   {
      p->graph_interactiv1Dplots_params.NX = *(int*)val;
   }
   else
   if ( strcmp(field,"NY") == 0 )
   {
      p->graph_interactiv1Dplots_params.NY = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_graphics_interactiv1Dplots_params");
   }   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_graphics_interactiv2Dplots_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"ENGINE") == 0 )
   {
      strncpy(p->graph_interactiv2Dplots_params.ENGINE , (char*)val, 16-1 );
   }
   else
   if ( strcmp(field,"DRIVER") == 0 )
   {
      strncpy(p->graph_interactiv2Dplots_params.DRIVER , (char*)val, 16-1 );
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_X") == 0 )
   {
      p->graph_interactiv2Dplots_params.SIZE_WINDOW_X = *(int*)val;
   }
   else
   if ( strcmp(field,"SIZE_WINDOW_Y") == 0 )
   {
      p->graph_interactiv2Dplots_params.SIZE_WINDOW_Y = *(int*)val;
   }
   else
   if ( strcmp(field,"SIZE_LABELS") == 0 )
   {
      p->graph_interactiv2Dplots_params.SIZE_LABELS = *(Real*)val;
   }
   else
   if ( strcmp(field,"NX") == 0 )
   {
      p->graph_interactiv2Dplots_params.NX = *(int*)val;
   }
   else
   if ( strcmp(field,"NY") == 0 )
   {
      p->graph_interactiv2Dplots_params.NY = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_graphics_interactiv2Dplots_params");
   }   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_time_scheme_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"QSTEP") == 0 )
   {
      p->timescheme_params.QSTEP = *(int*)val;
   }
   else
   if ( strcmp(field,"ALPHA_STEP") == 0 )
   {
      p->timescheme_params.ALPHA_STEP = *(int*)val;
   }
   else
   if ( strcmp(field,"BETA_STEPS") == 0 )
   {
      p->timescheme_params.BETA_STEPS = *(int*)val;
   }
   else
   if ( strcmp(field,"GAMMA_STEP") == 0 )
   {
      p->timescheme_params.GAMMA_STEP = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_time_scheme_params");
   }   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_miscellaneous_graphics(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"ispmatpattern") == 0 )
   {
      p->misc_graphics.ispmatpattern = *(int*)val;
   }
   else
   if ( strcmp(field,"igeomview") == 0 )
   {
      p->misc_graphics.igeomview = *(int*)val;
   }
   else
   if ( strcmp(field,"igeomcontourview") == 0 )
   {
      p->misc_graphics.igeomcontourview = *(int*)val;
   }
   else
   if ( strcmp(field,"igeomtotalview") == 0 )
   {
      p->misc_graphics.igeomtotalview = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_miscellaneous_graphics");
   }   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam0_cat_miscellaneous_params(PARAMS* p, const char* field, void*  val)
{
   if ( strcmp(field,"itprint") == 0 )
   {
      p->misc_params.itprint = *(int*)val;
   }
   else
   if ( strcmp(field,"itsave") == 0 )
   {
      p->misc_params.itsave = *(int*)val;
   }
   else
   if ( strcmp(field,"itstat") == 0 )
   {
      p->misc_params.itstat = *(int*)val;
   }
   else
   if ( strcmp(field,"itgraph") == 0 )
   {
      p->misc_params.itgraph = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam0_cat_miscellaneous_params");
   }   
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void Params_set_oneparam1(PARAMS* p, const char* category, const char* field, int idx1, void*  val)
{
   if ( !p        ) error(E_NULL, "Params_set_oneparam1");
   if ( !category ) error(E_NULL, "Params_set_oneparam1");
   if ( !field    ) error(E_NULL, "Params_set_oneparam1");
   if ( !val      ) error(E_NULL, "Params_set_oneparam1");


   /* check */
   if ( !Params_check_oneparam1_arguments(category, field, idx1) )
   {
      return;
   }

   /* set */
   if ( strcmp(category,"right_hand_side_params") == 0 )
   {
      Params_set_oneparam1_cat_right_hand_side_params(p, field, idx1, val);
   }
   else
   if ( strcmp(category,"initial_condition_params") == 0 )
   {
      Params_set_oneparam1_cat_initial_condition_params(p, field, idx1, val);
   }
   else
   if ( strcmp(category,"exact_solution_params") == 0 )
   {
      Params_set_oneparam1_cat_exact_solution_params(p, field, idx1, val);
   }
   else
   if ( strcmp(category,"graphics_interactiv1Dplots_params") == 0 )
   {
      Params_set_oneparam1_cat_graphics_interactiv1Dplots_params(p, field, idx1, val);
   }
   else
   if ( strcmp(category,"graphics_interactiv2Dplots_params") == 0 )
   {
      Params_set_oneparam1_cat_graphics_interactiv2Dplots_params(p, field, idx1, val);
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam1");
   }

   return;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam1_cat_right_hand_side_params(PARAMS* p, const char* field, int idx1, void*  val)
{
   /*
   if ( strcmp(field,"rhs") == 0 )
   {
      p->rhs_params.rhs[idx1] = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam1_cat_right_hand_side_params");
   }*/
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam1_cat_initial_condition_params(PARAMS* p, const char* field, int idx1, void*  val)
{
   /*
   if ( strcmp(field,"ic") == 0 )
   {
      p->ic_params.ini[idx1] = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam1_cat_initial_condition_params");
   }
   */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam1_cat_exact_solution_params(PARAMS* p, const char* field, int idx1, void*  val)
{
   /*
   if ( strcmp(field,"exact_sol") == 0 )
   {
      p->exactsol_params.exact_sol[idx1] = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam1_cat_exact_solution_params");
   }
   */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam1_cat_graphics_interactiv1Dplots_params(PARAMS* p, const char* field, int idx1, void*  val)
{
   {
      error(E_UNKNOWN, "Params_set_oneparam1_cat_graphics_interactiv1Dplots_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam1_cat_graphics_interactiv2Dplots_params(PARAMS* p, const char* field, int idx1, void*  val)
{
   {
      error(E_UNKNOWN, "Params_set_oneparam1_cat_graphics_interactiv2Dplots_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

void Params_set_oneparam2(PARAMS* p, const char* category, const char* field, int idx1, int idx2, void* val)
{
   if ( !p        ) error(E_NULL, "Params_set_oneparam2");
   if ( !category ) error(E_NULL, "Params_set_oneparam2");
   if ( !field    ) error(E_NULL, "Params_set_oneparam2");
   if ( !val      ) error(E_NULL, "Params_set_oneparam2");

   /* check */
   if ( !Params_check_oneparam2_arguments(category, field, idx1, idx2) )
   {
      return;
   }

   /* set */
   if ( strcmp(category,"convective_terms_params") == 0 )
   {
      Params_set_oneparam2_cat_convective_terms_params(p, field, idx1, idx2, val);
   }
   else
   if ( strcmp(category,"boundary_conditions_params") == 0 )
   {
      Params_set_oneparam2_cat_boundary_conditions_params(p, field, idx1, idx2, val);
   }
   else
   if ( strcmp(category,"graphics_interactiv1D_params") == 0 )
   {
      Params_set_oneparam2_cat_graphics_interactiv1Dplots_params(p, field, idx1, idx2, val);
   }
   else
   if ( strcmp(category,"graphics_interactiv2D_params") == 0 )
   {
      Params_set_oneparam2_cat_graphics_interactiv2Dplots_params(p, field, idx1, idx2, val);
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam2");
   }

   return;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam2_cat_convective_terms_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val)
{
   /*
   if ( strcmp(field,"adv1") == 0 )
   {
      p->adv_params.adv1[idx1][idx2] = *(int*)val;
   }
   else
   if ( strcmp(field,"adv2") == 0 )
   {
      p->adv_params.adv2[idx1][idx2] = *(int*)val;
   }
   else
   if ( strcmp(field,"adv3") == 0 )
   {
      p->adv_params.adv3[idx1][idx2] = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam2_cat_convective_terms_params");
   }
   */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam2_cat_boundary_conditions_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val)
{
   if ( strcmp(field,"TabBCMask") == 0 )
   {
      p->bc_params.TabBCMask[idx1][idx2]  = *(int*)val;
   }
   /*
   else
   if ( strcmp(field,"TabPhi_BCdirichlet") == 0 )
   {
      p->bc_params.TabPhi_BCdirichlet[idx1][idx2]  = *(int*)val;
   }
   else
   if ( strcmp(field,"TabPhi_BCneumann") == 0 )
   {
      p->bc_params.TabPhi_BCneumann[idx1][idx2] = *(int*)val;
   }
   else
   if ( strcmp(field,"TabPhi_BCcauchy") == 0 )
   {
      p->bc_params.TabPhi_BCcauchy[idx1][idx2]   = *(int*)val;
   }
   else
   if ( strcmp(field,"TabPhi_BCrobin1") == 0 )
   {
      p->bc_params.TabPhi_BCrobin1[idx1][idx2]   = *(int*)val;
   }
   else
   if ( strcmp(field,"TabPhi_BCrobin2") == 0 )
   {
      p->bc_params.TabPhi_BCrobin2[idx1][idx2]   = *(int*)val;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam2_cat_boundary_conditions_params");
   }
    */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam2_cat_graphics_interactiv1Dplots_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val)
{
   if ( strcmp(field,"LABEL_LEGEND") == 0 )
   {
      strncpy(p->graph_interactiv1Dplots_params.LABEL[idx1][idx2].LEGEND , (char*)val, 200-1 );
   }
   else
   if ( strcmp(field,"LABEL_AXE_X") == 0 )
   {
      strncpy(p->graph_interactiv1Dplots_params.LABEL[idx1][idx2].AXE_X , (char*)val, 50-1 );
   }
   else
   if ( strcmp(field,"LABEL_AXE_Y") == 0 )
   {
      strncpy(p->graph_interactiv1Dplots_params.LABEL[idx1][idx2].AXE_Y , (char*)val, 50-1 );
   }
   else
   if ( strcmp(field,"WINDOW_X_MAX") == 0 )
   {
      p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].X_MAX = *(Real*)val;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MAX") == 0 )
   {
      p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].Y_MAX = *(Real*)val ;
   }
   else
   if ( strcmp(field,"WINDOW_X_MIN") == 0 )
   {
      p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].X_MIN = *(Real*)val;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MIN") == 0 )
   {
      p->graph_interactiv1Dplots_params.WINDOW[idx1][idx2].Y_MIN = *(Real*)val ;
   }
   else
   if ( strcmp(field,"MARKER") == 0 )
   {
      p->graph_interactiv1Dplots_params.MARKER[idx1][idx2] = *(int*)val ;
   }
   else
   if ( strcmp(field,"MARKER_END") == 0 )
   {
      p->graph_interactiv1Dplots_params.MARKER_END[idx1][idx2] = *(int*)val ;
   }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam2_cat_graphics_interactiv1Dplots_params");
   }
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_set_oneparam2_cat_graphics_interactiv2Dplots_params(PARAMS* p, const char* field, int idx1, int idx2, void*  val)
{
   if ( strcmp(field,"LABEL_LEGEND") == 0 )
   {
      strncpy(p->graph_interactiv2Dplots_params.LABEL[idx1][idx2].LEGEND , (char*)val, 256-1 );
   }
   else
   if ( strcmp(field,"WINDOW_X_MAX") == 0 )
   {
      p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].X_MAX = *(Real*)val;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MAX") == 0 )
   {
      p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].Y_MAX = *(Real*)val ;
   }
   else
   if ( strcmp(field,"WINDOW_X_MIN") == 0 )
   {
      p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].X_MIN = *(Real*)val;
   }
   else
   if ( strcmp(field,"WINDOW_Y_MIN") == 0 )
   {
      p->graph_interactiv2Dplots_params.WINDOW[idx1][idx2].Y_MIN = *(Real*)val ;
   }
   else
   if ( strcmp(field,"NB_LEVELS") == 0 )
   {
      p->graph_interactiv2Dplots_params.NB_LEVELS[idx1][idx2] = *(int*)val ;
   }
   else
   if ( strcmp(field,"LEVELS") == 0 )
   {
      p->graph_interactiv2Dplots_params.LEVELS[idx1][idx2] = v_copy(val, NULL) ;
   }
    else
    if ( strcmp(field,"GRAPHICS_TYPE") == 0 )
    {
        strncpy(p->graph_interactiv2Dplots_params.GRAPHICS_TYPE[idx1][idx2] , (char*)val, 16-1 );
    }
   else
   {
      error(E_UNKNOWN, "Params_set_oneparam2_cat_graphics_interactiv2Dplots_params");
   }
}


/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

