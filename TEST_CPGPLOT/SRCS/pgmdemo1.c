#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include <Xm/Xm.h>
#include <X11/Intrinsic.h>
#include <Xm/MainW.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Frame.h>
#include <Xm/LabelG.h>
#include <Xm/Label.h>
#include <Xm/SeparatoG.h>
#include <Xm/CascadeB.h>
#include <Xm/SelectioB.h>
#include <Xm/TextF.h>

#include <X11/cursorfont.h>

#include "XmPgplot.h"
#include "cpgplot.h"

/* Make the demo backwardly compatible with Motif 1.1 */

#if XmVersion <= 1001
#ifndef XmFONTLIST_DEFAULT_TAG
#define XmFONTLIST_DEFAULT_TAG XmSTRING_DEFAULT_CHARSET
#endif
#endif

/*
 * Make the demo backwardly compatible with older versions of X.
 */
#if XtSpecificationRelease <= 4
#define XtSetLanguageProc(a,b,c) (void)((a),(b),(c))
#endif

/*
 * Gray-scale images of multiple analytic 2D functions will be supported.
 * Each 2D function will be encapsulated in a C function of the
 * following type.
 */
#define IMAGE_FN(fn) float (fn)(float x)
/*
 * Define a macro for prototyping and defining XtCallbackProc functions.
 */
#define CALL_FN(fn) void (fn)(Widget w, XtPointer client_data, XtPointer call_data)
/*
 * List the prototypes of the available 2D-function functions.
 */
static IMAGE_FN(sin_fn);
static IMAGE_FN(cos_fn);
static IMAGE_FN(gaus_fn);
static IMAGE_FN(star_fn);

static CALL_FN(sin_callback);
static CALL_FN(cos_callback);
static CALL_FN(gaus_callback);
static CALL_FN(star_callback);

/* Set the number of points plotted per data */

enum {DATA_SIZE=500};
enum {DATA_SCALE=50};

/*
 * Declare the object type that will contain the context of the
 * image and data plots.
 */
typedef struct {
  Widget w_coord;     /* Coordinate-display label widget */
  Widget w_data;      /* The data-plot image widget */
  float *coords;      /* The data compilation array */
  float *data;        /* The data compilation array */
  int data_size;      /* The length of the data array */
  int data_scale;     /* The length of the data array */
  IMAGE_FN(*fn);      /* The function to be displayed */
  Widget w_top;       /* The top-level widget of the application */
  Cursor busy;        /* The cursor to display when un-responsive */
} Image;

/*
 * Image object contructor and destructor functions.
 */
static Image *new_Image(unsigned data_size, unsigned data_scale, IMAGE_FN(*fn), Widget parent, Widget w_top);
static Image *del_Image(Image *im);

/*
 * Slice display functions.
 */

static void display_data(Image *im, IMAGE_FN(*fn));
static void display_help(Image *im);


/*
 * The following structure is used to describe menu fields to
 * CreateOptionMenu() and CreatePulldownMenu().
 * Note that title options are denoted by setting callback=NULL,
 * and that separators are specified by setting label=NULL.
 */
typedef struct {
  char *label;              /* The MenuItem label text */
  XtCallbackProc callback;  /* Function to be called when field is selected */
} MenuItem;

static Widget CreateOptionMenu(char *name, char *label, Widget parent, int nopt, MenuItem *opts, XtPointer client_data);
static Widget CreatePulldownMenu(char *name, char *label, Widget parent, int nfield, MenuItem *fields, XtPointer client_data);
static Widget CreateMainMenuBar(Image *im, Widget w_main);

static CALL_FN(quit_callback);
static CALL_FN(help_callback);

static void report_cursor(Widget w, XtPointer context, XEvent *event, Boolean *call_next);

/*.......................................................................
 * A demo program showing an example of how to use PGPLOT with Motif.
 *
 * Output:
 *  return   int  0 - OK.
 *                1 - Error.
 */
