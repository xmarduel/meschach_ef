
/** \file all_params_json_config.c
 *
 * json config file to meschach pde parameters!
 *
 *
 */

#include "MESCHACH_EF/INCLUDES/all_params.h"

#include "MESCHACH_EF/INCLUDES/all_params_json_config.h"
#include "MESCHACH_EF/INCLUDES/all_params_json_utils.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/eigen.h"

#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"


static void json_config_PARAMS_finite_element(PARAMS* p, const json_t* config);
static void json_config_PARAMS_physical_parameters(PARAMS* p, const json_t* config);
static void json_config_PARAMS_time_parameters(PARAMS* p, const json_t* config);
static void json_config_PARAMS_time_scheme(PARAMS* p, const json_t* config);
static void json_config_PARAMS_geometry(PARAMS* p, const json_t* config);
static void json_config_PARAMS_boundary_conditions(PARAMS* p, const json_t* config);
static void json_config_PARAMS_right_hand_side(PARAMS* p, const json_t* config);
static void json_config_PARAMS_initial_condition(PARAMS* p, const json_t* config);
static void json_config_PARAMS_solexacte(PARAMS* p, const json_t* config);
static void json_config_PARAMS_convective_terms(PARAMS* p, const json_t* config);
static void json_config_PARAMS_pde_solver(PARAMS* p, const json_t* config);
static void json_config_PARAMS_graphics_output(PARAMS* p, const json_t* config);
static void json_config_PARAMS_graphics_interactiv1D_plots(PARAMS* p, const json_t* config);
static void json_config_PARAMS_graphics_interactiv2D_plots(PARAMS* p, const json_t* config);
static void json_config_PARAMS_miscellaneous_params(PARAMS* p, const json_t* config);
static void json_config_PARAMS_miscellaneous_graphics(PARAMS* p, const json_t* config);
static void json_config_PARAMS_io_files(PARAMS* p, const json_t* config);

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int mem_attach_list8(void)
{
   return 0;
   //return mem_attach_list(MY_LIST8, NB_STRUCTS8, my_names8, my_frees8, my_tnums8);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int err_attach_list8(void)
{
   return err_list_attach(MY_LIST8, NB_ERRS8, my_err_mesg8 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int warn_attach_list8(void)
{
   return warn_list_attach(MY_LIST8, NB_ERRS8, my_warn_mesg8 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int err_warn_attach_lists8(void)
{
   err_attach_list8();
   warn_attach_list8();

   return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void json_config_PARAMS_finite_element(PARAMS* p, const json_t* config)
{
   json_t *PDE_RESOLUTION = json_object_get(config, "PDE_RESOLUTION");

   if ( PDE_RESOLUTION != NULL )
   {
      strncpy(p->ef_params.name_ef, json_object_get_string_value(PDE_RESOLUTION, "EF"), 16);
   }
}

static void json_config_PARAMS_physical_parameters(PARAMS* p, const json_t* config)
{
   json_t *PDE_PARAMATERS = json_object_get(config, "PDE_PARAMATERS");

   if ( PDE_PARAMATERS != NULL )
   {
      if ( json_object_get(PDE_PARAMATERS, "EPSILON") != NULL ) p->phys_params.epsilon = json_object_get_real_value(PDE_PARAMATERS, "EPSILON");
      if ( json_object_get(PDE_PARAMATERS, "SIGMA")   != NULL ) p->phys_params.sigma   = json_object_get_real_value(PDE_PARAMATERS, "SIGMA");
      if ( json_object_get(PDE_PARAMATERS, "KAPPA")   != NULL ) p->phys_params.kappa   = json_object_get_real_value(PDE_PARAMATERS, "KAPPA");
   }
}

static void json_config_PARAMS_time_parameters(PARAMS* p, const json_t* config)
{
   json_t *PDE_TIME_PARAMETERS = json_object_get(config, "PDE_TIME_PARAMETERS");

   if ( PDE_TIME_PARAMETERS == NULL )
   {
      return;
   }

   if ( json_object_get(PDE_TIME_PARAMETERS, "TIME_INI") != NULL ) p->time_params.TPS_INI = json_object_get_real_value(PDE_TIME_PARAMETERS, "TIME_INI");
   if ( json_object_get(PDE_TIME_PARAMETERS, "TIME_END") != NULL ) p->time_params.TPS_FIN = json_object_get_real_value(PDE_TIME_PARAMETERS, "TIME_END");
   if ( json_object_get(PDE_TIME_PARAMETERS, "DELTA_T")  != NULL ) p->time_params.DT      = json_object_get_real_value(PDE_TIME_PARAMETERS, "DELTA_T");
}

static void json_config_PARAMS_time_scheme(PARAMS* p, const json_t* config)
{
   json_t *PDE_TIME_SCHEME_PARAMETERS = json_object_get(config, "PDE_TIME_SCHEME_PARAMETERS");

   if ( PDE_TIME_SCHEME_PARAMETERS == NULL )
   {
      return;
   }

   if ( json_object_get(PDE_TIME_SCHEME_PARAMETERS, "QSTEP")      != NULL ) p->timescheme_params.QSTEP      = json_object_get_real_value(PDE_TIME_SCHEME_PARAMETERS, "QSTEP");
   if ( json_object_get(PDE_TIME_SCHEME_PARAMETERS, "ALPHA_STEP") != NULL ) p->timescheme_params.ALPHA_STEP = json_object_get_real_value(PDE_TIME_SCHEME_PARAMETERS, "ALPHA_STEP");
   if ( json_object_get(PDE_TIME_SCHEME_PARAMETERS, "BETA_STEPS") != NULL ) p->timescheme_params.BETA_STEPS = json_object_get_real_value(PDE_TIME_SCHEME_PARAMETERS, "BETA_STEPS");
   if ( json_object_get(PDE_TIME_SCHEME_PARAMETERS, "GAMMA_STEP") != NULL ) p->timescheme_params.GAMMA_STEP = json_object_get_real_value(PDE_TIME_SCHEME_PARAMETERS, "GAMMA_STEP");
}

static void json_config_PARAMS_geometry(PARAMS* p, const json_t* config)
{
   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");

   if (coords_dims == 1)
   {
      json_t *MESH_1D = json_object_get(config, "MESH_1D");

      if (MESH_1D == NULL)
      {
            return; //error8(E_JSON_MISSING_OBJECT, "MESH_1D");
      }

      json_t *MESH_SPEC = json_object_get(MESH_1D, "MESH_SPEC");

      if (MESH_SPEC == NULL)
      {
         return; //error8(E_JSON_MISSING_OBJECT, "MESH_SPEC");
      }

      const char* MESH_SPEC_VALUE = json_string_value(MESH_SPEC);

      json_t *MESHFILEDEFINITION = json_object_get(MESH_1D, "MESHFILEDEFINITION");
      json_t *MESHDATADEFINITION = json_object_get(MESH_1D, "MESHDATADEFINITION");

      if ( ((strcmp(MESH_SPEC_VALUE, "MESHFILEDEFINITION") == 0) && MESHFILEDEFINITION == NULL) ||
           ((strcmp(MESH_SPEC_VALUE, "MESHDATADEFINITION") == 0) && MESHDATADEFINITION == NULL) )
      {
         error8(E_JSON_MISSING_OBJECT, "MESH_1D # MESHFILEDEFINITION or MESH_1D # MESHDATADEFINITION");
      }

      if ( strcmp(MESH_SPEC_VALUE, "MESHFILEDEFINITION") == 0 )
      {
         json_t *MESHFILE = json_object_get(MESHFILEDEFINITION, "MESHFILE");
         json_t *MESHNAME = json_object_get(MESHFILEDEFINITION, "MESHNAME"); // may be null
         json_t *MESHTYPE = json_object_get(MESHFILEDEFINITION, "MESHTYPE"); // may be null

         if ( json_string_value(MESHTYPE) == NULL )
         {
            if ( MESHDATADEFINITION == NULL )
            {
               error8(E_JSON_MISSING_OBJECT, "MESH_1D # MESHDATADEFINITION");
            }
         }
         else
         {
            if ( strcmp("quad1", json_string_value(MESHTYPE)) || strcmp("quad2", json_string_value(MESHTYPE)) || strcmp("quad3", json_string_value(MESHTYPE)) )
            {
               // meshname must be given
               if ( MESHNAME == NULL )
               {
                  error8(E_JSON_MISSING_OBJECT, "MESH_1D # MESHFILEDEFINITION - MESHNAME");
               }

               // with a value
               if ( MESHNAME == NULL)
               {
                  error8(E_JSON_MISSING_OBJECT, "MESH_1D # MESHFILEDEFINITION - MESHNAME");
               }
               if ( json_string_value(MESHNAME) == NULL )
               {
                  error8(E_JSON_NULL_OBJECT_VALUE, "MESH_1D # MESHFILEDEFINITION - MESHNAME");
               }
            }

            strncpy(p->geom_params.meshfile, json_string_value(MESHFILE), 256);
            if (MESHNAME != NULL && json_string_value(MESHNAME) != NULL) strncpy(p->geom_params.meshname, json_string_value(MESHNAME), 64);
            strncpy(p->geom_params.meshtype, json_string_value(MESHTYPE), 64);
         }

      }

      if ( strcmp(MESH_SPEC_VALUE, "MESHDATADEFINITION") == 0 )
      {
         strncpy(p->geom_params.meshfile, "NULL", 256);

         if (json_object_get_boolean_value(MESHDATADEFINITION, "PERIODIC"))
            p->geom_params.periodicity = 1; // PERIODIC_MESHe
         else
            p->geom_params.periodicity = 0; // NON_PERIODIC_MESHe

         p->geom_params.xmin         = json_object_get_real_value(MESHDATADEFINITION,    "XMIN");
         p->geom_params.xmax         = json_object_get_real_value(MESHDATADEFINITION,    "XMAX");
         p->geom_params.nx           = json_object_get_integer_value(MESHDATADEFINITION, "NX");

         strncpy(p->geom_params.distribution, json_object_get_string_value(MESHDATADEFINITION, "DISTRIBUTION"), 64);
      }
   }

   if ( coords_dims == 2 )
   {
      json_t *MESH_2D = json_object_get(config, "MESH_2D");

      if (MESH_2D == NULL)
      {
            return; // error8(E_JSON_MISSING_OBJECT, "MESH_2D");
      }

      json_t *MESHFILEDEFINITION = json_object_get(MESH_2D, "MESHFILEDEFINITION");

      if (MESHFILEDEFINITION == NULL)
      {
         error8(E_JSON_MISSING_OBJECT, "MESH_2D - MESHFILEDEFINITION");
      }

      strncpy(p->geom_params.meshfile, json_object_get_string_value(MESHFILEDEFINITION, "MESHFILE"), 256);
      strncpy(p->geom_params.meshname, json_object_get_string_value(MESHFILEDEFINITION, "MESHNAME"), 64);
      strncpy(p->geom_params.meshtype, json_object_get_string_value(MESHFILEDEFINITION, "MESHTYPE"), 64);
   }

   if ( coords_dims == 3 )
   {
      json_t *MESH_3D = json_object_get(config, "MESH_3D");

      if (MESH_3D == NULL)
      {
         error8(E_JSON_MISSING_OBJECT, "MESH_3D");
      }

      json_t *MESHFILEDEFINITION = json_object_get(MESH_3D, "MESHFILEDEFINITION");

      if (MESHFILEDEFINITION == NULL)
      {
         error8(E_JSON_MISSING_OBJECT, "MESH_2D - MESHFILEDEFINITION");
      }

      strncpy(p->geom_params.meshfile, json_object_get_string_value(MESHFILEDEFINITION, "MESHFILE"), 256);
      strncpy(p->geom_params.meshname, json_object_get_string_value(MESHFILEDEFINITION, "MESHNAME"), 64);
      strncpy(p->geom_params.meshtype, json_object_get_string_value(MESHFILEDEFINITION, "MESHTYPE"), 64);
   }

}

typedef struct BcInfoType_ {
   int  idx;
   char *bctype;
   char *fundef1;
   char *fundef2;
} BcInfoType;

static void json_config_PARAMS_boundary_conditions(PARAMS* p, const json_t* config)
{
   json_t *PDE_BOUNDARY_CONDITIONS = json_object_get(config, "PDE_BOUNDARY_CONDITIONS");

   p->bc_params.nb_BC       = json_object_get_integer_value(PDE_BOUNDARY_CONDITIONS, "NB_BCS");

   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");
   int target_dims = json_object_get_integer_value(config, "TARGET_DIMS");

   if ( target_dims == 1 && p->bc_params.nb_BC )
   {
      json_t *DIM1 = json_object_get(PDE_BOUNDARY_CONDITIONS, "DIM1");

      if ( DIM1 == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_BOUNDARY_CONDITIONS - DIM1");
      }

      // loop on the data
      size_t index;
      json_t *bc_item;

      json_array_foreach(DIM1, index, bc_item)
      {
         BcInfoType bc_info;

         /* block of code that uses index and value */
         bc_info.idx     = json_object_get_integer_value(bc_item, "IDX");
         bc_info.bctype  = strdup(json_object_get_string_value(bc_item, "BC_TYPE"));

         if ( strcmp(bc_info.bctype, "ROBIN") == 0 )
         {
            json_t *FUNC_DEF  = json_object_get(bc_item, "FUNC_DEF");

            if ( FUNC_DEF != NULL ) {
               json_t *FUNC_DEF1 = json_array_get(FUNC_DEF, 0);
               json_t *FUNC_DEF2 = json_array_get(FUNC_DEF, 1);

               bc_info.fundef1 = strdup(json_string_value(FUNC_DEF1));
               bc_info.fundef2 = strdup(json_string_value(FUNC_DEF2));
            } else {
               bc_info.fundef1 = strdup("0");
               bc_info.fundef2 = strdup("0");
            }
         }
         else
         {
            const char *FUNC_DEF  = json_object_get_string_value(bc_item, "FUNC_DEF");

            if ( FUNC_DEF != NULL ) {
               bc_info.fundef1 = strdup(FUNC_DEF);
            } else {
               bc_info.fundef1 = strdup("0");
            }
         }

         // fill our parameter
         if ( strcmp(bc_info.bctype, "DIRICHLET") == 0 )
         {
            p->bc_params.BcType[0][bc_info.idx] = BC_1De_DIRICHLET;
            strcpy(p->bc_params.BcDirichlet[0][bc_info.idx].fundef, bc_info.fundef1);
         }
         if ( strcmp(bc_info.bctype, "NEUMANN") == 0 )
         {
            p->bc_params.BcType[0][bc_info.idx] = BC_1De_NEUMANN;
            strcpy(p->bc_params.BcDirichlet[0][bc_info.idx].fundef, bc_info.fundef1);
         }
         if ( strcmp(bc_info.bctype, "CAUCHY") == 0 )
         {
            p->bc_params.BcType[0][bc_info.idx] = BC_1De_CAUCHY;
            strcpy(p->bc_params.BcDirichlet[0][bc_info.idx].fundef, bc_info.fundef1);
         }
         if ( strcmp(bc_info.bctype, "ROBIN") == 0 )
         {
            p->bc_params.BcType[0][bc_info.idx] = BC_1De_ROBIN;
            strcpy(p->bc_params.BcRobin1[0][bc_info.idx].fundef , bc_info.fundef1) ;
            strcpy(p->bc_params.BcRobin2[0][bc_info.idx].fundef , bc_info.fundef2) ;
         }
      }

   }

   if ( target_dims == 2 )
   {
      json_t *DIM1 = json_object_get(PDE_BOUNDARY_CONDITIONS, "DIM1");
      json_t *DIM2 = json_object_get(PDE_BOUNDARY_CONDITIONS, "DIM2");

      if ( DIM1 == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_BOUNDARY_CONDITIONS - DIM1");
      }
      if ( DIM2 == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_BOUNDARY_CONDITIONS - DIM2");
      }

      // loop on the data
      size_t index;
      json_t *bc_item;

      int dim;

      for (dim=1; dim==2; dim++)
      {
         json_t *DIM;
         if (dim == 1) DIM = DIM1;
         if (dim == 2) DIM = DIM2;

         json_array_foreach(DIM, index, bc_item)
         {
            BcInfoType bc_info;

            /* block of code that uses index and value */
            bc_info.idx     = json_object_get_integer_value(bc_item, "IDX");
            bc_info.bctype  = strdup(json_object_get_string_value(bc_item, "BC_TYPE"));

            if ( strcmp(bc_info.bctype, "ROBIN") == 0 )
            {
               json_t *FUNC_DEF = json_object_get(bc_item, "FUNC_DEF");

               json_t *FUNC_DEF1 = json_array_get(FUNC_DEF, 0);
               json_t *FUNC_DEF2 = json_array_get(FUNC_DEF, 1);

               bc_info.fundef1 = strdup(json_string_value(FUNC_DEF1));
               bc_info.fundef2 = strdup(json_string_value(FUNC_DEF2));
            }
            else
            {
               bc_info.fundef1 = strdup(json_object_get_string_value(bc_item, "FUNC_DEF1"));
            }

            // fill our parameter
            if ( strcmp(bc_info.bctype, "DIRICHLET") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_DIRICHLET;
               strcpy(p->bc_params.BcDirichlet[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
            }
            if ( strcmp(bc_info.bctype, "NEUMANN") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_NEUMANN;
               strcpy(p->bc_params.BcDirichlet[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
            }
            if ( strcmp(bc_info.bctype, "CAUCHY") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_CAUCHY;
               strcpy(p->bc_params.BcDirichlet[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
            }
            if ( strcmp(bc_info.bctype, "ROBIN") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_ROBIN;
               strcpy(p->bc_params.BcRobin1[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
               strcpy(p->bc_params.BcRobin2[dim-1][bc_info.idx].fundef, bc_info.fundef2) ;
            }
         }
      }
   }

   if ( target_dims == 3 )
   {
      json_t *DIM1 = json_object_get(PDE_BOUNDARY_CONDITIONS, "DIM1");
      json_t *DIM2 = json_object_get(PDE_BOUNDARY_CONDITIONS, "DIM2");
      json_t *DIM3 = json_object_get(PDE_BOUNDARY_CONDITIONS, "DIM3");

      if ( DIM1 == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_BOUNDARY_CONDITIONS - DIM1");
      }
      if ( DIM2 == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_BOUNDARY_CONDITIONS - DIM2");
      }
      if ( DIM3 == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_BOUNDARY_CONDITIONS - DIM3");
      }

      // loop on the data
      size_t index;
      json_t *bc_item;

      int dim;

      for (dim=1; dim==3; dim++)
      {
         json_t *DIM;
         if (dim == 1) DIM = DIM1;
         if (dim == 2) DIM = DIM2;
         if (dim == 3) DIM = DIM3;

           json_array_foreach(DIM, index, bc_item)
         {
            BcInfoType bc_info;

            /* block of code that uses index and value */
            bc_info.idx     = json_object_get_integer_value(bc_item, "IDX");
            bc_info.bctype  = strdup(json_object_get_string_value(bc_item, "BC_TYPE"));

            if ( strcmp(bc_info.bctype, "ROBIN") == 0 )
            {
               json_t *FUNC_DEF = json_object_get(bc_item, "FUNC_DEF");

               json_t *FUNC_DEF1 = json_array_get(FUNC_DEF, 0);
               json_t *FUNC_DEF2 = json_array_get(FUNC_DEF, 1);

               bc_info.fundef1 = strdup(json_string_value(FUNC_DEF1));
               bc_info.fundef2 = strdup(json_string_value(FUNC_DEF2));
            }
            else
            {
               bc_info.fundef1 = strdup(json_object_get_string_value(bc_item, "FUNC_DEF2"));
            }

            // fill our parameter
            if ( strcmp(bc_info.bctype, "DIRICHLET") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_DIRICHLET;
               strcpy(p->bc_params.BcDirichlet[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
            }
            if ( strcmp(bc_info.bctype, "NEUMANN") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_NEUMANN;
               strcpy(p->bc_params.BCNeumann[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
            }
            if ( strcmp(bc_info.bctype, "CAUCHY") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_CAUCHY;
               strcpy(p->bc_params.BcCauchy[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
            }
            if ( strcmp(bc_info.bctype, "ROBIN") == 0 )
            {
               p->bc_params.BcType[dim-1][bc_info.idx] = BC_1De_ROBIN;
               strcpy(p->bc_params.BcRobin1[dim-1][bc_info.idx].fundef, bc_info.fundef1) ;
               strcpy(p->bc_params.BcRobin2[dim-1][bc_info.idx].fundef, bc_info.fundef2) ;
            }
         }
      }
   }
}

static void json_config_PARAMS_right_hand_side(PARAMS* p, const json_t* config)
{
   json_t *PDE_RHS = json_object_get(config, "PDE_RHS");

   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");
   int target_dims = json_object_get_integer_value(config, "TARGET_DIMS");

   char *names[] = {
      "DIM1",
      "DIM2",
      "DIM3",
   };

   int dim = 0;

   for (dim=1; dim<=target_dims; dim++)
   {
      char *name = names[dim-1];

      json_t *DIM = json_object_get(PDE_RHS, name);

      if ( DIM == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, strcat("PDE_RHS - ", name));
      }

      json_t *FUNC_DEF = json_object_get(DIM, "FUNC_DEF");

      if (FUNC_DEF != NULL)
      {
         const char *fundef = json_string_value(FUNC_DEF);

         if (fundef != NULL)
         {
            strcpy(p->rhs_params.rhs[dim-1].fundef, fundef);
         }
      }
   }
}

static void json_config_PARAMS_initial_condition(PARAMS* p, const json_t* config)
{
   json_t *PDE_INITIAL_CONDITION = json_object_get(config, "PDE_INITIAL_CONDITION");

   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");
   int target_dims = json_object_get_integer_value(config, "TARGET_DIMS");

   char *names[] = {
      "DIM1",
      "DIM2",
      "DIM3",
   };

   int dim = 0;

   for (dim=1; dim<=target_dims; dim++)
   {
      char *name = names[dim-1];

      json_t *DIM = json_object_get(PDE_INITIAL_CONDITION, name);

      if ( DIM == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, strcat("PDE_INITIAL_CONDITION - ", name));
      }

      json_t *FUNC_DEF = json_object_get(DIM, "FUNC_DEF");

      if (FUNC_DEF != NULL)
      {
         const char *fundef = json_string_value(FUNC_DEF);

         if (fundef != NULL)
         {
            strcpy(p->ini_params.ini[dim-1].fundef, fundef);
         }
      }
   }
}

static void json_config_PARAMS_solexacte(PARAMS* p, const json_t* config)
{
   json_t *PDE_EXACT_SOL = json_object_get(config, "PDE_EXACT_SOL");

   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");
   int target_dims = json_object_get_integer_value(config, "TARGET_DIMS");

   char *names[] = {
      "DIM1",
      "DIM2",
      "DIM3",
   };

   int dim = 0;

   for (dim=1; dim<=target_dims; dim++)
   {
      char *name = names[dim-1];

      json_t *DIM = json_object_get(PDE_EXACT_SOL, name);

      if ( DIM == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, strcat("PDE_EXACT_SOL - ", name));
      }

      json_t *FUNC_DEF = json_object_get(DIM, "FUNC_DEF");

      if (FUNC_DEF != NULL)
      {
         const char *fundef = json_string_value(FUNC_DEF);

         if (fundef != NULL)
         {
            strcpy(p->sol_params.sol[dim-1].fundef, fundef);
         }
      }

      json_t *DFUNC_DEF = json_object_get(DIM, "DFUNC_DEF");

      if (DFUNC_DEF != NULL)
      {
         const char *fundef = json_string_value(DFUNC_DEF);

         if (fundef != NULL)
         {
            strcpy(p->ini_params.dini[dim-1].fundef, fundef);
         }
      }
   }
}

static void json_config_PARAMS_convective_terms(PARAMS* p, const json_t* config)
{
   json_t *PDE_CONVECTION_DIFFUSION_FUNCTIONS = json_object_get(config, "PDE_CONVECTION_DIFFUSION_FUNCTIONS");

   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");
   int target_dims = json_object_get_integer_value(config, "TARGET_DIMS");

   const char *pde = json_object_get_string_value(config, "PDE");

   // 1-DIM
   if (target_dims == 1)
   {
      json_t *DIMS_1 = json_object_get(PDE_CONVECTION_DIFFUSION_FUNCTIONS, "1-DIM");

      if ( DIMS_1 == NULL && strcmp(pde, "CONVECTION-DIFFUSION") == 0 )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_CONVECTION_DIFFUSION_FUNCTIONS - 1-DIM");
      }

      json_t *FUNC_dudx = json_object_get(DIMS_1, "FUNC_dudx");
      json_t *FUNC_dudy = json_object_get(DIMS_1, "FUNC_dudy");
      json_t *FUNC_dudz = json_object_get(DIMS_1, "FUNC_dudz");

      if ( FUNC_dudx == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_CONVECTION_DIFFUSION_FUNCTIONS - 1-DIM - FUNC_dudx");
      }

      if ( FUNC_dudy == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_CONVECTION_DIFFUSION_FUNCTIONS - 1-DIM - FUNC_dudy");
      }

      if ( FUNC_dudz == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_CONVECTION_DIFFUSION_FUNCTIONS - 1-DIM - FUNC_dudz");
      }

      const char *fundef1 = json_string_value(FUNC_dudx);
      const char *fundef2 = json_string_value(FUNC_dudy);
      const char *funcdef3 = json_string_value(FUNC_dudz);

      if (fundef1) strcpy(p->adv_params.adv1[0][0].fundef , fundef1);
      if (fundef2) strcpy(p->adv_params.adv1[1][0].fundef , fundef2);
      if (funcdef3) strcpy(p->adv_params.adv1[2][0].fundef , funcdef3);
   }

   // 2-DIM
   if (target_dims == 2)
   {
      json_t *DIMS_2 = json_object_get(PDE_CONVECTION_DIFFUSION_FUNCTIONS, "2-DIM");

      if ( DIMS_2 == NULL && strcmp(pde, "CONVECTION-DIFFUSION") == 0 )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_CONVECTION_DIFFUSION_FUNCTIONS - 2-DIM");
      }

      json_t *DIM_1 = json_object_get(DIMS_2, "DIM1");

      json_t *FUNC_DIM1_dudx = json_object_get(DIM_1, "FUNC_dudx");
      json_t *FUNC_DIM1_dudy = json_object_get(DIM_1, "FUNC_dudy");
      json_t *FUNC_DIM1_dudz = json_object_get(DIM_1, "FUNC_dudz");
      json_t *FUNC_DIM1_dvdx = json_object_get(DIM_1, "FUNC_dvdx");
      json_t *FUNC_DIM1_dvdy = json_object_get(DIM_1, "FUNC_dvdy");
      json_t *FUNC_DIM1_dvdz = json_object_get(DIM_1, "FUNC_dvdz");

      json_t *DIM2 = json_object_get(DIMS_2, "DIM2");

      json_t *FUNC_DIM2_dudx = json_object_get(DIM2, "FUNC_dudx");
      json_t *FUNC_DIM2_dudy = json_object_get(DIM2, "FUNC_dudy");
      json_t *FUNC_DIM2_dudz = json_object_get(DIM2, "FUNC_dudz");
      json_t *FUNC_DIM2_dvdx = json_object_get(DIM2, "FUNC_dvdx");
      json_t *FUNC_DIM2_dvdy = json_object_get(DIM2, "FUNC_dvdy");
      json_t *FUNC_DIM2_dvdz = json_object_get(DIM2, "FUNC_dvdz");
   }

   // 3-DIM
   if (target_dims == 3)
   {
      json_t *DIMS_3 = json_object_get(PDE_CONVECTION_DIFFUSION_FUNCTIONS, "3-DIM");

      if ( DIMS_3 == NULL && strcmp(pde, "CONVECTION-DIFFUSION") == 0 )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_CONVECTION_DIFFUSION_FUNCTIONS - 3-DIM");
      }

      json_t *DIM1 = json_object_get(DIMS_3, "DIM1");

      json_t *FUNC_DIM1_dudx = json_object_get(DIM1, "FUNC_dudx");
      json_t *FUNC_DIM1_dudy = json_object_get(DIM1, "FUNC_dudy");
      json_t *FUNC_DIM1_dudz = json_object_get(DIM1, "FUNC_dudz");
      json_t *FUNC_DIM1_dvdx = json_object_get(DIM1, "FUNC_dvdx");
      json_t *FUNC_DIM1_dvdy = json_object_get(DIM1, "FUNC_dvdy");
      json_t *FUNC_DIM1_dvdz = json_object_get(DIM1, "FUNC_dvdz");
      json_t *FUNC_DIM1_dwdx = json_object_get(DIM1, "FUNC_dwdx");
      json_t *FUNC_DIM1_dwdy = json_object_get(DIM1, "FUNC_dwdy");
      json_t *FUNC_DIM1_dwdz = json_object_get(DIM1, "FUNC_dwdz");

      json_t *DIM2 = json_object_get(DIMS_3, "DIM2");

      json_t *FUNC_DIM2_dudx = json_object_get(DIM2, "FUNC_dudx");
      json_t *FUNC_DIM2_dudy = json_object_get(DIM2, "FUNC_dudy");
      json_t *FUNC_DIM2_dudz = json_object_get(DIM2, "FUNC_dudz");
      json_t *FUNC_DIM2_dvdx = json_object_get(DIM2, "FUNC_dvdx");
      json_t *FUNC_DIM2_dvdy = json_object_get(DIM2, "FUNC_dvdy");
      json_t *FUNC_DIM2_dvdz = json_object_get(DIM2, "FUNC_dvdz");
      json_t *FUNC_DIM2_dwdx = json_object_get(DIM2, "FUNC_dwdx");
      json_t *FUNC_DIM2_dwdy = json_object_get(DIM2, "FUNC_dwdy");
      json_t *FUNC_DIM2_dwdz = json_object_get(DIM2, "FUNC_dwdz");

      json_t *DIM3 = json_object_get(DIMS_3, "DIM3");

      json_t *FUNC_DIM3_dudx = json_object_get(DIM3, "FUNC_dudx");
      json_t *FUNC_DIM3_dudy = json_object_get(DIM3, "FUNC_dudy");
      json_t *FUNC_DIM3_dudz = json_object_get(DIM3, "FUNC_dudz");
      json_t *FUNC_DIM3_dvdx = json_object_get(DIM3, "FUNC_dvdx");
      json_t *FUNC_DIM3_dvdy = json_object_get(DIM3, "FUNC_dvdy");
      json_t *FUNC_DIM3_dvdz = json_object_get(DIM3, "FUNC_dvdz");
      json_t *FUNC_DIM3_dwdx = json_object_get(DIM3, "FUNC_dwdx");
      json_t *FUNC_DIM3_dwdy = json_object_get(DIM3, "FUNC_dwdy");
      json_t *FUNC_DIM3_dwdz = json_object_get(DIM3, "FUNC_dwdz");
   }
}




static BANDWRt_METHOD get_bandwidth_method(const json_t* BANDWIDTHREDUCTION)
{
   json_t *IMPLEMENTATION = json_object_get(BANDWIDTHREDUCTION, "IMPLEMENTATION");

   if ( IMPLEMENTATION == NULL || json_string_value(IMPLEMENTATION) == NULL )
   {
      return BANDWRe_NULL;
   }
   else
   {
      if ( strcmp(json_string_value(IMPLEMENTATION), "MESCHACH") == 0 )
      {
         return BANDWRe_MESCHACH;
      }
      else
      if ( strcmp(json_string_value(IMPLEMENTATION), "AMD") == 0 )
      {
         return BANDWRe_AMD;
      }
      else
      if ( strcmp(json_string_value(IMPLEMENTATION), "SPOOLES") == 0 )
      {
         return BANDWRe_SPOOLES;
      }
      else
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - ** - BANDWIDTHREDUCTION");
      }
   }

   return BANDWRe_NULL;
}

static BANDWRt_OPTION get_bandwidth_option(const json_t* BANDWIDTHREDUCTION)
{
   json_t *IMPLEMENTATION = json_object_get(BANDWIDTHREDUCTION, "IMPLEMENTATION");
   json_t *OPTION         = json_object_get(BANDWIDTHREDUCTION, "OPTION");

   if ( IMPLEMENTATION == NULL || json_string_value(IMPLEMENTATION) == NULL )
   {
      return BANDWRe_NO_OPTION;
   }
   else
   {
      if ( strcmp(json_string_value(IMPLEMENTATION), "MESCHACH") == 0 )
      {
         if ( strcmp(json_string_value(OPTION), "PROFILE") == 0 )
         {
            return BANDWRe_PROFILE;
         }
         else
         {
            return BANDWRe_BAND;
         }
      }
      else
      if ( strcmp(json_string_value(IMPLEMENTATION), "AMD") == 0 )
      {
         return BANDWRe_NO_OPTION;
      }
      else
      if ( strcmp(json_string_value(IMPLEMENTATION), "SPOOLES") == 0 )
      {
         return BANDWRe_NO_OPTION;
      }
      else
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - ** - BANDWIDTHREDUCTION");
      }
   }

   return BANDWRe_NO_OPTION;
}



static void json_config_PARAMS_laplacian(PARAMS* p, const json_t* config)
{
   json_t *PDE_RESOLUTION = json_object_get(config, "PDE_RESOLUTION");

   if ( PDE_RESOLUTION == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION");
   }

   json_t *LAPLACIAN_ALGO = json_object_get(PDE_RESOLUTION, "LAPLACIAN_ALGO");

   if (LAPLACIAN_ALGO == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - LAPLACIAN_ALGO");
   }

   json_t *LAPLACIAN_METHOD = json_object_get(LAPLACIAN_ALGO, "METHOD");

   strncpy(p->resol_params.resolution_method, json_string_value(LAPLACIAN_METHOD), 16);

   json_t *DIRECT_METHOD = json_object_get(LAPLACIAN_ALGO, "DIRECT-METHOD");
   json_t *ITERATIVE_METHOD = json_object_get(LAPLACIAN_ALGO, "ITERATIVE-METHOD");

   if ( strcmp(json_string_value(LAPLACIAN_METHOD), "DIRECT-METHOD") == 0 )
   {
      if ( DIRECT_METHOD == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - LAPLACIAN_ALGO - DIRECT-METHOD ");
      }
   }
   if ( strcmp(json_string_value(LAPLACIAN_METHOD), "ITERATIVE-METHOD") == 0 )
   {
      if ( ITERATIVE_METHOD == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - LAPLACIAN_ALGO - ITERATIVE-METHOD ");
      }
   }

   if ( strcmp(json_string_value(LAPLACIAN_METHOD), "DIRECT-METHOD") == 0 )
   {
      json_t *BANDWIDTHREDUCTION            = json_object_get(DIRECT_METHOD, "BANDWIDTHREDUCTION");

      if ( BANDWIDTHREDUCTION )
      {
         p->resol_params.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->resol_params.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);
      }
   }

   if ( strcmp(json_string_value(LAPLACIAN_METHOD), "ITERATIVE-METHOD") == 0 )
   {
      json_t *METHOD            = json_object_get(ITERATIVE_METHOD, "METHOD");
      json_t *PRECONDITIONER    = json_object_get(ITERATIVE_METHOD, "PRECONDITIONER");

      strncpy(p->resol_params.resolution_method, json_string_value(METHOD), 16);

      if (json_string_value(PRECONDITIONER))
      {
         strncpy(p->resol_params.preconditionning, json_string_value(PRECONDITIONER), 16);
      }
      else
      {
         strncpy(p->resol_params.preconditionning, "NULL", 16);
      }

      if ( strcmp(p->resol_params.preconditionning, "null") == 0 )
      {
         strncpy(p->resol_params.preconditionning, "NULL", 16);
      }

      json_t *PARAMETERS  = json_object_get(ITERATIVE_METHOD, "PARAMETERS");

      p->resol_params.max_steps = json_object_get_integer_value(PARAMETERS, "NMAX_ITER");
      p->resol_params.nb_steps  = 0;
      p->resol_params.eps_steps = json_object_get_real_value(PARAMETERS, "EPSILON");

      json_t *OUTPUT            = json_object_get(ITERATIVE_METHOD, "OUTPUT");

      if (OUTPUT)
      {
         json_t *OUTPUT_STDOUT     = json_object_get(OUTPUT, "STDOUT");
         json_t *OUTPUT_FILE       = json_object_get(OUTPUT, "FILE");

         p->resol_params.iter_info = 0;
         /*
          iter_info = 0 -> nix
          1 -> stdout
          2 -> file
          3 -> stdout | file
          */
         if (OUTPUT_STDOUT && json_boolean_value(OUTPUT_STDOUT))
         {
            p->resol_params.iter_info |= 1;
         }

         if (OUTPUT_FILE)
         {
            if (json_string_value(OUTPUT_FILE))
            {
               strncpy(p->resol_params.iter_file, json_string_value(OUTPUT_FILE), 128);
               p->resol_params.iter_info |= 2;
            }
            else
            {
               strncpy(p->resol_params.iter_file, "NULL", 128);
            }
         }
      }
   }
}

static void json_config_PARAMS_convection_diffusion(PARAMS *p, const json_t* config)
{
   json_t *PDE_RESOLUTION = json_object_get(config, "PDE_RESOLUTION");

   if ( PDE_RESOLUTION == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION");
   }

   json_t *CONVECTION_DIFFUSION_ALGO = json_object_get(PDE_RESOLUTION, "CONVECTION_DIFFUSION_ALGO");

   if (CONVECTION_DIFFUSION_ALGO == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - CONVECTION_DIFFUSION_ALGO");
   }

   json_t *CONVECTION_DIFFUSION_METHOD = json_object_get(CONVECTION_DIFFUSION_ALGO, "METHOD");

   strncpy(p->resol_params.resolution_method, json_string_value(CONVECTION_DIFFUSION_METHOD), 16);

   json_t *DIRECT_METHOD     = json_object_get(CONVECTION_DIFFUSION_ALGO, "DIRECT-METHOD");
   json_t *ITERATIVE_METHOD  = json_object_get(CONVECTION_DIFFUSION_ALGO, "ITERATIVE-METHOD");

   if ( strcmp(json_string_value(CONVECTION_DIFFUSION_METHOD), "DIRECT-METHOD") == 0 )
   {
      if ( DIRECT_METHOD == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - CONVECTION_DIFFUSION_ALGO - DIRECT-METHOD ");
      }
   }
   if ( strcmp(json_string_value(CONVECTION_DIFFUSION_METHOD), "ITERATIVE-METHOD") == 0 )
   {
      if ( ITERATIVE_METHOD == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION - CONVECTION_DIFFUSION_ALGO - ITERATIVE-METHOD ");
      }
   }

   if ( strcmp(json_string_value(CONVECTION_DIFFUSION_ALGO), "DIRECT-METHOD") == 0 )
   {
      json_t *BANDWIDTHREDUCTION = json_object_get(DIRECT_METHOD, "BANDWIDTHREDUCTION");

      if ( BANDWIDTHREDUCTION )
      {
         p->resol_params.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->resol_params.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);
      }
   }

   if ( strcmp(json_string_value(CONVECTION_DIFFUSION_ALGO), "ITERATIVE-METHOD") == 0 )
   {
      json_t *METHOD            = json_object_get(ITERATIVE_METHOD, "METHOD");
      json_t *PRECONDITIONER    = json_object_get(ITERATIVE_METHOD, "PRECONDITIONER");

      strncpy(p->resol_params.resolution_method, json_string_value(METHOD), 16);

      if (json_string_value(PRECONDITIONER))
      {
         strncpy(p->resol_params.preconditionning, json_string_value(PRECONDITIONER), 16);
      }
      else
      {
         strncpy(p->resol_params.preconditionning, "NULL", 16);
      }

      json_t *PARAMETERS  = json_object_get(ITERATIVE_METHOD, "PARAMETERS");

      p->resol_params.max_steps = json_object_get_integer_value(PARAMETERS, "NMAX_ITER");
      p->resol_params.nb_steps  = 0;
      p->resol_params.eps_steps = json_object_get_real_value(PARAMETERS, "EPSILON");

      json_t *OUTPUT            = json_object_get(ITERATIVE_METHOD, "OUTPUT");

      if (OUTPUT)
      {
         json_t *OUTPUT_STDOUT     = json_object_get(OUTPUT, "STDOUT");
         json_t *OUTPUT_FILE       = json_object_get(OUTPUT, "FILE");

         if (OUTPUT_STDOUT) p->resol_params.iter_info = json_integer_value(OUTPUT_STDOUT);
         if (OUTPUT_FILE)
         {
            if (json_string_value(OUTPUT_FILE))
            {
               strncpy(p->resol_params.iter_file, json_string_value(OUTPUT_FILE), 128);
            }
            else
            {
               strncpy(p->resol_params.iter_file, "NULL", 128);
            }
         }
      }
   }
}

static void json_config_PARAMS_burgers(PARAMS *p, const json_t* config)
{
   return;
}

static void json_config_PARAMS_eigen_solver(PARAMS* p, const json_t* config)
{
   const char *problem = json_object_get_string_value(config, "PDE");

   json_t *PDE_RESOLUTION = json_object_get(config, "PDE_RESOLUTION");

   if ( PDE_RESOLUTION == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION");
   }

   json_t *HELMHOLZ_ALGO  = json_object_get(PDE_RESOLUTION, "HELMHOLZ_ALGO");

   if ( HELMHOLZ_ALGO == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # HELMHOLZ_ALGO");
   }

   const char *eigen_problem = json_object_get_string_value(HELMHOLZ_ALGO, "EIGEN_PROBLEM");

   if (strcmp("ORDINARY_SYMMETRIC",      eigen_problem) == 0) p->eigen_params.eigen_problem = EIGENe_ORDINARY_SYMMETRIC;
   if (strcmp("GENERALIZED_SYMMETRIC_1", eigen_problem) == 0) p->eigen_params.eigen_problem = EIGENe_GENERALIZED_SYMMETRIC_1;
   if (strcmp("GENERALIZED_SYMMETRIC_2", eigen_problem) == 0) p->eigen_params.eigen_problem = EIGENe_GENERALIZED_SYMMETRIC_2;

   const char *eigen_option = json_object_get_string_value(HELMHOLZ_ALGO, "EIGEN_OPTION");

   if (strcmp("LOWER",     eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_LOWER;
   if (strcmp("UPPER",     eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_UPPER;
   if (strcmp("NEAREST_1", eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_NEAREST_1;
   if (strcmp("NEAREST_2", eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_NEAREST_2;
   if (strcmp("IBOUNDS",   eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_IBOUNDS;
   if (strcmp("RBOUNDS",   eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_RBOUNDS;
   if (strcmp("ALL",       eigen_option) == 0)    p->eigen_params.eigen_option = EIGENe_ALL;


   p->eigen_params.nb_eig             = json_object_get_integer_value(HELMHOLZ_ALGO, "NB_EIGENS");

   p->eigen_params.lower_ibound       = json_object_get_integer_value(HELMHOLZ_ALGO, "LOWER_IBOUND");
   p->eigen_params.upper_ibound       = json_object_get_integer_value(HELMHOLZ_ALGO, "UPPER_IBOUND");
   p->eigen_params.lower_rbound       = json_object_get_integer_value(HELMHOLZ_ALGO, "LOWER_RBOUND");
   p->eigen_params.upper_rbound       = json_object_get_integer_value(HELMHOLZ_ALGO, "UPPER_RBOUND");
}

static void json_config_PARAMS_stokes_solver(PARAMS* p, const json_t* config)
{
   json_t *PDE_RESOLUTION = json_object_get(config, "PDE_RESOLUTION");

   if ( PDE_RESOLUTION == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION");
   }

   json_t *STOKES_ALGO  = json_object_get(PDE_RESOLUTION, "STOKES_ALGO");

   if ( STOKES_ALGO == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO");
   }

   const char *method = json_object_get_string_value(STOKES_ALGO, "METHOD");

   strncpy(p->stokes_params.method, method, 64);

   if ( strcmp(method, "PRESSUREMATRIX") == 0 )
   {
      json_t *PRESSUREMATRIX_PARAMETERS = json_object_get(STOKES_ALGO, "PRESSUREMATRIX_PARAMETERS");

      if ( PRESSUREMATRIX_PARAMETERS == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # PRESSUREMATRIX_PARAMETERS");
      }

      p->stokes_params.pressurematrix.max_steps = json_object_get_real_value(PRESSUREMATRIX_PARAMETERS, "NMAX_ITER");
      p->stokes_params.pressurematrix.eps_steps = json_object_get_real_value(PRESSUREMATRIX_PARAMETERS, "EPSILON");

      json_t *INNERLOOP_SOLVER = json_object_get(PRESSUREMATRIX_PARAMETERS, "INNERLOOP_SOLVER");

      strncpy(p->stokes_params.pressurematrix.innerloop_method, json_object_get_string_value(INNERLOOP_SOLVER, "METHOD"), 64);

      if ( strcmp(p->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # PRESSUREMATRIX_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

      }
      if ( strcmp(p->stokes_params.pressurematrix.innerloop_method, "ITERATIVE-METHOD") == 0 )
      {

         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # PRESSUREMATRIX_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

         p->stokes_params.pressurematrix.innerloop_solver.max_steps = json_object_get_integer_value(INNERLOOP_SOLVER, "NMAX_ITER");
         p->stokes_params.pressurematrix.innerloop_solver.eps_steps = json_object_get_real_value(INNERLOOP_SOLVER, "EPSILON");
      }
   }

   if ( strcmp(method, "UZAWA") == 0 )
   {
      json_t *UZAWA_PARAMETERS = json_object_get(STOKES_ALGO, "UZAWA_PARAMETERS");

      if ( UZAWA_PARAMETERS == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # UZAWA_PARAMETERS");
      }

      json_t *RHO                     = json_object_get(UZAWA_PARAMETERS, "RHO");

      p->stokes_params.uzawa.rho = json_real_value(RHO);

      p->stokes_params.uzawa.max_steps = json_object_get_integer_value(UZAWA_PARAMETERS, "NMAX_ITER");
      p->stokes_params.uzawa.eps_steps = json_object_get_real_value(UZAWA_PARAMETERS, "EPSILON");

      json_t *INNERLOOP_SOLVER = json_object_get(UZAWA_PARAMETERS, "INNERLOOP_SOLVER");

      strncpy(p->stokes_params.uzawa.innerloop_method, json_object_get_string_value(INNERLOOP_SOLVER, "METHOD"), 64);

      if ( strcmp(p->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # UZAWA_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.uzawa.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.uzawa.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

      }
      if ( strcmp(p->stokes_params.uzawa.innerloop_method, "ITERATIVE-METHOD") == 0 )
      {

         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # UZAWA_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.uzawa.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.uzawa.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

         p->stokes_params.uzawa.innerloop_solver.max_steps = json_object_get_integer_value(INNERLOOP_SOLVER, "NMAX_ITER");
         p->stokes_params.uzawa.innerloop_solver.eps_steps = json_object_get_real_value(INNERLOOP_SOLVER, "EPSILON");
      }
   }

   if ( strcmp(method, "AUGMENTEDLAGR") == 0 )
   {
      json_t *AUGMENTEDLAGR_PARAMETERS = json_object_get(STOKES_ALGO, "AUGMENTEDLAGR_PARAMETERS");

      if ( AUGMENTEDLAGR_PARAMETERS == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # AUGMENTEDLAGR_PARAMETERS");
      }

      json_t *RHO                = json_object_get(AUGMENTEDLAGR_PARAMETERS, "RHO");

      p->stokes_params.augmentedlagrangian.rho = json_real_value(RHO);

      p->stokes_params.augmentedlagrangian.max_steps = json_object_get_integer_value(AUGMENTEDLAGR_PARAMETERS, "NMAX_ITER");
      p->stokes_params.augmentedlagrangian.eps_steps = json_object_get_real_value(AUGMENTEDLAGR_PARAMETERS, "EPSILON");

      json_t *INNERLOOP_SOLVER = json_object_get(AUGMENTEDLAGR_PARAMETERS, "INNERLOOP_SOLVER");

      strncpy(p->stokes_params.augmentedlagrangian.innerloop_method, json_object_get_string_value(INNERLOOP_SOLVER, "METHOD"), 64);

      if ( strcmp(p->stokes_params.augmentedlagrangian.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # AUGMENTEDLAGR_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

      }
      if ( strcmp(p->stokes_params.augmentedlagrangian.innerloop_method, "ITERATIVE-METHOD") == 0 )
      {

         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # AUGMENTEDLAGR_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

         p->stokes_params.augmentedlagrangian.innerloop_solver.max_steps = json_object_get_integer_value(INNERLOOP_SOLVER, "NMAX_ITER");
         p->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps = json_object_get_real_value(INNERLOOP_SOLVER, "EPSILON");
      }
   }
}

static void json_config_PARAMS_navierstokes_solver(PARAMS* p, const json_t* config)
{
   json_t *PDE_RESOLUTION = json_object_get(config, "PDE_RESOLUTION");

   if ( PDE_RESOLUTION == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION");
   }

   json_t *NAVIERSTOKES_ALGO = json_object_get(PDE_RESOLUTION, "NAVIERSTOKES_ALGO");

   if ( NAVIERSTOKES_ALGO == NULL )
   {
      error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # NAVIERSTOKES_ALGO");
   }

   const char *method = json_object_get_string_value(NAVIERSTOKES_ALGO, "METHOD");

   strncpy(p->navierstokes_params.method, method, 64);

   const char *outer_method = json_object_get_string_value(NAVIERSTOKES_ALGO, "OUTER-METHOD");

   strncpy(p->navierstokes_params.outer_method, outer_method, 64);

   if ( strcmp(outer_method, "UZAWA") == 0 )
   {
      json_t *UZAWA_PARAMETERS = json_object_get(PDE_RESOLUTION, "UZAWA_PARAMETERS");

      if ( UZAWA_PARAMETERS == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # NAVIERSTOKES_ALGO # UZAWA_PARAMETERS");
      }

      json_t *RHO                     = json_object_get(UZAWA_PARAMETERS, "RHO");

      p->stokes_params.uzawa.rho = json_real_value(RHO);

      p->stokes_params.uzawa.max_steps = json_object_get_integer_value(UZAWA_PARAMETERS, "NMAX_ITER");
      p->stokes_params.uzawa.eps_steps = json_object_get_real_value(UZAWA_PARAMETERS, "EPSILON");

      json_t *INNERLOOP_SOLVER = json_object_get(UZAWA_PARAMETERS, "INNERLOOP_SOLVER");

      strncpy(p->stokes_params.uzawa.innerloop_method, json_object_get_string_value(INNERLOOP_SOLVER, "METHOD"), 64);

      if ( strcmp(p->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # UZAWA_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.uzawa.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.uzawa.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

      }
      if ( strcmp(p->stokes_params.uzawa.innerloop_method, "ITERATIVE-METHOD") == 0 )
      {

         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # UZAWA_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.uzawa.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.uzawa.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

         p->stokes_params.uzawa.innerloop_solver.max_steps = json_object_get_integer_value(INNERLOOP_SOLVER, "NMAX_ITER");
         p->stokes_params.uzawa.innerloop_solver.eps_steps = json_object_get_real_value(INNERLOOP_SOLVER, "EPSILON");
      }
   }

   if ( strcmp(outer_method, "PRESSUREMATRIX") == 0 )
   {
      json_t *PRESSUREMATRIX_PARAMETERS = json_object_get(PDE_RESOLUTION, "PRESSUREMATRIX_PARAMETERS");

      if ( PRESSUREMATRIX_PARAMETERS == NULL )
      {
         error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # NAVIERSTOKES_ALGO # PRESSUREMATRIX_PARAMETERS");
      }

      p->stokes_params.pressurematrix.max_steps = json_object_get_real_value(PRESSUREMATRIX_PARAMETERS, "NMAX_ITER");
      p->stokes_params.pressurematrix.eps_steps = json_object_get_real_value(PRESSUREMATRIX_PARAMETERS, "EPSILON");

      json_t *INNERLOOP_SOLVER = json_object_get(PRESSUREMATRIX_PARAMETERS, "INNERLOOP_SOLVER");

      strncpy(p->stokes_params.pressurematrix.innerloop_method, json_object_get_string_value(INNERLOOP_SOLVER, "METHOD"), 64);

      if ( strcmp(p->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # PRESSUREMATRIX_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

      }
      if ( strcmp(p->stokes_params.pressurematrix.innerloop_method, "ITERATIVE-METHOD") == 0 )
      {

         json_t *BANDWIDTHREDUCTION      = json_object_get(INNERLOOP_SOLVER, "BANDWIDTHREDUCTION");

         if ( BANDWIDTHREDUCTION == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "PDE_RESOLUTION # STOKES_ALGO # PRESSUREMATRIX_PARAMETERS # BANDWIDTHREDUCTION");
         }

         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_method = get_bandwidth_method(BANDWIDTHREDUCTION);
         p->stokes_params.pressurematrix.innerloop_solver.bandwidth_option = get_bandwidth_option(BANDWIDTHREDUCTION);

         p->stokes_params.pressurematrix.innerloop_solver.max_steps = json_object_get_integer_value(INNERLOOP_SOLVER, "NMAX_ITER");
         p->stokes_params.pressurematrix.innerloop_solver.eps_steps = json_object_get_real_value(INNERLOOP_SOLVER, "EPSILON");
      }
   }
}

static void json_config_PARAMS_pde_solver(PARAMS* p, const json_t* config)
{
    json_t *PDE            = json_object_get(config, "PDE");

   if ( PDE == NULL )
    {
        return;
    }

    strncpy(p->problem, json_object_get_string_value(config, "PDE"), 128);

    if ( strcmp("LAPLACIAN", json_string_value(PDE)) == 0 )
   {
      json_config_PARAMS_laplacian(p, config);
   }

    if ( strcmp("CONVECTION-DIFFUSION", json_string_value(PDE)) == 0 )
   {
      json_config_PARAMS_convection_diffusion(p, config);
   }

   if ( strcmp("BURGERS", json_string_value(PDE)) == 0 )
   {
      json_config_PARAMS_burgers(p, config);
    }

    if ( strcmp("HELMHOLZ", json_string_value(PDE)) == 0 )
   {
      json_config_PARAMS_eigen_solver(p, config);
    }

    if ( strcmp("STOKES", json_string_value(PDE)) == 0 )
   {
      json_config_PARAMS_stokes_solver(p, config);
    }

   if ( strcmp("NAVIER-STOKES", json_string_value(PDE)) == 0 )
   {
      json_config_PARAMS_navierstokes_solver(p, config);
    }

    if ( strcmp("KURAMOTO-SIVASHINSKI", json_string_value(PDE)) == 0 )
   {
    }
}

static void json_config_PARAMS_graphics_output(PARAMS* p, const json_t* config)
{
   int coords_dims = json_object_get_integer_value(config, "COORDS_DIMS");

   if ( coords_dims == 1 )
   {
      json_t *GRAPHICS_1D_OUTPUT = json_object_get(config, "GRAPHICS_1D_OUTPUT");

      if ( GRAPHICS_1D_OUTPUT == NULL )
      {
         return;
      }

      json_t *GNUPLOT    = json_object_get(GRAPHICS_1D_OUTPUT, "GNUPLOT");
      json_t *MATLAB     = json_object_get(GRAPHICS_1D_OUTPUT, "MATLAB");
      json_t *SILO       = json_object_get(GRAPHICS_1D_OUTPUT, "SILO");
      json_t *VTK        = json_object_get(GRAPHICS_1D_OUTPUT, "VTK");

      if ( GNUPLOT )    p->graph_params.GNUPLOT    = json_boolean_value(GNUPLOT);
      if ( MATLAB )     p->graph_params.MATLAB     = json_boolean_value(MATLAB);
      if ( SILO )       p->graph_params.SILO       = json_boolean_value(SILO);
      if ( VTK )        p->graph_params.VTK        = json_boolean_value(VTK);
   }

   if ( coords_dims == 2 )
   {
      json_t *GRAPHICS_2D_OUTPUT = json_object_get(config, "GRAPHICS_2D_OUTPUT");

      if ( GRAPHICS_2D_OUTPUT == NULL )
      {
         return;
      }

      json_t *GNUPLOT    = json_object_get(GRAPHICS_2D_OUTPUT, "GNUPLOT");
      json_t *MATLAB     = json_object_get(GRAPHICS_2D_OUTPUT, "MATLAB");
      json_t *SILO       = json_object_get(GRAPHICS_2D_OUTPUT, "SILO");
      json_t *VTK        = json_object_get(GRAPHICS_2D_OUTPUT, "VTK");

      if ( GNUPLOT )    p->graph_params.GNUPLOT    = json_boolean_value(GNUPLOT);
      if ( MATLAB )     p->graph_params.MATLAB     = json_boolean_value(MATLAB);
      if ( SILO )       p->graph_params.SILO       = json_boolean_value(SILO);
      if ( VTK )        p->graph_params.VTK        = json_boolean_value(VTK);
   }

   if ( coords_dims == 3 )
   {
      json_t *GRAPHICS_3D_OUTPUT = json_object_get(config, "GRAPHICS_3D_OUTPUT");

      if ( GRAPHICS_3D_OUTPUT == NULL )
      {
         return;
      }

      json_t *SILO       = json_object_get(GRAPHICS_3D_OUTPUT, "SILO");
      json_t *VTK        = json_object_get(GRAPHICS_3D_OUTPUT, "VTK");

      if ( SILO )       p->graph_params.SILO       = json_boolean_value(SILO);
      if ( VTK )        p->graph_params.VTK        = json_boolean_value(VTK);
   }
}

static void json_config_PARAMS_graphics_interactiv1D_plots(PARAMS* p, const json_t* config)
{
   json_t *GRAPHICS_1D = json_object_get(config, "GRAPHICS_1D");

   if ( GRAPHICS_1D == NULL )
   {
      return;
   }

   strncpy(p->graph_interactiv1Dplots_params.ENGINE, json_object_get_string_value(GRAPHICS_1D, "ENGINE"), 16);
   strncpy(p->graph_interactiv1Dplots_params.DRIVER, json_object_get_string_value(GRAPHICS_1D, "DRIVER"), 16);

   p->graph_interactiv1Dplots_params.SIZE_WINDOW_X  = json_object_get_arrayitem_integer_value(GRAPHICS_1D, "WINDOW_SIZE", 0);
   p->graph_interactiv1Dplots_params.SIZE_WINDOW_Y  = json_object_get_arrayitem_integer_value(GRAPHICS_1D, "WINDOW_SIZE", 1);

   p->graph_interactiv1Dplots_params.SIZE_LABELS    = json_object_get_real_value(GRAPHICS_1D, "LABELS_SIZE");

   json_t *SUB_WINDOWS_ARRAY = json_object_get(GRAPHICS_1D, "SUB_WINDOWS");

   p->graph_interactiv1Dplots_params.NX             = 0; // init
   p->graph_interactiv1Dplots_params.NY             = 0; // init

   // loop on the sub windows
   size_t index;
   json_t *SUB_WINDOW;

   json_array_foreach(SUB_WINDOWS_ARRAY, index, SUB_WINDOW)
   {
      json_t *SUB_WINDOW_IDXS = json_object_get(SUB_WINDOW, "IDXS");
      json_t *USE_VIEWPORT = json_object_get(SUB_WINDOW, "USE_VIEWPORT");

      int i = json_integer_value(json_array_get(SUB_WINDOW_IDXS, 0));
      int j = json_integer_value(json_array_get(SUB_WINDOW_IDXS, 1));

      if ( i > p->graph_interactiv1Dplots_params.NX ) p->graph_interactiv1Dplots_params.NX = i;
      if ( j > p->graph_interactiv1Dplots_params.NY ) p->graph_interactiv1Dplots_params.NY = j;

      strncpy(p->graph_interactiv1Dplots_params.LABEL[i][j].LEGEND , json_object_get_string_value(SUB_WINDOW, "LEGEND"), 256);
      strncpy(p->graph_interactiv1Dplots_params.LABEL[i][j].AXE_X  , json_object_get_string_value(SUB_WINDOW, "LEGEND_AXE_X"), 64);
      strncpy(p->graph_interactiv1Dplots_params.LABEL[i][j].AXE_Y  , json_object_get_string_value(SUB_WINDOW, "LEGEND_AXE_Y"), 64);

      p->graph_interactiv1Dplots_params.MARKER[i][j]     = json_object_get_integer_value(SUB_WINDOW, "MARKER");
      p->graph_interactiv1Dplots_params.MARKER_END[i][j] = json_object_get_integer_value(SUB_WINDOW, "MARKER_END");
      
      
      if (USE_VIEWPORT != NULL)
      {
         p->graph_interactiv1Dplots_params.USE_VIEWPORT[i][j] = json_object_get_boolean_value(SUB_WINDOW, "USE_VIEWPORT");
      }
      else
      {
         p->graph_interactiv1Dplots_params.USE_VIEWPORT[i][j] = 1;
      }
      json_t *VIEWPORT = json_object_get(SUB_WINDOW, "VIEWPORT");

      p->graph_interactiv1Dplots_params.WINDOW[i][j].X_MIN = json_object_get_real_value(VIEWPORT, "X_MIN");
      p->graph_interactiv1Dplots_params.WINDOW[i][j].X_MAX = json_object_get_real_value(VIEWPORT, "X_MAX");
      p->graph_interactiv1Dplots_params.WINDOW[i][j].Y_MIN = json_object_get_real_value(VIEWPORT, "Y_MIN");
      p->graph_interactiv1Dplots_params.WINDOW[i][j].Y_MAX = json_object_get_real_value(VIEWPORT, "Y_MAX");
   }
}

static void json_config_PARAMS_graphics_interactiv2D_plots(PARAMS* p, const json_t* config)
{
   json_t *GRAPHICS_2D = json_object_get(config, "GRAPHICS_2D");

   if ( GRAPHICS_2D == NULL )
   {
      return;
   }

   strncpy(p->graph_interactiv2Dplots_params.ENGINE, json_object_get_string_value(GRAPHICS_2D, "ENGINE"), 16-1);
   strncpy(p->graph_interactiv2Dplots_params.DRIVER, json_object_get_string_value(GRAPHICS_2D, "DRIVER"), 16-1);

   p->graph_interactiv2Dplots_params.SIZE_WINDOW_X  = json_object_get_arrayitem_integer_value(GRAPHICS_2D, "WINDOW_SIZE", 0);
   p->graph_interactiv2Dplots_params.SIZE_WINDOW_Y  = json_object_get_arrayitem_integer_value(GRAPHICS_2D, "WINDOW_SIZE", 1);

   p->graph_interactiv2Dplots_params.SIZE_LABELS    = json_object_get_real_value(GRAPHICS_2D, "LABELS_SIZE");

   json_t *SUB_WINDOWS_ARRAY = json_object_get(GRAPHICS_2D, "SUB_WINDOWS");

    if ( SUB_WINDOWS_ARRAY == NULL )
    {
        return;
     }

   p->graph_interactiv2Dplots_params.NX             = 0;
   p->graph_interactiv2Dplots_params.NY             = 0;

   // loop on the sub windows
   size_t index;
   json_t *SUB_WINDOW;

   json_array_foreach(SUB_WINDOWS_ARRAY, index, SUB_WINDOW)
   {
      json_t *SUB_WINDOW_IDXS = json_object_get(SUB_WINDOW, "IDXS");

      int i = json_integer_value(json_array_get(SUB_WINDOW_IDXS, 0));
      int j = json_integer_value(json_array_get(SUB_WINDOW_IDXS, 1));

      if ( i > p->graph_interactiv2Dplots_params.NX ) p->graph_interactiv2Dplots_params.NX = i;
      if ( j > p->graph_interactiv2Dplots_params.NY ) p->graph_interactiv2Dplots_params.NY = j;

      strncpy(p->graph_interactiv2Dplots_params.LABEL[i][j].LEGEND , json_object_get_string_value(SUB_WINDOW, "LEGEND"), 256);
      /*
      strncpy(p->graph_interactiv2Dplots_params.LABEL[i][j].AXE_X  , json_object_get_string_value(SUB_WINDOW, "LEGEND_AXE_X"), 64);
      strncpy(p->graph_interactiv2Dplots_params.LABEL[i][j].AXE_Y  , json_object_get_string_value(SUB_WINDOW, "LEGEND_AXE_Y"), 64);

      p->graph_interactiv2Dplots_params.MARKER[i][j]     = json_object_get_integer_value(SUB_WINDOW, "MARKER");
      p->graph_interactiv2Dplots_params.MARKER_END[i][j] = json_object_get_integer_value(SUB_WINDOW, "MARKER_END");
      */

      json_t *VIEWPORT = json_object_get(SUB_WINDOW, "VIEWPORT");
      p->graph_interactiv2Dplots_params.WINDOW[i][j].X_MIN = json_object_get_real_value(VIEWPORT, "X_MIN");
      p->graph_interactiv2Dplots_params.WINDOW[i][j].X_MAX = json_object_get_real_value(VIEWPORT, "X_MAX");
      p->graph_interactiv2Dplots_params.WINDOW[i][j].Y_MIN = json_object_get_real_value(VIEWPORT, "Y_MIN");
      p->graph_interactiv2Dplots_params.WINDOW[i][j].Y_MAX = json_object_get_real_value(VIEWPORT, "Y_MAX");

        const char *GRAPHICS_TYPE = json_object_get_string_value(SUB_WINDOW, "GRAPHICS_TYPE");

        strncpy(p->graph_interactiv2Dplots_params.GRAPHICS_TYPE[i][j], GRAPHICS_TYPE, 16);

        if ( strcmp(GRAPHICS_TYPE, "CONTOURS") == 0)
        {
           json_t *CONTOURS = json_object_get(SUB_WINDOW, "CONTOURS");

           if ( CONTOURS == NULL )
           {
              error8(E_JSON_MISSING_OBJECT, "GRAPHICS2D - SUB_WINDOWS - SUB_WINDOW - CONTOURS");
              return;
           }

           json_t *LEVELS = json_object_get(CONTOURS, "LEVELS");
           if ( LEVELS != NULL )
         {
           // loop on the data
           size_t index;
           json_t *level_item;

           p->graph_interactiv2Dplots_params.LEVELS[i][j] = v_get(json_array_size(LEVELS));

           json_array_foreach(LEVELS, index, level_item)
           {
             p->graph_interactiv2Dplots_params.LEVELS[i][j]->ve[index] = json_real_value(json_array_get(LEVELS, index));
           }
         }
      else
      {
         json_t *NB_LEVELS = json_object_get(SUB_WINDOW, "NB_LEVELS");
         if ( NB_LEVELS == NULL )
         {
            error8(E_JSON_MISSING_OBJECT, "GRAPHICS2D - SUB_WINDOWS - SUB_WINDOW - CONTOURS - NB_LEVELS ");
         }
         else
         {
            p->graph_interactiv2Dplots_params.NB_LEVELS[i][j] =  json_object_get_integer_value(SUB_WINDOW, "NB_LEVELS");
         }
       }
      }
      else
      if ( strcmp(GRAPHICS_TYPE, "SURFACE") == 0)
      {
          json_t *SURFACE = json_object_get(SUB_WINDOW, "SURFACE");

          if ( SURFACE == NULL )
          {
              error8(E_JSON_MISSING_OBJECT, "GRAPHICS2D - SUB_WINDOWS - SUB_WINDOW - SURFACE");
              return;
          }

          // TODO


          // TODO
      }
      else
      {
          error8(E_JSON_MISSING_OBJECT, "GRAPHICS2D - SUB_WINDOWS - SUB_WINDOW - GRAPHICS_TYPE");
          return;
      }
   }
}

static void json_config_PARAMS_miscellaneous_params(PARAMS* p, const json_t* config)
{
   json_t *MISC_OUTPUT = json_object_get(config, "MISC_OUTPUT");

   if ( MISC_OUTPUT == NULL )
   {
      return;
   }

   if ( json_object_get(MISC_OUTPUT, "ITPRINT") ) p->misc_params.itprint = json_object_get_integer_value(MISC_OUTPUT, "ITPRINT");
   if ( json_object_get(MISC_OUTPUT, "ITSAVE")  ) p->misc_params.itsave  = json_object_get_integer_value(MISC_OUTPUT, "ITSAVE");
   if ( json_object_get(MISC_OUTPUT, "ITSTAT")  ) p->misc_params.itstat  = json_object_get_integer_value(MISC_OUTPUT, "ITSTAT");
   if ( json_object_get(MISC_OUTPUT, "ITGRAPH") ) p->misc_params.itgraph = json_object_get_integer_value(MISC_OUTPUT, "ITGRAPH");
}

static void json_config_PARAMS_miscellaneous_graphics(PARAMS* p, const json_t* config)
{
   json_t *MISC_GRAPHICS = json_object_get(config, "MISC_GRAPHICS");

   if ( MISC_GRAPHICS == NULL )
   {
      return;
   }

   if ( json_object_get(MISC_GRAPHICS, "IMTXPROF") )         p->misc_graphics.ispmatpattern    = json_object_get_boolean_value(MISC_GRAPHICS, "IMTXPROF");
   if ( json_object_get(MISC_GRAPHICS, "IGEOMVIEW") )        p->misc_graphics.igeomview        = json_object_get_boolean_value(MISC_GRAPHICS, "IGEOMVIEW");
   if ( json_object_get(MISC_GRAPHICS, "IGEOMCONTOURVIEW") ) p->misc_graphics.igeomcontourview = json_object_get_boolean_value(MISC_GRAPHICS, "IGEOMCONTOURVIEW");
   if ( json_object_get(MISC_GRAPHICS, "IGEOMTOTALVIEW") )   p->misc_graphics.igeomtotalview   = json_object_get_boolean_value(MISC_GRAPHICS, "IGEOMTOTALVIEW");
}

static void json_config_PARAMS_io_files(PARAMS* p, const json_t* config)
{
   if ( json_object_get(config, "OUTPUT_LOG_FILE") == NULL )
   {
      return;
   }

   const char *logger_file_name = json_object_get_string_value(config, "OUTPUT_LOG_FILE");

   if ( logger_file_name == NULL )
   {
      return;
   }

   // then "overwrite"
   if (p->logger != NULL)
   {
      fclose(p->logger);
   }

   p->logger = fopen(logger_file_name, "w");

   printf("Using log file %s ...\n", logger_file_name);
}

/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */

PARAMS *json_config_to_params(const json_t* config)
{

   PARAMS* p = Params_get();

   strncpy(p->problem, json_object_get_string_value(config, "PDE"), 128);

   json_config_PARAMS_finite_element(p, config);
   json_config_PARAMS_physical_parameters(p, config);
   json_config_PARAMS_time_parameters(p, config);
   json_config_PARAMS_time_scheme(p, config);
   json_config_PARAMS_geometry(p, config);
   json_config_PARAMS_boundary_conditions(p, config);
   json_config_PARAMS_right_hand_side(p, config);
   json_config_PARAMS_convective_terms(p, config);
   json_config_PARAMS_initial_condition(p, config);
   json_config_PARAMS_solexacte(p, config);
   json_config_PARAMS_pde_solver(p, config);
   json_config_PARAMS_graphics_output(p, config);
   json_config_PARAMS_graphics_interactiv1D_plots(p, config);
   json_config_PARAMS_graphics_interactiv2D_plots(p, config);
   json_config_PARAMS_miscellaneous_params(p, config);
   json_config_PARAMS_miscellaneous_graphics(p, config);
   json_config_PARAMS_io_files(p, config);

   return p;
}

PARAMS *json_config_append_to_params(PARAMS* p, const json_t* config)
{
   json_config_PARAMS_finite_element(p, config);
   json_config_PARAMS_physical_parameters(p, config);
   json_config_PARAMS_time_parameters(p, config);
   json_config_PARAMS_time_scheme(p, config);
   json_config_PARAMS_geometry(p, config);
   //json_config_PARAMS_boundary_conditions(p, config);
   //json_config_PARAMS_right_hand_side(p, config);
   //json_config_PARAMS_convective_terms(p, config);
   //json_config_PARAMS_initial_condition(p, config);
   //json_config_PARAMS_solexacte(p, config);
   json_config_PARAMS_pde_solver(p, config);
   json_config_PARAMS_graphics_output(p, config);
   json_config_PARAMS_graphics_interactiv1D_plots(p, config);
   json_config_PARAMS_graphics_interactiv2D_plots(p, config);
   json_config_PARAMS_miscellaneous_params(p, config);
   json_config_PARAMS_miscellaneous_graphics(p, config);
   json_config_PARAMS_io_files(p, config);

   return p;

}
