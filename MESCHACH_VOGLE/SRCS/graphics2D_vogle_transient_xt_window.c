

/** \file graphics2D_vogle_transient_xt_window.c
 *
 * Contains the routines for plotting with VOGLE library
 * Note: bug when debugging + output "P6" ( with pipe p6to )
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
#include <X11/Xaw/Command.h>
#endif

#ifdef HAVE_VOGLE
#include "vogle.h"         /* Librairie VOGLE       */
#include "voglex.h"        /* Librairie VOGLE + X11 */
#endif

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_transient.h"

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

} XAW_WINVOGLE ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINVOGLE   * xaw_win; /* pointer to window stuff */

   SVQueue          * queue; /* include the data to plot */
   VOGLE_SURFACE_DATA * pdata; /* stores the last data got from the queue */

} WIDGET_USERDATA_STRUCT;


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static WIDGET_USERDATA_STRUCT * xawWidgetUserData; /* static data */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* callbacks for "X11" */
static void quit       (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void repaint    (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void resize     (Widget w, XEvent *ev, String *params, Cardinal *n_params);

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

/* draw routine */
static void drawscene (const VOGLE_SURFACE_DATA *pdata);

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene_init(XtPointer data, Widget w)
{
   VOGLE_SURFACE_DATA* pdata = (VOGLE_SURFACE_DATA *)data;
   
   /*pdata->delete_objects(pdata);*/
   pdata->setup_view(pdata);
   pdata->setup_objects(pdata);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void drawscene (const VOGLE_SURFACE_DATA *pdata)
{
   if ( pdata == NULL )
   {
      return;
   }
   
   /* start */
   backbuffer();

   /**/
   pdata->draw( (void*)pdata ); /* the data knowns its "draw" function */
   /**/

   /* finito */
   swapbuffers();
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void generic_output(WIDGET_USERDATA_STRUCT *xawUserData, VOGLE_OUTPUT_FORMAT output_format)
{
   int w2;
   /*
    * output_format must be "VOGLEe_PPM" or "VOGLEe_GIF" or "VOGLEe_TIFF" or "VOGLEe_XBM" or "VOGLEe_PNG" or "VOGLEe_PPS"
    */
   char string_output[64];
   char vogle_output_id[8];
   char *ext = "ppm";

   if ( xawUserData->pdata == NULL )
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

      default: return;
   }
   
   snprintf(string_output, 63, "%s.%s", xawUserData->pdata->title, ext);

   /* in the script "p6to", we use the environment variables $P6TO and $VOGLE_OUTPUT_FILE */
   switch( output_format )
   {
		case VOGLEe_PPM:
		case VOGLEe_GIF:
		case VOGLEe_PPS :
			setenv("VOGLE_OUTPUT_FILE", string_output, 1);
			break;
		default:
			setenv("VOGLE_OUTPUT_FILE", xawUserData->pdata->title, 1); /* for p6to , see at the end of the file */
	}

   /**/
   fprintf(stderr,"Producing %s\n", string_output);
   /*voutput(string_output);*/

   prefsize(xawUserData->pdata->window_size_x, xawUserData->pdata->window_size_y);
   fprintf(stderr, "%d %d\n", xawUserData->pdata->window_size_x, xawUserData->pdata->window_size_y);
   w2 = winopen(vogle_output_id, string_output);
   winset(w2);
   drawscene_init(xawUserData->pdata, NULL);
   drawscene(xawUserData->pdata);
   windel(w2);  /* bug !!?? */

   winset(xawUserData->xaw_win->vogle_win);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void idle_workproc_no_x(SVQueue *queue)
{
   /* get the data from the SQueue - this waits in case of the queue is empty ... */
   VOGLE_SURFACE_DATA * pdata = queue->xget(queue);

   if ( pdata == NULL )
   {
      return;
   }

   if      ( strcmp(pdata->driver,"/PPM" ) == 0 ) generic_output(NULL, VOGLEe_PPM);
   else if ( strcmp(pdata->driver,"/GIF" ) == 0 ) generic_output(NULL, VOGLEe_GIF);
   else if ( strcmp(pdata->driver,"/TIFF") == 0 ) generic_output(NULL, VOGLEe_TIFF);
   else if ( strcmp(pdata->driver,"/XBM" ) == 0 ) generic_output(NULL, VOGLEe_XBM);
	else if ( strcmp(pdata->driver,"/PNG" ) == 0 ) generic_output(NULL, VOGLEe_PNG);
   else if ( strcmp(pdata->driver,"/PPS" ) == 0 ) generic_output(NULL, VOGLEe_PPS);
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
   WIDGET_USERDATA_STRUCT * xawWidgetUserData =  (WIDGET_USERDATA_STRUCT *)data;

   if ( xawWidgetUserData->xaw_win->exit == 1 )  /* exit handler */
   {
      XtRemoveWorkProc ( xawWidgetUserData->xaw_win->id_idleworkproc );
      vexit();
   }

   if ( xawWidgetUserData->xaw_win->stop == 1 && xawWidgetUserData->xaw_win->step == 0 )  /* stop handler */
   {
      usleep(100);
      return False;
   }
   
   if ( xawWidgetUserData->xaw_win->step == 1 )
   {
      xawWidgetUserData->xaw_win->step = 0;
      xawWidgetUserData->xaw_win->stop = 1;
   }

   {
      /* get the data from the SVQueue - this waits in case of the queue is empty ... */
      VOGLE_SURFACE_DATA * pdata = xawWidgetUserData->queue->xget(xawWidgetUserData->queue);

      pdata->view   = xawWidgetUserData->pdata->view;
      pdata->motion = xawWidgetUserData->pdata->motion;
      
      /* free old data stored in xawWidgetUserData->pdata */
      xawWidgetUserData->queue->freed(xawWidgetUserData->pdata);
      /* assign new data to xawWidgetUserData->pdata */
      xawWidgetUserData->pdata     = pdata;
      /* and draw the data */
      pdata->delete_objects( pdata );
      pdata->setup_objects( pdata );

      drawscene(xawWidgetUserData->pdata);
      
      /* film -> ppm output only */
      if (xawWidgetUserData->xaw_win->film == 1)
      {
         generic_output(xawWidgetUserData, VOGLEe_PPM);
      }

      return False;
   }
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void motion_action(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   if (*n_params != 1)
   {
      XtError ("Wrong number of args!");
   }

   if ( strcmp(params[0],"rotate") == 0 )
   {
      float rx,ry;

      int DELTA_X = event->xmotion.x - xawWidgetUserData->pdata->motion.MOTION_X_ROTATE;
      int DELTA_Y = event->xmotion.y - xawWidgetUserData->pdata->motion.MOTION_Y_ROTATE;

      xawWidgetUserData->pdata->motion.MOTION_X_ROTATE = event->xmotion.x;
      xawWidgetUserData->pdata->motion.MOTION_Y_ROTATE = event->xmotion.y;

      rx = 2 * DELTA_X / (float)(xawWidgetUserData->pdata->window_size_x);
      ry = 2 * DELTA_Y / (float)(xawWidgetUserData->pdata->window_size_y);

      xawWidgetUserData->pdata->view.ROTX  = fmod( xawWidgetUserData->pdata->view.ROTX + 180*ry , 360 );
      xawWidgetUserData->pdata->view.ROTY  = fmod( xawWidgetUserData->pdata->view.ROTY + 180*rx , 360 );

      /* redraw */
      repaint(widget, event, params, n_params);
   }
   
   if ( strcmp(params[0],"translate") == 0 )
   {
      float rx,ry;

      int DELTA_X = event->xmotion.x - xawWidgetUserData->pdata->motion.MOTION_X_TRANSLATE;
      int DELTA_Y = event->xmotion.y - xawWidgetUserData->pdata->motion.MOTION_Y_TRANSLATE;

      xawWidgetUserData->pdata->motion.MOTION_X_TRANSLATE = event->xmotion.x;
      xawWidgetUserData->pdata->motion.MOTION_Y_TRANSLATE = event->xmotion.y;

      rx = 20 * DELTA_X / (float)(xawWidgetUserData->pdata->window_size_x);
      ry = 20 * DELTA_Y / (float)(xawWidgetUserData->pdata->window_size_x);
   
      xawWidgetUserData->pdata->view.TX  += rx;
      xawWidgetUserData->pdata->view.TY  += ry;
      xawWidgetUserData->pdata->view.TZ   = 0.0;

      /* redraw */
      repaint(widget, event, params, n_params);
   }
}

static void motion_action_start(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   if (*n_params != 1)
   {
      XtError ("Wrong number of args!");
   }

   if ( strcmp(params[0],"rotate") == 0 )
   {
      xawWidgetUserData->pdata->motion.MOTION_X_ROTATE = event->xmotion.x ;
      xawWidgetUserData->pdata->motion.MOTION_Y_ROTATE = event->xmotion.y ;
   }
   if ( strcmp(params[0],"translate") == 0 )
   {
      xawWidgetUserData->pdata->motion.MOTION_X_TRANSLATE = event->xmotion.x ;
      xawWidgetUserData->pdata->motion.MOTION_Y_TRANSLATE = event->xmotion.y ;
   }
}

static void scale_action(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   xawWidgetUserData->pdata->view.SCAL *= ( event->xmotion.y > xawWidgetUserData->pdata->window_size_x/2 ?
                                 1 - xawWidgetUserData->pdata->motion.MOTION_SCALE_FACTOR :
                                 1 + xawWidgetUserData->pdata->motion.MOTION_SCALE_FACTOR );

   /* redraw */
   repaint(widget, event, params, n_params);
}

/*--------------------------------------------------------------------------------------*/

static void quit(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   xawWidgetUserData->xaw_win->exit = 1;
}

/*--------------------------------------------------------------------------------------*/

static void repaint(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   drawscene(xawWidgetUserData->pdata);
}

/*--------------------------------------------------------------------------------------*/

static void resize(Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   Dimension       l, h;

   XtVaGetValues (w, XtNwidth , &l, NULL);
   XtVaGetValues (w, XtNheight, &h, NULL);

   vo_xt_win_size(xawWidgetUserData->xaw_win->vogle_win, (int)l, (int)h);
   calcviewport();

   drawscene(xawWidgetUserData->pdata);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_quit(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT * widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   widgetdata->xaw_win->stop = 1;
   widgetdata->xaw_win->step = 0;
   widgetdata->xaw_win->exit = 1;
}

/*-------------------------------------------------------------------------------------*/

static void do_stop(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   /* >>> change legend of button Stop/Run */
   Arg		wargs[1];

   switch (widgetdata->xaw_win->stop)
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Run"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Stop"); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Stop/Run */

   widgetdata->xaw_win->stop = (widgetdata->xaw_win->stop==0) ? 1: 0;  
}

/*-------------------------------------------------------------------------------------*/

static void do_step(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   /* >>> change legend of button Stop/Run */
   Arg wargs[1];
   XtSetArg(wargs[0], XtNlabel, "Run");
   XtSetValues(*widgetdata->xaw_win->buttonStopRun, wargs, 1);
   /* <<< change legend of button Stop/Run */

   widgetdata->xaw_win->step = 1;
}

/*--------------------------------------------------------------------------------------*/

static void do_film(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   Arg		wargs[1];
   /* >>> change legend of button Film On/Off */
   switch ( (int)call_data )
   {
      case 0: XtSetArg(wargs[0], XtNlabel, "Film Off"); break;
      case 1: XtSetArg(wargs[0], XtNlabel, "Film On "); break;
   }

   XtSetValues(w, wargs, 1);
   /* <<< change legend of button Film On/Off */

   widgetdata->xaw_win->film = (int)call_data;
}

/*--------------------------------------------------------------------------------------*/

static void do_ppm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetdata->xaw_win->stop == 1 )
   {
       generic_output(widgetdata, VOGLEe_PPM);
   }
}

/*--------------------------------------------------------------------------------------*/

static void do_gif_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetdata->xaw_win->stop == 1 )
   {
       generic_output(widgetdata, VOGLEe_GIF);
   }
}

/*--------------------------------------------------------------------------------------*/

static void do_tiff_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetdata->xaw_win->stop == 1 )
   {
       generic_output(widgetdata, VOGLEe_TIFF);
   }
}

