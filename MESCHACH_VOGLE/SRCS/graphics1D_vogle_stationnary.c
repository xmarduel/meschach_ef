

/** \file graphics1D_vogle_stationnary.c
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

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_stationnary.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_plotcurve(void)
{
   int rc;

   /* set-up a new thread */
   pthread_t  thread;

   /* init data send to the new thread */
   SVQueue    * queue = svqueue_vogle_get_static();

   /* fill the queue */
   graphics1D_vopl_set_plotdata_in_queue();

   /* Create and run the new thread for the graphics */
   rc = pthread_create(&thread, NULL, vopl_curve_with_xt_toolkit, queue );
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

int  graphics1D_vopl_plotcontour(void)

{
   int rc;
   
   /* prepare a new thread */
   pthread_t  thread;
	
   /* init data send to the new thread */
   SVQueue            * queue = svqueue_vogle_get_static();
	
   /* fill the queue */
   graphics1D_vopl_set_plotdata_in_queue();
   
   /* Create and run the new thread */
   rc = pthread_create(&thread, NULL, vopl_contour_with_xt_toolkit, queue);
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

