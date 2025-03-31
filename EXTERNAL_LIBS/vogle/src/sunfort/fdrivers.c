#include <stdio.h>
#include <string.h>
#include "vogle.h"

/*
 * voutput_
 */
void
voutput_(char *path, int len)
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, path, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	voutput(buf);
}

#if 0
/*
 * pushdev_
 */
void
pushdev_(char *dev, int len)
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, dev, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	pushdev(buf);
}
	
/*
 * popdev_
 */
void
popdev_(void)
{
	popdev();
}
#endif

/*
 * vexit_
 */
void
vexit_(void)
{
	vexit();
}

/*
 * clear_
 */
void
clear_(void)
{
	clear();
}

/*
 * color_
 */
void
color_(int *col)
{
	color(*col);
}

/*
 * _mapcolor
 */
void
mapcolor_(int *indx, int *red, int *green, int *blue)
{
	mapcolor(*indx, *red, *green, *blue);
}

/*
 * getkey_
 */
int
getkey_(void)
{
	return(getkey());
}

/*
 * checkkey_
 */
int
checkkey_(void)
{
	return(checkkey());
}

/*
 * getdepth_
 */
int
getdepth_(void)
{
	return(getdepth());
}

/*
 * locator_
 */
int
locator_(float *xaddr, float *yaddr)
{
	return(locator(xaddr, yaddr));
}

/*
 * slocator_
 */
int
slocator_(float *xaddr, float *yaddr)
{
	return(slocator(xaddr, yaddr));
}

/*
 * vsetflush_
 */
void
vsetflush_(int *yn)
{
	vdevice.sync = *yn;
}

/*
 * vflush_
 */
void
vflush_(void)
{
	vflush();
}

/*
 * linewidth_
 */
void
linewidth_(int *w)
{
	linewidth(*w);
}
