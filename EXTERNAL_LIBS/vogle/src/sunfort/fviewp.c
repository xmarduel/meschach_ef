#include "vogle.h"

/*
 * pushviewport_
 */
void
pushviewport_(void)
{
	pushviewport();
}

/*
 * popviewport_
 */
void
popviewport_(void)
{
	popviewport();
}

/*
 * viewport_
 */
void
viewport_(float *xlow, float *xhigh, float *ylow, float *yhigh)
{
	viewport(*xlow, *xhigh, *ylow, *yhigh);
}

/*
 * getviewport_
 */
void
getviewport_(float *left, float *right, float *bottom, float *top)
{
	getviewport(left, right, bottom, top);
}

