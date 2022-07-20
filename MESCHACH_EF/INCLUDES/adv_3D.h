#ifndef MEF_ADV_3D_H
#define MEF_ADV_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file adv_3D.h
 *
 * Contains the structure of ADV in 3 dimensions
 *
 * The structure contains an array of functions. These are the functions s.t.
 *
 * a) for a 1-unknown problem :
 *
 *       - Uxx - Uyy - Uzz + Adv11(x,y,z).Ux + Adv12(x,y,z).Uy + Adv31(x,y,z)Uz  = f1(x,y,z)      ( with f a RHS_3D structure )
 *
 * b) for a 2-unknowns problem : ( actually not implemented )
 *
 *       - Uxx - Uyy - Uzz + Adv111.Ux + Adv112.Uy + Adv113.Uz + Adv121.Vx + Adv122.Vy + Adv123.Vz = f1(x,y,z)
 *       - Vxx - Vyy - Vzz + Adv211.Ux + Adv212.Uy + Adv213.Uz + Adv221.Vx + Adv222.Vy + Adv223.Vz = f2(x,y,z)
 *
 * b) for a 3-unknowns problem : ( actually not implemented )
 *
 *       - Uxx - Uyy - Uzz + Adv111.Ux + Adv112.Uy + Adv113.Uz + Adv121.Vx + Adv122.Vy + Adv123.Vz + Adv131.Wx + Adv132.Wy + Adv133.Wz = f1
 *       - Vxx - Vyy - Vzz + Adv211.Ux + Adv212.Uy + Adv213.Uz + Adv221.Vx + Adv222.Vy + Adv223.Vz + Adv231.Wx + Adv232.Wy + Adv233.Wz = f2
 *       - Wxx - Wyy - Wzz + Adv311.Ux + Adv312.Uy + Adv313.Uz + Adv321.Vx + Adv322.Vy + Adv323.Vz + Adv331.Wx + Adv332.Wy + Adv333.Wz = f2
 *
 *
 *
 * or for transient problems :
 *
 * a) for a 1-unknown problem :
 *
 *        Ut - Uxx - Uyy - Uzz + Adv11(x,y,z,t).Ux + Adv12(x,y,z,t).Uy + Adv13(x,y,z,t).Uy   = f1(x,y,z,t)
 *
 *
 *
 * For an homogeneous material, all the cells of the geometry have the same reference number (usually 0)
 * so that the structure can be set with \n
 *
 * MyAdv = Adv3D_get() \n
 *
 * a) for a 1-unknown problem :
 *
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, Adv11) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, Adv12) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z, Adv13) \n
 *
 *
 * b) for a 2-unknowns problem :
 *
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, Adv111) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, Adv112) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z, Adv113) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_X, Adv121) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_Y, Adv122) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_Z, Adv123) \n
 *
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_X, Adv211) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_Y, Adv212) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_Z, Adv213) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_X, Adv221) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Y, Adv222) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Z, Adv223) \n
 *
 *
 * c) for a 3-unknowns problem :
 *
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_X, Adv111) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Y, Adv112) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_X, AXEe_Z, Adv113) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_X, Adv121) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_Y, Adv122) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Y, AXEe_Z, Adv123) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Z, AXEe_X, Adv131) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Z, AXEe_Y, Adv132) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_X, AXEe_Z, AXEe_Z, Adv133) \n
 *
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_X, Adv211) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_Y, Adv212) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_X, AXEe_Z, Adv213) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_X, Adv221) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Y, Adv222) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Y, AXEe_Z, Adv223) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Z, AXEe_X, Adv231) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Z, AXEe_Y, Adv232) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Y, AXEe_Z, AXEe_Z, Adv233) \n
 *
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_X, AXEe_X, Adv311) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_X, AXEe_Y, Adv312) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_X, AXEe_Z, Adv313) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_Y, AXEe_X, Adv321) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_Y, AXEe_Y, Adv322) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_Y, AXEe_Z, Adv323) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_Z, AXEe_X, Adv331) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_Z, AXEe_Y, Adv332) \n
 * MyAdv = Adv3D_setCFunction( MyAdv, 0, AXEe_Z, AXEe_Z, AXEe_Z, Adv333) \n
 *
 *
 * --------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#define  NBMAX_ADV_3D_FUNCTIONS   11

/*---------------------------------------------------------------------- */

typedef struct {

   FUN_3D Fun1[3][3][NBMAX_ADV_3D_FUNCTIONS];  /**< functions for the "first" equation  */
   FUN_3D Fun2[3][3][NBMAX_ADV_3D_FUNCTIONS];  /**< functions for the "second" equation */
   FUN_3D Fun3[3][3][NBMAX_ADV_3D_FUNCTIONS];  /**< functions for the "third" equation  */

   Real tps;                                   /**< the "current" time */

   int  current_selected_axe1;                  /**< references the equation (Fun1, Fun2, Fun3) */
   int  current_selected_axe2;                  /**< references the first index of Fun_n[][]    */
   int  current_selected_axe3;                  /**< references the sec   index of Fun_n[][]    */

} ADV_3D ;


#define ADV_3D_NULL  ((ADV_3D *)NULL)

#define ADV_3D_FREE(MyAdv) ( Adv3D_free(MyAdv), (MyAdv)=(ADV_3D *)NULL )


ADV_3D* Adv3D_get(void);
ADV_3D* Adv3D_setup_from_params(const PARAMS *params);

