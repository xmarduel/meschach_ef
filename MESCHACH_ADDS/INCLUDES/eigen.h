#ifndef EIGEN_PLUS_H
#define EIGEN_PLUS_H

/** \file eigen.h
 *
 * Contains the functions to solve eigenvalues problems
 *
 * ------------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"


#ifdef __cplusplus
extern "C"
{
#endif


/* enumeration : indicates which library we use to solve the eigenvalue problem */
typedef enum {

   EIGENe_MESCHACH =  1,
   EIGENe_SPOOLES  =  2,
   EIGENe_LAPACK   =  3

} EIGENt_METHOD ;


/* enumeration : the type of eigenvalues we solve */
typedef enum {

   EIGENe_ORDINARY_SYMMETRIC        =  0,
   EIGENe_GENERALIZED_SYMMETRIC_1   =  1,
   EIGENe_GENERALIZED_SYMMETRIC_2   =  2

} EIGENt_PROBLEM ;


/* enumeration : set the options for expert eigenvalues problem solver */
typedef enum {

   EIGENe_LOWER     =  0,
   EIGENe_UPPER     =  1,
   EIGENe_NEAREST_1 =  2,
   EIGENe_NEAREST_2 =  3,
   EIGENe_IBOUNDS   =  4,
   EIGENe_RBOUNDS   =  5,   
   EIGENe_ALL       =  6

} EIGENt_OPTION ;


/* a struct to hold all parameters when finding eigenvalues and vectors for an "expert" solver */
typedef struct {

   EIGENt_PROBLEM problem;
   EIGENt_OPTION  option;
   
   int lower_ibound;
   int upper_ibound;

   double lower_rbound;
   double upper_rbound;

   int    nb_eig;
   
} EIGENt_PARAMS;


/* functions to set and fill-in the structure EIGENt_PARAMS */
EIGENt_PARAMS *eig_params_get(void);
int eig_params_set_problem(EIGENt_PARAMS *eig_param, EIGENt_PROBLEM problem);
int eig_params_set_option (EIGENt_PARAMS *eig_param, EIGENt_OPTION option);
int eig_params_set_ibounds(EIGENt_PARAMS *eig_param, int lower_bound, int upper_bound);
int eig_params_set_rbounds(EIGENt_PARAMS *eig_param, double lower_bound, double upper_bound);
int eig_params_set_nb_eigv(EIGENt_PARAMS *eig_param, int nb_eigv);
int eig_params_free(EIGENt_PARAMS *eig_param);


/* driver functions to solve the eigenvalue problem */
MAT  *eigen_ordinary(const MAT *A, VEC *eigen_values);

MAT  *sp_eigen_meschach_ordinary   (const SPMAT *A,                 VEC *eigen_values);
MAT  *sp_eigen_meschach_generalized(const SPMAT *A, const SPMAT *B, VEC *eigen_values);

/* the true driver function which should be used */
MAT  *sp_eigen(const SPMAT *A, const SPMAT *B, VEC *eigen_values, EIGENt_METHOD method, EIGENt_PARAMS *eig_params);

#ifdef __cplusplus
}
#endif


#endif

/*! \enum EIGENt_METHOD
* \brief List the different types of library to solve the eigen values problem
*/

/*! \enum EIGENt_PROBLEM
* \brief List the different types of problems for an eigenvalues problem (depends on the "method")
*/

/*! \enum EIGENt_OPTION
* \brief List the different types of options when solving the eigenvalues problem
*/

/*! \struct EIGENt_PARAMS
* \brief The structure containing all informations for an eigenvalues problem
*/


/*! \fn sp_eigen(const SPMAT *A, const SPMAT *B, VEC *eigen_values, EIGENt_METHOD method, EIGENt_PARAMS *eig_params);
* \brief Solve the eigenvalues problem A.x = k.B.x
*
* \param A :
* \param B :
* \param eigen_values  : the eigenvalues (In-Out)
* \param method  :
* \param eig_params :
*
* returns the eigenvectors
*/

