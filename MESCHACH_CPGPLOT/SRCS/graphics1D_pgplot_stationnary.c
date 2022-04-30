

/** \file graphics1D_pgplot.c
 *
 * Contains the routines for plotting with PGPLOT library
 *
 */

#include <pthread.h>

#include "cpgplot.h"

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_svqueue.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_stationnary.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * pgplot_curve_with_xt_toolkit(void *pdata); /* function sent to the thread */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_cpgplot_plotcurve(void)
{
   int rc;

   /* set-up a new thread */
   pthread_t  thread;

   /* init data send to the new thread */
   SVQueue            * queue = svqueue_pgplot_get_static();

   /* fill the queue */
   graphics1D_cpgplot_set_plotdata_in_queue();

   /* Create and run the new thread for the graphics */
   rc = pthread_create(&thread, NULL, pgplot_curve_with_xt_toolkit, queue );
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc); return EXIT_FAILURE;
   }

   /* "main" thread waits until the data is cloned */
   queue->xset(queue, NULL);

   /* finito */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

