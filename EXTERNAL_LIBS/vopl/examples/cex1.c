#include <stdio.h>
#include <math.h>
#include "vopl.h"

#define PI	3.14159265358979
#define	N	300

float	x[N], y[N];

/*
 *	A very simple test program for vopl.
 *
 * 	This one draws a graph of y = sin(x) 0 <= x <= 2 * PI
 */
main()
{
	float	t, dt;
	char	device[30];
	int	i;

/*
 *	Get VOGLE device
 */
	printf("Enter VOGLE device: ");
	gets(device);

/*
 *	Generate the points
 */
	t = 0.0;
	dt = 2 * PI / N;

	for (i = 0; i != N; i++) {
		x[i] = t;
		y[i] = sin(t);
		t = t + dt;
	}

/*
 *	Adjust the scaling according to x and y arrays
 */
	adjustscale(x, N, 'x');
	adjustscale(y, N, 'y');
/*
 *	As we are now about to do some graphics we initialise VOGLE
 *	and clear to BLACK
 */
	vinit(device);
	color(0);
	clear();
/*
 *	Now set the color to GREEN
 */
	color(2);

/*
 *	Draw the default set of axes (in GREEN)
 */
	drawaxes2();
/*
 *	Set color to RED
 */
	color(1);
/*
 *	Draw the Graph
 */
	plot2(x, y, N);
/*
 *	Wait around a bit
 */
	getkey();
/*
 *	Now draw a little one in the top right hand corner
 *	by reseting the VOGLE viewport.
 */
	viewport(0.0, 1.0, 0.0, 1.0);
/*
 *	Draw it again, but do the plot first (in BLUE) then the axes
 *	(in YELLOW)
 */
	color(4);
	plot2(x, y, N);
	color(3);
	drawaxes2();
/*
 *	Hang around again
 */
	getkey();
/*
 * bugger off
 */
	vexit();
}
