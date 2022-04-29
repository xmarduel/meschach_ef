
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
///////////////////////////////////////////////////////// 

#ifndef SPARSE2_ADDS_H
#define SPARSE2_ADDS_H

#include "matrix.h"
#include "sparse.h"


typedef enum {

   BANDWRe_NULL     = -1,
   BANDWRe_MESCHACH =  0,
   BANDWRe_SPOOLES  =  1,
   BANDWRe_AMD      =  2

} BANDWRt_METHOD ;

typedef enum {

   BANDWRe_NO_OPTION   =  0,
   BANDWRe_BAND        =  1,
   BANDWRe_PROFILE     =  2
   
} BANDWRt_OPTION ;

typedef enum {

   BANDWRe_SYM                = 1,
   BANDWRe_NONSYM             = 2

} BANDWRt_MATRIXTYPE ;


#ifdef __cplusplus
extern "C"
{
#endif
 
SPMAT* sp_makepermutation       ( const SPMAT* A, const PERM* P, const PERM* INVP, SPMAT* PAPt );
void   sp_evalpermutation_bandwr( const SPMAT* A,       PERM* P,       PERM* INVP, BANDWRt_METHOD method, BANDWRt_OPTION option, BANDWRt_MATRIXTYPE mtype);
SPMAT* sp_dopermutationforbandwr( const SPMAT* A,       PERM* P,       PERM* INVP, BANDWRt_METHOD method, BANDWRt_OPTION option, BANDWRt_MATRIXTYPE mtype, SPMAT* PAPt );

VEC* spCHsolve_bandwr( const SPMAT *CH_BWR,                   const PERM *P, const PERM* INVP, const VEC* in, VEC *out);
VEC* spLUsolve_bandwr( const SPMAT *LU_BWR, const PERM *LU_P, const PERM *P, const PERM* INVP, const VEC* in, VEC *out);

VEC* spCHresolution_meschach( const SPMAT *A, const VEC* in, VEC *out);
VEC* spLUresolution_meschach( const SPMAT *A, const VEC* in, VEC *out);
   
VEC* spCHresolution_amd( const SPMAT *A, const VEC* in, VEC *out);
VEC* spLUresolution_amd( const SPMAT *A, const VEC* in, VEC *out);


#ifdef __cplusplus
extern "C"
}
#endif


#endif
