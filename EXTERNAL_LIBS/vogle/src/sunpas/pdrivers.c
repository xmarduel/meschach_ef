#include <stdio.h>
#include <string.h>
#include "vogle.h"

/*
 * This should match the Pascal type 'varying [n] of char'
 */

typedef struct {
	int	n;
	char	s[1];	/* SO compiler doesn't bitch about zero length */
} Vstring;

#define COPYANDTERMINATE(buf, s, l)	strncpy(buf, s, l); buf[l] = '\0';

/*
 * Voutput
 */
void
Voutput(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, s.n);
	voutput(buf);
}

/*
 * Vinit
 */
void
Vinit(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, s.n);
        vinit(buf);
}

#if 0
/*
 * VnewDev
 */
void
VnewDev(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, s.n);
	vnewdev(buf);
}
#endif

/*
 * VgetDev
 */
void
VgetDev(Vstring *s)
{
	char	*p = (char *)s->s;

	(void)vgetdev(p);

	s->n = strlen(p);
}

#if 0
/*
 * PushDev
 */
void
PushDev(Vstring s)
{
	char	*p = (char *)s.s;
	char	buf[BUFSIZ];

        COPYANDTERMINATE(buf, p, s.n);
	pushdev(buf);
}

/*
 * PopDev
 */
void
PopDev(void)
{
	popdev();
}
#endif
	
/*
 * Vexit
 */
void
Vexit(void)
{
	vexit();
}

/*
 * VsetFlush
 */
void
VsetFlush(int i)
{
	vsetflush(i);
}
/*
 * Vflush
 */
void
Vflush(void)
{
	vflush();
}

/*
 * Clear
 */
void
Clear(void)
{
	clear();
}

/*
 * Color
 */
void
Color(int col)
{
	color(col);
}

/*
 * MapColor
 */
void
MapColor(int indx, int red, int green, int blue)
{
	mapcolor(indx, red, green, blue);
}

/*
 * GetKey
 */
int
GetKey(void)
{
	return(getkey());
}

/*
 * CheckKey
 */
int
CheckKey(void)
{
	return(checkkey());
}

/*
 * GetDepth
 */
int
GetDepth(void)
{
	return(getdepth());
}

/*
 * Locator
 */
int
Locator(double *xaddr, double *yaddr)
{
	float	x, y;
	int	i;

	i = locator(&x, &y);

	*xaddr = x;
	*yaddr = y;

	return(i);
}

/*
 * Slocator
 */
int
Slocator(double *xaddr, double *yaddr)
{
	float	x, y;
	int	i;

	i = slocator(&x, &y);

	*xaddr = x;
	*yaddr = y;

	return(i);
}
