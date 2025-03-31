#include "vogle.h"

/*
 * getaspect_
 */
float
getaspect_(void)
{
	return(getaspect());
}

/*
 * getfactors_
 */
void
getfactors_(float *xr, float *yr)
{
	getfactors(xr, yr);
}

/*
 * getdisplaysize_
 */
void
getdisplaysize_(float *xs, float *ys)
{
	getdisplaysize(xs, ys);
}

/*
 * expandviewport_
 */
void
expandviewport_(void)
{
	expandviewport();
}

/*
 * unexpandviewport_
 */
void
unexpandviewport_(void)
{
	unexpandviewport();
}
