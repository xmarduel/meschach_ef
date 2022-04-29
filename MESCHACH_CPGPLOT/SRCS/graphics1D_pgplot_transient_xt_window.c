/** \file graphics1D_pgplot_transient_xt_window.c
 *
 * Contains the routines for plotting with CPGPLOT library
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

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

#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_svqueue.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_transient.h"

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

typedef struct XAW_WINPGPLOT_
{
   /* PGPLOT window ID */
   int pgplot_win;

   /* Xt id workproc */
   int id_idleworkproc;
   
   /* pointer to widgets */
   Widget * buttonStopRun;
   Widget * buttonStep;
   Widget * buttonQuit;
   Widget * buttonFilm;

   /* flags */
   int stop ;
   int step ;
   int exit ;
   int film ;

   int film_output_type;

   Widget * w_frame;  /* A frame widget */
   Widget * w_plot;

} XAW_WINPGPLOT ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINPGPLOT  * xaw_win; /* pointer to window stuff */
   
   SVQueue        * queue; /* include the data to plot */
   PGPLOT_GRAPH_DATA * pdata; /* stores the last data got from the queue */
   
} WIDGET_USERDATA_STRUCT;


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void* idle_workproc_no_x(SVQueue *queue);

/* callbacks for buttons "Xaw" */
static void do_quit      (Widget w, XtPointer pdata, XtPointer data2);
static void do_stop      (Widget w, XtPointer pdata, XtPointer data2);
static void do_step      (Widget w, XtPointer pdata, XtPointer data2);
static void do_film      (Widget w, XtPointer pdata, XtPointer data2);

/* outputs from buttons "Xaw" */
static void do_ppm_output(Widget w, XtPointer pdata, XtPointer data2);
static void do_gif_output(Widget w, XtPointer pdata, XtPointer data2);
static void do_pps_output(Widget w, XtPointer pdata, XtPointer data2);

/* draw routines */
static void drawscene_init(Widget w, XtPointer data);
static void drawscene     (Widget w, XtPointer data);
static void redraw        (Widget w, XtPointer data);

static void report_cursor(Widget w, XtPointer context, XEvent *event, Boolean *call_next);
static void xaw_cursor_inputs(Widget w, XtPointer client_data, XtPointer call_data);

