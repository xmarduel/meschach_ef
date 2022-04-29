
#ifndef GRAPHICS_1D_LIBSCIPLOT_TRANSIENT__H
#define GRAPHICS_1D_LIBSCIPLOT_TRANSIENT__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_libsciplot_transient.h
 *
 * Contains the routines for plotting with LIBSCIPLOT library
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_libsciplot_plotcurves_start(void);
void graphics1D_libsciplot_plotcurves_flush(void);
void graphics1D_libsciplot_finalize_transient(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif

