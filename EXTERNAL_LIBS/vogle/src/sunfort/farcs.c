#include "vogle.h"

/*
 * circleprecision_
 */
void
circleprecision_(int *prec)
{
	circleprecision(*prec);
}

/*
 * arcprecision_
 */
void
arcprecision_(int *prec)
{
	circleprecision(*prec);
}

/*
 * arc_
 */
void
arc_(float *x, float *y, float *radius, float *startang, float *endang)
{
	arc(*x, *y, *radius, *startang, *endang);
}

/*
 * sector_
 */
void
sector_(float *x, float *y, float *radius, float *startang, float *endang)
{
	sector(*x, *y, *radius, *startang, *endang);
}

/*
 * circle_
 */
void
circle_(float *x, float *y, float *radius)
{
	circle(*x, *y, *radius);
}

