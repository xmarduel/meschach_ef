#include "vogle.h"

/*
 * GetAspect
 */
float
GetAspect(void)
{
	return(getaspect());
}

/*
 * GetFactors
 */
void
GetFactors(double *xr, double *yr)
{
	float	x, y;

	getfactors(&x, &y);

	*xr = x;
	*yr = y;
}

/*
 * GetDisplaySize
 */
void
GetDisplaySize(double *xs, double *ys)
{
	float	x, y;

	getdisplaysize(&x, &y);

	*xs = x;
	*ys = y;
}

/*
 * ExpandViewport
 */
void
ExpandViewport(void)
{
	expandviewport();
}

/*
 * UnExpandViewport
 */
void
UnExpandViewport(void)
{
	unexpandviewport();
}
