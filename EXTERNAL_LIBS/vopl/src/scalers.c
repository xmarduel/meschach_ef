#include "vopl.h"

extern double	pow();
extern double	log10();
static	float	del = 0.000002;

/*
 * ten2pow
 *
 *	Computes 10 to the power of i
 */
static float
ten2pow(
	int	i)
{
	float	a = 1.0;
	int	j;

	if (i > 0)
		for (j = 0; j < i; j++)
			a *= 10.0;
	else if (i < 0) 
		for (j = 0; j > i; j--)
			a /= 10.0;

	return (a);
}
	
/*
 * Algorithm 463 - scale3. (From CACM)
 *
 * given xmin , xmax and n, where n is greater than 1, scale3
 * finds a new range xminp and xmaxp divisible into exactly
 * n LOGARITHMIC intervals, where the ratio of adjacent
 * uniformly spaced scale values is dist.
 */
void
logscale(
	float	xmin,
	float	xmax,
	int	n,
	float	*xminp,
	float	*xmaxp,
	float	*dist)
{
	static	float vint[11] = {
		10.0, 9.0, 8.0, 7.0, 6.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.5
	};

	float	xminl, xmaxl, fn, a, al, b, distl, fm1, fm2;
	int	nal, i, m1, m2, np, nx;

	/*
	 * Check whether proper input values were supplied.
	 */
	if (xmin >= xmax || n <= 0 || xmin <= 0.0) {
		fprintf(stderr, "logscale: Improper values supplied as input\n");
		if (xmin <= 0.0)
			fprintf(stderr, "Zero or negative minimum value input: %f\n", xmin);

		exit(1);
	}

	/*
	 * Values are translated from the linear region to the logarithmic
	 */
	
	xminl = (float)log10((double)xmin);
	xmaxl = (float)log10((double)xmax);
	fn = (float)n;

	/*
	 * Find approximate interval size a
	 */

	a = (xmaxl - xminl) / fn;
	al = (float)log10((double)a);
	nal = (int)al;
	if (a < 1.0)
		nal--;

	/*
	 * A is scaled into the variable named b between 1 and 10
	 */

	b = a / ten2pow(nal);

	/*
	 * The closest permissible value for b is found
	 */
	
/*
	for (i = 0; i < 9 && b < (10.0 / vint[i] + del); i++)
		;
*/
	for (i = 0; i < 9; i++)
		if (b < (10.0 / vint[i] + del)) goto l1;

	i = 9;
l1:
	/*
	 * The interval size is computed
	 */
	
	do {
		distl = ten2pow(nal + 1) / vint[i];
		fm1 = xminl / distl;
		m1 = (int)fm1;
	
		if (fm1 < 0.0) 
			m1--;

		if (ABS((float)m1 + 1.0 - fm1) < del)
			m1--;

		/*
		 * The new minimum and maximum limits are found
		 */

		*xminp = distl * (float)m1;

		fm2 = xmaxl / distl;
		m2 = (int)(fm2 + 1.0);
		if (fm2 < -1.0)
			m2--;
	
		if (ABS(fm2 + 1.0 - (float)m2) < del)
			m2--;

		*xmaxp = distl * (float)m2;


		/*
		 * Check whether another pass is necessary
		 */
		np = m2 - m1;
		i++;
	} while (np > n);

	nx = (n - np) / 2;
	*xminp = *xminp - (float)nx * distl;
	*xmaxp = *xminp + (float)n * distl;

	/*
	 * Values are translated from the logarithmic into the
	 * linear region.
	 */
	
	/**dist = (float)pow((double)10.0, (double)distl);*/
	*dist = distl;
	*xminp = (float)pow((double)10.0, (double)*xminp);
	*xmaxp = (float)pow((double)10.0, (double)*xmaxp);

	/*
	 * Adjust limits to account for round-off if necessary
	 */

	if (*xminp > xmin)
		*xminp = xmin;

	if (*xmaxp < xmax)
		*xmaxp = xmax;

}


/*
 * Algorithm 463 - scale2. (From CACM)
 *
 * given xmin , xmax and n, where n is greater than 1, scale2
 * finds a new range xminp and xmaxp divisible into exactly
 * n LINEAR intervals of size dist, where n is greater than 1.
 */
