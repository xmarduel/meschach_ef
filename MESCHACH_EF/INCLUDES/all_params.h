#ifndef MEF_PARAMETERS_H
#define MEF_PARAMETERS_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file all_params.h 
 *
 * Contains the structure which contains various parameters of the problem
 *
 * Note : If you update this file, modify also :
 *        2- all_params_datas.h
 *        3- all_params_set_param_val.c
 *        3- all_params_get_param_val.c
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"
 
#include "MESCHACH_EF/INCLUDES/functions_structs.h"


#define NBPARAMS  5

#define NB_BOUNDARYCONDITIONS  10

#define GRAPHIC_NB_SUB_WINDOWS  3
#define GRAPHIC_NB_BOXES        10


/* an enum utilized everywhere */
typedef enum
{
   AXEe_X = 0 ,
   AXEe_Y = 1 ,
   AXEe_Z = 2 ,
   AXEe_NUMBER = 3
   
} AXES_IDX;


/*******************************************/
typedef struct {
		
	char   name_ef[16];   /**< string representing the name of the finite element */
		
} PARAMS_finite_elements ;
	
/*******************************************/
typedef struct {
   
   Real epsilon;  /**< the viscosity */
   Real sigma;    /**< the dump factor */
   Real kappa;    /**< the viscosity */
   
} PARAMS_physical_parameters ;

/*******************************************/
typedef struct {
   
   Real DT;        /**< the time increment */
   Real TPS_INI;   /**< the starting time (usually 0.0) */
   Real TPS_FIN;   /**< the ending time */
   
} PARAMS_time_parameters ;

/*******************************************/
typedef struct {

   int QSTEP;
   int ALPHA_STEP;
   int BETA_STEPS;
   int GAMMA_STEP;

} PARAMS_time_scheme ;

/*******************************************/
typedef struct {
   
   char meshfile[256]; /**< the name of the file                                   */
   char meshname[64];  /**< the name of the mesh inside the meshfile if more than 1*/
   char meshtype[64];  /**< 3D: "gmsh", "cube" meshes                              */
                       /**< 2D: "quad1", "quad2", "quad3", "emc2" or "gmsh" meshes */
                       /**< 1D: "line"                                             */

   Real xmin;          /**< only for 1D mesh : the left bound of the mesh          */
   Real xmax;          /**< only for 1D mesh : the right bound of the mesh         */
   
   int  nx;            /**< only for 1D mesh : the number of intervals of the mesh */

   char distribution[64];
   int  periodicity;
   
} PARAMS_geometry ;

/*******************************************/
/*******************************************/
   
typedef struct _FF_SPEC
{
   char fundef[128];
      
} FF_SPEC;
   
/*******************************************/
typedef struct {
   
   int nb_BC;
   
   int BcType[3][NB_BOUNDARYCONDITIONS];

   FF_SPEC BcDirichlet[3][NB_BOUNDARYCONDITIONS];
   FF_SPEC BCNeumann[3][NB_BOUNDARYCONDITIONS];
   FF_SPEC BcCauchy[3][NB_BOUNDARYCONDITIONS];

   FF_SPEC BcRobin1[3][NB_BOUNDARYCONDITIONS];
   FF_SPEC BcRobin2[3][NB_BOUNDARYCONDITIONS];

} PARAMS_boundary_conditions ;

/*******************************************/
typedef struct {
   
   FF_SPEC  rhs[3];
   
} PARAMS_right_hand_side ;

/*******************************************/
typedef struct {

   FF_SPEC  ini[3];  /**< the function lua code (3 dim) */
   FF_SPEC  dini[3];  /**< the derivate function lua code (3 dim) needed if hermite ef */

} PARAMS_initial_condition ;

/*******************************************/
typedef struct {
      
   FF_SPEC sol[3];  /**< the  function lua code (3 dim) */
      
} PARAMS_exact_solution ;
   
