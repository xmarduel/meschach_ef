#ifndef MEF_BOUNDARY_CONDITIONS_3D_H
#define MEF_BOUNDARY_CONDITIONS_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file boundary_conditions_3D.h
 *
 * Contains the structure of boundary conditions in 3 dimensions
 *
 * --------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#define  NBMAX_BC_3D_FUNCTIONS   11

/*---------------------------------------------------------------------- */

typedef enum {

   BC_3De_UNKNOWN   = -1,    /**< uninitialized BC type */

   BC_3De_DIRICHLET =  0,    /**< BC type is Dirichlet */
   BC_3De_NEUMANN   =  1,    /**< BC type is Neumann   */
   BC_3De_CAUCHY    =  2,    /**< BC type is Cauchy    */
   BC_3De_ROBIN     =  3     /**< BC type is Robin     */

} BC_3D_TYPE;


typedef struct {

   char   BCname[16];         /**< name of bc : dirichlet, neumann, cauchy or robin         */

   FUN_3D Fun1[3][NBMAX_BC_3D_FUNCTIONS]; /**< 3xNBMAX_BC_3D_FUNCTIONS functions on the border (1) */
   FUN_3D Fun2[3][NBMAX_BC_3D_FUNCTIONS]; /**< 3xNBMAX_BC_3D_FUNCTIONS functions on the border (2) */

   Real tps;                  /**< for time-dependent problems */

   int  current_selected_axe; /**< tag : which axing is currently selected (AXEe_X, AXEe_Y or AXEe_Z) */

} BC_3D_XTYPE ;            /* to INITIALIZE in Bc3D_XTYPE_get( char* name )            */


typedef struct {

   BC_3D_XTYPE* BC_dirichlet;   /**< gives the functions for Dirichlet BC on the given references values */
   BC_3D_XTYPE* BC_neumann;     /**< gives the functions for Neumann   BC on the given references values */
   BC_3D_XTYPE* BC_cauchy;      /**< gives the functions for Cauchy    BC on the given references values */
   BC_3D_XTYPE* BC_robin;       /**< gives the functions for Robin     BC on the given references values */

   BC_3D_TYPE   BC_MASK[3][NBMAX_BC_3D_FUNCTIONS]; /**< for each reference value, indicate which type of BC is selected */

} BC_3D ;

#define BC_3D_NULL  ((BC_3D*)NULL)

#define BC_3D_XTYPE_FREE(MyBC) ( Bc3D_XTYPE_free(MyBC), (MyBC)=(BC_3D_XTYPE *)NULL )
#define BC_3D_FREE(MyBC)       ( Bc3D_free(MyBC), (MyBC)=(BC_3D *)NULL )


BC_3D      *Bc3D_get(void);
BC_3D      *Bc3D_setup_from_params(const PARAMS *params);

int         Bc3D_free(BC_3D  *MyBC);

BC_3D      *Bc3D_setBcType               (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe );

