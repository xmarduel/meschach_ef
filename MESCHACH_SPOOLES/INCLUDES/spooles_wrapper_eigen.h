#ifndef SPOOLES_WRAPPER_EIGEN__H
#define SPOOLES_WRAPPER_EIGEN__H

#ifdef __cplusplus
extern "C"
{
#endif


/** \file spooles_wrapper_eigen.h
 *
 * Contains the routines for eigenvalues problems
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"


typedef enum {

   EIGENe_SPOOLES_ORDINARY_SYMMETRIC        =  0,
   EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_1   =  1,
   EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_2   =  2

} EIGENt_SPOOLES_PROBLEM ;

typedef enum {

   EIGENe_SPOOLES_LOWER     =  0,
   EIGENe_SPOOLES_UPPER     =  1,
   EIGENe_SPOOLES_NEAREST_1 =  2,
   EIGENe_SPOOLES_NEAREST_2 =  3,
   EIGENe_SPOOLES_IBOUNDS   =  4,
   EIGENe_SPOOLES_RBOUNDS   =  5,
   EIGENe_SPOOLES_ALL       =  6

} EIGENt_SPOOLES_OPTION ;



MAT* sp_eigen_spooles(const SPMAT *A, const SPMAT *B, VEC *eigen_values, EIGENt_SPOOLES_PROBLEM type, EIGENt_SPOOLES_OPTION flag );

#ifdef __cplusplus
}
#endif
      
#endif

/*! \enum EIGENt_SPOOLES_PROBLEM
* \brief
*
* list the different options for getting eigenvalues and eigenvectors
*/

/*! \enum EIGENt_SPOOLES_OPTION
* \brief
*
* list the different types of eigenvalues problems
*/

/*! \fn sp_eigen_spooles (const SPMAT *A, const SPMAT *B, VEC *eigen_values, EIGENt_SPOOLES_PROBLEM type, EIGENt_SPOOLES_OPTION flag );
* \brief
* \param A    : input : the sparse matrix
* \param B    : input : the sparse matrix
* \param eigen_values : output : the eigenvalues
* \param type : 
* \param flag : 
*
* Solve an "generalized" eigenvalues problem Ax = k.B.x. \n
* Return the array of the computed eigen-vectors ( the size depends on the problem's flag\n
*/