/*******************************************/
typedef struct {
   
   char method[8];  /**< "SUPG" , "GALS", "DWG" */

   Real delta;      /**< a coefficient parameter */
   Real rho_stab;   /**< "SUPG" -> -1 , "GALS" -> 0 , "DWG" -> +1 */
   
   FF_SPEC adv1[3][3];  /**< the functions for advection problem (first  equation) */
   FF_SPEC adv2[3][3];  /**< the functions for advection problem (second equation) */
   FF_SPEC adv3[3][3];  /**< the functions for advection problem (third  equation) */
   
} PARAMS_convective_terms ;
	
/*******************************************/
typedef struct {

   int eigen_problem;  /**< see eigen.h for options (takes values in EIGENt_PROBLEM enum ) */
   int eigen_option;   /**< see eigen.h for options (takes values in EIGENt_OPTION enum)  */
   
   int nb_eig;

   int lower_ibound;
   int upper_ibound;

   double lower_rbound;
   double upper_rbound;

} PARAMS_eigen_solver ;

/*******************************************/
typedef struct {
		
	char resolution_method[16];
	char preconditionning[16];
		
	BANDWRt_METHOD  bandwidth_method; /**< BANDWRe_MESCHACH or BANDWRe_SPOOLES or BANDWRe_AMD */
	BANDWRt_OPTION  bandwidth_option; /**< BANDWRe_MESCHACH -> BANDWRe_BAND or BANDWRe_PROFILE */
	/**< BANDWRe_SPOOLES  -> no opt */
	/**< BANDWRe_AMD      -> no opt */
		
	int  max_steps;
	int  nb_steps;
	Real eps_steps;
		
	char iter_file[128];
	int  iter_info;
		
} PARAMS_matrix_solver ;
 
/*******************************************/
typedef struct {
		
	// INV(A) always resolved iteratively
	
	Real rho;
	
	int  max_steps;
	Real eps_steps;
	
	char innerloop_method[64];    /**< "DIRECT-METHOD" "ITERATIV-METHOD" */
	
   PARAMS_matrix_solver  innerloop_solver;
		
} PARAMS_uzawa_params ;
	
typedef struct {
	
	int  max_steps;
	Real eps_steps;

	char   innerloop_method[64];    /**< "DIRECT-METHOD" "ITERATIV-METHOD" */
	
	PARAMS_matrix_solver        innerloop_solver;
		
} PARAMS_pressurematrix_params ;
	
typedef struct {
		
	Real   rho;
		
	int  max_steps;
	Real eps_steps;
	
	char innerloop_method[64];    /**< "DIRECT-METHOD" "ITERATIV-METHOD" */

	PARAMS_matrix_solver innerloop_solver;
		
} PARAMS_augmentedlagrangian_params ;
	
typedef struct {

   char method[64];    /**< UZAWA, PRESSUREMATRIX, AUGMENTEDLAGR */
	
	/* PRESSUREMATRIX */
	PARAMS_pressurematrix_params pressurematrix;
	
   /* USAWA */
	PARAMS_uzawa_params uzawa;
	
	/* AUGMENTEDLAGR */
	PARAMS_augmentedlagrangian_params augmentedlagrangian;
	

} PARAMS_stokes_solver ;

/*******************************************/
typedef struct {

   char method[64];    /**< NEWTON-EXACT, NEWTON-APPROCH */

   Real eps_steps;
   int  max_steps;
	
   char outer_method[64];
	
   /* PRESSUREMATRIX */
   PARAMS_pressurematrix_params pressurematrix;
	
   /* UZAWA */
   PARAMS_uzawa_params uzawa;
	
} PARAMS_navierstokes_solver ;

/*******************************************/
typedef struct {
		
   int ispmatpattern; 
   int igeomview;
		
} PARAMS_miscellaneous_graphics ;
	
/*******************************************/
typedef struct {

   int GNUPLOT;
   int MATLAB;
   int SILO;
   int VTK;
   
} PARAMS_graphics_output ;

/*******************************************/

typedef struct _LABEL
{
   char AXE_X[64];
   char AXE_Y[64];
   char LEGEND[256];
   
} LABEL_DATA;

