

/** \file graphics1D_vogle_transient_xt_window.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include <unistd.h>
#include <pthread.h>

#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/Toggle.h>
#endif

#ifdef HAVE_VOGLE
#include "vogle.h"         /* Librairie VOGLE       */
#include "voglex.h"        /* Librairie VOGLE + X11 */
#include "vopl.h"          /* Librairie VOPL        */
#endif

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_transient.h"

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

typedef struct XAW_WINVOGLE_
{
   /* VOGLE window ID */
   int vogle_win;

   /* Xt id workproc */
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
	
	int film_output_type;

} XAW_WINVOGLE ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINVOGLE    * xaw_win; /* pointer to window stuff */

   SVQueue         * queue; /* include the data to plot */
   VOPL_GRAPH_DATA * pdata; /* stores the last data got from the queue */

} WIDGET_USERDATA_STRUCT;


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

WIDGET_USERDATA_STRUCT * widget_data_tr; /* the static data containing the plotdata */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* callbacks for "X11" */
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

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene_init(const VOPL_GRAPH_DATA* pdata, Widget w)
{
   int nx = pdata->nx;
   int ny = pdata->ny;

   subp(nx,ny);

   expandviewport();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene (const VOPL_GRAPH_DATA *pdata)
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
   backbuffer();

   /**/
   for (k=0; k<nx*ny; k++)
   {
      pdata->dataplot[k]->draw( pdata->dataplot[k] ); /* the dataplot knowns its "draw" function */
   }

   /* finito */
   swapbuffers();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void generic_output(WIDGET_USERDATA_STRUCT *widget_data, VOGLE_OUTPUT_FORMAT output_format)
{
   int w2;
   /*
    * output_format must be "VOGLEe_PPM" or "VOGLEe_GIF" or "VOGLEe_TIFF" or "VOGLEe_XBM" or or "VOGLEe_PNG or "VOGLEe_PPS"
    */
   char string_output[64];
   char vogle_output_id[8];
   char *ext = "ppm";

   if ( widget_data->pdata == NULL )
   {
      return;
   }

   switch( output_format )
   {
      case VOGLEe_PPM :
         strncpy(vogle_output_id, "ppm", 4); ext = "ppm";
         break;

      case VOGLEe_GIF :
         strncpy(vogle_output_id, "gif", 4); ext = "gif";
         break;

      case VOGLEe_TIFF:
         strncpy(vogle_output_id, "p6", 4); ext = "tiff"; setenv("P6TO", "TIFF", 1); /* for p6to script */
         break;

      case VOGLEe_XBM:
         strncpy(vogle_output_id, "p6", 4); ext = "xbm" ; setenv("P6TO", "XBM", 1); /* for p6to script */
         break;

      case VOGLEe_PNG:
         strncpy(vogle_output_id, "p6", 4); ext = "png" ; setenv("P6TO", "PNG", 1); /* for p6to script */
         break;

      case VOGLEe_PPS :
         strncpy(vogle_output_id, "pps",4); ext = "pps";
         break;

      default:
         return;
   }

   snprintf(string_output, 63, "%s.%s", widget_data->pdata->title, ext);

   /* in the script "p6to", we use the environment variables $P6TO and $VOGLE_OUTPUT_FILE */
   switch( output_format )
   {
		case VOGLEe_PPM:
		case VOGLEe_GIF:
		case VOGLEe_PPS :
			setenv("VOGLE_OUTPUT_FILE", string_output, 1);
			break;
		default:
			setenv("VOGLE_OUTPUT_FILE", widget_data->pdata->title, 1); /* for p6to , see at the end of the file */
	}

   /**/
   fprintf(stderr, "Producing output %s\n", string_output);
	/*voutput(string_output);*/

   prefsize(widget_data->pdata->window_size_x, widget_data->pdata->window_size_y);

   w2 = vopl_winopen(vogle_output_id, string_output);
   vopl_winset(w2);
   drawscene_init(widget_data->pdata, NULL);
   drawscene(widget_data->pdata);
   vopl_windel(w2);  /* draw on del */

   vopl_winset(widget_data->xaw_win->vogle_win);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void idle_workproc_no_x(SVQueue *queue)
{
   /* get the data from the SQueue - this waits in case of the queue is empty ... */
   VOPL_GRAPH_DATA * pdata = queue->xget(queue);

   if ( pdata == NULL )
   {
      return;
   }

   if      ( strcmp(pdata->driver, "/PPM" ) == 0 ) generic_output(NULL, VOGLEe_PPM);
   else if ( strcmp(pdata->driver, "/GIF" ) == 0 ) generic_output(NULL, VOGLEe_GIF);
   else if ( strcmp(pdata->driver, "/TIFF") == 0 ) generic_output(NULL, VOGLEe_TIFF);
   else if ( strcmp(pdata->driver, "/XBM" ) == 0 ) generic_output(NULL, VOGLEe_XBM);
   else if ( strcmp(pdata->driver, "/PNG" ) == 0 ) generic_output(NULL, VOGLEe_PNG);
   else if ( strcmp(pdata->driver, "/PPS" ) == 0 ) generic_output(NULL, VOGLEe_PPS);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*
 * The function that the Xt-MainLoop calls every and every times...
 *
 * - it is waiting for the queue to be not empty
 *
 * - if there is nothing to plot, but stop_on is "true" , then we sleep 0.1 sec. The mainloop
 *      calls again and again the function idle_workproc() so the cpu will not be too overloaded
 *      thanks to this "sleep" ( the main thread itself is waiting for a signal "stop_off" )
 *
 */

static Boolean idle_workproc (XtPointer data)
{
   WIDGET_USERDATA_STRUCT * widgetUserData =  (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetUserData->xaw_win->exit == 1 )  /* exit handler */
   {
      /* XtRemoveWorkProc ( widgetUserData->xaw_win->id_idleworkproc ); */
      vexit();
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
      /* get the data from the SVQueue - this waits in case of the queue is empty ... */
      VOPL_GRAPH_DATA * pdata = widgetUserData->queue->xget(widgetUserData->queue);

      /* free old data stored in widgetUserData->pdata */
      widgetUserData->queue->freed(widgetUserData->pdata);
      /* assign new data to widgetUserData->pdata */
      widgetUserData->pdata     = pdata;
      
      /* and draw the data */
      drawscene(widgetUserData->pdata);
      
      /* film */
      if (widgetUserData->xaw_win->film == 1)
      {
         generic_output(widgetUserData, widgetUserData->xaw_win->film_output_type);
      }

      return False;
   }
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void quit(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   widget_data_tr->xaw_win->exit = 1;
}

static void repaint(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   drawscene(widget_data_tr->pdata);
}

static void resize(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   Dimension       w, h;

   XtVaGetValues (widget, XtNwidth , &w, NULL);
   XtVaGetValues (widget, XtNheight, &h, NULL);

   vo_xt_win_size(widget_data_tr->xaw_win->vogle_win, (int)w, (int)h);
   calcviewport();

   drawscene(widget_data_tr->pdata);
}

static void stop_run(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   Arg wargs[1];

   /* >>> change the label of the Xaw button */
   switch (widget_data_tr->xaw_win->stop)
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Run"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Stop"); break;
   }
   XtSetValues(*widget_data_tr->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change the label of the Xaw button */

   widget_data_tr->xaw_win->stop = (widget_data_tr->xaw_win->stop==0) ? 1 : 0 ;
}

/*--------------------------------------------------------------------------------------*/

static void step(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   Arg wargs[1];

   /* >>> change the label of the Xaw button */
   XtSetArg(wargs[0], XtNlabel, "Run");
   XtSetValues(*widget_data_tr->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change the label of the Xaw button */

   widget_data_tr->xaw_win->step = 1;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_quit(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT * widget_data = (WIDGET_USERDATA_STRUCT *)data;

   widget_data->xaw_win->stop = 1;
   widget_data->xaw_win->step = 0;
   widget_data->xaw_win->exit = 1;
}

static void do_stop(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   /* >>> change legend of button Stop/Run */
   Arg		wargs[1];

   switch (widget_data->xaw_win->stop)
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Run"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Stop"); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Stop/Run */

   widget_data->xaw_win->stop = (widget_data->xaw_win->stop==0) ? 1: 0;
}

static void do_step(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   /* >>> change legend of button Stop/Run */
   Arg wargs[1];
   XtSetArg(wargs[0], XtNlabel, "Run");
   XtSetValues(*widget_data->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change legend of button Stop/Run */

   widget_data->xaw_win->step = 1;
}

static void do_film(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   Arg		wargs[1];
   /* >>> change legend of button Film On/Off */
   switch ( (int)call_data )
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Film Off"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Film On "); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Film On/Off */

   widget_data->xaw_win->film = (int)call_data;
}

static void do_ppm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   if ( widget_data->xaw_win->stop == 1 )
   {
       generic_output(widget_data, VOGLEe_PPM);
   }
	
	widget_data->xaw_win->film_output_type = VOGLEe_PPM;
}

static void do_gif_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   if ( widget_data->xaw_win->stop == 1 )
   {
       generic_output(widget_data, VOGLEe_GIF);
   }
	
	widget_data->xaw_win->film_output_type = VOGLEe_GIF;
}

static void do_tiff_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   if ( widget_data->xaw_win->stop == 1 )
   {
       generic_output(widget_data, VOGLEe_TIFF);
   }
	
	widget_data->xaw_win->film_output_type = VOGLEe_TIFF; /* if "film" set "on", the program will be quite slow... */
}

static void do_xbm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *userdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( userdata->xaw_win->stop == 1 )
   {
       generic_output(userdata, VOGLEe_XBM);
   }
	
	userdata->xaw_win->film_output_type = VOGLEe_XBM; /* if "film" set "on", the program will be quite slow... */
}

