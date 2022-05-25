
#include <stdio.h>
#include <math.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include "vogle.h"
#include "voglex.h"

Widget	canvas1, canvas2;

#define SIZE	512


int	iii;
int	back = 0;
int	doubleb = 1;
int	fill = 0;
int	hatch = 0;

int	voglewin1 = -1, voglewin2 = -1;

#define TRANS           25.0
#define SCAL            0.1
#define FILLED          2
#define OUTLINE         3

float   tdir = TRANS;
float   scal = 1.0 + SCAL;
int     but, nplanes;

Display	*dpy;
Window	win;

void
do_plus()
{
	tdir = TRANS;
}

void
do_minus()
{
	tdir = -tdir;

	if (scal < 1.0)
		scal = 1.0 + SCAL;
	else
		scal = 1.0 - SCAL;
}

void
do_back()
{
	back = !back;
	backface(back);
}

void
do_fill()
{
	fill = !fill;
	hatch = 0;

        polyfill(fill);
}

void
do_hatch()
{
	hatch = !hatch;
	fill = 0;

        polyhatch(hatch);
}

void
do_scale()
{
	scale(scal, scal, scal);
}

void
do_x()
{
	translate(tdir, 0.0, 0.0);
}

void
do_y()
{
	translate(0.0, tdir, 0.0);
}

void
do_z()
{
	translate(0.0, 0.0, tdir);
}

void
do_double()
{
	doubleb = !doubleb;

	if (doubleb) {
		fprintf(stderr, "Double buffer on\n");
		winset(voglewin1);
		backbuffer();
		winset(voglewin2);
		backbuffer();
	} else {
		winset(voglewin1);
		frontbuffer();
		winset(voglewin2);
		frontbuffer();
	}
}

void
quit()
{
	vexit();
	exit(0);
}

void
resize()
{
	Dimension       w, h;
	Arg             arg[2];

	XtSetArg(arg[0], XtNwidth, &w);
	XtSetArg(arg[1], XtNheight, &h);
	XtGetValues(canvas1, arg, 2);

	fprintf(stderr, "resize() %d %d\n", w, h);
	vo_xt_win_size(voglewin1, (int)w, (int)h);
	XtGetValues(canvas2, arg, 2);
	vo_xt_win_size(voglewin2, (int)w, (int)h);
	calcviewport();
}

void
repaint()
{
	fprintf(stderr, "repaint() called\n");
	color(BLACK);
	clear();
	
}

XtActionsRec actions[] = {
	{"repaint", 	(XtActionProc)repaint},
	{"resize", 	(XtActionProc)resize}
};

String trans =
	"<Expose>:	repaint()\n \
	 <Configure>:	resize()";


Display		*dpy;
Window		win;
GC		gc;

/*
 * simple program to display a polygon file
 */
main(ac, av)
	int	ac;
	char	**av;
{
	int		w, h;
	Widget		toplevel, 
			panel,
			qbut,
			bbut,
			fbut,
			dbut,
			hbut,
			xbut,
			ybut,
			zbut,
			sbut,
			pbut,
			mbut;

	Arg		wargs[5];
	void		drawscene();
	Dimension	ww, wh, x, y;
	XtTranslations	trans_table;

	ww = wh = 100;
	x = 0;
	y = 0;
	toplevel = XtInitialize(av[0], "xtlcube", NULL, 0, &ac, av);

	panel = XtCreateManagedWidget("panel",
			formWidgetClass,
			toplevel,
			NULL,
		0);


	XtSetArg(wargs[0], XtNlabel, "Quit");
	qbut = XtCreateManagedWidget("quit", 
			commandWidgetClass,
			panel,
			wargs,
		1);

	XtAddCallback(qbut, XtNcallback, quit, NULL);

	XtSetArg(wargs[0], XtNlabel, "Backface");
	XtSetArg(wargs[1], XtNfromHoriz, qbut);
	bbut = XtCreateManagedWidget("backface",
			commandWidgetClass,
			panel,
			wargs,
		2);
	XtAddCallback(bbut, XtNcallback, do_back, NULL);

	XtSetArg(wargs[0], XtNlabel, "Fill");
	XtSetArg(wargs[1], XtNfromHoriz, bbut);
	fbut = XtCreateManagedWidget("fill",
			commandWidgetClass,
			panel,
			wargs,
		2);
	XtAddCallback(fbut, XtNcallback, do_fill, NULL);

	XtSetArg(wargs[0], XtNlabel, "Hatch");
	XtSetArg(wargs[1], XtNfromHoriz, fbut);
	hbut = XtCreateManagedWidget("hatch",
			commandWidgetClass,
			panel,
			wargs,
		2);
	XtAddCallback(hbut, XtNcallback, do_hatch, NULL);

	XtSetArg(wargs[0], XtNlabel, "DoubleBuffer");
	XtSetArg(wargs[1], XtNfromHoriz, hbut);
	dbut = XtCreateManagedWidget("doubleb",
			commandWidgetClass,
			panel,
			wargs,
		2);
	XtAddCallback(dbut, XtNcallback, do_double, NULL);

	XtSetArg(wargs[0], XtNwidth, 512);
	XtSetArg(wargs[1], XtNheight, 512);
	XtSetArg(wargs[2], XtNfromVert, qbut);
	canvas1 = XtCreateManagedWidget("canvas", 
			simpleWidgetClass,
			panel,
			wargs,
		3);

	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(trans);
	XtAugmentTranslations(canvas1, trans_table);

	XtSetArg(wargs[2], XtNfromHoriz, canvas1);
	XtSetArg(wargs[3], XtNfromVert, qbut);
	canvas2 = XtCreateManagedWidget("canvas", 
			simpleWidgetClass,
			panel,
			wargs,
		3);

	XtAddActions(actions, XtNumber(actions));
	trans_table = XtParseTranslationTable(trans);
	XtAugmentTranslations(canvas2, trans_table);

	XtRealizeWidget(toplevel);


	dpy = XtDisplay(canvas1);
	win = XtWindow(canvas2);

	if (voglewin1 == -1) {
		voglewin1 = vo_xt_window(dpy, XtWindow(canvas1), 512, 512);
		backbuffer();
	}

	if (voglewin2 == -1) {
		voglewin2 = vo_xt_window(dpy, XtWindow(canvas2), 512, 512);
		backbuffer();
	}

	setup_lcube();

	while(1) {
		XEvent	event;

		while(XtPending()) {
			XtNextEvent(&event);
			XtDispatchEvent(&event);
		}
		winset(voglewin1);
		drawscene();
		winset(voglewin2);
		drawscene();

	}
}

void
drawscene()
{
	float	x, y;

	but = slocator(&x, &y);
	pushmatrix();
		rotate(100.0 * x, 'y');
		rotate(100.0 * y, 'x');
		color(BLACK);
		clear();
		callobj(FILLED);	/* The filled or hatched one */
/*
		if (nplanes == 1 && (fill || hatch))
			callobj(OUTLINE);	/* The outline */

	popmatrix();

	if (doubleb)
		swapbuffers();

}

