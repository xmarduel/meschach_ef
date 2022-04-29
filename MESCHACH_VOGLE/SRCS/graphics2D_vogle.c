

/** \file graphics2D_vogle.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */


#ifdef HAVE_VOGLE
#include "vogle.h"         /* Librairie VOGLE  */
#endif

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics2D_vogle.h"


/* size for the mini-axes */
#define SIZE_ARROW 0.20
#define SIZE_AXE   1.50

/* place the mini-axes */
#define MINIAXES_X_TRANS -1.2
#define MINIAXES_Y_TRANS -3.8

/* scale the mini-axes */
#define MINIAXES_SCALE    0.1
/* scaling */
#define SCALE_FACTOR 0.1

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static VOGLE_SURFACE_DATA *pdata_vogle  =  NULL;


VOGLE_SURFACE_DATA * pdata_vogle_get_static(void)
{
   return pdata_vogle;
}

VOGLE_SURFACE_DATA * pdata_vogle_set_static(VOGLE_SURFACE_DATA *data)
{
   pdata_vogle = data;

   return pdata_vogle;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

void drawscene2D(const VOGLE_SURFACE_DATA *pdata)
{
   color(BLACK);
   clear();


   {  /* SURFACE  + BOUNBINGBOX(?) */
      pushmatrix();

      translate(-0.0, -2.0, 0.0); /* init in the center of the window */
      translate(pdata->view.TX,-pdata->view.TY,pdata->view.TZ);  /* move the object in the window */

      scale(pdata->view.SCAL,pdata->view.SCAL,pdata->view.SCAL);

      rotate( pdata->view.ROTX, 'x');
      rotate( pdata->view.ROTY, 'z');

      translate(-(pdata->view.BB_XMAX-pdata->view.BB_XMIN)/2,
                -(pdata->view.BB_YMAX-pdata->view.BB_YMIN)/2,
                 0.0); /* rotate relative to the middle of the object */

      callobj(pdata->SURFACE);

      if (pdata->show_boundingbox) callobj(pdata->BOUNDINGBOX);

      popmatrix();
   }

   if ( pdata->show_miniref )  /* MINIAXES */
   { 
      pushmatrix();

      translate(MINIAXES_X_TRANS, MINIAXES_Y_TRANS, 0.0);
      scale(MINIAXES_SCALE,MINIAXES_SCALE,MINIAXES_SCALE);

      rotate( pdata->view.ROTX, 'x');
      rotate( pdata->view.ROTY, 'z');

      callobj(pdata->MINIAXES);

      popmatrix();
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void setup_view(void * data)
{
   printf("setup_view...\n");
   
   /* set initial view */
   ortho(-1.5,1.5, -1.5,1.5, -5.0, 15.5);
   /*up(0,0,1);*/
   lookat(0.0, -3.0, 1.0, 0.0, 0.0, 0.0, 0.0);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void delete_objects(void * data)
{
   VOGLE_SURFACE_DATA* pdata = (VOGLE_SURFACE_DATA*) data;

   printf("delete_objects...\n");

   delobj(pdata->MINIAXES);
   delobj(pdata->SURFACE);
   delobj(pdata->BOUNDINGBOX);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void setup_objects(void * data)
{
   int  i;

   VOGLE_SURFACE_DATA* pdata = (VOGLE_SURFACE_DATA*) data;

   MAT * XYSOMM = pdata->XYSOMM;
   VEC * VALUES = pdata->VALUES;
   IMAT *NSELMT = pdata->NSELMT;

   /* for the bounding box */
   double BB_XMIN ; double BB_XMAX ;
   double BB_YMIN ; double BB_YMAX ;
   double BB_ZMIN ; double BB_ZMAX ;

   BB_XMIN =  1.0e10;
   BB_XMAX = -1.0e10;
   BB_YMIN =  1.0e10;
   BB_YMAX = -1.0e10;
   BB_ZMIN =  1.0e10;
   BB_ZMAX = -1.0e10;

   printf("setup_objects...\n");

   /*
    * Make a bounding box
    */
   makeobj(pdata->BOUNDINGBOX);
   {
      color(WHITE);

      for (i=0; i<XYSOMM->m; i++)
      {
         if ( BB_XMIN > XYSOMM->me[i][0] ) BB_XMIN =  XYSOMM->me[i][0] ;
         if ( BB_XMAX < XYSOMM->me[i][0] ) BB_XMAX =  XYSOMM->me[i][0] ;

         if ( BB_YMIN > XYSOMM->me[i][1] ) BB_YMIN =  XYSOMM->me[i][1] ;
         if ( BB_YMAX < XYSOMM->me[i][1] ) BB_YMAX =  XYSOMM->me[i][1] ;

         if ( BB_ZMIN > VALUES->ve[i] )    BB_ZMIN =  VALUES->ve[i] ;
         if ( BB_ZMAX < VALUES->ve[i] )    BB_ZMAX =  VALUES->ve[i] ;
      }

      move(BB_XMIN, BB_YMIN, BB_ZMIN);
      draw(BB_XMIN, BB_YMAX, BB_ZMIN);
      draw(BB_XMAX, BB_YMAX, BB_ZMIN);
      draw(BB_XMAX, BB_YMIN, BB_ZMIN);
      draw(BB_XMIN, BB_YMIN, BB_ZMIN);

      move(BB_XMIN, BB_YMIN, BB_ZMAX);
      draw(BB_XMIN, BB_YMAX, BB_ZMAX);
      draw(BB_XMAX, BB_YMAX, BB_ZMAX);
      draw(BB_XMAX, BB_YMIN, BB_ZMAX);
      draw(BB_XMIN, BB_YMIN, BB_ZMAX);

      move(BB_XMIN, BB_YMIN, BB_ZMIN);
      draw(BB_XMIN, BB_YMIN, BB_ZMAX);
      draw(BB_XMIN, BB_YMAX, BB_ZMAX);
      draw(BB_XMIN, BB_YMAX, BB_ZMIN);
      draw(BB_XMIN, BB_YMIN, BB_ZMIN);

      move(BB_XMAX, BB_YMIN, BB_ZMIN);
      draw(BB_XMAX, BB_YMIN, BB_ZMAX);
      draw(BB_XMAX, BB_YMAX, BB_ZMAX);
      draw(BB_XMAX, BB_YMAX, BB_ZMIN);
      draw(BB_XMAX, BB_YMIN, BB_ZMIN);
   }
   closeobj();

   pdata->view.BB_XMIN =  BB_XMIN;
   pdata->view.BB_XMAX =  BB_XMAX;
   pdata->view.BB_YMIN =  BB_XMIN;
   pdata->view.BB_YMAX =  BB_YMAX;
   pdata->view.BB_ZMIN =  BB_ZMIN;
   pdata->view.BB_ZMAX =  BB_ZMAX;


   /*
    * make the surface
    */
   makeobj(pdata->SURFACE);
   {
      color(pdata->surface_color);


      for (i=0; i<NSELMT->m; i++)
      {
         makepoly();

         move(XYSOMM->me[NSELMT->im[i][0]][0],XYSOMM->me[NSELMT->im[i][0]][1] , VALUES->ve[NSELMT->im[i][0]] );

         draw(XYSOMM->me[NSELMT->im[i][1]][0],XYSOMM->me[NSELMT->im[i][1]][1] , VALUES->ve[NSELMT->im[i][1]] );
         draw(XYSOMM->me[NSELMT->im[i][2]][0],XYSOMM->me[NSELMT->im[i][2]][1] , VALUES->ve[NSELMT->im[i][2]] );
         draw(XYSOMM->me[NSELMT->im[i][0]][0],XYSOMM->me[NSELMT->im[i][0]][1] , VALUES->ve[NSELMT->im[i][0]] );

         closepoly();
      }
   }
   closeobj();

   /*
    * Some Axis
    */
   makeobj(pdata->MINIAXES);
   {
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
   }
   closeobj();

   /* end */
   backface(0);
   backfacedir(1);
   polyfill(0);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOGLE_SURFACE_DATA *surfacedata_new()
{
   VOGLE_SURFACE_DATA * out = malloc( sizeof(VOGLE_SURFACE_DATA) );

   strncpy(out->title, "SOLUTION", 63);

   /**/
   out->VALUES = NULL;
   out->NSELMT = NULL;
   out->XYSOMM = NULL;
   /**/

   /* vogle data properties */
   out->surface_color = VOGLEe_COLOR_RED;
   out->labels_size   = 1.0;
   
   /* vogle motion properties */
   out->motion.MOTION_SCALE_FACTOR = SCALE_FACTOR;

   /* vogle view */
   out->view.SCAL = 1.0 + 3*SCALE_FACTOR;

   out->view.TX = 0.0;
   out->view.TY = 0.0;
   out->view.TZ = 0.0;

   out->view.ROTX =   0.0;
   out->view.ROTY = -30.0;

   /* for the bounding box */
   out->view.BB_XMIN =  1.0e10;
   out->view.BB_XMAX = -1.0e10;
   out->view.BB_YMIN =  1.0e10;
   out->view.BB_YMAX = -1.0e10;
   out->view.BB_ZMIN =  1.0e10;
   out->view.BB_ZMAX = -1.0e10;

   /* objects id */
   out->SURFACE     = 1;
   out->BOUNDINGBOX = 2;
   out->MINIAXES    = 3;

   /* flags */
   out->show_backface    = 0;
   out->show_boundingbox = 1;
   out->show_miniref     = 1;

   /* the drawing function */
   out->draw = drawscene2D;

   out->setup_view     = setup_view;
   out->setup_objects  = setup_objects;
   out->delete_objects = delete_objects;

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  *surfacedata_free (VOGLE_SURFACE_DATA *in)
{
   M_FREE(in->XYSOMM);
   IM_FREE(in->NSELMT);
   V_FREE(in->VALUES);

   return in;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOGLE_SURFACE_DATA *surfacedata_clone(VOGLE_SURFACE_DATA *in)
{
   VOGLE_SURFACE_DATA * out;

   if ( in == NULL )
   {
      return NULL;
   }
 
   out = malloc( sizeof(VOGLE_SURFACE_DATA) );


   strncpy(out->driver, in->driver, 63);

   out->window_size_x = in->window_size_x;
   out->window_size_y = in->window_size_y;

   strncpy(out->title, in->title, 63);
   
   /* the raw values */
   out->VALUES = v_copy (in->VALUES, NULL);
   out->NSELMT = im_copy(in->NSELMT, NULL);
   out->XYSOMM = m_copy (in->XYSOMM, NULL);
   /* the raw values */

   out->surface_color = in->surface_color;
   out->labels_size   = in->labels_size;

   /* vogle motion attr. */
   out->motion = in->motion ;

   /* vogle view attr. */
   out->view = in->view;

   /* vogle objects id */
   out->SURFACE     = in->SURFACE;
   out->BOUNDINGBOX = in->BOUNDINGBOX;
   out->MINIAXES    = in->MINIAXES;

   /* flags */
   out->show_backface     = in->show_backface;
   out->show_boundingbox  = in->show_boundingbox;
   out->show_miniref      = in->show_miniref;

   /* drawing function */
   out->draw = in->draw;

   out->setup_view     = in->setup_view;
   out->setup_objects  = in->setup_objects;
   out->delete_objects = in->delete_objects;

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void   graphics2D_vogle_initialize(char *driver, int window_size_x, int window_size_y)
{
   /* create a queue , set it in a static data  */
   SVQueue *queue = svqueue_new((CLONE_FUNC)surfacedata_clone, (FREED_FUNC)surfacedata_free);
	
   svqueue_vogle_set_static(queue);
	
   /* create the container for the data to plot , set it in a static data  */
   VOGLE_SURFACE_DATA *pdata = surfacedata_new();
	
   strncpy(pdata->driver, driver, 15);
   
   pdata->window_size_x  = window_size_x;
   pdata->window_size_y  = window_size_y;
	
   pdata_vogle_set_static(pdata);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

char *graphics2D_vogle_title(const char *title)
{
   VOGLE_SURFACE_DATA * pdata = pdata_vogle_get_static();

   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics2D_vogle_plotsurface_flush()
    */

   if ( title == NULL )
   {
      return pdata->title;
   }
   else
   {
      strncpy(pdata->title, title, 63);
      pdata->title[63] = '\0';
   }

   return NULL;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics2D_vogle_labelsize(double size)
{
   VOGLE_SURFACE_DATA * pdata = pdata_vogle_get_static();

   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics2D_vogle_plotsurface_flush()
    */

   pdata->labels_size = size;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics2D_vogle_store_plotdata(const VEC *vec)
{
   VOGLE_SURFACE_DATA * pdata = pdata_vogle_get_static();

   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics2D_vogle_plotsurface_flush()
    */

   if ( pdata->VALUES == NULL )
   {
      pdata->VALUES = v_copy(vec, NULL);
   }
   else
   {
      pdata->VALUES = v_copy(vec, pdata->VALUES);
   }

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics2D_vogle_store_surfacedata(const IMAT *NSELMT, const MAT *XYSOMM)
{
   VOGLE_SURFACE_DATA * pdata = pdata_vogle_get_static();

   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics2D_vogle_plotsurface_flush()
    */

   pdata->NSELMT = im_copy(NSELMT, NULL);
   pdata->XYSOMM = m_copy(XYSOMM, NULL);

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics2D_vogle_set_plotdata_in_queue(void)
{
   SVQueue            * queue = svqueue_vogle_get_static();
   VOGLE_SURFACE_DATA * pdata = pdata_vogle_get_static();
	
   /* set the data in the queue */
   queue->xset(queue, pdata);

   return;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


