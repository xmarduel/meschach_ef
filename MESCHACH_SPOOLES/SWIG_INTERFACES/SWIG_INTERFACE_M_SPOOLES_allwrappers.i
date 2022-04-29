
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_SPOOLES
//
///////////////////////////////////////////////////////// 

#ifndef MESCHACH_TO_SPOOLES__H
#define MESCHACH_TO_SPOOLES__H

#include <matrix.h>
#include <sparse.h>


typedef enum {

   BANDWRe_SPOOLES_SYM     =  0,
   BANDWRe_SPOOLES_NONSYM  =  1

} BANDWRt_SPOOLES_OPTION ;


VEC* spCHresolution_spooles( SPMAT *A, VEC *in, VEC *out);
VEC* spLUresolution_spooles( SPMAT *A, VEC *in, VEC *out);

PERM* sp_bandwr_spooles(SPMAT *A, PERM *P, PERM *INVP, BANDWRt_SPOOLES_OPTION type);


typedef enum {

   EIGENe_SPOOLES_ORDINARY_SYMMETRIC        =  0,
   EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_1   =  1,
   EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_2   =  2,

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


MAT* sp_eigen_spooles( SPMAT *A, SPMAT *B, VEC *eigen_values, EIGENt_SPOOLES_PROBLEM type, EIGENt_SPOOLES_OPTION flag );

#endif