typedef struct _WINDOW
{
   double X_MIN;
   double X_MAX;
   double Y_MIN;
   double Y_MAX;
      
} WINDOW_DATA;


typedef struct {

   /* for the whole window */
   char   ENGINE[16];
   char   DRIVER[16];

   int    SIZE_WINDOW_X; /* pixels */
   int    SIZE_WINDOW_Y; /* pixels */
   
   double SIZE_LABELS;

   int    NX;
   int    NY;

   /* for each viewport */
   LABEL_DATA     LABEL [GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1]; /* for 9 subwindows on a page ... */
   WINDOW_DATA    WINDOW[GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];
 
   /* misc */
   int   MARKER       [GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];
   int   MARKER_END   [GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];
   
   /* misc */
   int   USE_VIEWPORT [GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];

   
} PARAMS_graphics_interactiv1D_plots ;
	
	
typedef struct {
		
	/* for the whole window */
   char   ENGINE[16];
	char   DRIVER[16];
		
	int    SIZE_WINDOW_X; /* pixels */
	int    SIZE_WINDOW_Y; /* pixels */
		
	double SIZE_LABELS;
		
	int    NX;
	int    NY;
		
	/* for each viewport */
	LABEL_DATA    LABEL [GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1]; /* for 9 subwindows on a page ... */
	WINDOW_DATA   WINDOW[GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];

    char    GRAPHICS_TYPE[GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1][16];

	/* for contours */
	int      NB_LEVELS[GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];
	VEC      *LEVELS[GRAPHIC_NB_SUB_WINDOWS+1][GRAPHIC_NB_SUB_WINDOWS+1];

    /* for surface */


} PARAMS_graphics_interactiv2D_plots ;

/*******************************************/
typedef struct {

   int itprint;
   int itsave;
   int itstat;
   int itgraph;
   
} PARAMS_miscellaneous_params ;

/*******************************************/
/*******************************************/

typedef struct {
   
   char problem[128];  /* main problem */
   
   FILE* logger;
   
   PARAMS_finite_elements        ef_params;
   PARAMS_physical_parameters    phys_params;
   PARAMS_time_parameters        time_params;
   PARAMS_time_scheme            timescheme_params;
   PARAMS_geometry               geom_params;
   PARAMS_boundary_conditions    bc_params;
   PARAMS_right_hand_side        rhs_params;
   PARAMS_convective_terms       adv_params;
   PARAMS_initial_condition      ini_params;
   PARAMS_exact_solution         sol_params;
   PARAMS_matrix_solver          resol_params;
   PARAMS_eigen_solver           eigen_params;
   PARAMS_stokes_solver          stokes_params;
   PARAMS_navierstokes_solver    navierstokes_params;
   PARAMS_graphics_output        graph_params;
   PARAMS_graphics_interactiv1D_plots  graph_interactiv1Dplots_params;
   PARAMS_graphics_interactiv2D_plots  graph_interactiv2Dplots_params;
   PARAMS_miscellaneous_params   misc_params;
   PARAMS_miscellaneous_graphics misc_graphics;
   
} PARAMS ;


#define PARAMS_NULL ((PARAMS*)NULL)

PARAMS*    Params_get(void);
int        Params_free(PARAMS* p);

#define PARAMS_FREE(p)  ( Params_free(p), (p)=(PARAMS *)NULL)


PARAMS* Params_get_staticparam(int i);
PARAMS* Params_set_staticparam(PARAMS* p, int i);

PARAMS* Params_setup_from_file(const char* input_file);
PARAMS* Params_append_from_string(PARAMS* p, const char* config);
   
void  Params_set_oneparam (PARAMS* p, const char* category, const char* field,                     void*  val);
void  Params_set_oneparam1(PARAMS* p, const char* category, const char* field, int idx1,           void*  val);
void  Params_set_oneparam2(PARAMS* p, const char* category, const char* field, int idx1, int idx2, void*  val);

