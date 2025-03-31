#include <stdio.h>
#include "vogle.h"

int
verr(int err, const char *s)
{
	if (err == VERR_BADDEV) {
		printf("Shucks, you entered a bad device");
		return(-1);
	}
	return(0);
}

/*
 * A program showing basic line drawing, text and (if applicable)
 * colour. As none of the projection routines have been called we
 * move and draw in the initial coordinate system -1.0 to 1.0.
 */
int
main(int ac, char *av[])
{
	char	device[10], *p;

	fprintf(stderr,"Enter output device: ");
	gets(device);

	verrorhandler(verr);

	prefsize(300, 300);
	prefposition(100, 100);
	vinit(device);		/* set up device */


	if (ac == 2)
		font(av[1]);	/* change font to the argument */

	color(BLACK);		/* set current color */
	clear();		/* clear screen to current color */

	color(GREEN);
			/* 2 d move to start where we want drawstr to start */
	move2(-0.9, 0.9);

	drawstr("A Simple Example 0 1 2 3 4 5 6 7 8 9");	/* draw string in current color */

	/*
	 * the next four lines draw the x 
	 */
	move2(0.,0.);
	draw2(.76,.76);
	move2(0.,.76);
	draw2(.76,0.);

	move2(0.0,0.5);
	drawstr("x done");
	drawstr("next sentence");

	move2(0.0,0.1);
	for (p = "hello world"; *p != '\0'; p++) 
		drawchar(*p);		/* draw the string one char at a time */

	/*
	 * the next five lines draw the square
	 */
	move2(0.,0.);
	draw2(.76,0.);
	draw2(.76,.76);
	draw2(0.,.76);
	draw2(0.,0.);

	getkey();		/* wait for some input */

	vexit();		/* set the screen back to its original state */

	return(0);
}
