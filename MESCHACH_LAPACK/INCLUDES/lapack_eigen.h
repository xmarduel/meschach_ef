#ifndef MESCHACH_LAPACK_EIGEN__H
#define MESCHACH_LAPACK_EIGEN__H

#ifdef __cplusplus
extern "C"
{
#endif


/** \file lapack_eigen.h
 *
 * Contains the routines for calculating eigenvalues and eigenvectors of matrices \n
 *
 * There are two types of problems  :\n
 *  "ordinary" eigenvalues problems: Ax = k.x \n
 *  "generalized" eigenvalues problems: Ax = k.Bx \n
 *
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"


typedef enum {

   EIGENe_LAPACK_LOWER     =  0, /**< calculate only the "nb_eig" lower eigenvalues and eigenvectors \n*/
   EIGENe_LAPACK_UPPER     =  1, /**< calculate only the "nb_eig" upper eigenvalues and eigenvectors \n*/
   EIGENe_LAPACK_IBOUNDS   =  2, /**< calculate eigenvalues in the indices interval [i0 - j0] and related eigenvectors \n*/
   EIGENe_LAPACK_RBOUNDS   =  3, /**< calculate eigenvalues in the values  interval [a0 - b0] and related eigenvectors \n*/
   EIGENe_LAPACK_ALL       =  4  /**< calculate all the eigenvalues and eigenvectors \n*/

} EIGENt_LAPACK_OPTION ;


typedef enum {

   EIGENe_LAPACK_ORDINARY      =  0, /**< for an "ordinary" eigenvalues problem \n*/
   EIGENe_LAPACK_GENERALIZED   =  1  /**< for an "generalized" eigenvalues problem \n*/

} EIGENt_LAPACK_PROBLEM ;


/* eigenvalues problem - get back ALL eigen values and vectors */
MAT *sp_eigen_lapack_ordinary_full   (const SPMAT *A,                 VEC *eig_values );
MAT *sp_eigen_lapack_generalized_full(const SPMAT *A, const SPMAT *B, VEC *eig_values );

/* eigenvalues problem - get back A FEW eigen values and vectors */
MAT *sp_eigen_lapack_ordinary_expert_lower  (const SPMAT *A, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_ordinary_expert_upper  (const SPMAT *A, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_ordinary_expert_ibounds(const SPMAT *A, VEC *eig_values, int nb_eig , int    lower_ibound, int    upper_ibound );
MAT *sp_eigen_lapack_ordinary_expert_rbounds(const SPMAT *A, VEC *eig_values, int nb_eig , double lower_rbound, double upper_rbound );

MAT *sp_eigen_lapack_generalized_expert_lower  (const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_generalized_expert_upper  (const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_generalized_expert_ibounds(const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig , int    lower_ibound, int    upper_ibound );
MAT *sp_eigen_lapack_generalized_expert_rbounds(const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig , double lower_rbound, double upper_rbound );

/* DRIVER PROGRAM - SOME OF THE ARGS WILL BE NOT USED */
MAT* sp_eigen_lapack(const SPMAT *A, const SPMAT *B, VEC *eig_values, EIGENt_LAPACK_PROBLEM type, EIGENt_LAPACK_OPTION flag,
                     int    nb_eig,
                     int    lower_ibound, int    upper_ibound ,
                     double lower_rbound, double upper_rbound );


#ifdef __cplusplus
}
#endif


#endif

/*! \enum EIGENt_LAPACK_OPTION
* \brief
*
* list the different options for getting eigenvalues and eigenvectors
*/

/*! \enum EIGENt_LAPACK_PROBLEM
* \brief
*
* list the different types of eigenvalues problems
*/

/*! \fn sp_eigen_lapack_ordinary_full          (const SPMAT *A,           VEC *eig_values )
* \brief
* \param A    : input : the sparse matrix
* \param eig_values    : output : the eigenvalues
*
* Solve an "ordinary" eigenvalues problem Ax = k.x. All values and vectors are calculated.\n
* Return the array of the computed eigen-vectors\n
*/

/*! \fn sp_eigen_lapack_generalized_full         (const SPMAT *A, const SPMAT *B, VEC *eig_values )
* \brief
* \param A    : input : the sparse matrix
* \param B    : input : the sparse matrix
* \param eig_values    : output : the eigenvalues
*
* Solve an "generalized" eigenvalues problem Ax = k.Bx. All values and vectors are calculated\n
* Return the array of the computed eigen-vectors\n
*/
