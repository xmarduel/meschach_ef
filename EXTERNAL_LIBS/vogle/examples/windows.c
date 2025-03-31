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
	int	skip;
	int	w1, w2, w3, w4;
	Vevent	vev;

	prefsize(400, 300);
	prefposition(30, 30);
	w1 = winopen("", "Window 1");
	color(BLACK);
	clear();
	prefsize(300, 300);
	prefposition(400, 30);
	w2 = winopen("", "Window 2");
	color(BLACK);
	clear();
	prefposition(30, 400);
	prefsize(500, 300);
	w3 = winopen("", "Window 3");
	color(BLACK);
	clear();
	prefsize(400, 400);
	prefposition(400, 400);
	w4 = winopen("", "Window 4");
	color(BLACK);
	clear();

	textsize(0.5, 0.9);
	font("futura.m");

	makecube();

	/*
	 * set up an object which draws in the top left of the screen.
	 */
	makeobj(TOPLEFT);
		ortho2(-5.0, 5.0, -5.0, 5.0);

		color(BLACK);
		clear();

		color(RED);

		rect(-5.0, -5.0, 5.0, 5.0);

		perspective(40.0, 1.0, 0.1, 1000.0);
		lookat(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

		callobj(CUBE);

		color(GREEN);

		move2(-4.5, -4.5);
		drawstr("perspective/lookat");
	closeobj();

	/*
	 * now set up one which draws in the top right of the screen
	 */
	makeobj(TOPRIGHT);
		ortho2(-5.0, 5.0, -5.0, 5.0);

		color(BLACK);
		clear();

		color(GREEN);

		rect(-5.0, -5.0, 5.0, 5.0);

		window(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
		lookat(5.0, 8.0, 5.0, 0.0, 0.0, 0.0, 0.0);

		callobj(CUBE);

		color(RED);

		move2(-4.5, -4.5);
		drawstr("window/lookat");
	closeobj();

	/*
	 * try the bottom left
	 */
	makeobj(BOTTOMLEFT);
		ortho2(-5.0, 5.0, -5.0, 5.0);

		color(BLACK);
		clear();

		color(MAGENTA);

		rect(-5.0, -5.0, 5.0, 5.0);

		perspective(40.0, 1.0, 0.1, 1000.0);
		polarview(15.0, 30.0, 30.0, 30.0);

		callobj(CUBE);

		color(YELLOW);

		move2(-4.5, -4.5);
		drawstr("perspective/polarview");
	closeobj();

	/*
	 * and the bottom right
	 */
	makeobj(BOTTOMRIGHT);
		ortho2(-5.0, 5.0, -5.0, 5.0);

		color(BLACK);
		clear();

		color(CYAN);

		rect(-5.0, -5.0, 5.0, 5.0);

		window(-5.0, 5.0, -5.0, 5.0, -5.0, 5.0);
		polarview(8.0, -18.0, -3.0, 18.0);

		callobj(CUBE);

		color(BLUE);

		move2(-4.5, -4.5);
		drawstr("window/polarview");
	closeobj();

	expandviewport();

	/*
	 * now draw them
	 */
	winset(w1);
	callobj(TOPLEFT);
	winset(w2);
	callobj(TOPRIGHT);
	winset(w3);
	callobj(BOTTOMLEFT);
	winset(w4);
	callobj(BOTTOMRIGHT);
	winset(w1);
	color(RED);

	getkey();
	windel(w3);

	draw(1.0, 1.0, 1.0);

	ortho2(0.0, 1.0, 0.0, 1.0);
	move2(0.0, 0.0);
	drawstr("BLERK");
	getkey();

	unexpandviewport();

	prefposition(-1, -1);
	prefsize(768, 512);
	w3 = winopen("", "Anotehr one");
	printf("Anotehr one = %d\n", w3);
	color(BLACK);
	clear();

	ortho2(-1.0, 1.0, -1.0, 1.0);
	move2(0.0, 0.0);
	color(GREEN);
	drawstr("BLERK");
	skip = 0;
	do {
		int	w;
		w = vgetevent(&vev, 0);
		if (w != -1) {
			winset(w);
			printf("Window = %d\n", w);
			switch (vev.type) {
			case VRESIZE:
				printf("VRESIZE\n");
				calcviewport();
			case VREDRAW:
				printf("VREDRAW\n");
				if (w == w1)
					callobj(TOPLEFT);
				else if (w == w2)
					callobj(TOPRIGHT);
				else if (w == w3)
					callobj(BOTTOMRIGHT);
				else if (w == w4) {
					printf("BOTTOMLEFT\n");
					callobj(BOTTOMLEFT);
				}

				break;
			case VKEYPRESS:
				skip = 1;
				break;
			default:
				;
			}
		}
	} while(vev.type != -1 && !skip);
			
			

	voutput("/tmp/1.ps");
	fprintf(stderr, "doing the cps thimg\n");
	w3 = winopen("cps", "");
	color(RED);
	clear();
	callobj(BOTTOMRIGHT);

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


