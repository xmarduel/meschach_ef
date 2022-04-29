

/** \file graphics2D_vogle.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <pthread.h>

#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#endif

#ifdef HAVE_VOGLE
#include "vogle.h"         /* Librairie VOGLE      */
#include "voglex.h"        /* Librairie VOGLE      */
#endif

#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"

/* default size of the window */
#define SIZE	512

Widget	canvas;

/* vogle objects id */
#define SURFACE     1
#define BOUNDINGBOX 2
#define MINIAXES    3

/* size for the mini-axes */
#define SIZE_ARROW 0.20
#define SIZE_AXE   1.5

/* place the mini-axes */
#define MINIAXES_X_TRANS -1.2
#define MINIAXES_Y_TRANS -3.8
/* scale the mini-axes */
#define MINIAXES_SCALE   0.1

/* for the bounding box */
float XMIN =  1.0e10;
float XMAX = -1.0e10;
float YMIN =  1.0e10;
float YMAX = -1.0e10;
float ZMIN =  1.0e10;
float ZMAX = -1.0e10;


int	voglewin1   = -1;

int	back        = 0;
int	doubleb     = 1;
int	fill        = 0;
int	hatch       = 0;
int     boundingbox = 1;
int     miniref     = 1;

#define SCALE_FACTOR      0.1

float   SCAL = 1.0 + 3*SCALE_FACTOR;

float TX = 0.0;
float TY = 0.0;
float TZ = 0.0;

float ROTX =   0.0;
float ROTY = -30.0;


double MOTION_SCAL = 1.0;

int MOTION_X_ROTATE;
int MOTION_Y_ROTATE;

int MOTION_X_TRANSLATE;
int MOTION_Y_TRANSLATE;

int CONTINIOUS_ZOOM      = 0;
int CONTINIOUS_ROTATE    = 0;
int CONTINIOUS_TRANSLATE = 0;

#define ESC 27



/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void* vogle_surface_with_xt_toolkit(void *pdata);

static void setup_objects(const VEC *VALUES, const IMAT *NSELMT, const MAT* XYSOMM );

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct _VOGLE_SURFACE_DATA
{
   int thread_id; /* for debugging */

   const VEC  *VALUES;
   const IMAT *NSELMT;
   const MAT  *XYSOMM;

   int color;

} VOGLE_SURFACE_DATA;

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static void  init_threaded_data(VOGLE_SURFACE_DATA *data,
                                int   color          ,
                                const VEC *VALUES    ,
                                const IMAT *NSELMT   ,
                                const MAT *XYSOMM    )
{
   data->thread_id = 1;

   data->color = color;

   data->VALUES = VALUES; /* only a reference ? */
   data->NSELMT = NSELMT; /* only a reference ? */
   data->XYSOMM = XYSOMM; /* only a reference ? */ /* duplicate the data in the thread */
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics2D_vogle_plotsurface( const VEC *VALUES    ,
                                   const IMAT *NSELMT   ,
                                   const MAT *XYSOMM    )

{
   /* in a new thread */
   pthread_attr_t     attr;
   pthread_t          thread;
   VOGLE_SURFACE_DATA data;

   int rc,status;

   init_threaded_data(&data, VOGLEe_COLOR_RED, VALUES, NSELMT, XYSOMM);

   /* Initialize and set thread detached attribute */
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
   
   /* Create and run the thread */
   rc = pthread_create(&thread, NULL/* &attr */, vogle_surface_with_xt_toolkit , &data);
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc); exit(-1);
   }

   /* let the new thread time to copy in is own thread the datas it will utilize */
   sleep(1); /* 1s. should it be enought ?? */ /* many be buggy */

   /* we certainly do not wait for the end of the 'vogle# thread ... */
   /*
   rc = pthread_join(thread, (void **)&status);
   if (rc)
   {
      printf("ERROR; return code from pthread_join() is %d\n", rc); exit(-1);
   }
   */

   /* Free attribute */
   pthread_attr_destroy(&attr);

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void drawscene()
{
   color(BLACK);
   clear();


   {  /* SURFACE  + BOUNBINGBOX */
      pushmatrix();

      translate(-0.0, -2.0, 0.0); /* init in the center of the window */
      translate(TX,-TY,TZ);  /* move the object in the window */

      scale(SCAL,SCAL,SCAL);

      rotate( ROTX, 'x');
      rotate( ROTY, 'z');

      translate(-(XMAX-XMIN)/2, -(YMAX-YMIN)/2, 0.0); /* rotate relative to the middle of the object */

      callobj(SURFACE);

      if (boundingbox) callobj(BOUNDINGBOX);

      popmatrix();
   }

   {  /* MINIAXES */
      pushmatrix();

      translate(MINIAXES_X_TRANS, MINIAXES_Y_TRANS, 0.0);
      scale(MINIAXES_SCALE,MINIAXES_SCALE,MINIAXES_SCALE);

      rotate( ROTX, 'x');
      rotate( ROTY, 'z');

      if ( miniref ) callobj(MINIAXES);

      popmatrix();
   }
      
   if (doubleb)
   {
      swapbuffers();
   }
}