int main(int argc, char *argv[])
{
  XtAppContext app;/* Application context returned by XtVaAppInitialize */
  Widget w_top;    /* The top-level widget of the application */
  Widget w_main;   /* The geometry management widget of the application */
  Widget w_work;   /* The work-area widget of the main window */
  Image *im;       /* Image object container */
/*
 * Initialize Motif.
 */
  XtSetLanguageProc(NULL, NULL, NULL);
  w_top = XtVaAppInitialize(&app, "ImageSlice", NULL, 0,
#if XtSpecificationRelease <= 4
			    (Cardinal *)
#endif
			    &argc, argv, NULL,
                            XmNinput,               True,
			    NULL);
/*
 * Use the standard Motif management widget layout.
 */
  w_main = XtVaCreateManagedWidget("main", xmMainWindowWidgetClass, w_top,
				   XmNscrollBarDisplayPolicy, XmAS_NEEDED,
				   XmNscrollingPolicy,        XmAUTOMATIC,
				   XmNheight, 500,
				   XmNwidth, 650,
				   NULL);
/*
 * Create a row-column widget to contain the two PGPLOT widgets and
 * an option-menu button.
 */
  w_work = XtVaCreateManagedWidget("work", xmRowColumnWidgetClass, w_main,
				   XmNorientation, XmVERTICAL,
				   XmNnumColumns, 1,
				   NULL);
/*
 * Install the row-column widget as the work area of the main window.
 */
  XtVaSetValues(w_main, XmNworkWindow, w_work, NULL);
/*
 * Create the two PGPLOT widgets and the image container object.
 */
  im = new_Image(DATA_SIZE, DATA_SCALE, cos_fn, w_work, w_top);
  if(!im)
    return 1;
/*
 * Create the application menu-bar.
 */
  {
    Widget w_bar = CreateMainMenuBar(im, w_main);
    XtManageChild(w_bar);
  };
/*
 * Display the widgets.
 */
  XtRealizeWidget(w_top);
/*
 * Open the widgets to PGPLOT.
 */
  if(cpgopen(xmp_device_name(im->w_data)) <= 0)
    return 1;
/*
 * Display the initial image.
 */
  display_data(im, cos_fn);
  
/*
 * Interact with the user.
 */
  XtAppMainLoop(app);
  return 0;
}

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
static Image *new_Image(unsigned data_size, unsigned data_scale, IMAGE_FN(*fn), Widget parent, Widget w_top)
{
  Image *im;       /* The pointer to the container to be returned */
  Widget w_frame;  /* A frame widget */
  int i;
/*
 * Check the arguments.
 */
  if(data_size < 2) {
    fprintf(stderr, "new_Image: Illegal data size requested.\n");
    return NULL;
  };
  if(!fn) {
    fprintf(stderr, "new_Image: NULL display function intercepted.\n");
    return NULL;
  };
/*
 * Allocate the container.
 */
  im = (Image *) malloc(sizeof(Image));
  if(!im) {
    fprintf(stderr, "new_Image: Insufficient memory.\n");
    return NULL;
  };
/*
 * Before attempting any operation that might fail, initialize the
 * Image container at least up to the point at which it can safely be
 * passed to del_Image().
 */
  im->w_coord = NULL;
  im->w_data = NULL;
  im->coords = NULL;
  im->data = NULL;
  im->data_size = data_size;
  im->data_scale = data_scale;
  im->fn = fn;
  im->busy = None;
  im->w_top = w_top;

  /*
   * Allocate an array to be used when constructing datas through the
   * displayed image.
   */
  im->coords = (float *) malloc(sizeof(float) * data_size);
  if(!im->coords) {
     fprintf(stderr, "new_Image: Insufficient memory.\n");
     return del_Image(im);
  };
  /*
   * Initialize the data array.
   */
  for(i=0; i<data_size; i++)
     im->coords[i] = (float)i / (float)data_scale ;

/*
 * Allocate an array to be used when constructing datas through the
 * displayed image.
 */
  im->data = (float *) malloc(sizeof(float) * data_size);
  if(!im->data) {
    fprintf(stderr, "new_Image: Insufficient memory.\n");
    return del_Image(im);
  };
/*
 * Initialize the data array.
 */
  for(i=0; i<data_size; i++)
    im->data[i] = 0.0f;
/*
 * Create a horizontal row-column widget in which to arrange the
 * coordinate-display labels.
 */
  w_frame = XtVaCreateManagedWidget("coord_row", xmRowColumnWidgetClass, parent,
				    XmNorientation, XmHORIZONTAL,
				    XmNnumColumns, 1,
				    NULL);
/*
 * Create two labels. The first will contain a prefix, and the second
 * will contain the coordinates.
 */
  {
    char *text = "World coordinates: ";
    XtVaCreateManagedWidget("clab",
			    xmLabelWidgetClass, w_frame,
			    XtVaTypedArg, XmNlabelString,
			    XmRString, text, strlen(text)+1,
			    NULL);
  };
  im->w_coord = XtVaCreateManagedWidget("coord",
					xmLabelWidgetClass, w_frame,
					XtVaTypedArg, XmNlabelString,
					XmRString, "", 1,
					NULL);
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
                                        XmNheight, 400,
                                        XmNwidth, 600,
                                        XmpNmaxColors, 16,
                                        XmpNshare, True,
                                        XmNtraversalOn, False,
                                        NULL);
