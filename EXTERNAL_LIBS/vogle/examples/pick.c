#include <stdio.h>
#include <stdlib.h>
#include "vogle.h"

#include <math.h>

int
main(void)
{
	char	dev[20];
	int	but;
	float	x, y, z;

	fprintf(stderr,"Enter device: ");
	gets(dev);

	(void)winopen(dev, "Vogle Picking");
	backbuffer();

	vsetflush(0);
	viewport(-1.0, 1.0, -1.0, 1.0);

	/*
	 * set up our viewing transformation
	 */
	perspective(55.0, 1.0, 0.001, 50.0);
	lookat(14.0, 0.0, 3.0, 0.0, 0.0, 3.0, -90.0);

	color(BLACK);
	clear();

	for (z = 1.0; z < 7.0; z += 1.0) {
		color((int)z);
		pushmatrix();
		translate(0.0, 0.0, z);
		circle(0.0, 0.0, z);
		popmatrix();
	}
		
	swapbuffers();
	
	setpicksize(0.02, 0.02);
	while((but = slocator(&x, &y)) != 4) {

		int	tmp;

		if (but == 1) {
			color(WHITE);
			pickmode(1);
			setpickat(x, y);

			for (z = 1.0; z < 7.0; z += 1.0) {
				color((int)z);

				pushmatrix();
				translate(0.0, 0.0, z);
				circle(0.0, 0.0, z);
				popmatrix();

				tmp = picked();

				if (tmp) {
					pickmode(0);
					linewidth(THICK);
					frontbuffer();

					pushmatrix();
					translate(0.0, 0.0, z);
					circle(0.0, 0.0, z);
					popmatrix();

					vflush();
					sleep(1);
					backbuffer();
					swapbuffers();
					pickmode(1);
					setpickat(x, y);
				}
			}
			pickmode(0);
		}
	}
	vexit();

	return(0);
}
