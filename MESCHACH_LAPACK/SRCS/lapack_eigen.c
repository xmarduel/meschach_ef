#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"

#include "MESCHACH_LAPACK/INCLUDES/lapack_base.h"
#include "MESCHACH_LAPACK/INCLUDES/lapack_eigen.h"

#ifdef MACOSX
#include "CBLAS/cblas.h"
#include "CBLAS/clapack.h"
#endif

#ifdef LINUX
#include "clapack/f2c.h"
#include "clapack/cblas.h"
#include "clapack/clapack.h"
#endif


/** \file lapack_eigen.c
 *
 * Contains the routines for calculating eigenvalues and eigenvectors of matrices \n
 *
 */


static MAT *sp_eigen_lapack_ordinary_expert_common     (const SPMAT *A,  /* NIX , */   VEC *eig_values,
                                                        long nb_eig,
                                                        char *range,
                                                        long   lower_ibound, long   upper_ibound ,
                                                        double lower_rbound, double upper_rbound );

static MAT *sp_eigen_lapack_generalized_expert_common  (const SPMAT *A, const SPMAT *B, VEC *eig_values,
                                                        long nb_eig,
                                                        char *range,
                                                        long   lower_ibound, long   upper_ibound ,
                                                        double lower_rbound, double upper_rbound );


