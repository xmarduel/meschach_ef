
#include "spooles/LinSol/Bridge.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper.h"
#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_factor.h"

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

static DenseMtx* spCHresolution_spooles1(InpMtx *A, DenseMtx *X, DenseMtx *Y);
static DenseMtx* spLUresolution_spooles1(InpMtx *A, DenseMtx *X, DenseMtx *Y);

static DenseMtx* spXXresolution_spooles1(InpMtx *A, DenseMtx *X, DenseMtx *Y, BANDWRt_SPOOLES_OPTION type);

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

static DenseMtx* spCHresolution_spooles1(InpMtx *A, DenseMtx *x, DenseMtx *y)
{
   return spXXresolution_spooles1(A, x, y, SPOOLES_SYMMETRIC);
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

static DenseMtx* spLUresolution_spooles1(InpMtx *A, DenseMtx *x, DenseMtx *y)
{
   return spXXresolution_spooles1(A, x, y, SPOOLES_NONSYMMETRIC);
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

static DenseMtx* spXXresolution_spooles1(InpMtx *A, DenseMtx *X, DenseMtx *Y, BANDWRt_SPOOLES_OPTION type)
{
   Bridge   *Wrapp;

   int m,n;
   int X_ERROR;
   int PERMUTE_YES = 1;
   int rc;

   /* check arguments */
   if ( A == NULL ) error(E_NULL, "spXXresolution_spooles1");
   if ( X == NULL ) error(E_NULL, "spXXresolution_spooles1");
   if ( Y == NULL ) error(E_NULL, "spXXresolution_spooles1");

   DenseMtx_dimensions (X,&m,&n);

   Wrapp = Bridge_new();

   /* set parametrers for Bridge struct */
   rc = Bridge_setMatrixParams(Wrapp, m, SPOOLES_REAL, type);

   /* perform permutation */
   rc = Bridge_setup(Wrapp, A);

   /* factor (with permutation) */
   rc = Bridge_factor(Wrapp, A, PERMUTE_YES, &X_ERROR);

   /* and solve (with permutation) */
   rc = Bridge_solve(Wrapp, PERMUTE_YES, X, Y);

   /* clean mem */
   rc = Bridge_free(Wrapp);

   /* return */
   return X;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

VEC* spCHresolution_spooles(const SPMAT *A, const VEC *in, VEC *out)
{
   DenseMtx *x;
   DenseMtx *y;

   InpMtx *S;

   if ( A   == NULL ) error(E_NULL, "spCHresolution_spooles");
   if ( in  == NULL ) error(E_NULL, "spCHresolution_spooles");
   if ( out == NULL ) error(E_NULL, "spCHresolution_spooles");

   x = v_meschach_to_m_spooles(in, NULL);
   y = v_meschach_to_m_spooles(out, NULL);

   S = s_meschach_to_s_spooles_sym(A, NULL);

   /* main routine */
   y = spCHresolution_spooles1(S, y, x);
   /* main routine */

   out = m_spooles_to_v_meschach(y, out);

   /* clean */
   InpMtx_free(S);
   DenseMtx_free(x);
   DenseMtx_free(y);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

VEC* spLUresolution_spooles(const SPMAT *A, const VEC *in, VEC *out)
{
   DenseMtx *x;
   DenseMtx *y;

   InpMtx *S;

   if ( A   == NULL ) error(E_NULL, "spLUresolution_spooles");
   if ( in  == NULL ) error(E_NULL, "spLUresolution_spooles");
   if ( out == NULL ) error(E_NULL, "spLUresolution_spooles");

   x = v_meschach_to_m_spooles(in, NULL);
   y = v_meschach_to_m_spooles(out, NULL);

   S = s_meschach_to_s_spooles(A, NULL);

   /* main routine */
   y = spLUresolution_spooles1(S, y, x);
   /* main routine */

   out = m_spooles_to_v_meschach(y, out);

   /* clean */
   InpMtx_free(S);
   DenseMtx_free(x);
   DenseMtx_free(y);

   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

PERM* sp_bandwr_spooles(const SPMAT *A, PERM *P, PERM *INVP, BANDWRt_SPOOLES_OPTION type)
{
   InpMtx   *S;
   Bridge   *Wrapp;
   int i,rc;

   /* check arguments */ 
   if ( A    == NULL ) error(E_NULL, "sp_bandwr_spooles");
   if ( P    == NULL ) error(E_NULL, "sp_bandwr_spooles");
   if ( INVP == NULL ) error(E_NULL, "sp_bandwr_spooles");

   S = s_meschach_to_s_spooles(A, NULL);

   Wrapp = Bridge_new();

   rc = Bridge_setMatrixParams(Wrapp, A->m, SPOOLES_REAL, SPOOLES_NONSYMMETRIC);

   /* perform permutation */
   rc = Bridge_setup(Wrapp, S);

   /* get permutation in meschach structs */
   for (i=0; i<A->m; i++)
   {
      INVP->pe[i] = Wrapp->oldToNewIV->vec[i];
      P->pe[i]    = Wrapp->newToOldIV->vec[i];
   }

   /* clean mem */
   rc = Bridge_free(Wrapp);
   InpMtx_free(S);
   
   /* return */
   return P;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

