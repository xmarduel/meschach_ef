

/** \file graphics2D_vogle_stationnary_xt_window.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include <pthread.h>

#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#endif

#ifdef HAVE_VOGLE
#include "vogle.h"         /* Librairie VOGLE  */
#include "voglex.h"        /* Librairie VOGLE  */
#endif

#include "MESCHACH_ADDS/INCLUDES/queue.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle_stationnary.h"

/* default size of the window */
#define SIZE	624

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void do_quit       (Widget w, XtPointer data, XtPointer data2);
static void do_back       (Widget w, XtPointer data, XtPointer data2);
static void do_boundingbox(Widget w, XtPointer data, XtPointer data2);
static void do_miniref    (Widget w, XtPointer data, XtPointer data2);

static void do_ppm_output (Widget w, XtPointer data, XtPointer data2);
static void do_gif_output (Widget w, XtPointer data, XtPointer data2);
static void do_png_output (Widget w, XtPointer data, XtPointer data2);
static void do_xbm_output (Widget w, XtPointer data, XtPointer data2);
static void do_pps_output (Widget w, XtPointer data, XtPointer data2);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct XAW_WINVOGLE_
{
   /* VOGLE window ID */
   int voglewin;

   /* pointer to widgets */
   Widget * canvas;
   Widget * buttonQuit;

   /* flags */
   int exit ;

} XAW_WINVOGLE ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINVOGLE       * xaw_win; /* pointer to window stuff */

   VOGLE_SURFACE_DATA * data; /* include the data to plot */

} WIDGET_USERDATA_STRUCT;


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static WIDGET_USERDATA_STRUCT *xawWidgetUserdata;

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

