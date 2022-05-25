#include <stdio.h>
#include "vogle.h"


#define		CUBE		1
#define		TOPLEFT		2
#define		TOPRIGHT	3
#define		BOTTOMLEFT	4
#define		BOTTOMRIGHT	5

void makecube(void);

/*
 * Demonstrate just how much you can put in an object
 */
int
main(int ac, char *av[])
{
	char device[20];
	int	i, skip;
	int	w1, w2, w3, w4;
	Vevent	vev;
	float	r = 0.0;
	int	w[27];

	prefsize(200, 300);
	for (i = 0; i < 27; i++) {
		sprintf(device, "Window %d", i);
		w[i] = winopen("", device);
		backbuffer();
		color(BLACK);
		clear();
		swapbuffers();
	}

	textsize(0.5, 0.9);
	font("futura.m");

	makecube();

	for (i = 13; i < 19; i++)
		windel(w[i]);

	winclose(20);

	prefsize(100, 100);
	w3 = winopen("", "Anotehr one");
	printf("Anotehr one = %d\n", w3);
	w3 = winopen("", "Anotehr one");
	printf("Anotehr one = %d\n", w3);
	color(BLACK);
	clear();

	perspective(40.0, 1.0, 0.1, 1000.0);
	lookat(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

	skip = 0;
	do {
		int	thew;
		thew = vgetevent(&vev, 0);
		if (thew != -1) {
			winset(thew);
			printf("Window = %d\n", thew);
			switch (vev.type) {
			case VRESIZE:
				printf("VRESIZE\n");
				calcviewport();
			case VREDRAW:
				printf("VREDRAW\n");
				break;
			case VKEYPRESS:
			
				if (vev.data == 'r')
					winraise(20);
				else if (vev.data == 'i')
					winclose(thew);
				else
				skip = 1;
				break;
			default:
				;
			}
		}
		for (i = 0; i < 27; i++) {
			if (vwinidvalid(w[i])) {
				winset(w[i]);
				color(BLACK);
				clear();
				pushmatrix();
					rotate(r, 'y');
					rotate(r, 'z');
					rotate(r, 'x');
					color(GREEN);
					callobj(CUBE);
				popmatrix();
				swapbuffers();

			}
		}
		r += 2.0;
		if (r > 360.0)
			r = 0.0;
	} while(vev.type != -1 && !skip);
			
			

	voutput("/tmp/1.ps");
	fprintf(stderr, "doing the cps thimg\n");
	w3 = winopen("cps", "");
	color(RED);
	clear();
	callobj(TOPLEFT);

	fprintf(stderr, "finished the cps thimg\n");

	getkey();

	fprintf(stderr, "calling vexit\n");
	vexit();
	
	return(0);
}

/*
 * side
 *
 *	define a face for the cube
 */
void
side(void)
{
	pushmatrix();
		translate(0.0, 0.0, 1.0);
		rect(-1.0, -1.0, 1.0, 1.0);
	popmatrix();
}

/*
 * makecube
 *
 *	set up a cube
 */
void
makecube(void)
{

	makeobj(CUBE);

		/*
		 * The border around the cube
		 */
		rect(-5.0, -5.0, 10.0, 10.0);

		/*
		 * Make the cube from 4 squares
		 */
		pushmatrix();
			side();
			rotate(90.0, 'x');
			side();
			rotate(90.0, 'x');
			side();
			rotate(90.0, 'x');
			side();
		popmatrix();

	closeobj();
}


