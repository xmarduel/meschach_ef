#include "vopl.h"

/*
 * degree
 *
 *	set the order for a least-squares fit
 */
void
degree(
	int	ord)
{
	plotdev.degree = ord;
}

/*
 * avefit
 * 
 *	Draw a line y = average y[i] - used for degree 0
 */
void
avefit(
	float	x[],
	float	y[],
	int	n)
{
	float	a;
	int	i, na;
	
	a = 0.0;
	na = 0;
	for (i = plotdev.startind; i < n; i += plotdev.arrayind) {
		na++;
		a += WhatY(y[i]);
	}

	if (na > 0)
		a /= (float)na;

	move2(WhatX(plotdev.axes[XIND].min), a);
	draw2(WhatY(plotdev.axes[XIND].max), a);
}

/*
 * llsfit
 *
 *	linear least square fit - used for degree 1
 *
 *	y = a + b.x
 *
 *	b =    n.SUM(xi.yi) - SUM(xi).SUM(yi)
 *             -------------------------------
 *	       n.SUM(xi.xi) - SUM(xi).SUM(xi)
 *
 *	a =    yave - b.xave
 */
void
llsfit(
	float	x[],
	float	y[],
	int	n)
{
	float	sumx, sumy, sumxy, sumxx, a, b, yave, xave, an;
	int	i;
	
	sumx = sumy = sumxy = sumxx = 0.0;

	for (i = plotdev.startind; i < n; i += plotdev.arrayind) {
		sumx += WhatX(x[i]);
		sumy += WhatY(y[i]);
		sumxy += WhatX(x[i]) * WhatY(y[i]);
		sumxx += WhatX(x[i]) * WhatY(x[i]);
	}

	b = (n * sumxy - sumx * sumy) / (n * sumxx - sumx * sumx);
	yave = sumy / n;
	xave = sumx / n;
	a = yave - b * xave;

	yave = a + WhatX(plotdev.axes[XIND].min) * b;
	move2(WhatX(plotdev.axes[XIND].min), yave);
	yave = a + WhatX(plotdev.axes[XIND].max) * b;
	draw2(WhatX(plotdev.axes[XIND].max), yave);
}

static float 	*b, *c, *d;

/*
 *  orthofit
 *
 *   Calculate the orthogonal polynomial of the given degree 
 *   through the data set (x, y) - used for degree 2 and above.
 *
 */
int
orthofit(
	float 	*xx,
	float	*yy,
	int 	degree,
	int	npnts)
{
	
	float	*x, *y, *pjm1, *pj;
	float	ax, ay, xdiv; 
	int	j;
	
	/* get memory for variables */

	b = newm1(degree + 1);
	c = newm1(degree + 1);
	d = newm1(degree + 1);
	pjm1 = newm1(npnts);
	pj = newm1(npnts);
	x = newm1(npnts);
	y = newm1(npnts);

	for (j = 0; j < npnts; j++) {
		x[j] = WhatX(xx[j]);
		y[j] = WhatY(yy[j]);
	}

	ortpol(npnts, x, y, pjm1, pj, degree);

	xdiv = (WhatX(plotdev.axes[XIND].max) - WhatX(plotdev.axes[XIND].min)) / (float)plotdev.precision;
	ax = WhatX(plotdev.axes[XIND].min);

	ay = ortval(ax, degree);
	move2(ax, ay);
	for (j = 0; j <= plotdev.precision; j++) {
	 	ax += xdiv;
		ay = ortval(ax, degree);
		draw2(ax, ay);
	}
	
	free(b);
	free(c);
	free(d);
	free(pjm1);
	free(pj);
	free(x);
	free(y);

	return(j);
}

/*
 * ortpol
 *
 *    coeficients for orthogonal polynomial 
 *
 */
int 
ortpol(
	int	npnts,
	float	*x,
	float	*y,
	float	*pjm1,
	float	*pj,
	int	degree)
{
	float	*s, *w;
	float	p;
	int	i, j;

	s = newm1(degree + 1);
	w = newm1(npnts);

	for (j = 0; j <= degree; j++) {
		b[j] = 0.0;
		s[j] = 0.0;
		d[j] = 0.0;
	}

	for (i = 0; i < npnts; i++)
		w[i] = 1.0;

	c[0] = 0.0;

	for (i = 0; i < npnts; i++) {
		d[0] += y[i] * w[i];
		b[0] += x[i] * w[i];
		s[0] += w[i];
	}

	d[0] /= s[0];

	if (degree == 1)
		return(1);

	b[0] /= s[0];
	for (i = 0; i < npnts; i++) {
		pjm1[i] = 1.0;
		pj[i] = x[i] - b[0];
	}

	j = 0;

	for (;;) {
		j++;
		for(i = 0; i < npnts; i++) {
			p = pj[i] * w[i];
			d[j] +=  (y[i] - d[0]) * p;
			p *= pj[i];
			b[j] += x[i] * p;
			s[j] += p;
		}

	        d[j] /= s[j];

		if (j == degree)
			return(1);

		b[j] /= s[j];
		c[j] = s[j] / s[j - 1];
		for (i = 0; i < npnts; i++) {
			p = pj[i];
			pj[i] = (x[i] - b[j]) * pj[i] - c[j] * pjm1[i];
			pjm1[i] = p;
		}
	}
}

/*
 * ortval
 *
 *    calculates the value of the othogonal polynomial
 *    at the point x.
 *
 */
float
ortval(
	float 	x,
	int	degree)
{
	int 	k;
	float 	prev, prev2,  val;
	
	k = degree;
	val = d[k];
	prev = 0.0;
	while (k != 0) {
		k--;
		prev2 = prev;
		prev = val;
		val = d[k] + (x - b[k]) * prev - c[k + 1] * prev2;
	}
	return(val);
}

/*
 * calc_coef
 *   
 *     Note that the following two routines are not used to 
 *     to interpolate for the polynomial fit of the function. They merely
 *     to elicit the values of the coeficients of the polynomial.
 *     
 */
void
calc_coef(
	int	degree,
	float	*coef)
{
		
	float	**p;
	int	i, j;

	p = newm2(degree + 1, degree + 1);

	for (i = 0; i <= degree; i++)
		coef[i] = 0.0;

	p[0][0] = 1.0;
	p[1][0] = -b[0];
	p[1][1] = 1.0;

	coef[0] += d[0] * p[0][0] + d[1] * p[1][0];
	coef[1] += d[1] * p[1][1];

	for (i = 2; i <= degree; i++) {
		bp(i, b[i - 1], c[i - 1], p[i], p[i - 1], p[i - 2]);
		for(j = 0; j <= i; j++) 
			coef[j] += d[i] * p[i][j];
	}

	fprintf(stderr, "coef:");
	for (i = 0; i <= degree; i++)
		fprintf(stderr, " %e\n", coef[i]);
	fprintf(stderr, "\n");

	for (i = 0; i < degree + 1; i++)
		free(p[i]);
	
	free(p);
}

/*
 * bp
 *
 */
void
bp(
	int 	degree,
	float	b,
	float	c,
	float	*p3,
	float	*p2,
	float	*p1)
{
	int i;

	p3[0] = -b * p2[0] - c * p1[0];
	for (i = 1; i <= degree; i++) 
		p3[i] = p2[i - 1] - b * p2[i] - c * p1[i];
}
