#ifndef MEF_RHS_2D_H
#define MEF_RHS_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file rhs_2D.h
 *
 * Contains the structure of rhs in 2 dimension
 *
 * The structure contains a "double" array of functions.\n
 * For an homogeneous material, all the cells of the geometry have the same reference number (usually 0)
 * so that the structure can be set with \n
 *
 * MyRhs = Rhs2D_get() \n
 * MyRhs = Rhs2D_setCFunction( MyRhs, 0, AXEe_X, the_c_function1) # 0 correspond to the reference "0" of the geometry's element \n
 * MyRhs = Rhs2D_setCFunction( MyRhs, 0, AXEe_Y, the_c_function2) # 0 correspond to the reference "0" of the geometry's element \n
 *
 * MyRhs = Rhs2D_get() \n
 * MyRhs = Rhs2D_setLUAFunction( MyRhs, 0, AXEe_X, "x + y") # 0 correspond to the reference "0" of the geometry's element \n
 * MyRhs = Rhs2D_setLUAFunction( MyRhs, 0, AXEe_Y, "math.sin(x) + math.exp(y)") # 0 correspond to the reference "0" of the geometry's element \n
 *
 * In a Python program :  \n
 *
 * MyRhs = Rhs2D_get() \n
 * MyRhs = Rhs2D_setPythonFunction( MyRhs, 0, AXEe_X, the_python_function1) # 0 correspond to the reference "0" of the geometry's element \n
 * MyRhs = Rhs2D_setPythonFunction( MyRhs, 0, AXEe_Y, the_python_function2) # 0 correspond to the reference "0" of the geometry's element \n
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#define  NBMAX_RHS_2D_FUNCTIONS   11

/*---------------------------------------------------------------------- */


typedef struct {

   FUN_2D Fun[2][NBMAX_RHS_2D_FUNCTIONS];   /**< functions for the "first" and "second" equations   */

   Real tps;                                /**< the "current time" */

   int  current_selected_axe;               /**< the "current selected axe" */

} RHS_2D ;


#define RHS_2D_NULL  ((RHS_2D *)NULL)

#define RHS_2D_FREE(MyRHS) ( Rhs2D_free(MyRHS), (MyRHS)=(RHS_2D *)NULL )



RHS_2D* Rhs2D_get(void);
RHS_2D* Rhs2D_setup_from_params(const PARAMS* params);

int     Rhs2D_free(RHS_2D *MyRhs);

RHS_2D* Rhs2D_setFunction          (RHS_2D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata);

RHS_2D* Rhs2D_setCFunction         (RHS_2D* MyRhs, int ref_e, int axe, FUNC_2D phi);
RHS_2D* Rhs2D_setCFunctionTransient(RHS_2D* MyRhs, int ref_e, int axe, FUNC_3D phi);

RHS_2D* Rhs2D_setLUAFunction         (RHS_2D* MyRhs, int ref_e, int axe, const char *def);
RHS_2D* Rhs2D_setLUAFunctionTransient(RHS_2D* MyRhs, int ref_e, int axe, const char *def);

RHS_2D* Rhs2D_setTps               (RHS_2D* MyRhs, Real tps);
RHS_2D* Rhs2D_setCurrentSelectedAxe(RHS_2D* MyRhs, int axe );

Real    Rhs2D_evalFunction         (const RHS_2D* MyRhs, int ref_e, int axe, Real x, Real y);

#ifdef __cplusplus
}
#endif

#endif

/*! \struct RHS_2D
* \brief the structure for the RHS in 2-D
*
* The RHS structure contains the pointers to the functions defined in the domain\n
* as well as the time (for transient problems)
*/


/*! \fn Rhs2D_get          ( void );
* \brief Create an uninitialized RHS_2D structure (to be filled later)
*/

/*! \fn Rhs2D_free          (RHS_2D  *MyRhs);
* \brief Release the structure
*
* \param MyRhs   : the RHS structure
*/

/*! \fn Rhs2D_setFunction   ( RHS_2D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X or AXEe_Y
* \param type    : the type of the function we actually store ( "C" or "Python" function)
* \param phi     : a "C" function
* \param clientdata  : NULL for a "C" function, a pointer to the "Python" function in the other case
*
* Do not used this function - It should be actually declared "static",
* but we need it in the python extension module -
*/

/*! \fn Rhs2D_setCFunction   ( RHS_2D* MyRhs, int ref_e, int axe, FUNC_2D phi);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X or AXEe_Y
* \param phi     : the "C" function ( 2 arguments (x,y) )
*
* Set into the structure a function
*/

/*! \fn Rhs2D_setCFunctionTransient   ( RHS_2D* MyRhs, int ref_e, int axe, FUNC_3D phi);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X or AXEe_Y
* \param phi     : the "C" function ( 3 arguments (x,y,tps) )
*
* Set into the structure a function
*/

/*! \fn Rhs2D_setCFunction   ( RHS_2D* MyRhs, int ref_e, int axe, const char* def);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X or AXEe_Y
* \param def     : the "C" function as string ( 2 arguments (x,y) )
*
* Set into the structure a function
*/

/*! \fn Rhs2D_setLUAFunctionTransient   ( RHS_2D* MyRhs, int ref_e, int axe, const char* def);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X or AXEe_Y
* \param def     : the "C" function as string ( 3 arguments (x,y,tps) )
*
* Set into the structure a function
*/

/*! \fn Rhs2D_setTps          ( RHS_2D* MyRhs, Real tps);
*
* \param MyRhs : the RHS structure
* \param tps   : the "current" time
*
* Only useful when a "transient" function has been set into the structure
*/

/*! \fn Rhs2D_setCurrentSelectedAxe  ( RHS_2D* MyRhs, int axe  );
*
* \param MyRhs : the RHS structure
* \param axe   : the "current" axe : AXEe_X or AXEe_Y
*
* In the function "assemblage2D_vector_fun", the function used to build the rhs will be the one given with the "current selected axe"
*/

/*! \fn Rhs2D_evalFunction          ( const RHS_2D* MyRhs, int ref_e, int axe, Real x, Real y);
*
* \param MyRhs : the RHS structure
* \param ref_e :
* \param axe   : AXEe_X or AXEe_Y
* \param x     : the coordinate where to evaluate the function
* \param y     : the coordinate where to evaluate the function
*
* For transient function, set first the current time into the structure
*/

