#ifndef CONTROL_H
#define CONTROL_H

#include "MESCHACH/INCLUDES/matrix.h"

#ifdef __cplusplus
extern "C" {
#endif
   

/** \file control.h
 *
 * Contains the functions to solve control problems
 *
 * ------------------------------------------------------------------------ */

typedef enum
{
   CONTROLe_SLICOT = 1,
   CONTROLe_SCILAB = 2

} CONTROLt_METHOD ;

void set_control_method( int method );
int  get_control_method();

MAT *riccati  (const MAT *A, const MAT *B, const MAT *C, MAT *X);
MAT *lyapunov (const MAT *A, const MAT *C,               MAT *X);
MAT *sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X);



#ifdef __cplusplus
}
#endif

#endif


/*! \enum CONTROLt_METHOD
* \brief List the different types of option for performing control routines
*/

/*! \fn void set_control_method( int method );
* \brief Set the global variable to indicate which control algorithm will be used
* \param method : a value in CONTROLt_METHOD enumeration \n
*
* You can use the library "SCILAB" or the library "SLICOT" to perform the control routines \n
* (ricatti, lyapunov or sylvester)
*/

/*! \fn int get_control_method();
* \brief Get the global variable which indicates which contrl algorithm is used
*
* Retrieve the method (see CONTROLt_METHOD enumeration) to perform the control routines
*/

/*! \fn MAT * riccati(const MAT *A, const MAT *B, const MAT *C, MAT *X);
* \brief Ricatti routine
* \param A :
* \param B :
* \param C :
* \param X :
*
* calculates X solution of A'*X + X*A + C - X*B*X = 0 
*/

/*! \fn MAT * lyapunov(const MAT *A, const MAT *C, MAT *X);
* \brief Lyapunov routine
* \param A :
* \param C :
* \param X :
*
* calculates X solution of A'*X + X*A = C
*/

/*! \fn MAT * sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X);
* \brief Sylvester routine
* \param A :
* \param B :
* \param C : 
* \param X :  
*
* calculates X solution of A*X + X*B = C
*/
