
#include <stdlib.h>
#include <vogle.h>

#define	CUBE_SIZE	200.0
#define	TRANS		25.0
#define	SCAL		0.1
#define FACE		1
#define FILLED		2
#define OUTLINE		3

void makecube(int obj);

int
main(int argc, char *argv[])
{
        char    device[10];
	float	x, y, tdir = TRANS;
	float	scal = 1.0 + SCAL;
	int	but, nplanes;
	int	back, fill, hatch, dodouble;
	int	w, w1, w2, oldwin, tmpwin;
	Vevent	vev;

	fprintf(stderr,"Enter output device: ");
	gets(device);


	prefsize(500, 500);

	w1 = winopen(device, "Lcube 1");
	backbuffer();

	w2 = winopen(device, "Lcube 2");
	backbuffer();

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
	dodouble = 1;

	makeobj(FACE); 	/* hatched or filled polygon */
		makepoly();
			rect(-CUBE_SIZE, -CUBE_SIZE, CUBE_SIZE, CUBE_SIZE);
		closepoly();
	closeobj();

	makecube(FILLED);

	if ((nplanes = getdepth()) == 1)
		makecube(OUTLINE);

	backface(1);
		
	while(1) {
		pushmatrix();
			rotate(100.0 * x, 'y');
			rotate(100.0 * y, 'x');
			winset(w1);
			color(BLACK);
			clear();
			if (hatch)
				polyhatch(!hatch);
			polyfill(!fill);
			callobj(FILLED);	/* The filled or hatched one */
			if (nplanes == 1 && (fill || hatch))
				callobj(OUTLINE);	/* The outline */
			swapbuffers();

			winset(w2);
			color(BLACK);
			clear();
			if (hatch)
				polyfill(fill);
			polyhatch(hatch);
			callobj(FILLED);	/* The filled or hatched one */
			if (nplanes == 1 && (fill || hatch))
				callobj(OUTLINE);	/* The outline */
			swapbuffers();
		popmatrix();

		w = vgetevent(&vev, 1);
		if (vev.type == VMOTION) {
			x = 0.02 * vev.x;
			y = 0.02 * vev.y;
			/* see also VtoWxy(...) .. src/mapping.c */
		} else if (vev.type == VRESIZE) {
			winset(w);
			calcviewport();
		} else if (vev.type == VBUTTONPRESS) {
			if (vev.data == 1) {
				vexit();
				exit(0);
			}
		} else if (vev.type == VKEYPRESS) {
			switch(vev.data) {
			case 'p':
				voutput("lcube.ps");
				oldwin = vgetwin();
				tmpwin = winopen("cps", "");
				pushmatrix();
					rotate(100.0 * x, 'y');
					rotate(100.0 * y, 'x');
					color(BLACK);
					clear();
					callobj(FILLED);
					if (nplanes == 1 && (fill || hatch))
						callobj(OUTLINE);
				popmatrix();
				windel(tmpwin);
				winset(oldwin);
				break;
			case 'f':	/* Toggle filling */
				fill = !fill;
				hatch = 0;
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
			case 'd': 	/* Toggle doublebuffering */
				dodouble = !dodouble;
				if (dodouble) {
					winset(w1);
					frontbuffer();
					winset(w2);
					frontbuffer();
				} else {
					winset(w1);
					backbuffer();
					winset(w2);
					backbuffer();
				}
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
	}

	vexit();

	return(0);
}

void
makecube(int obj)
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
