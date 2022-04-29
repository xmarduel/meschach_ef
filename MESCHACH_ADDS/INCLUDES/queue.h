

#ifndef MESCHACH_THREADS_QUEUE__H
#define MESCHACH_THREADS_QUEUE__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file queue.h
*
* Contains the structures+routines for synchronization for plotting with VOGLE library
*
* ----------------------------------------------------------------------- */

#include <pthread.h>

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef void* (*CLONE_FUNC)(void*);
typedef void* (*FREED_FUNC)(void*);


typedef struct _SVQueue  /* a queue which contains only 1 item ! (SV = Single Value) */
{
   /**/
   pthread_mutex_t  * mutex;
   pthread_cond_t   * condi;

   /**/
   void * data;
   void * (*clone)(void* data);
   void * (*freed)(void* data);

   /**/
   void * (*xget)(struct _SVQueue* q);
   void * (*xset)(struct _SVQueue* q, void* data);
   /**/
   void * (*xset_with_funcs)(struct _SVQueue* q, void* data, void * (set)(struct _SVQueue* q, void* data), void * (*get)(struct _SVQueue* q));

} SVQueue;

typedef void* (*SET_FUNC)(SVQueue *q, void* data);
typedef void* (*GET_FUNC)(SVQueue *q);

SVQueue * svqueue_new(CLONE_FUNC clone, FREED_FUNC freed);

void * xget(SVQueue* q);
void * xset(SVQueue* q, void *data);

void * xget_nowait(SVQueue* q);
void * xset_nowait(SVQueue* q, void *data);

void * xset_with_funcs(SVQueue* q, void *data, SET_FUNC set, GET_FUNC get);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
