#include <stdio.h>
#include <string.h>
#include "vogle.h"

/*
 * setdash_
 */
void
setdash_(float *d)
{
	setdash(*d);
}

/*
 * linestyle_
 */
void
linestyle_(char *l, int len)
{
        char            buf[BUFSIZ];
        char   *p;

        strncpy(buf, l, len);
        buf[len] = 0;

        for (p = &buf[len - 1]; *p == ' '; p--)
                ;

        *++p = 0;

        linestyle(buf);
}

/*
 * draw_
 */
void
draw_(float *x, float *y, float *z)
{
	draw(*x, *y, *z);
}

/*
 * draw2_
 */
void
draw2_(float *x, float *y)
{
	draw2(*x, *y);
}

/*
 * rdraw_
 */
void
rdraw_(float *dx, float *dy, float *dz)
{
	rdraw(*dx, *dy, *dz);
}

/*
 * rdraw2_
 */
void
rdraw2_(float *dx, float *dy)
{
	rdraw2(*dx, *dy);
}

/*
 * sdraw2_
 */
void
sdraw2_(float *xs, float *ys)
{
	sdraw2(*xs, *ys);
}

/*
 * rsdraw2_
 */
void
rsdraw2_(float *dxs, float *dys)
{
	rsdraw2(*dxs, *dys);
}
