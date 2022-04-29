/** \file graphics1D_libsciplot_transient_xt_window.c
 *
 * Contains the routines for plotting with LIBPLOT library
 *
 */

#include <stdio.h>
#include <unistd.h>

#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Core.h>
#include <X11/Xaw/Form.h>         /* XAW   */
#include <X11/Xaw/Command.h>      /* XAW   */
#include <X11/Xaw/Toggle.h>       /* XAW   */
#endif

#ifdef HAVE_LIBSCIPLOT
#include "libsciplot.h"
#endif

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_svqueue.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_stationnary.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_transient.h"

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

typedef struct XAW_WINLIBPLOT_
{
   /* VOGLE window ID */
   int libplot_win;

   /* MOTIF id workproc */
   int id_idleworkproc;

   /* pointer to widgets */
   Widget * buttonStopRun;
   Widget * buttonStep;
   Widget * buttonQuit;
   Widget * buttonFilm;

   /* flags */
   int exit ;
   int stop ;
   int step ;
   int film ;

} XAW_WINLIBPLOT ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINLIBPLOT * xaw_win; /* pointer to window stuff */

   SVQueue        * queue; /* include the data to plot */
   LIBSCIPLOT_GRAPH_DATA * pdata; /* stores the last data got from the queue */

} WIDGET_USERDATA_STRUCT;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static WIDGET_USERDATA_STRUCT * widgetUserData;

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* callbacks for "Xt" */
static void quit       (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void repaint    (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void resize     (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void stop_run   (Widget w, XEvent *ev, String *params, Cardinal *n_params); /* click button-1 */
static void step       (Widget w, XEvent *ev, String *params, Cardinal *n_params); /* click button-3 */

/* callbacks for "Xaw" */
static void do_quit        (Widget w, XtPointer data, XtPointer call_data);
static void do_stop        (Widget w, XtPointer data, XtPointer call_data);
static void do_step        (Widget w, XtPointer data, XtPointer call_data);
static void do_film        (Widget w, XtPointer data, XtPointer call_data);

static void do_ppm_output  (Widget w, XtPointer data, XtPointer call_data);
static void do_pps_output  (Widget w, XtPointer data, XtPointer call_data);
static void do_gif_output  (Widget w, XtPointer data, XtPointer call_data);
static void do_tiff_output (Widget w, XtPointer data, XtPointer call_data);
static void do_xbm_output  (Widget w, XtPointer data, XtPointer call_data);

static void generic_output(const WIDGET_USERDATA_STRUCT *widgetUserdata, LIBSCIPLOT_OUTPUT output_format);

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void generic_output(const WIDGET_USERDATA_STRUCT *widgetUserdata, LIBSCIPLOT_OUTPUT output_format)
{
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene_init(XtPointer data, Widget w)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = (LIBSCIPLOT_GRAPH_DATA *)data;

   int nx = pdata->nx;
   int ny = pdata->ny;

   graphics1D_libsciplot_subp(nx,ny);
}


static void drawscene(XtPointer data)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = (LIBSCIPLOT_GRAPH_DATA *)data;

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
   graphics1D_libsciplot_erase(); /* backbuffer(); */

   /**/
   for (k=0; k<nx*ny; k++)
   {
      pdata->dataplot[k]->draw( pdata->dataplot[k] ); /* the dataplot knowns its "draw" function */
   }
   /**/
   
   /* finito */
   graphics1D_libsciplot_flush(); /* swapbuffers(); */
   
}

void Redraw (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   /*drawscene(XXdata);*/
}


/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*
 * The function that the Xt-MainLoop calls every and every times...
 *
 * - it is waiting for the queue to be not empty
 *
 * - if there is nothing to plot, but stop_on is "true" , then we sleep 0.1 sec. The mainloop
 *      calls again and again the function idle_work_proc() so the cpu will not be too overloaded
 *      thanks to this "sleep" ( the main thread itself is waiting for a signal "stop_off" )
 *
 */

static Boolean idle_workproc (XtPointer data)
{
   WIDGET_USERDATA_STRUCT * widgetUserData =  (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetUserData->xaw_win->exit == 1 )  /* exit handler */
   {
			//XtRemoveWorkProc ( widgetUserData->xaw_win->id_idleworkproc ); // BAD_EXEC ???
      exit(0);
   }

   if ( widgetUserData->xaw_win->stop == 1 && widgetUserData->xaw_win->step == 0 )  /* stop handler */
   {
      usleep(100);
      return False;
   }
   
   if ( widgetUserData->xaw_win->step == 1 ) /* step handler */
   {
      widgetUserData->xaw_win->step = 0; /* reset step */
      widgetUserData->xaw_win->stop = 1; /* and stop later */
   }

   {
      /* get the data from the SQueue - this waits in case of the queue is empty ... */
      LIBSCIPLOT_GRAPH_DATA * pdata = widgetUserData->queue->xget(widgetUserData->queue);

      if (pdata) {
			/* free old data stored in widgetUserData->pdata */
         widgetUserData->queue->freed(widgetUserData->pdata);
         /* assign new data to widgetUserData->pdata */
         widgetUserData->pdata     = pdata;

         /* and then */
         drawscene(pdata);

         /* film -> ppm output */
         if (widgetUserData->xaw_win->film == 1)
         {
            generic_output(widgetUserData, LIBSCIPLOTe_PPM);
         }

         return False;
		}
   }
	
	return False;
	
}


void Quit (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
	exit (0);
}

/*--------------------------------------------------------------------------------------*/

static void stop_run(Widget w, XEvent *ev, String *params, Cardinal *n_params) /* Motif callback */
{
   /*WIDGET_USERDATA_STRUCT * canvasUserData;*/
   Arg wargs[1];

   /* >>> change the label of the Xaw button */
   /*       XtVaGetValues (w, XmNuserData, &canvasUserData, NULL); */
   switch (widgetUserData->xaw_win->stop==1)
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Run"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Stop"); break;
   }
   XtSetValues(*widgetUserData->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change the label of the Xaw button */

   widgetUserData->xaw_win->stop = (widgetUserData->xaw_win->stop==0) ? 1:0 ;
}

/*--------------------------------------------------------------------------------------*/

static void step(Widget w, XEvent *ev, String *params, Cardinal *n_params) /* Motif callback */
{
   /*WIDGET_USERDATA_STRUCT * canvasUserData;*/
   Arg wargs[1];

   /* >>> change the label of the Xaw button */
   /*XtVaGetValues (w, XmNuserData, &canvasUserData, NULL);*/
   XtSetArg(wargs[0], XtNlabel, "Run");
   XtSetValues(*widgetUserData->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change the label of the Xaw button */

   widgetUserData->xaw_win->step = 1;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_quit(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT * xdata = (WIDGET_USERDATA_STRUCT *)data;

   xdata->xaw_win->stop = 1;
   xdata->xaw_win->step = 0;
   xdata->xaw_win->exit = 1;
}

/*-------------------------------------------------------------------------------------*/

static void do_stop(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   /* >>> change legend of button Stop/Run */
   Arg		wargs[1];

   switch (xdata->xaw_win->stop)
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Run"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Stop"); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Stop/Run */

   xdata->xaw_win->stop = (xdata->xaw_win->stop==0) ? 1: 0;  
}

/*-------------------------------------------------------------------------------------*/

static void do_step(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   /* >>> change legend of button Stop/Run */
   Arg wargs[1];
   XtSetArg(wargs[0], XtNlabel, "Run");
   XtSetValues(*xdata->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change legend of button Stop/Run */

   xdata->xaw_win->step = 1;
}

/*--------------------------------------------------------------------------------------*/

static void do_film(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   Arg		wargs[1];
   /* >>> change legend of button Film On/Off */
   switch ( (int)call_data )
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Film Off"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Film On "); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Film On/Off */

   xdata->xaw_win->film = (int)call_data;
}

/*--------------------------------------------------------------------------------------*/

static void do_ppm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xdata, LIBSCIPLOTe_PPM);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_gif_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xdata, LIBSCIPLOTe_GIF);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_tiff_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   /*generic_output(xdata, LIBSCIPLOTe_TIFF);*/
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_xbm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   /*generic_output(xdata, LIBSCIPLOTe_XBM);*/
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_pps_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xdata, LIBSCIPLOTe_PPS);
}

