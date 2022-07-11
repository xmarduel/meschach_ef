
#ifndef GRAPHICS_1D_VOGLE_TRANSIENT__H
#define GRAPHICS_1D_VOGLE_TRANSIENT__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_vogle_transient.h
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_vopl_plotcurves_start(void);
void graphics1D_vopl_plotcurves_flush(void);

void graphics1D_vopl_plotcontours_start(void);
void graphics1D_vopl_plotcontours_flush(void);

void graphics1D_vopl_finalize_transient(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif



/*! \fnvoid graphics1D_vogle_finalize_transient(void);
 *
 * function to call at last for trnasient problem plots
 */

/*! \fnvoid graphics1D_vogle_plotcurves_start(void);
 *
 * for transient plots, for the first plot
 */

/*! \fnvoid graphics1D_vogle_plotcurves_flush(void);
*
* for transient plots, after the first plot
*/



