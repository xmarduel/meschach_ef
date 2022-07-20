#ifndef MEF_RHS_1D_H
#define MEF_RHS_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file rhs_1D.h
 *
 * Contains the structure of rhs in 1 dimension
 *
 * The structure contains an array of functions.\n
 * For an homogeneous material, all the cells of the geometry have the same reference number (usually 0)
 * so that the structure can be set with \n
 *
 * MyRhs = Rhs1D_get() \n
 * MyRhs = Rhs1D_setCFunction( MyRhs, 0, AXEe_X, the_c_function) # 0 is the reference of the geometry's element \n
 *
 * MyRhs = Rhs1D_get() \n
 * MyRhs = Rhs1D_setLUAFunction( MyRhs, 0, AXEe_X, "2 + x + math.sin(x)") # 0 is the reference "0" of the geometry's element \n
 *
 * In a Python program :  \n
 *
 * MyRhs = Rhs1D_get() \n
 * MyRhs = Rhs1D_setPythonFunction( MyRhs, 0, AXEe_X, the_python_function) # 0 is the reference "0" of the geometry's element \n
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#define  NBMAX_RHS_1D_FUNCTIONS   11

/*------------------------------------------------------------------------- */

typedef struct {

   FUN_1D Fun[1][NBMAX_RHS_1D_FUNCTIONS];    /**< nb functions on the domain */

   Real tps;                                 /**< the "current working axe" */

} RHS_1D ;


#define RHS_1D_NULL  ((RHS_1D *)NULL)

#define RHS_1D_FREE(MyRHS)       ( Rhs1D_free(MyRHS),       (MyRHS)=(RHS_1D *)NULL )

RHS_1D* Rhs1D_get(void);
RHS_1D* Rhs1D_setup_from_params(const PARAMS* params);

int     Rhs1D_free(RHS_1D *MyRhs);

RHS_1D* Rhs1D_setFunction          ( RHS_1D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata);

RHS_1D* Rhs1D_setCFunction         ( RHS_1D* MyRhs, int ref_e, int axe, FUNC_1D phi);
RHS_1D* Rhs1D_setCFunctionTransient( RHS_1D* MyRhs, int ref_e, int axe, FUNC_2D phi);

RHS_1D* Rhs1D_setLUAFunction         ( RHS_1D* MyRhs, int ref_e, int axe, const char *def);
RHS_1D* Rhs1D_setLUAFunctionTransient( RHS_1D* MyRhs, int ref_e, int axe, const char *def);


RHS_1D* Rhs1D_setTps               ( RHS_1D* MyRhs, Real tps);

Real    Rhs1D_evalFunction         ( const RHS_1D* MyRhs, int ref_e, int axe, Real x);


#ifdef __cplusplus
}
#endif

#endif

/*! \struct RHS_1D
* \brief the structure for the RHS in 1-D
*
* The RHS structure contains the pointers to the functions defined in the domain\n
* as well as the time (for transient problems)
*/


/*! \fn Rhs1D_get          ( void );
* 
* Create an uninitialized structure RHS_1D (to be filled later)
*/

/*! \fn Rhs1D_free          (RHS_1D  *MyRhs);
*
* \param MyRhs   : the RHS structure
*
* Release a RHS_1D structure
*/

/*! \fn Rhs1D_setFunction   ( RHS_1D* MyRhs, int ref_e, int axe, FUN_TYPE type, void* phi, void* clientdata);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X
* \param type    : the type of the function we actually store ( "C" or "Python" function)
* \param phi     : a "C" function
* \param clientdata  : NULL for a "C" function, a pointer to the "Python" function in the other case
*
* Do not used this function - It should be actually declared "static",
* but we need it in the python extension module -
*/

/*! \fn Rhs1D_setCFunction   ( RHS_1D* MyRhs, int ref_e, int axe, FUNC_1D phi);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X
* \param phi     : the "C" function ( 1 argument (x) )
*
* Set into the structure a function
*/

/*! \fn Rhs1D_setCFunctionTransient   ( RHS_1D* MyRhs, int ref_e, int axe, FUNC_2D phi);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X
* \param phi     : the "C" function ( 2 arguments (x,tps) )
*
* Set into the structure a function
*/

/*! \fn Rhs1D_setLUAFunction   ( RHS_1D* MyRhs, int ref_e, int axe, const char* def);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X
* \param def     : the "C" function as string ( 1 argument (x) )
*
* Set into the structure a function
*/

/*! \fn Rhs1D_setLUAFunctionTransient   ( RHS_1D* MyRhs, int ref_e, int axe, const char* def);
*
* \param MyRhs   : the RHS structure
* \param ref_e   : 0
* \param axe     : AXEe_X
* \param def     : the "C" function as string( 2 arguments (x,tps) )
*
* Set into the structure a function
*/

/*! \fn Rhs1D_setTps          ( RHS_1D* MyRhs, Real tps);
*
* \param MyRhs : the RHS structure
* \param tps   : the "current" time
*
* Only useful when a "transient" function has been set into the structure
*/


/*! \fn Rhs1D_evalFunction     ( const RHS_1D* MyRhs, int ref_e, int axe, Real x);
*
* \param MyRhs : the RHS structure
* \param ref_e :
* \param axe   : AXEe_X only
* \param x     : the coordinate where to evaluate the function
*
* For transient function, set first the current time into the structure
*/

