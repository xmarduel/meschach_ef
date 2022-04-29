

/** \file graphics1D_vogle_stationnary_xt_window.c
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
#include "vogle.h"        /* Librairie VOGLE           */
#include "voglex.h"       /* Librairie VOGLE + X11     */
#include "vopl.h"         /* Librairie VOPL            */
#endif

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_stationnary.h"

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static int TYPE_OF_THREAD = VOPLe_THREAD_POSIX; /* the default */

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

int  graphics1D_voplcurve_set_thread_type(int type)
{
   switch(type)
   {
      case VOPLe_THREAD_PYTHON: TYPE_OF_THREAD = type; break;
      case VOPLe_THREAD_POSIX : TYPE_OF_THREAD = type; break;
      default                 : return EXIT_FAILURE;
   }
	
   return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

typedef struct XAW_WINVOGLE_
{
   /* VOGLE window ID */
   int vogle_win;

   /* pointer to widgets */
   Widget * buttonPPS;
   Widget * buttonPPM;
   Widget * buttonQuit;
   
} XAW_WINVOGLE ;


typedef struct WIDGET_USERDATA_STRUCT_
{
   XAW_WINVOGLE    * xaw_win; /* pointer to window stuff */

   VOPL_GRAPH_DATA * pdata; /* include the data to plot */

} WIDGET_USERDATA_STRUCT;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

WIDGET_USERDATA_STRUCT * widget_data; /* the static data containing the plotdata */

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/* callbacks for "X11" */
static void quit       (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void repaint    (Widget w, XEvent *ev, String *params, Cardinal *n_params);
static void resize     (Widget w, XEvent *ev, String *params, Cardinal *n_params);

/* callbacks for "Xaw" */
static void do_quit        (Widget w, XtPointer data, XtPointer call_data);

static void do_ppm_output  (Widget w, XtPointer data, XtPointer call_data);
static void do_pps_output  (Widget w, XtPointer data, XtPointer call_data);
static void do_gif_output  (Widget w, XtPointer data, XtPointer call_data);
static void do_tiff_output (Widget w, XtPointer data, XtPointer call_data);
static void do_xbm_output  (Widget w, XtPointer data, XtPointer call_data);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

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

static void vogle_curve_hardcopy(const VOPL_GRAPH_DATA *pdata)
{
   int w2;
   /*
    * output_format must be "VOGLEe_PPM" or "VOGLEe_GIF" or "VOGLEe_TIFF" or "VOGLEe_XBM" or or "VOGLEe_PNG or "VOGLEe_PPS"
    */
   char string_output[64];
   char vogle_output_id[8];
   char *type = "ppm";

   VOGLE_OUTPUT_FORMAT output_format;

   if      ( strcmp(pdata->driver,"/PPM" ) == 0 ) output_format = VOGLEe_PPM;
   else if ( strcmp(pdata->driver,"/GIF" ) == 0 ) output_format = VOGLEe_GIF;
   else if ( strcmp(pdata->driver,"/TIFF") == 0 ) output_format = VOGLEe_TIFF;
   else if ( strcmp(pdata->driver,"/XBM" ) == 0 ) output_format = VOGLEe_XBM;
	else if ( strcmp(pdata->driver,"/PNG" ) == 0 ) output_format = VOGLEe_PNG;
   else if ( strcmp(pdata->driver,"/PPS" ) == 0 ) output_format = VOGLEe_PPS;
   else
   {
      fprintf(stderr, "Bad driver: %s\n", pdata->driver);
      return;
   }

   switch( output_format )
   {
      case VOGLEe_PPM :
         strncpy(vogle_output_id, "ppm", 4); type = "ppm";
         break;

      case VOGLEe_GIF :
         strncpy(vogle_output_id, "gif", 4); type = "gif";
         break;

      case VOGLEe_TIFF:
         strncpy(vogle_output_id, "p6", 4); type = "tiff"; setenv("P6TO", "TIFF", 1); /* for p6to script */
         break;

      case VOGLEe_XBM:
         strncpy(vogle_output_id, "p6", 4); type = "xbm" ; setenv("P6TO", "XBM", 1); /* for p6to script */
         break;
		
		case VOGLEe_PNG:
         strncpy(vogle_output_id, "p6", 4); type = "png" ; setenv("P6TO", "PNG", 1); /* for p6to script */
         break;

      case VOGLEe_PPS :
         strncpy(vogle_output_id, "pps",4); type = "pps";
         break;

      default:
         return;
   }

   snprintf(string_output, 63, "%s.%s", pdata->title, type);

   /* in the script "p6to", we use the environment. var $VOGLE_OUTPUT_FILE */
   setenv("VOGLE_OUTPUT_FILE", pdata->title, 1); /* for p6to , see at the end of the file */

   /**/
   fprintf(stderr, "Producing output on file %s\n", string_output);
   /*voutput(string_output);*/
	
   prefsize(pdata->window_size_x, pdata->window_size_y);
   w2 = vopl_winopen(vogle_output_id, string_output);
   drawscene_init(pdata, NULL);
   drawscene(pdata);
   vopl_windel(w2); /* draw on del */
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void generic_output(const WIDGET_USERDATA_STRUCT *widget_data, VOGLE_OUTPUT_FORMAT output_format)
{
   int w2;
   /*
    * type must be "pps" or "ppm" or "gif" or "tiff"
    */
   char string_output[64];
   char vogle_output_id[8];
   char *ext = "ppm";
	
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
   
   w2 = vopl_winopen(vogle_output_id, string_output);
   vopl_winset(w2);
   drawscene(widget_data->pdata);
   vopl_windel(w2);  /* draw on del */

   vopl_winset(widget_data->xaw_win->vogle_win);
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void generic_quit(void)
{
   fprintf(stderr, "quit...\n");

   switch(TYPE_OF_THREAD)
   {
      case VOPLe_THREAD_PYTHON: exit(0);            break;
      case VOPLe_THREAD_POSIX : exit(0); /*pthread_exit(NULL); */ break;
      default                 : /* nix */           break;
   }
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void quit(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   vexit();
   generic_quit();
}

static void repaint(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   drawscene(widget_data->pdata);
}

static void resize(Widget widget, XEvent *ev, String *params, Cardinal *n_params)
{
   Dimension       w, h;

   XtVaGetValues (widget, XtNwidth , &w, NULL);
   XtVaGetValues (widget, XtNheight, &h, NULL);

   vo_xt_win_size(widget_data->xaw_win->vogle_win, (int)w, (int)h);
   calcviewport();

   drawscene(widget_data->pdata);
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void do_quit(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   vexit();
   generic_quit();
}

static void do_ppm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(widget_data, VOGLEe_PPM);
}

static void do_gif_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(widget_data, VOGLEe_GIF);
}

static void do_tiff_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(widget_data, VOGLEe_TIFF);
}

static void do_xbm_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(widget_data, VOGLEe_XBM);
}

static void do_png_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;
	
   generic_output(widget_data, VOGLEe_PNG);
}

static void do_pps_output(Widget w, XtPointer data, XtPointer call_data) /* Xaw callback */
{
   WIDGET_USERDATA_STRUCT *widget_data = (WIDGET_USERDATA_STRUCT *)data;

   generic_output(widget_data, VOGLEe_PPS);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * vopl_curve_with_xt_toolkit(void* data)
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

   int vogle_win = -1;

   /* on this separate thread, the Xt window ... */
   int ac    = 0;
   char **av = NULL;

   Display   *dpy;
   Window    win;

   Arg		 wargs[5];
   Widget    shell, canvas;
   Widget	 xpanel;
   Widget    quit_button;
   Widget    pps_button, ppm_button, gif_button, tiff_button, xbm_button, png_button;

   XtAppContext app_con;   /* application context */

   /*-----------------------------------------------------------------------------------*/

   XAW_WINVOGLE *xaw_window = malloc( sizeof(XAW_WINVOGLE) );

   WIDGET_USERDATA_STRUCT xawWidgetUserdata;

   /*-----------------------------------------------------------------------------------*/

   if ( strcmp(pdata->driver,"/XWINDOW") != 0 )
   {
	  /* no X11 */
      vogle_curve_hardcopy(pdata);

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
   xawWidgetUserdata.pdata                  = pdata;
   xawWidgetUserdata.xaw_win                = xaw_window;
   xawWidgetUserdata.xaw_win->buttonQuit    = &quit_button;
   xawWidgetUserdata.xaw_win->vogle_win     = vogle_win;

   /* set callback & callback data for XAW buttons ---------------------------*/
   XtAddCallback(quit_button, XtNcallback, do_quit        , &xawWidgetUserdata);
   XtAddCallback(ppm_button , XtNcallback, do_ppm_output  , &xawWidgetUserdata);
   XtAddCallback(gif_button , XtNcallback, do_gif_output  , &xawWidgetUserdata);
   XtAddCallback(tiff_button, XtNcallback, do_tiff_output , &xawWidgetUserdata);
   XtAddCallback(png_button , XtNcallback, do_png_output  , &xawWidgetUserdata);
   XtAddCallback(xbm_button , XtNcallback, do_xbm_output  , &xawWidgetUserdata);
   XtAddCallback(pps_button , XtNcallback, do_pps_output  , &xawWidgetUserdata);
   /* ------------------------------------------------------------------------*/

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
      drawscene(pdata);
   }

   widget_data = &xawWidgetUserdata; /* assign static data pointer */

   /* the event-loop */
   XtAppMainLoop(app_con);

   /* end of the routine ... */
   pthread_exit(NULL);
   
   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

