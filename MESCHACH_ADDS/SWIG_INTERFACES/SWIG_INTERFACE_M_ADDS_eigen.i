
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
///////////////////////////////////////////////////////// 

#ifndef EIGEN_PLUS_H
#define EIGEN_PLUS_H

#include "matrix.h"
#include "sparse.h"


#ifdef __cplusplus
extern "C"
{
#endif


typedef enum {

   EIGENe_MESCHACH =  1,
   EIGENe_SPOOLES  =  2,
   EIGENe_LAPACK   =  3

} EIGENt_METHOD ;


typedef enum {

   EIGENe_ORDINARY_SYMMETRIC        =  0,
   EIGENe_GENERALIZED_SYMMETRIC_1   =  1,
   EIGENe_GENERALIZED_SYMMETRIC_2   =  2,

} EIGENt_PROBLEM ;

typedef enum {

   EIGENe_LOWER     =  0,
   EIGENe_UPPER     =  1,
   EIGENe_NEAREST_1 =  2,
   EIGENe_NEAREST_2 =  3,
   EIGENe_IBOUNDS   =  4,
   EIGENe_RBOUNDS   =  5,   
   EIGENe_ALL       =  6

} EIGENt_OPTION ;

typedef struct {

   EIGENt_PROBLEM problem;
   EIGENt_OPTION  option;
   
   int lower_ibound;
   int upper_ibound;

   double lower_rbound;
   double upper_rbound;

   int    nb_eig;
   
} EIGENt_PARAMS;


EIGENt_PARAMS *eig_params_get(void);
int eig_params_set_problem(EIGENt_PARAMS *eig_param, EIGENt_PROBLEM problem);
int eig_params_set_option (EIGENt_PARAMS *eig_param, EIGENt_OPTION option);
int eig_params_set_ibounds(EIGENt_PARAMS *eig_param, int lower_bound, int upper_bound);
int eig_params_set_rbounds(EIGENt_PARAMS *eig_param, double lower_bound, double upper_bound);
int eig_params_set_nb_eigv(EIGENt_PARAMS *eig_param, int nb_eigv);
int eig_params_free(EIGENt_PARAMS *eig_param);


MAT  *eigen_ordinary(MAT *A, VEC *eigen_values);

MAT  *sp_eigen_meschach_ordinary   (SPMAT *A,           VEC *eigen_values);
MAT  *sp_eigen_meschach_generalized(SPMAT *A, SPMAT *B, VEC *eigen_values);

MAT  *sp_eigen( SPMAT *A, SPMAT *B, VEC *eigen_values, EIGENt_METHOD method, EIGENt_PARAMS *eig_params);

#ifdef __cplusplus
}
#endif


#endif