
#include <stdio.h>
#include "vogle.h"

#define	CUBE_SIZE	200.0
#define	TRANS		25.0
#define	SCAL		0.1
#define FACE		1
#define FILLED		2
#define OUTLINE		3

extern float	tdir;
extern float	scal;
extern int	but, nplanes;
int	i, n;

setup_lcube()
{
	window(-800.0, 800.0, -800.0, 800.0, -800.0, 800.0);
	lookat(0.0, 0.0, 1500.0, 0.0, 0.0, 0.0, 0.0);

	/*
	 * Start with a very ordinary filled cube like the old demo..
	 */
	polyhatch(0);
	hatchang(45.0);
	hatchpitch(40.0);

	makeobj(FACE); 	/* hatched or filled polygon */
		makepoly();
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		closepoly();
	closeobj();

	makecube(FILLED);

	if ((nplanes = getdepth()) == 1)
		makecube(OUTLINE);
}

makecube(obj)
	int	obj;
{
	makeobj(obj);
		if (obj == OUTLINE) {
			pushattributes();
			polyfill(0);
			polyhatch(0);
			color(BLACK);
		}

		pushmatrix();
			translate(0.0, 0.0, CUBE_SIZE);
			if (obj == FILLED)
				color(RED);

			callobj(FACE);
		popmatrix();

		pushmatrix();
			translate(CUBE_SIZE, 0.0, 0.0);
			rotate(90.0, 'y');
			if (obj == FILLED)
				color(GREEN);

			callobj(FACE);
		popmatrix();

		pushmatrix();
			translate(0.0, 0.0, -CUBE_SIZE);
			rotate(180.0, 'y');
			if (obj == FILLED)
				color(BLUE);

			callobj(FACE);
		popmatrix();

		pushmatrix();
			translate(-CUBE_SIZE, 0.0, 0.0);
			rotate(-90.0, 'y');
			if (obj == FILLED)
				color(CYAN);

			callobj(FACE);
		popmatrix();

		pushmatrix();
			translate(0.0, CUBE_SIZE, 0.0);
			rotate(-90.0, 'x');
			if (obj == FILLED)
				color(MAGENTA);

			callobj(FACE);
		popmatrix();

		pushmatrix();
			translate(0.0, -CUBE_SIZE, 0.0);
			rotate(90.0, 'x');
			if (obj == FILLED)
				color(YELLOW);

			callobj(FACE);
		popmatrix();

		if (obj == OUTLINE)
			popattributes();
		
	closeobj();
}
