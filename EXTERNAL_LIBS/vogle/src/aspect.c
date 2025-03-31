#include "vogle.h"

#define	MIN(x, y)	((x) < (y) ? (x) : (y))

/*
 * getaspect
 *
 *	Gets the aspect ratio of the display/window.
 *	IE. y / x
 */
float
getaspect(void)
{
	return((float)vdevice.dev.sizeSy / (float)vdevice.dev.sizeSx);
}

/*
 * getdisplaysize
 *
 *	Returns the raw size of the display window in pixel units
 *	as floating point values.
 */
void
getdisplaysize(float *x, float *y)
{
	*x = (float)vdevice.dev.sizeSx;
	*y = (float)vdevice.dev.sizeSy;
}

/*
 * getfactors
 *
 *	returns two x and y scaling factors for use with the
 *	viewport call so as the viewport can be set to the
 *	whole display/window.
 */
void
getfactors(float *x, float *y)
{
	*x = (float)vdevice.dev.sizeSx / (float)vdevice.dev.sizeX;
	*y = (float)vdevice.dev.sizeSy / (float)vdevice.dev.sizeY;
}

/*
 * expandviewport
 *
 *	Vogle will normally use the largest square it can fit onto the 
 * actual display device. This call says to use the whole device... however
 * you must then take into account any distortion that will occur due to 
 * the non square mapping.
 */
void
expandviewport(void)
{
	vdevice.attr->a.exvp = 1;
	vdevice.dev.sizeX = vdevice.dev.sizeSx;
	vdevice.dev.sizeY = vdevice.dev.sizeSy;

	CalcW2Vcoeffs();
}

/*
 * unexpandviewport
 * 	- opposite of the above...
 */
void
unexpandviewport(void)
{
	vdevice.attr->a.exvp = 0;
	vdevice.dev.sizeY = vdevice.dev.sizeX = MIN(vdevice.dev.sizeSx, vdevice.dev.sizeSy);

	CalcW2Vcoeffs();
}
