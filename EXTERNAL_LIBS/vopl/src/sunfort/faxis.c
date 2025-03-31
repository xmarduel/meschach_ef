#include <stdio.h>
#include <string.h>
#include "vopl.h"

/*
 * axistitle_
 *
 *	Draws an x, y or z axis, with or without annotation, tickmarks etc
 */
void
axistitle_(title, ax, len)
	char	*title;
	char	*ax;
	int	len;
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, title, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	axistitle(buf, *ax);
}

/*
 * drawaxis_
 */
void
drawaxis_(ax)
	char	*ax;
{
	drawaxis(*ax);
}

/*
 * drawaxes2_
 */
void
drawaxes2_()
{
	drawaxes2();
}

/*
 * drawaxes_
 */
void
drawaxes_()
{
	drawaxes();
}
