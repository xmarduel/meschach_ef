#include <stdio.h>
#include <math.h>
#include "vopl.h"

static	char	charbuf[10];

float	cardinal[4][4] = {
	{-0.5,	1.5,	-1.5,	0.5},
	{1.0,	-2.5,	2.0,	-0.5},
	{-0.5,	0.0,	0.5,	0.0},
	{0.0,	1.0,	0.0,	0.0}
};

/*
 * the initial structure
 */
vopldev plotdev = {
	0.0, 0.0,		/* s1, sn */
	45.0,			/* Hist hatch ang */
	0.75,			/* markerscale */
	STRAIGHT_LINE,		/* fit */
	2,			/* degree */
	0,			/* spline type */
	0,			/* hatch histograms? */
	0,			/* grid? */
	0, 1,			/* startind, arrayind */
	256, 1,			/* precision, markerspacing*/
	(char *)0,		/* marker */
	(char *)0,		/* graphtitle */
	{						/* x axis */
		{
			BLOODYBIG,	/* min */
			-BLOODYBIG, 	/* max */
			1.0,		/* div */
			LINEAR,		/* scaling */
			1,		/* annotate? */
			5,		/* nticks */
			1,		/* ntspacing */
			10,		/* minorticks */
			0,		/* scaleset */
			0,		/* forceticks */
			0,		/* mod */
			(char *)0,	/* format */
			(char *)0	/* title */
		},
		{						/* y axis */
			BLOODYBIG,	/* min */
			-BLOODYBIG, 	/* max */
			1.0,		/* div */
			LINEAR,		/* scaling */
			1,		/* annotate? */
			5,		/* nticks */
			1,		/* ntspacing */
			10,		/* minorticks */
			0,		/* scaleset */
			0,		/* forceticks */
			0,		/* mod */
			(char *)0,	/* format */
			(char *)0	/* title */
		},
		{						/* z axis */
			BLOODYBIG,	/* min */
			-BLOODYBIG, 	/* max */
			1.0,		/* div */
			LINEAR,		/* scaling */
			1,		/* annotate? */
			5,		/* nticks */
			1,		/* ntspacing */
			10,		/* minorticks */
			0,		/* scaleset */
			0,		/* forceticks */
			0,		/* mod */
			(char *)0,	/* format */
			(char *)0	/* title */
		},
    	        {					/* sec x axis */
			BLOODYBIG,	/* min */
			-BLOODYBIG, 	/* max */
			1.0,		/* div */
			LINEAR,		/* scaling */
			1,		/* annotate? */
			5,		/* nticks */
			1,		/* ntspacing */
			10,		/* minorticks */
			0,		/* scaleset */
			0,		/* forceticks */
			0,		/* mod */
			(char *)0,	/* format */
			(char *)0	/* title */
		},
		{						/* sec y axis */
			BLOODYBIG,	/* min */
			-BLOODYBIG, 	/* max */
			1.0,		/* div */
			LINEAR,		/* scaling */
			1,		/* annotate? */
			5,		/* nticks */
			1,		/* ntspacing */
			10,		/* minorticks */
			0,		/* scaleset */
			0,		/* forceticks */
			0,		/* mod */
			(char *)0,	/* format */
			(char *)0	/* title */
		}
	}
};

/*
 * fit
 *
 *	Specify the type of fit for a graph
 */
void
fit(
	int	type)
{
	char	err_buf[EBUF_SIZE];

	if (type < 0 || type > MAX_FIT) {
		sprintf(err_buf, "fit: unknown fit type: %d", type);
		vopl_error(err_buf);
	}

	plotdev.fit = type;
}

/*
 * skip
 *
 *	causes scaling and plotting to skip every n data points
 */
void
skip(
	int	n)
{
	plotdev.arrayind = n;
}

/*
 * endslopes
 *
 *	Specify the end slopes for a cubic spline fit.
 *	If No slopes have been specified, then we'll use
 *	the VOGLE curve routine.
 */
void
endslopes(
	float	a,
	float	b)
{
	plotdev.s1 = a;
	plotdev.sn = b;
	plotdev.splinetype = CLAMPED;
}

/*
 * gridspacing
 *
 *	Turns grids on or off
 */
void
gridspacing(
	int	spacing)
{
	plotdev.grid = spacing;
}

/*
 * scaling
 *
 *	Specify the type of scaling to be used for axis drawing
 * and graph plotting.
 */
