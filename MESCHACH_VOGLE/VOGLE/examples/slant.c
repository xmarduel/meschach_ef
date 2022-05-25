#include <stdio.h>
#include <string.h>
#include "vogle.h"

void
drawslant(float xt)
{
	float	s, sinc;
	char	*str = "I'm getting more and more tipped over";
	char	*c;

	s = 2 * xt;
	textslant(s);
	sinc = 3.0 / strlen(str);
	move2(0.0, 0.5);
	for (c = str; *c; c++) {
		s += sinc;

		drawchar(*c);
	}
}

/*
 * demonstrate some more features of text
 */
int
main(int argc, char *argv[])
{
	char	dev[20];
	float	xt = 0.0;
	float	xtinc = 0.01;
	
	fprintf(stderr, "Enter device: ");
	gets(dev);

	prefsize(512, 200);
	vinit(dev);
	expandviewport();

	color(BLACK);
	clear();
	color(RED);

	if (argc == 2)
		font(argv[1]);

	ortho2(0.0, 1.2, 0.0, 1.0);
	move2(0.01, 0.5);

	font("futura.l");


	getkey();

	backbuffer();

	xt = -0.5;
	while (checkkey() != 27) {
		color(BLACK);
		clear();
		color(RED);
		pushmatrix();
			translate(xt, 0.0, 0.0);
			xt += xtinc;
			if (xt >= 1.3) {
				xtinc = -0.01;
			} else if (xt < -1.0) {
				xtinc = 0.01;
			}
			drawslant(xt);
		popmatrix();
		swapbuffers();
	}
		
	vexit();

	return(0);
}
