#include "vogle.h"

/*
 * PushViewPort
 */
void
PushViewPort(void)
{
	pushviewport();
}

/*
 * PopViewPort
 */
void
PopViewPort(void)
{
	popviewport();
}

/*
 * CalcViewPort
 */
void
CalcViewPort(void)
{
	calcviewport();
}

/*
 * ViewPort
 */
void
ViewPort(double xlow, double xhigh, double ylow, double yhigh)
{
	viewport(xlow, xhigh, ylow, yhigh);
}

/*
 * GetViewPort
 */
void
GetViewPort(double *left, double *right, double *bottom, double *top)
{
	float	l, r, b, t;
	getviewport(&l, &r, &b, &t);

	*left = l;
	*right = r;
	*bottom = b;
	*top = t;
}

