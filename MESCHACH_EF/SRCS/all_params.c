/** \file all_params.c
 *
 * Contains the routines for setting-getting values of all parameters
 *
 */

#include "MESCHACH/INCLUDES/err.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/eigen.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/all_params_datas.h"
#include "MESCHACH_EF/INCLUDES/all_params_json_config.h"
#include "MESCHACH_EF/INCLUDES/all_params_json_utils.h"

#include "MESCHACH_EF/INCLUDES/functions_definitions.h"


static PARAMS*  MyParams[NBPARAMS];

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_init_physical_parameters(PARAMS* p);
static void Params_init_time_parameters(PARAMS* p);
static void Params_init_finite_elements(PARAMS* p);
static void Params_init_geometry(PARAMS* p);
static void Params_init_boundary_conditions(PARAMS* p);
static void Params_init_right_hand_side(PARAMS* p);
static void Params_init_initial_condition(PARAMS* p);
static void Params_init_convective_terms(PARAMS* p);
static void Params_init_matrix_solver(PARAMS* p);
static void Params_init_eigen_solver(PARAMS* p);
static void Params_init_stokes_solver(PARAMS* p);
static void Params_init_navierstokes_solver(PARAMS* p);
static void Params_init_graphics_output(PARAMS* p);
static void Params_init_graphics_interactiv1D_plots(PARAMS* p);
static void Params_init_graphics_interactiv2D_plots(PARAMS* p);
static void Params_init_time_scheme(PARAMS* p);
static void Params_init_miscellaneous_params(PARAMS* p);
static void Params_init_miscellaneous_graphics(PARAMS* p);

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