/*
 * Register a motion-event callback such that the cursor position can
 * be reported in the im->w_coord label widget.
 */
  XtAddEventHandler(im->w_data, PointerMotionMask, False, report_cursor,
		    (XtPointer)im->w_coord);
/*
 * Create a pulldown menu of optional 2-D image functions.
 */
  {
    static MenuItem functions[] = {
      {"Display Functions", NULL},  /* Title */
      {NULL, NULL},                 /* Separator */
      {"cos(R)",                       cos_callback},
      {"sin(R)",                       sin_callback},
      {"exp(-R^2/2.0)",                gaus_callback},
      {"(1+sin(6A))exp(-R^2/10)",      star_callback},
    };
    Widget menu = CreateOptionMenu("functions", "Select a display function:",
				   parent,
				   sizeof(functions)/sizeof(functions[0]),
				   functions, (XtPointer) im);
    if(menu == NULL)
      return del_Image(im);
    XtManageChild(menu);
  };

/*
 * Get the standard X busy cursor.
 */
  im->busy = XCreateFontCursor(XtDisplay(im->w_top), XC_watch);
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
    if(im->w_coord)
      XtDestroyWidget(im->w_coord);
    if(im->w_data)
      XtDestroyWidget(im->w_data);
    if(im->busy != None)
      XFreeCursor(XtDisplay(im->w_top), im->busy);
    free(im);
  };
  return NULL;
}


/*.......................................................................
 * Display a new data in the data window.
 *
 * Input:
 *  im    Image *   The image context object.
 */
static void display_data(Image *im,IMAGE_FN(*fn))
{
  int i;


   im->fn = fn;

/*
 * Construct the data in im->data[] and keep a tally of the
 * range of data values seen.
 */


  for(i=0; i<im->data_size; i++)
  {
     im->data[i] = im->fn( im->coords[i] );
  };


/*
 * Set up the data axes.
 */
  cpgslct(xmp_device_id(im->w_data));
  cpgask(0);
  cpgpage();
  cpgbbuf();
  cpgsch(2.0f);
  cpgvstd();
  cpgswin(0.0f, (float)im->data_size/(float)im->data_scale , -4.0, 4.0);
  cpgbox("BCNST", 0.0f, 0, "BCNST", 0.0f, 0);
  cpglab("Radius", "Image value", "A 1D data through the image");
/*
 * Draw the data.
 */
  for(i=0; i<im->data_size; i++)
  {
    if(i==0)
      cpgmove(0.0f, im->data[0]);
    else
      cpgdraw(im->coords[i] , im->data[i]);
  };
  cpgebuf();
  return;
}

/*.......................................................................
 * Display usage instructions in the data window.
 *
 * Input:
 *  im     Image *   The image object.
 */
static void display_help(Image *im)
{
/*
 * Clear the data plot and replace it with instructional text.
 */
  cpgslct(xmp_device_id(im->w_data));
  cpgask(0);
  cpgpage();
  cpgsch(3.5f);
  cpgsvp(0.0, 1.0, 0.0, 1.0);
  cpgswin(0.0, 1.0, 0.0, 1.0);
  cpgmtxt("T", -2.0, 0.5, 0.5,
	  "To see a data through the image, move the");
  cpgmtxt("T", -3.0, 0.5, 0.5,
	  "mouse into the image display window and select");
  cpgmtxt("T", -4.0, 0.5, 0.5,
	  " the two end points of a line.");
}


/*.......................................................................
 * A exp(-(x^2+y^2)/20) function.
 *
 * Input:
 *  x,y     float   The coordinates to evaluate the function at.
 * Output:
 *  return  float   The function value at the specified coordinates.
 */
static IMAGE_FN(gaus_fn)
{
  return exp(-sqrt(x*x)/2.0f);
}

/*.......................................................................
 * Callback to select the gaus_fn() fucntion.
 */
static CALL_FN(gaus_callback)
{
  display_data((Image *) client_data, gaus_fn);
}