/*--------------------------------------------------------------------------------------*/

static void quit(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   ;
}

/*--------------------------------------------------------------------------------------*/

static void repaint(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   drawscene(widgetUserData->pdata);
}

/*--------------------------------------------------------------------------------------*/

static void resize(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   ;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void* graphics1D_libsciplot_curve_with_xt_toolkit_transient(void* data)
{
   SVQueue *queue = (SVQueue *)data;
   
   LIBSCIPLOT_GRAPH_DATA *pdata = queue->xget(queue);

   int nx = pdata->nx;
   int ny = pdata->ny;
   
   int SIZEX = pdata->window_size_x;
   int SIZEY = pdata->window_size_y;
   
   int id_idleworkproc;
   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;
   
   Display	*dpy;
   Window	win;

   Arg		    wargs[5];
   Widget	    shell, canvas;
   XtAppContext app_con;         /* application context */
   Widget       xpanel;
   Widget       quit_button, stop_button, step_button;
   Widget       ppm_button, gif_button, tiff_button, xbm_button, pps_button;
   Widget       film_button;

   XAW_WINLIBPLOT *xaw_window = malloc( sizeof(XAW_WINLIBPLOT) );

   WIDGET_USERDATA_STRUCT xawWidgetUserdata;

   static const String translations =
      "<Expose>:        _repaint()\n \
       <Configure>:     _resize()\n \
       <Btn1Down>:      _toggle()\n\
       <Btn2Down>:      _quit()\n\
       <Btn3Down>:      _step()\n\
       <Key>q:          _quit()";

   static XtActionsRec trans_actions[] =
   {
      {"_repaint",    repaint},
      {"_resize",     resize},
      {"_toggle",     stop_run},
      {"_step",       step},
      {"_quit",       quit}
   };

   
   /* default parameters for widgets */
   static String default_resources[] =
   {
      (String)NULL
   };

   /* --------------------------------------------------------------------------------- */

   xaw_window->stop = 1;
   xaw_window->step = 0;
   xaw_window->exit = 0;
   xaw_window->film = 0;
   
   /* create toplevel shell widget */
   shell = XtAppInitialize (&app_con,
                           (String)"Solution FEM", /* app class */
                           NULL                    /* options */,
                           (Cardinal)0,            /* num of options */
                           &ac,                    /* command line */
                           av,                     /* command line */
                           default_resources,
                           NULL,                   /* ArgList */
                           (Cardinal)0             /* num of Args */ );


   /* set default widget parameters (including window size) */
   XtAppSetFallbackResources (app_con, default_resources);

   /* map actions to subroutines */
   XtAppAddActions (app_con, trans_actions, XtNumber (trans_actions));
   XtAddActions(trans_actions, XtNumber(trans_actions));

   xpanel = XtCreateManagedWidget("panel", formWidgetClass, shell, NULL, 0);
   /* Athena widgets : */

   /* QUIT */
   XtSetArg(wargs[0], XtNlabel, "Quit");
   quit_button = XtCreateManagedWidget("quit", commandWidgetClass, xpanel, wargs, 1);

   /* STOP */
   XtSetArg(wargs[0], XtNlabel, "Run ");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   stop_button = XtCreateManagedWidget("stop", commandWidgetClass, xpanel, wargs, 2);

   /* STEP */
   XtSetArg(wargs[0], XtNlabel, "Step");
   XtSetArg(wargs[1], XtNfromHoriz, stop_button);
   step_button = XtCreateManagedWidget("step", commandWidgetClass, xpanel, wargs, 2);

   /* FIM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "Film Off");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   XtSetArg(wargs[2], XtNhorizDistance, SIZEX / 2);
   film_button = XtCreateManagedWidget(" film ", toggleWidgetClass, xpanel, wargs, 3);

   /* PPM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   XtSetArg(wargs[2], XtNhorizDistance, SIZEX - 210);
   ppm_button = XtCreateManagedWidget("ppm", commandWidgetClass, xpanel, wargs, 3);

   /* GIF-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "GIF");
   XtSetArg(wargs[1], XtNfromHoriz, ppm_button);
   gif_button = XtCreateManagedWidget("gif", commandWidgetClass, xpanel, wargs, 2);

   /* TIFF-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "TIFF");
   XtSetArg(wargs[1], XtNfromHoriz, gif_button);
   tiff_button = XtCreateManagedWidget("tiff", commandWidgetClass, xpanel, wargs, 2);

   /* XBM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "XBM");
   XtSetArg(wargs[1], XtNfromHoriz, tiff_button);
   xbm_button = XtCreateManagedWidget("xbm", commandWidgetClass, xpanel, wargs, 2);

   /* PPS-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PS");
   XtSetArg(wargs[1], XtNfromHoriz, xbm_button);
   pps_button = XtCreateManagedWidget("pps", commandWidgetClass, xpanel, wargs, 2);

   /* user value */
   xawWidgetUserdata.queue                  = queue;
   xawWidgetUserdata.pdata                  = pdata;
   xawWidgetUserdata.xaw_win                = xaw_window;
   xawWidgetUserdata.xaw_win->buttonStopRun = &stop_button;
   xawWidgetUserdata.xaw_win->buttonStep    = &step_button;
   xawWidgetUserdata.xaw_win->buttonQuit    = &quit_button;
   xawWidgetUserdata.xaw_win->buttonFilm    = &film_button;
   
   /* set callback & callback data for XAW buttons --------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit       , &xawWidgetUserdata);
   XtAddCallback(stop_button, XtNcallback, do_stop       , &xawWidgetUserdata);
   XtAddCallback(step_button, XtNcallback, do_step       , &xawWidgetUserdata);
   XtAddCallback(film_button, XtNcallback, do_film       , &xawWidgetUserdata);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output , &xawWidgetUserdata);
   XtAddCallback(gif_button , XtNcallback, do_gif_output , &xawWidgetUserdata);
   XtAddCallback(tiff_button, XtNcallback, do_tiff_output, &xawWidgetUserdata);
   XtAddCallback(xbm_button , XtNcallback, do_xbm_output , &xawWidgetUserdata);
   XtAddCallback(pps_button , XtNcallback, do_pps_output , &xawWidgetUserdata);
   /* -----------------------------------------------------------------------*/

   
   /* vogle canvas */
   XtSetArg(wargs[0], XtNwidth , SIZEX);
   XtSetArg(wargs[1], XtNheight, SIZEY);
   XtSetArg(wargs[2], XtNfromVert, quit_button);
   XtSetArg(wargs[3], XtNtranslations, XtParseTranslationTable (translations));
   /* create canvas */
   canvas = XtCreateManagedWidget("canvas", simpleWidgetClass, xpanel, wargs, 4);
   
   /* finish */
   XtManageChild(canvas);
   
   /* finish */
   XtRealizeWidget(shell);

   
   /* set-up the vogle window */
   dpy = XtDisplay(canvas);
   win = XtWindow(canvas);


   {
      int rc = graphics1D_libsciplot_init_multigrapher("X", dpy, &win, SIZEX, SIZEY);
      drawscene_init(pdata, NULL);
   }

   /* set a idle_workproc who will stop/run throught thread condition/signal */
   id_idleworkproc = XtAppAddWorkProc (app_con, (XtWorkProc)idle_workproc, &xawWidgetUserdata );
   /* set a idle_workproc who will stop/run throught thread condition/signal */
   xawWidgetUserdata.xaw_win->id_idleworkproc = id_idleworkproc;

   widgetUserData = &xawWidgetUserdata;
      
   /* the event-loop */
   XtAppMainLoop(app_con);

   /* end of the routine ... */
   pthread_exit(NULL);
   
   /* as return value ! */
   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

