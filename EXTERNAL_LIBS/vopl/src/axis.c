#include <stdio.h>
#include <string.h>
#include "vopl.h"

void	log_axis(), lin_axis();

extern	double log10();
extern	double alog10();
extern	double pow();

/*
 * formatlabel
 *
 *	chose a "nice" format for an axis label, awful at the moment.
 */
void
formatlabel(
	float	alabel,
	char	*form)
{
	int	i, j;

	if (ABS(alabel) < 1.0e-9)
		alabel = 0.0;

	if ((ABS(alabel) > 999999.0) || (ABS(alabel) < 0.000001))
		sprintf(form, "%.4g", alabel);
	else 
		sprintf(form, "%f", alabel);

	/*
	 * if there is no 'e' in there or there is a dot then
	 * eat trailing zeros.....
	 */
	if ((strchr(form, 'e') == (char *)NULL &&
	    strchr(form, 'E') == (char *)NULL) &&
	    strchr(form, '.') != (char *)NULL) {
		i = strlen(form) - 1;
		while (i >= 0 && form[i] == '0') 
			i--;

		if (form[i] == '.')
			i--;

		form[++i] = '\0';
		if (form[0] == '\0') {
			form[0] = '0';
			form[1] = '\0';
		}
	}
}

/*
 * axistitle
 *
 *	Draws an x, y or z axis, with or without annotation, tickmarks etc
 */
void
axistitle(
	char	*title,
	char	ax)
{
	char	**s, err_buf[EBUF_SIZE];

	/*
	 * save the goddam title somewhere....
	 */
	switch (ax) {
	case 'x':
	case 'X':
		s = &plotdev.axes[XIND].title;
		break;
	case 'y':
	case 'Y':
		s = &plotdev.axes[YIND].title;
		break;
	case 'z':
	case 'Z':
		s = &plotdev.axes[ZIND].title;
		break;
	case 's':
	case 'S':
		s = &plotdev.axes[XSEC].title;
		break;
	case 't':
	case 'T':
		s = &plotdev.axes[YSEC].title;
		break;
	default:
		sprintf(err_buf, "axistitle: unknown axis '%c'", ax);
		vopl_error(err_buf);
	}

	*s = savestr(*s, title);
}

/*
 * drawaxis
 *
 *	Draws an x, y or z axis, with or without annotation, tickmarks etc
 */
void
drawaxis(
	char	axis)
{
	int	ind;
	char	err_buf[EBUF_SIZE];

	switch (axis) {
	case 'x':
	case 'X':
		ind = XIND;
		break;
	case 'y':
	case 'Y':
		ind = YIND;
		break;
	case 'z':
	case 'Z':
		ind = ZIND;
		break;
	case 's':
	case 'S':
		ind = XSEC;
		break;
	case 't':
	case 'T':
		ind = YSEC;
		break;
	default:
		sprintf(err_buf, "drawaxis: unknown axis '%c'", axis);
		vopl_error(err_buf);
	}

	if (!plotdev.axes[ind].scaleset) { 
		plotdev.axes[ind].min = -1.0;
		plotdev.axes[ind].max = 1.0;
	}

	if (plotdev.axes[ind].scaling == LOGARITHMIC)
		log_axis(ind);
	else
		lin_axis(ind);
}

/*
 * drawaxes2
 *
 *	Draws the x and y axes, with or without annotation, tickmarks etc
 */
void
drawaxes2(void)
{
	int	axis;

	for (axis = 0; axis < AXES; axis++)
		if (!plotdev.axes[axis].scaleset) { 
			plotdev.axes[axis].min = -1.0;
			plotdev.axes[axis].max = 1.0;
		}

	if (plotdev.axes[XIND].scaling == LOGARITHMIC) 
		log_axis(XIND);
	else
		lin_axis(XIND);

	if (plotdev.axes[YIND].scaling == LOGARITHMIC) 
		log_axis(YIND);
	else 
		lin_axis(YIND);
}

/*
 * drawaxes
 *
 *	Draws an x, y, and z axes, with or without annotation, tickmarks etc
 */
void
drawaxes(void)
{
	int	axis;

	for (axis = 0; axis < AXES; axis++) {
		if (!plotdev.axes[axis].scaleset) { 
			plotdev.axes[axis].min = -1.0;
			plotdev.axes[axis].max = 1.0;
		}

		if (plotdev.axes[axis].scaling == LOGARITHMIC) 
			log_axis(axis);
		else
			lin_axis(axis);
	}
}

