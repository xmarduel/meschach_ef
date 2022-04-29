
/** \file graphics1D_libsciplot_transient_xt.c
 *
 * Contains the routines for plotting with LIBSCIPLOT library
 *
 */

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_svqueue.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_transient.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * graphics1D_libsciplot_curve_with_xt_toolkit_transient(void * data); /* the thread function -> plot into a Xt window */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  graphics1D_libsciplot_plotcurves_start(void)
{
   int rc ;

   /* prepare a new thread */
   pthread_t  mythread;

   /* get a static queue */
   SVQueue *queue = svqueue_libsciplot_get_static();

   /* fill the queue */
   graphics1D_libsciplot_set_plotdata_in_queue();
	
   /* create the new thread for graphics with VOGLE/VOPL + MOTIF-XAW -- othing is plotted !*/
   rc = pthread_create(&mythread, NULL, graphics1D_libsciplot_curve_with_xt_toolkit_transient, queue);

   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  graphics1D_libsciplot_finalize_transient(void)
{
   /* get the static queue */
   SVQueue *queue = svqueue_libsciplot_get_static();
   /* get the static data */
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   /* set in the queue "no_wait" functions */
   queue->xset_with_funcs(queue, pdata, xset_nowait, xget_nowait);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void graphics1D_libsciplot_plotcurves_flush(void)
{
   /* fill the queue */
   graphics1D_libsciplot_set_plotdata_in_queue();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

