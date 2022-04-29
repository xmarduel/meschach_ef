#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <X11/Xlib.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>
#include <X11/StringDefs.h>
#include <X11/Core.h>
#include <X11/cursorfont.h>

#include <Xm/Xm.h>
#include <Xm/MainW.h>
#include <Xm/Frame.h>

#include "XmPgplot.h"
#include "cpgplot.h"
#include "graphics_pgplot1D.h"

#define DATA_SIZE 500
#define DATA_SCALE 50


/*
 * Declare the object type that will contain the context of the
 * image and data plots.
 */
typedef struct {

  Widget w_data;      /* The data-plot image widget */
  int data_size;      /* The length of the data array */
  int data_scale;     /* The length of the data array */
  
  Widget w_top;       /* The top-level widget of the application */

} Image;

/*
 * Image object contructor and destructor functions.
 */
static Image *new_Image(unsigned data_size, unsigned data_scale, Widget parent, Widget w_top);
static Image *del_Image(Image *im);


/*.......................................................................
 * Allocate and return an initialized Image container object.
 * This function creates two PGPLOT widgets. One will be used to display
 * a gray-scale image. The other will be used to display a data through
 * the image.
 *
 * Note that the widgets are not opened to PGPLOT and nothing
 * will be displayed until display_fn() is first
 * called. These operations must be postponed until after the widgets have
 * been realized.
 *
 * Input:
 *  data_size unsigned    The dimension of the data array (>=2).
 *  data_scale unsigned   The dimension of the data array (>=2).
 *  fn         IMAGE_FN(*) The initial display function.
 *  parent       Widget    The widget in which to create the PGPLOT widgets.
 *  w_top        Widget    The top-level widget of the application.
 * Output:
 *  return   Image *  The new image container, or NULL on error.
 */
static Image *new_Image(unsigned data_size, unsigned data_scale, Widget parent, Widget w_top)
{
  Image *im;       /* The pointer to the container to be returned */
  Widget w_frame;  /* A frame widget */
  
/*
 * Allocate the container.
 */
  im = (Image *) malloc(sizeof(Image));

  im->data_size  = data_size;
  im->data_scale = data_scale;
  
  im->w_top = w_top;
  
  /*
   * Create an etched-in frame widget to provide a border for the
   * data-plot window.
   */
  w_frame = XtVaCreateManagedWidget("data_frame", xmFrameWidgetClass,
                                    parent,
                                    XmNshadowType, XmSHADOW_IN,
                                    NULL);
  /*
   * Create the data-display widget.
   */
  im->w_data = XtVaCreateManagedWidget("data", xmPgplotWidgetClass,
                                        w_frame,
                                        XmNheight, 500,
                                        XmNwidth, 500,
                                        XmpNmaxColors, 16,
                                        XmpNshare, True,
                                        XmNtraversalOn, False,
                                        NULL);


  return im;
}

/*.......................................................................
* Delete an Image container previously returned by new_Image().
*
* Input:
*  im        Image *  The container to be deleted (or NULL).
* Output:
*  return    Image *  The deleted container (always NULL).
*/
Image *del_Image(Image *im)
{
   if(im)
   {
      if(im->w_data)
      {
         XtDestroyWidget(im->w_data);
      }
      free(im);
   }
   
   return NULL;
}


void draw_curve (Widget w)
{
   int i;
   static int j=0;
   
   float x[DATA_SIZE];
   float y[DATA_SIZE];

   j += 2;

   for(i=0; i<DATA_SIZE; i++)
   {
      x[i] = i / (float)DATA_SCALE;
      y[i] = cos(x[i] - (j) / (float)DATA_SCALE);
   }


   /*
    * Set up the data axes.
    */
   /*if ( j != 0 ) cpgslct(xmp_device_id(w));*/
   

   /*
   cpgask(0);
   cpgpage();
   */
   
   cpgbbuf();
   cpgeras();
   /*cpgsci(color);*/
   cpgsch(2.0);
   cpgvstd();
   cpgswin(0.0f, DATA_SIZE/DATA_SCALE , -4.0, 4.0);
   cpgbox("BCNST", 0.0f, 0, "BCNST", 0.0, 0);
   cpglab("x", "y", "A 1D curve");
   
   /*
    * Draw the data.
    */
   for(i=0; i<DATA_SIZE; i++)
   {
      if(i==0)
         cpgmove(0.0, y[0]);
      else
         cpgdraw(x[i] , y[i]);
   }
   cpgebuf();
}

void Display_data (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   draw_curve(w);
}

void Toggle (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   draw_curve(w);
}

void ToggleLoop (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   static XtWorkProcId id;
   static int go = 0;

   go = (go ? 0 : 1);
   printf("go = %d \n", go);

   if ( go==1 )
   {
      /* get the app */
      XtAppContext theApp = XtWidgetToApplicationContext(w);

      id = XtAppAddWorkProc (theApp, (XtWorkProc)draw_curve,(XtPointer)(NULL) );

      /*XtAppAddTimeOut( theApp, 1000 , NULL, NULL );*/
   }
   else
   {
      XtRemoveWorkProc( id );
   }

}

void Quit (Widget w, XEvent *ev, String *params, Cardinal *n_params)
{
   exit (0);
}

/* mapping of events to actions */
static const String translations =
"<Expose>:      display_data()\n\
<Btn1Down>:     toggle()\n\
<Btn3Down>:     loop()\n\
<Key>q:         quit()";

/* mapping of actions to subroutines */
static XtActionsRec actions[] =
{
   {"display_data",      Display_data},
   {"toggle",            Toggle},
   {"loop",              ToggleLoop},
   {"quit",              Quit}
};

/* default parameters for widgets */
static String default_resources[] =
{
   "Example*geometry:      550x550",
   (String)NULL
};

int main (int argc, char *argv[])
{
   Arg wargs[10];                /* storage of widget args */
   Widget w_top;                 /* toplevel widget; child */
   Widget w_main;                /* The geometry management widget of the application */
   XtAppContext app_con;         /* application context */

   Image *im;
   

   /* create toplevel shell widget */
   w_top = XtAppInitialize (&app_con,
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
   /*
    * Use the standard Motif management widget layout.
    */
   /*w_main = XtCreateManagedWidget ((String)"", coreWidgetClass, w_top, wargs, (Cardinal)2);*/

   w_main = XtVaCreateManagedWidget("main", xmMainWindowWidgetClass, w_top,
                                    XmNscrollBarDisplayPolicy, XmAS_NEEDED,
                                    XmNscrollingPolicy,        XmAUTOMATIC,
                                    XmNheight, 550,
                                    XmNwidth, 550,
                                    NULL);

   /*
    * Display the widgets.
    */
   XtRealizeWidget(w_top);

   /* initialize GNU libplot */
   /*
    * Create the two PGPLOT widgets and the image container object.
    */
   im = new_Image(DATA_SIZE, DATA_SCALE, w_main, w_top);

   XtSetArg (wargs[0], XtNtranslations, XtParseTranslationTable (translations));
   XtSetValues (im->w_data, wargs, (Cardinal)1);

   /*
    * Open the widgets to PGPLOT.
    */
   if(cpgopen(xmp_device_name(im->w_data)) <= 0)
      return 1;
   /*
    * Display the initial image.
    */
   draw_curve(im->w_data);

   /* transfer control to X Toolkit event loop (doesn't return) */
   XtAppMainLoop (app_con);

   return 1;
}
