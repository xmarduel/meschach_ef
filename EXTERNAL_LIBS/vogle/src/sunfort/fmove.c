#include "vogle.h"

/*
 * move_
 */
void
move_(float *x, float *y, float *z)
{
	move(*x, *y, *z);
}


/*
 * move2_
 */
void
move2_(float *x, float *y)
{
	move2(*x, *y);
}

/*
 * rmove_
 */
void
rmove_(float *dx, float *dy, float *dz)
{
	rmove(*dx, *dy, *dz);
}

/*
 * rmove2_
 */
void
rmove2_(float *dx, float *dy)
{
	rmove2(*dx, *dy);
}

/*
 * smove2_
 */
void
smove2_(float *xs, float *ys)
{
	smove2(*xs, *ys);
}

/*
 * rsmove2_
 */
void
rsmove2_(float *dxs, float *dys)
{
	rsmove2(*dxs, *dys);
}