void
scaling(
	int	type,
	char	axis)
{
	char	err_buf[EBUF_SIZE];

	switch (axis) {
	case 'x':
	case 'X':
		plotdev.axes[XIND].scaling = type;
		break;
	case 'y':
	case 'Y':
		plotdev.axes[YIND].scaling = type;
		break;
	case 'z':
	case 'Z':
		plotdev.axes[ZIND].scaling = type;
		break;
	case 's':
	case 'S':
		plotdev.axes[XSEC].scaling = type;
		break;
	case 't':
	case 'T':
		plotdev.axes[YSEC].scaling = type;
		break;
	default:
		sprintf(err_buf, "scaling: unknown axis '%c'", axis);
		vopl_error(err_buf);
	}
}

/*
 * tickmarks
 *
 *	Specify the number of tickmarks on an axis. For autoscaled
 *	axes (ie those using "adjustscale"), the number of tick marks
 *	may not come out exactly as specified.
 */
void
tickmarks(
	int	num,
	char	axis)
{
	char	err_buf[EBUF_SIZE];

	if (num < 0) {
		sprintf(err_buf,"tickmarks: number of tickmarks on axis '%c' must be >= 0 - was %d", axis, num);
		vopl_error(err_buf);
	} else {
		switch (axis) {
		case 'x':
		case 'X':
			plotdev.axes[XIND].nticks = num;
			plotdev.axes[XIND].forceticks = num ? 1 : 0;		
			break;
		case 'y':
		case 'Y':
			plotdev.axes[YIND].nticks = num;
			plotdev.axes[YIND].forceticks = num ? 1 : 0;		
			break;
		case 'z':
		case 'Z':
			plotdev.axes[ZIND].nticks = num;
			plotdev.axes[ZIND].forceticks = num ? 1 : 0;		
			break;
		case 's':
		case 'S':
			plotdev.axes[XSEC].nticks = num;
			plotdev.axes[XSEC].forceticks = num ? 1 : 0;		
			break;
		case 't':
		case 'T':
			plotdev.axes[YSEC].nticks = num;
			plotdev.axes[YSEC].forceticks = num ? 1 : 0;		
			break;
		default:
			sprintf(err_buf,"tickmarks: unknown axis '%c'", axis);
			vopl_error(err_buf);
		}

	}
}

/*
 * tickspacing
 *
 *	Specify the spacing of tickmarks on an axis. For autoscaled
 *	axes (ie those using "adjustscale"), the number of tick marks
 *	may not come out exactly as specified.
 */
void
tickspacing(
	int	num,
	char	axis)
{
	char	err_buf[EBUF_SIZE];

	if (num < 0) {
		sprintf(err_buf,"tickmarks: number of tickmarks on axis '%c' must be >= 0 - was %d", axis, num);
		vopl_error(err_buf);
	} else {
		switch (axis) {
		case 'x':
		case 'X':
			plotdev.axes[XIND].ntspacing = num;
			break;
		case 'y':
		case 'Y':
			plotdev.axes[YIND].ntspacing = num;
			break;
		case 'z':
		case 'Z':
			plotdev.axes[ZIND].ntspacing = num;
			break;
		case 's':
		case 'S':
			plotdev.axes[XSEC].ntspacing = num;
			break;
		case 't':
		case 'T':
			plotdev.axes[YSEC].ntspacing = num;
			break;
		default:
			sprintf(err_buf, "tickspacing: unknown axis '%c'", axis);
			vopl_error(err_buf);
		}
	}
}


/*
 * minorticks
 *
 *	Specify the number of minortickmarks bewteen each tick on an axis. 
 * 	On a log scaled axis, this simply says to draw/not draw the minor
 *	minor tick marks ... their number being always set to 10.
 */
void
minorticks(
	int	num,
	char	axis)
{
	char	err_buf[EBUF_SIZE];

	if (num < 0) {
		sprintf(err_buf,"minorticks: number of minor tickmarks on axis '%c' must be >= 0 - was %d", axis, num);
		vopl_error(err_buf);
	} else {
		switch (axis) {
		case 'x':
		case 'X':
			plotdev.axes[XIND].minorticks = num;
			break;
		case 'y':
		case 'Y':
			plotdev.axes[YIND].minorticks = num;
			break;
		case 'z':
		case 'Z':
			plotdev.axes[ZIND].minorticks = num;
			break;
		case 's':
		case 'S':
			plotdev.axes[XSEC].minorticks = num;
			break;
		case 't':
		case 'T':
			plotdev.axes[YSEC].minorticks = num;
			break;
		default:
			sprintf(err_buf,"minorticks: unknown axis '%c'", axis);
			vopl_error(err_buf);
		}
	}
}

/*
 * annotate
 *
 *	Turn on or off labeling of the drawn axes
 */
