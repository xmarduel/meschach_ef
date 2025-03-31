#include <stdio.h>
#include <math.h>
#include <xview/xview.h>
#include <xview/canvas.h>
#include <xview/panel.h>
#include "vogle.h"
#include "voglex.h"

#define SIZE	512

#define DOUBLEB		1
#define BACKFACE	2
#define FILL		4
#define HATCH		8

#define TRANS           25.0
#define SCAL            0.1

unsigned	flags = DOUBLEB | HATCH;

int	thing = HATCH;

float   tdir = TRANS;
float   scal = 1.0 + SCAL;
int     but, nplanes;

Display	*dpy;
Window	win;

int vogle_win = -1;

Panel_item 	toggle;

extern void setup_lcube();

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
	if (flags & BACKFACE) {
		backface(1);
	} else {
		backface(0);
	}
}

void
do_fill()
{
	if (flags & FILL) {
		polyhatch(0);
		polyfill(1);
	} else {
		polyfill(0);
	}
}

void
do_hatch()
{
	if (flags & HATCH) {
		polyfill(0);
		polyhatch(1);
	} else {
		polyhatch(0);
	}
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
	if (flags & DOUBLEB) {
		backbuffer();
	} else {
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
resize(win, event, arg, type)
	Xv_window       win;
	Event           *event;
	Notify_arg      arg;
	Notify_event_type type;
{
	int	w, h;
	
	w = xv_get(win, XV_WIDTH);
        h = xv_get(win, XV_HEIGHT);

	fprintf(stderr, "Resize proc: 0x%x, 0x%x (%d %d)\n", dpy, win, w, h);

        vo_xt_win_size(vogle_win, w, h);
	calcviewport();
	viewport(-1.0, 1.0, -1.0, 1.0);
}

void
menu_proc(menu, menu_item)
	Menu		menu;
	Menu_item	menu_item;
{
	char	*choice = (char *)xv_get(menu_item, MENU_STRING);

	if (!strcmp(choice, "DoubleBuffer")) {
		if (flags & DOUBLEB)
			flags &= ~DOUBLEB;
		else
			flags |= DOUBLEB;

		do_double();
	} else if (!strcmp(choice, "Backface")) {
		if (flags & BACKFACE)
			flags &= ~BACKFACE;
		else
			flags |= BACKFACE;

		do_back();
	} else if (!strcmp(choice, "Filled")) {
		if (flags & FILL)
			flags &= ~FILL;
		else {
			flags |= FILL;
			flags &= ~HATCH;
		}

		do_fill();
	} else if (!strcmp(choice, "Hatched")) {
		if (flags & HATCH)
			flags &= ~HATCH;
		else {
			flags |= HATCH;
			flags &= ~FILL;
		}

		do_hatch();
	} else if (!strcmp(choice, "QUIT"))
		quit();

	xv_set(toggle, PANEL_VALUE, flags, NULL);
}

/*
 * Call menu_show() to display menu on right mouse button push.
 */
void
my_event_proc(window, event)
	Xv_Window window;
	Event *event;
{
	if (event_action(event) == ACTION_MENU && event_is_down(event)) {
		Menu menu = (Menu)xv_get(window, WIN_CLIENT_DATA);
		menu_show(menu, window, event, NULL);
	}
}



toggle_selected(item, value, event)
	Panel_item item;
	unsigned value;
	Event *event;
{
	char	buf[32];
	Frame	frame = xv_get(item, PANEL_CLIENT_DATA);

	buf[0] = 0;

	if (event_id(event) == MS_LEFT) {
		if (flags & HATCH)
			if (value & FILL) {
				value &= ~HATCH;
				flags |= FILL;
				do_fill();
			}

		if (flags & FILL)
			if (value & HATCH) {
				value &= ~FILL;
				flags |= HATCH;
				do_hatch();
			}

		flags = value;
		do_back();
		do_double();

		xv_set(item, PANEL_VALUE, flags, NULL);
		xv_set(toggle, PANEL_VALUE, flags, NULL);

		return XV_OK;
	}
	return XV_ERROR;
}

void
do_hatchp(item, val, event)
	Panel_item	item;
	int		val;
	Event		*event;
{
	hatchpitch((float)val);
}

void
do_hatcha(item, val, event)
	Panel_item	item;
	int		val;
	Event		*event;
{
	hatchang((float)val);
}

int
main(ac, av)
	int	ac;
	char	**av;
	{
	Frame		frame;
	Canvas		canvas;
	Panel		panel;
	Menu		menu;
	Notify_value	drawscene();
	int		w, h;

	frame = xv_create(
		0, FRAME,
		FRAME_LABEL, av[1],
		WIN_HEIGHT, SIZE,
		WIN_WIDTH, SIZE,
	0);

	canvas = xv_create(
		frame, CANVAS,
		CANVAS_RESIZE_PROC, resize,
		WIN_HEIGHT, SIZE,
		WIN_WIDTH, SIZE,
	0);

	menu = xv_create(0, MENU, 
		MENU_TITLE_ITEM, "Gunge",
		MENU_STRINGS, "DoubleBuffer",
			      "Backface",
			      "Filled",
			      "Hatched",
			      "QUIT",
			      NULL,
		MENU_NOTIFY_PROC, menu_proc,
		NULL
	);


	xv_set(canvas_paint_window(canvas),
		WIN_CONSUME_EVENTS,  WIN_MOUSE_BUTTONS, NULL,
		WIN_EVENT_PROC,  my_event_proc,
		/* associate the menu to the canvas win for easy retrieval */
		WIN_CLIENT_DATA,  menu,
		NULL
	);

	panel = xv_create(
		frame, PANEL, 
		WIN_BELOW, canvas,
		WIN_X, 0,
	0);


	toggle = (Panel_item)xv_create(panel, PANEL_TOGGLE,
		/*PANEL_FEEDBACK,         PANEL_MARKED,*/
		PANEL_LABEL_STRING,     "Choices",
		PANEL_VALUE,            flags, /* choice 1 */
		PANEL_CHOICE_STRINGS,   "Double Buffer",
					"Backface",
					"Filled",
					"Hatched",
					 NULL,
		PANEL_NOTIFY_PROC,      toggle_selected,
		PANEL_CLIENT_DATA,      frame,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Scale",

		PANEL_NEXT_ROW, -1, 
		PANEL_NOTIFY_PROC, do_scale,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "X-trans",

		PANEL_NOTIFY_PROC, do_x,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Y-trans",

		PANEL_NOTIFY_PROC, do_y,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "Z-trans",

		PANEL_NOTIFY_PROC, do_z,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, "QUIT",

		PANEL_NOTIFY_PROC, quit,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, " + ",

		PANEL_NOTIFY_PROC, do_plus,
	0);

	panel_create_item(
		panel, PANEL_BUTTON,
		PANEL_LABEL_STRING, " - ",
		PANEL_NOTIFY_PROC, do_minus,
	0);

	xv_set(panel, PANEL_LAYOUT, PANEL_VERTICAL, 0);

	panel_create_item(panel, PANEL_SLIDER,
		PANEL_LABEL_STRING, "Hatchpitch",
		PANEL_VALUE, 40,
		PANEL_MIN_VALUE, 1,
		PANEL_MAX_VALUE, 200,
		PANEL_SLIDER_WIDTH, 200,
		PANEL_NOTIFY_PROC, do_hatchp,
		PANEL_NOTIFY_LEVEL, PANEL_ALL,
		NULL
	);
		
	panel_create_item(panel, PANEL_SLIDER,
		PANEL_LABEL_STRING, "Hatchangle",
		PANEL_VALUE, 40,
		PANEL_MIN_VALUE, 0,
		PANEL_MAX_VALUE, 180,
		PANEL_SLIDER_WIDTH, 200,
		PANEL_NOTIFY_PROC, do_hatcha,
		PANEL_NOTIFY_LEVEL, PANEL_ALL,
		NULL
	);


	window_fit(panel);
	window_fit(frame);

	w = (int)xv_get(canvas, WIN_WIDTH);
	h = (int)xv_get(canvas, WIN_HEIGHT);
	win = (Window)xv_get(canvas_paint_window(canvas), XV_XID);
	dpy = (Display *)xv_get(frame, XV_DISPLAY);

	if (vogle_win == -1)
		vogle_win = vo_xt_window(dpy, win, w, h);

	setup_lcube();

	polyhatch(1);
	hatchang(45.0);

#define USE_TIMER 1
#ifdef USE_TIMER
/* The following sets the timer */

	/* FAST AS POSSIBLE */

	notify_set_itimer_func(frame,
		(Notify_func)drawscene, ITIMER_REAL, &NOTIFY_POLLING_ITIMER, NULL);

	xv_main_loop(frame);

#else
	xv_set(frame, WIN_SHOW, TRUE, NULL);

	while(1) {
		XFlush(dpy);
		notify_dispatch();
		drawscene((Notify_client)canvas, 1);
	}
#endif
	return(0);
}

Notify_value
drawscene(c, fd)
	Notify_client c;
	int	fd;
{
	float	x, y;

	(void)slocator(&x, &y);
	x *= 90;
	y *= 90;
	pushmatrix();
		rotate(x, 'y');
		rotate(y, 'x');
		color(BLACK);
		clear();
		callobj(thing);
	popmatrix();

	if (flags & DOUBLEB)
		swapbuffers();

	return NOTIFY_DONE;
}