PARAMS* Params_setup_from_file(const char* input_file)
{
   if ( !input_file ) error(E_NULL, "Params_setup_from_file");
   
   char validation_output[8156];
   char schema[] = "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/JSON_SCHEMAS/SCHEMA_PDE_PROBLEM.json";

   int rc = json_check_data(input_file, schema, validation_output);
   
   json_t *config = json_load_xfile(input_file);

   PARAMS *p = json_config_to_params(config);
   
   return p;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

PARAMS* Params_append_from_string(PARAMS* p, const char* text)
{
   size_t flags = 0;
   json_error_t error;
   
   json_t *config = json_loads(text, 0, &error);
   
   if ( config != NULL )
   {
      json_config_append_to_params(p, config);
   }

   return p;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void Params_init_physical_parameters(PARAMS* p)
{
   p->phys_params.epsilon = 0.0;
   p->phys_params.sigma   = 0.0;
   p->phys_params.kappa   = 0.0;
}

static void Params_init_time_parameters(PARAMS* p)
{
   p->time_params.DT      = 0.0;
   p->time_params.TPS_INI = 0.0;
   p->time_params.TPS_FIN = 0.0;
}

static void Params_init_finite_elements(PARAMS* p)
{
   strncpy(p->ef_params.name_ef,"", 2);
}

static void Params_init_geometry(PARAMS* p)
{
   strncpy(p->geom_params.meshfile, "", 2);  /* the name of the file */
   strncpy(p->geom_params.meshname, "", 2);  /* name of the mesh inside the meshfile */
   strncpy(p->geom_params.meshtype, "", 2);
   /* 3D: "gmsh" , "cube" meshes                                */
   /* 2D: "quad1" , "quad2" , "quad3" , "emc2" or "gmsh" meshes */
   /* 1D: "periodic" or "non-periodic"                          */

   p->geom_params.xmin = 0.0;
   p->geom_params.xmax = 1.0;

   p->geom_params.nx   = 100;

   strcpy(p->geom_params.distribution, "UNIFORM");
}

static void Params_init_boundary_conditions(PARAMS* p)
{
   int i,k;
   
   p->bc_params.nb_BC = 0;
   
   for (k=0; k<AXEe_NUMBER; k++)
      for (i=0; i<NB_BOUNDARYCONDITIONS; i++)
      {
         p->bc_params.TabBCMask[k][i] = -1;
      }
   
   for (k=0; k<AXEe_NUMBER; k++)
      for (i=0; i<NB_BOUNDARYCONDITIONS; i++)
      {
         p->bc_params.TabPhi_BCdirichlet[k][i] = -1;
         
         p->bc_params.TabPhi_BCneumann[k][i]   = -1;
         
         p->bc_params.TabPhi_BCcauchy[k][i]    = -1;
         
         p->bc_params.TabPhi_BCrobin1[k][i]    = -1;
         p->bc_params.TabPhi_BCrobin2[k][i]    = -1;
      }
}

static void Params_init_right_hand_side(PARAMS* p)
{
   p->rhs_params.rhs[AXEe_X] = 0;
   p->rhs_params.rhs[AXEe_Y] = 0;
   p->rhs_params.rhs[AXEe_Z] = 0;
}

static void Params_init_initial_condition(PARAMS* p)
{
   p->ic_params.ic[AXEe_X] = 0;
   p->ic_params.ic[AXEe_Y] = 0;
   p->ic_params.ic[AXEe_Z] = 0;
}

static void Params_init_convective_terms(PARAMS* p)
{
   int i,k;
   
   strncpy(p->adv_params.method, "SUPG", 5);
   p->adv_params.method[5] = '\0';
   p->adv_params.rho_stab  = 0.0;
   
   p->adv_params.delta     = 0.0;
   
   for (k=0; k<AXEe_NUMBER; k++)
   {
      for (i=0; i<AXEe_NUMBER; i++)
      {
         p->adv_params.adv1[k][i] = 0;
         p->adv_params.adv2[k][i] = 0;
         p->adv_params.adv3[k][i] = 0;
      }
   }
}

static void Params_init_matrix_solver(PARAMS* p)
{
   strncpy(p->resol_params.resolution_method, "", 2);
   strncpy(p->resol_params.preconditionning, "", 2);
   
   p->resol_params.bandwidth_method = BANDWRe_NULL;
   p->resol_params.bandwidth_option = BANDWRe_NO_OPTION;
   
   p->resol_params.max_steps = 100;
   p->resol_params.nb_steps  = 0;
   p->resol_params.eps_steps = 1.0e-12;
   
   strncpy( p->resol_params.iter_file, "", 2);
   p->resol_params.iter_info = 0;
}

static void Params_init_eigen_solver(PARAMS* p)
{
   p->eigen_params.eigen_problem  = EIGENe_GENERALIZED_SYMMETRIC_1;
   p->eigen_params.eigen_option   = EIGENe_ALL;
   
   p->eigen_params.nb_eig = 20;
   
   p->eigen_params.lower_ibound = 1;
   p->eigen_params.upper_ibound = 20;
   
   p->eigen_params.lower_rbound = 1.1;
   p->eigen_params.upper_rbound = 500.0;
}

static void Params_init_stokes_solver(PARAMS* p)
{
   strcpy(p->stokes_params.method, "");
   /* PRESSUREMATRIX */
   p->stokes_params.pressurematrix.eps_steps = 1.0e-6;
   p->stokes_params.pressurematrix.max_steps = 100;
   strcpy(p->stokes_params.pressurematrix.innerloop_method, "");    /**< "DIRECT-METHOD" "ITERATIV-METHOD" */
   p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option = 0;
   p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method = 0;
   p->stokes_params.pressurematrix.innerloop_solver.eps_steps = 1.0e-8;
   p->stokes_params.pressurematrix.innerloop_solver.max_steps = 100;
   
   /* UZAWA */
   p->stokes_params.uzawa.rho = 1.0;
   p->stokes_params.uzawa.eps_steps = 1.0e-6;
   p->stokes_params.uzawa.max_steps = 100;
   p->stokes_params.uzawa.innerloop_solver.bandwidth_option = 0;
   p->stokes_params.uzawa.innerloop_solver.bandwidth_method = 0;
   p->stokes_params.uzawa.innerloop_solver.eps_steps = 1.0e-8;
   p->stokes_params.uzawa.innerloop_solver.max_steps = 100;
   
   /* AUGMENTEDLAGR */
   p->stokes_params.augmentedlagrangian.rho = 1.0;
   p->stokes_params.augmentedlagrangian.eps_steps = 1.0e-6;
   p->stokes_params.augmentedlagrangian.max_steps = 100;
   strcpy(p->stokes_params.augmentedlagrangian.innerloop_method, "");    /**< "DIRECT-METHOD" "ITERATIV-METHOD" */
   p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_option = 0;
   p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_method = 0;
   p->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps = 1.0e-8;
   p->stokes_params.augmentedlagrangian.innerloop_solver.max_steps = 100;
}

static void Params_init_navierstokes_solver(PARAMS* p)
{
   strncpy(p->navierstokes_params.method, "", 2);
   strncpy(p->navierstokes_params.outer_method, "", 2);
   p->navierstokes_params.eps_steps = 1.0;
   p->navierstokes_params.max_steps = 1;
   
   /* PRESSUREMATRIX */
   p->navierstokes_params.pressurematrix.eps_steps = 1.0e-6;
   p->navierstokes_params.pressurematrix.max_steps = 100;
   strcpy(p->navierstokes_params.pressurematrix.innerloop_method, "");    /**< "DIRECT-METHOD" "ITERATIV-METHOD" */
   p->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_option = 0;
   p->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_method = 0;
   p->navierstokes_params.pressurematrix.innerloop_solver.eps_steps = 1.0e-8;
   p->navierstokes_params.pressurematrix.innerloop_solver.max_steps = 100;
   
   /* UZAWA */
   p->navierstokes_params.uzawa.rho = 1.0;
   p->navierstokes_params.uzawa.eps_steps = 1.0e-6;
   p->navierstokes_params.uzawa.max_steps = 100;
   p->navierstokes_params.uzawa.innerloop_solver.bandwidth_option = 0;
   p->navierstokes_params.uzawa.innerloop_solver.bandwidth_method = 0;
   p->navierstokes_params.uzawa.innerloop_solver.eps_steps = 1.0e-8;
   p->navierstokes_params.uzawa.innerloop_solver.max_steps = 100;
}

static void Params_init_graphics_output(PARAMS* p)
{
   p->graph_params.GNUPLOT    = 0;
   p->graph_params.GRAPH      = 0;
   p->graph_params.LIBSCIPLOT = 0;
   p->graph_params.MATLAB     = 0;
   p->graph_params.PGPLOT     = 0;
   p->graph_params.VOGLE      = 0;
   p->graph_params.MATPLOTLIB = 0;
   p->graph_params.SILO       = 0;
   p->graph_params.VTK        = 0;
}

static void Params_init_graphics_interactiv1D_plots(PARAMS* p)
{
   int i,j;
   
   strncpy(p->graph_interactiv1Dplots_params.DRIVER, "X", 2);
   
   p->graph_interactiv1Dplots_params.SIZE_WINDOW_X = 700;
   p->graph_interactiv1Dplots_params.SIZE_WINDOW_Y = 500;
   
   p->graph_interactiv1Dplots_params.SIZE_LABELS = 2;
   
   p->graph_interactiv1Dplots_params.NX = 1;
   p->graph_interactiv1Dplots_params.NY = 1;
   
   /* */
   for (i=0; i<=GRAPHIC_NB_SUB_WINDOWS; i++)
   {
      for (j=0; j<=GRAPHIC_NB_SUB_WINDOWS; j++)
      {
         strncpy(p->graph_interactiv1Dplots_params.LABEL[i][j].AXE_X,"???", 4);
         strncpy(p->graph_interactiv1Dplots_params.LABEL[i][j].AXE_Y,"???", 4);
         
         strncpy(p->graph_interactiv1Dplots_params.LABEL[i][j].LEGEND,"???", 4);
         
         p->graph_interactiv1Dplots_params.WINDOW[i][j].X_MIN = 0.0;
         p->graph_interactiv1Dplots_params.WINDOW[i][j].X_MAX = 0.0;
         p->graph_interactiv1Dplots_params.WINDOW[i][j].Y_MIN = 0.0;
         p->graph_interactiv1Dplots_params.WINDOW[i][j].Y_MAX = 0.0;
         
         /* misc */
         p->graph_interactiv1Dplots_params.MARKER[i][j]           = 0;
         p->graph_interactiv1Dplots_params.MARKER_END[i][j]       = 0;
      }
   }
}

static void Params_init_graphics_interactiv2D_plots(PARAMS* p)
{
   int i,j;
   
   strncpy(p->graph_interactiv2Dplots_params.DRIVER, "X", 2);
   
   p->graph_interactiv2Dplots_params.SIZE_WINDOW_X = 700;
   p->graph_interactiv2Dplots_params.SIZE_WINDOW_Y = 500;
   
   p->graph_interactiv2Dplots_params.SIZE_LABELS = 2;
   
   p->graph_interactiv2Dplots_params.NX = 1;
   p->graph_interactiv2Dplots_params.NY = 1;
   
   /* */
   for (i=0; i<=GRAPHIC_NB_SUB_WINDOWS; i++)
   {
      for (j=0; j<=GRAPHIC_NB_SUB_WINDOWS; j++)
      {
          strncpy(p->graph_interactiv2Dplots_params.LABEL[i][j].LEGEND,"???", 4);
         
          p->graph_interactiv2Dplots_params.WINDOW[i][j].X_MIN = 0.0;
          p->graph_interactiv2Dplots_params.WINDOW[i][j].X_MAX = 0.0;
          p->graph_interactiv2Dplots_params.WINDOW[i][j].Y_MIN = 0.0;
          p->graph_interactiv2Dplots_params.WINDOW[i][j].Y_MAX = 0.0;
         
          /* contours specifications */
          p->graph_interactiv2Dplots_params.NB_LEVELS[i][j] = 10;
          p->graph_interactiv2Dplots_params.LEVELS[i][j] = NULL;
      }
   }
}


static void Params_init_time_scheme(PARAMS* p)
{
   p->timescheme_params.QSTEP = 1;
   
   p->timescheme_params.ALPHA_STEP = 0;
   p->timescheme_params.BETA_STEPS = 0;
   p->timescheme_params.GAMMA_STEP = 0;
}

static void Params_init_miscellaneous_params(PARAMS* p)
{
   p->misc_params.itprint = 0;
   p->misc_params.itsave  = 0;
   p->misc_params.itstat  = 0;
   p->misc_params.itgraph = 0;
}

static void Params_init_exact_solution_params(PARAMS* p)
{   
   p->exactsol_params.exact_sol[0] = -1;
   p->exactsol_params.exact_sol[1] = -1;
   p->exactsol_params.exact_sol[2] = -1;
}

static void Params_init_miscellaneous_graphics(PARAMS* p)
{   
   p->misc_graphics.ispmatpattern = 0;
   p->misc_graphics.igeomview = 0;
   p->misc_graphics.igeomcontourview = 0;
   p->misc_graphics.igeomtotalview = 0;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

PARAMS* Params_get(void)
{
   PARAMS* p;
   
   if ( (p=NEW(PARAMS)) == (PARAMS *)NULL ) error(E_MEM, "Params_get");
   
   /* init default datas */
   strncpy(p->problem , "", 2);
   /* logger */
   p->logger = NULL;
   
   Params_init_physical_parameters(p);
   Params_init_time_parameters(p);
   Params_init_finite_elements(p);
   Params_init_geometry(p);
   Params_init_boundary_conditions(p);
   Params_init_right_hand_side(p);
   Params_init_initial_condition(p);
   Params_init_convective_terms(p);
   Params_init_exact_solution_params(p);
   Params_init_matrix_solver(p);
   Params_init_eigen_solver(p);
   Params_init_stokes_solver(p);
   Params_init_navierstokes_solver(p);
   Params_init_graphics_output(p);
   Params_init_graphics_interactiv1D_plots(p);
   Params_init_graphics_interactiv2D_plots(p);
   Params_init_time_scheme(p);
   Params_init_miscellaneous_params(p);
   Params_init_miscellaneous_graphics(p);
   
   return p;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

int  Params_free(PARAMS* p)
{
   if ( !p ) error(E_NULL, "Params_free");
   
   if (p->logger != NULL)
   {
      fclose(p->logger);
   }
   
   return EXIT_SUCCESS;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

PARAMS* Params_get_staticparam(int i)
{
   if ( (i<0)||(i>=NBPARAMS ) )   error(E_RANGE, "Params_get_staticparam");
   
   return MyParams[i];
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

PARAMS* Params_set_staticparam(PARAMS* p, int i)
{
   if ( !p )                      error(E_NULL, "Params_set_staticparam");
   if ( (i<0)||(i>=NBPARAMS ) )   error(E_RANGE, "Params_set_staticparam");
   
   printf("Set a static param ... \n");
   return MyParams[i] = p;
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

int Params_check_oneparam0_arguments(const char* category, const char* field)
{
   int k=0;
   
   while ( params_list0[k].p1 != NULL )
   {
      if ( (strcmp(category, params_list0[k].p1) == 0) && (strcmp(field, params_list0[k].p2) == 0) )
      {
         return 1; /* found! */
      }

      k++;
   }

   error(E_UNKNOWN, "Params_check_oneparam_arguments");

   return 0; /* not found? */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

int Params_check_oneparam1_arguments(const char* category, const char* field, int idx1 )
{
   int k=0;

   while ( params_list1[k].p1 != NULL )
   {
      if ( (strcmp(category, params_list1[k].p1) == 0) && (strcmp(field, params_list1[k].p2) == 0) )
      {
         if ( (idx1 < params_list1[k].idx_min) && (idx1 > params_list1[k].idx_max) ) /* check idx */
         {
            error(E_UNKNOWN, "Params_check_oneparam1_arguments");
            return 0;
         }

         return 1; /* found! */
      }
      k++;
   }

   error(E_UNKNOWN, "Params_check_oneparam1_arguments");

   return 0; /* not found? */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

int Params_check_oneparam2_arguments(const char* category, const char* field, int idx1, int idx2 )
{
   int k=0;

   while ( params_list2[k].p1 != NULL )
   {
//      if ( (strcmp(category, params_list2[k].p1) == 0) && (strcmp(field, params_list2[k].p2) == 0) )
      {
         if ( (idx1 < params_list2[k].idx1_min) && (idx1 > params_list2[k].idx1_max) ) /* check idx */
         {
            error(E_UNKNOWN, "Params_check_oneparam2_arguments");
            return 0;
         }

         if ( (idx2 < params_list2[k].idx2_min) && (idx2 > params_list2[k].idx2_max) ) /* check idx */
         {
            error(E_UNKNOWN, "Params_check_oneparam2_arguments");
            return 0;
         }
         
         return 1; /* found! */
      }
      k++;
   }

   error(E_UNKNOWN, "Params_check_oneparam2_arguments");

   return 0; /* not found? */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

int Params_get_valuetype(const char* category, const char* field)
{
   int k=0;

   while ( params_list0[k].p1 != NULL )
   {
      if ( (strcmp(category, params_list0[k].p1) == 0) && (strcmp(field, params_list0[k].p2) == 0) )
      {
         return params_list0[k].type;
      }
      k++;
   }

   k = 0;
   while ( params_list1[k].p1 != NULL )
   {
      if ( (strcmp(category, params_list1[k].p1) == 0) && (strcmp(field, params_list1[k].p2) == 0) )
      {
         return params_list1[k].type;
      }
      k++;
   }

   k = 0;
   while ( params_list2[k].p1 != NULL )
   {
      if ( (strcmp(category, params_list2[k].p1) == 0) && (strcmp(field, params_list2[k].p2) == 0) )
      {
         return params_list2[k].type;
      }
      k++;
   }

   printf("category : %s  ** field : %s\n", category, field);
   error(E_UNKNOWN, "Params_get_valuetype -- field in category unknown --");

   /* if here, we did not found any data with <category,field> */
   return TYPEd_UNDEFINED; /* not found ? */
}

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

