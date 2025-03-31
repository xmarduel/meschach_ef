#include "vogle.h"

/*
 * polarview_
 */
void
polarview_(float *dist, float *azim, float *inc, float *twist)
{
	polarview(*dist, *azim, *inc, *twist);
}

/*
 * up_
 */
void
up_(float *x, float *y, float *z)
{
	up(*x, *y, *z);
}

/*
 * lookat_
 */
void
lookat_(float *vx, float *vy, float *vz, float *px, float *py, float *pz, float *twist)
{
	lookat(*vx, *vy, *vz, *px, *py, *pz, *twist);
}

/*
 * perspective_
 */
void
perspective_(float *fov, float *aspect, float *hither, float *yon)
{
	perspective(*fov, *aspect, *hither, *yon);
}

/*
 * window_
 */
void
window_(float *left, float *right, float *bottom, float *top, float *hither, float *yon)
{
	window(*left, *right, *bottom, *top, *hither, *yon);
}

/*
 * ortho_
 */
void
ortho_(float *left, float *right, float *bottom, float *top, float *hither, float *yon)
{
	ortho(*left, *right, *bottom, *top, *hither, *yon);
}

/*
 * ortho2_
 */
void
ortho2_(float *left, float *right, float *bottom, float *top)
{
	ortho2(*left, *right, *bottom, *top);
}
