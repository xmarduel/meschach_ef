
#include "vopl.h"

/*
 *	adjustscale
 *
 */
void
adjustscale_(x, n, axis)
	float	x[];
	int	*n;
	char	*axis;
{
	adjustscale(x, *n, *axis);
}
