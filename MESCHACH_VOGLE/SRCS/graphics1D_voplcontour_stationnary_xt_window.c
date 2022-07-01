

/** \file graphics1D_voplcontour_stationnary_xt_window.c
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
#include "vopl.h"         /* Librairie VOPL            */
#endif

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_stationnary.h"

/* default size of the window */
#define SIZE	624

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static int TYPE_OF_THREAD = VOPLe_THREAD_POSIX; /* the default */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_voplcontour_set_thread_type(int type)
{
   switch(type)
   {
      case VOPLe_THREAD_PYTHON: TYPE_OF_THREAD = type;  break;
      case VOPLe_THREAD_POSIX : TYPE_OF_THREAD = type;  break;
      default                   : return EXIT_FAILURE;
   }
	
   return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void do_quit       (Widget w, XtPointer data, XtPointer data2);
static void do_border     (Widget w, XtPointer data, XtPointer data2);
static void do_mesh       (Widget w, XtPointer data, XtPointer data2);
static void do_contours   (Widget w, XtPointer data, XtPointer data2);

static void do_ppm_output (Widget w, XtPointer data, XtPointer data2);
static void do_gif_output (Widget w, XtPointer data, XtPointer data2);
static void do_png_output (Widget w, XtPointer data, XtPointer data2);
static void do_tif_output (Widget w, XtPointer data, XtPointer data2);
static void do_xbm_output (Widget w, XtPointer data, XtPointer data2);
static void do_pps_output (Widget w, XtPointer data, XtPointer data2);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

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
	
   VOPL_GRAPH_DATA  * data; /* include the data to plot */
	
} WIDGET_USERDATA_STRUCT;


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static WIDGET_USERDATA_STRUCT *xawWidgetUserdata;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static void drawscene_init(VOPL_GRAPH_DATA* pdata, Widget w)
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

static void generic_output(const XAW_WINVOGLE *wxaw, const VOPL_GRAPH_DATA *pdata, VOGLE_OUTPUT_FORMAT output_format)
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
   drawscene_init(pdata, NULL);
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

/* some callback from athena widgets */
static void do_quit(Widget w, XtPointer data, XtPointer data2)
{
   vexit();
   pthread_exit(NULL);
}


static void do_mesh(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
	VOPL_CONTOUR_DATA* pcontourdata = (VOPL_CONTOUR_DATA*)(xawWidgetUserdata->data->dataplot[0]);
	
   pcontourdata->show_mesh  = !pcontourdata->show_mesh;
	
   /* redraw */
   repaint(*xawWidgetUserdata->xaw_win->canvas, NULL, NULL, NULL);
}

static void do_border(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
   VOPL_CONTOUR_DATA* pcontourdata = (VOPL_CONTOUR_DATA*)(xawWidgetUserdata->data->dataplot[0]);
	
	pcontourdata->show_border  = !pcontourdata->show_border;
	
   /* redraw */
   repaint(*xawWidgetUserdata->xaw_win->canvas, NULL, NULL, NULL);
}

