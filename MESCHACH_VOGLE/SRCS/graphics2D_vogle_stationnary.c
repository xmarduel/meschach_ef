

/** \file graphics2D_vogle_stationnary.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include <pthread.h>

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#ifdef HAVE_VOGLE
#include "vogle.h"         /* Librairie VOGLE  */
#endif

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_stationnary.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * vogle_surface_with_xt_toolkit(void *pdata);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics2D_vogle_plotsurface(void)
{
   int rc;
   
   /* prepare a new thread */
   pthread_t  thread;

   /* get a static queue */
   SVQueue *queue = svqueue_vogle_get_static();
	
	/* fill the queue */
   graphics2D_vogle_set_plotdata_in_queue();
	
	/* Create and run the new thread */
   rc = pthread_create(&thread, NULL, vogle_surface_with_xt_toolkit, queue );
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc); return EXIT_FAILURE;
   }

   /* wait until the data is cloned */
   queue->xset(queue, NULL);

   /* finito */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

