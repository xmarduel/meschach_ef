
#ifndef GRAPHICS_2D_VOGLE_TRANSIENT__H
#define GRAPHICS_2D_VOGLE_TRANSIENT__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics2D_vogle_transient.h
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics2D_vogle_plotsurface_start(void);
void graphics2D_vogle_plotsurface_flush(void);
void graphics2D_vogle_finalize_transient(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif



/*! \fnvoid graphics2D_vogle_finalize_transient(void);
 *
 * function to call at last for trnasient problem plots
 */

/*! \fnvoid graphics2D_vogle_plotcurves_start(void);
 *
 * for transient plots, for the first plot
 */

/*! \fnvoid graphics2D_vogle_plotcurves_flush(void);
*
* for transient plots, after the first plot
*/

/*! \fnvoid graphics2D_vogle_wait_cursor_click(void);
 *
 * for transient plots, ask for a stop
 */

