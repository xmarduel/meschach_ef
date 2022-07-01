

/** \file graphics1D_libsciplot_stationnary_xt_window.c
 *
 * Contains the routines for plotting with LIBSCIPLOT library
 *
 */

#include <pthread.h>

#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#endif

#ifdef HAVE_LIBSCIPLOT
#include "libsciplot.h"
#endif

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_svqueue.h"

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_stationnary.h"

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static int TYPE_OF_THREAD = LIBSCIPLOTe_THREAD_POSIX; /* the default */

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

int  graphics1D_libsciplot_set_thread_type(int type)
{
   switch(type)
   {
      case LIBSCIPLOTe_THREAD_PYTHON: TYPE_OF_THREAD = type; break;
      case LIBSCIPLOTe_THREAD_POSIX : TYPE_OF_THREAD = type; break;
      default                       : return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

typedef struct XAW_WINLIBSCIPLOT_
{
   /* LIBSCIPLOT window ID */
   int libsciplot_win;

   /* pointer to widgets */
   Widget * buttonPPS;
   Widget * buttonPPM;
   Widget * buttonQuit;
   
   /* flags */
   int exit;
   
} XAW_WINLIBSCIPLOT ;

typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINLIBSCIPLOT * xaw_win; /* pointer to window stuff */
   
   LIBSCIPLOT_GRAPH_DATA * pdata; /* include the data to plot */

} WIDGET_USERDATA_STRUCT;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static WIDGET_USERDATA_STRUCT * widget_data;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void drawscene_init(const LIBSCIPLOT_GRAPH_DATA* pdata, Widget w)
{
   int nx = pdata->nx;
   int ny = pdata->ny;

   /* expandviewport(); */
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */
static void drawscene (const LIBSCIPLOT_GRAPH_DATA *pdata)
{
   int nx ;
   int ny ;

   int k;

   if ( pdata == NULL )
   {
      return;
   }

   nx = pdata->nx;
   ny = pdata->ny;

   /* start */
   /*graphics1D_libsciplot_erase();*/ /* backbuffer(); */

   for (k=0; k<nx*ny; k++)
   {
      pdata->dataplot[k]->draw( pdata->dataplot[k] ); /* the dataplot knowns its "draw" function */
   }

   /* end */
   graphics1D_libsciplot_erase(); /* swapbuffers(); */
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void libsciplot_curve_hardcopy(const LIBSCIPLOT_GRAPH_DATA *pdata)
{
   // TODO
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void generic_quit(void)
{
   fprintf(stderr, "quit...\n");

   switch(TYPE_OF_THREAD)
   {
      case LIBSCIPLOTe_THREAD_PYTHON: exit(0);            break;
      case LIBSCIPLOTe_THREAD_POSIX : exit(0);            break;
      default                       : /* nix */           break;
   }
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void quit(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   generic_quit();
}

static void repaint(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   drawscene(widget_data->pdata);
}

static void resize(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   drawscene(widget_data->pdata);
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void do_quit(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   generic_quit();
}

static void do_ppm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   printf("not implemented!\n");
}

static void do_pps_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   printf("not implemented!\n");
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * graphics1D_libsciplot_curve_with_xt_toolkit(void* data)
{
   SVQueue * queue = (SVQueue *)data;

   LIBSCIPLOT_GRAPH_DATA * pdata = queue->xget(queue);

   int SIZEX = pdata->window_size_x;
   int SIZEY = pdata->window_size_y;

   /*-----------------------------------------------------------------------------------*/
   /*-----------------------------------------------------------------------------------*/

   static const String translations =
      "<Expose>:       _repaint()\n \
      <Configure>:     _resize()\n \
      <Key>q:          _quit()";
   
   static XtActionsRec trans_actions[] =
   {
      {"_repaint",    repaint},
      {"_resize",     resize},
      {"_quit",       quit}
   };

   /* default parameters for widgets */
   static String default_resources[] =
   {
      (String)NULL
   };

   /*-----------------------------------------------------------------------------------*/

   XInitThreads();

   /*-----------------------------------------------------------------------------------*/

   int libsciplot_win = -1;

   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;

   Display   *dpy;
   Window    win;

   Arg       wargs[5];
   Widget    shell, canvas;
   Widget    xpanel;
   Widget    quit_button, pps_button, ppm_button;

   XtAppContext app_con;   /* application context */

   /*-----------------------------------------------------------------------------------*/

   XAW_WINLIBSCIPLOT *xaw_window = malloc( sizeof(XAW_WINLIBSCIPLOT) );
   
   WIDGET_USERDATA_STRUCT xawWidgetUserdata;

   xaw_window->libsciplot_win = -1;

   /*-----------------------------------------------------------------------------------*/

   if ( strcmp(pdata->driver, "/XWINDOW") != 0 )
   {
      /* no X11 */
      libsciplot_curve_hardcopy(pdata);

      /* finito */
      return NULL;
   }

   /*-----------------------------------------------------------------------------------*/

   /* create toplevel shell widget */
   shell = XtAppInitialize(&app_con,
                           (String)"Solution FEM",           /* app class */
                           NULL,                             /* options */
                           (Cardinal)0,                      /* num of options */
                           &ac,                              /* command line */
                           av,                               /* command line */
                           default_resources,
                           NULL,                             /* ArgList */
                           (Cardinal)0                       /* num of Args */ );

   /* set default widget parameters (including window size) */
   XtAppSetFallbackResources (app_con, default_resources);

   /* map actions to subroutines */
   XtAppAddActions (app_con, trans_actions, XtNumber (trans_actions));
   XtAddActions(trans_actions, XtNumber(trans_actions));

   xpanel = XtCreateManagedWidget("panel", formWidgetClass, shell, NULL, 0);

   /* Athena widgets */

   /* QUIT */
   XtSetArg(wargs[0], XtNlabel, "Quit");
   quit_button = XtCreateManagedWidget("quit", commandWidgetClass, xpanel, wargs, 1);

   /* PPM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   XtSetArg(wargs[2], XtNhorizDistance, SIZEX - 100);
   ppm_button = XtCreateManagedWidget("ppm", commandWidgetClass, xpanel, wargs, 3);

   /* PPS-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PS");
   XtSetArg(wargs[1], XtNfromHoriz, ppm_button);
   pps_button = XtCreateManagedWidget("pps", commandWidgetClass, xpanel, wargs, 2);

   /* user value */
   xawWidgetUserdata.pdata                        = pdata;
   xawWidgetUserdata.xaw_win                      = xaw_window;
   xawWidgetUserdata.xaw_win->buttonQuit          = &quit_button;
   xawWidgetUserdata.xaw_win->libsciplot_win      = libsciplot_win;

   /* set callback & callback data for XAW buttons --------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit       , &xawWidgetUserdata);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output , &xawWidgetUserdata);
   XtAddCallback(pps_button , XtNcallback, do_pps_output , &xawWidgetUserdata);
   /* -----------------------------------------------------------------------*/

   XtSetArg(wargs[0], XtNwidth , SIZEX);
   XtSetArg(wargs[1], XtNheight, SIZEY);
   XtSetArg(wargs[2], XtNfromVert, quit_button);
   XtSetArg(wargs[3], XtNtranslations, XtParseTranslationTable(translations));
   
   /* create canvas */
   canvas = XtCreateManagedWidget("canvas", simpleWidgetClass, xpanel, wargs, 4);

   /* manage */
   XtManageChild(canvas);

   /* finish */
   XtRealizeWidget(shell);

   /* set-up the libplot window */
   dpy = XtDisplay(canvas);
   win = XtWindow(canvas);
   
   graphics1D_libsciplot_init_multigrapher("/XWINDOW", dpy, &win, 400, 400);
   graphics1D_libsciplot_subp(1,1);

   drawscene_init(pdata, NULL);
   drawscene(pdata);

   widget_data = &xawWidgetUserdata;
   
   XtAppMainLoop(app_con);
   
   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