int     Adv3D_free(ADV_3D  *MyAdv);

ADV_3D* Adv3D_setFunction          ( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata);

ADV_3D* Adv3D_setCFunction         ( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_3D phi);
ADV_3D* Adv3D_setCFunctionTransient( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_4D phi);

ADV_3D* Adv3D_setLUAFunction         ( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, const char* def);
ADV_3D* Adv3D_setLUAFunctionTransient( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, const char* def);

ADV_3D* Adv3D_setCurrentSelectedAxe( ADV_3D* MyAdv, int axe1, int axe2, int axe3 );
ADV_3D* Adv3D_setTps               ( ADV_3D* MyAdv, Real tps);

Real    Adv3D_evalFunction         ( const ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, Real x, Real y, Real z);

#ifdef __cplusplus
}
#endif

#endif

/*! \struct ADV_3D
* \brief the structure for the ADVection functions in 3-D
*
* The ADV_3D structure contains the pointers to the functions defined in the domain\n
* as well as the time (for transient problems)
*/

/*! \fn Adv3D_setup_from_params ( const PARAMS *params );
* \brief Create a new structure of type ADV_3D
*
* Create an structure filled from the params specifications.
*/

/*! \fn Adv3D_get          ( void );
* \brief Create a new structure of type ADV_3D
*
* Create an uninitialized structure (to be filled later)
*/

/*! \fn Adv3D_free          (ADV_3D  *MyAdv);
* \brief Release the structure MyAdv
* \param MyAdv     : the ADV structure
*
* Release the structure MyAdv
*/

/*! \fn Adv3D_setFunction   ( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUN_TYPE type, void* phi, void* clientdata);
* \brief Set a function in the structure MyAdv for zone of reference number ref_e for axes axe1, axe2 , axe3
* \param MyAdv   : the ADV_3D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X , AXEe_Y or AXEe_Z
* \param axe2    : AXEe_X , AXEe_Y or AXEe_Z
* \param axe3    : AXEe_X , AXEe_Y or AXEe_Z
* \param type    : the type of the function we actually store ( "C" or "Python" function)
* \param phi     : a "C" function
* \param clientdata  : NULL for a "C" function, a pointer to the "Python" function in the other case
*
* This is a "private" function : do not used this function - It should be actually declared "static", but we need it in the python extension module -
*/

/*! \fn Adv3D_setCFunction   ( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_3D phi);
* \brief Set a "C" function in the structure MyAdv for zone of reference number ref_e for axes axe1, axe2 , axe3
* \param MyAdv   : the ADV_3D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X , AXEe_Y or AXEe_Z
* \param axe2    : AXEe_X , AXEe_Y or AXEe_Z
* \param axe3    : AXEe_X , AXEe_Y or AXEe_Z
* \param phi     : the "C" function ( 3 arguments (x,y,z) )
*
* Set into the structure a function
*/

/*! \fn Adv3D_setCFunctionTransient   ( ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, FUNC_4D phi);
* \brief Set a function in the structure MyAdv for zone of reference number ref_e for axes axe1, axe2 , axe3
* \param MyAdv   : the ADV_3D structure
* \param ref_e   : 0
* \param axe1    : AXEe_X , AXEe_Y or AXEe_Z
* \param axe2    : AXEe_X , AXEe_Y or AXEe_Z
* \param axe3    : AXEe_X , AXEe_Y or AXEe_Z
* \param phi     : the "C" function ( 4 arguments (x,y,z,tps) )
*
* Set into the structure a function
*/

/*! \fn Adv3D_setTps          ( ADV_3D* MyAdv, Real tps);
* \brief Set in the structure MyAdv the "current" time tps - for transient problems -
* \param MyAdv : the ADV_3D structure
* \param tps   : the "current" time
*
* Only useful when a "transient" function has been set into the structure
*/

/*! \fn Adv3D_setCurrentSelectedAxe ( ADV_3D* MyAdv, int axe1, int axe2, int axe3 );
* \brief Set in the structure MyAdv the "current axes axe1, axe2 , axe3
* \param MyAdv : the ADV_3D structure
* \param axe1  : AXEe_X , AXEe_Y or AXEe_Z - for the first, second or the third equation (Fun1, Fun2 or Fun3)
* \param axe2  : AXEe_X , AXEe_Y or AXEe_Z - in Adv_ijk_, select the index "j"
* \param axe3  : AXEe_X , AXEe_Y or AXEe_Z - in Adv_ijk_, select the index "k"
*
* In the function "_systel_matrix_Conv_G_fun", the function used to build the matrix will be the one given with the "current selected axe(s)"
*/

/*! \fn Adv3D_evalFunction          ( const ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, Real x, Real y, Real z);
* \brief Evaluate the function stored in the structure MyAdv for  zone of reference number ref_e for axes axe1, axe2 , axe3 at coordinates x, y, z
* \param MyAdv  : the ADV_3D structure
* \param ref_e  : 0
* \param axe1   : AXEe_X , AXEe_Y or AXEe_Z
* \param axe2   : AXEe_X , AXEe_Y or AXEe_Z
* \param axe3   : AXEe_X , AXEe_Y or AXEe_Z
* \param x      : the coordinate where to evaluate the function
* \param y      : the coordinate where to evaluate the function
* \param z      : the coordinate where to evaluate the function
*
* For transient function, set first the current time into the structure
*/

