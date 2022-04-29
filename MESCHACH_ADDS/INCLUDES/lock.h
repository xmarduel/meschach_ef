

#ifndef MESCHACH_THREADS_LOCK__H
#define MESCHACH_THREADS_LOCK__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file lock.h
*
* Contains the structures+routines for synchronization with the plotting routines
*
* ----------------------------------------------------------------------- */

#include <pthread.h>

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct Lock_  /* a "simple" lock */
{
   /**/
   pthread_mutex_t  * mutex;

   /**/
   void * (*xlock)(struct Lock_ *);
   void * (*xunlock)(struct Lock_ *);

} Lock;


Lock * lock_new(void);

void * xlock(Lock* lock);
void * xunlock(Lock* lock);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