void repaint()
{
   winset(voglewin1);
   drawscene();
}

void resize()
{
   Dimension       w, h;
   Arg             arg[2];

   XtSetArg(arg[0], XtNwidth, &w);
   XtSetArg(arg[1], XtNheight, &h);
   XtGetValues(canvas, arg, 2);

   vo_xt_win_size(voglewin1, (int)w, (int)h);
   XtGetValues(canvas, arg, 2);
   calcviewport();
}

void do_rotate(Widget w, XEvent *event)
{
   float rx,ry;

   if ( CONTINIOUS_ROTATE == 0 )
   {
      int DELTA_X = event->xmotion.x - MOTION_X_ROTATE;
      int DELTA_Y = event->xmotion.y - MOTION_Y_ROTATE;

      MOTION_X_ROTATE = event->xmotion.x;
      MOTION_Y_ROTATE = event->xmotion.y;

      rx = 2 * DELTA_X / (float)(SIZE);
      ry = 2 * DELTA_Y / (float)(SIZE);

      ROTX  = fmod( ROTX + 180*ry , 360 );
      ROTY  = fmod( ROTY + 180*rx , 360 );
   }
   else
   {
   }
   
   repaint();
}


void do_translate(Widget w, XEvent *event)
{
   float rx,ry;

   if ( CONTINIOUS_TRANSLATE == 0 )
   {
      int DELTA_X = event->xmotion.x - MOTION_X_TRANSLATE;
      int DELTA_Y = event->xmotion.y - MOTION_Y_TRANSLATE;

      MOTION_X_TRANSLATE = event->xmotion.x;
      MOTION_Y_TRANSLATE = event->xmotion.y;

      rx = 20 * DELTA_X / (float)(SIZE);
      ry = 20 * DELTA_Y / (float)(SIZE);

      TX  += rx;
      TY  += ry;
      TZ   = 0.0;
   }
   else
   {
   }
   
   repaint();
}


static XtWorkProcId idlerun_id;

void do_idle_scale(Widget w, XEvent *event)
{
   SCAL *= MOTION_SCAL;
   printf("do_idle_scale : SCAL = %lf, MOTION_SCAL = %lf\n", SCAL,MOTION_SCAL);
   repaint();
}


void do_scale(Widget w, XEvent *event)
{
   if ( CONTINIOUS_ZOOM )
   {
      printf("event->xmotion.y = %d --> MOTION_SCAL = %lf (SCALE_FACTOR=%lf)\n", event->xmotion.y, MOTION_SCAL,SCALE_FACTOR);

      MOTION_SCAL = ( event->xmotion.y > SIZE/2 ?
                        1-SCALE_FACTOR*fabs(((float)(event->xmotion.y-SIZE/2))/SIZE) :
                        1+SCALE_FACTOR*fabs(((float)(event->xmotion.y-SIZE/2))/SIZE) );

      printf("event->xmotion.y = %d --> MOTION_SCAL = %lf (SCALE_FACTOR=%lf)\n", event->xmotion.y, MOTION_SCAL,SCALE_FACTOR);
   }
}


void do_start_rotation(Widget w, XEvent *event)
{
   MOTION_X_ROTATE = event->xmotion.x ;
   MOTION_Y_ROTATE = event->xmotion.y ;
}

void do_start_translation(Widget w, XEvent *event)
{
   MOTION_X_TRANSLATE = event->xmotion.x ;
   MOTION_Y_TRANSLATE = event->xmotion.y ;
}

