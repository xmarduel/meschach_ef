#include <stdio.h>
#include "vogle.h"


/*
 * demonstrate some features of text
 */
int
main(int argc, char *argv[])
{
	char	dev[20];
	
	fprintf(stderr, "Enter device: ");
	gets(dev);

	prefsize(512, 512);
	vinit(dev);

	color(BLACK);
	clear();

	color(YELLOW);

	if (argc == 2)
		font(argv[1]);

	ortho2(0.0, 1.0, 0.0, 1.0);

	boxtext(0.1, 0.9, 0.8, 0.07, "ABCDEFGHJKLMNOPQRSTUVWXYZ");
	boxtext(0.1, 0.8, 0.8, 0.07, "abcdefghjklmnopqrstuvwxyz");
	boxtext(0.1, 0.7, 0.8, 0.07, "1234567890");

	textweight(BOLD);
	boxtext(0.1, 0.6, 0.8, 0.07, "ABCDEFGHJKLMNOPQRSTUVWXYZ");
	boxtext(0.1, 0.5, 0.8, 0.07, "abcdefghjklmnopqrstuvwxyz");
	boxtext(0.1, 0.4, 0.8, 0.07, "1234567890");

	textslant(0.3);
	boxtext(0.1, 0.3, 0.8, 0.07, "ABCDEFGHJKLMNOPQRSTUVWXYZ");
	boxtext(0.1, 0.2, 0.8, 0.07, "abcdefghjklmnopqrstuvwxyz");
	boxtext(0.1, 0.1, 0.8, 0.07, "1234567890");
	getkey();

	vexit();

	return(0);
}
