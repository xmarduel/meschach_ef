
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  

#ifndef MEF_ALGO_STOKES_H
#define MEF_ALGO_STOKES_H

#include "matrix.h"
#include "sparse.h"


int stokes_resol_pressurematrix       (SPMAT *A ,               SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps);
int stokes_resol_pressurematrix_factOk(SPMAT *A , SPMAT *LLT_A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps,
                                       PERM *Pu, PERM *INVPu);

int stokes_resol_uzawa        (SPMAT *A ,               SPMAT *B, SPMAT *C,                 SPMAT *LLT_PRECOND, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps);
int stokes_resol_uzawa_factOk (SPMAT *A , SPMAT *LLT_A, SPMAT *B, SPMAT *C, SPMAT *PRECOND, SPMAT *LLT_PRECOND, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps,
                               PERM *Pu, PERM *INVPu, PERM *Pp, PERM *INVPp);

int stokes_resol_augmentedlagrangian(SPMAT *A, SPMAT *B, SPMAT *J, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps);


#endif
