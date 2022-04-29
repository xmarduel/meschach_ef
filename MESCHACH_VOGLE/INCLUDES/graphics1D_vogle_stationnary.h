
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

int  graphics1D_vopl_plotcurve();
int  graphics1D_vopl_plotcontour();
	
/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/      

#ifdef __cplusplus
}
#endif


#endif


/*! \fn  graphics1D_vogle_plotcurve( vogle_color color1, const VEC *X , const VEC *Y );
* \brief
*
* Plot the curve X-Y in a Xt-window
*/

