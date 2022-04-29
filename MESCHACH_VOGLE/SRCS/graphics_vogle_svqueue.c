/** \file graphics1D_vogle_svqueue.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"


/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static SVQueue *svqueue = NULL;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

SVQueue * svqueue_vogle_get_static()
{
   return svqueue;
}

SVQueue * svqueue_vogle_set_static(SVQueue* queue)
{
   svqueue = queue;

   return svqueue;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