/*
 * lin_axis
 *
 *	draw a linear axis
 */
void
lin_axis(
	int	axis)
{
	float 		x, y, sx, sy;
	float		ssx, ssy, tinc, alabel;
	int 		i, j, count;
        char		form[50], err_buf[EBUF_SIZE];
	axisdata	ax;

	pushattributes();
	pushmatrix();
	pushviewport();

	ortho2(-1.0, 1.0, -1.0, 1.0);

	clipping(0);

	switch (axis) {
	case XIND:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		move2(XMIN, YMIN);
		draw2(XMAX, YMIN);

		textang(0.0);
		centertext(1);

		ax = plotdev.axes[XIND];

		x = ax.max - ax.min;
		count = x / ax.div + 0.5;

		xcentertext();
		topjustify();

		tinc = (XMAX - XMIN) / x * ax.div;
		for (i = 0; i <= count; i++) {
			sx = i * tinc + XMIN;

			if (i < count && ax.minorticks)
				for (j = 1; j <= ax.minorticks; j++) {
					ssx = sx + tinc * j / ax.minorticks;

					move2(ssx, YMIN);
					if (!(ax.minorticks & 1) && j == ax.minorticks/2)
						draw2(ssx, YMIN - 0.75 * LINELEN);
					else
						draw2(ssx, YMIN - 0.5 * LINELEN);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(ssx, YMAX);
				}

			if (plotdev.grid) 
				if (!(i % plotdev.grid)) {
					move2(sx, YMAX);
					draw2(sx, YMIN);
				}
		

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(sx, YMIN);
				draw2(sx, YMIN - LINELEN);
			}
		
			if (ax.annotate) {
				alabel = ax.min + ax.div * (ax.mod ? (i % ax.mod) : i);
				if (ax.format)
					sprintf(form, ax.format, alabel);
				else
					formatlabel(alabel, form);

				move2(sx, YMIN - 1.2 * LINELEN);
				drawstr(form);
			}
		}

		if (ax.title != (char *)NULL) {
			textang(0.0);
			centertext(1);
			x = 0.5 * (XMAX - XMIN) + XMIN;
			y = (YMIN - 6.0 * TEXTWIDTH);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			move2(x, y);
			drawstr(ax.title);
		}
		break;
	case YIND:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		centertext(0);
		textang(0.0);

		move2(XMIN, YMIN);
		draw2(XMIN, YMAX);

		textjustify(0);
		ycentertext();
		rightjustify();

		ax = plotdev.axes[YIND];

		y = ax.max - ax.min;
		tinc = (YMAX - YMIN) / y * ax.div;
		count = y / ax.div + 0.5;

		for (i = 0; i <= count; i++) {
			sy = i * tinc + YMIN;
			if (i < count && ax.minorticks)
				for (j = 1; j <= ax.minorticks; j++) {
					ssy = sy + tinc * j / ax.minorticks;

					move2(XMIN, ssy);
					if (!(ax.minorticks & 1) && j == ax.minorticks/2)
						draw2(XMIN - 0.75 * LINELEN, ssy);
					else
						draw2(XMIN - 0.5 * LINELEN, ssy);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(XMAX, ssy);
				}

			if (plotdev.grid) 
				if (!(i % plotdev.grid)) {
					move2(XMIN, sy);
					draw2(XMAX, sy);
				}

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(XMIN, sy);
				draw2(XMIN - LINELEN, sy);
			}

			if (ax.annotate) {
				alabel = ax.min + ax.div * i;
				if (ax.format)
					sprintf(form, ax.format, alabel);
				else
					formatlabel(alabel, form);

				/*
				move2(XMIN - strlength(form) - 2 * LINELEN, sy - TEXTHEIGHT / 2);
				*/
				move2(XMIN - 1.2 * LINELEN, sy);
				drawstr(form);
			}
		}

		if (ax.title != (char *)NULL) {
			x = XMIN - 7.0 * TEXTWIDTH;
			y = 0.5 * (YMAX - YMIN) + YMIN;
			move2(x, y);
			textang(90.0);
			centertext(1);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			drawstr(ax.title);
			textang(0.0);
		}
		break;
	case ZIND:
		vopl_error("Z-Axis not yet implemented");
		break;
	case XSEC:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		move2(XMIN, YMAX);
		draw2(XMAX, YMAX);

		textang(0.0);
		centertext(1);

		ax = plotdev.axes[XSEC];

		x = ax.max - ax.min;
		count = x / ax.div + 0.5;

		xcentertext();
		bottomjustify();

		tinc = (XMAX - XMIN) / x * ax.div;
		for (i = 0; i <= count; i++) {
			sx = i * tinc + XMIN;

			if (i < count && ax.minorticks)
				for (j = 1; j <= ax.minorticks; j++) {
					ssx = sx + tinc * j / ax.minorticks;

					move2(ssx, YMAX);
					if (!(ax.minorticks & 1) && j == ax.minorticks/2)
						draw2(ssx, YMAX + 0.75 * LINELEN);
					else
						draw2(ssx, YMAX + 0.5 * LINELEN);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(ssx, YMAX);
				}

			if (plotdev.grid) 
				if (!(i % plotdev.grid)) {
					move2(sx, YMAX);
					draw2(sx, YMIN);
				}
		

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(sx, YMAX);
				draw2(sx, YMAX + LINELEN);
			}
		
			if (ax.annotate) {
				alabel = ax.min + ax.div * i;
				if (ax.format)
					sprintf(form, ax.format, alabel);
				else
					formatlabel(alabel, form);

				/*
				move2(sx, YMIN - LINELEN - 1.1 * TEXTHEIGHT / 2);
				*/
				move2(sx, YMAX + 1.2 * LINELEN);
				drawstr(form);
			}
		}

		if (ax.title != (char *)NULL) {
			textang(0.0);
			centertext(1);
			x = 0.5 * (XMAX - XMIN) + XMIN;
			y = (YMAX - 6.0 * TEXTWIDTH);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			move2(x, y);
			drawstr(ax.title);
		}
		break;
	case YSEC:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		centertext(0);
		textang(0.0);

		move2(XMAX, YMIN);
		draw2(XMAX, YMAX);

		textjustify(0);
		ycentertext();
		leftjustify();

		ax = plotdev.axes[YSEC];

		y = ax.max - ax.min;
		tinc = (YMAX - YMIN) / y * ax.div;
		count = y / ax.div + 0.5;

		for (i = 0; i <= count; i++) {
			sy = i * tinc + YMIN;
			if (i < count && ax.minorticks)
				for (j = 1; j <= ax.minorticks; j++) {
					ssy = sy + tinc * j / ax.minorticks;

					move2(XMAX, ssy);
					if (!(ax.minorticks & 1) && j == ax.minorticks/2)
						draw2(XMAX + 0.75 * LINELEN, ssy);
					else
						draw2(XMAX + 0.5 * LINELEN, ssy);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(XMAX, ssy);
				}

			if (plotdev.grid) 
				if (!(i % plotdev.grid)) {
					move2(XMIN, sy);
					draw2(XMAX, sy);
				}

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(XMAX, sy);
				draw2(XMAX + LINELEN, sy);
			}

			if (ax.annotate) {
				alabel = ax.min + ax.div * i;
				if (ax.format)
					sprintf(form, ax.format, alabel);
				else
					formatlabel(alabel, form);

				/*
				move2(XMIN - strlength(form) - 2 * LINELEN, sy - TEXTHEIGHT / 2);
				*/
				move2(XMAX + 1.2 * LINELEN, sy);
				drawstr(form);
			}
		}

		if (ax.title != (char *)NULL) {
			x = XMAX + 7.0 * TEXTWIDTH;
			y = 0.5 * (YMAX - YMIN) + YMIN;
			move2(x, y);
			textang(90.0);
			centertext(1);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			drawstr(ax.title);
			textang(0.0);
		}
		break;
	default:
		sprintf(err_buf, "axis: unknown axis: %d", axis);
		vopl_error(err_buf);
	}

	popviewport();
	popmatrix();
	popattributes();
	clipping(1);
}