/*.......................................................................
 * A sin(angle) function.
 *
 * Input:
 *  x,y     float   The coordinates to evaluate the function at.
 * Output:
 *  return  float   The function value at the specified coordinates.
 */
static IMAGE_FN(sin_fn)
{
  return sin(x);
}

/*.......................................................................
 * Callback to select the sin_angle_fn() fucntion.
 */
static CALL_FN(sin_callback)
{
  display_data((Image *) client_data, sin_fn);
}

/*.......................................................................
 * A cos(radius) function.
 *
 * Input:
 *  x,y     float   The coordinates to evaluate the function at.
 * Output:
 *  return  float   The function value at the specified coordinates.
 */
static IMAGE_FN(cos_fn)
{
  return cos(x);
}

/*.......................................................................
 * Callback to select the cos_radius_fn() fucntion.
 */
static CALL_FN(cos_callback)
{
  display_data((Image *) client_data, cos_fn);
}

/*.......................................................................
 * A (1+sin(6*angle))*exp(-radius^2 / 100)function.
 *
 * Input:
 *  x,y     float   The coordinates to evaluate the function at.
 * Output:
 *  return  float   The function value at the specified coordinates.
 */
static IMAGE_FN(star_fn)
{
  return ( 1.0 + sin(x)) * exp(-(x*x)/10.0) ;
}

/*.......................................................................
 * Callback to select the star_fn() fucntion.
 */
static CALL_FN(star_callback)
{
  display_data((Image *) client_data, star_fn);
}

/*.......................................................................
 * Create an option menu.
 *
 * Input:
 *  name             char *  The name of the menu.
 *  label            char *  The instructive label to place to the left of
 *                           the option menu.
 *  parent         Widget    The widget in which to place the option menu.
 *  nopt              int    The number of option fields.
 *  opts         MenuItem *  An array of nopt option fields.
 *  client_data XtPointer    The client_data argument to be passed to each
 *                           callback function.
 * Output:
 *  return      Widget    The menu, or NULL on error.
 */
static Widget CreateOptionMenu(char *name, char *label, Widget parent,
			       int nopt, MenuItem *opts, XtPointer client_data)
{
  Widget w_menu;       /* The option menu to be returned */
  Widget w_pulldown;   /* The pulldown-menu of the option menu widget */
  int i;
/*
 * Check arguments.
 */
  if(nopt < 1 || !opts) {
    fprintf(stderr, "CreateOptionMenu: No options.\n");
    return NULL;
  };
/*
 * Create a pulldown menu.
 */
  w_pulldown = XmCreatePulldownMenu(parent, "pulldown", NULL, 0);
/*
 * Create the option-menu button to attach the menu to.
 */
  {
    XmString label_str = XmStringCreateSimple(label);
    Arg args[5];       /* Resource arguments for XmCreateOptionMenu() */
    int narg = 0;
    XtSetArg(args[narg], XmNsubMenuId, w_pulldown); narg++;
    XtSetArg(args[narg], XmNlabelString, label_str); narg++;
    w_menu = XmCreateOptionMenu(parent, name, args, narg);
    XmStringFree(label_str);
  };
/*
 * Install the option fields.
 */
  for(i=0; i<nopt; i++) {
    MenuItem *opt = opts + i;
    if(opt->label) {
/*
 * Add an option field.
 */
      if(opt->callback) {
	Widget widget = XtVaCreateManagedWidget(opt->label,
				   xmPushButtonWidgetClass, w_pulldown, NULL);
	XtAddCallback(widget, XmNactivateCallback, opt->callback, client_data);
/*
 * Add a title widget.
 */
      } else {
	XtVaCreateManagedWidget(opt->label,
				xmLabelGadgetClass, w_pulldown,
				XmNalignment, XmALIGNMENT_BEGINNING,
				NULL);
      };
/*
 * Add a separator widget.
 */
    } else {
      XtVaCreateManagedWidget("separator", xmSeparatorGadgetClass, w_pulldown,
			      NULL);
    };
  };
  return w_menu;
}

/*.......................................................................
 * Create a pulldown menu.
 *
 * Input:
 *  name             char *  The name of the menu.
 *  label            char *  The label for the menubar button.
 *  parent         Widget    The widget in which to place the pulldown menu.
 *  nfield            int    The number of menu fields.
 *  fields       MenuItem *  An array of nfield menu fields.
 *  client_data XtPointer    The client_data argument to be passed to each
 *                           callback function.
 * Output:
 *  return         Widget    The cascade button of the menu, or NULL on error.
 */