static void do_contours(Widget w, XtPointer data, XtPointer data2)
{
   WIDGET_USERDATA_STRUCT *xawWidgetUserdata = (WIDGET_USERDATA_STRUCT *)data;
	
   VOPL_CONTOUR_DATA* pcontourdata = (VOPL_CONTOUR_DATA*)(xawWidgetUserdata->data->dataplot[0]);
	
	pcontourdata->show_contours  = !pcontourdata->show_contours;
	
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

static void do_tif_output(Widget w, XtPointer data, XtPointer data2)
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

void * vopl_contour_with_xt_toolkit(void *data)
{
   /*--------------------------------------------------------------------------------------*/
	
   String trans =
	"<Expose>:        _repaint()\n  \
	<Configure>:      _resize()\n \
	<Key>q:           _quit()";
	
   XtActionsRec actions[] =
   {
      {"_repaint",             (XtActionProc)repaint},
      {"_resize",              (XtActionProc)resize},
      {"_quit",                (XtActionProc)quit}
   };
	
   /* default parameters for widgets */
   static String default_resources[] =
   {
      (String)NULL
   };
	
   /*--------------------------------------------------------------------------------------*/
	
   SVQueue *queue = (SVQueue*)data;
   VOPL_GRAPH_DATA* pdata = queue->xget(queue);
	
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
   Widget	quit_button, mesh_button, border_button, contours_button;
   Widget	ppm_button, pps_button, gif_button, xbm_button, tiff_button, png_button;
	
   XtAppContext    app_con;         /* application context */
	
   XAW_WINVOGLE *xaw_window = malloc( sizeof(XAW_WINVOGLE) );
	
	WIDGET_USERDATA_STRUCT xxawWidgetUserdata;
	
	xawWidgetUserdata = &xxawWidgetUserdata;
	
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
   
   XtSetArg(wargs[0], XtNlabel, "Mesh");
   XtSetArg(wargs[1], XtNfromHoriz, quit_button);
   mesh_button = XtCreateManagedWidget("mesh", commandWidgetClass, xpanel, wargs, 2);
   
   XtSetArg(wargs[0], XtNlabel, "Border");
   XtSetArg(wargs[1], XtNfromHoriz, mesh_button);
   border_button = XtCreateManagedWidget("border", commandWidgetClass, xpanel, wargs, 2);
	
   XtSetArg(wargs[0], XtNlabel, "Contours");
   XtSetArg(wargs[1], XtNfromHoriz, border_button);
   contours_button = XtCreateManagedWidget("contours", commandWidgetClass, xpanel, wargs, 2);
	
   
   XtSetArg(wargs[0], XtNlabel, "PPM");
   XtSetArg(wargs[1], XtNfromHoriz, contours_button);
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
   xawWidgetUserdata->data                   = vopl_graphdata_clone(pdata);
   xawWidgetUserdata->xaw_win                = xaw_window;
   xawWidgetUserdata->xaw_win->buttonQuit    = &quit_button;
   xawWidgetUserdata->xaw_win->canvas        = &canvas;
	
   /* ------- set callback functions & callback datas for XAW buttons --------------- */
   XtAddCallback(quit_button     , XtNcallback, do_quit        , xawWidgetUserdata);
   XtAddCallback(mesh_button     , XtNcallback, do_mesh        , xawWidgetUserdata);
   XtAddCallback(border_button   , XtNcallback, do_border      , xawWidgetUserdata);
   XtAddCallback(contours_button , XtNcallback, do_contours    , xawWidgetUserdata);
   XtAddCallback(ppm_button      , XtNcallback, do_ppm_output  , xawWidgetUserdata);
	XtAddCallback(gif_button      , XtNcallback, do_gif_output  , xawWidgetUserdata);
	XtAddCallback(png_button      , XtNcallback, do_png_output  , xawWidgetUserdata);
	XtAddCallback(tiff_button     , XtNcallback, do_tif_output  , xawWidgetUserdata);
	XtAddCallback(xbm_button      , XtNcallback, do_xbm_output  , xawWidgetUserdata);
   XtAddCallback(pps_button      , XtNcallback, do_pps_output  , xawWidgetUserdata);
   /* --------------------------------------------------------------------------------*/
	
   XtSetArg(wargs[0], XtNwidth, SIZE);
   XtSetArg(wargs[1], XtNheight, SIZE);
   XtSetArg(wargs[2], XtNfromVert, quit_button);
   XtSetArg(wargs[3], XtNtranslations, XtParseTranslationTable(trans));
   /*canvas = XmCreateDrawingArea (xpanel, "", wargs, (Cardinal)4);*/
   canvas = XtCreateManagedWidget("canvas", simpleWidgetClass, xpanel, wargs, 4);
	
   XtManageChild (canvas);
	
   XtRealizeWidget(toplevel);
	
   dpy = XtDisplay(canvas);
   win = XtWindow(canvas);
	
   if (xaw_window->voglewin == -1)
   {
      xaw_window->voglewin = vo_xt_window(dpy, XtWindow(canvas), SIZE, SIZE);
      vopl_wininit(xaw_window->voglewin);
   }
   
   /* setup the objects that vogle really use to plot (need a valid voglewin) */
   VOPL_CONTOUR_DATA * pcontourdata = (VOPL_CONTOUR_DATA*)pdata->dataplot[0];
	pcontourdata->setup_objects(pcontourdata);
	
   /* event loop */
   XtAppMainLoop(app_con);
	
   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


