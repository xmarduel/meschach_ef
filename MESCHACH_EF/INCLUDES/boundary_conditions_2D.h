#ifndef MEF_BOUNDARY_CONDITIONS_2D_H
#define MEF_BOUNDARY_CONDITIONS_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file boundary_conditions_2D.h
 * 
 * Contains the structure of boundary conditions in 2 dimensions
 *
 * --------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
	
#define  NBMAX_BC_2D_FUNCTIONS   12

/*---------------------------------------------------------------------- */

typedef enum {

   BC_2De_UNKNOWN   = -1,    /**< uninitialized BC type \n*/

   BC_2De_DIRICHLET =  0,    /**< BC type is Dirichlet */
   BC_2De_NEUMANN   =  1,    /**< BC type is Neumann   */
   BC_2De_CAUCHY    =  2,    /**< BC type is Cauchy    */
   BC_2De_ROBIN     =  3     /**< BC type is Robin     */

} BC_2D_TYPE;


typedef struct {

   char   BCname[16];         /**< name of bc : dirichlet, neumann, cauchy or robin         */
  
   FUN_2D Fun1[2][NBMAX_BC_2D_FUNCTIONS];  /**< 2 x NBMAX_BC_2D_FUNCTIONS functions on the border */
   FUN_2D Fun2[2][NBMAX_BC_2D_FUNCTIONS];  /**< 2 x NBMAX_BC_2D_FUNCTIONS functions on the border */
   
   Real tps;                   /**< for time-dependent problems */

   int  current_selected_axe;  /**< tag : which axing is currently selected (AXEe_X or AXEe_Y) */

} BC_2D_XTYPE ;            /* to INITIALIZE in Bc2D_XTYPE_get( char* name )            */


typedef struct {

   BC_2D_XTYPE* BC_dirichlet;   /**< gives the functions for Dirichlet BC on the given references values */
   BC_2D_XTYPE* BC_neumann;     /**< gives the functions for Neumann   BC on the given references values */
   BC_2D_XTYPE* BC_cauchy;      /**< gives the functions for Cauchy    BC on the given references values */
   BC_2D_XTYPE* BC_robin;       /**< gives the functions for Robin     BC on the given references values */

   BC_2D_TYPE   BC_MASK[2][NBMAX_BC_2D_FUNCTIONS]; /**< for each reference value, indicate which type of BC is selected */

} BC_2D ;

#define BC_2D_NULL  ((BC_2D*)NULL)

#define BC_2D_XTYPE_FREE(MyBC) ( Bc2D_XTYPE_free(MyBC), (MyBC)=(BC_2D_XTYPE *)NULL )
#define BC_2D_FREE(MyBC)       ( Bc2D_free(MyBC),       (MyBC)=(BC_2D *)NULL )


BC_2D      *Bc2D_get(void);
BC_2D      *Bc2D_setup_from_params(const PARAMS *params);
	
int         Bc2D_free(BC_2D  *MyBC);

BC_2D      *Bc2D_setBcType              (BC_2D* MyBC, BC_2D_TYPE bctype, int ref, int axe);

