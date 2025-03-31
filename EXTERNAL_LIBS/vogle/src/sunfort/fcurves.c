#include "vogle.h"

/*
 * curvebasis_
 */
void
curvebasis_(Matrix basis)
{
	curvebasis(basis);
}

/*
 * curveprecision_
 */
void
curveprecision_(int *nsegments)
{
	curveprecision(*nsegments);
}

/*
 * rcurve_
 */
void
rcurve_(Matrix geom)
{
	rcurve(geom);
}

/*
 * curve_
 */
void
curve_(float geom[4][3])
{
	curve(geom);
}

/*
 * curven_
 */
void
curven_(int *n, float geom[][3])
{
	curven(*n, geom);
}
