#ifndef MEF_ADV_2D_H
#define MEF_ADV_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file adv_2D.h
 *
 * Contains the structure of ADV in 2 dimensions
 *
 * The structure contains an array of functions. These are the functions s.t.
 *
 * a) for a 1-unknown problem :
 *
 *      - Uxx - Uyy + Adv11(x,y).Ux + Adv12(x,y).Uy   = f1(x,y)      ( with f a RHS_2D structure ) 
 *
 * b) for a 2-unknowns problem : ( actually not implemented )
 *
 *      - Uxx - Uyy + Adv111(x,y).Ux + Adv112(x,y).Uy + Adv121(x,y).Vx + Adv122(x,y).Vy  = f1(x,y) 
 *      - Vxx - Vyy + Adv211(x,y).Ux + Adv212(x,y).Uy + Adv221(x,y).Vx + Adv222(x,y).Vy  = f2(x,y) 
 *
 *
 * or for transient problems :
 *
 * a) for a 1-unknown problem :
 *
 *       Ut - Uxx - Uyy + Adv11(x,y,t).Ux + Adv12(x,y,t).Uy   = f1(x,y,t)      ( with f a RHS_2D structure )
 *
 * b) for a 2-unknowns problem : ( actually not implemented )
 *
 *       Ut - Uxx - Uyy + Adv111.Ux + Adv112.Uy + Adv121.Vx + Adv122.Vy  = f1 
 *       Vt - Vxx - Vyy + Adv211.Ux + Adv212.Uy + Adv221.Vx + Adv222.Vy  = f2 
 *
 *
 * For an homogeneous material, all the cells of the geometry have the same reference number (usually 0)
 * so that the structure can be set with \n
 *
 * MyAdv = Adv2D_get() \n
 *
 * a) for a 1-unknown problem :
 *
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, Adv11)  \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, Adv12)  \n
 *
 * b) for a 2-unknowns problem :
 *
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, Adv111) \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, Adv112) \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_X, Adv121) \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_Y, Adv122) \n
 *
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_X, Adv211) \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_Y, Adv212) \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_X, Adv221) \n
 * MyAdv = Adv2D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Y, Adv222) \n
 *
 *
 * --------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
	
#define  NBMAX_ADV_2D_FUNCTIONS   11

/*---------------------------------------------------------------------- */


typedef struct {
  
   FUN_2D Fun1[2][2][NBMAX_ADV_2D_FUNCTIONS];   /**< functions for the "first" equation   */
   FUN_2D Fun2[2][2][NBMAX_ADV_2D_FUNCTIONS];   /**< functions for the "second" equation  */

   Real tps;                                    /**< the "current" time */

   int  current_selected_axe1;                   /**< references the equation (Fun1 or Fun2)  */
   int  current_selected_axe2;                   /**< references the first index of Fun_n[][] */
   int  current_selected_axe3;                   /**< references the sec   index of Fun_n[][] */
   
} ADV_2D ;


#define ADV_2D_NULL  ((ADV_2D *)NULL)

#define ADV_2D_FREE(MyAdv) ( Adv2D_free(MyAdv), (MyAdv)=(ADV_2D *)NULL )



ADV_2D* Adv2D_get(void);
ADV_2D* Adv2D_setup_from_params(const PARAMS *params);
	
int     Adv2D_free(ADV_2D *MyAdv);

ADV_2D* Adv2D_setFunction          ( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata);

ADV_2D* Adv2D_setCFunction         ( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_2D phi);
ADV_2D* Adv2D_setCFunctionTransient( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_3D phi);
   
ADV_2D* Adv2D_setLUAFunction         ( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, const char *def);
ADV_2D* Adv2D_setLUAFunctionTransient( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, const char *def);

ADV_2D* Adv2D_setCurrentSelectedAxe( ADV_2D* MyAdv, int axe1, int axe2, int axe3 );
ADV_2D* Adv2D_setTps               ( ADV_2D* MyAdv, Real tps);

