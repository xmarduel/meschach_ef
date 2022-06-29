#ifndef MEF_BOUNDARY_CONDITIONS_1D_H
#define MEF_BOUNDARY_CONDITIONS_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file boundary_conditions_1D.h
 *
 * Contains the structure of boundary conditions in 1 dimension
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#define  NBMAX_BC_1D_FUNCTIONS   11

/*------------------------------------------------------------------------- */

typedef enum {

   BC_1De_UNKNOWN   = -1,    /**< uninitialized BC type */

   BC_1De_DIRICHLET =  0,    /**< BC type is Dirichlet */
   BC_1De_NEUMANN   =  1,    /**< BC type is Neumann   */
   BC_1De_CAUCHY    =  2,    /**< BC type is Cauchy    */
   BC_1De_ROBIN     =  3     /**< BC type is Robin     */

} BC_1D_TYPE;



typedef struct {

   char   BCname[16];      /**< name of bc : dirichlet, neumann, cauchy or robin         */

   FUN_1D Fun1[1][NBMAX_BC_1D_FUNCTIONS];  /**< NBMAX_BC_1D_FUNCTIONS functions on the border (1) */
   FUN_1D Fun2[1][NBMAX_BC_1D_FUNCTIONS];  /**< NBMAX_BC_1D_FUNCTIONS functions on the border (2) */

   Real tps;                              /**< for time-dependent problems */

} BC_1D_XTYPE ;


typedef struct {

   BC_1D_XTYPE* BC_dirichlet;   /**< gives the functions for Dirichlet BC on the given references values */
   BC_1D_XTYPE* BC_neumann;     /**< gives the functions for Neumann   BC on the given references values */
   BC_1D_XTYPE* BC_cauchy;      /**< gives the functions for Cauchy    BC on the given references values */
   BC_1D_XTYPE* BC_robin;       /**< gives the functions for Robin     BC on the given references values */

   BC_1D_TYPE   BC_MASK[1][NBMAX_BC_1D_FUNCTIONS]; /**< for each reference value, indicate which type of BC is selected */

} BC_1D ;

#define BC_1D_NULL  ((BC_1D*)NULL)

#define BC_1D_XTYPE_FREE(MyBC) ( Bc1D_XTYPE_free(MyBC), (MyBC)=(BC_1D_XTYPE *)NULL )
#define BC_1D_FREE(MyBC)       ( Bc1D_free(MyBC),       (MyBC)=(BC_1D *)NULL )


BC_1D      *Bc1D_get(void);
BC_1D      *Bc1D_setup_from_params(const PARAMS *params);

int         Bc1D_free(BC_1D *MyBC);

BC_1D      *Bc1D_setBcType               (BC_1D* MyBC, BC_1D_TYPE bctype, int ref, int axe);

