#include "vopl.h"

extern	double log10();

/*
 * plot2_
 */
plot2_(x, y, n)
	float	x[], y[];
	int	*n;
{
	plot2(x, y, *n);
}
