#ifndef MEF_RHS_3D_H
#define MEF_RHS_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file rhs_3D.h
 *
 * Contains the structure of rhs in 3 dimension
 *
 * The structure contains a "triple" array of functions.\n
 * For an homogeneous material, all the cells of the geometry have the same reference number (usually 0)
 * so that the structure can be set with \n
 *
 * MyRhs = Rhs3D_get() \n
 * MyRhs = Rhs3D_setCFunction( MyRhs, 0, AXEe_X, the_c_function1) # 0 is the reference "0" of the geometry's element \n
 * MyRhs = Rhs3D_setCFunction( MyRhs, 0, AXEe_Y, the_c_function2) # 0 is the reference "0" of the geometry's element \n
 * MyRhs = Rhs3D_setCFunction( MyRhs, 0, AXEe_Z, the_c_function3) # 0 is the reference "0" of the geometry's element \n
 *
 * MyRhs = Rhs3D_get() \n
 * MyRhs = Rhs3D_setLUAFunction( MyRhs, 0, AXEe_X, "x + y + z") # 0 is the reference "0" of the geometry's element \n
 * MyRhs = Rhs3D_setLUAFunction( MyRhs, 0, AXEe_Y, "x + y + z") # 0 is the reference "0" of the geometry's element \n
 * MyRhs = Rhs3D_setLUAFunction( MyRhs, 0, AXEe_Z, "x + y + z") # 0 is the reference "0" of the geometry's element \n
 *
 * In a Python program : \n
 *
 * MyRhs = Rhs3D_get() \n
 * MyRhs = Rhs3D_setPythonFunction( MyRhs, 0, AXEe_X, the_python_function1) # 0 is the reference "0" of the geometry's element \n
 * MyRhs = Rhs3D_setPythonFunction( MyRhs, 0, AXEe_Y, the_python_function2) # 0 is the reference "0" of the geometry's element \n
 * MyRhs = Rhs3D_setPythonFunction( MyRhs, 0, AXEe_Z, the_python_function3) # 0 is the reference "0" of the geometry's element \n
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#define  NBMAX_RHS_3D_FUNCTIONS   11

/*---------------------------------------------------------------------- */

typedef struct {

   FUN_3D Fun[3][NBMAX_RHS_3D_FUNCTIONS];   /**< functions for the "first", "second" and "third" equations   */

   Real tps;                                /**< the "current" time */

   int  current_selected_axe;               /**< the "current selected axe" */

} RHS_3D ;


#define RHS_3D_NULL  ((RHS_3D *)NULL)

#define RHS_3D_FREE(MyRHS) ( Rhs3D_free(MyRHS), (MyRHS)=(RHS_3D *)NULL )


RHS_3D* Rhs3D_get(void);
RHS_3D* Rhs3D_setup_from_params(const PARAMS* params);

int     Rhs3D_free(RHS_3D  *MyRhs);

RHS_3D* Rhs3D_setFunction          (RHS_3D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata);

RHS_3D* Rhs3D_setCFunction         (RHS_3D* MyRhs, int ref_e, int axe, FUNC_3D phi);
RHS_3D* Rhs3D_setCFunctionTransient(RHS_3D* MyRhs, int ref_e, int axe, FUNC_4D phi);

RHS_3D* Rhs3D_setLUAFunction         (RHS_3D* MyRhs, int ref_e, int axe, const char *def);
RHS_3D* Rhs3D_setLUAFunctionTransient(RHS_3D* MyRhs, int ref_e, int axe, const char *def);

RHS_3D* Rhs3D_setTps               (RHS_3D* MyRhs, Real tps);
RHS_3D* Rhs3D_setCurrentSelectedAxe(RHS_3D* MyRhs, int axe );

Real    Rhs3D_evalFunction         (const RHS_3D* MyRhs, int ref_e, int axe, Real x, Real y, Real z);

#ifdef __cplusplus
}
#endif

#endif

/*! \struct RHS_3D
*
* The structure for the RHS in 3-D
*
* The RHS structure contains the pointers to the functions defined in the domain\n
* as well as the time (for transient problems)
*/


/*! \fn Rhs3D_get          ( void );
*
* Create an uninitialized structure
*/

/*! \fn Rhs3D_free          (RHS_3D  *MyRhs);
*
* \param MyRhs     : the RHS structure
*
* Release a RHS_3D structure
*/

/*! \fn Rhs3D_setFunction   ( RHS_3D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X , AXEe_Y or AXEe_Z
* \param type    : the type of the function we actually store ( "C" or "Python" function)
* \param phi     : a "C" function
* \param clientdata  : NULL for a "C" function, a pointer to the "Python" function in the other case
*
* Do not used this function - It should be actually declared "static",
* but we need it in the python extension module -
*/

/*! \fn Rhs3D_setCFunction   ( RHS_3D* MyRhs, int ref_e, int axe, FUNC_3D phi);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X , AXEe_Y or AXEe_Z
* \param phi     : the "C" function ( 3 arguments (x,y,z) )
*
* Set into the structure a function
*/

/*! \fn Rhs3D_setCFunctionTransient   ( RHS_3D* MyRhs, int ref_e, int axe, FUNC_4D phi);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X , AXEe_Y or AXEe_Z
* \param phi     : the "C" function ( 4 arguments (x,y,z,tps) )
*
* Set into the structure a function
*/

/*! \fn Rhs3D_setLUAFunction   ( RHS_3D* MyRhs, int ref_e, int axe, const char* def);
*
* \param MyRhs   : the RHS structure 
* \param ref_e   : 0
* \param axe     : AXEe_X , AXEe_Y or AXEe_Z
* \param def     : the "C" function  as string( 3 arguments (x,y,z) )
*
* Set into the structure a function
*/

/*! \fn Rhs3D_setLUAFunctionTransient   ( RHS_3D* MyRhs, int ref_e, int axe, const char* def);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X , AXEe_Y or AXEe_Z
* \param def     : the "C" function as string ( 4 arguments (x,y,z,tps) )
*
* Set into the structure a function
*/

/*! \fn Rhs3D_setTps          ( RHS_3D* MyRhs, Real tps);
*
* \param MyRhs : the RHS structure
* \param tps   : the "current" time
*
* Only useful when a "transient" function has been set into the structure
*/

/*! \fn Rhs3D_setCurrentSelectedAxe  ( RHS_3D* MyRhs, int axe  );
*
* \param MyRhs : the RHS structure
* \param axe   : the "current" axe : AXEe_X , AXEe_Y or AXEe_Z
*
* In the function "assemblage3D_vector_fun", the function used to build the rhs will be the one given with the "current selected axe"
*/

/*! \fn Rhs3D_evalFunction          ( const RHS_3D* MyRhs, int ref_e, int axe, Real x, Real y, Real z);
*
* \param MyRhs : the RHS structure
* \param ref_e : 0
* \param axe   : AXEe_X , AXEe_Y or AXEe_Z
* \param x     : the coordinate where to evaluate the function
* \param y     : the coordinate where to evaluate the function
* \param z     : the coordinate where to evaluate the function
*
* For transient function, set first the current time into the structure
*/

