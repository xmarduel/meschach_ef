
#ifndef SPARSE_ADDS_H
#define SPARSE_ADDS_H

/** \file sparse_adds.h
 *
 * extentions to MESCHACH LIBRARY for usual structs 
 *
 * ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"
{
#endif

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"


/* -------------   New functions on SPMAT struct ----*/

/* initialisation */
SPMAT* sp_ident(SPMAT* in);
SPMAT* sp_eye(SPMAT* in);
SPMAT* sp_ones(SPMAT* in);
SPMAT* sp_tridiag(SPMAT* in, real lower_val, real diag_val, real upper_val);

/* Pour l'assemblage matrice element fini -- */
real sp_add_val(SPMAT *A, int i, int j, Real val);
 
/* solution de LDL^T.x = b pour sparses matrices */
VEC *spLDLsolve(const SPMAT *L, const VEC *b, VEC *x);
/* remark: LDL a ete cree par LDLfactor puis mat2sp */ 
SPMAT *dense2sp(const MAT *in, SPMAT *out);

SPMAT *sp_insert_submat(SPMAT *A, int i0, int j0, const MAT *B);
MAT   *m_get_subsp(MAT *A, int i0, int j0, const SPMAT *B);
SPMAT *sp_transpose_rows(SPMAT *A, int i0, int i1);

/* nouvelles operation pour les SPMAT */
SPMAT *sp_transp(const SPMAT *in, SPMAT *out);

SPMAT *sp_m_mlt   (const SPMAT *A, const SPMAT *B, SPMAT *C);  /* C = A.B   */
SPMAT *sp_mmtr_mlt(const SPMAT *A, const SPMAT *B, SPMAT *C);  /* C = A.B^T */
SPMAT *sp_mtrm_mlt(const SPMAT *A, const SPMAT *B, SPMAT *C);  /* C = A^T.B */

SPMAT *sp_zero_subsp(SPMAT *A, int i0, int j0, int m, int n);

SPMAT *sp_insert_subsp(SPMAT *A, int i0, int j0, const SPMAT *B);
SPMAT *sp_add_subsp   (SPMAT *A, int i0, int j0, const SPMAT *B);

SPMAT *sp_insert_subsp_block(SPMAT *A, int p, int q, const SPMAT *B);
SPMAT *sp_add_subsp_block   (SPMAT *A, int p, int q, const SPMAT *B);

SPMAT *sp_move(const SPMAT *A, int i0, int j0, int m0 , int n0, SPMAT *B, int i1, int j1);
SPMAT *sp_madd(const SPMAT *A, int i0, int j0, int m0 , int n0, SPMAT *B, int i1, int j1);

SPMAT *px_sp_rows(const PERM *P, const SPMAT *A, SPMAT* B);
SPMAT *px_sp_cols(const PERM *P, const SPMAT *A, SPMAT* B);

SPMAT *spILUTfactor(SPMAT *A, SPMAT *B , int lfil, Real tol);
VEC   *spILUsolve(const SPMAT *A, const VEC *b , VEC *x);

int   sp_nonzeros(const SPMAT *A);
int   sp_nonzeros_symmetric_part(const SPMAT *A);

int   sp_profile(const SPMAT *A);
int   sp_bandw(const SPMAT *A);

MAT  *m_sp_mmm_invL_A_invLt(const SPMAT *LLT, const SPMAT *A, MAT* M);
MAT  *m_spLTsolve_vectorset(const SPMAT *LLT, MAT *M);

/* operations on SPROW */
SPROW *sprow_lincomb(int, SPROW **allrows, Real *coeffs, int type_sprow, SPROW *out);         /* out <- sum_i s[i].x[i] */
SPROW *sprow_linlist(SPROW *out, int type_sprow, SPROW *r1, real a1, ...);  /* out <- s1.x1 + s2.x2 + ... */

   
#ifdef __cplusplus
}
#endif

#endif