void
linscale2(
	float	xmin,
	float	xmax,
	int	n,
	float	*xminp,
	float	*xmaxp,
	float	*dist)
{
	static	float vint[5] = {
		1.0, 2.0, 5.0, 10.0, 20.0
	};

	float	fn, a, al, b, fm1, fm2;
	int	nal, i, m1, m2, np, nx;

	/*
	 * Check whether proper input values were supplied.
	 */
	if (xmin >= xmax || n <= 0) {
		fprintf(stderr, "linscale2: improper values supplied as input\n");
		exit(1);
	}

	fn = (float)n;

	/*
	 * Find approximate interval size a
	 */

	a = (xmax - xmin) / fn;
	al = (float)log10((double)a);
	nal = (int)al;
	if (a < 1.0)
		nal--;

	/*
	 * A is scaled into the variable named b between 1 and 10
	 */

	b = a / ten2pow(nal);

	/*
	 * The closest permissible value for b is found
	 */
	
	for (i = 0; i < 3 && b < (vint[i] + del); i++)
		;

	/*
	 * The interval size is computed
	 */
	
	do {
		*dist = ten2pow(nal) * vint[i];
		fm1 = xmin / *dist;
		m1 = (int)fm1;
	
		if (fm1 < 0.0) 
			m1--;

		if (ABS((float)m1 + 1.0 - fm1) < del)
			m1--;

		/*
		 * The new minimum and maximum limits are found
		 */

		*xminp = *dist * (float)m1;

		fm2 = xmax / *dist;
		m2 = (int)(fm2 + 1.0);
		if (fm2 < -1.0)
			m2--;
	
		if (ABS(fm2 + 1.0 - (float)m2) < del)
			m2--;

		*xmaxp = *dist * (float)m2;


		/*
		 * Check whether another pass is necessary
		 */
		np = m2 - m1;
		i++;
	} while (np > n);

	nx = (n - np) / 2;
	*xminp = *xminp - (float)nx * *dist;
	*xmaxp = *xminp + (float)n * *dist;

	/*
	 * Adjust limits to account for round-off if necessary
	 */

	if (*xminp > xmin)
		*xminp = xmin;

	if (*xmaxp < xmax)
		*xmaxp = xmax;
}

/*
 *	Algorithm 463 scale1 (from CACM)
 *	given min, max and n, scaleaxis finds a new range minp and
 *	maxp divisible into approximately n linear intervals
 *	of size dist
 *	vint is an array of acceptable values for dis (times
 *	an integer power of 10)
 *	sqr is an array of geometric neans of adjacent values
 *	of vint, it is used as break points to determine
 *	which vint value to assign to dist
 *
 */
void
linscale1(
	float	min,
	float	max,
	int	n,
	float	*minp,
	float	*maxp,
	float	*dist)
{
	static float	vint[] = { 1.0, 2.0, 5.0, 10.0 };
	static float	sqr[] = { 1.414214, 3.162278, 7.071068 };
	float		fn, a, al, b, fm1;
	int		i,nal,m1;

	/*
	 *	check whether proper input values were supplied 
	 */

	if (min > max || n < 0) {
	    fprintf(stderr," improper input supplied to linscale1 %f %f %d\n",
		    min, max, n);
	    exit(1);
	}

	fn = (float) n;

	/*
	 *	find approximate interval size a 
	 */

	a = (max - min) / fn;
	if (fabs(a) < 0.000001) {
		max += 1.0;
		min -= 1.0;
		a = (max - min) / fn;
	}

	al = log10((double)a);
	nal = (int) al;
	if (a < 1.0) 
	    nal -= 1;

	/*
	 *	a is scaled into variable named b between 1 and 10 
	 */

	b = a / ten2pow(nal);

	/*	
	 *	the closest permissible value for b is found 
	 */

	for (i = 0; b > sqr[i] && i < 3; i++)
		;

	/*
	 *	the interval size is computed  
	 */

  	*dist = vint[i] * ten2pow(nal);
	fm1 = min / *dist;

	m1 = fm1;

	if (fm1 < 0.0) 
	    m1 -= 1;

	if (ABS((m1 + 1.0 - fm1)) < del) 
	    m1 += 1;

	/*
	 *	the new minimum and maximum limits are found
	 */

	*minp = *dist * (float)m1;

	fm1 = max / *dist;
	m1 = (int)(fm1 + 1.0);

	if (fm1 < -1.0 || ABS((fm1 + 1.0 - (float)m1)) < del)
	    m1 -= 1;

	*maxp = *dist * (float)m1;

	/*
	 *	adjust limits to account for round-off if necessary 
	 */

	if (*minp > min) 
	    *minp = min;
	if (*maxp < max)
	    *maxp = max;

}
