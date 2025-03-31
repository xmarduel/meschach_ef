#include "vogle.h"

/*
 * CirclePrecision
 */
void
CirclePrecision(int prec)
{
	circleprecision(prec);
}

/*
 * ArcPrecision
 */
void
ArcPrecision(int prec)
{
	circleprecision(prec);
}

/*
 * Arc
 */
void
Arc(double x, double y, double radius, double startang, double endang)
{
	arc(x, y, radius, startang, endang);
}

/*
 * Sector
 */
void
Sector(double x, double y, double radius, double startang, double endang)
{
	sector(x, y, radius, startang, endang);
}

/*
 * Circle
 */
void
Circle(double x, double y, double radius)
{
	circle(x, y, radius);
}

