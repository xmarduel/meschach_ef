#include <stdio.h>

/*
 * C routines to test vopl Fortran/C interface
 */

void
axistitle(t, ax)
	char	*t, ax;
{
	printf("\naxistitle: %s, %c\n", t, ax);
}

void
drawaxis(ax)
	char	ax;
{
	printf("\ndrawaxis: %c\n", ax);
}

void
drawaxes()
{
	printf("\ndrawaxes:\n");
}
	
void
drawaxes2()
{
	printf("\ndrawaxes2:\n");
}

void
plot2(x, y, n)
	float	x[], y[];
	int	n;
{
	int	i;
	printf("\nplot2: n = %d\n", n);

	for (i = 0; i < n; i++)
		printf("x[%d] = %f, y[%d] = %f\n", i, x[i], i, y[i]);
}

void
adjustscale(x, n, ax)
	float	x[];
	int	n;
	char	ax;
{
	int	i;

	printf("\nadjustscale: n = %d, ax = %c\n", n, ax);
	for (i = 0; i < n; i++)
		printf("x[%d] = %f\n", i, x[i]);
}

void
fit(i)
	int	i;
{
	printf("\nfit: %d\n", i);
}

void
skip(i)
	int	i;
{
	printf("\nskip: %d\n", i);
}

void
endslopes(a, b)
	float	a, b;
{
	printf("\nendslopes: %f %f\n", a, b);
}

void
gridspacing(i)
	int	i;
{
	printf("\ngridspacing: %d\n", i);
}

void
scaling(i, ax)
	int	i;
	char	ax;
{
	printf("\nscaling: %d, %c\n", i, ax);
}

void
tickmarks(i, ax)
	int	i;
	char	ax;
{
	printf("\ntickmarks: %d, %c\n", i, ax);
}

void
tickspacing(i, ax)
	int	i;
	char	ax;
{
	printf("\ntickspacing: %d, %c\n", i, ax);
}

void
annotate(s, ax)
	char	*s;
	char	ax;
{
	printf("\nannotate: %s, %c\n", s, ax);
}

void
arrayindex(i)
	int	i;
{
	printf("\narrayindex: %d\n", i);
}

void
axisintersection(a, b)
	float	a, b;
{
	printf("\naxisintersection: %f %f\n", a, b);
}

void
marker(s)
	char	*s;
{
	printf("\nmarker: %s\n", s);
}

void
markerspacing(i)
	int	i;
{
	printf("\nmarkerspacing: %d\n", i);
}

void
markerscale(i)
	float	i;
{
	printf("\nmarkerscale: %f\n", i);
}

void
graphtitle(s)
	char	*s;
{
	printf("\ngraphtitle: %s\n", s);
}

void
drawtitle()
{
	printf("\ndrawtitle:\n");
}

void
range(a, b, ax)
	float	a, b;
	char	ax;
{
	printf("\nrange: %f %f %c\n", a, b, ax);
}

void
degree(i)
	int	i;
{
	printf("\ndegree: %d\n", i);
}
