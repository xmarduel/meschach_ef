#ifndef MEF_NAVIERSTOKES_H
#define MEF_NAVIERSTOKES_H

/** \file algo_navierstokes.h
 *
 * Contains algorithms for NavierStokes  - dimension independant -
 *
 * -------------------------------------------------------------------- */


#ifdef __cplusplus
extern "C"
{
#endif


#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

int navierstokes_resol_uzawa       (SPMAT *A, SPMAT *B, SPMAT *C, SPMAT *PRECOND, VEC *U, VEC *P, VEC *F, VEC *G);
int navierstokes_resol_uzawa_factOk(SPMAT *LU_A, PERM *PERM_LU_A, SPMAT *B, SPMAT *C, SPMAT *LU_P , PERM *PERM_LU_P, VEC *U, VEC *P,
                                    VEC *F, VEC *G, PERM *Pu, PERM *INVPu, PERM *Pp, PERM *INVPp);


int navierstokes_resol_pressurematrix       (SPMAT *A,                                SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G);
int navierstokes_resol_pressurematrix_factOk(SPMAT *A, SPMAT *LU_A , PERM *PERM_LU_A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G, PERM *Pu, PERM *INVPu);


#ifdef __cplusplus
}
#endif

#endif




