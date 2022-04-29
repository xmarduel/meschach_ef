
/** \file graphics_pgplot_svqueue.c
 *
 * Contains the routines for plotting with PGPLOT library
 *
 */

#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_svqueue.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static SVQueue *svqueue = NULL;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

SVQueue * svqueue_pgplot_get_static()
{
   return svqueue;
}

SVQueue * svqueue_pgplot_set_static(SVQueue* queue)
{
   svqueue = queue;

   return svqueue;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