/*--------------------------------------------------------------------------------------*/

static void do_xbm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetdata->xaw_win->stop == 1 )
   {
       generic_output(widgetdata, VOGLEe_XBM);
   }
}

/*--------------------------------------------------------------------------------------*/

static void do_png_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;
	
   if ( widgetdata->xaw_win->stop == 1 )
   {
		generic_output(widgetdata, VOGLEe_PNG);
   }
}

/*--------------------------------------------------------------------------------------*/

static void do_pps_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT  *widgetdata = (WIDGET_USERDATA_STRUCT *)data;

   if ( widgetdata->xaw_win->stop == 1 )
   {
       generic_output(widgetdata, VOGLEe_PPS);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*
 * 	Init a xt-window 
 */
void * vogle_surface_with_xt_toolkit_transient(void* data)
{
   SVQueue *queue = (SVQueue *)data;

   VOGLE_SURFACE_DATA *pdata = queue->xget(queue);

   int SIZEX = pdata->window_size_x;
   int SIZEY = pdata->window_size_y;

   /*--------------------------------------------------------------------------------------*/
   /* ------------------------------------------------------------------------------------ */

   /*
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
   */
   static const  String translations =
      "<Expose>:        _repaint()\n  \
      <Configure>:      _resize()\n \
      Button1<Motion>:	_motion_action(rotate)\n \
      Button2<Motion>:	_motion_action(translate)\n \
      <Btn1Down>:       _motion_action_start(rotate)\n \
      <Btn2Down>:       _motion_action_start(translate)\n \
      <Btn3Down>:       _scale()\n \
      <Key>q:           _quit()";

   static XtActionsRec trans_actions[]  =
   {
      {"_repaint",             (XtActionProc)repaint},
      {"_resize",              (XtActionProc)resize},
      {"_motion_action",       (XtActionProc)motion_action},
      {"_motion_action_start", (XtActionProc)motion_action_start},
      {"_scale",               (XtActionProc)scale_action},
      {"_quit",                (XtActionProc)quit}
   };


   
   /* default parameters for widgets */
   static String default_resources[] =
   {
      (String)NULL
   };

   /* --------------------------------------------------------------------------------- */

   XInitThreads();
      
   /* --------------------------------------------------------------------------------- */

   int vogle_win1 = -1;
   int id_idleworkproc;
   /* on this separate thread, the Xt window ... */
   int    ac    = 0;
   char **av    = NULL;
   
   Display	*dpy;
   Window	win;

   Arg          wargs[5];
   Widget       shell, canvas;
   XtAppContext app_con;         /* application context */
   Widget       xpanel;
   Widget       quit_button, stop_button, step_button;
   Widget       ppm_button, gif_button, tiff_button, xbm_button, png_button, pps_button;
   Widget       film_button;

   XAW_WINVOGLE *xaw_window = malloc( sizeof(XAW_WINVOGLE) );

   WIDGET_USERDATA_STRUCT widgetUserData;

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

   /* FIM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "Film Off");
   XtSetArg(wargs[1], XtNfromHoriz, stop_button);
   XtSetArg(wargs[2], XtNhorizDistance, 100);
   film_button = XtCreateManagedWidget(" film ", toggleWidgetClass, xpanel, wargs, 3);

   /* PPM-OUTPUT */
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
	XtSetArg(wargs[2], XtNhorizDistance, pdata->window_size_x - 250);
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
   widgetUserData.queue                  = queue;
   widgetUserData.pdata                  = pdata;
   widgetUserData.xaw_win                = xaw_window;
   widgetUserData.xaw_win->buttonStopRun = &stop_button;
   widgetUserData.xaw_win->buttonStep    = &step_button;
   widgetUserData.xaw_win->buttonQuit    = &quit_button;
   widgetUserData.xaw_win->buttonFilm    = &film_button;
   
   /* set callback & callback data for XAW buttons --------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit       , &widgetUserData);
   XtAddCallback(stop_button, XtNcallback, do_stop       , &widgetUserData);
   XtAddCallback(step_button, XtNcallback, do_step       , &widgetUserData);
   XtAddCallback(film_button, XtNcallback, do_film       , &widgetUserData);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output , &widgetUserData);
   XtAddCallback(gif_button , XtNcallback, do_gif_output , &widgetUserData);
   XtAddCallback(tiff_button, XtNcallback, do_tiff_output, &widgetUserData);
   XtAddCallback(png_button , XtNcallback, do_png_output , &widgetUserData);
	XtAddCallback(xbm_button , XtNcallback, do_xbm_output , &widgetUserData);
   XtAddCallback(pps_button , XtNcallback, do_pps_output , &widgetUserData);
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

   if (vogle_win1 == -1)
   {
      vogle_win1 = vo_xt_window(dpy, win, SIZEX, SIZEY);
      xaw_window->vogle_win = vogle_win1;
      winset(xaw_window->vogle_win);
   }

   /* set a idle_workproc who will stop/run throught thread condition/signal */
   id_idleworkproc = XtAppAddWorkProc (app_con, (XtWorkProc)idle_workproc, &widgetUserData );
   /* set a idle_workproc who will be removed at quit/exit */
   widgetUserData.xaw_win->id_idleworkproc = id_idleworkproc;

   xawWidgetUserData = &widgetUserData;
   
   drawscene_init(pdata, NULL);

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
