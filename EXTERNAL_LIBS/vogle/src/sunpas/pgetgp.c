#include "vogle.h"

/*
 * GetGp
 */
void
GetGp(double *x, double *y, double *z)
{
	float	a, b, c;
	getgp(&a, &b, &c);

	*x = a;
	*y = b;
	*z = c;
}

/*
 * GetGpT
 */
void
GetGpT(double *x, double *y, double *z, double *w)
{
	float	a, b, c, d;
	getgpt(&a, &b, &c, &d);

	*x = a;
	*y = b;
	*z = c;
	*w = d;
}

/*
 * GetGp2
 */
void
GetGp2(double *x, double *y)
{
	float	a, b;
	getgp2(&a, &b);

	*x = a;
	*y = b;
}

/*
 * SgetGp2
 */
void
Sgetgp2(double *x, double *y)
{
	float	a, b;

	sgetgp2(&a, &b);

	*x = a;
	*y = b;
}

/*
 * PushPos
 */
void
PushPos(void)
{
	pushpos();
}

/*
 * PopPos
 */
void
PopPos(void)
{
	poppos();
}