static Widget CreatePulldownMenu(char *name, char *label, Widget parent,
				 int nfield, MenuItem *fields,
				 XtPointer client_data)
{
  Widget w_pulldown; /* The pulldown-menu */
  Widget w_cascade;  /* The cascade button to be returned */
  int i;
/*
 * Check arguments.
 */
  if(nfield < 1 || !fields) {
    fprintf(stderr, "CreatePulldownMenu: No fields.\n");
    return NULL;
  };
/*
 * Create a pulldown menu.
 */
  w_pulldown = XmCreatePulldownMenu(parent, "pulldown", NULL, 0);
/*
 * Create the cascade button that invokes the menu.
 */
  {
    XmString label_str = XmStringCreateSimple(label);
    w_cascade = XtVaCreateManagedWidget(name,
			   xmCascadeButtonWidgetClass, parent,
                           XmNsubMenuId, w_pulldown,
                           XmNlabelString, label_str,
                           NULL);
    XmStringFree(label_str);
  }; 
/*
 * Install the menu fields.
 */
  for(i=0; i<nfield; i++) {
    MenuItem *field = fields + i;
    if(field->label) {
/*
 * Add a button widget.
 */
      if(field->callback) {
	Widget widget = XtVaCreateManagedWidget(field->label,
				   xmPushButtonWidgetClass, w_pulldown, NULL);
	XtAddCallback(widget, XmNactivateCallback, field->callback,
		      client_data);
/*
 * Add a title widget.
 */
      } else {
	XtVaCreateManagedWidget(field->label,
				xmLabelGadgetClass, w_pulldown,
				XmNalignment, XmALIGNMENT_BEGINNING,
				NULL);
      };
/*
 * Add a separator widget.
 */
    } else {
      XtVaCreateManagedWidget("separator", xmSeparatorGadgetClass, w_pulldown,
			      NULL);
    };
  };
  return w_cascade;
}

/*.......................................................................
 * Create the menu bar of the application.
 *
 * Input:
 *  im        Image * The image object of the application.
 *  w_main   Widget   The main widget of the application. 
 * Output:
 *  return   Widget   The menu-bar widget or NULL on error.
 */
static Widget CreateMainMenuBar(Image *im, Widget w_main)
{
  Widget w_bar;        /* The menu-bar widget */
/*
 * Create the menu bar with File and Help menus.
 */
  w_bar = XmCreateMenuBar(w_main, "menubar", NULL, 0);
/*
 * Install the File menu.
 */
  {
    static MenuItem file_fields[] = {
      {NULL, NULL},     /* Separator */
      {"Quit",          quit_callback}
    };
    CreatePulldownMenu("file_menu", "File", w_bar,
		       sizeof(file_fields)/sizeof(file_fields[0]),
		       file_fields, (XtPointer) im);
  };
/*
 * Install the Help menu.
 */
  {
    static MenuItem help_fields[] = {
      {"Usage", help_callback}
    };
    Widget w_help = CreatePulldownMenu("help_menu", "Help", w_bar,
		       sizeof(help_fields)/sizeof(help_fields[0]),
		       help_fields, (XtPointer) im);
    XtVaSetValues(w_bar, XmNmenuHelpWidget, w_help, NULL);
  };
  return w_bar;
}

/*.......................................................................
 * The file-menu "Quit" callback function.
 */
static CALL_FN(quit_callback)
{
  exit(0);
}

/*.......................................................................
 * The help-menu callback function.
 */
static CALL_FN(help_callback)
{
  Image *im = (Image *) client_data;
  display_help(im);
}


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
static void report_cursor(Widget w, XtPointer context, XEvent *event,
			  Boolean *call_next)
{
  Widget w_coord = (Widget) context;
  if(event->type == MotionNotify)
  {
    char text[80];
    float wx, wy;
/*
 * Convert from X-window coordinates to world coordinates.
 */
    if(xmp_pixel_to_world(w, event->xmotion.x, event->xmotion.y, &wx, &wy) == 0)
    {
      sprintf(text, "X=%-10g Y=%-10g", wx, wy);
      XtVaSetValues(w_coord,
		    XtVaTypedArg, XmNlabelString,
		    XmRString, text, strlen(text)+1,
		    NULL);
    };

    if ( ((10.0<wx)&&(wx<11.0)) && ((4.0<wy)&&(wy<4.5)) )
   {
       static int count = 0;
       printf("inside ! (%d)\n", count++);
   }
  };
  *call_next = True;

}

