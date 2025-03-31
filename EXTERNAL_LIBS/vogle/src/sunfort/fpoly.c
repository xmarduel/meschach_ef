#include "vogle.h"

/*
 * polyfill_
 */
void
polyfill_(int *onoff)
{
	polyfill(*onoff);
}

/*
 * hatchang_
 */
void
hatchang_(float *ang)
{
	hatchang(*ang);
}

/*
 * hatchpitch_
 */
void
hatchpitch_(float *pitch)
{
	hatchpitch(*pitch);
}

/*
 * polyhatch_
 */
void
polyhatch_(int *onoff)
{
	polyhatch(*onoff);
}

/*
 * poly2_
 */
void
poly2_(int *n, float parray[][2])
{
	poly2(*n, parray);
}

/*
 * poly_
 */
void
poly_(int *n, float parray[][3])
{
	poly(*n, parray);
}

/*
 * makepoly_
 */
void
makepoly_(void)
{
	makepoly();
}

/*
 * closepoly_
 */
void
closepoly_(void)
{
	closepoly();
}

/*
 * backface_
 */
void
backface_(int *onoff)
{
	backface(*onoff);
}

/*
 * backfacedir_
 */
void
backfacedir_(int *clockwise)
{
	backfacedir(*clockwise);
}