/* these two functions are also used in the Python extension module */
BC_3D      *Bc3D_setFunction1            (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
BC_3D      *Bc3D_setFunction2            (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
/* these two functions are also used in the Python extension module */

BC_3D      *Bc3D_setCFunction            (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_3D phi);
BC_3D      *Bc3D_setCFunction2           (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_3D phi);

BC_3D      *Bc3D_setCFunctionTransient   (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_4D phi);
BC_3D      *Bc3D_setCFunctionTransient2  (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_4D phi);


BC_3D      *Bc3D_setLUAFunction          (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe , const char* def);
BC_3D      *Bc3D_setLUAFunction2         (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe , const char* def);

BC_3D      *Bc3D_setLUAFunctionTransient (BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe , const char* def);
BC_3D      *Bc3D_setLUAFunctionTransient2(BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe , const char* def);


BC_3D      *Bc3D_setTps                  (BC_3D *MyBC, Real tps    );
BC_3D      *Bc3D_setCurrentSelectedAxe   (BC_3D *MyBC, int axe     );


Real        Bc3D_evalFunction1         (const BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, Real x, Real y, Real z);
Real        Bc3D_evalFunction2         (const BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, Real x, Real y, Real z);

BC_3D_TYPE  Bc3D_getBcType             (const BC_3D* MyBC, int axe, int ref);

int         Bc3D_checkBC               (const BC_3D* MyBC, int axe);

int         Bc3D_hasDirichletBC        (const BC_3D* MyBC, int axe);
int         Bc3D_hasNeumannBC          (const BC_3D* MyBC, int axe);
int         Bc3D_hasCauchyBC           (const BC_3D* MyBC, int axe);
int         Bc3D_hasRobinBC            (const BC_3D* MyBC, int axe);

int         Bc3D_getNumberOfBC         (const BC_3D* MyBC, int axe);
int         Bc3D_getNumberOfBcOnSomms  (const BC_3D* MyBC, int axe);
int         Bc3D_getNumberOfBcOnFaces  (const BC_3D* MyBC, int axe);

#ifdef __cplusplus
}
#endif

#endif

/*! \enum BC_3D_TYPE
* \brief List the different types of boundary conditions
*/

/*! \struct BC_3D_XTYPE
* \brief an intermediate structure for BC in 3-D
*
* For a given type of boundary condition, store the corresponding functions\n
* as well as the time ( for transient problems)
*/

/*! \struct BC_3D
* \brief the structure for the BC in 3-D
*
* Store all functions for all types of boundary conditions
*/


/*! \fn  Bc3D_get(void);
* \brief create our main structure for BC in 3-D
*
* Usage :\n
*
* BC_3D *MyBC = Bc3D_get()\n
*
* Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 1, AXEe_X) to set Dirichlet BC on Boundary Point with reference 1\n
* Bc3D_setBcType(MyBC, BC_3De_NEUMANN  , 2, AXEe_X) to set Neumann   BC on Boundary Point with reference 2\n
* Bc3D_setBcType(MyBC, BC_3De_CAUCHY   , 3, AXEe_X) to set Cauchy    BC on Boundary Point with reference 3\n
* Bc3D_setBcType(MyBC, BC_3De_ROBIN    , 4, AXEe_X) to set Robin     BC on Boundary Point with reference 4\n
* Bc3D_setBcType(MyBC, BC_3De_DIRICHLET, 5, AXEe_X) to set Dirichlet BC on Boundary Point with reference 5\n
*
* Bc3D_setCFunction(MyBC, BC_3De_DIRICHLET, 1, AXEe_X, MyFunDirichlet_a) \n
* Bc3D_setCFunction(MyBC, BC_3De_NEUMANN  , 3, AXEe_X, MyFunNeumann) \n
* Bc3D_setCFunction(MyBC, BC_3De_CAUCHY   , 3, AXEe_X, MyFunCauchy) \n
* Bc3D_setCFunction(MyBC, BC_3De_ROBIN    , 4, AXEe_X, MyFunRobin1) \n
* Bc3D_setCFunction(MyBC, BC_3De_DIRICHLET, 5, AXEe_X, MyFunDirichlet_b) \n
*
* Bc3D_setCFunction3(MyBC, "robin", 4, AXEe_X, MyFunRobin2) \n
*/

/*! \fn Bc3D_setBcType             ( BC_3D* MyBC, BC_3D_TYPE type, int ref, int axe )
*
* \param MyBC : the BC structure
* \param type : the type of boundary condition we set on a given reference (next arg)
* \param ref  : the reference index that we consider
* \param axe  : AXEe_X or AXEe_Y or AXEe_Z
*
* Ex: If we have Dirichlet boundary conditions on the somms of the geometry with reference 1, we do \n
* Bc3D_setBcType(BC, BC_3De_DIRICHLET, 1, AXEe_X)
* Ex: If we have Neumann boundary conditions on the faces of the geometry with reference 2, we do \n
* Bc3D_setBcType(BC, BC_3De_NEUMANN  , 2, AXEe_X)\n
* Ex: If we have Cauchy boundary conditions on the somms of the geometry with reference 3, we do \n
* Bc3D_setBcType(BC, BC_3De_CAUCHY   , 3, AXEe_X)
* Ex: If we have Robin boundary conditions on the somms of the geometry with reference 4, we do \n
* Bc3D_setBcType(BC, BC_3De_ROBIN    , 4, AXEe_X)
*/

/*! \fn Bc3D_getBcType             ( const BC_3D* MyBC, int axe, int ref)
*
* \param MyBC : the BC structure
* \param axe  : AXEe_X or AXEe_Y or AXEe_Z
* \param ref  : the reference index that we consider
*
* return the mask value that we set : BC->BC_MASK[axe][ref]
*/

/*! \fn Bc3D_setFunction1         ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
*
* \param MyBC       : the BC structure
* \param bctype     : the type of boundary condition we set a function into
* \param ref_s      : the reference index of the geometry that we consider
* \param axe        : AXEe_X or AXEe_Y or AXEe_Z
* \param phi        : the function we set into the BC_3D structure
* \param clientdata : the function we set into the BC_3D structure (used for Python function)
*
* set the "first" function in the boundary condition \n
* Note: Do not use this function. It should be declared static (and thus hidden), but we have to expose it for the python wrapper.
*/

/*! \fn Bc3D_setFunction2          ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
*
* \param MyBC       : the BC structure
* \param bctype     : the type of boundary condition we set a function into
* \param ref_s      : the reference index of the geometry that we consider
* \param axe        : AXEe_X or AXEe_Y or AXEe_Z
* \param phi        : the function we set into the BC_3D structure
* \param clientdata : the function we set into the BC_3D structure (used for Python function)
*
* set the "second" function in the boundary condition \n
* Note: Do not use this function. It should be declared static (and thus hidden), but we have to expose it for the python wrapper.
*/

/*! \fn Bc3D_setCFunction          ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_3D phi)
*
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y or AXEe_Z
* \param phi    : the function we set into the BC_3D structure
*
* Ex: to set a function corresponding to the faces of the geometry with reference 1 (with Dirichlet boundary conditions), \n
* do Bc3D_setCFunction(BC, BC_3De_DIRICHLET, 1, AXEe_X, MyFunDirichlet) \n
* Ex: to set a function corresponding to the somms of the geometry with reference 2 (with Neumann boundary conditions), \n
* do Bc3D_setCFunction(BC, BC_3De_NEUMANN, 2, AXEe_X, MyFunNeumann) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 3 (with Cauchy boundary conditions), \n
* do Bc3D_setCFunction(BC, BC_3De_CAUCHY, 3, AXEe_X, MyFunCauchy) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc3D_setCFunction(BC, BC_3De_ROBIN, 4, AXEe_X, MyFunRobin1) \n
*/

/*! \fn Bc3D_setCFunction2          ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_3D phi)
*
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y or AXEe_Z
* \param phi    : the function we set into the BC_3D structure
*
* Ex: to set the "first" function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc3D_setCFunction2(BC, BC_3De_ROBIN, 4, AXEe_X, MyFunRobin2) \n
*
* Attention : Use this fuction only for the "robin" boundary conditions
*/

/*! \fn Bc3D_setCFunctionTransient   ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_4D phi)
*
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y or AXEe_Z
* \param phi    : the function we set into the BC_3D structure
*
* Ex: to set a function corresponding to the faces of the geometry with reference 1 (with Dirichlet boundary conditions), \n
* do Bc3D_setCFunctionTransient(BC, BC_3De_DIRICHLET, 1, AXEe_X, MyFunDirichlet) \n
* Ex: to set a function corresponding to the somms of the geometry with reference 2 (with Neumann boundary conditions), \n
* do Bc3D_setCFunctionTransient(BC, BC_3De_NEUMANN, 2, AXEe_X, MyFunNeumann) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 3 (with Cauchy boundary conditions), \n
* do Bc3D_setCFunctionTransient(BC, BC_3De_CAUCHY, 3, AXEe_X, MyFunCauchy) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc3D_setCFunctionTransient(BC, BC_3De_ROBIN, 4, AXEe_X, MyFunRobin1) \n
*
*/

/*! \fn Bc3D_setCFunctionTransient2   ( BC_3D* MyBC, BC_3D_TYPE bctype, int ref_s, int axe, FUNC_4D phi)
*
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X or AXEe_Y or AXEe_Z
* \param phi    : the function we set into the BC_3D structure
*
* Ex: to set the "second" function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc3D_setCFunctionTransient2(BC, "robin", 4, AXEe_X, MyFunRobin2) \n
*
* Attention : Use this fuction only for the "robin" boundary conditions
*/

/*! \fn Bc3D_checkBC               ( const BC_3D* MyBC , int axe )
*
* \param MyBC   : the BC structure we check
* \param axe    : AXEe_X or AXEe_Y or AXEe_Z
*
* We check the BC: where the values of BC_MASK are set (not BC_UNKNOWN), the correspondings\n
* functions int the structure BC_3D_XTYPE must be set.\n
* Ex: if BC_MASK[AXEe_X][1] = BC_3D_NEUMANN, then the function BC->BC_neumann->Fun[AXEe_X][1] must be set \n
* Ex: if BC_MASK[AXEe_X][2] = BC_3D_DIRICHLET, then the function BC->BC_dirichlet->Fun[AXEe_X][2] must be set
*/

/*! \fn Bc3D_setTps               ( BC_3D* MyBC, Real tps)
*
* \param MyBC   : the BC structure we check
* \param tps    : the current time
*
* In transient problems, set the current time in the structure. It will be needed when \n
* evaluating the functions on the bc (which depend on time)
*/

