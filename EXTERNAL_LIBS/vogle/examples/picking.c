#include <stdio.h>
#include <stdlib.h>
#include "vogle.h"

#ifdef	TC
#define	poly	_poly
#endif 

#include <math.h>
#include <stdlib.h>

#ifdef TC
#undef	poly
#define	poly	poly
#endif

#define RADIUS 10.0

#define	SPHERE	10

#define	ONE	1
#define	TWO	2
#define	THREE	3
#define	FOUR	4
#define	FIVE	5
#define	SIX	6
#define	TEXT	7
#define	POL	8

void drawpicked(int i);

/*
 * makesphere
 *
 *	make a sphere object
 */
void
makesphere(void)
{
	float	i, r, z, a;

	makeobj(SPHERE);

		/*
		 * create the latitudinal rings
		 */
		for (i = 0; i < 180; i += 40) {
			pushmatrix();
				rotate(i, 'y');
				circle(0.0, 0.0, RADIUS);
			popmatrix();
		}
		
		/*
		 * create the longitudinal rings
		 */
		pushmatrix();
			rotate(90.0, 'x');
			for (a = -90.0; a < 90.0; a += 40.0) {
				r = RADIUS * cos((double)a * PI / 180.0);
				z = RADIUS * sin((double)a * PI / 180.0);
				pushmatrix();
					translate(0.0, 0.0, -z);
					circle(0.0, 0.0, r);
				popmatrix();	
			}
		popmatrix();

	closeobj();
}

/*
 * a demonstration of objects
 */
int
main(void)
{
	char	dev[20];
	int	but;
	float	x, y;

	fprintf(stderr,"Enter device: ");
	gets(dev);

	vinit(dev);

	vsetflush(0);
	viewport(0.0, 1.0, 0.0, 1.0);
	/*viewport(-1.0, 1.0, -1.0, 1.0);*/

	/*
	 * set up our viewing transformation
	 */
	perspective(90.0, 1.0, 0.001, 500.0);
	lookat(13.0, 13.0, 8.0, 0.0, 0.0, 0.0, 90.0);

	backbuffer();

	color(BLACK);
	clear();

	color(WHITE);
	srect(-1.0, -1.0, 1.0, 1.0);
	srect(-0.5, -0.5, 0.5, 0.5);

	/*
	 * Call a routine to make the sphere object
	 */
	makesphere();

	/*
	 * Now draw the sphere object scaled down. We use the pushmatrix
	 * and the popmatrix to preserve the transformation matrix so
	 * that only this sphere is drawn scaled.
	 */

	makeobj(ONE);
		color(CYAN);
		pushmatrix();
			scale(0.5, 0.5, 0.5);
			callobj(SPHERE);
		popmatrix();
	closeobj();

	/*
	 * now we draw the same sphere translated, with a different
	 * scale and color.
	 */


	makeobj(TWO);
		color(WHITE);
		pushmatrix();
			translate(0.0, -1.4 * RADIUS, 1.4 * RADIUS);
			scale(0.3, 0.3, 0.3);
			callobj(SPHERE);
		popmatrix();
	closeobj();

	/*
	 * and maybe a few more times....
	 */



	makeobj(THREE);
		color(RED);
		pushmatrix();
			translate(0.0, RADIUS, 0.7 * RADIUS);
			scale(0.2, 0.2, 0.2);
			callobj(SPHERE);
		popmatrix();
	closeobj();


	makeobj(FOUR);
		color(GREEN);
		pushmatrix();
			translate(0.0, 1.5 * RADIUS, -RADIUS);
			scale(0.15, 0.15, 0.15);
			callobj(SPHERE);
		popmatrix();
	closeobj();


	makeobj(FIVE);
		color(YELLOW);
		pushmatrix();
			translate(0.0, -RADIUS, -RADIUS);
			scale(0.12, 0.12, 0.12);
			callobj(SPHERE);
		popmatrix();
	closeobj();


	makeobj(SIX);
		color(BLUE);
		pushmatrix();
			translate(0.0, -2.0*RADIUS, -RADIUS);
			scale(0.3, 0.3, 0.3);
			callobj(SPHERE);
		popmatrix();
	closeobj();

	makeobj(POL);
		color(MAGENTA);
		pushmatrix();
			translate(RADIUS, -3.0*RADIUS, RADIUS);
			polyfill(1);
			makepoly();
			rect(RADIUS, -RADIUS, RADIUS, RADIUS);
			closepoly();
			polyfill(0);
		popmatrix();
	closeobj();

	font("times.rb");
	makeobj(TEXT);
		color(WHITE);
		/* The next line will stuff up the picking */
		ortho2(0.0, 1.0, 0.0, 1.0);
		centertext(1);
		textsize(0.08, 0.15);
		move2(0.8, 0.5);
		textang(-90.0);
		drawstr("I'm very ordinary!");
	closeobj();



		color(CYAN);
	callobj(ONE);

	callobj(TWO);

	callobj(THREE);

	callobj(FOUR);

	callobj(FIVE);

	callobj(SIX);

	callobj(POL);

/*
	callobj(TEXT);
*/

	swapbuffers();


	setpicksize(0.01, 0.01);
	while((but = slocator(&x, &y)) != 4) {
		pickmode(1);

		if (but == 1) {
			/*
			color(WHITE);
			srect(x -0.01, y -0.01, x + 0.01, y + 0.01);
			*/
			setpickat(x, y);

			callobj(ONE);
			if (picked()) {
				/*printf("You picked the CYAN one\n");*/
				drawpicked(ONE);
				continue;
			}
			callobj(TWO);
			if (picked()) {
				/*printf("You picked the WHITE one\n");*/
				drawpicked(TWO);
				continue;
			}
			callobj(THREE);
			if (picked()) {
				/*printf("You picked the RED one\n");*/
				drawpicked(THREE);
				continue;
			}
			callobj(FOUR);
			if (picked()) {
				/*printf("You picked the GREEN one\n");*/
				drawpicked(FOUR);
				continue;
			}
			callobj(FIVE);
			if (picked()) {
				/*printf("You picked the YELLOW one\n");*/
				drawpicked(FIVE);
				continue;
			}
			callobj(SIX);
			if (picked()) {
				/*printf("You picked the BLUE one\n");*/
				drawpicked(SIX);
				continue;
			}
			callobj(POL);
			printf("POL\n");
			if (picked()) {
				/*printf("You picked the MAGENTA polygon\n");*/
				goto fart;
			}
/*
			callobj(TEXT);
			printf("TEXT\n");
			if (picked()) {
				printf("You picked the TEXT\n");
				goto fart;
			}
*/

			printf("You didn't pick anything\n");
fart: ;
		}
		pickmode(0);
	}

	vexit();

	return(0);
}

void
drawpicked(int i)
{
	/* Flash it as hi-lighted */
	pickmode(0);
	linewidth(THICK);
	frontbuffer();
	callobj(i);
	vflush();
	sleep(1);
	backbuffer();
	swapbuffers();
	pickmode(1);
}
