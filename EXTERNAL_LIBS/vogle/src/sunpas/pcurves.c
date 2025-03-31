#include "vogle.h"

/*
 * CurveBasis
 */
void
CurveBasis(Matrix basis)
{
	curvebasis(basis);
}

/*
 * CurvePrecision
 */
void
CurvePrecision(int nsegments)
{
	curveprecision(nsegments);
}

/*
 * Rcurve
 */
void
Rcurve(Matrix geom)
{
	rcurve(geom);
}

/*
 * curve
 */
void
Curve(float geom[4][3])
{
	curve(geom);
}

/*
 * Curven
 */
void
Curven(int n, float geom[][3])
{
	curven(n, geom);
}
