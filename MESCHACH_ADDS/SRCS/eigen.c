
#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"
#include "MESCHACH_ADDS/INCLUDES/gpskmesh.h"
#include "MESCHACH_ADDS/INCLUDES/amd_wrapper.h"
#include "MESCHACH_ADDS/INCLUDES/eigen.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_eigen.h"
#include "MESCHACH_LAPACK/INCLUDES/lapack_eigen.h"

/** \file eigen.c
 *
 * Contains the function to solve eigenvalues problems
 *
 */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

EIGENt_PARAMS *eig_params_get(void)
{
   EIGENt_PARAMS *eig_param = (EIGENt_PARAMS*)malloc( sizeof(EIGENt_PARAMS) );

   /* init */
   eig_param->problem = EIGENe_ORDINARY_SYMMETRIC;
   eig_param->option = EIGENe_ALL;

   eig_param->lower_ibound = 0;
   eig_param->upper_ibound = 1;

   eig_param->lower_rbound = 0.0;
   eig_param->upper_rbound = 1.0e20;

   eig_param->nb_eig = 1;
   
   /* finito*/
   return eig_param;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int eig_params_set_problem (EIGENt_PARAMS *eig_param, EIGENt_PROBLEM problem)
{
   eig_param->problem = problem;

   return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int eig_params_set_option (EIGENt_PARAMS *eig_param, EIGENt_OPTION option)
{
   eig_param->option = option;

   return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int eig_params_set_ibounds(EIGENt_PARAMS *eig_param, int lower_bound, int upper_bound)
{
   eig_param->lower_ibound = lower_bound;
   eig_param->upper_ibound = upper_bound;
   
   return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int eig_params_set_rbounds(EIGENt_PARAMS *eig_param, double lower_bound, double upper_bound)
{
   eig_param->lower_rbound = lower_bound;
   eig_param->upper_rbound = upper_bound;
   
   return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int eig_params_set_nb_eigv(EIGENt_PARAMS *eig_param, int nb_eigv)
{
   eig_param->nb_eig = nb_eigv;
   
   return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int eig_params_free(EIGENt_PARAMS *eig_param)
{
   free(eig_param);
   eig_param = NULL;
   
   return 0;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */


MAT  *sp_eigen(const SPMAT *A, const SPMAT *B, VEC *eigen_values, EIGENt_METHOD method, EIGENt_PARAMS *eig_params)
{
   MAT *eigen_vectors = NULL;  /* the return value */

   switch(method)
   {
      case EIGENe_MESCHACH:
      {
         PERM *P;
         PERM *INVP;

         SPMAT *PAPt;
         SPMAT *PBPt;

         VEC* tmp1;
         VEC* tmp2;

         switch (eig_params->problem)
         {
            case EIGENe_ORDINARY_SYMMETRIC:
               return eigen_vectors = sp_eigen_meschach_ordinary(A, eigen_values);
               break;
               
            case EIGENe_GENERALIZED_SYMMETRIC_1:
            case EIGENe_GENERALIZED_SYMMETRIC_2:

               /* do permutations to reduce the band width par default */
               P    = px_get(A->m);
               INVP = px_get(A->m);

               PAPt = sp_get(A->m, A->n, 10);
               PBPt = sp_get(B->m, B->n, 10);


               sp_evalpermutation_bandwr(B , P, INVP, BANDWRe_MESCHACH, BANDWRe_BAND, BANDWRe_NONSYM);

               sp_makepermutation(B, P, INVP, PBPt);
               sp_makepermutation(A, P, INVP, PAPt);

               eigen_vectors = sp_eigen_meschach_generalized(PAPt, PBPt, eigen_values);
   
               if ( eigen_vectors != MNULL )
               {
                  int i;
                  tmp1 = v_get(A->m);
                  tmp2 = v_get(A->m);

                  for (i=0; i<eigen_vectors->n; i++) /* re-order the columns of the eigen_vectors */
                  {
                     tmp1 = get_col(eigen_vectors, i, tmp1);

                     px_vec(INVP, tmp1, tmp2);

                     set_col(eigen_vectors, i, tmp2);
                  }

                  V_FREE(tmp1);
                  V_FREE(tmp2);
               }

               /* clean */
               SP_FREE(PAPt);
               SP_FREE(PBPt);

               PX_FREE(P);
               PX_FREE(INVP);

               return eigen_vectors;
               break;
               
            default:
               error(E_UNKNOWN, "sp_eigen");
               return MNULL;
         }
      }
         
      case EIGENe_SPOOLES:
      {
         EIGENt_SPOOLES_PROBLEM prb = EIGENe_SPOOLES_ORDINARY_SYMMETRIC;
         EIGENt_SPOOLES_OPTION  opt = EIGENe_SPOOLES_ALL;

         switch(eig_params->problem)
         {
            case EIGENe_ORDINARY_SYMMETRIC     : prb = EIGENe_SPOOLES_ORDINARY_SYMMETRIC; break;
            case EIGENe_GENERALIZED_SYMMETRIC_1: prb = EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_1; break;
            case EIGENe_GENERALIZED_SYMMETRIC_2: prb = EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_2; break;
            default:                             error(E_UNKNOWN, "sp_eigen");
         }
      
         switch(eig_params->option)
         {
            case EIGENe_LOWER     : opt = EIGENe_SPOOLES_LOWER; break;
            case EIGENe_UPPER     : opt = EIGENe_SPOOLES_UPPER; break;
            case EIGENe_NEAREST_1 : opt = EIGENe_SPOOLES_NEAREST_1; break;
            case EIGENe_NEAREST_2 : opt = EIGENe_SPOOLES_NEAREST_2; break;
            case EIGENe_IBOUNDS   : opt = EIGENe_SPOOLES_IBOUNDS; break;
            case EIGENe_RBOUNDS   : opt = EIGENe_SPOOLES_RBOUNDS; break;
            case EIGENe_ALL       : opt = EIGENe_SPOOLES_ALL; break;
            default:                error(E_UNKNOWN, "sp_eigen");
         }

         return eigen_vectors = sp_eigen_spooles(A, B, eigen_values, prb, opt);
            
      }
         
      case EIGENe_LAPACK:
      {
         EIGENt_LAPACK_PROBLEM prb = EIGENe_LAPACK_ORDINARY;
         EIGENt_LAPACK_OPTION  opt = EIGENe_LAPACK_ALL;

         PERM *P;
         PERM *INVP;

         SPMAT *PAPt;
         SPMAT *PBPt;

         VEC* tmp1;
         VEC* tmp2;
         
         switch(eig_params->problem)
         {
            case EIGENe_ORDINARY_SYMMETRIC     : prb = EIGENe_LAPACK_ORDINARY; break;
            case EIGENe_GENERALIZED_SYMMETRIC_1: prb = EIGENe_LAPACK_GENERALIZED; break;
            case EIGENe_GENERALIZED_SYMMETRIC_2: error(E_UNKNOWN, "sp_eigen");
            default:                             error(E_UNKNOWN, "sp_eigen");
         }

         switch(eig_params->option)
         {
            case EIGENe_LOWER     : opt = EIGENe_LAPACK_LOWER; break;
            case EIGENe_UPPER     : opt = EIGENe_LAPACK_UPPER; break;
            case EIGENe_NEAREST_1 : error(E_UNKNOWN, "sp_eigen"); break;
            case EIGENe_NEAREST_2 : error(E_UNKNOWN, "sp_eigen"); break;
            case EIGENe_IBOUNDS   : opt = EIGENe_LAPACK_IBOUNDS; break;
            case EIGENe_RBOUNDS   : opt = EIGENe_LAPACK_RBOUNDS; break;
            case EIGENe_ALL       : opt = EIGENe_LAPACK_ALL; break;
            default:                error(E_UNKNOWN, "sp_eigen");
         }

         /* do permutations to reduce the band width par default */
         P    = px_get(A->m);
         INVP = px_get(A->m);

         PAPt = sp_get(A->m,A->n, 10);
         PBPt = sp_get(B->m,B->n, 10);

         
         sp_evalpermutation_bandwr(B , P, INVP, BANDWRe_MESCHACH, BANDWRe_BAND, BANDWRe_NONSYM);

         sp_makepermutation    (B , P, INVP, PBPt);
         sp_makepermutation    (A , P, INVP, PAPt);

         eigen_vectors = sp_eigen_lapack(PAPt, PBPt, eigen_values, prb, opt,
                                         eig_params->nb_eig,
                                         eig_params->lower_ibound, eig_params->upper_ibound,
                                         eig_params->lower_rbound, eig_params->upper_rbound);

         if ( eigen_vectors != MNULL )
         {
            int i;
            tmp1 = v_get(A->m);
            tmp2 = v_get(A->m);

            for (i=0; i<eigen_vectors->n; i++) /* re-order the columns of the eigen_vectors */
            {
               tmp1 = get_col(eigen_vectors, i, tmp1);

               px_vec(INVP, tmp1, tmp2);
            
               set_col(eigen_vectors, i, tmp2);
            }

            V_FREE(tmp1);
            V_FREE(tmp2);
         }

         /* clean */
         SP_FREE(PAPt);
         SP_FREE(PBPt);

         PX_FREE(P);
         PX_FREE(INVP);
         
         /* finito */
         return eigen_vectors;
      }
         
      default:
      {
         error(E_UNKNOWN, "sp_eigen");
         return MNULL;
      }
   }
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *eigen_ordinary(const MAT *A, VEC *eigen_values)
{
   MAT *eigen_vectors; /* the return value */
   
   int NBSOMM;

   MAT *T;
   MAT *Q;

   VEC *eigen_values_im;
   MAT *eigen_vectors_im;
   
   /* check */
   if ( A == MNULL ) error(E_NULL, "eigen_simple");

   if ( eigen_values == VNULL ) error(E_NULL, "eigen_simple");

   NBSOMM = A->m;

   /* ------ solve the system Ax = k.x  ----- */

   T = m_get(NBSOMM, NBSOMM);
   Q = m_get(NBSOMM, NBSOMM);

   T = m_copy((MAT*)A,T);

   /* schur form X = Q.T.Q' */
   schur(T,Q);


   eigen_values_im  = v_get(NBSOMM);
   /* eigenvalues */
   schur_evals(T, eigen_values, eigen_values_im);


   eigen_vectors    = m_get(NBSOMM,NBSOMM);
   eigen_vectors_im = m_get(NBSOMM,NBSOMM);
   /* k-th eigenvector is k-th colunm of (X_re + i.X_im) */
   schur_vecs(T,Q, eigen_vectors, eigen_vectors_im);

   /* clean */
   V_FREE(eigen_values_im);
   M_FREE(eigen_vectors_im);

   M_FREE(T);
   M_FREE(Q);

   /* finito */
   return eigen_vectors;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *sp_eigen_meschach_ordinary(const SPMAT *A, VEC *eigen_values)
{
   MAT *eigen_vectors; /* the return value */
   PERM *PX, *INVPX;
   MAT  *TMP;
   
   MAT *A_;
   
   /* check */
   if ( A == SMNULL ) error(E_NULL, "sp_eigen_simple");
   
   if ( eigen_values  == VNULL ) error(E_NULL, "sp_eigen_simple");
   
   A_ = sp_m2dense((SPMAT*)A,MNULL);
   
   eigen_vectors = eigen_ordinary(A_, eigen_values);
   
   
   PX    = px_get(eigen_vectors->m);
   INVPX = px_get(eigen_vectors->m);
   TMP   = m_get(eigen_vectors->m, eigen_vectors->n);
   /* re-order */
   eigen_values  = v_sort(eigen_values, PX); /* the eig_values */
   
   INVPX = px_inv(PX, INVPX);
   TMP = px_cols(INVPX, eigen_vectors, TMP); /* and the eig_vectors */
   eigen_vectors = m_copy(TMP, eigen_vectors);
   
   /* clean */
   PX_FREE(PX);
   PX_FREE(INVPX);
   M_FREE(TMP);
   
   M_FREE(A_);
   
   /* finito */
   return eigen_vectors;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *sp_eigen_meschach_generalized(const SPMAT *A, const SPMAT *B, VEC *eigen_values)
{
   MAT *eigen_vectors; /* the return value */

   PERM *PX, *INVPX;
   MAT  *TMP;

   SPMAT *LLT; /* cholesky fact. of B */
      
   MAT *C;

   /* check */
   if ( A == SMNULL ) error(E_NULL, "sp_eigen_generalized");
   if ( B == SMNULL ) error(E_NULL, "sp_eigen_generalized");

   if ( eigen_values  == VNULL ) error(E_NULL, "sp_eigen_generalized");

   /* ------ calculate C = inv(L).A.inv(Lt)  ----- LLT Cholesky factorisation of B ---  */

   LLT = sp_copy2((SPMAT*)B, SMNULL);
   spCHfactor(LLT);

   C = m_sp_mmm_invL_A_invLt(LLT, A, NULL);
            
   /* process */
   eigen_vectors = eigen_ordinary(C, eigen_values);

   /* get the true eigen_vectors Lt.x = y ( we have y in matrix eigen_vectors) */
   eigen_vectors = m_spLTsolve_vectorset(LLT, eigen_vectors);

   /* re-order */
   PX    = px_get(eigen_vectors->m);
   INVPX = px_get(eigen_vectors->m);
   TMP   = m_get(eigen_vectors->m, eigen_vectors->n);
   /* re-order now */
   eigen_values  = v_sort(eigen_values, PX); /* the eig_values */

   INVPX = px_inv(PX, INVPX);
   TMP = px_cols(INVPX, eigen_vectors, TMP); /* and the eig_vectors */
   eigen_vectors = m_copy(TMP, eigen_vectors);

   /* clean */
   PX_FREE(PX);
   PX_FREE(INVPX);
   M_FREE(TMP);

   /* clean */
   SP_FREE(LLT);
   M_FREE(C);
   
   /* finito */
   return eigen_vectors;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

