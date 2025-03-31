#include "vogle.h"

/*
 * PatchBasis
 */
void
PatchBasis(Matrix ubasis, Matrix vbasis)
{
	patchbasis(ubasis, vbasis);
}

/*
 * PatchPrecision
 */
void
PatchPrecision(int useg, int vseg)
{
	patchprecision(useg, vseg);
}

/* 
 * PatchCurves
 */
void
PatchCurves(int nu, int nv)
{
	patchcurves(nu, nv);
}

/*
 * Patch
 */
void
Patch(Matrix geomx, Matrix geomy, Matrix geomz)
{
	patch(geomx, geomy, geomz);
}

/*
 * Rpatch
 */
void
Rpatch(Matrix geomx, Matrix geomy, Matrix geomz, Matrix geomw)
{
	rpatch(geomx, geomy, geomz, geomw);
}