void* Params_get_oneparam (const PARAMS* p, const char* category, const char* field);
void* Params_get_oneparam1(const PARAMS* p, const char* category, const char* field, int idx1);
void* Params_get_oneparam2(const PARAMS* p, const char* category, const char* field, int idx1, int idx2);

void  Params_get_absolute_path(const char* input_file, char* absolute_input_file);
void  Params_resolve_envvars(const char* string, char* resolved_string);

#ifdef __cplusplus
}
#endif


#endif

/*! \struct PARAMS
* \brief the structure for storing all kind of parameters, for all kind of problems
*
* Contains actually \n
*   PARAMS_physical_parameters    phys_params; \n
*   PARAMS_finite_elements        ef_params; \n
*   PARAMS_time_parameters        time_params; \n
*   PARAMS_time_scheme            timescheme_params; \n
*   PARAMS_geometry               geom_params; \n
*   PARAMS_boundary_conditions    bc_params; \n
*   PARAMS_right_hand_side        rhs_params; \n
*   PARAMS_convective_terms       adv_params; \n
*   PARAMS_initial_condition      ic_params; \n
*   PARAMS_eigen_solver           eigen_params; \n
*   PARAMS_stokes_solver          stokes_params; \n
*   PARAMS_navierstokes_solver    navierstokes_params; \n
*   PARAMS_matrix_solver          resol_params; \n
*   PARAMS_graphics_output        graph_params; \n
*   PARAMS_graphics_curveplots    graph_curveplots_params; \n
*   PARAMS_graphics_contourplots  graph_contourplots_params; \n
*   PARAMS_miscellaneous_params   misc_params; \n
*   PARAMS_miscellaneous_graphics misc_graphics; \n
*/

/*! \fn Params_get(void)
*  \brief initialize a PARAMS structure
*
* initialize all parameters to default values
*/

/*! \fn Params_free(PARAMS *p)
*  \brief release an PARAMS structure
*  \param p : the structure to be released \n
*/


/*! \fn Params_get_oneparam(const PARAMS* p, const char* category, const char* field);
*  \brief get a single parameter in the PARAMS structure ( used for the Python wrapper )
*  \param p: the structure to be filled \n
*  \param category: the category \n
*  \param field: the field \n
*
* category can be : \n
*  "physical_parameters"    \n
*  "finite_elements"        \n
*  "time_parameters"        \n
*  "time_scheme"            \n
*  "geometry"               \n
*  "boundary_conditions"    \n
*  "right_hand_side"        \n
*  "convective_terms"       \n
*  "initial_condition"      \n
*  "eigen_solver"           \n
*  "stokes_solver"          \n
*  "navierstokes_solver"    \n
*  "matrix_solver"          \n
*  "graphics_output"        \n
*  "graphics_plots"         \n
*  "miscellaneous_params"   \n
*  "miscellaneous_graphics" \n
*
* the field depends on the category and is a string corresponding to the name of the member of the corresponding structure \n
*
* the returned value is meant to be a "python" object, containing the true data
*/

/*! \fn Params_set_oneparam(PARAMS* p, const char* category, const char* field, void*  val);
*  \brief set a single parameter in the PARAMS structure ( used for the Python wrapper )
*  \param p : the structure to be filled \n
*  \param category: the category \n
*  \param field: the field \n
*  \param val: pointer to the value \n
*
* the value is meant to be a "python" object, containing the true data
*
*/

/*! \struct PARAMS_physical_parameters
* \brief the structure for storing physical parameters
*
* In the python script, write \n
* Params_set_oneparam(p, "physical_parameters", "epsilon", 1.0); \n
* Params_set_oneparam(p, "physical_parameters", "sigma"  , 1.0); \n
* Params_set_oneparam(p, "physical_parameters", "kappa"  , 1.0); \n
*
* In the script "1.0" is a python object.
*/

