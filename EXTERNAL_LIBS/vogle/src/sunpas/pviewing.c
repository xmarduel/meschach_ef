#include "vogle.h"

/*
 * PolarView
 */
void
PolarView(double dist, double azim, double inc, double twist)
{
	polarview(dist, azim, inc, twist);
}

/*
 * Up
 */
void
Up(double x, double y, double z)
{
	up(x, y, z);
}

/*
 * LookAt
 */
void
LookAt(double vx, double vy, double vz, double px, double py, double pz, double twist)
{
	lookat(vx, vy, vz, px, py, pz, twist);
}

/*
 * Perspective
 */
void
Perspective(double fov, double aspect, double hither, double yon)
{
	perspective(fov, aspect, hither, yon);
}

/*
 * Window
 */
void
Window(double left, double right, double bottom, double top, double hither, double yon)
{
	window(left, right, bottom, top, hither, yon);
}

/*
 * Ortho
 */
void
Ortho(double left, double right, double bottom, double top, double hither, double yon)
{
	ortho(left, right, bottom, top, hither, yon);
}

/*
 * Ortho2
 */
void
Ortho2(double left, double right, double bottom, double top)
{
	ortho2(left, right, bottom, top);
}
