
#ifndef GRAPHICS_1D_PGPLOT_TRANSIENT__H
#define GRAPHICS_1D_PGPLOT_TRANSIENT__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_pgplot_transient.h
 *
 * Contains the routines for plotting with CPGPLOT library
 *
 */

#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_cpgplot_plotcurves_start(void);
void graphics1D_cpgplot_plotcurves_flush(void);
void graphics1D_cpgplot_finalize_transient(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif


/*! \fnvoid graphics1D_cpgplot_finalize_transient(void)
 *
 * function to call at last for trnasient problem plots
 */

/*! \fnvoid graphics1D_cpgplot_plotcurves_start(void)
 *
 * for transient plots, for the first plot
 */

/*! \fnvoid graphics1D_cpgplot_plotcurves_flush(void)
 *
 * for transient plots, after the first plot
 */

/*! \fnvoid graphics1D_cpgplot_wait_cursor_click(void)
 *
 * for transient plots, ask for a stop
 */

