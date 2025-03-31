
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vogle.h"

#ifdef	TC
#define	poly	_poly
#endif 

#include <math.h>

#ifdef TC
#undef	poly
#define	poly	poly
#endif

#define pi 3.1415926535

char *fonts[] = {
	"astrology",
	"cursive",
	"futura.l",
	"futura.m",
	"gothic.eng",
	"gothic.ger",
	"gothic.ita",
	"greek",
	"japanese",
	"markers",
	"math.low",
	"math.upp",
	"meteorology",
	"music",
	"cyrillic",
	"script",
	"symbolic",
	"times.g",
	"times.ib",
	"times.i",
	"times.r",
	"times.rb"
};

void ShowCircularText(double r, char *str);

/*
 * display all the hershey fonts and demonstrate textang
 */
int
main(int ac, char *av[])
{
	char	buf[50];
	char	*str1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" ;
	char	*str2 = "abcdefghijklmnopqrstuvwxyz" ;
	char	*str3 = "1234567890+-=!@#$%^&*(){}[]" ;
	char	*str4 = "<>,./?~`\\|_BONK,blark" ;
	float	slant = 0.0;
	int	i;

	fprintf(stderr,"Enter device name: ");
	gets(buf);
	prefsize(768, 768);
	vinit(buf);

	for (i = 1; i < ac; i++) {
		if (av[i][0] == '-') {
			if (av[i][1] == 's') {
				if (av[i][2])
					slant = atof(&av[i][2]);
				else
					slant = atof(av[++i]);
			} else if (av[i][1] == 'b')
				textweight(1);
		}
	}

	textslant(slant);


	color(BLACK);
	clear();

	ortho2(-14.0, 14.0, -14.0, 14.0);	/* define the world space */

	vsetflush(0);

	for(i = 0; i < 22; i++) {

		/*
		 * textang is used to specify the orientation of text. As
		 * we want the title to come out straight we make sure it is
		 * zero each time we go through this loop.
		 */
		textang(0.0);

		/*
		 * do the title
		 */
		color(YELLOW);
		font("futura.m");
		sprintf(buf, "This is hershey font %s", fonts[i]);
		boxtext(-11.0, 12.0, 20.0, 1.0, buf);

		/*
		 * draw a box around the title
		 */
		rect(-11.0, 12.0, 9.0, 13.0);

		color(GREEN);

		font(fonts[i]);		/* grab a font from the table */

		textsize(1.5, 1.5);		/* show the outer ring */
		ShowCircularText(11.0, str1);

		textsize(1.3, 1.3);		/* show the second ring */
		ShowCircularText(8.5, str2);

		textsize(1.1, 1.1);		/* show the third ring */
		ShowCircularText(7.0, str3);

		textsize(0.9, 0.9);		/* show the inside ring */
		ShowCircularText(5.0, str4);

		if (getkey() == 'q') {
			vexit();
			exit(0);
		}

		color(BLACK);
		clear();
	}

	vexit();

	return(0);
}

/*
 * ShowCircularText
 *
 *	show a ring of text
 */
void
ShowCircularText(double r, char *str)
{
	double	i, inc, x, y;
	double	a;

	inc = 360.0 / (double)strlen(str);

	for (i = 0; i < 360.0; i += inc) {
		/*
		 * calculate the next drawing position
		 */
		x = r * cos(i * pi / 180.0);
		y = r * sin(i * pi / 180.0);
		move2(x, y);
		/*
		 * calculate angle for next character
		 */
		a = (90 + i);
		/*
		 * set the orientation of the next character
		 */
		textang(a);
		/*
		 * draw the character
		 */
		drawchar(*str++);
	}
}
