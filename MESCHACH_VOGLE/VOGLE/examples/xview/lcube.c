
#include <stdio.h>
#include "vogle.h"

#define	CUBE_SIZE	200.0
#define	TRANS		25.0
#define	SCAL		0.1
#define FACE		1
#define FILLED		4
#define HATCHED		8
#define OUTLINE		10

extern float	tdir;
extern float	scal;
extern int	but, nplanes;
int	i, n;


void
makecube(obj)
	int	obj;
{
	polyfill(0);
	polyhatch(0);

	if (obj == FILLED)
		polyfill(1);
	else if (obj == HATCHED)
		polyhatch(1);

	makeobj(obj);
		pushmatrix();
			translate(0.0, 0.0, CUBE_SIZE);
			color(RED);
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		popmatrix();

		pushmatrix();
			translate(CUBE_SIZE, 0.0, 0.0);
			rotate(90.0, 'y');
			color(GREEN);
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		popmatrix();

		pushmatrix();
			translate(0.0, 0.0, -CUBE_SIZE);
			rotate(180.0, 'y');
			color(BLUE);
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		popmatrix();

		pushmatrix();
			translate(-CUBE_SIZE, 0.0, 0.0);
			rotate(-90.0, 'y');
			color(CYAN);
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		popmatrix();

		pushmatrix();
			translate(0.0, CUBE_SIZE, 0.0);
			rotate(-90.0, 'x');
			color(MAGENTA);
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		popmatrix();

		pushmatrix();
			translate(0.0, -CUBE_SIZE, 0.0);
			rotate(90.0, 'x');
			color(YELLOW);
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		popmatrix();

	closeobj();
}

void
setup_lcube()
{
	window(-800.0, 800.0, -800.0, 800.0, -800.0, 800.0);
	lookat(0.0, 0.0, 1500.0, 0.0, 0.0, 0.0, 0.0);

	makecube(FILLED);
	makecube(HATCHED);
	makecube(OUTLINE);

	backbuffer();

	hatchang(45.0);
	hatchpitch(40.0);

}
