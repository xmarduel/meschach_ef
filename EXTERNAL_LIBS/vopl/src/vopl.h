#ifndef ___VOPL_INC

#define ___VOPL_INC 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vogle.h>

#define MAX(a,b)	((a) > (b) ? (a) : (b))
#define MIN(a,b)	((a) < (b) ? (a) : (b))
#define ABS(a)		((a) < 0.0 ? (-a) : (a))

#define	EBUF_SIZE	100		/* size of error message buffer */
#define	LABEL_LEN	50		/* max length of an axis label */

/*
 * fits
 */
#define	NO_LINES	0
#define	STRAIGHT_LINE	1
#define LEAST_SQUARE	2
#define CUBIC_SPLINE	3
#define	POWER_EQN	4
#define	SGR_FIT		5

#define	MAX_FIT		5		/* must be maximum fit number */

/*
 * spline types
 */
#define	FREE		0
#define	CLAMPED		1

/*
 * scalings
 */
#define	LINEAR		0
#define	LOGARITHMIC	1

/*
 * boundaries in which the graph is plotted
 */
#define	XMIN		-0.7
#define	YMIN		-0.7
#define	XMAX		1.0
#define	YMAX		0.8

/*
 * size of line-markers on axes
 */
#define	LINELEN		0.02

/*
 * textsizes
 */
#define	TEXTHEIGHT	0.05
#define	TEXTWIDTH	0.035

/*
 * marker size
 */
#define	MARKERSIZE	0.05

/*
 * A big number (Some PC things don't seem to have this in math.h)
 */
#define BLOODYBIG	1.0e38

extern float	*newm1(), **newm2();

extern float	cardinal[4][4];

/*
 * axis indexes
 */
#define	XIND		0
#define	YIND		1
#define	ZIND		2

#define XSEC            3	/* Index for secondary axes */
#define YSEC            4

#define	AXES		3	/* number of axes */

typedef struct AXISDATA {
	float	min,
		max,
		div;
	int	scaling,
		annotate,
		nticks,
		ntspacing,
		minorticks,
		scaleset,
		forceticks,
		mod;
	char	*format,
		*title;
} axisdata;

typedef struct VOPL {
	float		s1, sn,
			histhatchang,
			markerscale;

	int		fit, 
			degree,
			splinetype,
			histhatch,
			grid, 
			startind, arrayind,
			precision,
			markerspacing;
	char		*marker,
			*graphtitle;

	axisdata	axes[AXES+2];
} vopldev;

extern vopldev	plotdev;

extern	double	log10(double);

extern	char	*savestr();


#define WhatX(x) (plotdev.axes[XIND].scaling == LINEAR ? \
				(x) : (float)log10((double)(x)))
#define WhatY(y) (plotdev.axes[YIND].scaling == LINEAR ? \
				(y) : (float)log10((double)(y)))
#define WhatZ(z) (plotdev.axes[ZIND].scaling == LINEAR ? \
				(z) : (float)log10((double)(z)))
#define WhatXs(z) (plotdev.axes[XSEC].scaling == LINEAR ? \
				(z) : (float)log10((double)(z)))
#define WhatYs(z) (plotdev.axes[YSEC].scaling == LINEAR ? \
				(z) : (float)log10((double)(z)))


void adjustscale(float x[], int n, char axis);
void formatlabel(float alabel, char *form);
void axistitle(char *title, char ax);
void drawaxis(char axis);

void drawaxes2(void);
void drawaxes(void);
void lin_axis(int axis);
void log_axis(int axis);
void cubicsp(float *x, float *y, int n);
void vopl_error(char *str);
void hist2(float *d, int ng, int ns, char *names[]);
void pefit(float *x, float *y, int n);
void sgrfit(float *x, float *y, int n);
void degree(int ord);
void avefit(float *x, float *y, int n);
void llsfit(float *x, float *y, int n);
int orthofit(float *xx, float *yy, int degree, int npnts);
int ortpol(int npnts, float *x, float *y, float *pjm1, float *pj, int degree);
float ortval(float x, int degree);
void calc_coef(int degree, float *coef);
void bp(int degree, float b, float c, float *p3, float *p2, float *p1);
float * newm1(int n);
float ** newm2(int m, int n);
void plot2(float *x, float *y, int n);
char * savestr(char *old, char *string);
void logscale(float xmin, float xmax, int n , float *xminp, float *xmaxp, float *dist);
void linscale2(float xmin, float xmax, int n , float *xminp, float *xmaxp, float *dist);
void linscale1(float min, float max, int n, float *minp, float *maxp, float *dist);
void spline(float *x, float *y, int np);
void skip(int n);
void endslopes(float a, float b);
void gridspacing(int spacing);
void scaling(int type, char axis);
void tickmarks(int num, char axis);
void tickspacing(int num, char axis);
void annotate(char *format, char axis);
void anotatemod(int i, char a);
void arrayindex(int i);
void marker(char *string);
void markerspacing(int spacing);
void markerscale(float s);
void graphtitle(char *s);
void drawtitle(void);
void range(float min, float max, char axis);
#endif
