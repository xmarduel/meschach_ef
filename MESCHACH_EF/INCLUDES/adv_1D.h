#ifndef MEF_ADV_1D_H
#define MEF_ADV_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file adv_1D.h
 *
 * Contains the structure of ADV in 1 dimension
 *
 * The structure contains an array of functions. These are the functions s.t.
 *
 *
 * for stationnary problems :
 *
 *    - Uxx + Adv111(x).Ux   = f(x)      ( with f a RHS_1D structure )
 *
 *
 * for transient problems :
 *
 *    Ut - Uxx + Adv111(x,t).Ux = f(x,t)    ( with f a RHS_1D structure )
 *
 *
 * For an homogeneous material, all the cells of the geometry have the same reference number (usually 0)
 * so that the structure can be set with
 *
 * MyAdv = Adv1D_get()
 * MyAdv = Adv1D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, the_c_function) 
 *
 * In a Python program : 
 *
 * MyAdv = Adv1D_get()
 * MyAdv = Adv1D_setPythonFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, the_python_function)
 *
 * ------------------------------------------------------------------------*/

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
	
#define  NBMAX_ADV_1D_FUNCTIONS   11

/*------------------------------------------------------------------------- */

typedef struct {

   FUN_1D Fun[1][1][NBMAX_ADV_1D_FUNCTIONS];   /**< function for the equation */

   Real tps;                                   /**< the "current" time */

} ADV_1D ;


#define ADV_1D_NULL  ((ADV_1D *)NULL)

#define ADV_1D_FREE(MyAdv)       ( Adv1D_free(MyAdv),       (MyAdv)=(ADV_1D *)NULL )

ADV_1D* Adv1D_get(void);
ADV_1D* Adv1D_setup_from_params(const PARAMS *params);

int     Adv1D_free(ADV_1D  *MyAdv);

ADV_1D* Adv1D_setFunction          ( ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata);

ADV_1D* Adv1D_setCFunction         ( ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_1D phi);
ADV_1D* Adv1D_setCFunctionTransient( ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_2D phi);

ADV_1D* Adv1D_setTps               ( ADV_1D* MyAdv, Real tps);

Real    Adv1D_evalFunction         ( const ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, Real x);

#ifdef __cplusplus
}
#endif

#endif

/*! \struct ADV_1D
* \brief The structure for the ADVection functions in 1-D
*
* The ADV_1D structure contains the pointers to the functions defined in the domain\n
* as well as the time (for transient problems)
*/


/*! \fn Adv1D_get          ( void );
* \brief Create a new structure of type ADV_1D
*
* Create an uninitialized structure (to be filled later)
*/

/*! \fn Adv1D_free          (ADV_1D  *MyAdv);
* \brief Release the structure MyAdv
* \param MyAdv   : the ADV_1D structure
*
* Release the structure MyAdv
*/

/*! \fn Adv1D_setFunction   ( ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata);
* \brief Set a function in the structure MyAdv for zones of reference number ref_e for axes axe1, axe2 and axe3
* \param MyAdv   : the ADV_1D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X only
* \param axe2    : AXEe_X only
* \param axe3    : AXEe_X only
* \param type    : the type of the function we actually store ( "C" or "Python" function)
* \param phi     : a "C" function
* \param clientdata  : NULL for a "C" function, a pointer to the "Python" function in the other case
*
* This is a "private" function : do not used this function - It should be actually declared "static",
* but we need it in the python extension module -
*/

/*! \fn Adv1D_setCFunction   ( ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_1D phi);
* \brief Set a "C" function in the structure MyAdv for zones of reference number ref_e for axes axe1, axe2 and axe3
* \param MyAdv   : the ADV_1D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X only
* \param axe2    : AXEe_X only
* \param axe3    : AXEe_X only
* \param phi     : the "C" function ( 1 argument (x) )
*
* Set into the structure a function
*/

/*! \fn Adv1D_setCFunctionTransient   ( ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_2D phi);
* \brief Set a "C" function in the structure MyAdv for zones of reference number ref_e for axes axe1, axe2 and axe3
* \param MyAdv   : the ADV_1D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X only
* \param axe2    : AXEe_X only
* \param axe3    : AXEe_X only
* \param phi     : the "C" function ( 2 arguments (x,tps) )
*
* Set into the structure a function
*/

/*! \fn Adv1D_setTps          ( ADV_1D* MyAdv, Real tps);
* \brief Set the current time tps in the structure MyAdv (for transient problems)
* \param MyAdv : the ADV_1D structure
* \param tps   : the "current" time
*
* Only useful when a "transient" function has been set into the structure
*/


/*! \fn Adv1D_evalFunction          ( const ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, Real x);
* \brief Evaluate at "x" the function stored in the structure MyAdv for zones of reference ref_e at axes axe1, axe2 and axe3
* \param MyAdv  : the ADV_1D structure
* \param ref_e  : 0
* \param axe1   : AXEe_X only
* \param axe2   : AXEe_X only
* \param axe3   : AXEe_X only
* \param x      : the coordinate where to evaluate the function
*
* For transient function, set first the current time into the structure
*/

