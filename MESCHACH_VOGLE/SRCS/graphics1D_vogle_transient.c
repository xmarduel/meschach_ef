

/** \file graphics1D_vogle_transient.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_transient.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  graphics1D_vopl_plotcurves_start(void)
{
   int rc ;

   /* prepare a new thread */
   pthread_t  thread;

   /* get a static queue */
   SVQueue   * queue = svqueue_vogle_get_static();

   /* fill the queue */
   graphics1D_vopl_set_plotdata_in_queue();

   /* create the new thread for graphics with VOGLE/VOPL + MOTIF-XAW -- othing is plotted !*/
   rc = pthread_create(&thread, NULL, vopl_curve_with_xt_toolkit_transient, queue);

   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
void graphics1D_vopl_plotcurves_flush(void)
{
   /* fill the queue */
   graphics1D_vopl_set_plotdata_in_queue();
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
void  graphics1D_vopl_finalize_transient(void)
{
   /* get the static queue */
   SVQueue *queue = svqueue_vogle_get_static();
   /* get the static data */
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();

   /* set in the queue "no_wait" functions */
   queue->xset_with_funcs(queue, pdata, xset_nowait, xget_nowait);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_vopl_plotcontours_start(void)
{
   int rc ;
	
   /* prepare a new thread */
   pthread_t  thread;
	
   /* get a static queue */
   SVQueue   * queue = svqueue_vogle_get_static();
	
   /* fill the queue */
   graphics1D_vopl_set_plotdata_in_queue();
	
   /* create the new thread for graphics with VOGLE/VOPL + MOTIF-XAW -- othing is plotted !*/
   rc = pthread_create(&thread, NULL, vopl_contours_with_xt_toolkit_transient, queue);
	
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_vopl_plotcontours_flush(void)
{
   /* fill the queue */
   graphics1D_vopl_set_plotdata_in_queue();
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