void do_start_scale(Widget w, XEvent *event)
{
   if ( CONTINIOUS_ZOOM == 0 )
   {
      SCAL *= ( event->xmotion.y > SIZE/2 ? 1-SCALE_FACTOR : 1+SCALE_FACTOR );

      repaint();
   }
   else
   {
      /* get the app */
      XtAppContext theApp = XtWidgetToApplicationContext(w);

      /* set default background routine */
      idlerun_id = XtAppAddWorkProc (theApp, (XtWorkProc)do_idle_scale,(XtPointer)(NULL) );

      printf("enable  CONTINIOUS_ZOOM idle function when stating draging ...(idle_run_id = %d)\n",idlerun_id);
   }
}


void do_end_rotation()
{
   /* and remove default background routine */
   if (CONTINIOUS_ROTATE)
   {
      XtRemoveWorkProc( idlerun_id );
   }
}

void do_end_translation()
{
   /* and remove default background routine */
   if (CONTINIOUS_TRANSLATE)
   {
      XtRemoveWorkProc( idlerun_id );
   }
}

void do_end_scale()
{
   if (CONTINIOUS_ZOOM)
   {
      MOTION_SCAL = 1.0;
   }

   /* and remove default background routine */
   if (CONTINIOUS_ZOOM)
   {
      XtRemoveWorkProc( idlerun_id );
      printf("end  CONTINIOUS_ZOOM idle function\n");
   }
}


void do_continious_zoom()
{
   CONTINIOUS_ZOOM  = !CONTINIOUS_ZOOM;
   fprintf(stdout, "CONTINIOUS_ZOOM = %d\n",CONTINIOUS_ZOOM);
}

void do_continious_rot()
{
   CONTINIOUS_ROTATE  = !CONTINIOUS_ROTATE;
}

void do_continious_trans()
{
   CONTINIOUS_TRANSLATE  = !CONTINIOUS_TRANSLATE;
}


void do_back()
{
   back = !back;
   backface(back);
   repaint();
}

void do_fill()
{
   fill = !fill;
   hatch = 0;

   polyfill(fill);
   repaint();
}

void do_wireframe()
{
   fill  = 0;
   hatch = 0;

   polyfill(fill);
   repaint();
}

void do_boundingbox()
{
   boundingbox  = !boundingbox;

   repaint();
}

void do_miniref()
{
   miniref  = !miniref;

   repaint();
}


void do_double()
{
   doubleb = !doubleb;

   if (doubleb)
   {
      fprintf(stderr, "Double buffer on\n");
      winset(voglewin1);
      backbuffer();
   }
   else
   {
      winset(voglewin1);
      frontbuffer();
   }
}