static void generic_output(const XAW_WINVOGLE *wxaw, const VOGLE_SURFACE_DATA *pdata, VOGLE_OUTPUT_FORMAT output_format)
{
   int w2;
   /*
    * type must be "pps" or "ppm" or "gif" or "tiff"
    */
   char string_output[64];
   char vogle_output_id[8];
   char *ext = "ppm ";

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

   snprintf(string_output, 63, "%s.%s", pdata->title, ext);

	/* in the script "p6to", we use the environment variables $P6TO and $VOGLE_OUTPUT_FILE */
   switch( output_format )
   {
		case VOGLEe_PPM:
		case VOGLEe_GIF:
		case VOGLEe_PPS :
			setenv("VOGLE_OUTPUT_FILE", string_output, 1);
			break;
		default:
			setenv("VOGLE_OUTPUT_FILE", pdata->title, 1); /* for p6to , see at the end of the file */
	}
	
   /**/
   fprintf(stderr, "Producing %s\n", string_output);
   voutput(string_output);
   w2 = winopen(vogle_output_id, string_output);
   winset(w2);
   drawscene(pdata);
   windel(w2);
   winset(wxaw->voglewin);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void quit(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   vexit();
   pthread_exit(NULL);
}

static void repaint(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   drawscene(xawWidgetUserdata->data);
}

static void resize(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   /* widget is the canvas */
   Dimension       w, h;

   XtVaGetValues (widget, XtNwidth, &w, NULL);
   XtVaGetValues (widget, XtNheight, &h, NULL);
   
   /* set vogle window */
   vo_xt_win_size(xawWidgetUserdata->xaw_win->voglewin, (int)w, (int)h);
   calcviewport();
}

static void motion_action(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   if (*n_params != 1)
   {
      XtError ("Wrong number of args!");
   }

   if ( strcmp(params[0], "rotate") == 0 )
   {
      float rx,ry;

      int DELTA_X = event->xmotion.x - xawWidgetUserdata->data->motion.MOTION_X_ROTATE;
      int DELTA_Y = event->xmotion.y - xawWidgetUserdata->data->motion.MOTION_Y_ROTATE;

      xawWidgetUserdata->data->motion.MOTION_X_ROTATE = event->xmotion.x;
      xawWidgetUserdata->data->motion.MOTION_Y_ROTATE = event->xmotion.y;

      rx = 2 * DELTA_X / (float)(SIZE);
      ry = 2 * DELTA_Y / (float)(SIZE);

      xawWidgetUserdata->data->view.ROTX  = fmod( xawWidgetUserdata->data->view.ROTX + 180*ry , 360 );
      xawWidgetUserdata->data->view.ROTY  = fmod( xawWidgetUserdata->data->view.ROTY + 180*rx , 360 );

      /* redraw */
      repaint(widget, event, params, n_params);
   }
   
   if ( strcmp(params[0], "translate") == 0 )
   {
      float rx,ry;

      int DELTA_X = event->xmotion.x - xawWidgetUserdata->data->motion.MOTION_X_TRANSLATE;
      int DELTA_Y = event->xmotion.y - xawWidgetUserdata->data->motion.MOTION_Y_TRANSLATE;

      xawWidgetUserdata->data->motion.MOTION_X_TRANSLATE = event->xmotion.x;
      xawWidgetUserdata->data->motion.MOTION_Y_TRANSLATE = event->xmotion.y;

      rx = 20 * DELTA_X / (float)(SIZE);
      ry = 20 * DELTA_Y / (float)(SIZE);
   
      xawWidgetUserdata->data->view.TX  += rx;
      xawWidgetUserdata->data->view.TY  += ry;
      xawWidgetUserdata->data->view.TZ   = 0.0;

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

   if ( strcmp(params[0], "rotate") == 0 )
   {
      xawWidgetUserdata->data->motion.MOTION_X_ROTATE = event->xmotion.x ;
      xawWidgetUserdata->data->motion.MOTION_Y_ROTATE = event->xmotion.y ;
   }
   if ( strcmp(params[0], "translate") == 0 )
   {
      xawWidgetUserdata->data->motion.MOTION_X_TRANSLATE = event->xmotion.x ;
      xawWidgetUserdata->data->motion.MOTION_Y_TRANSLATE = event->xmotion.y ;
   }
}

static void scale_action(Widget widget, XEvent *event, String *params, Cardinal *n_params)
{
   xawWidgetUserdata->data->view.SCAL *= ( event->xmotion.y > SIZE/2 ?
                                 1 - xawWidgetUserdata->data->motion.MOTION_SCALE_FACTOR :
                                 1 + xawWidgetUserdata->data->motion.MOTION_SCALE_FACTOR );

   /* redraw */
   repaint(widget, event, params, n_params);
}


/* some callback from athena widgets */
static void do_quit(Widget w, XtPointer data, XtPointer data2)
{
   vexit();
   pthread_exit(NULL);
}

static void do_back(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;

   xawWidgetUserdata->data->show_backface = !xawWidgetUserdata->data->show_backface;
   backface(xawWidgetUserdata->data->show_backface);

   /* redraw */
   repaint(*xawWidgetUserdata->xaw_win->canvas, NULL, NULL, NULL);
}

static void do_boundingbox(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;

   xawWidgetUserdata->data->show_boundingbox  = !xawWidgetUserdata->data->show_boundingbox;

   /* redraw */
   repaint(*xawWidgetUserdata->xaw_win->canvas, NULL, NULL, NULL);
}

static void do_miniref(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;

   xawWidgetUserdata->data->show_miniref  = !xawWidgetUserdata->data->show_miniref;

   /* redraw */
   repaint(*xawWidgetUserdata->xaw_win->canvas, NULL, NULL, NULL);
}

/* outputs */
static void do_ppm_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xawWidgetUserdata->xaw_win, xawWidgetUserdata->data, VOGLEe_PPM);
}

static void do_gif_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(xawWidgetUserdata->xaw_win, xawWidgetUserdata->data, VOGLEe_GIF);
}

static void do_png_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(xawWidgetUserdata->xaw_win, xawWidgetUserdata->data, VOGLEe_PNG);
}

static void do_xbm_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(xawWidgetUserdata->xaw_win, xawWidgetUserdata->data, VOGLEe_XBM);
}

static void do_tiff_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(xawWidgetUserdata->xaw_win, xawWidgetUserdata->data, VOGLEe_TIFF);
}

