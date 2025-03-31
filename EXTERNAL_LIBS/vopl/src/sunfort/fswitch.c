#include <stdio.h>
#include "vopl.h"

/*
 * fit_
 */
void
fit_(type)
	int	*type;
{
	fit(*type);
}

/*
 * skip_
 */
skip_(n)
	int	*n;
{
	skip(n);
}

/*
 * endslopes_
 */
void
endslopes_(a, b)
	float	*a, *b;
{
	endslopes(*a, *b);
}

/*
 * gridspacing_
 */
void
gridspacing_(spacing)
	int	*spacing;
{
	gridspacing(*spacing);
}

/*
 * scaling_
 */
void
scaling_(type, axis)
	int	*type;
	char	*axis;
{
	scaling(*type, *axis);
}

/*
 * tickmarks_
 */
void
tickmarks_(num, axis)
	int	*num;
	char	*axis;
{
	tickmarks(*num, *axis);
}

/*
 * tickspacing_
 */
void
tickspacing_(num, axis)
	int	*num;
	char	*axis;
{
	tickspacing(*num, *axis);
}

/*
 * minorticks_
 */
void
minorticks_(num, axis)
	int     *num;
	char    *axis;
{
	minorticks(*num, *axis);
}


/*
 * annotate_
 */
void
annotate_(format, axis, len)
	char	*format;
	char	*axis;
	int	len;
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, format, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	annotate(buf, *axis);
}

/*
 * arrayindex_
 */
void
arrayindex_(i)
	int	*i;
{
	arrayindex(*i);
}

/*
 * marker_
 */
void
marker_(string, len)
	char	*string;
	int	len;
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, string, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	marker(buf);
}
		
/*
 * markerspacing_
 */
void
markerspacing_(spacing)
	int	*spacing;
{
	markerspacing(*spacing);
}

/*
 * markerscale_
 */
void
markerscale_(s)
	float	*s;
{
	markerscale(*s);
}

/*
 * graphtitle_
 */
void
graphtitle_(s, len)
	char	*s;
	int	len;
{
	char		buf[BUFSIZ];
	register char	*p;

	strncpy(buf, s, len);
	buf[len] = 0;

	for (p = &buf[len - 1]; *p == ' '; p--)
		;

	*++p = 0;

	graphtitle(buf);
}

/*
 * drawtitle_
 */
void
drawtitle_()
{
	drawtitle();
}

/*
 * range_
 */
void
range_(min, max, axis)
	float	*min, *max;
	char	*axis;
{
	range(*min, *max, *axis);
}

