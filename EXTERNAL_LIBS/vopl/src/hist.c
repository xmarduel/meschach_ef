#include <vogle.h>
#include "vopl.h"

extern	double log10();
extern	double sqrt();

static	int	*hskip = NULL;
static	int	dohistnum = 0;

/*
 * hist2
 *
 *	display the 2-d histogram described by d;
 *      d is an array of <ng> groups with <ns> species in each group
 *      names is an array with the name for each group
 */
void hist2(
	float	*d,
	int	ng,
	int	ns,
	char	*names[])
{
	int	i, j, k;
	char	err_buf[EBUF_SIZE];
	float	sx, sy, left, right, bottom, top;
	float	width, swidth, x0, tinc;

	/*
	 * Set the view up so that clipping happens...
	 */

	pushmatrix();
	pushattributes();
	pushviewport();

	getviewport(&left, &right, &bottom, &top);

	viewport(left + (1.0 + XMIN) / 2.0 * (right - left),
		right - (1.0 - XMAX) / 2.0 * (right - left),
		bottom + (1.0 + YMIN) / 2.0 * (top - bottom),
		top - (1.0 - YMAX) / 2.0 * (top - bottom));

	annotate("", 'x');
	tickmarks(0, 'x');
	minorticks(0,'x');
	range(0., ng+1., 'x');

	if (!plotdev.axes[YIND].scaleset) {
		plotdev.axes[YIND].min=0.0;
		adjustscale(d, ns*ng, 'y');
	}
	
	ortho2(WhatX(plotdev.axes[XIND].min),
	       WhatX(plotdev.axes[XIND].max),
	       WhatY(plotdev.axes[YIND].min),
	       WhatY(plotdev.axes[YIND].max));

       /*
        * now plot the bars
        */
	width=(WhatX(plotdev.axes[XIND].max)-WhatX(plotdev.axes[XIND].min))/ng;
	swidth= width*5./7./ns;


	k = 0;
	for (i=0; i<ng; i++) {
	   if (hskip && hskip[i])
		continue;
	   x0=(k+1./7.)*width;
	   for (j=0; j<ns; j++) {
	      color(j%6+1);
	      rect(x0+j*swidth, 0., x0+(j+1)*swidth, d[i*ns+j]);
	   }
	   if (dohistnum) {
		   char buf[100];
		   centertext(1);
		   move2(x0 + (j + 0.5)*swidth, d[i * ns + j] + TEXTHEIGHT);
		   sprintf(buf, "%g", d[i * ns + j]);
			printf("%s\n", buf);
		   drawstr(buf);
	   }
	   k++;
	}

	popviewport();
	popattributes();
	popmatrix();

	/*
	 * Set up the text labels for each group of bars
	 */

	pushattributes();
	pushmatrix();
	pushviewport();

	ortho2(-1.0, 1.0, -1.0, 1.0);

	leftjustify();
	color(0);
	textang(-45.0);
	clipping(0);
	textsize(1.1*TEXTWIDTH, 1.1*TEXTHEIGHT);
	linestyle("");

	tinc = (XMAX - XMIN) / ng;
	k = 0;
	for (i = 0; i < ng; i++) {
	   if (hskip && hskip[i])
		continue;
	   sx = (k+0.3) * tinc + XMIN;
	   sy= YMIN-1.1*TEXTHEIGHT;
  	   move2(sx, sy);
	   drawstr(names[i]);
	   k++;
	}

	popviewport();
	popmatrix();
	popattributes();

}

void
histskip(
	int	*h)
{
	hskip = h;
}

void
histnum(
	int	h)
{
	dohistnum = h;
}