/* these two functions are also used in the Python extension module */
BC_1D      *Bc1D_setFunction1            (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
BC_1D      *Bc1D_setFunction2            (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
/* these two functions are also used in the Python extension module */

BC_1D      *Bc1D_setCFunction            (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_1D phi);
BC_1D      *Bc1D_setCFunction2           (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_1D phi);

BC_1D      *Bc1D_setCFunctionTransient   (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_2D phi);
BC_1D      *Bc1D_setCFunctionTransient2  (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_2D phi);

BC_1D      *Bc1D_setLUAFunction          (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def);
BC_1D      *Bc1D_setLUAFunction2         (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def);

BC_1D      *Bc1D_setLUAFunctionTransient (BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def);
BC_1D      *Bc1D_setLUAFunctionTransient2(BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe , const char* def);

BC_1D      *Bc1D_setTps                  (BC_1D* MyBC, Real tps    );


Real        Bc1D_evalFunction1           (const BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, Real x);
Real        Bc1D_evalFunction2           (const BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, Real x);

BC_1D_TYPE  Bc1D_getBcType               (const BC_1D* MyBC, int axe, int ref);

int         Bc1D_checkBC                 (const BC_1D* MyBC );

int         Bc1D_hasDirichletBC          (const BC_1D* MyBC, int axe);
int         Bc1D_hasNeumannBC            (const BC_1D* MyBC, int axe);
int         Bc1D_hasCauchyBC             (const BC_1D* MyBC, int axe);
int         Bc1D_hasRobinBC              (const BC_1D* MyBC, int axe);

int         Bc1D_getNumberOfBC           (const BC_1D* MyBC, int axe);
int         Bc1D_getNumberOfBcOnSomms    (const BC_1D* MyBC, int axe);
int         Bc1D_getNumberOfBcOnFaces    (const BC_1D* MyBC, int axe);



#ifdef __cplusplus
}
#endif

#endif

/*! \enum BC_1D_TYPE
* \brief The possible types of boundary conditions
*
* list the different types of boundary conditions
*/

/*! \struct BC_1D_XTYPE
* \brief an intermediate structure for BC in 1-D
*
* For a given type of boundary condition, store the corresponding functions\n
* as well as the time ( for transient problems)
*/

/*! \struct BC_1D
* \brief the structure for the BC in 1-D
*
* Store all functions for all types of boundary conditions
*/

/*! \fn  Bc1D_get(void);
* \brief create our main structure for BC in 1-D
*
* Usage :
*
* BC_1D *MyBC = Bc1D_get()\n
* \n
* Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 1, AXEe_X) to set Dirichlet BC on Boundary Point with reference 1\n
* Bc1D_setBcType(MyBC, BC_1De_NEUMANN  , 2, AXEe_X) to set Neumann   BC on Boundary Point with reference 2\n
* Bc1D_setBcType(MyBC, BC_1De_CAUCHY   , 3, AXEe_X) to set Cauchy    BC on Boundary Point with reference 3\n
* Bc1D_setBcType(MyBC, BC_1De_ROBIN    , 4, AXEe_X) to set Robin     BC on Boundary Point with reference 4\n
* Bc1D_setBcType(MyBC, BC_1De_DIRICHLET, 5, AXEe_X) to set Dirichlet BC on Boundary Point with reference 5\n
* \n
* Bc1D_setCFunction(MyBC, BC_1De_DIRICHLET, 1, AXEe_X, MyFunDirichlet_a)\n
* Bc1D_setCFunction(MyBC, BC_1De_NEUMANN  , 2, AXEe_X, MyFunNeumann)\n
* Bc1D_setCFunction(MyBC, BC_1De_CAUCHY   , 3, AXEe_X, MyFunCauchy)\n
* Bc1D_setCFunction(MyBC, BC_1De_ROBIN    , 4, AXEe_X, MyFunRobin1)\n
* Bc1D_setCFunction(MyBC, BC_1De_DIRICHLET, 5, AXEe_X, MyFunDirichlet_b)\n
* \n
* Bc1D_setCFunction2(MyBC, BC_1De_ROBIN   , 4, AXEe_X, MyFunRobin2)\n
*/

/*! \fn  Bc1D_free(BC_1D * MyBC);
* \brief release the structure MyBC
* \param MyBC: the structure to release
*/

/*! \fn Bc1D_setBcType             ( BC_1D* MyBC, BC_1D_TYPE type, int ref, int axe )
* \brief Set in MyBC the type the bc for the nodes of reference "ref" on axe "axe"
* \param MyBC : the BC structure
* \param type : the type of boundary condition we set on a given reference (next arg)
* \param ref  : the reference index that we consider
* \param axe  : AXEe_X only
*
* Ex: If we have Dirichlet boundary conditions on the somms of the geometry with reference 1, we do \n
* Bc1D_setBcType(BC, BC_1De_DIRICHLET, 1, AXEe_X)
* Ex: If we have Neumann boundary conditions on the faces of the geometry with reference 2, we do \n
* Bc1D_setBcType(BC, BC_1De_NEUMANN  , 2, AXEe_X)\n
* Ex: If we have Cauchy boundary conditions on the somms of the geometry with reference 3, we do \n
* Bc1D_setBcType(BC, BC_1De_CAUCHY   , 3, AXEe_X)
* Ex: If we have Robin boundary conditions on the somms of the geometry with reference 4, we do \n
* Bc1D_setBcType(BC, BC_1De_ROBIN    , 4, AXEe_X)
*/

/*! \fn Bc1D_getBcType             ( const BC_1D* MyBC, int axe, int ref)
* \brief  Get in MyBC the type the bc for the nodes of reference "ref" on axe "axe"
* \param MyBC : the BC structure
* \param axe  : AXEe_X only
* \param ref  : the reference index that we consider
*
* return the mask value that we set : BC->BC_MASK[axe][ref]
*/

/*! \fn Bc1D_setFunction1         ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
* \brief Set the "first" function at axe "axe" for node of reference "ref_s" in MyBC
* \param MyBC       : the BC structure
* \param bctype     : the type of boundary condition we set a function into
* \param ref_s      : the reference index of the geometry that we consider
* \param axe        : AXEe_X only
* \param phi        : the function we set into the BC_1D structure
* \param clientdata : the function we set into the BC_1D structure (used for Python function)
*
* set the "first" function in the boundary condition \n
* Note: Do not use this function. It should be declared static (and thus hidden), but we have to expose it for the python wrapper.
*/

/*! \fn Bc1D_setFunction2          ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUN_TYPE type, void* phi, void* clientdata);
* \brief Set the "second" function at axe "axe" for node of reference "ref_s" in MyBC
* \param MyBC       : the BC structure
* \param bctype     : the type of boundary condition we set a function into
* \param ref_s      : the reference index of the geometry that we consider
* \param axe        : AXEe_X only
* \param phi        : the function we set into the BC_1D structure
* \param clientdata : the function we set into the BC_1D structure (used for Python function)
*
* set the "second" function in the boundary condition \n
* Note: Do not use this function. It should be declared static (and thus hidden), but we have to expose it for the python wrapper.
*/

/*! \fn Bc1D_setCFunction          ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_1D phi)
* \brief Set the "first" function at axe "axe" for node of reference "ref_s" in MyBC - phi is a "C" function -
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X only
* \param phi    : the function we set into the BC_1D structure
*
* Ex: to set a function corresponding to the faces of the geometry with reference 1 (with Dirichlet boundary conditions), \n
* do Bc1D_setCFunction(BC, BC_1De_DIRICHLET, 1, AXEe_X, MyFunDirichlet) \n
* Ex: to set a function corresponding to the somms of the geometry with reference 2 (with Neumann boundary conditions), \n
* do Bc1D_setCFunction(BC, BC_1De_NEUMANN  , 2, AXEe_X, MyFunNeumann) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 3 (with Cauchy boundary conditions), \n
* do Bc1D_setCFunction(BC, BC_1De_CAUCHY   , 3, AXEe_X, MyFunCauchy) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc1D_setCFunction(BC, BC_1De_ROBIN    , 4, AXEe_X, MyFunRobin1) \n
*/

/*! \fn Bc1D_setCFunction2          ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_1D phi)
* \brief Set the "second" function at axe "axe" for node of reference "ref_s" in MyBC - phi is a "C" function -
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X only
* \param phi    : the function we set into the BC_1D structure
*
* Ex: to set the "second" function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc1D_setCFunction2(BC, "robin", 4, AXEe_X, MyFunRobin2) \n
*
* Attention : Use this fuction only for the "robin" boundary conditions
*/

/*! \fn Bc1D_setCFunctionTransient   ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_2D phi)
* \brief Set the "first" function at axe "axe" for node of reference "ref_s" in MyBC - phi is a "C" function -
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X only
* \param phi    : the function we set into the BC_1D structure (function of (x,t))
*
* Ex: to set a function corresponding to the faces of the geometry with reference 1 (with Dirichlet boundary conditions), \n
* do Bc1D_setCFunctionTransient1(BC, BC_1De_DIRICHLET, 1, AXEe_X, MyFunDirichlet) \n
* Ex: to set a function corresponding to the somms of the geometry with reference 2 (with Neumann boundary conditions), \n
* do Bc1D_setCFunctionTransient(BC, BC_1De_NEUMANN   , 2, AXEe_X, MyFunNeumann) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 3 (with Cauchy boundary conditions), \n
* do Bc1D_setCFunctionTransient(BC, BC_1De_CAUCHY    , 3, AXEe_X, MyFunCauchy) \n
* Ex: to set a function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc1D_setCFunctionTransient(BC, BC_1De_ROBIN     , 4, AXEe_X, MyFunRobin1) \n
*
*/

/*! \fn Bc1D_setCFunctionTransient2   ( BC_1D* MyBC, BC_1D_TYPE bctype, int ref_s, int axe, FUNC_2D phi)
* \brief Set the "second" function at axe "axe" for node of reference "ref_s" in MyBC - phi is a "C" function -
* \param MyBC   : the BC structure
* \param bctype : the type of boundary condition we set a function into
* \param ref_s  : the reference index that we consider
* \param axe    : AXEe_X only
* \param phi    : the function we set into the BC_1D structure (function of (x,t))
*
* Ex: to set the "second" function corresponding to the faces of the geometry with reference 4 (with Robin boundary conditions), \n
* do Bc1D_setCFunctionTransient2(BC, "robin", 4, AXEe_X, MyFunRobin2) \n
*
* Attention : Use this fuction only for the "robin" boundary conditions
*/

/*! \fn Bc1D_setTps               ( BC_1D* MyBC, Real tps)
* \brief Set the current time in MyBC - for transient problems -
* \param MyBC  : the BC structure we check
* \param tps   : the current time
*
* In transient problems, set the current time in the structure. It will be needed when \n
* evaluating the functions on the bc (which depend on time)
*/

/*! \fn Bc1D_checkBC               ( const BC_1D* MyBC )
* \brief Check if the functions for BC are correctly set
* \param MyBC   : the BC structure we check
*
* We check the BC: where the values of BC_MASK are set (not BC_UNKNOWN), the correspondings\n
* functions int the structure BC_1D_XTYPE must be set.\n
* Ex: if BC_MASK[AXEe_X][1] = BC_1D_NEUMANN, then the function BC->BC_neumann->Fun[AXEe_X][1] must be set \n
* Ex: if BC_MASK[AXEe_X][2] = BC_1D_DIRICHLET, then the function BC->BC_dirichlet->Fun[AXEe_X][2] must be set
*/