/*
 * log_axis
 *
 *	Does a logarithmic axis with exponential style annotation
 */
void
log_axis(
	int	axis)
{

	float 		x, y, sx, sy;
	float		ssx, ssy, tinc, mtinc, alabel, ldiv;
	int 		i, j;
        char		form[21], err_buf[EBUF_SIZE];
	axisdata	ax;
	
	pushattributes();
	pushmatrix();
	pushviewport();

	ortho2(-1.0, 1.0, -1.0, 1.0);

	clipping(0);

	switch (axis) {
	case XIND:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		move2(XMIN, YMIN);
		draw2(XMAX, YMIN);

		textang(0.0);
		centertext(0);

		ax = plotdev.axes[XIND];

		/*
		 * Get the minimum exponent value.
		 */
		alabel = (float)log10((double)ax.min);

		tinc = (XMAX - XMIN) / (ax.nticks);
		for (i = 0; i <= ax.nticks; i++) {

			sx = i * tinc + XMIN;
			/* Minor ticks */
			if (i < ax.nticks && ax.minorticks) 
				for (j = 1; j < 10; j++) {
					ssx = sx + tinc * log10((double)j);

					move2(ssx, YMIN);
					if (j == 5)
						draw2(ssx, YMIN - 0.75 * LINELEN);
					else
						draw2(ssx, YMIN - 0.5 * LINELEN);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(ssx, YMAX);
				}

			if (plotdev.grid)
				if (!(i % plotdev.grid)) {
					move2(sx, YMAX);
					draw2(sx, YMIN);
				}

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(sx, YMIN);
				draw2(sx, YMIN - LINELEN);
			}
			
			if (ax.annotate) {
				formatlabel(alabel, form);

				textsize(0.7 * TEXTWIDTH, 0.7 * TEXTHEIGHT);
				move2(sx + 0.5 * TEXTWIDTH, YMIN - LINELEN - 0.8 * TEXTHEIGHT);
				drawstr(form);
				rmove2(-strlength(form), 0.0);
				textsize(TEXTWIDTH, TEXTHEIGHT);
				rmove2(-1.3 * TEXTWIDTH, -0.9 * TEXTHEIGHT);
				drawstr("10");
			}
			alabel += ax.div;
		}

		if (ax.title != (char *)NULL) {
			centertext(1);
			x = 0.5 * (XMAX - XMIN) + XMIN;
			y = (YMIN - 6.0 * TEXTWIDTH);
			move2(x, y);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			drawstr(ax.title);
		}
		break;
	case YIND:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		centertext(0);
		textang(0.0);

		move2(XMIN, YMIN);
		draw2(XMIN, YMAX);

		ax = plotdev.axes[YIND];

		/*
		 * Get the minimum exponent value.
		 */
		alabel = (float)log10((double)ax.min);

		centertext(0);

		tinc = (YMAX - YMIN) / (ax.nticks);

		for (i = 0; i <= ax.nticks; i++) {
			sy = i * tinc + YMIN;

			if (i < ax.nticks && ax.minorticks)
				for (j = 1; j < 10; j++) {
					ssy = sy + tinc * log10((double)j);

					move2(XMIN, ssy);
					if (j == 5)
						draw2(XMIN - 0.75 * LINELEN, ssy);
					else
						draw2(XMIN - 0.5 * LINELEN, ssy);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(XMAX, ssy);
				}

			if (plotdev.grid) 
				if (!(i % plotdev.grid)) {
					move2(XMIN, sy);
					draw2(XMAX, sy);
				}

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(XMIN, sy);
				draw2(XMIN - LINELEN, sy);
			}

			if (ax.annotate) {
				formatlabel(alabel, form);
				textsize(0.7 * TEXTWIDTH, 0.7 * TEXTHEIGHT);
				move2(XMIN - strlength(form) -1.5 * LINELEN, sy + 0.5 * TEXTHEIGHT);
				drawstr(form);
				rmove2(-strlength(form), -0.75 * TEXTHEIGHT);
				textsize(TEXTWIDTH, TEXTHEIGHT);
				rmove2(-1.1 * TEXTWIDTH, 0.0);
				drawstr("10");
			}

			alabel += ax.div;
		}

		if (ax.title != (char *)NULL) {
			x = XMIN - 7.0 * TEXTWIDTH;
			y = 0.5 * (YMAX - YMIN) + YMIN;
			move2(x, y);
			textang(90.0);
			centertext(1);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			drawstr(ax.title);
		}
		break;
	case ZIND:
		vopl_error("Z-Axis not yet implemented");
		break;
	case XSEC:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		move2(XMIN, YMAX);
		draw2(XMAX, YMAX);

		textang(0.0);
		centertext(0);

		ax = plotdev.axes[XSEC];

		/*
		 * Get the minimum exponent value.
		 */
		alabel = (float)log10((double)ax.min);

		tinc = (XMAX - XMIN) / (ax.nticks);
		for (i = 0; i <= ax.nticks; i++) {

			sx = i * tinc + XMIN;
			/* Minor ticks */
			if (i < ax.nticks && ax.minorticks) 
				for (j = 1; j < 10; j++) {
					ssx = sx + tinc * log10((double)j);

					move2(ssx, YMAX);
					if (j == 5)
						draw2(ssx, YMAX + 0.75 * LINELEN);
					else
						draw2(ssx, YMAX + 0.5 * LINELEN);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(ssx, YMAX);
				}

			if (plotdev.grid)
				if (!(i % plotdev.grid)) {
					move2(sx, YMAX);
					draw2(sx, YMIN);
				}

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(sx, YMAX);
				draw2(sx, YMAX + LINELEN);
			}
			
			if (ax.annotate) {
				formatlabel(alabel, form);

				textsize(0.7 * TEXTWIDTH, 0.7 * TEXTHEIGHT);
				move2(sx + 0.5 * TEXTWIDTH, YMAX + LINELEN + 0.8 * TEXTHEIGHT);
				drawstr(form);
				rmove2(-strlength(form), 0.0);
				textsize(TEXTWIDTH, TEXTHEIGHT);
				rmove2(-1.3 * TEXTWIDTH, -0.9 * TEXTHEIGHT);
				drawstr("10");
			}
			alabel += ax.div;
		}

		if (ax.title != (char *)NULL) {
			centertext(1);
			x = 0.5 * (XMAX - XMIN) + XMIN;
			y = (YMAX + 6.0 * TEXTWIDTH);
			move2(x, y);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			drawstr(ax.title);
		}
		break;
	case YSEC:
		textsize(TEXTWIDTH, TEXTHEIGHT);
		centertext(0);
		textang(0.0);

		move2(XMAX, YMIN);
		draw2(XMAX, YMAX);

		ax = plotdev.axes[YSEC];

		/*
		 * Get the minimum exponent value.
		 */
		alabel = (float)log10((double)ax.min);

		centertext(0);

		tinc = (YMAX - YMIN) / (ax.nticks);

		for (i = 0; i <= ax.nticks; i++) {
			sy = i * tinc + YMIN;

			if (i < ax.nticks && ax.minorticks)
				for (j = 1; j < 10; j++) {
					ssy = sy + tinc * log10((double)j);

					move2(XMAX, ssy);
					if (j == 5)
						draw2(XMAX + 0.75 * LINELEN, ssy);
					else
						draw2(XMAX + 0.5 * LINELEN, ssy);
					if (ax.minorticks && plotdev.grid && !(j % plotdev.grid))
						draw2(XMAX, ssy);
				}

			if (plotdev.grid) 
				if (!(i % plotdev.grid)) {
					move2(XMIN, sy);
					draw2(XMAX, sy);
				}

			if (ax.ntspacing && !(i % ax.ntspacing)) {
				move2(XMAX, sy);
				draw2(XMAX +LINELEN, sy);
			}

			if (ax.annotate) {
				formatlabel(alabel, form);
				textsize(0.7 * TEXTWIDTH, 0.7 * TEXTHEIGHT);
				move2(XMAX + strlength(form) +1.5 * LINELEN, sy + 0.5 * TEXTHEIGHT);
				drawstr(form);
				rmove2(-strlength(form), -0.75 * TEXTHEIGHT);
				textsize(TEXTWIDTH, TEXTHEIGHT);
				rmove2(-1.1 * TEXTWIDTH, 0.0);
				drawstr("10");
			}

			alabel += ax.div;
		}

		if (ax.title != (char *)NULL) {
			x = XMAX + 7.0 * TEXTWIDTH;
			y = 0.5 * (YMAX - YMIN) + YMIN;
			move2(x, y);
			textang(90.0);
			centertext(1);
			textsize(1.3 * TEXTWIDTH, 1.3 * TEXTHEIGHT);
			drawstr(ax.title);
		}
		break;
	default:
		sprintf(err_buf, "axis: unknown axis: %d", axis);
		vopl_error(err_buf);
	}

	popviewport();
	popmatrix();
	popattributes();
	clipping(1);
}
