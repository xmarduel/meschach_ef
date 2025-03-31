
#include "vopl.h"

/*
 *	adjustscale
 *
 *	Adjust the scaling for the specified axis according to the
 *	values in the array x.
 */
void
adjustscale(
	float		x[],
	int		n,
	char		axis)
{
	int		i, nticks;
	float		min, max, a, b, c, d, div;
	axisdata	*ax;

	switch (axis) {
	case 'x':
	case 'X':
		ax = &plotdev.axes[XIND];
		break;
	case 'y':
	case 'Y':
		ax = &plotdev.axes[YIND];
		break;
	case 'z':
	case 'Z':
		ax = &plotdev.axes[ZIND];
		break;
	case 's':
	case 'S':
		ax = &plotdev.axes[XSEC];
		break;
	case 't':
	case 'T':
		ax = &plotdev.axes[YSEC];
		break;
	default:
		vopl_error("Unknown axis in adjust scale");
	}

	nticks = ax->nticks;
	min = ax->min;
	max = ax->max;
	ax->scaleset = 1;

	/*
	 * Find min and max of array x.
	 */

	a = b = x[0];
	for (i = plotdev.arrayind; i < n; i += plotdev.arrayind) {
		a = MIN(a, x[i]);
		b = MAX(b, x[i]);
	}

	if (a < min || b > max) {	/* We only do this if we have to */

		if (a < min) 
			min = a;

		if (b > max)
			max = b;

		if (!nticks)
			nticks = 5;

		if (ax->scaling == LOGARITHMIC)
			logscale(min, max, nticks, &a, &b, &div);
		else if (ax->forceticks)  {
			linscale2(min, max, nticks, &a, &b, &div);
		} else
			linscale1(min, max, 5, &a, &b, &div);

		ax->min = a;
		ax->max = b;
		ax->div = div;
	}
}
