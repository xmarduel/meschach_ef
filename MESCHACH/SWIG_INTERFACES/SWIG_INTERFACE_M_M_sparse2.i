
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
///////////////////////////////////////////////////////// 

#ifndef SPARSE2H
#define SPARSE2H

#include "sparse.h"


SPMAT	*spCHfactor(SPMAT *), *spICHfactor(SPMAT *), *spCHsymb(SPMAT *);
VEC	*spCHsolve(SPMAT *,VEC *,VEC *);

SPMAT	*spLUfactor(SPMAT *,PERM *,double);
SPMAT	*spILUfactor(SPMAT *,double);
VEC	*spLUsolve(SPMAT *,PERM *,VEC *,VEC *),
	*spLUTsolve(SPMAT *,PERM *,VEC *,VEC *);

SPMAT	*spBKPfactor(SPMAT *, PERM *, PERM *, double);
VEC	*spBKPsolve(SPMAT *, PERM *, PERM *, VEC *, VEC *);

//VEC	*pccg(VEC *(*A)(),void *A_par,VEC *(*M_inv)(),void *M_par,VEC *b, double tol,VEC *x);
//VEC	*sp_pccg(SPMAT *,SPMAT *,VEC *,double,VEC *);
//VEC	*cgs(VEC *(*A)(),void *A_par,VEC *b,VEC *r0,double tol,VEC *x);
//VEC	*sp_cgs(SPMAT *,VEC *,VEC *,double,VEC *);
//VEC	*lsqr(VEC *(*A)(),VEC *(*AT)(),void *A_par,VEC *b,double tol,VEC *x);
//VEC	*sp_lsqr(SPMAT *,VEC *,double,VEC *);
//int	cg_set_maxiter(int);

//void	lanczos(VEC *(*A)(),void *A_par,int m,VEC *x0,VEC *a,VEC *b,Real *beta_m1,MAT *Q);
//void	sp_lanczos(SPMAT *,int,VEC *,VEC *,VEC *,Real *,MAT *);
//VEC	*lanczos2(VEC *(*A)(),void *A_par,int m,VEC *x0,VEC *evals, VEC *err_est);
//VEC	*sp_lanczos2(SPMAT *,int,VEC *,VEC *,VEC *);
extern  void    scan_to(SPMAT *,IVEC *,IVEC *,IVEC *,int);
extern  row_elt  *chase_col(SPMAT *,int,int *,int *,int);
extern  row_elt  *chase_past(SPMAT *,int,int *,int *,int);
extern  row_elt  *bump_col(SPMAT *,int,int *,int *);


#endif
