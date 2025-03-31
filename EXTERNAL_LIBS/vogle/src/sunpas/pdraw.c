#include <stdio.h>
#include <string.h>
#include "vogle.h"

typedef struct {
	int     n;
	char    s[1];   /* SO compiler doesn't bitch about zero length */
} Vstring;

#define COPYANDTERMINATE(buf, s, l)	strncpy(buf, s, l); buf[l] = '\0';

/*
 * SetDash
 */
void
SetDash(double d)
{
	setdash(d);
}


/*
 * LineStyle
 */
void
LineStyle(Vstring l)
{
        char    *p = (char *)l.s;
        char    buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, l.n);
        linestyle(buf);
}

/*
 * Draw
 */
void
Draw(double x, double y, double z)
{
	draw(x, y, z);
}

/*
 * Draw2
 */
void
Draw2(double x, double y)
{
	draw2(x, y);
}

/*
 * Rdraw
 */
void
Rdraw(double dx, double dy, double dz)
{
	rdraw(dx, dy, dz);
}

/*
 * Rdraw2
 */
void
Rdraw2(double dx, double dy)
{
	rdraw2(dx, dy);
}

/*
 * Sdraw2
 */
void
Sdraw2(double xs, double ys)
{
	sdraw2(xs, ys);
}

/*
 * Rsdraw2
 */
void
Rsdraw2(double dxs, double dys)
{
	rsdraw2(dxs, dys);
}
