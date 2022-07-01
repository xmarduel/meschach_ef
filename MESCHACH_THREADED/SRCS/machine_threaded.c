

#include	"machine.h"
#include        "matrix.h"

#include <pthread.h>
#define NUM_THREADS 1


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

typedef struct
{
   int thread_id; /* for debugging */

   Real *v1;
   Real *v2;

   int len;

   double in_prod_val;

   int start; /* for debugging */
   
} IN_PROD_PARTIAL;

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

typedef struct
{
   int thread_id; /* for debugging */

   Real *v1;
   Real *v2;
   Real *v3;

   int len;

   int start; /* for debugging */

} V_ADD_PARTIAL;

typedef V_ADD_PARTIAL V_SUB_PARTIAL;

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

typedef struct
{
   int thread_id; /* for debugging */

   Real *v1;
   Real *v2;

   double s;
   int len;

   int start; /* for debugging */

} SV_MLT_PARTIAL;

typedef SV_MLT_PARTIAL V_MLTADD_PARTIAL;

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void  init_threaded_data_for_ip(IN_PROD_PARTIAL *data, int nb_threads, int total_len, double *v1, double *v2)
{
   int i;

   int dim_1  = floor( total_len / nb_threads );
   int dim_2  = dim_1 + 1;

   int nb_threads_with_dim_2 = total_len % nb_threads;
   int nb_threads_with_dim_1 = nb_threads - nb_threads_with_dim_2 ;

   for (i=0; i<nb_threads_with_dim_2; i++)
   {
      data[i].thread_id = i;

      data[i].start = dim_2*i;

      data[i].v1 = v1 + data[i].start;
      data[i].v2 = v2 + data[i].start;

      data[i].len = dim_2  ;
   }

   for (i=nb_threads_with_dim_2; i<nb_threads_with_dim_2+nb_threads_with_dim_1; i++)
   {
      data[i].thread_id = i;

      data[i].start = nb_threads_with_dim_2*dim_2 + (i-nb_threads_with_dim_2)*dim_1;

      data[i].v1 = v1 + data[i].start;
      data[i].v2 = v2 + data[i].start;

      data[i].len = dim_1;
   }

   /*
   for (i=0; i<nb_threads; i++)
   {
      printf(" thread %d : start = %9d   len = %9d \n", data[i].thread_id, data[i].start, data[i].len);
   }
   */
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void  init_threaded_data_for_add(V_ADD_PARTIAL *data, int nb_threads, int total_len, double *v1, double *v2, double *v3)
{
   int i;

   int dim_1  = floor( total_len / nb_threads );
   int dim_2  = dim_1 + 1;

   int nb_threads_with_dim_2 = total_len % nb_threads;
   int nb_threads_with_dim_1 = nb_threads - nb_threads_with_dim_2 ;

   for (i=0; i<nb_threads_with_dim_2; i++)
   {
      data[i].thread_id = i;

      data[i].start = dim_2*i;

      data[i].v1 = v1 + data[i].start;
      data[i].v2 = v2 + data[i].start;
      data[i].v3 = v3 + data[i].start;

      data[i].len = dim_2  ;
   }

   for (i=nb_threads_with_dim_2; i<nb_threads_with_dim_2+nb_threads_with_dim_1; i++)
   {
      data[i].thread_id = i;

      data[i].start = nb_threads_with_dim_2*dim_2 + (i-nb_threads_with_dim_2)*dim_1;

      data[i].v1 = v1 + data[i].start;
      data[i].v2 = v2 + data[i].start;
      data[i].v3 = v3 + data[i].start;

      data[i].len = dim_1;
   }

   /*
    for (i=0; i<nb_threads; i++)
    {
       printf(" thread %d : start = %9d   len = %9d \n", data[i].thread_id, data[i].start, data[i].len);
    }
    */
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void  init_threaded_data_for_smlt(SV_MLT_PARTIAL *data, int nb_threads, int total_len, double *v1, double s, double *v2)
{
   int i;

   int dim_1  = floor( total_len / nb_threads );
   int dim_2  = dim_1 + 1;

   int nb_threads_with_dim_2 = total_len % nb_threads;
   int nb_threads_with_dim_1 = nb_threads - nb_threads_with_dim_2 ;

   for (i=0; i<nb_threads_with_dim_2; i++)
   {
      data[i].thread_id = i;

      data[i].start = dim_2*i;

      data[i].v1 = v1 + data[i].start;
      data[i].v2 = v2 + data[i].start;

      data[i].s   = s  ;
      data[i].len = dim_2  ;
   }

   for (i=nb_threads_with_dim_2; i<nb_threads_with_dim_2+nb_threads_with_dim_1; i++)
   {
      data[i].thread_id = i;

      data[i].start = nb_threads_with_dim_2*dim_2 + (i-nb_threads_with_dim_2)*dim_1;

      data[i].v1 = v1 + data[i].start;
      data[i].v2 = v2 + data[i].start;

      data[i].s   = s  ;
      data[i].len = dim_1;
   }

   /*
    for (i=0; i<nb_threads; i++)
    {
       printf(" thread %d : start = %9d   len = %9d \n", data[i].thread_id, data[i].start, data[i].len);
    }
    */
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void * routine_threaded_ip(void *pdata)
{
   Real *v1 = ((IN_PROD_PARTIAL*)pdata)->v1;
   Real *v2 = ((IN_PROD_PARTIAL*)pdata)->v2;

   int len  = ((IN_PROD_PARTIAL*)pdata)->len;

   ((IN_PROD_PARTIAL*)pdata)->in_prod_val = __ip__(v1,v2,len);

   return NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void * routine_threaded_add(void *pdata)
{
   Real *v1 = ((V_ADD_PARTIAL*)pdata)->v1;
   Real *v2 = ((V_ADD_PARTIAL*)pdata)->v2;
   Real *v3 = ((V_ADD_PARTIAL*)pdata)->v3;

   int len  = ((V_ADD_PARTIAL*)pdata)->len;

   __add__(v1,v2,v3,len);

   return NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void * routine_threaded_sub(void *pdata)
{
   Real *v1 = ((V_SUB_PARTIAL*)pdata)->v1;
   Real *v2 = ((V_SUB_PARTIAL*)pdata)->v2;
   Real *v3 = ((V_SUB_PARTIAL*)pdata)->v3;

   int len  = ((V_SUB_PARTIAL*)pdata)->len;

   __sub__(v1,v2,v3,len);

   return NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void * routine_threaded_smlt(void *pdata)
{
   Real *v1 = ((SV_MLT_PARTIAL*)pdata)->v1;
   Real *v2 = ((SV_MLT_PARTIAL*)pdata)->v2;

   double s = ((SV_MLT_PARTIAL*)pdata)->s;
   int len  = ((SV_MLT_PARTIAL*)pdata)->len;

   /*printf("thread %d begin \n", ((SV_MLT_PARTIAL*)pdata)->thread_id ); fflush(stdout);*/

   __smlt__(v1,s,v2,len);

   /*printf("thread %d finished \n", ((SV_MLT_PARTIAL*)pdata)->thread_id ); fflush(stdout);*/
   
   return NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void * routine_threaded_mltadd(void *pdata)
{
   Real *v1 = ((V_MLTADD_PARTIAL*)pdata)->v1;
   Real *v2 = ((V_MLTADD_PARTIAL*)pdata)->v2;

   double s = ((V_MLTADD_PARTIAL*)pdata)->s;
   int len  = ((V_MLTADD_PARTIAL*)pdata)->len;

   /*printf("thread %d begin \n", ((V_MLTADD_PARTIAL*)pdata)->thread_id ); fflush(stdout);*/

   __mltadd__(v1,v2,s,len);

   /*printf("thread %d finished \n", ((V_MLTADD_PARTIAL*)pdata)->thread_id ); fflush(stdout);*/

   return NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* __threaded_ip__  */
static  double	__threaded_ip__( double *dp1,  double *dp2, int len)
{
   double val = 0.0;
   pthread_attr_t attr;
   pthread_t threads[NUM_THREADS];
   IN_PROD_PARTIAL data[NUM_THREADS];
    
   int rc,t,status;

   init_threaded_data_for_ip(data, NUM_THREADS, len, dp1, dp2);

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0 ; t < NUM_THREADS ; t++)
   {
      rc = pthread_create(&threads[t], &attr , routine_threaded_ip , &data[t]);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
      }
   }

   for(t=0; t < NUM_THREADS; t++)
   {
      rc = pthread_join(threads[t], (void **)&status);
      if (rc)
      {
         printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
      }
   }

   for(t=0; t < NUM_THREADS; t++)
   {
      val += data[t].in_prod_val;
   }    

   /* Free attribute */
   pthread_attr_destroy(&attr);

   return val; 
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */


/* __threaded_add__  */
static  void	__threaded_add__( double *dp1,  double *dp2,  double *dp3,  int len)
{
   pthread_attr_t attr;
   pthread_t threads[NUM_THREADS];
   V_ADD_PARTIAL data[NUM_THREADS];

   int rc,t,status;

   init_threaded_data_for_add(data, NUM_THREADS, len, dp1, dp2, dp3);

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0 ; t < NUM_THREADS ; t++)
   {
      rc = pthread_create(&threads[t], &attr , routine_threaded_add , &data[t]);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
      }
   }

   for(t=0; t < NUM_THREADS; t++)
   {
      rc = pthread_join(threads[t], (void **)&status);
      if (rc)
      {
         printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
      }
   }

   /* Free attribute */
   pthread_attr_destroy(&attr);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* __threaded_sub__  */
static  void	__threaded_sub__( double *dp1,  double *dp2,  double *dp3,  int len)
{
   pthread_attr_t attr;
   pthread_t threads[NUM_THREADS];
   V_SUB_PARTIAL data[NUM_THREADS];

   int rc,t,status;

   init_threaded_data_for_add(data, NUM_THREADS, len, dp1, dp2, dp3);

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0 ; t < NUM_THREADS ; t++)
   {
      rc = pthread_create(&threads[t], &attr , routine_threaded_sub , &data[t]);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
      }
   }

   for(t=0; t < NUM_THREADS; t++)
   {
      rc = pthread_join(threads[t], (void **)&status);
      if (rc)
      {
         printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
      }
   }

   /* Free attribute */
   pthread_attr_destroy(&attr);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* __threaded_smlt__  */
static  void	__threaded_smlt__( double *dp1,  double s,  double *dp2,  int len)
{
   pthread_attr_t attr;
   pthread_t threads[NUM_THREADS];
   SV_MLT_PARTIAL data[NUM_THREADS];

   int rc,t,status;

   init_threaded_data_for_smlt(data, NUM_THREADS, len, dp1, s, dp2);

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0 ; t < NUM_THREADS ; t++)
   {
      rc = pthread_create(&threads[t], &attr , routine_threaded_smlt , &data[t]);

      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
      }
   }

   for(t=0; t < NUM_THREADS; t++)
   {
      rc = pthread_join(threads[t], (void **)&status);
      if (rc)
      {
         printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
      }
   }

   /* Free attribute */
   pthread_attr_destroy(&attr);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* __threaded_mltadd__  */
static  void	__threaded_mltadd__( double *dp1, double *dp2, double s, int len)
{
   pthread_attr_t attr;
   pthread_t threads[NUM_THREADS];
   V_MLTADD_PARTIAL data[NUM_THREADS];

   int rc,t,status;

   init_threaded_data_for_smlt(data, NUM_THREADS, len, dp1, s, dp2);

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

   for(t=0 ; t < NUM_THREADS ; t++)
   {
      rc = pthread_create(&threads[t], &attr , routine_threaded_mltadd , &data[t]);
      if (rc)
      {
         printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
      }
   }

   for(t=0; t < NUM_THREADS; t++)
   {
      rc = pthread_join(threads[t], (void **)&status);
      if (rc)
      {
         printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
      }
   }

   /* Free attribute */
   pthread_attr_destroy(&attr);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

extern double in_prod_threaded(VEC *v1, VEC *v2)
{
   if ( v1==(VEC *)NULL || v2==(VEC *)NULL )  error(E_NULL, "in_prod_threaded");
   if ( v1->dim != v2->dim )                  error(E_SIZES, "in_prod_threaded");
   
   return __threaded_ip__(v1->ve, v2->ve, v1->dim);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

extern VEC * v_add_threaded(VEC *v1, VEC *v2, VEC *out)
{
   if ( v1==(VEC *)NULL || v2==(VEC *)NULL )  error(E_NULL, "v_add");
   if ( v1->dim != v2->dim )                  error(E_SIZES, "v_add");

   if ( out==(VEC *)NULL || out->dim != v1->dim )
   {
      out = v_resize(out, v1->dim);
   }

   __threaded_add__(v1->ve, v2->ve, out->ve, (int)v1->dim);

   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

extern VEC * v_sub_threaded(VEC *v1, VEC *v2, VEC *out)
{
   if ( v1==(VEC *)NULL || v2==(VEC *)NULL )  error(E_NULL, "v_sub_threaded");
   if ( v1->dim != v2->dim )                  error(E_SIZES, "v_sub_threaded");

   if ( out==(VEC *)NULL || out->dim != v1->dim )
   {
      out = v_resize(out, v1->dim);
   }

   __threaded_sub__(v1->ve, v2->ve, out->ve, (int)v1->dim);

   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

extern VEC * sv_mlt_threaded(double s, VEC *v1, VEC *out)
{
   if ( v1==(VEC *)NULL )    error(E_NULL, "sv_mlt_threaded");

   if ( out==(VEC *)NULL || out->dim != v1->dim )
   {
      out = v_resize(out, v1->dim);
   }

   __threaded_smlt__(v1->ve, s, out->ve, (int)v1->dim);

   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* v_mltadd -- scalar/vector multiplication and addition -- out = v1 + scale.v2		*/
VEC	*v_mltadd_threaded(VEC *v1,VEC *v2,double scale,VEC *out)
{
   if ( v1==(VEC *)NULL || v2==(VEC *)NULL )   error(E_NULL , "v_mltadd_threaded");
   if ( v1->dim != v2->dim )                   error(E_SIZES, "v_mltadd_threaded");

   if ( scale == 0.0 )  return v_copy(v1,out);
   if ( scale == 1.0 )  return v_add_threaded(v1,v2,out);

   if ( v2 != out )
   {
      out = v_copy(v1,out);
      __threaded_mltadd__(out->ve,v2->ve,scale,(int)(v1->dim));
   }
   else
   {
      out = sv_mlt_threaded(scale,v2,out);
      out = v_add_threaded(v1,out,out);
   }

   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */