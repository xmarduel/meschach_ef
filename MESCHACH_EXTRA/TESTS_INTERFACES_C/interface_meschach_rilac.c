/* Equation KS 1D
 *
 * ---------------
 * $Author: Xavier Marduel $
 * $Date: 23/02/2000 $
 * $Revision: 1.0 $
 * -------------------------------------------------------------- */

#include <matrix.h>

#include "interface_meschach_rilac.h"


/* subr. rilac(n,nn,a,na,c,d,rcond,x,w,nnw,z,eps,iwrk,wrk1,wrk2,ierr)   */
/* subr. lybsc(n,a,na,c,x,u,eps,wrk,mode,ierr)                          */
/* subr. syhsc(n,m,a,na,b,mb,c,z,wrk1,nwrk1,wrk2,nwrk2,iwrk,niwrk,ierr) */
/*                                                                      */
/* must be linked with -llapack -lblas -lfor -lriccati                  */
/* with the LDFLAGS okayed                                              */

/************************************************************************/
MAT *interface_meschach_rilac(MAT *A, MAT *B, MAT *C, MAT *X)
{
   /*                                             */
   /* X solution of A'*X + X*A + C - X*B*X = 0    */
   /*                                             */
   int n  = A->m ;          /* Achtung : the transposed */
   int nn = A->m + A->m ;   /* Achtung : the transposed */
   int na = A->n ;          /* Achtung : the transposed */

   int ierr=0;

   double rcond;
   double eps=MACHEPS;

   MAT *W = m_get(nn,nn);
   MAT *Z = m_get(nn,nn);

   MAT *At   = m_get(A->n,A->m);
   MAT *Xt   = m_get(X->n,X->m);
   MAT *Bt   = m_get(B->n,B->m);
   MAT *Ct   = m_get(C->n,C->m);

   VEC  *WRK1 = v_get(nn);
   VEC  *WRK2 = v_get(nn);

   IVEC *IWRK = iv_get(nn) ;


   At = m_transp(A,At);
   Bt = m_transp(B,Bt);
   Ct = m_transp(C,Ct);

   rilac_(&n, &nn, At->base, &na,
                  Ct->base,
                  Bt->base,
          &rcond,  Xt->base, W->base, &nn, Z->base, &eps ,
          IWRK->ive, WRK1->ve, WRK2->ve , &ierr );

   X = m_transp(Xt,X);

   V_FREE(WRK1);
   V_FREE(WRK2);
   IV_FREE(IWRK);
   
   M_FREE(At);
   M_FREE(Xt);
   M_FREE(Bt);
   M_FREE(Ct);

   M_FREE(W);
   M_FREE(Z);

   return X;
}


/**********************************************************************/


MAT *interface_meschach_lybsc(MAT *A, MAT *C, MAT *X)
{
/*                                             */
/* X solution of A'*X + X*A = C                */
/*                                             */
int n  = A->m ;          /* Achtung : the transposed */
int nn = A->m + A->m ;   /* Achtung : the transposed */
int na = A->n ;          /* Achtung : the transposed */

int ierr=0;
int mode=0;

double eps=MACHEPS;

MAT *At   = m_get(A->n,A->m);
MAT *Ct   = m_get(C->n,C->m);
MAT *Xt   = m_get(X->n,X->m);

MAT *Ut   = m_get(A->n,A->m);

VEC  *WRK = v_get(n);


  At = m_transp(A,At);
  Ct = m_transp(C,Ct);

  /* note : C must be symetric */

  lybsc_(&n, At->base, &na,
             Ct->base,
             Xt->base,
             Ut->base, &eps , WRK->ve, &mode , &ierr );

  X = m_transp(Xt,X);

  V_FREE(WRK);
   
  M_FREE(At);
  M_FREE(Xt);
  M_FREE(Ut);


return X;
}
/******************************************************************/

MAT *interface_meschach_syhsc(MAT *A, MAT *B, MAT *C, MAT *X)
{
/*                                             */
/* X solution of A*X + X*B = C                 */
/*                                             */
int n  = A->m ;          /* Achtung : the transposed */
int na = A->n ;          /* Achtung : the transposed */

int m  = B->m ;          /* Achtung : the transposed */
int mb = B->n ;          /* Achtung : the transposed */

int ierr=0;

double eps=MACHEPS;

MAT *At   = m_get(A->n,A->m);
MAT *Xt   = m_get(X->n,X->m);
MAT *Bt   = m_get(B->n,B->m);
MAT *Ct   = m_get(C->n,C->m);

MAT *Zt   = m_get(B->n,B->m);

int nwork1 = 2*n*n  +  7*n ; VEC  *WRK1 = v_get(nwork1);
int nwork2 = n+na          ; VEC  *WRK2 = v_get(nwork2);

int niwork = 4*n           ;IVEC  *IWRK = iv_get(niwork);


  At = m_transp(A,At);
  Bt = m_transp(B,Bt);
  Ct = m_transp(C,Ct);

  syhsc_(&n, &m, At->base, &na,
                 Bt->base, &mb,
                 Ct->base,
                 Zt->base,  &eps ,
                 WRK1->ve , &nwork1,
                 WRK2->ve , &nwork2,
                 IWRK->ive, &niwork, &ierr );

  X = m_transp(Ct,X); /* Ct was on return the solution */

  V_FREE(WRK1);
  V_FREE(WRK2);
  IV_FREE(IWRK);
   
  M_FREE(At);
  M_FREE(Xt);
  M_FREE(Bt);
  M_FREE(Ct);

  M_FREE(Zt);

return X;
}
/**********************************************************************/