/* these two functions are also used in the Python extension module */
BC_2D      *Bc2D_setFunction1           (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
BC_2D      *Bc2D_setFunction2           (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
/* these two functions are also used in the Python extension module */

BC_2D      *Bc2D_setCFunction           (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_2D phi);
BC_2D      *Bc2D_setCFunction2          (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_2D phi);

BC_2D      *Bc2D_setCFunctionTransient  (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_3D phi);
BC_2D      *Bc2D_setCFunctionTransient2 (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_3D phi);

BC_2D      *Bc2D_setLUAFunction          (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def);
BC_2D      *Bc2D_setLUAFunction2         (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def);
   
BC_2D      *Bc2D_setLUAFunctionTransient (BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def);
BC_2D      *Bc2D_setLUAFunctionTransient2(BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe , const char* def);
   
   
BC_2D      *Bc2D_setTps                  (BC_2D *MyBC, Real tps    );
BC_2D      *Bc2D_setCurrentSelectedAxe   (BC_2D *MyBC, int axe     );


Real        Bc2D_evalFunction1         (const BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, Real x, Real y );
Real        Bc2D_evalFunction2         (const BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, Real x, Real y );

BC_2D_TYPE  Bc2D_getBcType             (const BC_2D* MyBC, int axe, int ref);

int         Bc2D_checkBC               (const BC_2D* MyBC, int axe);

int         Bc2D_hasDirichletBC        (const BC_2D* MyBC, int axe);
int         Bc2D_hasNeumannBC          (const BC_2D* MyBC, int axe);
int         Bc2D_hasCauchyBC           (const BC_2D* MyBC, int axe);
int         Bc2D_hasRobinBC            (const BC_2D* MyBC, int axe);

int         Bc2D_getNumberOfBC         (const BC_2D* MyBC, int axe);
int         Bc2D_getNumberOfBcOnSomms  (const BC_2D* MyBC, int axe);
int         Bc2D_getNumberOfBcOnFaces  (const BC_2D* MyBC, int axe);

#ifdef __cplusplus
}
#endif

#endif

/*! \enum BC_2D_TYPE
* \brief List the different types of boundary conditions
*/

/*! \struct BC_2D_XTYPE
* \brief an intermediate structure for BC in 2-D
*
* For a given type of boundary condition, store the corresponding functions\n
* as well as the time ( for transient problems)
*/

/*! \struct BC_2D
* \brief the structure for the BC in 2-D
*
* Store all functions for all types of boundary conditions
*/


/*! \fn  Bc2D_get(void);
* \brief Create our main structure for BC in 2-D
*
* Usage :\n
*
* BC_2D *MyBC = Bc2D_get()\n
*
* Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 1, AXEe_X) to set Dirichlet BC on Boundary Point with reference 1\n
* Bc2D_setBcType(MyBC, BC_2De_NEUMANN  , 2, AXEe_X) to set Neumann   BC on Boundary Point with reference 2\n
* Bc2D_setBcType(MyBC, BC_2De_CAUCHY   , 3, AXEe_X) to set Cauchy    BC on Boundary Point with reference 3\n
* Bc2D_setBcType(MyBC, BC_2De_ROBIN    , 4, AXEe_X) to set Robin     BC on Boundary Point with reference 4\n
* Bc2D_setBcType(MyBC, BC_2De_DIRICHLET, 5, AXEe_X) to set Dirichlet BC on Boundary Point with reference 5\n
*
* Bc2D_setCFunction(MyBC, BC_2De_DIRICHLET, 1, AXEe_X, MyFunDirichlet_a) \n
* Bc2D_setCFunction(MyBC, BC_2De_NEUMANN  , 2, AXEe_X, MyFunNeumann) \n
* Bc2D_setCFunction(MyBC, BC_2De_CAUCHY   , 3, AXEe_X, MyFunCauchy) \n
* Bc2D_setCFunction(MyBC, BC_2De_ROBIN    , 4, AXEe_X, MyFunRobin1) \n
* Bc2D_setCFunction(MyBC, BC_2De_DIRICHLET, 5, AXEe_X, MyFunDirichlet_b) \n
*
* Bc2D_setCFunction2(MyBC, BC_2De_ROBIN   , 4, AXEe_X, MyFunRobin2) \n
*/

/*! \fn Bc2D_setBcType             ( BC_2D* MyBC, BC_2D_TYPE type, int ref, int axe )
* 
* \param MyBC : the BC structure
* \param type : the type of boundary condition we set on a given reference (next arg)
* \param ref  : the reference index that we consider
* \param axe  : AXEe_X or AXEe_Y
*
* Ex: If we have Dirichlet boundary conditions on the somms of the geometry with reference 1, we do \n
* Bc2D_setBcType(BC, BC_2De_DIRICHLET, 1, AXEe_X)
* Ex: If we have Neumann boundary conditions on the faces of the geometry with reference 2, we do \n
* Bc2D_setBcType(BC, BC_2De_NEUMANN  , 2, AXEe_X)\n
* Ex: If we have Cauchy boundary conditions on the somms of the geometry with reference 3, we do \n
* Bc2D_setBcType(BC, BC_2De_CAUCHY   , 3, AXEe_X)
* Ex: If we have Robin boundary conditions on the somms of the geometry with reference 4, we do \n
* Bc2D_setBcType(BC, BC_2De_ROBIN   , 4, AXEe_X)
*/

/*! \fn Bc2D_getBcType             ( const BC_2D* MyBC, int axe, int ref)
* 
* \param MyBC : the BC structure
* \param axe  : AXEe_X or AXEe_Y
* \param ref  : the reference index that we consider
*
* return the mask value that we set : BC->BC_MASK[axe][ref]
*/

/*! \fn Bc2D_setFunction1         ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
* 
* \param MyBC       : the BC structure
* \param bctype     : the type of boundary condition we set a function into
* \param ref_s      : the reference index of the geometry that we consider
* \param axe        : AXEe_X or AXEe_Y
* \param phi        : the function we set into the BC_2D structure
* \param clientdata : the function we set into the BC_2D structure (used for Python function)
*
* set the "first" function in the boundary condition \n
* Note: Do not use this function. It should be declared static (and thus hidden), but we have to expose it for the python wrapper.
*/

/*! \fn Bc2D_setFunction2         ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
*
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y
* \param phi    : the function we set into the BC_2D structure
*
* set the "second" function in the boundary condition \n
* Note: Do not use this function. It should be declared static (and thus hidden), but we have to expose it for the python wrapper.
*/


/*! \fn Bc2D_setCFunction          ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_2D phi)
* 
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y
* \param phi    : the function we set into the BC_2D structure
*
* Ex: to set a function corresponding to the faces of the geometry with reference 1 (with Dirichlet boundary conditions), \n
* do Bc2D_setCFunction(BC, BC_2De_DIRICHLET, 1, AXEe_X, MyFunDirichlet) \n
* Ex: to set a function corresponding to the somms of the geometry with reference 2 (with Neumann boundary conditions), \n
* do Bc2D_setCFunction(BC, BC_2De_NEUMANN  , 2, AXEe_X, MyFunNeumann) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 3 (with Cauchy boundary conditions), \n
* do Bc2D_setCFunction(BC, BC_2De_CAUCHY   , 3, AXEe_X, MyFunCauchy) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc2D_setCFunction(BC, BC_2De_ROBIN    , 4, AXEe_X, MyFunRobin1) \n
*/

/*! \fn Bc2D_setCFunction2          ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_2D phi)
* 
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y
* \param phi    : the function we set into the BC_2D structure
*
* Ex: to set the "second" function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc2D_setCFunction2(BC, "robin", 4, AXEe_X, MyFunRobin2) \n
*
* Attention : Use this fuction only for the "robin" boundary conditions
*/

/*! \fn Bc2D_setCFunctionTransient   ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_3D phi)
* 
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y
* \param phi    : the function we set into the BC_2D structure
*
* Ex: to set a function corresponding to the faces of the geometry with reference 1 (with Dirichlet boundary conditions), \n
* do Bc2D_setCFunctionTransient(BC, BC_2De_DIRICHLET, 1, AXEe_X, MyFunDirichlet) \n
* Ex: to set a function corresponding to the somms of the geometry with reference 2 (with Neumann boundary conditions), \n
* do Bc2D_setCFunctionTransient(BC, BC_2De_NEUMANN  , 2, AXEe_X, MyFunNeumann) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 3 (with Cauchy boundary conditions), \n
* do Bc2D_setCFunctionTransient(BC, BC_2De_CAUCHY   , 3, AXEe_X, MyFunCauchy) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc2D_setCFunctionTransient(BC, BC_2De_ROBIN    , 4, AXEe_X, MyFunRobin1) \n
*
*/

/*! \fn Bc2D_setCFunctionTransient2   ( BC_2D* MyBC, BC_2D_TYPE bctype, int ref_s, int axe, FUNC_3D phi)
* 
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y
* \param phi    : the function we set into the BC_2D structure
*
* Ex: to set the "second" function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc2D_setCFunctionTransient2(BC, BC_2De_ROBIN, 4, AXEe_X, MyFunRobin2) \n
*
* Attention : Use this fuction only for the "robin" boundary conditions
*/

/*! \fn Bc2D_checkBC               ( const BC_2D* MyBC , int axe )
* 
* \param MyBC   : the BC structure we check
* \param axe    : AXEe_X or AXEe_Y
*
* We check the BC: where the values of BC_MASK are set (not BC_UNKNOWN), the correspondings\n
* functions int the structure BC_1D_XTYPE must be set.\n
* Ex: if BC_MASK[AXEe_X][1] = BC_1D_NEUMANN, then the function BC->BC_neumann->Fun[AXEe_X][1] must be set \n
* Ex: if BC_MASK[AXEe_X][2] = BC_1D_DIRICHLET, then the function BC->BC_dirichlet->Fun[AXEe_X][2] must be set
*/

/*! \fn Bc2D_setTps               ( BC_2D* MyBC, Real tps)
* 
* \param MyBC   : the BC structure we check
* \param tps    : the current time
*
* In transient problems, set the current time in the structure. It will be needed when \n
* evaluating the functions on the bc (which depend on time)
*/

