
//#include <stdlib.h>
//#include <stdio.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_factor.h"

#define SIZE 15000

int main(void)
{
   SPMAT   *A;
   VEC     *x, *y;
   int i;

   A = sp_get(SIZE,SIZE,3);

   for (i=0; i<SIZE; i++)
   {
      sp_set_val(A,i,i, 4.0);
   }
   for (i=0; i<SIZE-1; i++)
   {
      sp_set_val(A,i,i+1, -1.0 );
      sp_set_val(A,i+1,i, -1.0 );
   }


   x = v_get(SIZE);
   y = v_get(SIZE);

   for (i=0; i<SIZE; i++)
   {
      v_set_val(x,i, i   );
   }

   y = sp_mv_mlt(A, x, y); /* A.x = y */

   /*
   printf("x = ");
   v_output(x);
   printf("Ax = ");
   v_output(y);
   */

   x = v_zero(x);

   x = spCHresolution_spooles(A, y, x);

   /*
   printf("\nSolve x st Ax = y :");
   v_output(x);
   */

   printf("\nEND\n");

   /* end */
   return 0;
}