static void do_png_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;
	
   if ( widget_data->xaw_win->stop == 1 )
   {
		generic_output(widget_data, VOGLEe_PNG);
   }
	
	widget_data->xaw_win->film_output_type = VOGLEe_PNG; /* if "film" set "on", the program will be quite slow... */
}

static void do_pps_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   if ( widget_data->xaw_win->stop == 1 )
   {
       generic_output(widget_data, VOGLEe_PPS);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * vopl_curve_with_xt_toolkit_transient(void* data)
{
   SVQueue * queue = (SVQueue *)data;

   VOPL_GRAPH_DATA * pdata = queue->xget(queue);

   int SIZEX = pdata->window_size_x;
   int SIZEY = pdata->window_size_y;

   /*-----------------------------------------------------------------------------------*/
   /*-----------------------------------------------------------------------------------*/

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

   /*-----------------------------------------------------------------------------------*/

   XInitThreads();

   /*-----------------------------------------------------------------------------------*/

   int vogle_win = -1;

   int id_idleworkproc;
   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;

   Display   *dpy;
   Window    win;

   Arg		 wargs[5];
   Widget	 shell, canvas;
   Widget    xpanel;
   Widget    quit_button, stop_button, step_button;
   Widget    ppm_button, gif_button, tiff_button, xbm_button, png_button, pps_button;
   Widget    film_button;
   
   XtAppContext app_con;   /* application context */

   /*-----------------------------------------------------------------------------------*/

   XAW_WINVOGLE *xaw_window = malloc( sizeof(XAW_WINVOGLE) );

   WIDGET_USERDATA_STRUCT xawWidgetUserdata;

   /*-----------------------------------------------------------------------------------*/

   xaw_window->stop = 1;
   xaw_window->step = 0;
   xaw_window->exit = 0;
   xaw_window->film = 0;

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

   /* STOP */
   XtSetArg(wargs[0], XtNlabel, "Run ");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   stop_button = XtCreateManagedWidget("stop", commandWidgetClass, xpanel, wargs, 2);

   /* STEP */
   XtSetArg(wargs[0], XtNlabel, "Step");
   XtSetArg(wargs[1], XtNfromHoriz, stop_button);
   step_button = XtCreateManagedWidget("step", commandWidgetClass, xpanel, wargs, 2);

   /* FILM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "Film Off");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   XtSetArg(wargs[2], XtNhorizDistance, SIZEX / 2);
   film_button = XtCreateManagedWidget(" film ", toggleWidgetClass, xpanel, wargs, 3);

   /* PPM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   XtSetArg(wargs[2], XtNhorizDistance, SIZEX - 250);
   ppm_button = XtCreateManagedWidget("ppm", commandWidgetClass, xpanel, wargs, 3);

   /* GIF-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "GIF");
   XtSetArg(wargs[1], XtNfromHoriz, ppm_button);
   gif_button = XtCreateManagedWidget("gif", commandWidgetClass, xpanel, wargs, 2);

   /* TIFF-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "TIFF");
   XtSetArg(wargs[1], XtNfromHoriz, gif_button);
   tiff_button = XtCreateManagedWidget("tiff", commandWidgetClass, xpanel, wargs, 2);

   /* PNG-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PNG");
   XtSetArg(wargs[1], XtNfromHoriz, tiff_button);
   png_button = XtCreateManagedWidget("png", commandWidgetClass, xpanel, wargs, 2);

   /* XBM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "XBM");
   XtSetArg(wargs[1], XtNfromHoriz, png_button);
   xbm_button = XtCreateManagedWidget("xbm", commandWidgetClass, xpanel, wargs, 2);

   /* PPS-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PS");
   XtSetArg(wargs[1], XtNfromHoriz, xbm_button);
   pps_button = XtCreateManagedWidget("pps", commandWidgetClass, xpanel, wargs, 2);

   /* user value */
   xawWidgetUserdata.queue                  = queue;
   xawWidgetUserdata.pdata                  = pdata;
   xawWidgetUserdata.xaw_win                = xaw_window;
   xawWidgetUserdata.xaw_win->buttonQuit    = &quit_button;
   xawWidgetUserdata.xaw_win->buttonStopRun = &stop_button;
   xawWidgetUserdata.xaw_win->buttonStep    = &step_button;
   xawWidgetUserdata.xaw_win->buttonFilm    = &film_button;
   
   /* set callback & callback data for XAW buttons --------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit       , &xawWidgetUserdata);
   XtAddCallback(stop_button, XtNcallback, do_stop       , &xawWidgetUserdata);
   XtAddCallback(step_button, XtNcallback, do_step       , &xawWidgetUserdata);
   XtAddCallback(film_button, XtNcallback, do_film       , &xawWidgetUserdata);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output , &xawWidgetUserdata);
   XtAddCallback(gif_button , XtNcallback, do_gif_output , &xawWidgetUserdata);
   XtAddCallback(tiff_button, XtNcallback, do_tiff_output, &xawWidgetUserdata);
   XtAddCallback(png_button,  XtNcallback, do_png_output,  &xawWidgetUserdata);
   XtAddCallback(xbm_button , XtNcallback, do_xbm_output , &xawWidgetUserdata);
   XtAddCallback(pps_button , XtNcallback, do_pps_output , &xawWidgetUserdata);
   /* -----------------------------------------------------------------------*/

   /* vogle canvas */
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

   /* set-up the vogle window */
   dpy = XtDisplay(canvas);
   win = XtWindow(canvas);

   if (vogle_win == -1)
   {
      vogle_win = vo_xt_window(dpy, win, SIZEX, SIZEY);
      vopl_wininit(vogle_win);

      xaw_window->vogle_win = vogle_win;

      drawscene_init(pdata, NULL);
   }

   /* set a idle_workproc who will stop/run throught thread condition/signal */
   id_idleworkproc = XtAppAddWorkProc (app_con, (XtWorkProc)idle_workproc, &xawWidgetUserdata );
   /* set a idle_workproc who will be removed at quit/exit */
   xawWidgetUserdata.xaw_win->id_idleworkproc = id_idleworkproc;

   widget_data_tr = &xawWidgetUserdata; /* assign static data pointer */

   /* the event-loop */
   XtAppMainLoop(app_con);

   /* end of the routine ... */
   pthread_exit(NULL);
   
   /* as return value ! */
   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* the p6to script used by vogle for the output 

      #!/bin/sh
      PATH=$PATH:/usr/local/netpbm
      export PATH

      # this is the p6to script used by the VOGLE p6 driver

      case "$P6TO" in
      PPM)  cat       > $VOGLE_OUTPUT_FILE.ppm   # ppm file
      ;;
      GIF)  ppmtogif  > $VOGLE_OUTPUT_FILE.gif   # gif file
      ;;
      TIFF) pnmtotiff > $VOGLE_OUTPUT_FILE.tiff  # tiff file
      ;;
      XBM) 'ppmtopgm|pgmtopbm|pbmtoxbm' > $VOGLE_OUTPUT_FILE.xbm # greyscale X11 bitmap
      ;;
      *)    cat > $$.ppm # the default is ppm
      ;;
      esac
      exit

*/

/* for thsi reason we need to set P6TO and VOGLE_OUTPUT_FILE */
