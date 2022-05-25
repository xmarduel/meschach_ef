#include <stdio.h>
#include "vogle.h"
#include "math.h"

void drawscene(void);
void drawbox(float s);
void drawsine(float s);

/*
 * A program showing basic line styles.
 */
int
main(int ac, char *av[])
{
	char	device[20];

	fprintf(stderr,"Enter output device: ");
	gets(device);

	vinit(device);		/* set up device */
	vsetflush(0);
	linewidth(THICK);
	up(0.0, 1.0, 0.0);
	perspective(90.0, 1.0, 0.3, 3.0);
	translate(0.0, 0.0, -1.3);

	drawscene();
	rotate(-30.0, 'y');
	rotate(-30.0, 'x');
	linestyle("");
	drawscene();

	vexit();		/* set the screen back to its original state */

	return(0);
}

void
drawscene(void)
{
	color(BLACK);		/* set current color */
	clear();		/* clear screen to current color */

	color(GREEN);
	setdash(0.03);
			/* 2 d move to start where we want drawstr to start */

	xcentertext();
	move2(-0.45, 0.9);
	drawstr("Linestyle: \"10\"");
	move2(-0.45, 0.7);
	drawstr("Linestyle: \"110\"");
	move2(-0.45, 0.5);
	drawstr("Linestyle: \"111010\"");
	move2(-0.45, 0.3);
	drawstr("Linestyle: \"0001\"");

	linestyle("10");
	move2(-0.9, 0.9);
	draw2( 0.0, 0.9);
	circle(0.6, 0.6, 0.4);	/* Should be pi * 0.2 /0.03 dashes ~= 21 */

	drawbox(0.9);
	drawsine(0.9);

	color(RED);
	linestyle("110");
	move2(-0.9, 0.7);
	draw2( 0.0, 0.7);
	circle(0.6, 0.6, 0.3);
	drawbox(0.7);
	drawsine(0.7);

	color(CYAN);
	linestyle("111010");
	move2(-0.9, 0.5);
	draw2( 0.0, 0.5);
	circle(0.6, 0.6, 0.2);
	drawbox(0.5);
	drawsine(0.5);

	color(YELLOW);
	linestyle("0001");
	move2(-0.9, 0.3);
	draw2( 0.0, 0.3);
	circle(0.6, 0.6, 0.1);
	drawbox(0.3);
	drawsine(0.3);

	getkey();		/* wait for some input */

}

void
drawbox(float s)
{
	pushmatrix();

	rotate(30.0, 'x');
	rotate(60.0, 'y');
	translate(-0.7, -1.2, 0.0);
	scale(s, s, s);

	move(0.0, 0.0, 0.0);

	draw(1.0, 0.0, 0.0);
	draw(1.0, 1.0, 0.0);
	draw(0.0, 1.0, 0.0);
	draw(0.0, 0.0, 0.0);

	draw(0.0, 0.0, -1.0);
	draw(1.0, 0.0, -1.0);
	draw(1.0, 1.0, -1.0);
	draw(0.0, 1.0, -1.0);
	draw(0.0, 0.0, -1.0);

	move(0.0, 1.0, -1.0);
	draw(0.0, 1.0, 0.0);

	move(1.0, 1.0, 0.0);
	draw(1.0, 1.0, -1.0);

	move(1.0, 0.0, 0.0);
	draw(1.0, 0.0, -1.0);

	popmatrix();
}

#define RAD 0.5
#define AMP 0.03

void
drawsine(float s)
{
	float	a, x, y, z;

	pushmatrix();

	translate(RAD + 0.2, -0.5, 0.0);
	scale(s, s, s);

	move(RAD, 0.0, 0.0);
	for (a = 0.0; a <= 2 * 3.1415926; a += 0.02) {
		x = RAD * cos(a);
		z = RAD * sin(a);
		y = AMP * sin(a * 6.0);

		draw(x, y, z);
	}

	popmatrix();
}
