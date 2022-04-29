
/** \file graphics2D_vogle_transient.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_stationnary.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_transient.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * vogle_surface_with_xt_toolkit_transient(void* data); /* the thread function -> plot into a Xt window */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  graphics2D_vogle_plotsurface_start(void)
{
   int rc ;

   /* prepare a new thread */
   pthread_t  mythread;

   /* get a static queue */
   SVQueue *queue = svqueue_vogle_get_static();

   /* fill the queue */
   graphics2D_vogle_set_plotdata_in_queue();

   /* create the new thread for graphics with VOGLE/VOPL + XAW - the data in the queue is plotted !*/
   rc = pthread_create(&mythread, NULL, vogle_surface_with_xt_toolkit_transient, queue);

   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  graphics2D_vogle_finalize_transient(void)
{
   /* get the static queue */
   SVQueue *queue = svqueue_vogle_get_static();
   /* get the static data */
   VOGLE_SURFACE_DATA * pdata = pdata_vogle_get_static();

   /* set in the queue "no_wait" functions */
   queue->xset_with_funcs(queue, pdata, (SET_FUNC)xset_nowait, (GET_FUNC)xget_nowait);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void graphics2D_vogle_plotsurface_flush(void)
{
   /* fill the queue */
   graphics2D_vogle_set_plotdata_in_queue();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */
