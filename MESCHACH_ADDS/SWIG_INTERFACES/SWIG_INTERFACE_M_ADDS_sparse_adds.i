
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
///////////////////////////////////////////////////////// 

#ifndef SPARSE_ADDS_H
#define SPARSE_ADDS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "matrix.h"
#include "sparse.h"



/* -------------   New functions on usual structs ----*/

SPMAT* sp_ident(SPMAT* in);
SPMAT* sp_eye(SPMAT* in);
SPMAT* sp_ones(SPMAT* in);
SPMAT* sp_tridiag(SPMAT* in, real lower_val, real diag_val, real upper_val);

/* Pour l'assemblage matrice element fini -- */
real sp_add_val(SPMAT *A, int i, int j, Real val);
 
/* solution de LDL^T.x = b pour sparses matrices */
VEC *spLDLsolve(SPMAT *L, VEC *b, VEC *x);
/* remark: LDL a ete cree par LDLfactor puis mat2sp */ 
SPMAT *dense2sp(MAT *inn, SPMAT *out);

SPMAT *sp_insert_submat(SPMAT *A, int i0, int j0, const MAT *B);
MAT   *m_get_subsp(MAT *A, int i0, int j0, const SPMAT *B);


/* nouvelles operation pour les SPMAT */
SPMAT *sp_transp(SPMAT *in,SPMAT *out);

SPMAT *sp_m_mlt(SPMAT *A, SPMAT *B, SPMAT *C);     /* C = A.B   */
SPMAT *sp_mmtr_mlt(SPMAT *A, SPMAT *B, SPMAT *C);  /* C = A.B^T */
SPMAT *sp_mtrm_mlt(SPMAT *A, SPMAT *B, SPMAT *C);  /* C = A^T.B */

SPMAT *sp_zero_subsp(SPMAT *A, int i0, int j0, int m, int n);

SPMAT *sp_insert_subsp(SPMAT *A, int i0 , int j0 , SPMAT *B);
SPMAT *sp_add_subsp   (SPMAT *A, int i0, int j0, SPMAT *B);

SPMAT *sp_insert_subsp_block(SPMAT *A, int p, int q, SPMAT *B);
SPMAT *sp_add_subsp_block   (SPMAT *A, int p, int q, SPMAT *B);

SPMAT *sp_move(SPMAT *A, int i0, int j0, int m0 , int n0, SPMAT *B, int i1, int j1);
SPMAT *sp_madd(SPMAT *A, int i0, int j0, int m0 , int n0, SPMAT *B, int i1, int j1);

SPMAT *px_sp_rows(PERM *P, SPMAT *A, SPMAT* B);
SPMAT *px_sp_cols(PERM *P, SPMAT *A, SPMAT* B);

SPMAT *spILUTfactor(SPMAT *A, SPMAT *B , int lfil, Real tol);
VEC   *spILUsolve(SPMAT *A, VEC *b , VEC *x);

int   sp_nonzeros(SPMAT *A);
int   sp_nonzeros_symmetric_part(const SPMAT *A);

int   sp_bandw  (SPMAT *A);
int   sp_profile(SPMAT *A);




//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int    sp_dim1(const SPMAT *A)
{
   if ( A == SMNULL )      error(E_NULL,"sp_dim1"); return A->m;
}

int    sp_dim2(const SPMAT *A)
{
   if ( A == SMNULL )      error(E_NULL,"sp_dim2");   return A->n;
}

int sp_check(SPMAT* spmat)
{
   return 1; //if ( ! imat ) error(E_NULL,"sp_check");
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

%}




#ifdef __cplusplus
}
#endif

#endif