static void do_pps_output(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(xawWidgetUserdata->xaw_win, xawWidgetUserdata->data, VOGLEe_PPS);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * vogle_surface_with_xt_toolkit(void *data)
{
   /*--------------------------------------------------------------------------------------*/

   String trans =
      "<Expose>:        _repaint()\n  \
      <Configure>:      _resize()\n \
      Button1<Motion>:	_motion_action(rotate)\n \
      Button2<Motion>:	_motion_action(translate)\n \
      <Btn1Down>:       _motion_action_start(rotate)\n \
      <Btn2Down>:       _motion_action_start(translate)\n \
      <Btn3Down>:       _scale()\n \
      <Key>q:           _quit()";

   XtActionsRec actions[] =
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

   /*--------------------------------------------------------------------------------------*/
	
   SVQueue *queue = (SVQueue*)data;
   VOGLE_SURFACE_DATA* pdata = queue->xget(queue);

   XInitThreads();

   /*--------------------------------------------------------------------------------------*/

   Display		*dpy;
   Window		win;

   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;

   Arg		wargs[5];
   Widget	toplevel, xpanel;
   Widget	canvas;
   Widget	quit_button, backf_button, bbox_button, miniaxes_button;
   Widget	ppm_button, pps_button, gif_button, xbm_button, tiff_button, png_button;
  
   XtAppContext    app_con;         /* application context */

   XAW_WINVOGLE *xaw_window = malloc( sizeof(XAW_WINVOGLE) );

   WIDGET_USERDATA_STRUCT xxawWidgetUserdata;

   xaw_window->exit = 0;
   xaw_window->voglewin = -1;
   
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

   /* map actions to subroutines */
   XtAppAddActions (app_con, actions, XtNumber (actions));
   XtAddActions(actions, XtNumber(actions));


   xpanel = XtCreateManagedWidget("panel", formWidgetClass, toplevel, NULL, 0);

   /* Athena widgets */
   XtSetArg(wargs[0], XtNlabel, "Quit");
   quit_button = XtCreateManagedWidget("quit", commandWidgetClass, xpanel, wargs, 1);
   
   XtSetArg(wargs[0], XtNlabel, "Backface");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   backf_button = XtCreateManagedWidget("backface", commandWidgetClass, xpanel, wargs, 2);
   
   XtSetArg(wargs[0], XtNlabel, "Bounding Box");
   XtSetArg(wargs[1], XtNfromHoriz, backf_button);
   bbox_button = XtCreateManagedWidget("Bounding Box", commandWidgetClass, xpanel, wargs, 2);

   XtSetArg(wargs[0], XtNlabel, "Mini Axes");
   XtSetArg(wargs[1], XtNfromHoriz, bbox_button);
   miniaxes_button = XtCreateManagedWidget("Mini Axes", commandWidgetClass, xpanel, wargs, 2);

/*   
   XtSetArg(wargs[0], XtNlabel, "Cont. Zoom");
   XtSetArg(wargs[1], XtNfromHoriz, mbutton);
   czbutton = XtCreateManagedWidget("cont_zoom", commandWidgetClass, xpanel, wargs, 2);
   XtAddCallback(czbutton, XtNcallback, do_continious_zoom, pdata);

   XtSetArg(wargs[0], XtNlabel, "Cont. Rot");
   XtSetArg(wargs[1], XtNfromHoriz, czbutton);
   crbutton = XtCreateManagedWidget("cont_rot", commandWidgetClass, xpanel, wargs, 2);
   XtAddCallback(crbutton, XtNcallback, do_continious_rot, pdata);
   
   XtSetArg(wargs[0], XtNlabel, "Cont. Trans");
   XtSetArg(wargs[1], XtNfromHoriz, crbutton);
   ctbutton = XtCreateManagedWidget("cont_trans", commandWidgetClass, xpanel, wargs, 2);
   XtAddCallback(ctbutton, XtNcallback, do_continious_trans, cloned_data);
   */

   
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNfromHoriz, miniaxes_button);
   XtSetArg(wargs[2], XtNhorizDistance, SIZE - 500); 
   ppm_button = XtCreateManagedWidget("ppm", commandWidgetClass, xpanel, wargs, 3);

	XtSetArg(wargs[0], XtNlabel, "GIF");
   XtSetArg(wargs[1], XtNfromHoriz, ppm_button);
   gif_button = XtCreateManagedWidget("gif", commandWidgetClass, xpanel, wargs, 2);

   XtSetArg(wargs[0], XtNlabel, "TIFF");
   XtSetArg(wargs[1], XtNfromHoriz, gif_button);
   tiff_button = XtCreateManagedWidget("tiff", commandWidgetClass, xpanel, wargs, 2);

	XtSetArg(wargs[0], XtNlabel, "PNG");
   XtSetArg(wargs[1], XtNfromHoriz, tiff_button);
   png_button = XtCreateManagedWidget("png", commandWidgetClass, xpanel, wargs, 2);

	XtSetArg(wargs[0], XtNlabel, "XBM");
   XtSetArg(wargs[1], XtNfromHoriz, png_button);
   xbm_button = XtCreateManagedWidget("xbm", commandWidgetClass, xpanel, wargs, 2);

	XtSetArg(wargs[0], XtNlabel, "PS");
   XtSetArg(wargs[1], XtNfromHoriz, xbm_button);
   pps_button = XtCreateManagedWidget("pps", commandWidgetClass, xpanel, wargs, 2);

   /* user value for the callbacks */
   xxawWidgetUserdata.data                   = pdata;
   xxawWidgetUserdata.xaw_win                = xaw_window;
   xxawWidgetUserdata.xaw_win->buttonQuit    = &quit_button;
   xxawWidgetUserdata.xaw_win->canvas        = &canvas;

   /* ------- set callback functions & callback datas for XAW buttons --------------- */
   XtAddCallback(quit_button     , XtNcallback, do_quit        , &xxawWidgetUserdata);
   XtAddCallback(backf_button    , XtNcallback, do_back        , &xxawWidgetUserdata);
   XtAddCallback(bbox_button     , XtNcallback, do_boundingbox , &xxawWidgetUserdata);
   XtAddCallback(miniaxes_button , XtNcallback, do_miniref     , &xxawWidgetUserdata);
   XtAddCallback(ppm_button      , XtNcallback, do_ppm_output  , &xxawWidgetUserdata);
	XtAddCallback(gif_button      , XtNcallback, do_gif_output  , &xxawWidgetUserdata);
	XtAddCallback(png_button      , XtNcallback, do_png_output  , &xxawWidgetUserdata);
	XtAddCallback(tiff_button     , XtNcallback, do_tiff_output , &xxawWidgetUserdata);
	XtAddCallback(xbm_button      , XtNcallback, do_xbm_output  , &xxawWidgetUserdata);
   XtAddCallback(pps_button      , XtNcallback, do_pps_output  , &xxawWidgetUserdata);
   /* --------------------------------------------------------------------------------*/

   XtSetArg(wargs[0], XtNwidth, SIZE);
   XtSetArg(wargs[1], XtNheight, SIZE);
   XtSetArg(wargs[2], XtNfromVert, quit_button);
   XtSetArg(wargs[3], XtNtranslations, XtParseTranslationTable(trans));
   /*canvas = XmCreateDrawingArea (xpanel, "", wargs, (Cardinal)4);*/
   canvas = XtCreateManagedWidget("canvas", simpleWidgetClass, xpanel, wargs, 4);

   xawWidgetUserdata = &xxawWidgetUserdata;

   XtManageChild (canvas);

   XtRealizeWidget(toplevel);

   dpy = XtDisplay(canvas);
   win = XtWindow(canvas);

   if (xaw_window->voglewin == -1)
   {
      xaw_window->voglewin = vo_xt_window(dpy, XtWindow(canvas), SIZE, SIZE);
      winset(xaw_window->voglewin);
   }

   /* setup the initial view */
   pdata->setup_view(pdata);
   
   /* setup the objects that vogle really use to plot (need a valid voglewin) */
   pdata->setup_objects(pdata);

   /* event loop */
   XtAppMainLoop(app_con);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
