#include <stdlib.h>
#include <stdio.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"

#include "MESCHACH_LAPACK/INCLUDES/lapack_base.h"

#ifdef MACOSX
#include "CBLAS/cblas.h"
#include "CBLAS/clapack.h"
#endif

#ifdef LINUX
#include "clapack/f2c.h"
#include "clapack/cblas.h"
#include "clapack/clapack.h"
#endif

/** \file lapack_base.c
 *
 * Contains the basic routines to convert meschach stucture to lapack arrays (fortran)
 * 
 */

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT* m_transp2(MAT *A)
{
   int old_m, old_n;
   int i,j;
   Real *tmp;
   
   /* check */
   if ( A  == NULL ) error(E_NULL,"m_transp2");

   /* start */
   old_m = A->m;
   old_n = A->n;

   /* set the transposed dimensions */
   A->max_m = A->m = old_n;
   A->max_n = A->n = old_m;
   
   /* allocate a tmp vector of the size of the whole matrix */
   tmp = calloc( old_m*old_n , sizeof(Real) );
   /* and copy the matrix into it */
   memcpy(tmp, A->base, old_m*old_n * sizeof(Real));
   
   /* free the old rows pointers (m) */
   free(A->me);

   if (mem_info_is_on())
   {
      mem_bytes(TYPE_MAT,old_m*sizeof(Real *),0);
   }

   /* and allocate then again (n) */
   A->me = calloc( old_n,  sizeof(Real*) );
   /* and set them at the right location */
   for (i=0; i<old_n; i++)
   {
      A->me[i] = &(A->base[i*old_m]);
   }
   if (mem_info_is_on())
   {
      mem_bytes(TYPE_MAT,0,old_n*sizeof(Real *));
   }

   /* set the values in A' */
   for (i=0; i<A->m; i++)
   for (j=0; j<A->n; j++)
   {
      A->me[i][j] = tmp[j*old_n + i];  /* A'[i][j] = A[j][i] = a->base[j*old_n+i] */
   }
   
   /* clean */
   free(tmp);

   /* finito */
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void b_foutput(FILE* fp, const BAND* B)
{
   /* check */
   if ( fp  == NULL ) error(E_NULL,"b_foutput");
   if ( B   == NULL ) error(E_NULL,"b_foutput");

   fprintf(fp, "# BAND lb = %d   ub = %d \n", B->lb, B->ub);
   m_foutput(fp,B->mat);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

BAND *sp2band     (const SPMAT *A )
{
   BAND *AB;

   int lb,ub;
   int i,I,J;
   int idx,idx1,idx2;
   SPROW *row;
   double val;
   
   /* check */
   if ( A  == SMNULL ) error(E_NULL,"sp2band");

   ub = 0;
   lb = 0;
   /* calculate from the sparse matrix lb and lu */
   for (i=0; i<A->m; i++)
   {
      row = A->row + i;

      idx1 = row->elt[0].col ;
      idx2 = row->elt[row->len-1].col ;

      lb = max(lb, i-idx1);
      ub = max(ub, idx2-i);
   }

   /*
   printf( " lb = %d \n", lb);
   printf( " ub = %d \n", ub);
   */
   
   /* create */
   AB = bd_get(lb, ub, A->m);

   m_zero(AB->mat);
   /* fill */
   for (i=0; i<A->m; i++)
   {
      row = A->row + i;

      for (idx=0; idx<row->len; idx++)
      {
         idx2 = row->elt[idx].col ;
         val  = row->elt[idx].val ;

         I = lb - (i-idx2);
         J = idx2 ;

         assert( (I>=0) && (I<lb+ub+1) );

         AB->mat->me[I][J] = val;
      }
   }

   /* finito */
   return AB;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

BAND *sp2lowerband(const SPMAT *A )
{
   BAND *AB;

   int lb;
   int i,I,J;
   int idx,idx1,idx2;
   SPROW *row;
   double val;

   /* check */
   if ( A  == SMNULL ) error(E_NULL,"sp2lowerband");

   lb = 0;
   /* calculate from the sparse matrix lb and lu */
   for (i=0; i<A->m; i++)
   {
      row = A->row + i;

      idx1 = row->elt[0].col ;

      lb = max(lb, i-idx1);
   }

   /*
    printf( " lb = %d \n", lb);
    printf( " ub = %d \n", ub);
    */

   /* create */
   AB = bd_get(lb, 0, A->m);

   m_zero(AB->mat);
   /* fill */
   for (i=0; i<A->m; i++)
   {
      row = A->row + i;

      for (idx=0; idx<row->len; idx++)
      {
         idx2 = row->elt[idx].col ;
         val  = row->elt[idx].val ;

         if ( idx2 <= i )
         {
            I = lb - (i-idx2);
            J = idx2 ;

            assert( (I>=0) && (I<=lb) );

            AB->mat->me[I][J] = val;
         }
      }
   }

   /* finito */
   return AB;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/*
                    meschach banded     lapack banded
 [ 1 3 0 0 0 ]
 [ 2 1 3 0 0 ]      [ 2 2 2 2 0 ]       [ 0 3 3 3 3 ]
 [ 0 2 1 3 0 ]  ->  [ 1 1 1 1 1 ]   ->  [ 1 1 1 1 1 ]
 [ 0 0 2 1 3 ]      [ 0 3 3 3 3 ]       [ 2 2 2 2 0 ]
 [ 0 0 0 2 1 ]

 */

MAT* band2lapackbanded(const BAND* in )
{
   MAT* OUTt;
   MAT* OUT;

   VEC *tmp;
   int i;

   /* tests NULL structures */
   if ( in==BDNULL ) error(E_NULL,"band2bandedlapack");
   if ( in->mat==MNULL ) error(E_NULL,"band2bandedlapack");


   OUT  = m_get(in->mat->m, in->mat->n);
   OUTt = m_get(in->mat->n, in->mat->m);

   /* reverse the columns for lapack format */
   tmp = v_get(in->mat->n);

   for ( i=0; i<in->mat->m; i++)
   {
      get_row(in->mat,          i, tmp);
      set_row(OUT    , OUT->m-i-1, tmp);
   }

   /* transpose for FORTRAN format  */
   m_transp(OUT, OUTt);

   /* clean */
   V_FREE(tmp);
   M_FREE(OUT);

   /* finito */
   return OUTt;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT  *spmat2lapackbanded(const SPMAT *A )
{
   MAT *B;

   BAND *X;

   /* tests NULL structures */
   if ( A==SMNULL ) error(E_NULL,"spmat2lapackbanded");

   /* first a meschach banded matrix */
   X = sp2band(A);

   /* get a banded matrix for lapack format */
   B = band2lapackbanded(X);

   /* clean */
   BD_FREE(X);

   /* finito */
   return B;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT  *spmat2lapacklowerbanded(const SPMAT *A )
{
   MAT *B;

   BAND *X;

   /* tests NULL structures */
   if ( A==SMNULL ) error(E_NULL,"spmat2lapacklowerbanded");

   /* first a meschach banded matrix */
   X = sp2lowerband(A);

   /* get a banded matrix for lapack format */
   B = band2lapackbanded(X);

   /* clean */
   BD_FREE(X);

   /* finito */
   return B;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* Ax_lapacklowerbanded(const SPMAT *A, const VEC * v1, VEC *v2)
{
   MAT *LAPACK;

   /* check */
   if ( A == SMNULL) error(E_NULL,"Ax_lapacklowerbanded");
   if ( v1 == VNULL) error(E_NULL,"Ax_lapacklowerbanded");
   if ( v2 == VNULL) error(E_NULL,"Ax_lapacklowerbanded");
   
   /* init */
   LAPACK = spmat2lapacklowerbanded(A);


   /* dsbmv.f :    y := alpha*A*x + beta*y

   void cblas_dsbmv(const enum CBLAS_ORDER Order, const enum CBLAS_UPLO Uplo,
                    const int N, const int K, const double alpha, const double *A,
                    const int lda, const double *X, const int incX,
                    const double beta, double *Y, const int incY);
   */

#ifdef MACOSX
   cblas_dsbmv(CblasColMajor, CblasLower,
               LAPACK->m, LAPACK->n-1,  1.0, LAPACK->base,  LAPACK->n, v1->ve, 1, 0.0, v2->ve, 1);
#endif

#ifdef LINUX
   dsbmv_(CblasColMajor, CblasLower,
               LAPACK->m, LAPACK->n-1,  1.0, LAPACK->base,  LAPACK->n, v1->ve, 1, 0.0, v2->ve, 1);
#endif
   /* finito */
   return v2;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* Ax_lapackbanded(const SPMAT *A, const VEC * v1, VEC *v2)
{
   MAT *LAPACK;

   /* check */
   if ( A == SMNULL) error(E_NULL,"Ax_lapackbanded");
   if ( v1 == VNULL) error(E_NULL,"Ax_lapackbanded");
   if ( v2 == VNULL) error(E_NULL,"Ax_lapackbanded");
   
   /* init */
   LAPACK = spmat2lapackbanded(A);

   /* dsbmv.f :     y := alpha*A*x + beta*y,

   void cblas_dgbmv(const enum CBLAS_ORDER Order, const enum CBLAS_TRANSPOSE TransA,
                    const int M, const int N,
                    const int KL, const int KU,
                    const double alpha,
                    const double *A, const int lda, const double *X,
                    const int incX, const double beta, double *Y, const int incY);
   */

#ifdef MACOSX
   cblas_dgbmv(CblasColMajor, CblasNoTrans, /* to check */
               A->m, A->n, /* "true" dims of the matrix */
               (LAPACK->n-1)/2,(LAPACK->n-1)/2, /* to check  BUGGY if not etwas sym */
               1.0,
               LAPACK->base, LAPACK->n, v1->ve, 1, 0.0, v2->ve, 1);
#endif

#ifdef LINUX
   dgbmv_(CblasColMajor, CblasNoTrans, /* to check */
               A->m, A->n, /* "true" dims of the matrix */
               (LAPACK->n-1)/2,(LAPACK->n-1)/2, /* to check  BUGGY if not etwas sym */
               1.0,
               LAPACK->base, LAPACK->n, v1->ve, 1, 0.0, v2->ve, 1);
#endif
   /* finito */
   return v2;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */
