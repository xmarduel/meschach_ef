#include <stdio.h>
#include <plot.h>
#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Core.h>

int green = 0;                  /* draw in green, not red? */

#define MAXORDER 12
void draw_c_curve (double dx, double dy, int order)
{
  if (order >= MAXORDER)
    pl_fcontrel (dx, dy);       /* continue path along (dx, dy) */
  else
    {
      draw_c_curve (0.5 * (dx - dy), 0.5 * (dx + dy), order + 1);
      draw_c_curve (0.5 * (dx + dy), 0.5 * (dy - dx), order + 1);
    }
}

void Redraw (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
  /* draw C curve */
  pl_erase ();
  pl_pencolorname (green ? "green" : "red");
  pl_fmove (600.0, 300.0);  
  draw_c_curve (0.0, 400.0, 0);
  pl_endpath ();
}

void Toggle (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
  green = (green ? 0 : 1);
  Redraw (w, ev, params, n_params);
}

void Quit (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
  exit (0);
}

/* mapping of events to actions */
static const String translations =
"<Expose>:      redraw()\n\
<Btn1Down>:     toggle()\n\
<Key>q:         quit()";

/* mapping of actions to subroutines */
static XtActionsRec actions[] = 
{
  {"redraw",            Redraw},
  {"toggle",            Toggle},
  {"quit",              Quit},
};

/* default parameters for widgets */
static String default_resources[] = 
{
  "Example*geometry:      250x250",
  (String)NULL
};

int main (int argc, char *argv[])
{
  Arg wargs[10];                /* storage of widget args */
  Display *display;             /* X display */
  Widget shell, canvas;         /* toplevel widget; child */
  Window window;                /* child widget's window */
  XtAppContext app_con;         /* application context */
  int handle, i;
  char *bg_colorname = "white";
  
  /* take background color from command line */
  for (i = 0; i < argc - 1; i++)
    if (strcmp (argv[i], "-bg") == 0)
      bg_colorname = argv[i + 1];

  /* create toplevel shell widget */
  shell = XtAppInitialize (&app_con, 
                           (String)"Example", /* app class */
                           NULL,              /* options */
                           (Cardinal)0,       /* num of options */
                           &argc,             /* command line */
                           argv,              /* command line */
                           default_resources,
                           NULL,              /* ArgList */
                           (Cardinal)0        /* num of Args */
                           );

  /* set default widget parameters (including window size) */
  XtAppSetFallbackResources (app_con, default_resources);

  /* map actions to subroutines */
  XtAppAddActions (app_con, actions, XtNumber (actions));

  /* create canvas widget as child of shell widget; realize both */
  XtSetArg(wargs[0], XtNargc, argc);
  XtSetArg(wargs[1], XtNargv, argv);
  canvas = XtCreateManagedWidget ((String)"", coreWidgetClass,
                                  shell, wargs, (Cardinal)2);
  XtRealizeWidget (shell);

  /* for the canvas widget, map events to actions */
  XtSetArg (wargs[0], XtNtranslations, 
            XtParseTranslationTable (translations));
  XtSetValues (canvas, wargs, (Cardinal)1);

  /* initialize GNU libplot */
  display = XtDisplay (canvas);
  pl_parampl ("XDRAWABLE_DISPLAY", display);
  window = XtWindow (canvas);
  pl_parampl ("XDRAWABLE_DRAWABLE1", &window); 
  pl_parampl ("BG_COLOR", bg_colorname);
  handle = pl_newpl ("Xdrawable", NULL, NULL, stderr);
  pl_selectpl (handle);
  pl_openpl ();
  pl_fspace (0.0, 0.0, 1000.0, 1000.0);
  pl_flinewidth (0.25);     

  /* transfer control to X Toolkit event loop (doesn't return) */
  XtAppMainLoop (app_con);

  return 1;
}
