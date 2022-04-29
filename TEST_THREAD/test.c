#include <pthread.h>
#include <stdio.h>
#include <sys/malloc.h>

/*
The following structure contains the necessary information
 to allow the function "dotprod" to access its input data and
 place its output into the structure.  This structure is
 unchanged from the sequential version.
 */

typedef struct
{
   double      *a;
   double      *b;
   double     sum;
   int     veclen;
} DOTDATA;

/* Define globally accessible variables and a mutex */

#define NUMTHRDS 1
#define VECLEN 4000000
DOTDATA dotstr;
pthread_t callThd[NUMTHRDS];
pthread_mutex_t mutexsum;

/*
 The function dotprod is activated when the thread is created.
 All input to this routine is obtained from a structure
 of type DOTDATA and all output from this function is written into
 this structure. The benefit of this approach is apparent for the
 multi-threaded program: when a thread is created we pass a single
 argument to the activated function - typically this argument
 is a thread number. All  the other information required by the
 function is accessed from the globally accessible structure.
 */

void *dotprod(void *arg)
{

   /* Define and use local variables for convenience */

   int i,j, start, end, offset, len ;
   double mysum, *x, *y;
   offset = (int)arg;

   len = dotstr.veclen;
   start = offset*len;
   end   = start + len;
   x = dotstr.a;
   y = dotstr.b;

   printf("start thread %d... \n", pthread_self()); fflush(stdout);
   /*
    Perform the dot product and assign result
    to the appropriate variable in the structure.
    */

   mysum = 0;
   for (j=0; j<200; j++)
   for (i=start; i < end ; i++)
   {
      mysum += (x[i] * y[i]);
   }

   /*
    Lock a mutex prior to updating the value in the shared
    structure, and unlock it upon updating.
    */
   pthread_mutex_lock (&mutexsum);
   dotstr.sum += mysum;
   pthread_mutex_unlock (&mutexsum);

   pthread_exit((void*) 0);
}

/*
The main program creates threads which do all the work and then
 print out result upon completion. Before creating the threads,
 the input data is created. Since all threads update a shared structure,
 we need a mutex for mutual exclusion. The main thread needs to wait for
 all threads to complete, it waits for each one of the threads. We specify
 a thread attribute value that allow the main thread to join with the
 threads it creates. Note also that we free up handles  when they are
 no longer needed.
 */

int main (int argc, char *argv[])
{
   time_t t1, t2;
   int i;
   double *a, *b;
   int status;
   pthread_attr_t attr;
   int indices[NUMTHRDS] = {0,1,2,3};
  
   printf("start ... \n"); fflush(stdout);

   /* Assign storage and initialize values */
   a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
   b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));

   for (i=0; i < VECLEN*NUMTHRDS; i++)
   {
      a[i]=1;
      b[i]=a[i];
   }

   dotstr.veclen = VECLEN;
   dotstr.a = a;
   dotstr.b = b;
   dotstr.sum=0;

   pthread_mutex_init(&mutexsum, NULL);

   /* Create threads to perform the dotproduct  */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   t1 = time(NULL);

   for(i=0;i < NUMTHRDS;i++)
   {
      /*
      Each thread works on a different set of data.
       The offset is specified by 'i'. The size of
       the data for each thread is indicated by VECLEN.
       */
      pthread_create( &callThd[i], &attr, dotprod, (void *)indices[i]);
   }

   pthread_attr_destroy(&attr);

   /* Wait on the other threads */
   for(i=0;i < NUMTHRDS;i++)
   {
      pthread_join( callThd[i], (void **)&status);
   }

   t2 = time(NULL);

   /* After joining, print out the results and cleanup */
   printf ("Sum =  %f -- time = %lf \n", dotstr.sum, difftime(t2,t1));
   free (a);
   free (b);
   pthread_mutex_destroy(&mutexsum);
   pthread_exit(NULL);
}
