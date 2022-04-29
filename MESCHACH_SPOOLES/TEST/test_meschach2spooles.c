

#include "meschach2spooles.h"

/*#include <meschach_plus.h>*/


int main()
{
   VEC *v1,*v3;
   DV *v2;

   MAT *M1, *M3;
   DenseMtx *M2;

   SPMAT *A1, *A3;
   InpMtx *A2;

   int size = 5;

   /*---------------------------------- */
   v1 = v_get(size);
   v1 = v_ones(v1);

   v_foutput(stdout, v1);

   v2 = v_meschach_to_spooles(v1);
   DV_writeForHumanEye(v2, stdout);

   v2->vec[2] = 2.2;
   DV_writeForHumanEye(v2, stdout);

   v3 = v_spooles_to_meschach(v2);
   v_foutput(stdout, v3);

   /*---------------------------------- */
   M1 = m_get(size,size-2);
   M1 = m_ones(M1);

   m_foutput(stdout, M1);
   
   M2 = m_meschach_to_spooles(M1);
   DenseMtx_writeForHumanEye(M2, stdout);

   DenseMtx_setRealEntry(M2, 2,2, 2.2);
   DenseMtx_setRealEntry(M2, 3,1, 3.3);
   DenseMtx_writeForHumanEye(M2, stdout);

   M3 = m_spooles_to_meschach(M2);
   m_foutput(stdout, M3);

   /*---------------------------------- */

   A1 = sp_get(size, size, 2);
   A1 = sp_tridiag(A1, -1.0, 4.0,-1.0);

   sp_foutput(stdout, A1);

   A2 = sp_meschach_to_spooles(A1);
   InpMtx_writeForHumanEye(A2, stdout);

   A3 = sp_spooles_to_meschach(A2);
   sp_foutput(stdout, A3);

   /*---------------------------------- */
   
   return EXIT_SUCCESS;
}