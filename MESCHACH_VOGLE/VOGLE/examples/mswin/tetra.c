/*
 * Demonstrate a rotating translating tetrahedron.
 */

#include <stdio.h>
#ifdef TC
extern double sin(), cos();
#else
#include <math.h>
#endif
#include "vogle.h"

#define	TETRAHEDRON	1
#define	NSIDES	3
#define	NFACES	4
#define	NPNTS	4

float	pnts[NPNTS][3] = {
	{-0.5, 0.866, -0.667},
	{-0.5, -0.866, -0.667},
	{ 1.0, 0.0, -0.667},
	{ 0.0, 0.0, 1.334}
};

int	faces[NFACES][NSIDES] = {
	{2, 1, 0},
	{0, 1, 3},
	{1, 2, 3},
	{2, 0, 3}
};

int	colface[NFACES] = {
		GREEN,
		YELLOW,
		CYAN,
		MAGENTA
};

main(argc, argv)
	int	argc;
	char	**argv;
{
	char	c;
	int	i;
	float	rotval = 0.0, drotval = 5.0, zeye = 5.0;
	float	R = 1.6, tx = 0.0, tz = R;
	int	do_backface;
	int	do_backfacedir;
	int	do_fill;
	char	device[10], *p;

	prefsize(200, 200);

	vinit("mswin");          /* set up device */

	do_fill = 1;
	do_backface = 1;
	do_backfacedir = 1;

	/*
	 * Make the tetrahedral object
	 */
	maketheobject();

	/*
	 * If there is a command line arg to use filled polygons
	 */
	polyfill(do_fill);
	backface(do_backface);
	backfacedir(do_backfacedir);

	/*
	 * set up a perspective projection with a field of view of
	 * 40.0 degrees, aspect ratio of 1.0, near clipping plane 0.1,
	 * and the far clipping plane at 1000.0.
	 */
	perspective(40.0, 1.0, 0.001, 15.0);
	lookat(0.0, 0.0, zeye, 0.0, 0.0, 0.0, 0.0);

	/*
	 * Setup drawing into the backbuffer....
	 */

	if (backbuffer() < 0) {
		verror("The device  can't support doublebuffering\n");
	}

	do {
		for (rotval = 0.0; rotval < 360.0; rotval += drotval) {
			color(BLACK);
			clear();

			/*
			 * Rotate the whole scene...(this acumulates - hence
			 * drotval)
			 */
			rotate(drotval * 0.1, 'x');
			rotate(drotval * 0.1, 'z');

			color(RED);
			pushmatrix();
				polyfill(0);
				rotate(90.0, 'x');
				circle(0.0, 0.0, R);
				polyfill(do_fill);
			popmatrix();

			color(BLUE);
			move(0.0, 0.0, 0.0);
			draw(tx, 0.0, tz);
			
			/*
			 * Remember! The order of the transformations is
			 * the reverse of what is specified here in between
			 * the pushmatrix and the popmatrix. These ones don't
			 * accumulate because of the push and pop.
			 */
			pushmatrix();
				translate(tx, 0.0, tz);
				rotate(rotval, 'x');
				rotate(rotval, 'y');
				rotate(rotval, 'z');
				scale(0.4, 0.4, 0.4);
				callobj(TETRAHEDRON);
			popmatrix();

			tz = R * cos((double)(rotval * 3.1415926535 / 180));
			tx = R * sin((double)(rotval * 3.1415926535 / 180));

			swapbuffers();

			c = checkkey();

			if (c == 'b') {		
				do_backface = !do_backface;
				backface(do_backface);
			} else if (c == 'f') {
				do_fill = !do_fill;
				polyfill(do_fill);
			} else if (c == 'd') {
				do_backfacedir = !do_backfacedir;
				backfacedir(do_backfacedir);
			} else if (c != 0) {
				vexit();
				exit(0);
			}
		}

	} while (1);
		
	vexit();
}

/*
 * maketheobject
 *
 *	generate a tetraedron as a series of move draws
 */
maketheobject()
{
	int	i, j;
	float	x, y, z;

	makeobj(TETRAHEDRON);

		for (i = 0; i < NFACES; i++) {
			color(colface[i]);
			makepoly();
				x = pnts[faces[i][0]][0];
				y = pnts[faces[i][0]][1];
				z = pnts[faces[i][0]][2];
				move(x, y, z);
				for (j = 1; j < NSIDES; j++) {
					x = pnts[faces[i][j]][0];
					y = pnts[faces[i][j]][1];
					z = pnts[faces[i][j]][2];
					draw(x, y, z);
				}
			closepoly();
		}

	closeobj();
}