/*! \struct PARAMS_finite_elements
* \brief the structure for storing the type of the finite element
*
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "P1" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "P2" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "P3" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "H3" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "S2" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "S3" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "S4" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "S5" ) \n
* Params_set_oneparam(MyParams, "finite_elements_params", "name_ef", "P1b" ) \n
*
* In the script "P1" is a python object.
*/

/*! \struct PARAMS_time_parameters
* \brief the structure for storing the time parameters (for transient problems)
*
* Params_set_oneparam(MyParams, "time_params", "TPS_INI",    0.0  )  \n
* Params_set_oneparam(MyParams, "time_params", "TPS_FIN",    10.0  )  \n
* Params_set_oneparam(MyParams, "time_params", "DT",         0.005 )  \n
*
* In the script, the values are python objects.
*/

/*! \struct PARAMS_time_scheme
* \brief the structure for storing the time scheme (for transient problems)
*/

/*! \struct PARAMS_geometry
* \brief the structure for storing geometry parameters
*/

/*! \struct PARAMS_boundary_conditions
* \brief the structure for storing the boundary conditions functions indices
*
* In a python script, this will not neccessary utilized.\n
* Instead, python functions are directly set into a BC structure.
*/

/*! \struct PARAMS_right_hand_side
* \brief the structure for storing the right hand sides functions indices
*
* In a python script, this will not neccessary utilized.\n
* Instead, python functions are directly set into a RHS structure.
*/

/*! \struct PARAMS_convective_terms
* \brief the structure for storing the advections functions indices
*
* In a python script, this will not neccessary utilized.\n
* Instead, python functions are directly set into a ADV structure.
*/

/*! \struct PARAMS_initial_condition
* \brief the structure for storing the initial conditions functions indices
*
* In a python script, this will not neccessary utilized.\n
* Instead, python functions are directly set into a CI (RHS) structure.
*/

/*! \struct PARAMS_eigen_solver
* \brief the structure for storing the eigenvalues problem options
*
* Params_set_oneparam(MyParams, "eigen_params", "problem"     , EIGENe_GENERALIZED_SYMMETRIC_1) \n
* Params_set_oneparam(MyParams, "eigen_params", "option_eigen", EIGENe_RBOUNDS) \n
* Params_set_oneparam(MyParams, "eigen_params", "nb_eig"      , 10) \n
* Params_set_oneparam(MyParams, "eigen_params", "lower_ibound", 50) \n
* Params_set_oneparam(MyParams, "eigen_params", "upper_ibound", 100) \n
* Params_set_oneparam(MyParams, "eigen_params", "lower_rbound", 1.1) \n
* Params_set_oneparam(MyParams, "eigen_params", "upper_rbound", 500.0) \n
*
*/

/*! \struct PARAMS_stokes_solver
* \brief the structure for storing all kind of parameters, for all kind of problems
*/

/*! \struct PARAMS_navierstokes_solver
* \brief the structure for storing all kind of parameters, for all kind of problems
*/

/*! \struct PARAMS_matrix_solver
* \brief the structure for storing all kind of parameters, for all kind of problems
*
* Params_set_oneparam(MyParams, "matrix_solver_params", "max_steps",  500  )\n
* Params_set_oneparam(MyParams, "matrix_solver_params", "eps_steps", 1.e-12) \n
*/

/*! \struct PARAMS_graphics_output
* \brief the structure for storing all kind of graphics output formats
*
* Params_set_oneparam(MyParams, "graphics_output_params", "GNUPLOT",     1) # GNUPLOT \n
* Params_set_oneparam(MyParams, "graphics_output_params", "MATLAB",      0) # MATLAB \n
* Params_set_oneparam(MyParams, "graphics_output_params", "VTK",         1) # VTK \n
* Params_set_oneparam(MyParams, "graphics_output_params", "SILO",        1) # SILO \n
*
*/

/*! \struct PARAMS_graphics_plots
* \brief the structure for storing all kind of parameters for PGPLOT or LIBSCIPLOT plots
*/

/*! \struct PARAMS_miscellaneous_params
* \brief the structure for storing miscellaneous parameters
*/
