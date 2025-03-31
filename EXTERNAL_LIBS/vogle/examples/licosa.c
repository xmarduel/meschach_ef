

#include <stdlib.h>
#include <vogle.h>

#define	TRANS		0.06
void drawshape(int fill);

int
main(int ac, char *av[])
{
        char    device[10];
	float	x, y, tdir = TRANS;
	int	but, nplanes;
	int	i;

	fprintf(stderr,"Enter output device: ");
	gets(device);

	prefposition(50, 50);
	prefsize(300, 300);

	vinit(device);
	nplanes = getdepth();

	window(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	lookat(0.0, 0.0, 2.1, 0.0, 0.0, 0.0, 0.0);

	textsize(0.15, 0.3);

	backface(1);

	/*
	 * Green color ramp...
	 */
	for (i = 1; i <= 20; i++)
		mapcolor(i, 20, 20 + i * 10 , 20);
		
	if (backbuffer() < 0) {
		vexit();
		fprintf(stderr, "licosa: device doesn't support double buffering.\n"); 
		exit(0); 
	}        

	while((but = slocator(&x, &y)) != 44) {
		pushmatrix();
			rotate(120.0 * x, 'y');
			rotate(120.0 * y, 'x');
			color(BLACK);
			clear();
			color(20);
			move(0.0, 0.0, 0.0);
			drawstr("Hello, world");
			drawshape(1);
			if (nplanes == 1)
				drawshape(0);
		popmatrix();
		swapbuffers();

		switch (but = checkkey()) {
		case 'x':
			translate(tdir, 0.0, 0.0);
			break;
		case 'y':
			translate(0.0, tdir, 0.0);
			break;
		case 'z':
			translate(0.0, 0.0, tdir);
			break;
		case '-':
			tdir = -tdir;
			break;
		case '+':
			tdir = TRANS;
			break;
		case 27: /* ESC */
		case 'q':
			vexit();
			exit(0);
		default:
			;
		}
	}

	vexit();

	return(0);
}

static float	xyz[12][3] = {
	{0.000000, 0.000000, 1.0},
	{0.809017, -0.587785, 0.500000},
	{0.809017, 0.587785, 0.500000},
	{-0.309017, 0.951057, 0.500000},
	{-1.000000, 0.000000, 0.500000},
	{-0.309017, -0.951057, 0.500000},
	{1.000000, 0.000000, -0.500000},
	{0.309017, 0.951057, -0.500000},
	{-0.809017, 0.587785, -0.500000},
	{-0.809017, -0.587785, -0.500000},
	{0.309017, -0.951057, -0.500000},
	{0.000000, 0.000000, -1.0}
};
static int	ncon[20][3] = {
	{1, 2, 3},
	{1, 3, 4},
	{1, 4, 5},
	{1, 5, 6},
	{1, 6, 2},
	{2, 7, 3},
	{3, 8, 4},
	{4, 9, 5},
	{5, 10, 6},
	{6, 11, 2},
	{7, 8, 3},
	{8, 9, 4},
	{9, 10, 5},
	{10, 11, 6},
	{11, 7, 2},
	{7, 12, 8},
	{8, 12, 9},
	{9, 12, 10},
	{10, 12, 11},
	{11, 12, 7}
};

void
drawshape(int fill)
{
	int	i;

	polyfill(fill);
	if (!fill)
		color(BLACK);

	for (i = 0; i < 20; i++) {
		if (fill)
			color(i + 1);

		makepoly();
	move(xyz[ncon[i][0]-1][0], xyz[ncon[i][0]-1][1], xyz[ncon[i][0]-1][2]);
	draw(xyz[ncon[i][1]-1][0], xyz[ncon[i][1]-1][1], xyz[ncon[i][1]-1][2]);
	draw(xyz[ncon[i][2]-1][0], xyz[ncon[i][2]-1][1], xyz[ncon[i][2]-1][2]);
		closepoly();
	}
}