static void generic_output(const WIDGET_USERDATA_STRUCT *widgetUserdata, PGPLOT_OUTPUT_FORMAT output_format);

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene_init(Widget w, XtPointer data)
{
   PGPLOT_GRAPH_DATA* pdata = (PGPLOT_GRAPH_DATA *)data;

   int nx = pdata->nx;
   int ny = pdata->ny;

   double size_x = pdata->window_size_x;
   double size_y = pdata->window_size_y;
   
   cpgscr(0, 0.0, 0.0, 0.0); /* black background */
   cpgpap(PIXEL_TO_CENTIMETER(pdata->window_size_x), size_y/size_x);
   cpgask(0);

   cpgsubp(nx,ny); /* plotting window divided in (nx x ny) sub-windows */
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene(Widget w, XtPointer data)
{
   PGPLOT_GRAPH_DATA* pdata = (PGPLOT_GRAPH_DATA *)data;

   int nx ;
   int ny ;

   int k;

   if ( pdata == NULL )
   {
      return;
   }

   nx = pdata->nx;
   ny = pdata->ny;
   
   cpgbbuf();
   /**/
   for (k=0; k<nx*ny; k++)
   {
      pdata->dataplot[k]->draw( pdata->dataplot[k] ); /* the dataplot knowns its "draw" function */
   }
   /**/
   cpgebuf();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void redraw(Widget w, XtPointer data)
{
   PGPLOT_GRAPH_DATA * pdata = ( PGPLOT_GRAPH_DATA *)data;

   if ( pdata == NULL )
   {
      return;
   }

   /* the plotting stuff here ... */
   drawscene(w, pdata);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void* idle_workproc_no_x(SVQueue *queue)
{
   do
   {
      /* get the data from the SQueue - this waits in case of the queue is empty ... */
      PGPLOT_GRAPH_DATA * pdata = queue->xget(queue);

      if ( pdata == NULL )
      {
         return NULL;
      }

      {
         /* ppm-gif-tiff-pps */
         char pgplot_output_id[64];
         char* pgplot_output = pgplot_output_id;

         if ( strcmp(pdata->driver, "/PPM") == 0 )
         {
            snprintf(pgplot_output_id, 63, "%s.%s", pdata->title, "ppm");
            pgplot_output = strncat(pgplot_output_id, "/PPM", 7);
         }
         else
         if ( strcmp(pdata->driver, "/GIF") == 0 )
         {
            snprintf(pgplot_output_id, 63, "%s.%s", pdata->title, "gif");
            pgplot_output = strncat(pgplot_output_id, "/GIF", 7);
         }
         else
         if ( strcmp(pdata->driver, "/PPS") == 0 )
         {
            snprintf(pgplot_output_id, 63, "%s.%s", pdata->title, "pps");
            pgplot_output = strncat(pgplot_output_id, "/PPS", 7);
         }
         else
         {
            fprintf(stderr,"Driver !!! %s\n", pdata->driver);
            return NULL;
         }
      
         fprintf(stderr,"Producing output on file %s\n", pgplot_output);

         /* start new driver plot */
         cpgopen(pgplot_output);
   
         drawscene_init(NULL, pdata);
         drawscene(NULL, pdata);

         /* close it */
         cpgclos();
      }
      
      /* free mem */
      queue->freed(pdata);
      
   } while (1);

   /**/
   return NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static Boolean idle_workproc (XtPointer data)
{
   WIDGET_USERDATA_STRUCT * canvasUserData =  (WIDGET_USERDATA_STRUCT *)data;
   
   if ( canvasUserData->xaw_win->exit == 1 )
   {
      XtRemoveWorkProc ( canvasUserData->xaw_win->id_idleworkproc );
      exit(0);
   }


   if ( canvasUserData->xaw_win->stop == 1 && canvasUserData->xaw_win->step == 0 )  /* stop handler */
   {
      usleep(100); 
      return False;
   }

   if ( canvasUserData->xaw_win->step == 1 ) /* step handler */
   {
      canvasUserData->xaw_win->step = 0; /* reset step */
      canvasUserData->xaw_win->stop = 1; /* and stop later */
   }

   
   {
      /* get the data from the SQueue - this waits in case of the queue is empty ... */
      PGPLOT_GRAPH_DATA * pdata = canvasUserData->queue->xget(canvasUserData->queue);
      /* free old data stored in widgetUserData->pdata */
      canvasUserData->queue->freed(canvasUserData->pdata); /* trying to free the old mem there */
      /* store pointer to new data */
      canvasUserData->pdata = pdata;
      
      /* and then */
      redraw(*canvasUserData->xaw_win->w_plot, pdata);

      /* film -> gif output */
      if (canvasUserData->xaw_win->film == 1)
      {
			if (canvasUserData->xaw_win->film_output_type != PGPLOTe_NO_OUTPUT)
         {
            generic_output(canvasUserData, canvasUserData->xaw_win->film_output_type);
         }
      }

      return False;
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_quit(Widget w, XtPointer data, XtPointer data2) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT * xdata = (WIDGET_USERDATA_STRUCT *)data;

   xdata->xaw_win->stop = 0;
   xdata->xaw_win->step = 0;
   xdata->xaw_win->exit = 1;
}

static void do_stop(Widget w, XtPointer data, XtPointer data2) /* Xaw callback */
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

static void do_step(Widget w, XtPointer data, XtPointer data2) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;
   
   /* >>> change legend of button Stop/Run */
   Arg wargs[1];
   XtSetArg(wargs[0], XtNlabel, "Run");
   XtSetValues(*xdata->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change legend of button Stop/Run */

   xdata->xaw_win->step = 1;
}

static void do_film(Widget w, XtPointer data, XtPointer data2) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;
   
   /* >>> change legend of button Film On/Off */
   Arg		wargs[1];

   switch ((int)data2)
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Film Off"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Film On "); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Film On/Off */

   xdata->xaw_win->film = (int)data2;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void generic_output(const WIDGET_USERDATA_STRUCT *widgetUserdata, PGPLOT_OUTPUT_FORMAT output_format)
{
   char pgplot_output_id[64];
   char* pgplot_output = pgplot_output_id;

   if ( widgetUserdata->pdata == NULL )
   {
      return;
   }
        
   switch(output_format)
   {
      case PGPLOTe_PPM:
         snprintf(pgplot_output_id, 63, "%s.%s", widgetUserdata->pdata->title, "ppm");
         pgplot_output = strncat(pgplot_output_id, "/PPM", 7);
         break;

      case PGPLOTe_GIF:
         snprintf(pgplot_output_id, 63, "%s.%s", widgetUserdata->pdata->title, "gif");
         pgplot_output = strncat(pgplot_output_id, "/GIF", 7);
         break;

      case PGPLOTe_PPS:
         snprintf(pgplot_output_id, 63, "%s.%s", widgetUserdata->pdata->title, "pps");
         pgplot_output = strncat(pgplot_output_id, "/PPS", 7);
         break;

      default:
         return;
   }

   fprintf(stderr,"Producing %s\n", pgplot_output);

   
   /* start new driver plot */
   cpgopen(pgplot_output);

   drawscene_init(*widgetUserdata->xaw_win->w_plot, widgetUserdata->pdata);
   drawscene(*widgetUserdata->xaw_win->w_plot, widgetUserdata->pdata);
   
   /* close it */
   cpgclos();

   /* back to X11 */
   cpgslct(widgetUserdata->xaw_win->pgplot_win);
}

static void do_ppm_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( xdata->xaw_win->stop == 1 )
   {
      generic_output(xdata, PGPLOTe_PPM);
   }
	
   xdata->xaw_win->film_output_type = PGPLOTe_PPM;
}

static void do_pps_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( xdata->xaw_win->stop == 1 )
   {
      generic_output(xdata, PGPLOTe_PPS);
   }
	
   xdata->xaw_win->film_output_type = PGPLOTe_PPS;
} 

static void do_gif_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT  *xdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( xdata->xaw_win->stop == 1 )
   {
      generic_output(xdata, PGPLOTe_GIF);
   }
	
   xdata->xaw_win->film_output_type = PGPLOTe_GIF;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*
 *	The "graphic thread"
 */
void * pgplot_curve_with_xt_toolkit_transient(void* data)
{
   SVQueue *queue = (SVQueue *)data;

   PGPLOT_GRAPH_DATA *pdata = queue->xget(queue);

   /*--------------------------------------------------------------------------------------*/

   if ( strcmp(pdata->driver,"/XWINDOW") != 0 )
   {
      return idle_workproc_no_x(queue);
   }

   /*--------------------------------------------------------------------------------------*/

   /* default parameters for widgets */
   static String default_resources[] =
   {
      (String)NULL
   };

   /*--------------------------------------------------------------------------------------*/

   XtAppContext app_con;         /* application context */

   int pgplot_win = -1;
   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;

   Arg		    wargs[5];
   Widget	    shell;
   Widget       xpanel;
   Widget       quit_button, stop_button, step_button;
   Widget       film_button, ppm_button, pps_button, gif_button;

   Widget w_frame;  /* A frame widget */
   Widget w_plot;
      
   XAW_WINPGPLOT *xaw_pgplot_window = malloc( sizeof(XAW_WINPGPLOT) );

   WIDGET_USERDATA_STRUCT xawButtonUserdata;

   xaw_pgplot_window->stop = 1;
   xaw_pgplot_window->step = 0;
   xaw_pgplot_window->exit = 0;
   xaw_pgplot_window->film = 0;
	
	xaw_pgplot_window->film_output_type = PGPLOTe_NO_OUTPUT;
   
   /* ---------------------------------------------------------------------------------------- */

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

   /* pgplot film-output */
   XtSetArg(wargs[0], XtNlabel, " Film Off ");
   XtSetArg(wargs[1], XtNhorizDistance, pdata->window_size_x - 300);
   film_button = XtCreateManagedWidget("film", toggleWidgetClass, xpanel, wargs, 2);

   /* pgplot ppm-output */
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNhorizDistance, pdata->window_size_x - 90);
   ppm_button = XtCreateManagedWidget("ppm", commandWidgetClass, xpanel, wargs, 2);

   /* pgplot gif-output */
   XtSetArg(wargs[0], XtNlabel, "GIF");
   XtSetArg(wargs[1], XtNfromHoriz, ppm_button);
   gif_button = XtCreateManagedWidget("gif", commandWidgetClass, xpanel, wargs, 2);

   /* pgplot pps-output */
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
   xawButtonUserdata.queue                        = queue;
   xawButtonUserdata.pdata                        = pdata;
   xawButtonUserdata.xaw_win                = xaw_pgplot_window;
   xawButtonUserdata.xaw_win->buttonStopRun = &stop_button;
   xawButtonUserdata.xaw_win->buttonStep    = &step_button;
   xawButtonUserdata.xaw_win->buttonQuit    = &quit_button;
   xawButtonUserdata.xaw_win->buttonFilm    = &film_button;
   xawButtonUserdata.xaw_win->w_frame       = &w_frame;
   xawButtonUserdata.xaw_win->w_plot        = &w_plot;
   
   /* set callback & callback data for XAW buttons --------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit      , &xawButtonUserdata);
   XtAddCallback(stop_button, XtNcallback, do_stop      , &xawButtonUserdata);
   XtAddCallback(step_button, XtNcallback, do_step      , &xawButtonUserdata);
   XtAddCallback(film_button, XtNcallback, do_film      , &xawButtonUserdata);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output, &xawButtonUserdata);
   XtAddCallback(gif_button , XtNcallback, do_gif_output, &xawButtonUserdata);
   XtAddCallback(pps_button , XtNcallback, do_pps_output, &xawButtonUserdata);
   /* -----------------------------------------------------------------------*/

   /*
    * Register a motion-event callback such that the cursor position can
    * be reported in the im->w_coord label widget.
    */
   XtAddEventHandler(w_plot, PointerMotionMask, False, report_cursor, &xawButtonUserdata);

   /*
    * Create the application menu-bar.
    */
   XtManageChild(xpanel);
   
   /* finish */
   XtRealizeWidget(shell);


   /*
    * Open the widgets to PGPLOT.
    */
   if ( (pgplot_win = cpgopen(xap_device_name(w_plot))) <= 0 )
   {
      return NULL;
   }

   xawButtonUserdata.xaw_win->pgplot_win = pgplot_win;

   /*
    * Display the initial plot.
    */
   drawscene_init(w_plot, pdata);
   drawscene(w_plot, pdata);
   
   /* set a idle_workproc who will stop/run throught thread condition/signal */
   xawButtonUserdata.xaw_win->id_idleworkproc = XtAppAddWorkProc (app_con, (XtWorkProc)idle_workproc, &xawButtonUserdata );
   /* set a idle_workproc who will stop/run throught thread condition/signal */

   /* the event-loop */
   XtAppMainLoop(app_con);
   

   /* end of the routine */
   pthread_exit(NULL);
   
   /* as return value ! */
   return NULL;
}

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
         do_stop(*canvasUserData->xaw_win->buttonStopRun, client_data, call_data);
         break;

      case 'X':
         printf("cursor -> X \n");
         do_step(*canvasUserData->xaw_win->buttonStep, client_data, call_data);
         break;

      case 'D':
         printf("cursor -> D \n");
         do_quit(w, client_data, call_data);
         break;
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