void
annotate(
	char	*format,
	char	axis)
{
	axisdata	*ax;
	char		err_buf[EBUF_SIZE];


	/*
	 * wooh Jimmy!
	 */
	switch (axis) {
	case 'x':
	case 'X':
		ax = &plotdev.axes[XIND];
		break;
	case 'y':
	case 'Y':
		ax = &plotdev.axes[YIND];
		break;
	case 'z':
	case 'Z':
		ax = &plotdev.axes[ZIND];
		break;
	case 's':
	case 'S':
		ax = &plotdev.axes[XSEC];
		break;
	case 't':
	case 'T':
		ax = &plotdev.axes[YSEC];
		break;
	default:
		sprintf(err_buf, "annotate: unknown axis '%c'", axis);
		vopl_error(err_buf);
	}

	ax->annotate = (format ? 1 : 0);
	ax->format = savestr(ax->format, format);
}

void
anotatemod(
	int	i,
	char	a)
{
	axisdata	*ax;

	switch(a) {
	case 'x':
	case 'X':
		ax = &plotdev.axes[XIND];
		break;
	case 'y':
	case 'Y':
		ax = &plotdev.axes[YIND];
		break;
	case 'z':
	case 'Z':
		ax = &plotdev.axes[ZIND];
		break;
	}

	ax->mod = i;
}

/*
 * arrayindex
 *
 *	How to index into the arrays passed to adjustscale and plot
 *	eg, increment the index by one or by more.
 *
 */
void
arrayindex(
	int	i)
{
	plotdev.arrayind = i;
}

/*
 * marker
 *
 *	Defines the current marker string to be used on subsequent
 *	plot calls.
 */
void
marker(
	char	*string)
{
	plotdev.marker = savestr(plotdev.marker, string);
}
		
/*
 * markerspacing
 *
 *	Sets the spacing for the markers.
 */
void
markerspacing(
	int	spacing)
{
	plotdev.markerspacing = spacing;
}

/*
 * markerscale
 *
 *	Defines a multiplicative scale factor (from the default) for
 *	the size of markers drawn.
 */
void
markerscale(
	float	s)
{
	plotdev.markerscale = s;
}


/*
 * graphtitle
 *
 *	sets the title for the graph
 */
void
graphtitle(
	char	*s)
{
	plotdev.graphtitle = savestr(plotdev.graphtitle, s);
}
		

/*
 * drawtitle
 *
 * 	Draws a title for the graph.
 */
void
drawtitle(void)
{
	if (plotdev.graphtitle == (char *)NULL)
		return;

	pushattributes();
	pushmatrix();
	pushviewport();

	ortho2(-1.0, 1.0, -1.0, 1.0);

	textsize(TEXTWIDTH * 1.5, TEXTHEIGHT * 1.5);

	move2(0.5 * (XMAX - XMIN) + XMIN, YMAX + 2 * TEXTHEIGHT);
	centertext(1);

	drawstr(plotdev.graphtitle);

	centertext(0);
	popviewport();
	popmatrix();
	popattributes();
}


/*
 * range
 *
 *	explicitly set the range of an axis
 */
void
range(
	float	min,
	float	max,
	char	axis)
{
	axisdata	*ax;
	char		err_buf[EBUF_SIZE];


	switch (axis) {
	case 'x':
	case 'X':
		ax = &plotdev.axes[XIND];
		ax->div = (WhatX(max) - WhatX(min)) / ax->nticks;
		break;
	case 'y':
	case 'Y':
		ax = &plotdev.axes[YIND];
		ax->div = (WhatY(max) - WhatY(min)) / ax->nticks;
		break;
	case 'z':
	case 'Z':
		ax = &plotdev.axes[ZIND];
		ax->div = (WhatZ(max) - WhatZ(min)) / ax->nticks;
		break;
	case 's':
	case 'S':
		ax = &plotdev.axes[XSEC];
		ax->div = (WhatXs(max) - WhatXs(min)) / ax->nticks;
		break;
	case 't':
	case 'T':
		ax = &plotdev.axes[YSEC];
		ax->div = (WhatYs(max) - WhatYs(min)) / ax->nticks;
		break;
	default:
		sprintf(err_buf, "range: unknown axis '%c'", axis);
		vopl_error(err_buf);
	}

	ax->min = min;
	ax->max = max;

	if (ax->scaling == LINEAR)
		ax->div = (max - min) / ax->nticks;
	else
		ax->div = (log10((double)max) - log10((double)min)) / ax->nticks;

	ax->scaleset = 1;
}