Real    Adv2D_evalFunction         ( const ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, Real x, Real y);

#ifdef __cplusplus
}
#endif

#endif


/*! \struct ADV_2D
* \brief the structure for the ADVection functions in 2-D
*
* The ADV_2D structure contains the pointers to the functions defined in the domain\n
* as well as the time (for transient problems)
*/


/*! \fn Adv2D_get          ( void );
* \brief Create a new structure of type ADV_2D
*
* Create an uninitialized structure (to be filled later)
*/

/*! \fn Adv2D_free          (ADV_2D  *MyAdv);
* \brief Release the structure MyAdv
* \param MyAdv   : the ADV_2D structure
*
* Release the structure
*/

/*! \fn Adv2D_setFunction   ( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata);
* \brief Set a function in the structure MyAdv for zone of reference ref_e at axes axe1, axe2 and axe3
* \param MyAdv   : the ADV_2D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X or AXEe_Y
* \param axe2    : AXEe_X or AXEe_Y
* \param axe3    : AXEe_X or AXEe_Y
* \param type    : the type of the function we actually store ( "C" or "Python" function)
* \param phi     : a "C" function
* \param clientdata  : NULL for a "C" function, a pointer to the "Python" function in the other case
*
* This is a "private" function : do not used it - It should be actually declared "static", but we need it in the python extension module -
*/

/*! \fn Adv2D_setCFunction   ( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_2D phi);
* \brief Set a "C" function in the structure MyAdv for zone of reference ref_e at axes axe1, axe2 and axe3
* \param MyAdv   : the ADV_2D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X or AXEe_Y
* \param axe2    : AXEe_X or AXEe_Y
* \param axe3    : AXEe_X or AXEe_Y
* \param phi     : the "C" function ( 2 arguments (x,y) )
*
* Set into the structure a function
*/

/*! \fn Adv2D_setCFunctionTransient   ( ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_3D phi);
* \brief  Set a "C" function in the structure MyAdv for zone of reference ref_e at axes axe1, axe2 and axe3
* \param MyAdv   : the ADV_2D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X or AXEe_Y
* \param axe2    : AXEe_X or AXEe_Y
* \param axe3    : AXEe_X or AXEe_Y
* \param phi     : the "C" function ( 3 arguments (x,y,tps) )
*
* Set into the structure a function
*/

/*! \fn Adv2D_setTps          ( ADV_2D* MyAdv, Real tps);
* \brief Set in MyAdv the current time tps - for transient problems -
* \param MyAdv : the ADV_2D structure
* \param tps   : the "current" time
*
* Only useful when a "transient" function has been set into the structure
*/


/*! \fn Adv2D_setCurrentSelectedAxe ( ADV_2D* MyAdv, int axe1, int axe2, int axe3 );
* \brief Set in the structure MyAdv the "current" axes
* \param MyAdv : the ADV_2D structure
* \param axe1  : AXEe_X or AXEe_Y - for the first or the second equation (Fun1 or Fun2)
* \param axe2  : AXEe_X or AXEe_Y - in Adv_ijk_, select the index "j"
* \param axe3  : AXEe_X or AXEe_Y - in Adv_ijk_, select the index "k"
*
* Some functions utilize the "current" axes for their calculation : for example "_systel_matrix_Conv_G_fun", the function used to build the matrix will be the one given with the "current selected axe(s)"
*/

/*! \fn Adv2D_evalFunction          ( const ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, Real x, Real y);
* \brief Evaluate the function stored in the structure MyAdv at coordinates x and y
* \param MyAdv  : the ADV_2D structure
* \param ref_e  : 0
* \param axe1   : AXEe_X or AXEe_Y
* \param axe2   : AXEe_X or AXEe_Y
* \param axe3   : AXEe_X or AXEe_Y
* \param x      : the coordinate where to evaluate the function
* \param y      : the coordinate where to evaluate the function
*
* For transient function, set first the current time into the structure
*/