/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_ordinary_full   (const SPMAT *A, VEC *eig_values )
{
   MAT* eig_vectors = NULL; /* the return value */

   long rc;
   long m,n;

   MAT* lapack_banded_a;

   double *dwork;
   long   *iwork;
   
   long a_bandsize;
   long a_bandsize_p1;

   long ldwork;
   long liwork;

   /* check */
   if ( A           == SMNULL ) error(E_NULL, "sp_eigen_lapack_ordinary_full");
   if ( eig_values  == VNULL  ) error(E_NULL, "sp_eigen_lapack_ordinary_full");

   /* allocate memory */
   eig_vectors = m_get(A->m, A->n);
      
   /* start */
   m = (long)A->m;
   n = (long)A->n;

   ldwork =  1 + 5*m + 2*m*m ;
   liwork =  3 + 5*m ;
   
   lapack_banded_a = spmat2lapacklowerbanded(A);

   a_bandsize    = lapack_banded_a->n - 1 ;
   a_bandsize_p1 = lapack_banded_a->n;

   printf( " LAPACK ORDINARY EIGEN-PROBLEM : \n");
   printf( " A->lb = %ld \n", a_bandsize);


   dwork     = calloc( sizeof(double), ldwork);
   iwork     = calloc( sizeof(double), liwork);

   /* ------ solve the system Ax = k.x  ----- */
      
   dsbevd_( "V", "L", &m, &a_bandsize,
            lapack_banded_a->base, &(a_bandsize_p1),
            eig_values->ve,
            eig_vectors->base, &n,
            dwork, &ldwork,
            iwork, &liwork, &rc);

   if ( rc != 0 )
   {
      error(E_UNKNOWN, "sp_eigen_lapack_ordinary_full");
   }

   /* ------ solve the system Ax = k.x  ----- */

   /* set-up the eigenvectors */
   m_transp(eig_vectors,eig_vectors);


   /* clean */
   M_FREE(lapack_banded_a);
   
   free(dwork);
   free(iwork);

   /* finito */
   return eig_vectors;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_generalized_full(const SPMAT *A, const SPMAT *B, VEC *eig_values )
{
   MAT* eig_vectors = NULL; /* the return value */

   long rc;

   MAT* lapack_banded_a;
   MAT* lapack_banded_b;

   long m,n;

   double *dwork;
   long   *iwork;

   long a_bandsize;
   long b_bandsize;

   long a_bandsize_p1;
   long b_bandsize_p1;

   long ldwork,liwork;


   /* check */
   if ( A == SMNULL ) error(E_NULL, "sp_eigen_lapack_generalized_full");
   if ( B == SMNULL ) error(E_NULL, "sp_eigen_lapack_generalized_full");

   if ( eig_values == VNULL ) error(E_NULL, "sp_eigen_lapack_generalized_full");

   /* allocate memory */   
   eig_vectors = m_get(A->m, A->n);

   /* start */
   m = (long)A->m;
   n = (long)A->n;

   ldwork = 1 + 5*m + 2*m*m ;
   liwork = 3 + 5*m;

   lapack_banded_a = spmat2lapacklowerbanded(A);
   lapack_banded_b = spmat2lapacklowerbanded(B);   

   a_bandsize = lapack_banded_a->n -1 ;
   b_bandsize = lapack_banded_b->n -1 ;

   printf( " LAPACK GENERALIZED EIGEN-PROBLEM : \n");
   printf( " A->lb = %ld \n", a_bandsize);
   printf( " B->lb = %ld \n", b_bandsize);



   if (a_bandsize <  b_bandsize)
   {
      MAT* lapack_banded_tmp = m_get(lapack_banded_a->m, lapack_banded_a->n + (b_bandsize -  a_bandsize) );
      /* add a column to A ... because the band of A must not be smaller than those of B ... */
      m_zero(lapack_banded_tmp);

      /* init lapack_banded_tmp with lapack_banded_a */
      lapack_banded_tmp =  m_move(lapack_banded_a, 0,0,lapack_banded_a->m,lapack_banded_a->n, lapack_banded_tmp, 0,0);

      M_FREE(lapack_banded_a);
      lapack_banded_a = lapack_banded_tmp;

      a_bandsize += (b_bandsize -  a_bandsize);
   }

   a_bandsize_p1 =  a_bandsize +1;
   b_bandsize_p1 =  b_bandsize +1;

   dwork     = calloc( sizeof(double), ldwork);
   iwork     = calloc( sizeof(long)  , liwork);

   /* ------ solve the system Ax = k.Bx  ----- */

   dsbgvd_( "V", "L", &m, &a_bandsize, &b_bandsize,
            lapack_banded_a->base, &(a_bandsize_p1),
            lapack_banded_b->base, &(b_bandsize_p1),
            eig_values->ve,
            eig_vectors->base, &n,
            dwork, &ldwork,
            iwork, &liwork,   &rc);

   if ( rc != 0 )
   {
      error(E_UNKNOWN, "sp_eigen_lapack_generalized_all");
   }

   /* ------ solve the system Ax = k.Bx  ----- */
   
   /* set-up the eigenvectors */
   m_transp(eig_vectors,eig_vectors);


   /* clean */
   M_FREE(lapack_banded_a);
   M_FREE(lapack_banded_b);

   free(dwork);
   free(iwork);

   /* finito */
   return eig_vectors;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

static MAT *sp_eigen_lapack_ordinary_expert_common(const SPMAT *A, VEC *eig_values,
                                                   long nb_eig,
                                                   char *range,
                                                   long   lower_ibound, long   upper_ibound ,
                                                   double lower_rbound, double upper_rbound )
{
   MAT* eig_vectors = NULL; /* the return value - not the "full" size hopefully */

   int i;
   long rc;

   MAT* lapack_banded_a;

   MAT  *Q = MNULL;

   long m,n;

   double *dwork;
   long   *iwork;
   long   *ifail;

   long a_bandsize;
   long a_bandsize_p1;

   long eig_found;

   double tol = 1.0e-8;


   /* check */
   if ( A           == SMNULL ) error(E_NULL, "sp_eigen_lapack_ordinary_expert_common");
   if ( eig_values  == VNULL  ) error(E_NULL, "sp_eigen_lapack_ordinary_expert_common");

   /* allocate memory for the eigen-vectors */
   switch(*range)
   {
      case 'A': eig_vectors = m_get(A->m  , A->n ); break;
      case 'I': eig_vectors = m_get(nb_eig, A->m ); break; /* not all */
      case 'V': eig_vectors = m_get(A->m  , A->n ); break;

      default: error(E_UNKNOWN, "sp_eigen_lapack_ordinary_expert_common");
   }

   /* start */
   /*
    printf ( " A prof  = %d \n", sp_profile(A) );
    printf ( " A bandw = %d \n", sp_bandw(A) );
    */

   m = A->m;
   n = A->n;

   Q = m_get(A->m, A->n);

   lapack_banded_a = spmat2lapacklowerbanded(A);

   a_bandsize = lapack_banded_a->n -1 ;

   /*
    m_output(lapack_banded_b);
    */

   printf( " LAPACK ORDINARY EIGEN-PROBLEM (EXPERT): \n");
   printf( " A->lb = %ld \n", a_bandsize);

   dwork     = calloc( sizeof(double), 7*A->m);
   iwork     = calloc( sizeof(long)  , 5*A->m);
   ifail     = calloc( sizeof(long)  , 1*A->m);


   a_bandsize_p1 =  a_bandsize +1;

   /* ------ solve the system Ax = k.x  ----- */

   dsbevx_( "V", range, "L", &m , &a_bandsize,
            lapack_banded_a->base, &(a_bandsize_p1),
            Q->base     , &m,
            &lower_rbound, &upper_rbound,   /* referenced if range = R" -> EIGENe_LAPACK_RBOUNDS */
            &lower_ibound, &upper_ibound,   /* the first n eigen_values & vectors : referenced if EIGENe_LAPACK_IBOUNDS */
            &tol,
            &eig_found, eig_values->ve, eig_vectors->base, &m,
            dwork, iwork, ifail, &rc );

   if ( rc != 0 )
   {
      error(E_UNKNOWN, "sp_eigen_lapack_ordinary_expert_common");
   }
   else
   {
      printf("  ---> eig_found = %ld \n", eig_found);
   }

   /* ------ solve the system Ax = k.x  ----- */

   if ( strcmp(range, "I") == 0 )
   {
      /* fill-out the other eigenvalues */
      for (i=nb_eig; i<eig_values->dim; i++)
      {
         eig_values->ve[i] =  1.0e20;
      }
   }
   if ( strcmp(range, "V") == 0 )
   {
      /* fill-out the other eigenvalues */
      for (i=eig_found; i<eig_values->dim; i++)
      {
         eig_values->ve[i] = 1.0e20;
      }
   }

   /* set-up the eigenvectors */
   switch(*range)
   {
      case 'A': eig_vectors = m_transp(eig_vectors, eig_vectors); break;
      case 'I': eig_vectors = m_transp2(eig_vectors); break; /* because the matrix is MxN  with M != N */
      case 'V': eig_vectors = m_transp(eig_vectors,eig_vectors);
                eig_vectors = m_resize(eig_vectors, A->m, eig_found); break;

      default: error(E_UNKNOWN, "sp_eigen_lapack_ordinary_expert_common");
   }

   /* clean */
   M_FREE(Q);

   M_FREE(lapack_banded_a);

   free(dwork);
   free(iwork);
   free(ifail);

   /* finito */
   return eig_vectors;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_ordinary_expert_lower(const SPMAT *A, VEC *eig_values, int nb_eig )
{
   char range[] = "I";

   long first_eig = 1;
   long last_eig  = 1 + nb_eig ;

   if ( nb_eig < 0 )
   {
      error(E_NEG, "sp_eigen_lapack_ordinary_expert_lower");
   }
   if ( last_eig > A->m )
   {
      last_eig = A->m;
      nb_eig   = A->m;

      warning(WARN_UNKNOWN, "sp_eigen_lapack_ordinary_expert_lower");
   }

   return sp_eigen_lapack_ordinary_expert_common(A, eig_values, nb_eig, range, first_eig, last_eig, 0.0, 0.0);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_ordinary_expert_upper(const SPMAT *A, VEC *eig_values, int nb_eig )
{
   char range[] = "I";

   long first_eig = A->m - nb_eig ;
   long last_eig  = A->m ;

   if ( nb_eig < 0 )
   {
      error(E_NEG, "sp_eigen_lapack_ordinary_expert_upper");
   }
   if ( first_eig < 1 )
   {
      first_eig = 1;
      nb_eig    = A->m;

      warning(WARN_UNKNOWN, "sp_eigen_lapack_ordinary_expert_upper");
   }
   
   return sp_eigen_lapack_ordinary_expert_common(A, eig_values, nb_eig, range, first_eig, last_eig, 0.0, 0.0);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_ordinary_expert_ibounds(const SPMAT *A, VEC *eig_values, int nb_eig, int lower_ibound, int upper_ibound )
{
   char range[] = "I";

   long first_eig = lower_ibound;
   long last_eig  = upper_ibound;

   if ( upper_ibound - lower_ibound  < 0 )
   {
      error(E_NEG, "sp_eigen_lapack_ordinary_expert_ibounds");
   }

   return sp_eigen_lapack_ordinary_expert_common(A, eig_values, upper_ibound - lower_ibound + 1, range, first_eig, last_eig, 0.0, 0.0);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_ordinary_expert_rbounds(const SPMAT *A, VEC *eig_values, int nb_eig, double lower_rbound, double upper_rbound )
{
   char range[] = "V";

   return sp_eigen_lapack_ordinary_expert_common(A, eig_values, nb_eig, range, 0, 0, lower_rbound, upper_rbound);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

static MAT *sp_eigen_lapack_generalized_expert_common  (const SPMAT *A, const SPMAT *B, VEC *eig_values,
                                             long nb_eig,
                                             char *range,
                                             long   lower_ibound, long   upper_ibound ,
                                             double lower_rbound, double upper_rbound )
{
   MAT* eig_vectors = NULL; /* the return value - not the "full" size hopefully */

   int i;
   long rc;

   MAT* lapack_banded_a;
   MAT* lapack_banded_b;

   MAT  *Q = MNULL;
   
   long m,n;

   double *dwork;
   long   *iwork;
   long   *ifail;

   long a_bandsize;
   long b_bandsize;
   
   long a_bandsize_p1;
   long b_bandsize_p1;

   long eig_found;

   double tol = 1.0e-8;
   
   
   /* check */
   if ( A == SMNULL ) error(E_NULL, "sp_eigen_lapack_generalized_expert");
   if ( B == SMNULL ) error(E_NULL, "sp_eigen_lapack_generalized_expert");

   if ( eig_values  == VNULL ) error(E_NULL, "sp_eigen_lapack_generalized_expert");

   /* allocate memory */
   switch(*range)
   {
      case 'A': eig_vectors = m_get(A->m  , A->n); break;
      case 'I': eig_vectors = m_get(nb_eig, A->n); break;
      case 'V': eig_vectors = m_get(A->m  , A->n); break; /* we don't know the size a-priori */

      default: error(E_UNKNOWN, "sp_eigen_lapack_ordinary_expert_common");
   }

   /* start */
   /*
   printf ( " A prof = %d \n",  sp_profile(A) );
   printf ( " A bandw = %d \n",  sp_bandw(A) );
   */

   m = A->m;
   n = A->n;

   Q = m_get(A->m, A->n);

   lapack_banded_a = spmat2lapacklowerbanded(A);
   lapack_banded_b = spmat2lapacklowerbanded(B);

   a_bandsize = lapack_banded_a->n -1 ;
   b_bandsize = lapack_banded_b->n -1 ;

   /*
   m_output(lapack_banded_b);
   m_output(lapack_banded_a);
   */

   printf( " LAPACK GENERALIZED EIGEN-PROBLEM (EXPERT): \n");
   printf( " A->lb = %ld \n", a_bandsize);   
   printf( " B->lb = %ld \n", b_bandsize);
   


   if (a_bandsize <  b_bandsize)
   {
      MAT* lapack_banded_tmp = m_get(lapack_banded_a->m, lapack_banded_a->n + (b_bandsize -  a_bandsize) );
      /* add a column to A ... because it must not be smaller than those of B ... */
      m_zero(lapack_banded_tmp);

      /* init lapack_banded_tmp with lapack_banded_a */
      lapack_banded_tmp =  m_move(lapack_banded_a, 0,0,lapack_banded_a->m,lapack_banded_a->n, lapack_banded_tmp, 0,0);

      M_FREE(lapack_banded_a);
      lapack_banded_a = lapack_banded_tmp;

      a_bandsize += (b_bandsize -  a_bandsize);
   }
   
   dwork     = calloc( sizeof(double), 7*A->m);
   iwork     = calloc( sizeof(long)  , 5*A->m);
   ifail     = calloc( sizeof(long)  , 1*A->m);   


   a_bandsize_p1 =  a_bandsize +1;
   b_bandsize_p1 =  b_bandsize +1;

   /* ------ solve the system Ax = k.Bx  ----- */
   
   dsbgvx_( "V", range, "L", &m , &a_bandsize, &b_bandsize,
            lapack_banded_a->base, &(a_bandsize_p1),
            lapack_banded_b->base, &(b_bandsize_p1),
            Q->base     , &m,
            &lower_rbound, &upper_rbound,   /* referenced if range = R" -> EIGENe_LAPACK_RBOUNDS */
            &lower_ibound, &upper_ibound,   /* the first n eigen_values & vectors : referenced if EIGENe_LAPACK_IBOUNDS */
            &tol, 
            &eig_found, eig_values->ve, eig_vectors->base, &m,
            dwork, iwork, ifail, &rc );

   if ( rc != 0 )
   {
      error(E_UNKNOWN, "sp_eigen_lapack_generalized_all");
   }
   else
   {
      printf("  ---> eig_found = %ld \n", eig_found);
   }

   /* ------ solve the system Ax = k.Bx  ----- */

   if ( strcmp(range, "I") == 0 )
   {
      /* fill-out the other eigenvalues */
      for (i=nb_eig; i<eig_values->dim; i++)
      {
         eig_values->ve[i] =  1.0e20;
      }
   }
   if ( strcmp(range, "V") == 0 )
   {
      /* fill-out the other eigenvalues */
      for (i=eig_found; i<eig_values->dim; i++)
      {
         eig_values->ve[i] = 1.0e20;
      }
   }

   /* set-up the eigenvectors */
   switch(*range)
   {
      case 'A': eig_vectors = m_transp(eig_vectors, eig_vectors); break;
      case 'I': eig_vectors = m_transp2(eig_vectors); break; /* because the matrix is MxN  with M != N */
      case 'V': eig_vectors = m_transp(eig_vectors,eig_vectors);
                eig_vectors = m_resize(eig_vectors, A->m, eig_found);
                break;

      default: error(E_UNKNOWN, "sp_eigen_lapack_ordinary_expert_common");
   }

   /* clean */
   M_FREE(Q);

   M_FREE(lapack_banded_a);
   M_FREE(lapack_banded_b);

   free(dwork);
   free(iwork);
   free(ifail);

   /* finito */
   return eig_vectors;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_generalized_expert_lower(const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig )
{
   char range[] = "I";

   long first_eig = 1;
   long last_eig  = 1 + nb_eig ;

   if ( nb_eig  < 1 )
   {
      error(E_NEG, "sp_eigen_lapack_generalized_expert_lower");
   }
   if ( last_eig > A->m )
   {
      last_eig = A->m;
      nb_eig   = A->m;

      warning(WARN_UNKNOWN, "sp_eigen_lapack_generalized_expert_lower");
   }

   return sp_eigen_lapack_generalized_expert_common(A, B, eig_values, nb_eig, range, first_eig, last_eig, 0.0, 0.0);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_generalized_expert_upper(const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig )
{
   char range[] = "I";

   long first_eig = A->m - nb_eig ;
   long last_eig  = A->m ;

   if ( nb_eig  < 1 )
   {
      error(E_NEG, "sp_eigen_lapack_generalized_expert_upper");
   }
   if ( first_eig < 1 )
   {
      first_eig = 1;
      nb_eig    = A->m;

      warning(WARN_UNKNOWN, "sp_eigen_lapack_generalized_expert_upper");
   }
   
   return sp_eigen_lapack_generalized_expert_common(A, B, eig_values, nb_eig, range, first_eig, last_eig, 0.0, 0.0);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_generalized_expert_ibounds(const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig, int lower_ibound, int upper_ibound )
{
   char range[] = "I";

   long first_eig = lower_ibound;
   long last_eig  = upper_ibound;

   if ( upper_ibound - lower_ibound  < 0 )
   {
      error(E_NEG, "sp_eigen_lapack_generalized_expert_ibounds");
   }

   return sp_eigen_lapack_generalized_expert_common(A, B, eig_values, upper_ibound - lower_ibound + 1, range, first_eig, last_eig, 0.0, 0.0);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT *sp_eigen_lapack_generalized_expert_rbounds(const SPMAT *A, const SPMAT *B, VEC *eig_values, int nb_eig, double lower_rbound, double upper_rbound )
{
   char range[] = "V";

   if ( lower_rbound - upper_rbound  < 0 )
   {
      double tmp = upper_rbound; /*swap*/
      upper_rbound = lower_rbound;
      lower_rbound = tmp;

      warning(WARN_UNKNOWN, "sp_eigen_lapack_generalized_expert_ibounds");
   }

   return sp_eigen_lapack_generalized_expert_common(A, B, eig_values, nb_eig, range, 0, 0, lower_rbound, upper_rbound);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

MAT* sp_eigen_lapack(const SPMAT *A, const SPMAT *B, VEC *eig_values, EIGENt_LAPACK_PROBLEM type, EIGENt_LAPACK_OPTION flag,
                     int nb_eig,
                     int    lower_ibound, int    upper_ibound ,
                     double lower_rbound, double upper_rbound )
{
   switch(type)
   {
      case EIGENe_LAPACK_ORDINARY:
      {
         switch(flag)
         {
            case EIGENe_LAPACK_LOWER:
               return sp_eigen_lapack_ordinary_expert_lower  (A, eig_values, nb_eig);
               break;
               
            case EIGENe_LAPACK_UPPER:
               return sp_eigen_lapack_ordinary_expert_upper  (A, eig_values, nb_eig);
               break;
               
            case EIGENe_LAPACK_IBOUNDS:
               return sp_eigen_lapack_ordinary_expert_ibounds(A, eig_values, nb_eig, lower_ibound, upper_ibound);
               break;
               
            case EIGENe_LAPACK_RBOUNDS:

               return sp_eigen_lapack_ordinary_expert_rbounds(A, eig_values, nb_eig, lower_rbound, upper_rbound);
               break;

            case EIGENe_LAPACK_ALL:

               return sp_eigen_lapack_ordinary_full          (A, eig_values);
               break;

            default:

               error(E_UNKNOWN, "sp_eigen_lapack");  return MNULL;
         }
      }
         
      case EIGENe_LAPACK_GENERALIZED:
      {
         switch(flag)
         {
            case EIGENe_LAPACK_LOWER:
               return sp_eigen_lapack_generalized_expert_lower  (A, B, eig_values, nb_eig);
               break;

            case EIGENe_LAPACK_UPPER:
               return sp_eigen_lapack_generalized_expert_upper  (A, B, eig_values, nb_eig);
               break;

            case EIGENe_LAPACK_IBOUNDS:
               return sp_eigen_lapack_generalized_expert_ibounds(A, B, eig_values, nb_eig, lower_ibound, upper_ibound);
               break;

            case EIGENe_LAPACK_RBOUNDS:

               return sp_eigen_lapack_generalized_expert_rbounds(A, B, eig_values, nb_eig, lower_rbound, upper_rbound);
               break;

            case EIGENe_LAPACK_ALL:

               return sp_eigen_lapack_generalized_full          (A, B, eig_values);
               break;

            default:

               error(E_UNKNOWN, "sp_eigen_lapack");
               return MNULL;
         }
      }

         
      default:
      {
         error(E_UNKNOWN, "sp_eigen_lapack");
         return MNULL;
      }
   }
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */
