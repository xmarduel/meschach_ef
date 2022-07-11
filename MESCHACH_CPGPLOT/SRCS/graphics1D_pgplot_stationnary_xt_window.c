

/** \file graphics1D_pgplot.c
 *
 * Contains the routines for plotting with PGPLOT library
 *
 */

#include <pthread.h>

#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Shell.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Paned.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/MenuButton.h>
#include <X11/Xaw/SimpleMenu.h>
#include <X11/Xaw/SmeBSB.h>
#include <X11/Xaw/SmeLine.h>
#include <X11/Xaw/Viewport.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Toggle.h>

#include "XaPgplot.h"
#include "cpgplot.h"

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_stationnary.h"

static void report_cursor(Widget w, XtPointer context, XEvent *event, Boolean *call_next);
static void xaw_cursor_inputs(Widget w, XtPointer client_data, XtPointer call_data);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static int TYPE_OF_THREAD = PGPLOTe_THREAD_POSIX; /* the default */

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

int  graphics1D_cpgplot_set_thread_type(int type)
{
   switch(type)
   {
      case PGPLOTe_THREAD_PYTHON: TYPE_OF_THREAD = type;  break;
      case PGPLOTe_THREAD_POSIX : TYPE_OF_THREAD = type;  break;
      default                   : return EXIT_FAILURE;
   }

   return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

typedef struct XAW_WINPGPLOT_
{
   /* PGPLOT window ID */
   int pgplot_win;

   /* pointer to widgets */
   Widget * buttonQuit;
   Widget * buttonPPM;
   Widget * buttonGIF;

   /* flags */
   int exit ;

   Widget * w_frame;  /* A frame widget */
   Widget * w_plot;

} XAW_WINPGPLOT ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINPGPLOT * xaw_winpgplot; /* pointer to window stuff */

   PGPLOT_GRAPH_DATA * data; /* include the data to plot */

} WIDGET_USERDATA_STRUCT;

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void generic_quit(void);
static void generic_output(const XAW_WINPGPLOT *wmotif, const PGPLOT_GRAPH_DATA *pdata, PGPLOT_OUTPUT_FORMAT output_format);

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene_init(const PGPLOT_GRAPH_DATA *pdata, Widget w)
{
   int nx = pdata->nx;
   int ny = pdata->ny;

   double  size_x = pdata->window_size_x;
   double  size_y = pdata->window_size_y;

   cpgscr(0, 0.0, 0.0, 0.0); /* black background */
   cpgpap(PGPLOT_PIXEL_TO_CENTIMETER(pdata->window_size_x), size_y/size_x);
   cpgask(0);

   cpgsubp(nx, nx);
}


