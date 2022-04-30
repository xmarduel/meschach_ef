
#include "EXTERNAL_LIBS/amd/amd.h"

#include "MESCHACH_ADDS/INCLUDES/amd_wrapper.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/spmat2formats.h"

/** \file amd_wrapper.c
 *
 * Contains the function to call "amd" main function (band width reduction)
 *
 */

PERM* sp_bandwr_amd(const SPMAT *A, PERM* P, PERM *INVP, int type)
{
   int rc;

   VEC  *Values;
   IVEC *Ap;
   IVEC *Ai;

   Real Control[AMD_CONTROL];
   Real Info[AMD_INFO];

   int nnz;

   /*printf(" ... AMD START ... \n");*/

   /* tests NULL structures */
   if ( A      == (SPMAT *)NULL ) error(E_NULL,"sp_bandwr_amd");
   if ( P      == (PERM  *)NULL ) error(E_NULL,"sp_bandwr_amd");
   if ( INVP   == (PERM  *)NULL ) error(E_NULL,"sp_bandwr_amd");
   /* tests SIZE structures */


   nnz    = sp_nonzeros(A);
   Values = v_get(nnz);
   Ai     = iv_get(nnz);
   Ap     = iv_get(A->m+1);

   /* get column compressed storage */
   spmat2ccs(A, Values , Ai , Ap , 'C' );


   /*
   int amd_order (	    // returns 0 if OK, negative value if error 
      int n,		    // A is n-by-n.  n must be >= 0. 
      const int Ap [ ],	    // column pointers for A, of size n+1 
      const int Ai [ ],	    // row indices of A, of size nz = Ap [n] 
      int P [ ],	    // output permutation, of size n 
      double Control [ ],    // input Control settings, of size AMD_CONTROL 
      double Info [ ]	    // output Info statistics, of size AMD_INFO 
   */

   rc = amd_order(A->m, Ap->ive, Ai->ive, P->pe, NULL, NULL);

   /* set INVP */
   INVP = px_inv(P, INVP);

   /* clean */
   IV_FREE(Ap);
   IV_FREE(Ai);
   V_FREE(Values);

   /*printf(" ... AMD END ... \n");*/
   /* end */
   return P;
}

