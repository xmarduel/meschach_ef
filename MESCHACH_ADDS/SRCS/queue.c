

/** \file queue.c
 *
 * Contains the structures+routine for synchronization for plotting in an other thread
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "MESCHACH_ADDS/INCLUDES/queue.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

SVQueue * svqueue_new( CLONE_FUNC clone , FREED_FUNC freed)
{
   SVQueue *q  = (SVQueue*)malloc( sizeof(SVQueue) );

   q->mutex    = (pthread_mutex_t*)malloc( sizeof(pthread_mutex_t) );
   q->condi    = (pthread_cond_t*)malloc( sizeof(pthread_cond_t) );

   pthread_mutex_init(q->mutex , NULL);
   pthread_cond_init (q->condi , NULL);

   q->data = NULL; /* no data at creation */
   
   q->clone = clone;
   q->freed = freed;   
   
   q->xset = xset; /* the standart set function */
   q->xget = xget; /* the standart get function */

   q->xset_with_funcs = xset_with_funcs; /* to set data + new functions xset/xget */
   
   return q;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xset(SVQueue* q, void *data)
{
   pthread_mutex_lock(q->mutex);

   {
      /* printf("\nxset : new data Ox%lx\n", data); fflush(stdout); usleep(100); */

      while( q->data != NULL)
      {
         /* printf("xset : waiting for free slot...\n"); fflush(stdout); */
         pthread_cond_wait(q->condi, q->mutex);
         /* printf("xset : wait finished\n"); fflush(stdout); */
      }

      q->data = q->clone(data);
	  
      /* printf("xset : (cloned) data in queue Ox%lx\n", q->data); fflush(stdout); */
      
		pthread_mutex_unlock(q->mutex);
   }

	pthread_cond_signal(q->condi);
	
   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xget(SVQueue* q)
{
   void * mydata = NULL;

   pthread_mutex_lock(q->mutex);

   {
      /* printf("\txget\n"); fflush(stdout); */
      
      while( q->data == NULL )
      {
         /* printf("\txget : waiting for slot...\n"); fflush(stdout); */
         pthread_cond_wait(q->condi, q->mutex);
         /* printf("\txget : waiting finished\n"); fflush(stdout); */
      }

      /* printf("\txget : got Ox%lx\n", q->data); fflush(stdout); */
      
      mydata = q->data;
      
      q->data = NULL;
   
      /* printf("\txget : queue has free slot\n"); fflush(stdout); */
   
		pthread_mutex_unlock(q->mutex);
   }
   
	pthread_cond_signal(q->condi);
	
   return mydata;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xset_nowait(SVQueue* q, void *data)
{
   pthread_mutex_lock(q->mutex);

   {
      q->data = q->clone(data);

      pthread_cond_signal(q->condi);
   }

   pthread_mutex_unlock(q->mutex);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xget_nowait(SVQueue* q)
{
   void * mydata = NULL;

   pthread_mutex_lock(q->mutex);

   {
      if ( q->data != NULL)
      {
         mydata = q->clone(q->data);

         q->freed(q->data);
         q->data = NULL;
      }
      
      pthread_cond_signal(q->condi);
   }

   pthread_mutex_unlock(q->mutex);

   return mydata;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xset_with_funcs(SVQueue* q, void *data, SET_FUNC xset, GET_FUNC xget)
{
   pthread_mutex_lock(q->mutex);

   {
      while( q->data != NULL )
      {
         pthread_cond_wait(q->condi, q->mutex);
      }

      q->data = q->clone(data);

      q->xset = xset;
      q->xget = xget;

      pthread_cond_signal(q->condi);
   }

   pthread_mutex_unlock(q->mutex);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

