#include "vogle.h"

/*
 * patchbasis_
 */
void
patchbasis_(Matrix ubasis, Matrix vbasis)
{
	patchbasis(ubasis, vbasis);
}

/*
 * patchprecision_
 */
void
patchprecision_(int *useg, int *vseg)
{
	patchprecision(*useg, *vseg);
}

/* 
 * patchcurves_
 */
void
patchcurves_(int *nu, int *nv)
{
	patchcurves(*nu, *nv);
}

/*
 * patch_
 */
void
patch_(Matrix geomx, Matrix geomy, Matrix geomz)
{
	patch(geomx, geomy, geomz);
}

/*
 * rpatch_
 */
void
rpatch_(Matrix geomx, Matrix geomy, Matrix geomz, Matrix geomw)
{
	rpatch(geomx, geomy, geomz, geomw);
}
