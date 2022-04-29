#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"
#include "MESCHACH_ADDS/INCLUDES/gpskmesh.h"
#include "MESCHACH_ADDS/INCLUDES/amd_wrapper.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_factor.h"


/** \file sparse2_adds.c 
 *
 * extentions to MESCHACH LIBRARY for usual structs : solve Ax = b with band width reduction algorithm
 *
 */

/*
 * We make Ax= y -> B = P.A.P'  with P from sp_evalpermutation_bandwr
 *
 *               -> solve B.s = u  with u = P.y ( see spCHsolve_bandwr() )
 *
 *
 *               -> P.A.P'.s = P.y
 *               ->   A.P'.s = y
 *               ->   A.x    = y
 *
 *               -> x = P'.s 
 *
 */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT* sp_makepermutation(const SPMAT* A, const PERM* P, const PERM* INVP, SPMAT* PAPt)
{
   SPMAT* TMP;

   /* check */
   if ( !A    ) error(E_NULL, "sp_makepermutation");
   if ( !P    ) error(E_NULL, "sp_makepermutation");
   if ( !INVP ) error(E_NULL, "sp_makepermutation");
   if ( !PAPt ) error(E_NULL, "sp_makepermutation");

   if ( P->size != INVP->size ) error(E_SIZES,"sp_makepermutation");
   if ( P->size != A->m       ) error(E_SIZES,"sp_makepermutation");
   if ( P->size != A->n       ) error(E_SIZES,"sp_makepermutation");
   if ( PAPt->m != A->m       ) error(E_SIZES,"sp_makepermutation");
   if ( PAPt->n != A->n       ) error(E_SIZES,"sp_makepermutation");
   
   /* procceed */
   TMP    = sp_get(A->m, A->n, 10);
   
   px_sp_rows(P   , A  , TMP  );   /*  P * A       */
   px_sp_cols(INVP, TMP, PAPt );   /*  P * A * P'  */

   /* clean */
   SP_FREE(TMP);

   /* return */
   return PAPt;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void sp_evalpermutation_bandwr(const SPMAT* A, PERM* P, PERM* INVP, BANDWRt_METHOD method, BANDWRt_OPTION option, BANDWRt_MATRIXTYPE mtype)
{
   /* check */
   if ( !A    ) error(E_NULL, "sp_evalpermutation_bandwr");
   if ( !P    ) error(E_NULL, "sp_evalpermutation_bandwr");
   if ( !INVP ) error(E_NULL, "sp_evalpermutation_bandwr");

   if ( P->size != INVP->size ) error(E_SIZES,"sp_evalpermutation_bandwr");
   if ( P->size != A->m       ) error(E_SIZES,"sp_evalpermutation_bandwr");
   if ( P->size != A->n       ) error(E_SIZES,"sp_evalpermutation_bandwr");

   /* get the permutation */
   switch( method )
   {
      case BANDWRe_MESCHACH:  
      {
         switch( option )
         {
            case BANDWRe_BAND   : sp_bandwr_meschach(A, P, INVP, BANDWRe_MESCHACH_BAND); break;
            case BANDWRe_PROFILE: sp_bandwr_meschach(A, P, INVP, BANDWRe_MESCHACH_PROFILE); break;
            default:              sp_bandwr_meschach(A, P, INVP, BANDWRe_MESCHACH_BAND); break;
         }
      }
      case BANDWRe_SPOOLES:   
      {
         switch( mtype )
         {
            case BANDWRe_SYM:     sp_bandwr_spooles (A, P, INVP, BANDWRe_SPOOLES_SYM); break;
            case BANDWRe_NONSYM:  sp_bandwr_spooles (A, P, INVP, BANDWRe_SPOOLES_NONSYM); break;
            default:              sp_bandwr_spooles (A, P, INVP, BANDWRe_SPOOLES_NONSYM); break;
        }
      }
      case BANDWRe_AMD:
      {
         sp_bandwr_amd     (A, P, INVP, BANDWRe_NO_OPTION); break;
      }

      default:                error(E_UNKNOWN, "sp_evalpermutation_bandwr");
   }
   
   /* return */
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT* sp_dopermutationforbandwr(const SPMAT* A, PERM* P, PERM* INVP, BANDWRt_METHOD method, BANDWRt_OPTION option, BANDWRt_MATRIXTYPE mtype, SPMAT* PAPt )
{
   /* check */
   if ( !A    ) error(E_NULL, "sp_dopermutationforbandwr");
   if ( !P    ) error(E_NULL, "sp_dopermutationforbandwr");
   if ( !INVP ) error(E_NULL, "sp_dopermutationforbandwr");
   if ( !PAPt ) error(E_NULL, "sp_dopermutationforbandwr");
   
   if ( P->size != INVP->size ) error(E_SIZES,"sp_dopermutationforbandwr");
   if ( P->size != A->m       ) error(E_SIZES,"sp_dopermutationforbandwr");
   if ( P->size != A->n       ) error(E_SIZES,"sp_dopermutationforbandwr");
   if ( A->m != PAPt->n       ) error(E_SIZES,"sp_dopermutationforbandwr");
   if ( A->n != PAPt->n       ) error(E_SIZES,"sp_dopermutationforbandwr");
      
   /* get the permutation */
   sp_evalpermutation_bandwr(A, P, INVP, method, option, mtype);
   
   /* permute A */
   PAPt = sp_makepermutation(A, P, INVP, PAPt);

   /* return */
   return PAPt;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* spCHsolve_bandwr(const SPMAT *CH_BWR, const PERM *P, const PERM* INVP, const VEC* in, VEC *out)
{
   int NX;

   VEC* in_BWR;
   VEC* out_BWR;

   /* check */
   if ( !CH_BWR ) error(E_NULL, "spCHsolve_bandwr");
   if ( !P      ) error(E_NULL, "spCHsolve_bandwr");
   if ( !INVP   ) error(E_NULL, "spCHsolve_bandwr");

   if ( P->size != INVP->size ) error(E_SIZES,"spCHsolve_bandwr");
   if ( in->dim != out->dim   ) error(E_SIZES,"spCHsolve_bandwr");
   if ( in->dim != P->size    ) error(E_SIZES,"spCHsolve_bandwr");

   /* procceed */
   NX = CH_BWR->m;

   in_BWR  = v_get(NX);
   out_BWR = v_get(NX);


   /* the permutated matrix must be st.  P * A * P'  */

   in_BWR  = px_vec(P, in, in_BWR);                   /* u = P.y */

   out_BWR = spCHsolve(CH_BWR, in_BWR, out_BWR);      /* solve (P.A.Pt).s = u with (PAPt aleady factorized) */

   out = px_vec(INVP, out_BWR, out);                  /* x = Pt.s */

   /* the permutated matrix must be st.  P * A * P'  */


   /* clean */
   V_FREE(in_BWR);
   V_FREE(out_BWR);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* spLUsolve_bandwr(const SPMAT *LU_BWR, const PERM *LU_P, const PERM *P, const PERM* INVP, const VEC* in, VEC *out)
{
   int NX;

   VEC* in_BWR;
   VEC* out_BWR;

   /* check */
   if ( !LU_BWR ) error(E_NULL, "spLUsolve_bandwr");
   if ( !LU_P   ) error(E_NULL, "spLUsolve_bandwr");
   if ( !P      ) error(E_NULL, "spLUsolve_bandwr");
   if ( !INVP   ) error(E_NULL, "spLUsolve_bandwr");

   if ( P->size != INVP->size ) error(E_SIZES,"spLUsolve_bandwr");
   if ( in->dim != out->dim   ) error(E_SIZES,"spLUsolve_bandwr");
   if ( in->dim != P->size    ) error(E_SIZES,"spLUsolve_bandwr");

   /* procceed */
   NX = LU_BWR->m;

   in_BWR  = v_get(NX);
   out_BWR = v_get(NX);


   /* the permutated matrix must be st.  P * A * P'  = LU_BWR */

   in_BWR  = px_vec(P, in, in_BWR);                      /* u = P.y */        

   out_BWR = spLUsolve(LU_BWR, LU_P, in_BWR, out_BWR);   /* solve (P.A.Pt).s = u with (PAPt aleady factorized) */

   out = px_vec(INVP, out_BWR, out);                     /* x = Pt.s */      

   /* the permutated matrix must be st.  P * A * P'  */


   /* clean */
   V_FREE(in_BWR);
   V_FREE(out_BWR);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* spCHresolution_meschach(const SPMAT *A, const VEC* in, VEC *out)
{
   int NX;

   PERM  *P, *INVP;
   SPMAT *PAPt;

   /* check */
   if ( !A   ) error(E_NULL, "spCHresolution_meschach");
   if ( !in  ) error(E_NULL, "spCHresolution_meschach");
   if ( !out ) error(E_NULL, "spCHresolution_meschach");

   if ( in->dim != out->dim ) error(E_SIZES,"spCHresolution_meschach");
   if ( in->dim != A->m     ) error(E_SIZES,"spCHresolution_meschach");
   if ( in->dim != A->n     ) error(E_SIZES,"spCHresolution_meschach");

   /* procceed */
   NX = A->m;

   PAPt = sp_get(NX, NX, 10);

   P    = px_get(NX);
   INVP = px_get(NX);

   /* get A_BWR the matrix this re-ordered col&rows  A_BWR = P*A*P' */
   PAPt = sp_dopermutationforbandwr(A, P, INVP, BANDWRe_MESCHACH, BANDWRe_PROFILE, BANDWRe_SYM, PAPt );

   /* Cholesky factorisation */
   spCHfactor(PAPt);
   spCHsolve_bandwr(PAPt, P, INVP, in, out);

   /* clean */
   PX_FREE(P);
   PX_FREE(INVP);

   SP_FREE(PAPt);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* spCHresolution_amd(const SPMAT *A, const VEC* in, VEC *out)
{
   int NX;

   PERM  *P, *INVP;
   SPMAT *PAPt;

   /* check */
   if ( !A   ) error(E_NULL, "spCHresolution_amd");
   if ( !in  ) error(E_NULL, "spCHresolution_amd");
   if ( !out ) error(E_NULL, "spCHresolution_amd");

   if ( in->dim != out->dim ) error(E_SIZES,"spCHresolution_amd");
   if ( in->dim != A->m     ) error(E_SIZES,"spCHresolution_amd");
   if ( in->dim != A->n     ) error(E_SIZES,"spCHresolution_amd");

   /* procceed */
   NX = A->m;

   PAPt = sp_get(NX, NX, 10);

   P    = px_get(NX);
   INVP = px_get(NX);

   /* get A_BWR the matrix this re-ordered col&rows  A_BWR = P*A*P' */
   PAPt = sp_dopermutationforbandwr(A, P, INVP, BANDWRe_AMD, BANDWRe_NO_OPTION, BANDWRe_SYM, PAPt );

   /* Cholesky factorisation */
   spCHfactor(PAPt);
   spCHsolve_bandwr(PAPt, P, INVP, in, out);

   /* clean */
   PX_FREE(P);
   PX_FREE(INVP);

   SP_FREE(PAPt);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* spLUresolution_meschach(const SPMAT *A, const VEC* in, VEC *out)
{
   int NX;

   PERM *P;
   PERM *INVP;

   PERM *LU_P;
   
   SPMAT *PAPt;

   /* check */
   if ( !A    ) error(E_NULL, "spLUresolution_meschach");
   if ( !in   ) error(E_NULL, "spLUresolution_meschach");
   if ( !out  ) error(E_NULL, "spLUresolution_meschach");

   if ( in->dim != A->m )     error(E_SIZES,"spLUresolution_meschach");
   if ( in->dim != A->m )     error(E_SIZES,"spLUresolution_meschach");
   if ( in->dim != out->dim ) error(E_SIZES,"spLUresolution_meschach");

   /* procceed */
   NX = A->m;

   PAPt = sp_get(NX, NX, 10);

   P    = px_get(NX);
   INVP = px_get(NX);

   /* get A_BWR the matrix this re-ordered col&rows  LU_BWR = P*A*P' */
   PAPt = sp_dopermutationforbandwr(A, P, INVP, BANDWRe_MESCHACH, BANDWRe_PROFILE, BANDWRe_NONSYM, PAPt);

   LU_P = px_get(NX);
   /* LU factorisation */
   spLUfactor(PAPt, LU_P, 0.5);

   /* ans solve */
   spLUsolve_bandwr(PAPt, LU_P, P, INVP, in, out);

   /* clean */
   PX_FREE(P);
   PX_FREE(INVP);

   PX_FREE(LU_P);
   
   SP_FREE(PAPt);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC* spLUresolution_amd(const SPMAT *A, const VEC* in, VEC *out)
{
   int NX;

   PERM *P;
   PERM *INVP;

   PERM *LU_P;

   SPMAT *PAPt;

   /* check */
   if ( !A    ) error(E_NULL, "spLUresolution_amd");
   if ( !in   ) error(E_NULL, "spLUresolution_amd");
   if ( !out  ) error(E_NULL, "spLUresolution_amd");

   if ( in->dim != A->m )     error(E_SIZES,"spLUresolution_amd");
   if ( in->dim != A->m )     error(E_SIZES,"spLUresolution_amd");
   if ( in->dim != out->dim ) error(E_SIZES,"spLUresolution_amd");

   /* procceed */
   NX = A->m;

   PAPt = sp_get(NX, NX, 10);

   P    = px_get(NX);
   INVP = px_get(NX);

   /* get A_BWR the matrix this re-ordered col&rows  LU_BWR = P*A*P' */
   PAPt = sp_dopermutationforbandwr(A, P, INVP, BANDWRe_AMD, BANDWRe_NO_OPTION, BANDWRe_NONSYM, PAPt);

   LU_P = px_get(NX);
   /* LU factorisation */
   spLUfactor(PAPt, LU_P, 0.5);

   /* ans solve */
   spLUsolve_bandwr(PAPt, LU_P, P, INVP, in, out);

   /* clean */
   PX_FREE(P);
   PX_FREE(INVP);

   PX_FREE(LU_P);

   SP_FREE(PAPt);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

