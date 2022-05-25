/*
 *	A Present for Eric
 *
 *	(An example of Software Reuse in action :-)
 */

#include <stdlib.h>
#ifdef TC
extern double sin(), cos();
#else
#include <math.h>
#endif
#include "vogle.h"

void makebeer(void);

#define	BEERGLASS	1

int
main(int argc, char *argv[])
{
	char	c, dev[20];
	float	rotval = 0.0, drotval = 10.0, zeye = 5.0;
	float	R = 1.6, tx = 0.0, tz = R;
	char	device[10];

	prefsize(400, 400);

	fprintf(stderr,"Enter output device: ");
	gets(device);

	vinit(device);          /* set up device */

	font("futura.l");

	makebeer();

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
		vexit();
		fprintf(stderr, "The device '%s' can't support doublebuffering\n", vgetdev(dev));
		exit(1);
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

			color(GREEN);
			move(0.0, 0.0, 0.0);
			boxtext(-0.9 * R, -0.1 * R, 1.8 * R, 0.2 * R, "Don't spill it!");
			
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
				callobj(BEERGLASS);
			popmatrix();

			tz = R * cos((double)(rotval * 3.1415926535 / 180));
			tx = R * sin((double)(rotval * 3.1415926535 / 180));

			swapbuffers();

			c = checkkey();

			if (c != 0) {
				vexit();
				exit(0);
			}
		}

	} while (1);
}

#define	SIZE 1.5
#define HEAD 0.2
#define TOPRAD 0.4
#define BOTRAD 0.3

/*
 * makebeer
 *
 *	generate a beer glass
 */
void
makebeer(void)
{
	int	i;

	makeobj(BEERGLASS);

	color(WHITE);
	pushmatrix();
		rotate(90.0, 'x');
		translate(0.0, 0.0, -SIZE / 2.0);
		circle(0.0, 0.0, TOPRAD);
		translate(0.0, 0.0, -HEAD);
		circle(0.0, 0.0, TOPRAD);
	popmatrix();

	for (i = 0; i < 180; i += 30)
	{
		pushmatrix();
			rotate((float) i, 'y');
			color(YELLOW);
			move(TOPRAD, SIZE / 2.0, 0.0);
			draw(BOTRAD, -SIZE / 2.0, 0.0);
			draw(-BOTRAD, -SIZE / 2.0, 0.0);
			draw(-TOPRAD, SIZE / 2.0, 0.0);
			color(WHITE);
			draw(-TOPRAD, SIZE / 2.0 + HEAD, 0.0);
			draw(TOPRAD, SIZE / 2.0 + HEAD, 0.0);
			draw(TOPRAD, SIZE / 2.0, 0.0);
			draw(-TOPRAD, SIZE / 2.0, 0.0);
		popmatrix();
	}
	pushmatrix();
		color(YELLOW);
		rotate(90.0, 'x');
		translate(0.0, 0.0, SIZE / 2.0);
		for (i = 0; i < 6; i++)
		{
			circle(0.0, 0.0,
			       BOTRAD + (TOPRAD - BOTRAD) * (float) i / 6.0);
			translate(0.0, 0.0, -SIZE / 6.0);
		}
	popmatrix();

	closeobj();
}
