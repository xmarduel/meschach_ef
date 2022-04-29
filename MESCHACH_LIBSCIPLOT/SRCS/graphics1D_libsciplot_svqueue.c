
/** \file graphics_libsciplot_svqueue.c
*
* Contains the routines for plotting with LIBSCIPLOT library
*
*/

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_svqueue.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static SVQueue *svqueue = NULL;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

SVQueue * svqueue_libsciplot_get_static()
{
   return svqueue;
}

SVQueue * svqueue_libsciplot_set_static(SVQueue* queue)
{
   svqueue = queue;

   return svqueue;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

