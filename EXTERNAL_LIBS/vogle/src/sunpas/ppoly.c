#include "vogle.h"

/*
 * PolyFill
 */
void
PolyFill(int onoff)
{
	polyfill(onoff);
}

/*
 * HatchAng
 */
void
HatchAng(double ang)
{
	hatchang(ang);
}

/*
 * HatchPitch
 */
void
HatchPitch(double pitch)
{
	hatchpitch(pitch);
}

/*
 * PolyHatch
 */
void
PolyHatch(int onoff)
{
	polyhatch(onoff);
}

/*
 * Poly2
 */
void
Poly2(int n, float parray[][2])
{
	poly2(n, parray);
}

/*
 * Poly
 */
void
Poly(int n, float parray[][3])
{
	poly(n, parray);
}

/*
 * MakePoly
 */
void
MakePoly(void)
{
	makepoly();
}

/*
 * ClosePoly
 */
void
ClosePoly(void)
{
	closepoly();
}

/*
 * BackFace
 */
void
BackFace(int onoff)
{
	backface(onoff);
}

/*
 * BackFaceDir
 */
void
BackFaceDir(int clockwise)
{
	backfacedir(clockwise);
}
