
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
/////////////////////////////////////////////////////////
 
#ifndef ITER_ADDS_H
#define ITER_ADDS_H


#include "sparse.h"
#include "iter.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
   ITER_INFOe_NONE   = 0,
   ITER_INFOe_STDOUT = 1,
   ITER_INFOe_FILE   = 2,
   ITER_INFOe_ALL    = 3
} ITER_INFOe ;

	
	
ITER_OUTPUT_INFO* iter_output_info(ITER_INFOe iter_output_type, const char* output_filename);

	
void iter_std1_info(const ITER *ip, Real nres, VEC *res, VEC *Bres);
void iter_std2_info(const ITER *ip, Real nres, VEC *res, VEC *Bres);
void iter_std3_info(const ITER *ip, Real nres, VEC *res, VEC *Bres);

VEC  *iter_bicgstab  ( ITER *ip );

/* same as iter_spcg, but with info ON */

VEC *iter_xspcg      (const SPMAT *A, const SPMAT *LLT, const VEC *b, Real eps, VEC *x,        int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);
VEC *iter_xspcgs     (const SPMAT *A, const SPMAT *LLT, const VEC *b, Real eps, VEC *x,        int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);
VEC *iter_xspgmres   (const SPMAT *A, const SPMAT *B  , const VEC *b, Real eps, VEC *x, int k, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);
VEC *iter_xspmgcr    (const SPMAT *A, const SPMAT *B  , const VEC *b, Real eps, VEC *x, int k, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);
VEC *iter_xspbicgstab(const SPMAT *A, const SPMAT *LLT, const VEC *b, Real eps, VEC *x,        int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);


VEC *iter_xspcg_bandwr      (const SPMAT *A, const SPMAT *LLT, const PERM *P, const PERM *INVP, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);
VEC *iter_xspbicgstab_bandwr(const SPMAT *A, const SPMAT *LLT, const PERM *P, const PERM *INVP, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info);


#ifdef __cplusplus
}
#endif

#endif
