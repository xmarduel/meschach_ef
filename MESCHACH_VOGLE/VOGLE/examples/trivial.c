#include <stdio.h>
#include <stdlib.h>
#include "vogle.h"

int
verr(int n, const char *s)
{
	if (n == VERR_UNINIT)
		vinit("");
	else if (n == VERR_BADDEV)
		printf("No device\n");
	else
		printf("%s\n", s);

	return(0);
}

/*
 * the basic test program for a driver if we can draw a line and do
 * hardware text we are almost there!
 */
int
main(int argc, char *argv[])
{
	char device[100];
	int	w;

	fprintf(stderr, "Enter output device: ");   /* read in device name */
	gets(device);

	/* verrorhandler(verr); */
	verrignore(1);

	if ((w = vinit(device)) < 0) {
		vgeterrtext(vgeterrno(), device);
		fprintf(stderr, "%s\n", device);
		exit(1);
	}

	printf("w = %d\n", w);

	if (argc == 2)
		font(argv[1]);		/* set font to argument */
	else
		font("large");		/* set font to hardware text large */

	color(BLACK);			/* we want to clear in black */
	clear();			/* clear to current color */

	color(GREEN);			/* set current color to green */

	move2(-1.0, 0.0);		/* draw a horizontal line at y = 0 */
	draw2(1.0, 0.0);

	getkey();			/* pause for some input */

	move2(0.0, 0.0);		/* draw a line along x = 0 */
	draw2(0.0, 1.0);

	move2(0.0, 0.0);		/* move to the middle of the screen */
	drawstr("Hello");		/* draw "Hello" starting at the origin */

	getkey();			/* pause again */

	vexit();			/* set screen back to original state */

	return(0);
}
