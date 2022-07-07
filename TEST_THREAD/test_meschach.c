
#include <time.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_THREADED/INCLUDES/machine_threaded.h"


int main (int argc, char *argv[])
{
   int LOOP = 5;
   int DIM = 100000000;

   VEC* v1 = v_get(DIM);
   VEC* v2 = v_get(DIM);

   printf("sizeof long double = %ld\n", sizeof(long double));
   printf("sizeof      double = %ld\n", sizeof(double));
   printf("sizeof      float  = %ld\n", sizeof(float));
   printf("sizeof      void*  = %ld\n", sizeof(void*));
   printf("sizeof long long   = %ld\n", sizeof(long long));
   printf("sizeof      long   = %ld\n", sizeof(long));
   printf("sizeof      int    = %ld\n", sizeof(int));
   
   v1 = v_ones(v1);
   sv_mlt(2.0, v1, v1);

   v2 = v_ones(v2);
   sv_mlt(2.0, v2, v2);

   fprintf(stdout, "start in_prod ...\n");

   int t1 = time(NULL);

   for (int i = 0; i < LOOP; i++)
   {
      double ip = in_prod(v1, v2);
      printf("NO THREAD: ip = %f\n", ip);
   }

   int t2 = time(NULL);


   printf("time (in_prod) = %d\n\n\n", t2-t1);

   // ----------------------------------------

   t1 = time(NULL);

   for (int i = 0; i < LOOP; i++)
   {
      double ip = in_prod_threaded(v1, v2);
      printf("WITH THREADS: ip = %f\n", ip);
   }

   t2 = time(NULL);

   printf("time (in_prod_threaded) = %d\n", t2-t1);


   V_FREE(v1);
   V_FREE(v2);

   return 1;
}
