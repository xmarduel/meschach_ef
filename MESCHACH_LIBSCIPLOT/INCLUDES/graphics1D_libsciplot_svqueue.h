#ifndef GRAPHICS_1D_LIBSCIPLOT_SVQUEUE__H
#define GRAPHICS_1D_LIBSCIPLOT_SVQUEUE__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_libsciplot_svqueue.h
 *
 * Contains the routines for plotting with LIBSCIPLOT library
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH_ADDS/INCLUDES/queue.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* access the static stuff */
SVQueue * svqueue_libsciplot_get_static(void);
SVQueue * svqueue_libsciplot_set_static(SVQueue* queue);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

