

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_ADDS/INCLUDES/control.h"

/** \file control.c
 *
 * Contains the functions to solve control problems
 *
 */

static int CONTROL_METHOD = CONTROLe_SLICOT; /* or CONTROLe_SCILAB */

static MAT *scilab_riccati  (const MAT *A, const MAT *B, const MAT *C, MAT *X);
static MAT *slicot_riccati  (const MAT *A, const MAT *B, const MAT *C, MAT *X);

static MAT *scilab_lyapunov (const MAT *A, const MAT *C, MAT *X);
static MAT *slicot_lyapunov (const MAT *A, const MAT *C, MAT *X);

static MAT *scilab_sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X);
static MAT *slicot_sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X);

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void set_control_method( int method )
{
   switch (method)
   {
      case CONTROLe_SLICOT: CONTROL_METHOD = CONTROLe_SLICOT; break;
      case CONTROLe_SCILAB: CONTROL_METHOD = CONTROLe_SCILAB; break;

      default: warning(WARN_UNKNOWN, "set_control_method"); CONTROL_METHOD = CONTROLe_SLICOT; break;
   }
}

int get_control_method()
{
   return CONTROL_METHOD;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *riccati(const MAT *A, const MAT *B, const MAT *C, MAT *X)
{
   switch(CONTROL_METHOD)
   {
      case CONTROLe_SLICOT: return slicot_riccati(A, B, C, X); break;
      case CONTROLe_SCILAB: return scilab_riccati(A, B, C, X); break;
   }

   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *lyapunov(const MAT *A, const MAT *C, MAT *X)
{
   switch(CONTROL_METHOD)
   {
      case CONTROLe_SLICOT: return slicot_lyapunov(A, C, X); break;
      case CONTROLe_SCILAB: return scilab_lyapunov(A, C, X); break;
   }

   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X)
{
   switch(CONTROL_METHOD)
   {
      case CONTROLe_SLICOT: return slicot_sylvester(A, B, C, X); break;
      case CONTROLe_SCILAB: return scilab_sylvester(A, B, C, X); break;
   }

   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static MAT *scilab_riccati(const MAT *A, const MAT *B, const MAT *C, MAT *X)
{
#ifdef HAVE_SCILAB_CONTROL
   /*                                             */
   /* X solution of A'*X + X*A + C - X*B*X = 0    */
   /*                                             */
   long ierr=0;
   
   Real rcond;
   Real eps=MACHEPS;

   /* check */
   if ( A == MNULL ) error(E_NULL,"scilab_riccati");
   if ( B == MNULL ) error(E_NULL,"scilab_riccati");
   if ( C == MNULL ) error(E_NULL,"scilab_riccati");
   if ( X == MNULL ) error(E_NULL,"scilab_riccati");

   /* init */
   long n  = A->m ;          /* Achtung : the transposed */
   long nn = A->m + A->m ;   /* Achtung : the transposed */
   long na = A->n ;          /* Achtung : the transposed */

   MAT* At = m_transp((MAT*)A, MNULL);
   MAT* Bt = m_transp((MAT*)B, MNULL);
   MAT* Ct = m_transp((MAT*)C, MNULL);
   MAT* Xt = m_transp((MAT*)X, MNULL);

   MAT* W = m_get(nn,nn);
   MAT* Z = m_get(nn,nn);

   VEC* WRK1 = v_get(nn);
   VEC* WRK2 = v_get(nn);

   IVEC* IWRK = iv_get(2*nn) ;

   /* SCILAB ROUTINE */
   rilac_(&n, &nn, At->base, &na,
      Ct->base,
      Bt->base,
      &rcond,  Xt->base, W->base, &nn, Z->base, &eps ,
      IWRK->ive, WRK1->ve, WRK2->ve , &ierr );
   /* SCILAB ROUTINE */
   
   X = m_transp(Xt,X);
   
   /* clean */
   V_FREE(WRK1);
   V_FREE(WRK2);
   IV_FREE(IWRK);
   
   M_FREE(At);
   M_FREE(Xt);
   M_FREE(Bt);
   M_FREE(Ct);
   
   M_FREE(W);
   M_FREE(Z);
   
#endif

   /* finito */
   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static MAT *scilab_lyapunov(const MAT *A, const MAT *C, MAT *X)
{
#ifdef HAVE_SCILAB_CONTROL
   /*                                             */
   /* X solution of A'*X + X*A = C                */
   /*                                             */
   long ierr=0;
   long mode=0;
   
   Real eps=MACHEPS;

   /* check */
   if ( A == MNULL ) error(E_NULL,"scilab_lyapunov");
   if ( C == MNULL ) error(E_NULL,"scilab_lyapunov");
   if ( X == MNULL ) error(E_NULL,"scilab_lyapunov");

   /* init */
   long n  = A->m ;          /* Achtung : the transposed */
   long na = A->n ;          /* Achtung : the transposed */

   MAT* At = m_transp((MAT*)A, MNULL);
   MAT* Ct = m_transp((MAT*)C, MNULL);
   MAT* Xt = m_transp((MAT*)X, MNULL);
   
   MAT* Ut   = m_get(A->n,A->m);
   
   VEC* WRK = v_get(n);
   
   /* note : C must be symetric */
   
   /* SCILAB ROUTINE */
   lybsc_(&n, At->base, &na,
      Ct->base,
      Xt->base,
      Ut->base, &eps , WRK->ve, &mode , &ierr );
   /* SCILAB ROUTINE */
   
   X = m_transp(Xt,X);
   
   /* clean */
   V_FREE(WRK);
   
   M_FREE(At);
   M_FREE(Xt);
   M_FREE(Ut);
   
#endif

   /* finito */
   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static MAT *scilab_sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X)
{
#ifdef HAVE_SCILAB_CONTROL
   /*                                             */
   /* X solution of A*X + X*B = C                 */
   /*                                             */
   long ierr=0;
   Real eps=MACHEPS;
   
   /* check */
   if ( A == MNULL ) error(E_NULL,"scilab_sylvester");
   if ( B == MNULL ) error(E_NULL,"scilab_sylvester");
   if ( C == MNULL ) error(E_NULL,"scilab_sylvester");
   if ( X == MNULL ) error(E_NULL,"scilab_sylvester");

   /* init */
   long n  = A->m ;          /* Achtung : the transposed */
   long na = A->n ;          /* Achtung : the transposed */

   long m  = B->m ;          /* Achtung : the transposed */
   long mb = B->n ;          /* Achtung : the transposed */
   
   MAT* At = m_transp((MAT*)A, MNULL);
   MAT* Bt = m_transp((MAT*)B, MNULL);
   MAT* Ct = m_transp((MAT*)C, MNULL);
   
   MAT* Zt   = m_get(B->n,B->m);

   int nwork1 = 2*n*n  +  7*n ; VEC*  WRK1 = v_get(nwork1);
   int nwork2 = n+na          ; VEC*  WRK2 = v_get(nwork2);

   int niwork = 4*n           ; IVEC* IWRK = iv_get(2*niwork);


   /* SCILAB ROUTINE */
   syhsc_(&n, &m, At->base, &na,
      Bt->base, &mb,
      Ct->base,
      Zt->base,  &eps ,
      WRK1->ve , &nwork1,
      WRK2->ve , &nwork2,
      IWRK->ive, &niwork, &ierr );
   /* SCILAB ROUTINE */
   
   X = m_transp(Ct,X); /* Ct was on return the solution */
   
   /* clean */
   V_FREE(WRK1);
   V_FREE(WRK2);
   IV_FREE(IWRK);
   
   M_FREE(At);
   M_FREE(Bt);
   M_FREE(Ct);
   
   M_FREE(Zt);
   
#endif

   /* finito */
   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static MAT *slicot_riccati  (const MAT *A, const MAT *B, const MAT *C, MAT *X)
{
   /*                                             */
   /* X solution of A'*X + X*A + C - X*B*X = 0    */
   /*                                             */
   /* C and B must be symmetric                   */

#ifdef HAVE_SLICOT
   
   double sep;
   double rcond;
   double ferr;

   long info;
   
   /* check */
   if ( A == MNULL ) error(E_NULL,"slicot_sylvester");
   if ( B == MNULL ) error(E_NULL,"slicot_sylvester");
   if ( C == MNULL ) error(E_NULL,"slicot_sylvester");
   if ( X == MNULL ) error(E_NULL,"slicot_sylvester");

   /* init */
   MAT* AA = m_transp((MAT*)A, MNULL);
   MAT* BB = m_transp((MAT*)B, MNULL);
   MAT* CC = m_transp((MAT*)C, MNULL);

   long ldwork = 5+ 4*A->n*A->n + 8*A->n ;

   IVEC* iwork  = iv_get(2*2*X->m); /* "long" size ! */
   VEC*  dwork  = v_get (ldwork);
   IVEC* bwork  = iv_get(2*2*X->m);
   
   long m = A->m;
   long n = A->n;

   VEC* wr = v_get(2*A->m);
   VEC* wi = v_get(2*A->m);

   long swork = 2*A->m;
   MAT* S = m_get(swork,swork);

   /* SLICOT ROUTINE */
   sb02rd_("X", "C", ".", "N", "U", "G", "S", ".", ".",
           &m, AA->base, &n, NULL, &m, NULL, &m, BB->base, &m, CC->base, &m, X->base, &m,
           &sep, &rcond, &ferr, wr->ve, wi->ve, S->base, &swork, iwork->ive, dwork->ve, &ldwork, bwork->ive, &info);
   /* SLICOT ROUTINE */

   if ( info != 0 )
   {
      printf("slicot_riccati: rc = %ld \n",info );
   }
   
   /* clean */
   M_FREE(AA);
   M_FREE(BB);
   M_FREE(CC);

   V_FREE(dwork);
   IV_FREE(iwork);
   IV_FREE(bwork);

   V_FREE(wr);
   V_FREE(wi);

   M_FREE(S);

#endif
   
   /* finito */
   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static MAT *slicot_lyapunov (const MAT *A, const MAT *C, MAT *X)
{
#ifdef HAVE_SLICOT

   long info;
   long one = 1.0;

   double sep, ferr;

   /* check */
   if ( A == MNULL ) error(E_NULL,"slicot_lyapunov");
   if ( C == MNULL ) error(E_NULL,"slicot_lyapunov");
   if ( X == MNULL ) error(E_NULL,"slicot_lyapunov");

   /* init */
   MAT* AA = m_transp((MAT*)A, MNULL);
   MAT* CC = m_transp((MAT*)C, MNULL);

   long ldwork = (X->n*X->n) ;

   IVEC* iwork  = iv_get(2*ldwork); /* "long" size ! */
   VEC*  dwork  = v_get (2*ldwork);

   long m = A->m;
   long n = A->n;

   VEC* wr = v_get(A->m);
   VEC* wi = v_get(A->m);

   /* SLICOT ROUTINE */
   sb03md_("C", "X", "N", "N", &m, AA->base, &n, X->base, &n, CC->base, &m, &one, &sep, &ferr,
           wr->ve, wi->ve,
           iwork->ive, dwork->ve, &ldwork, &info);
   /* SLICOT ROUTINE */

   if ( info != 0 )
   {
      printf("slicot_lyapunov: rc = %ld \n",info );
   }

   /* get the solution from CC */
   X = m_transp(CC, X);

   /* clean */
   M_FREE(AA);
   M_FREE(CC);

   V_FREE(dwork);
   IV_FREE(iwork);

   V_FREE(wr);
   V_FREE(wi);
   
#endif

   /* finito */
   return X;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static MAT *slicot_sylvester(const MAT *A, const MAT *B, const MAT *C, MAT *X)
{
#ifdef HAVE_SLICOT

   long info;

   /* check */
   if ( A == MNULL ) error(E_NULL,"slicot_sylvester");
   if ( B == MNULL ) error(E_NULL,"slicot_sylvester");
   if ( C == MNULL ) error(E_NULL,"slicot_sylvester");
   if ( X == MNULL ) error(E_NULL,"slicot_sylvester");

   /* init */
   MAT* AA = m_transp((MAT*)A, MNULL);
   MAT* BB = m_transp((MAT*)B, MNULL);
   MAT* CC = m_transp((MAT*)C, MNULL);

   long ldwork = (2*X->n*X->n + 8*X->m) ;
   
   IVEC* iwork  = iv_get(2*4*X->m); /* "long" size ! */
   VEC*  dwork  = v_get (ldwork);

   long m = A->m;
   long n = A->n;

   /* SLICOT ROUTINE */
   sb04md_(&m, &m, AA->base, &n, BB->base, &n, CC->base, &m, X->base, &m, iwork->ive, dwork->ve, &ldwork, &info);
   /* SLICOT ROUTINE */

   if ( info != 0 )
   {
      printf("slicot_sylvester: rc = %ld \n",info );
   }

   /* get the solution from CC */
   X = m_transp(CC, X);

   /* clean */
   M_FREE(AA);
   M_FREE(BB);
   M_FREE(CC);

   V_FREE(dwork);
   IV_FREE(iwork);
   
#endif

   /* finito */
   return X;
}

