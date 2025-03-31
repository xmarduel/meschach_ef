
#ifndef GRAPHICS_1D_VOGLE_STATIONNARY__H
#define GRAPHICS_1D_VOGLE_STATIONNARY__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_vogle_stationnary.h
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * vopl_curve_with_xt_toolkit(void *pdata); /* function sent to the thread */
void * vopl_contour_with_xt_toolkit(void *pdata); /* function sent to the thread */

int  graphics1D_voplcontour_set_thread_type(int type);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_plotcurve(void);
int  graphics1D_vopl_plotcontour(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


#endif


/*! \fn  graphics1D_vopl_plotcurve(void);
* \brief
*
* Plot the curve X-Y in a Xt-window
*/

/*! \fn  graphics1D_vopl_plotcontour(void);
* \brief
*
* Plot the curve X-Y in a Xt-window
*/

