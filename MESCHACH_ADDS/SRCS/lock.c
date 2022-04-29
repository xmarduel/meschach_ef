
/** \file lock.c
 *
 * Contains the structures+routine for synchronization for plotting in an other thread
 *
 */

#include <stdlib.h>
#include <stdio.h>

#include "MESCHACH_ADDS/INCLUDES/lock.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Lock * lock_new(void)
{
   Lock *lock  = (Lock*)malloc( sizeof(Lock) );

   lock->mutex    = (pthread_mutex_t*)malloc( sizeof(pthread_mutex_t) );
   pthread_mutex_init(lock->mutex , NULL);
   
   lock->xlock   = xlock;
   lock->xunlock = xunlock;  
   
   return lock;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xlock(Lock* lock)
{
   pthread_mutex_lock(lock->mutex);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * xunlock(Lock* lock)
{
   pthread_mutex_unlock(lock->mutex);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

