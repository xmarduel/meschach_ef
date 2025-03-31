
#include <stdio.h>
#include <vogle.h>

#define	CUBE_SIZE	200.0
#define	TRANS		25.0
#define	SCAL		0.1
#define FACE		1
#define FILLED		2
#define OUTLINE		3

main(argc, argv)
	int	argc;
	char	*argv[];
{
	float	x, y, tdir = TRANS;
	float	scal = 1.0 + SCAL;
	int	but, nplanes;
	int	back, fill, hatch, i, n;


	prefposition(50, 50);
	prefsize(250, 250);

	vinit("mswin");

	window(-800.0, 800.0, -800.0, 800.0, -800.0, 800.0);
	lookat(0.0, 0.0, 1500.0, 0.0, 0.0, 0.0, 0.0);

	/*
	 * Start with a very ordinary filled cube like the old demo..
	 */
	polyhatch(0);
	hatchang(45.0);
	hatchpitch(40.0);
	polyfill(1);

	fill = 1;
	hatch = 0;
	back = 1;

	makeobj(FACE); 	/* hatched or filled polygon */
		makepoly();
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		closepoly();
	closeobj();

	makecube(FILLED);

	if ((nplanes = getdepth()) == 1)
		makecube(OUTLINE);

	backface(1);
		
	if (backbuffer() < 0) {
		verror("lcube: device doesn't support double buffering.\n"); 
	}        

	while((but = slocator(&x, &y)) != 44) {
		pushmatrix();
			rotate(100.0 * x, 'y');
			rotate(100.0 * y, 'x');
			color(BLACK);
			clear();
			callobj(FILLED);	/* The filled or hatched one */
			if (nplanes == 1 && (fill || hatch))
				callobj(OUTLINE);	/* The outline */

		popmatrix();
		swapbuffers();

		switch (but = checkkey()) {
		case 'p':
			voutput("lcube.ps");
			vnewdev("postscript");
			pushmatrix();
				rotate(100.0 * x, 'y');
				rotate(100.0 * y, 'x');
				color(BLACK);
				clear();
				callobj(FILLED);
				if (nplanes == 1 && (fill || hatch))
					callobj(OUTLINE);
			popmatrix();
			vnewdev("mswin");
			(void)backbuffer();
			break;
		case 'f':	/* Toggle filling */
			fill = !fill;
			hatch = 0;
			polyfill(fill);
			break;
		case 'h':	/* Toggle hatching */
			hatch = !hatch;
			fill = 0;
			polyhatch(hatch);
			break;
		case 'b': 	/* Toggle backfacing */
			back = !back;
			backface(back);
			break;
		case 's':
			scale(scal, scal, scal);
			break;
		case 'x':
			translate(tdir, 0.0, 0.0);
			break;
		case 'y':
			translate(0.0, tdir, 0.0);
			break;
		case 'z':
			translate(0.0, 0.0, tdir);
			break;
		case '-':
			tdir = -tdir;

			if (scal < 1.0)
				scal = 1.0 + SCAL;
			else
				scal = 1.0 - SCAL;

			break;
		case '+':
			tdir = TRANS;
			break;
		case 27: /* ESC */
		case 'q':
			vexit();
			exit(0);
		default:
			;
		}
	}

	vexit();
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
