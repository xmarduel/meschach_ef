#include <stdio.h>
#include "vogle.h"

void drawstuff(void);
void drawstuff2(float ang);

/*
 * demonstrate still more features of text
 */
int
main(int argc, char *argv[])
{
	char	dev[20];

	printf("Enter device: ");
	gets(dev);
	vinit(dev);

	if (argc == 2)
		font(argv[1]);
	else
		font("futura.l");

	textsize(0.03, 0.04);

	ortho2(0.0, 1.0, 0.0, 1.0);

	color(RED);
	clear();

	drawstuff();

	/* Now do it all with the text rotated .... */

	textang(45.0);
	drawstuff();

	textang(160.0);
	drawstuff();

	textang(270.0);
	drawstuff();

	/* Now with a single character */

	textjustify(0);

	drawstuff2(0.0);

	drawstuff2(90.0);

	drawstuff2(160.0);

	drawstuff2(270.0);

	vexit();

	return(0);
}

void
drawstuff(void)
{
	color(BLACK);
	polyfill(1);	/* So rect clears a bit for us */
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	color(GREEN);
	move2(0.5, 0.5);
	leftjustify();
	drawstr("This is Left Justified text");

	getkey();

	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	color(YELLOW);
	move2(0.5, 0.5);
	centertext(1);
	drawstr("This is Centered text");
	centertext(0);

	getkey();

	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	color(MAGENTA);
	move2(0.5, 0.5);
	rightjustify();
	drawstr("This is Right Justified text");
	textjustify(0);

	getkey();
}

void
drawstuff2(float ang)
{
	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	textang(ang);

	color(GREEN);
	move2(0.5, 0.5);
	leftjustify();
	drawchar('B');

	textang(0.0);
	textjustify(0);
	boxtext(0.1, 0.1, 0.4, 0.02, "The 'B' should be leftjustified");
	
	getkey();

	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	textang(ang);
	color(YELLOW);
	move2(0.5, 0.5);
	centertext(1);
	drawchar('B');
	centertext(0);

	textang(0.0);
	textjustify(0);
	boxtext(0.1, 0.1, 0.4, 0.02, "The 'B' should be centered");

	getkey();

	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	textang(ang);
	color(MAGENTA);
	move2(0.5, 0.5);
	rightjustify();
	drawchar('B');

	textang(0.0);
	textjustify(0);
	boxtext(0.1, 0.1, 0.4, 0.02, "The 'B' should be rightjustified");

	getkey();

	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	textang(ang);
	color(MAGENTA);
	move2(0.5, 0.5);
	topjustify();
	drawchar('B');

	textang(0.0);
	textjustify(0);
	boxtext(0.1, 0.1, 0.4, 0.02, "The 'B' should be topjustified");

	getkey();

	color(BLACK);
	rect(0.1, 0.1, 0.9, 0.9);
	color(WHITE);
	move2(0.1, 0.5);
	draw2(0.9, 0.5);
	move2(0.5, 0.1);
	draw2(0.5, 0.9);

	textang(ang);
	color(MAGENTA);
	move2(0.5, 0.5);
	topjustify();
	rightjustify();
	drawchar('B');

	textang(0.0);
	textjustify(0);
	boxtext(0.1, 0.1, 0.4, 0.02, "The 'B' should be right/topjustified");

	textjustify(0);

	getkey();

}
