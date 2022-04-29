
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_LAPACK
//
///////////////////////////////////////////////////////// 

#ifndef MESCHACH_LAPACK__H
#define MESCHACH_LAPACK__H

#include "matrix.h"
#include "sparse.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum {

   EIGENe_LAPACK_LOWER     =  0,
   EIGENe_LAPACK_UPPER     =  1,
   EIGENe_LAPACK_IBOUNDS   =  2,
   EIGENe_LAPACK_RBOUNDS   =  3,
   EIGENe_LAPACK_ALL       =  4

} EIGENt_LAPACK_OPTION ;


typedef enum {

   EIGENe_LAPACK_ORDINARY      =  0,
   EIGENe_LAPACK_GENERALIZED   =  1

} EIGENt_LAPACK_PROBLEM ;



VEC* Ax_lapackbanded     (const SPMAT *A, const VEC * v1, VEC *v2);
VEC* Ax_lapacklowerbanded(const SPMAT *A, const VEC * v1, VEC *v2);


BAND *sp2band     (const SPMAT *A );
BAND *sp2lowerband(const SPMAT *A );

void b_foutput(FILE* fp, const BAND* B);

MAT  *spmat2lapackbanded     (const SPMAT *A );
MAT  *spmat2lapacklowerbanded(const SPMAT *A );


/* eigenvalues problem - get back ALL eigen values and vectors */
MAT *sp_eigen_lapack_ordinary_full   (SPMAT *A,           VEC *eig_values );
MAT *sp_eigen_lapack_generalized_full(SPMAT *A, SPMAT *B, VEC *eig_values );

/* eigenvalues problem - get back FEW eigen values and vectors */
MAT *sp_eigen_lapack_ordinary_expert_lower  (SPMAT *A, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_ordinary_expert_upper  (SPMAT *A, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_ordinary_expert_ibounds(SPMAT *A, VEC *eig_values, int nb_eig , int    lower_ibound, int    upper_ibound );
MAT *sp_eigen_lapack_ordinary_expert_rbounds(SPMAT *A, VEC *eig_values, int nb_eig , double lower_rbound, double upper_rbound );

MAT *sp_eigen_lapack_generalized_expert_lower  (SPMAT *A, SPMAT *B, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_generalized_expert_upper  (SPMAT *A, SPMAT *B, VEC *eig_values, int nb_eig );
MAT *sp_eigen_lapack_generalized_expert_ibounds(SPMAT *A, SPMAT *B, VEC *eig_values, int nb_eig , int    lower_ibound, int    upper_ibound );
MAT *sp_eigen_lapack_generalized_expert_rbounds(SPMAT *A, SPMAT *B, VEC *eig_values, int nb_eig , double lower_rbound, double upper_rbound );

/* DRIVER */
MAT* sp_eigen_lapack( SPMAT *A, SPMAT *B, VEC *eig_values, EIGENt_LAPACK_PROBLEM type, EIGENt_LAPACK_OPTION flag,
                      int    nb_eig,
                      int    lower_ibound, int    upper_ibound ,
                      double lower_rbound, double upper_rbound );


#ifdef __cplusplus
}
#endif

#endif