void quit()
{
   vexit();
   pthread_exit(NULL);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

String trans =
"<Expose>:	 _repaint()\n \
<Configure>:	 _resize()\n \
Button1<Motion>: _do_rotate()\n \
Button2<Motion>: _do_translate()\n \
Button3<Motion>: _do_scale()\n \
<Btn1Down>:      _do_start_rotat()\n \
<Btn2Down>:      _do_start_trans()\n \
<Btn3Down>:      _do_start_scale()\n \
<Btn1Up>:        _do_end_rotat()\n \
<Btn2Up>:        _do_end_trans()\n \
<Btn3Up>:        _do_end_scale()\n \
<Key>s:          _do_solid()\n \
<Key>w:          _do_wireframe()\n \
<Key>q: 	 _quit()";

XtActionsRec actions[] =
{
   {"_repaint",        (XtActionProc)repaint},
   {"_resize", 	       (XtActionProc)resize},
   {"_do_rotate",      (XtActionProc)do_rotate},
   {"_do_translate",   (XtActionProc)do_translate},
   {"_do_scale",       (XtActionProc)do_scale},
   {"_do_start_rotat", (XtActionProc)do_start_rotation},
   {"_do_start_trans", (XtActionProc)do_start_translation},
   {"_do_start_scale", (XtActionProc)do_start_scale},
   {"_do_end_rotat",   (XtActionProc)do_end_rotation},
   {"_do_end_trans",   (XtActionProc)do_end_translation},
   {"_do_end_scale",   (XtActionProc)do_end_scale},
   {"_do_solid",       (XtActionProc)do_fill},
   {"_do_wireframe",   (XtActionProc)do_wireframe},
   {"_quit",           (XtActionProc)quit}
};

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Display		*dpy;
Window		win;
GC		gc;

/* default parameters for widgets */
static String default_resources[] =
{
   (String)NULL
};

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void* vogle_surface_with_xt_toolkit(void *pdata)
{
   VOGLE_SURFACE_DATA *data = (VOGLE_SURFACE_DATA*)pdata;

   /* duplicate the datas as soon as possible, so that his thread owns these datas */
   VEC  *VALUES = v_copy (data->VALUES, NULL);
   IMAT *NSELMT = im_copy(data->NSELMT, NULL);
   MAT  *XYSOMM = m_copy (data->XYSOMM, NULL); 
   /* duplicate the datas */
   
   int ac    = 0;
   char **av = NULL;

   Arg		wargs[5];
   Widget	toplevel, panel, qbutton, bbutton, kbutton, mbutton, czbutton, crbutton, ctbutton;

   XtTranslations	trans_table;
   XtAppContext app_con;         /* application context */

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


   panel = XtCreateManagedWidget("panel", formWidgetClass, toplevel, NULL, 0);

   /* Athena widgets */
   /*
   XtSetArg(wargs[0], XtNlabel, "Quit");
   qbutton = XtCreateManagedWidget("quit", commandWidgetClass, panel, wargs, 1);
   XtAddCallback(qbutton, XtNcallback, quit, NULL);
   */
   
   XtSetArg(wargs[0], XtNlabel, "Backface");
   /*XtSetArg(wargs[1], XtNfromHoriz, qbutton);*/
   bbutton = XtCreateManagedWidget("backface", commandWidgetClass, panel, wargs, 1);
   XtAddCallback(bbutton, XtNcallback, do_back, NULL);

   XtSetArg(wargs[0], XtNlabel, "Bounding Box");
   XtSetArg(wargs[1], XtNfromHoriz, bbutton);
   kbutton = XtCreateManagedWidget("Bounding Box", commandWidgetClass, panel, wargs, 2);
   XtAddCallback(kbutton, XtNcallback, do_boundingbox, NULL);

   XtSetArg(wargs[0], XtNlabel, "Mini Axes");
   XtSetArg(wargs[1], XtNfromHoriz, kbutton);
   mbutton = XtCreateManagedWidget("Mini Axes", commandWidgetClass, panel, wargs, 2);
   XtAddCallback(mbutton, XtNcallback, do_miniref, NULL);

   XtSetArg(wargs[0], XtNlabel, "Cont. Zoom");
   XtSetArg(wargs[1], XtNfromHoriz, mbutton);
   czbutton = XtCreateManagedWidget("cont_zoom", commandWidgetClass, panel, wargs, 2);
   XtAddCallback(czbutton, XtNcallback, do_continious_zoom, NULL);

   XtSetArg(wargs[0], XtNlabel, "Cont. Rot");
   XtSetArg(wargs[1], XtNfromHoriz, czbutton);
   crbutton = XtCreateManagedWidget("cont_rot", commandWidgetClass, panel, wargs, 2);
   XtAddCallback(crbutton, XtNcallback, do_continious_rot, NULL);

   XtSetArg(wargs[0], XtNlabel, "Cont. Trans");
   XtSetArg(wargs[1], XtNfromHoriz, crbutton);
   ctbutton = XtCreateManagedWidget("cont_trans", commandWidgetClass, panel, wargs, 2);
   XtAddCallback(ctbutton, XtNcallback, do_continious_trans, NULL);

   
   XtSetArg(wargs[0], XtNwidth, 512);
   XtSetArg(wargs[1], XtNheight, 512);
   XtSetArg(wargs[2], XtNfromVert, bbutton); /**/
   canvas = XtCreateManagedWidget("canvas", simpleWidgetClass, panel, wargs, 3);


   XtAddActions(actions, XtNumber(actions));
   trans_table = XtParseTranslationTable(trans);
   XtAugmentTranslations(canvas, trans_table);

   XtRealizeWidget(toplevel);


   dpy = XtDisplay(canvas);
   win = XtWindow(canvas);

   if (voglewin1 == -1)
   {
      voglewin1 = vo_xt_window(dpy, XtWindow(canvas), SIZE, SIZE);
      backbuffer();
   }

   setup_objects(VALUES, NSELMT, XYSOMM);

   winset(voglewin1);
   drawscene();

   XtAppMainLoop(app_con);

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static void setup_objects(const VEC *VALUES, const IMAT *NSELMT, const MAT* XYSOMM )
{
   int	i;

   backbuffer();

   ortho(-1.5,1.5, -1.5,1.5, -5.0, 15.5);
   /*up(0,0,1);*/
   lookat(0.0, -3.0, 1.0, 0.0, 0.0, 0.0, 0.0);

   /*
    * Make a bounding box
    */
   makeobj(BOUNDINGBOX);
   color(WHITE);

   for (i=0; i<XYSOMM->m; i++)
   {
      if ( XMIN > XYSOMM->me[i][0] ) XMIN =  XYSOMM->me[i][0] ;
      if ( XMAX < XYSOMM->me[i][0] ) XMAX =  XYSOMM->me[i][0] ;

      if ( YMIN > XYSOMM->me[i][1] ) YMIN =  XYSOMM->me[i][1] ;
      if ( YMAX < XYSOMM->me[i][1] ) YMAX =  XYSOMM->me[i][1] ;

      if ( ZMIN > VALUES->ve[i] ) ZMIN =  VALUES->ve[i] ;
      if ( ZMAX < VALUES->ve[i] ) ZMAX =  VALUES->ve[i] ;
   }

   move(XMIN, YMIN, ZMIN);
   draw(XMIN, YMAX, ZMIN);
   draw(XMAX, YMAX, ZMIN);
   draw(XMAX, YMIN, ZMIN);
   draw(XMIN, YMIN, ZMIN);

   move(XMIN, YMIN, ZMAX);
   draw(XMIN, YMAX, ZMAX);
   draw(XMAX, YMAX, ZMAX);
   draw(XMAX, YMIN, ZMAX);
   draw(XMIN, YMIN, ZMAX);

   move(XMIN, YMIN, ZMIN);
   draw(XMIN, YMIN, ZMAX);
   draw(XMIN, YMAX, ZMAX);
   draw(XMIN, YMAX, ZMIN);
   draw(XMIN, YMIN, ZMIN);

   move(XMAX, YMIN, ZMIN);
   draw(XMAX, YMIN, ZMAX);
   draw(XMAX, YMAX, ZMAX);
   draw(XMAX, YMAX, ZMIN);
   draw(XMAX, YMIN, ZMIN);

   closeobj();

   /*
    * make the surface
    */
   makeobj(SURFACE);
   color(RED);


   for (i=0; i<NSELMT->m; i++)
   {
      makepoly();

      move(XYSOMM->me[NSELMT->im[i][0]][0],XYSOMM->me[NSELMT->im[i][0]][1] , VALUES->ve[NSELMT->im[i][0]] );

      draw(XYSOMM->me[NSELMT->im[i][1]][0],XYSOMM->me[NSELMT->im[i][1]][1] , VALUES->ve[NSELMT->im[i][1]] );
      draw(XYSOMM->me[NSELMT->im[i][2]][0],XYSOMM->me[NSELMT->im[i][2]][1] , VALUES->ve[NSELMT->im[i][2]] );
      draw(XYSOMM->me[NSELMT->im[i][0]][0],XYSOMM->me[NSELMT->im[i][0]][1] , VALUES->ve[NSELMT->im[i][0]] );

      closepoly();
   }
   closeobj();

   /*
    * Some Axis
    */
   makeobj(MINIAXES);

   textsize(1.0,1.0);

   color(YELLOW);
   move(0.0,0.0,0.0);
   draw(SIZE_AXE, 0.0,0.0);
   draw(SIZE_AXE-SIZE_ARROW, 0.0, -SIZE_ARROW);
   move(SIZE_AXE, 0.0,0.0);
   draw(SIZE_AXE-SIZE_ARROW, 0.0,  SIZE_ARROW);

   color(WHITE);
   move(SIZE_AXE, 0.0,0.0);
   drawchar('x');

   color(YELLOW);
   move(0.0,0.0,0.0);
   draw(0.0, SIZE_AXE, 0.0);
   draw(0.0, SIZE_AXE-SIZE_ARROW, -SIZE_ARROW);
   move(0.0, SIZE_AXE, 0.0);
   draw(0.0, SIZE_AXE-SIZE_ARROW,  SIZE_ARROW);

   color(WHITE);
   move(0.0, SIZE_AXE, 0.0);
   drawchar('y');

   color(GREEN);
   move(0.0,0.0,0.0);
   draw(0.0, 0.0, SIZE_AXE);
   draw(-SIZE_ARROW, 0.0, SIZE_AXE-SIZE_ARROW);
   move(0.0, 0.0, SIZE_AXE);
   draw( SIZE_ARROW, 0.0, SIZE_AXE-SIZE_ARROW);

   color(WHITE);
   move(0.0, 0.0, SIZE_AXE);
   drawchar('z');

   closeobj();



   backface(0);
   backfacedir(1);

   polyfill(0);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
