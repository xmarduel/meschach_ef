
#include <stdio.h>
#include <vogle.h>

float carray[][3] = { -1.0,  -1.0,   1.0, /* front */
		      1.0,  -1.0,   1.0,
		      1.0,   1.0,   1.0,
		     -1.0,   1.0,   1.0,
		     -1.0,  -1.0,  -1.0, /* rear */
		      1.0,  -1.0,  -1.0,
		      1.0,   1.0,  -1.0,
		     -1.0,   1.0,  -1.0
		};

int	nplanes;

/*
 * drawcube
 *
 *	draw the cube setting colours if available
 */
void
drawcube()
{
	if (nplanes > 1)
		color(RED);

	makepoly();	/* Front */
		move(carray[0][0], carray[0][1], carray[0][2]);
		draw(carray[1][0], carray[1][1], carray[1][2]);
		draw(carray[2][0], carray[2][1], carray[2][2]);
		draw(carray[3][0], carray[3][1], carray[3][2]);
		draw(carray[0][0], carray[0][1], carray[0][2]);
	closepoly();
	
	if (nplanes > 1)
		color(GREEN);

	makepoly();	/* Back */
		move(carray[5][0], carray[5][1], carray[5][2]);
		draw(carray[4][0], carray[4][1], carray[4][2]);
		draw(carray[7][0], carray[7][1], carray[7][2]);
		draw(carray[6][0], carray[6][1], carray[6][2]);
		draw(carray[5][0], carray[5][1], carray[5][2]);
	closepoly();

	if (nplanes > 1)
		color(YELLOW);

	makepoly();	/* Right side */
		move(carray[1][0], carray[1][1], carray[1][2]);
		draw(carray[5][0], carray[5][1], carray[5][2]);
		draw(carray[6][0], carray[6][1], carray[6][2]);
		draw(carray[2][0], carray[2][1], carray[2][2]);
		draw(carray[1][0], carray[1][1], carray[1][2]);
	closepoly();

	if (nplanes > 1)
		color(BLUE);

	makepoly();	/* Left side */
		move(carray[0][0], carray[0][1], carray[0][2]);
		draw(carray[3][0], carray[3][1], carray[3][2]);
		draw(carray[7][0], carray[7][1], carray[7][2]);
		draw(carray[4][0], carray[4][1], carray[4][2]);
		draw(carray[0][0], carray[0][1], carray[0][2]);
	closepoly();

	if (nplanes > 1)
		color(MAGENTA);

	makepoly();	/* Top */
		move(carray[2][0], carray[2][1], carray[2][2]);
		draw(carray[6][0], carray[6][1], carray[6][2]);
		draw(carray[7][0], carray[7][1], carray[7][2]);
		draw(carray[3][0], carray[3][1], carray[3][2]);
		draw(carray[2][0], carray[2][1], carray[2][2]);
	closepoly();
	
	if (nplanes > 1)
		color(CYAN);

	makepoly();	/* Bottom */
		move(carray[0][0], carray[0][1], carray[0][2]);
		draw(carray[4][0], carray[4][1], carray[4][2]);
		draw(carray[5][0], carray[5][1], carray[5][2]);
		draw(carray[1][0], carray[1][1], carray[1][2]);
		draw(carray[0][0], carray[0][1], carray[0][2]);
	closepoly();
}


main(argc, argv)
{
	char	c;
	float	r, t, dr = 10.0, dt = 0.2;
	int	fill, back, backdir, nkey;

	prefsize(300, 300);

	vinit("mswin");

	nplanes = getdepth();

	fill = 0;
	back = 1;
	backdir = 1;

	polyfill(fill);

	color(BLACK);
	clear();

	window(-1.5, 1.5, -1.5, 1.5, 9.0, -5.0);
	lookat(0.0, 0.0, 12.0, 0.0, 0.0, 0.0, 0.0);

	backface(back);
	backfacedir(backdir);
	
	if (backbuffer() < 0) {
		verror("cube: device doesn't support double buffering.\n");
	}

	t = 0.0;
	c = '\0';

	do {
		for (r = 0.0; r < 360.0; r += dr) {
			color(BLACK);
			clear();
			pushmatrix();
				translate(0.0, 0.0, t);
				rotate(r, 'y');
				rotate(r, 'z');
				rotate(r, 'x');
				color(WHITE);
				drawcube();
				if (nplanes == 1 && fill) {
					polyfill(0);
					color(0);
					drawcube();
					polyfill(fill);
				}

			popmatrix();

			if (c == 'p') {
				vnewdev("mswin");
				c = '\0';
			}

			t += dt;
			if (t > 3.0 || t < -18.0)
				dt = -dt;

			swapbuffers();

			if (c = checkkey()) {
				if (c == 'p') {
					voutput("cube.ps");
					vnewdev("postscript");
				} else if (c == 'f') {
					fill = !fill;
					polyfill(fill);
				} else if (c == 'b') {
					back = !back;
					backface(back);
				} else if (c == 'd') {
					backdir = !backdir;
					backfacedir(backdir);
				} else {
					vexit();
					exit(0);
				}
			}
		}
	} while(1);

	vexit();
}