static void drawscene (const PGPLOT_GRAPH_DATA *pdata, Widget w)
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
   cpgbbuf();

   /**/
   for (k=0; k<nx*ny; k++)
   {
      pdata->dataplot[k]->draw( pdata->dataplot[k] ); /* the dataplot knowns its "draw" function */
   }

   /* finito */
   cpgebuf();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void pgplot_curve_hardcopy(const PGPLOT_GRAPH_DATA *pdata)
{
   char  pgplot_output_id[64];
   char* pgplot_output = pgplot_output_id;

   /* check driver */
   if ( strcmp(pdata->driver, "/GIF")  == 0 )
   {
      snprintf(pgplot_output_id, 31, "%s.%s", PGPLOT_OUTPUT_FILE, "gif");
   }
   else
   if ( strcmp(pdata->driver, "/PPM")  == 0 )
   {
      snprintf(pgplot_output_id, 31, "%s.%s", PGPLOT_OUTPUT_FILE, "ppm");
   }
   else
   if ( strcmp(pdata->driver, "/PPS")  == 0 )
   {
      snprintf(pgplot_output_id, 31, "%s.%s", PGPLOT_OUTPUT_FILE, "ps");
   }
   else
   {
      fprintf(stderr, "Bad driver: %s\n", pdata->driver);
      return;
   }

   pgplot_output = strncat(pgplot_output_id, pdata->driver, 7);
   /* a msg */
   fprintf(stderr, "Producing output on file %s\n", pgplot_output);

   /* start new driver plot */
   cpgopen(pgplot_output);

   drawscene_init(pdata, NULL);
   drawscene(pdata, NULL);

   /* close it */
   cpgclos();

   return ;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void generic_output(const XAW_WINPGPLOT *wxaw, const PGPLOT_GRAPH_DATA *pdata, PGPLOT_OUTPUT_FORMAT output_format)
{
   /*
    * type must be "pps" or "ppm""
    */

   char  pgplot_output_id[64];
   char* pgplot_output = pgplot_output_id;

   switch( output_format )
   {
      case PGPLOTe_PPS:
         snprintf(pgplot_output_id, 31, "%s.%s", PGPLOT_OUTPUT_FILE, "ps");
         pgplot_output = strncat(pgplot_output_id, "/PPS", 7);
         break;

      case PGPLOTe_PPM:
         snprintf(pgplot_output_id, 31, "%s.%s", PGPLOT_OUTPUT_FILE, "ppm");
         pgplot_output = strncat(pgplot_output_id, "/PPM", 7);
         break;

      case PGPLOTe_GIF:
         snprintf(pgplot_output_id, 31, "%s.%s", PGPLOT_OUTPUT_FILE, "gif");
         pgplot_output = strncat(pgplot_output_id, "/GIF", 7);
         break;

      default:
         return;
   }

   fprintf(stderr, "Producing output on file %s\n", pgplot_output);

   /* start new driver plot */
   cpgopen(pgplot_output);

   drawscene_init(pdata, *wxaw->w_plot);
   drawscene(pdata, *wxaw->w_plot);

   /* close it */
   cpgclos();

   /* back to X11 */
   cpgslct(wxaw->pgplot_win);
}


static void generic_quit(void)
{
   fprintf(stderr, "quit...\n");

   switch(TYPE_OF_THREAD)
   {
      case PGPLOTe_THREAD_PYTHON: exit(0);            break;
      case PGPLOTe_THREAD_POSIX : exit(0);            break;
      default                   : /* nix */           break;
   }
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void repaint(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
}

static void quit(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   generic_quit();
}

static void resize(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void do_quit( Widget w, XtPointer data, XtPointer data2)
{
   generic_quit();
}


static void do_ppm_output( Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xdata->xaw_winpgplot, xdata->data, PGPLOTe_PPM);
}

static void do_gif_output( Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xdata->xaw_winpgplot, xdata->data, PGPLOTe_GIF);
}

static void do_pps_output( Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xdata->xaw_winpgplot, xdata->data, PGPLOTe_PPS);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * pgplot_curve_with_xt_toolkit(void *data)
{
   SVQueue *queue = (SVQueue*)data;

   PGPLOT_GRAPH_DATA * pdata = queue->xget(queue);

   /*--------------------------------------------------------------------------------------*/

   if ( strcmp(pdata->driver, "/XWINDOW") != 0 )
   {
      /* no X11 */
      pgplot_curve_hardcopy(pdata);

      /* finito */
      return NULL;
   }

   /*--------------------------------------------------------------------------------------*/

   /* default parameters for widgets */
   static String default_resources[] =
   {
      (String)NULL
   };

   /*--------------------------------------------------------------------------------------*/

   XtAppContext app_con;   /* application context */

   int pgplot_win = -1;
   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;

   Arg		wargs[5];
   Widget	toplevel, xpanel, quit_button;
   Widget       pps_button, ppm_button, gif_button;

   Widget w_frame;  /* A frame widget */
   Widget w_plot;

   /*--------------------------------------------------------------------------------------*/

   XAW_WINPGPLOT *xaw_winpgplot = malloc( sizeof(XAW_WINPGPLOT) );

   WIDGET_USERDATA_STRUCT xawButtonUserdata;

   xaw_winpgplot->exit = 0;

   /*--------------------------------------------------------------------------------------*/

   /* create toplevel shell widget */
   toplevel = XtAppInitialize (&app_con, (String)"Solution FEM", /* app class */
                               NULL /* options */, (Cardinal)0, /* num of options */
                               &ac, av,              /* command line */
                               default_resources,
                               NULL,              /* ArgList */
                               (Cardinal)0        /* num of Args */ );


   /* set default widget parameters (including window size) */
   XtAppSetFallbackResources (app_con, default_resources);


   xpanel = XtCreateManagedWidget("panel", formWidgetClass, toplevel, NULL, 0);

   /* Athena widgets */
   XtSetArg(wargs[0], XtNlabel, "Quit");
   quit_button = XtCreateManagedWidget("quit", commandWidgetClass, xpanel, wargs, 1);

   /* pgplot ppm-output */
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNhorizDistance, pdata->window_size_x - 90);
   ppm_button = XtCreateManagedWidget("ppm", commandWidgetClass, xpanel, wargs, 2);

   /* pgplot gif-output */
   XtSetArg(wargs[0], XtNlabel, "GIF");
   XtSetArg(wargs[1], XtNfromHoriz, ppm_button);
   gif_button = XtCreateManagedWidget("gif", commandWidgetClass, xpanel, wargs, 2);

   /* pgplot ps-output */
   XtSetArg(wargs[0], XtNlabel, "PS");
   XtSetArg(wargs[1], XtNfromHoriz, gif_button);
   pps_button = XtCreateManagedWidget("pps", commandWidgetClass, xpanel, wargs, 2);


   /*
    * Create an etched-in frame widget to provide a border for the
    * image window.
    */
   w_frame = XtVaCreateManagedWidget("image_frame",
                                     viewportWidgetClass,
                                     xpanel,
                                     XtNfromVert, quit_button,
                                     NULL);
   /*
    * Create the image-display widget.
    */
   w_plot = XtVaCreateManagedWidget("image", xaPgplotWidgetClass,
                                    w_frame,
                                    XtNheight, pdata->window_size_y,
                                    XtNwidth, pdata->window_size_x,
                                    XapNmaxColors, 50,
                                    XapNshare, True,
                                    NULL);

   /* user value */
   xawButtonUserdata.data                         = pdata;
   xawButtonUserdata.xaw_winpgplot                = xaw_winpgplot;
   xawButtonUserdata.xaw_winpgplot->buttonQuit    = &quit_button;
   xawButtonUserdata.xaw_winpgplot->w_frame       = &w_frame;
   xawButtonUserdata.xaw_winpgplot->w_plot        = &w_plot;

   /* set callback & callback data for XAW buttons --------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit       , &xawButtonUserdata);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output , &xawButtonUserdata);
   XtAddCallback(gif_button , XtNcallback, do_gif_output , &xawButtonUserdata);
   XtAddCallback(pps_button , XtNcallback, do_pps_output , &xawButtonUserdata);
   /* -----------------------------------------------------------------------*/

   /*
    * Register a motion-event callback such that the cursor position can
    * be reported in the im->w_coord label widget.
    */
   XtAddEventHandler(w_plot, PointerMotionMask, False, report_cursor, &xawButtonUserdata);


   XtManageChild(xpanel);
   XtRealizeWidget(toplevel);

   /*
    * Open the widgets to PGPLOT.
    */
   if ( (pgplot_win = cpgopen(xap_device_name(w_plot))) <= 0 )
   {
      return NULL;
   }

   xawButtonUserdata.xaw_winpgplot->pgplot_win = pgplot_win;

   /*
    * Display the initial plot.
    */
   drawscene_init(pdata, w_plot);
   drawscene(pdata, w_plot);


   XtAppMainLoop(app_con);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*.......................................................................
* This is a motion-event callback for the image window. It reports the
* current position of the cursor in world coordinates.
*
* Input:
*  w           Widget   The im->w_image PGPLOT widget.
*  context  XtPointer   The im->w_coord label widget cast to XtPointer.
*  event       XEvent * The motion event.
*  call_next  Boolean * *call_next will be left as True so that any
*                       following event handlers will get called.
*/
static void report_cursor(Widget w, XtPointer context, XEvent *event, Boolean *call_next)
{
   xap_arm_cursor(w, XAP_NORM_CURSOR, 0.0f, 0.0f, xaw_cursor_inputs, context);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void xaw_cursor_inputs(Widget w, XtPointer client_data, XtPointer call_data)
{
   XapCursorCallbackStruct *cursor         = (XapCursorCallbackStruct *) call_data;
   WIDGET_USERDATA_STRUCT  *canvasUserData = (WIDGET_USERDATA_STRUCT *) client_data;

   switch(cursor->key)
   {

      case 'A':
         printf("cursor -> A \n");
         do_quit(w, client_data, call_data);
         break;

      /*
      case 'X':
         printf("cursor -> X \n");
         do_step(*canvasUserData->xaw_winpgplot->buttonStep, client_data, call_data);
         break;
      */
      case 'X':
         printf("cursor -> D \n");
         do_quit(w, client_data, call_data);
         break;
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


