#ifndef GRAPHICS_PGPLOT_SVQUEUE__H
#define GRAPHICS_PGPLOT_SVQUEUE__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_pgplot_svqueue.h
 *
 * Contains the routines for plotting with PGPLOT library
 *
 */


#include "MESCHACH_ADDS/INCLUDES/queue.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* access the static stuff */
SVQueue * svqueue_pgplot_get_static(void);
SVQueue * svqueue_pgplot_set_static(SVQueue* queue);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif
