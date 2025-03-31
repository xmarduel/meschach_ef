

/** \file graphics1D_vogle.c
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include <pthread.h>

#ifdef HAVE_VOGLE
#include "vogle.h"        /* Librairie VOGLE   */
#include "vopl.h"         /* Librairie VOPL    */
#endif

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle_svqueue.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle.h"
#include "MESCHACH_VOGLE/INCLUDES/graphics1D_vogle_stationnary.h"

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static VOPL_GRAPH_DATA *pdata_vopl  =  NULL;


VOPL_GRAPH_DATA * pdata_vopl_get_static(void)
{
   return pdata_vopl;
}

VOPL_GRAPH_DATA * pdata_vopl_set_static(VOPL_GRAPH_DATA *data)
{
   pdata_vopl = data;

   return pdata_vopl;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_CURVE_DATA *curvedata_new(void)
{
   VOPL_CURVE_DATA *pdata = malloc ( sizeof(VOPL_CURVE_DATA) );

	pdata->plot_type = VOPL_CURVE;

   pdata->ix = -1; /* to be set later */
	pdata->iy = -1; /* to be set later */
	
	pdata->xmin =  0.0;
	pdata->xmax =  1.0;
	pdata->ymin = -1.0;
	pdata->ymax =  1.0;
	
	strcpy(pdata->legend, "solution");
	
	pdata->labels_size  = 1.5;
	pdata->font         = VOGLEe_FONT_FUTURA_L;
	
	pdata->draw         = drawcurve_vopl1D; /* default drawing */
   pdata->set_drawfunc = set_drawfunc_voplcurve; /* to set a new drawing function */
   
	/* plot datas */
	pdata->x = NULL;
	pdata->y = NULL;
	/* plot attributes */
	pdata->color        = VOGLEe_COLOR_RED;
	pdata->withbox      = 1;
	pdata->adjustscale  = 1;

	pdata->marker       = VOGLEe_NO_MARKER;
	pdata->end_marker   = VOGLEe_NO_MARKER;

   return pdata;
}

void *curvedata_free (VOPL_CURVE_DATA *in)
{
   if (in)
   {
      V_FREE(in->x);
      V_FREE(in->y);

      free(in);
		in = NULL;
   }
   
   return NULL;
}

VOPL_CURVE_DATA *curvedata_clone(VOPL_CURVE_DATA *in)
{
   VOPL_CURVE_DATA *out = curvedata_new();

   out->ix = in->ix;
   out->iy = in->iy;

   out->x = v_copy(in->x, out->x);
   out->y = v_copy(in->y, out->y);

   out->labels_size  = in->labels_size;
   out->font         = in->font;
   
   out->color  = in->color;
   
	out->marker = in->marker;
	out->end_marker = in->end_marker;

   out->adjustscale = in->adjustscale;

   out->xmin = in->xmin; /* window */
   out->xmax = in->xmax; /* window */

   out->ymin = in->ymin; /* window */
   out->ymax = in->ymax; /* window */

   out->withbox = in->withbox;

   strncpy(out->legend, in->legend, 63);

   out->draw         = in->draw; /* default drawing */
   out->set_drawfunc = in->set_drawfunc; /* to set a new drawing function */

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void set_drawfunc_voplcurve(VOPL_CURVE_DATA* pdata, VOPL_CURVE_DRAW_FUNC drawfunc)
{
   pdata->draw = drawfunc; /* set the function who draws */
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_CONTOUR_DATA *contourdata_new(void)
{
   VOPL_CONTOUR_DATA * pdata = malloc( sizeof(VOPL_CONTOUR_DATA) );
	
   pdata->plot_type = VOPL_CONTOURS;
	
   pdata->ix = -1; /* to be set later */
   pdata->iy = -1; /* to be set later */
	
	/* window - must be set from the config file */
   pdata->xmin   = 0.0;
   pdata->xmax   = 1.0;
   pdata->ymin   = 0.0;
   pdata->ymax   = 1.0;
	
   strcpy(pdata->legend, "solution");
	
   pdata->labels_size  = 1.5;
   pdata->font         = VOGLEe_FONT_FUTURA_L;
	
   /* the drawing function */
   pdata->draw = drawcontours_vopl1D;
    pdata->set_drawfunc = (void (*)(VOPL_CONTOUR_DATA*, void (*)(VOPL_CONTOUR_DATA *)))set_drawfunc_voplcontours; /* to set a new drawing function */
	
   /* plot datas */
   pdata->VALUES = NULL;
   pdata->NSELMT = NULL;
   pdata->XYSOMM = NULL;
	
   /* plot attributes */
   pdata->mesh_color  = VOGLEe_COLOR_RED;
   pdata->border_color= VOGLEe_COLOR_WHITE;
	
   /* objects id */
   pdata->MESH     = 1;
   pdata->BORDER   = 2;
   pdata->CONTOURS = 3;
	
   /* flags*/
   pdata->show_mesh     = 0;
   pdata->show_border   = 1;
   pdata->show_contours = 1;
	
	/* nb levels*/
	pdata->nb_levels = 10;
	pdata->levels = NULL;
	
	/* governs which object to delete */
	pdata->delete_border_object_flag = 1;
	pdata->delete_mesh_object_flag = 1;
	pdata->delete_contours_object_flag = 1;
	
	/* governs which object to build */
	pdata->build_border_object_flag = 1;
	pdata->build_mesh_object_flag = 1;
	pdata->build_contours_object_flag = 1;
	
	pdata->ucdmesh = NULL;

   pdata->setup_objects  = (VOPL_CONTOUR_DRAW_FUNC)setup_voplcontour_objects;
   pdata->delete_objects = (VOPL_CONTOUR_DRAW_FUNC)delete_voplcontour_objects;
	
   /* finito */
   return pdata;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void  *contourdata_free (VOPL_CONTOUR_DATA *in)
{
   M_FREE(in->XYSOMM);
   IM_FREE(in->NSELMT);
   V_FREE(in->VALUES);
	
	V_FREE(in->levels);
	
   return in;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_CONTOUR_DATA *contourdata_clone(VOPL_CONTOUR_DATA *in)
{
   VOPL_CONTOUR_DATA * out;
	
   if ( in == NULL )
   {
      return NULL;
   }
	
   out = malloc( sizeof(VOPL_CONTOUR_DATA) );
	
	out->plot_type = in->plot_type;
	
	out->ix = in->ix;
   out->iy = in->iy;
	
	/* window */
   out->xmin     = in->xmin;
   out->xmax     = in->xmax;
   out->ymin     = in->ymin;
	out->ymax     = in->ymax;
	
	out->labels_size  = in->labels_size;
   out->font         = in->font;
	
	strncpy(out->legend, in->legend, 63);

	/* drawing function */
   out->draw = in->draw;
	out->set_drawfunc = in->set_drawfunc;
   
   /* the raw values */
   out->VALUES = v_copy (in->VALUES, NULL);
   out->NSELMT = im_copy(in->NSELMT, NULL);
   out->XYSOMM = m_copy (in->XYSOMM, NULL);
   /* the raw values */
	
   out->mesh_color   = in->mesh_color;
   out->border_color = in->border_color;
	
   /* vogle objects id */
   out->MESH      = in->MESH;
   out->BORDER    = in->BORDER;
   out->CONTOURS  = in->CONTOURS;
	
   /* flags */
   out->show_mesh     = in->show_mesh;
   out->show_border   = in->show_border;
   out->show_contours = in->show_contours;
	
	/* governs which object to delete */
	out->delete_border_object_flag = in->delete_border_object_flag;
	out->delete_mesh_object_flag = in->delete_mesh_object_flag;
	out->delete_contours_object_flag = in->delete_contours_object_flag;
	
	/* governs which object to build */
	out->build_border_object_flag = in->build_border_object_flag;
	out->build_mesh_object_flag = in->build_mesh_object_flag;
	out->build_contours_object_flag = in->build_contours_object_flag;
	
	out->ucdmesh = in->ucdmesh;

	/* nb levels */
	out->nb_levels = in->nb_levels;
	if ( in->levels != NULL )
	{
		out->levels = v_copy (in->levels, NULL);
	}
	else
	{
		out->levels = NULL;
	}

   out->setup_objects  = in->setup_objects;
   out->delete_objects = in->delete_objects;
	
   /**/
   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void set_drawfunc_voplcontours(VOPL_CONTOUR_DATA* pdata, VOPL_CONTOUR_DRAW_FUNC drawfunc)
{
   pdata->draw = (VOPL_CONTOUR_DATA*)drawfunc; /* set the function who draws */
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_GRAPH_DATA *vopl_graphdata_new(int nx, int ny, int vopl_plot_type)
{
   VOPL_GRAPH_DATA *pdata = malloc ( sizeof(VOPL_GRAPH_DATA) );
   int i,j,k;

   pdata->nx = nx;
   pdata->ny = ny;

   for (k=0; k<NB_MAX_SUBPLOTS; k++)
   {
      pdata->dataplot[k] = NULL;
   }
	
   for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	{
		k = i*nx + j;
			
      if (vopl_plot_type == VOPL_CURVE) 
			pdata->dataplot[k] = curvedata_new();
	   if (vopl_plot_type == VOPL_CONTOURS)
			pdata->dataplot[k] = contourdata_new();

      pdata->dataplot[k]->ix = i+1; 
		pdata->dataplot[k]->iy = j+1;
   }
	
   return pdata;
}

void *vopl_graphdata_free (VOPL_GRAPH_DATA *in)
{
   /* printf("freeing 0x%lx\n", in); */
   
   if ( in != NULL)
   {
      int nx = in->nx;
      int ny = in->ny;

      int k;

      for (k=0; k<nx*ny; k++)
      {
         if (in->dataplot[k]->plot_type == VOPL_CURVE)
				curvedata_free(in->dataplot[k]);
			if (in->dataplot[k]->plot_type == VOPL_CONTOURS)
				contourdata_free(in->dataplot[k]);
      }

      free(in);
   }

   return NULL;
}

VOPL_GRAPH_DATA *vopl_graphdata_clone(VOPL_GRAPH_DATA *in)
{
   int k;
   VOPL_GRAPH_DATA *out;

   int nx ;
   int ny ;

   if ( in == NULL )
   {
      return NULL;
   }

   out = malloc ( sizeof(VOPL_GRAPH_DATA) );
   
   nx = in->nx;
   ny = in->ny;

   for (k=0; k<NB_MAX_SUBPLOTS; k++)
   {
      out->dataplot[k] = NULL;
   }

   for (k=0; k<nx*ny; k++)
   {
      if (in->dataplot[k]->plot_type == VOPL_CURVE)
			out->dataplot[k] = curvedata_clone(in->dataplot[k]);
		if (in->dataplot[k]->plot_type == VOPL_CONTOURS)
			out->dataplot[k] = contourdata_clone(in->dataplot[k]);
   }

   strncpy(out->driver, in->driver, 15);

   out->window_size_x = in->window_size_x;
   out->window_size_y = in->window_size_y;
   
   out->nx = in->nx;
   out->ny = in->ny;

   strncpy(out->title, in->title, 63);

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
 
typedef struct marker_map_s_
{
   int  vogle_id;
   char *vopl_marker_name;
} marker_map_s;

static marker_map_s marker_map[10] = {
   { 0                           ,  0                  },
   { VOGLEe_MARKER_CIRCLE        ,  VOPL_CIRCLE        },
   { VOGLEe_MARKER_CUBE          ,  VOPL_CUBE          },
   { VOGLEe_MARKER_TRIANGLE      ,  VOPL_TRIANGLE      },
   { VOGLEe_MARKER_LOSANGE       ,  VOPL_LOSANGE       },
   { VOGLEe_MARKER_CHRISMAS      ,  VOPL_CHRISMAS      },
   { VOGLEe_MARKER_CROSS         ,  VOPL_CROSS         },
   { VOGLEe_MARKER_ADDITION      ,  VOPL_ADDITION      },
   { VOGLEe_MARKER_MULTIPICATION ,  VOPL_MULTIPICATION },
   { VOGLEe_MARKER_STAR          ,  VOPL_STAR          }
};

typedef struct font_map_s_
{
   int  vogle_id;
   char *hershey_font_name;
} font_map_s;

static font_map_s font_map[22] = { 
   
   { VOGLEe_FONT_ASTROLOGY    , "astrology"  },
   { VOGLEe_FONT_CURSIVE      , "cursive"    },
   { VOGLEe_FONT_FUTURA_L     , "futura.l"   },
   { VOGLEe_FONT_FUTURA_M     , "futura.m"   },
   { VOGLEe_FONT_GOTHIC_ENG   , "gothic.eng" },
   { VOGLEe_FONT_GOTHIC_GER   , "gothic.ger" },
   { VOGLEe_FONT_GOTHIC_ITA   , "gothic.ita" },
   { VOGLEe_FONT_GREEK        , "greek"      },
   { VOGLEe_FONT_JAPANESE     , "japanese"   },
   { VOGLEe_FONT_MARKERS      , "markers"    },
   { VOGLEe_FONT_MATH_LOW     , "math.low"   },
   { VOGLEe_FONT_MATH_UPP     , "math.upp"   },
   { VOGLEe_FONT_METEOROLOGY  , "meteorology"},
   { VOGLEe_FONT_MUSIC        , "music"      },
   { VOGLEe_FONT_CYRILLIC     , "cyrillic"   },
   { VOGLEe_FONT_SCRIPT       , "script"     },
   { VOGLEe_FONT_SYMBOLIC     , "symbolic"   },
   { VOGLEe_FONT_TIMES_G      , "times.g"    },
   { VOGLEe_FONT_TIMES_IB     , "times.ib"   },
   { VOGLEe_FONT_TIMES_I      , "times.i"    },
   { VOGLEe_FONT_TIMES_R      , "times.r"    },
   { VOGLEe_FONT_TIMES_RB     , "times.rb"   }
};

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void drawcurve_vopl1D(const VOPL_CURVE_DATA *dataplot )
{
   int dim = dataplot->x->dim;
   

   panel(dataplot->ix,dataplot->iy);

   clearpanel(VOGLEe_COLOR_BLACK);

   font(font_map[dataplot->font].hershey_font_name);

   /* the label size is automatically calculated in vopl */
   /*
   textsize(dataplot->labelsize, dataplot->labelsize);
   */
   
   /*annotate("%d", 'x');*/

   scaling(LINEAR, 'x');
   scaling(LINEAR, 'y');

   withbox(dataplot->withbox);

   /*
    *  Set a fix window
    */
   if ( ! dataplot->adjustscale )
   {
      setwindow( dataplot->xmin, dataplot->xmax, 'x');
      setwindow( dataplot->ymin, dataplot->ymax, 'y');
   }
   
   /*
    *	Adjust the window according to according to x and y arrays -- overwrite "setwindow" --
    */
   if ( dataplot->adjustscale )
   {
      adjustscale(dataplot->x->ve, dim, 'x');
      adjustscale(dataplot->y->ve, dim, 'y');
   }

   
   /*
    *	Now set the color to GREEN
    */
   color(GREEN);

   /*
    *	Draw the default set of axes (in GREEN)
    */
   axistitle("(X)", 'x');
   axistitle("(Y)", 'y');

   drawaxes();

   /*
    * and the title ...
    */
   graphtitle(dataplot->legend);
   drawtitle();

   /*
    *	Set color to RED
    */
   color(dataplot->color);
   /*
    *	Draw the Graph
    */
   plot2(dataplot->x->ve, dataplot->y->ve, dim);

   /*
    * marker
    */
   if (dataplot->end_marker)
   {
      color(VOGLEe_COLOR_RED);
      plotmarker(dataplot->x->ve[dim-1], dataplot->y->ve[dim-1], marker_map[dataplot->end_marker].vopl_marker_name);
   }

}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_vopl_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny, int vopl_plot_type)
{	
   /* create a queue , set it in a static data  */
   SVQueue *queue = svqueue_new((CLONE_FUNC)vopl_graphdata_clone, (FREED_FUNC)vopl_graphdata_free);
	
   svqueue_vogle_set_static(queue);
	
   /* create the container for the data to plot , set it in a static data  */
   VOPL_GRAPH_DATA *pdata = vopl_graphdata_new(nx,ny, vopl_plot_type);
	
   strncpy(pdata->driver, driver, 15);
   
   pdata->window_size_x  = window_size_x;
   pdata->window_size_y  = window_size_y;

   pdata_vopl_set_static(pdata);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

char *graphics1D_vopl_title(const char *title)
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
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

int graphics1D_vopl_window( int ix,  int iy,  double window_xmin , double window_xmax ,
                                              double window_ymin , double window_ymax )
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->xmin = window_xmin ;
   pdata->dataplot[k]->xmax = window_xmax ;
   pdata->dataplot[k]->ymin = window_ymin ;
   pdata->dataplot[k]->ymax = window_ymax ;
	
	if (pdata->dataplot[k]->plot_type == VOPL_CONTOURS) /* to be sure to see the border */
	{
		pdata->dataplot[k]->xmin -= 1.0e-7 ;
		pdata->dataplot[k]->xmax += 1.0e-7 ;
		pdata->dataplot[k]->ymin -= 1.0e-7 ;
		pdata->dataplot[k]->ymax += 1.0e-7 ;
	}

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_labelsize( int ix,  int iy, double size)
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
	pdata->dataplot[k]->labels_size = size;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_font     (int ix,  int iy, int font_id)
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
	pdata->dataplot[k]->font = font_id;
	
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_legend(int ix, int iy, const char *legend)
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
   strncpy(pdata->dataplot[k]->legend, legend, 63);

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_setbox( int ix,  int iy,  int hasbox     )
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);
	
	VOPL_CURVE_DATA * pcurvedata = (VOPL_CURVE_DATA*)(pdata->dataplot[k]);

   pcurvedata->withbox = hasbox;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_endmarker(int ix, int iy, const int marker)
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   VOPL_CURVE_DATA * pcurvedata = (VOPL_CURVE_DATA*)(pdata->dataplot[k]);
	
	pcurvedata->end_marker = marker;

   return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int graphics1D_vopl_curvedata(int ix, int iy, int color1, const VEC *X, const VEC *Y)
{
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   VOPL_CURVE_DATA * pcurvedata = (VOPL_CURVE_DATA*)(pdata->dataplot[k]);
	
	/* set the data in curvedata structure */
   pcurvedata->x = v_copy(X, NULL);
   pcurvedata->y = v_copy(Y, NULL);

   pcurvedata->color = color1;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_curvedata1(int ix, int iy, int color, const VEC *X, const VEC *Y, int idx1, int idx2)
{
   VOPL_GRAPH_DATA* pdata = pdata_vopl_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_vogle_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

	VOPL_CURVE_DATA * pcurvedata = (VOPL_CURVE_DATA*)(pdata->dataplot[k]);
	
   pcurvedata->color  = color;
   pcurvedata->adjustscale = 0;

   if ( idx1 == VOGLEd_IDX_FIRST && idx2 == VOGLEd_IDX_LAST )
   {
      pcurvedata->x = v_copy(X, pcurvedata->x);
      pcurvedata->y = v_copy(Y, pcurvedata->y);
   }
   else
   {
      VEC *XX = v_get(idx2-idx1+1);
      VEC *YY = v_get(idx2-idx1+1);

      v_move(X, idx1, idx2-idx1+1, XX, 0);
      v_move(Y, idx1, idx2-idx1+1, YY, 0);

      /*
       if (pcurvedata->x == NULL) pcurvedata->x = v_get(X->dim);
       if (pcurvedata->y == NULL) pcurvedata->y = v_get(Y->dim);
       */
      pcurvedata->x = v_copy(XX, pcurvedata->x);
      pcurvedata->y = v_copy(YY, pcurvedata->y);

      V_FREE(XX);
      V_FREE(YY);
   }

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_vopl_set_plotdata_in_queue(void)
{
   SVQueue           * queue = svqueue_vogle_get_static();
   VOPL_GRAPH_DATA   * pdata = pdata_vopl_get_static();
	
   /* set the data in the queue */
   queue->xset(queue, pdata);
	
   /**/
   return;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_nblevels(int ix, int iy, int nb_levels)
{
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();
	
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
   VOPL_CONTOUR_DATA * pcontourdata = (VOPL_CONTOUR_DATA*)(pdata->dataplot[k]);
	
	/* set the data in curvedata structure */
   pcontourdata->nb_levels = nb_levels;
	
   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_levels(int ix, int iy, VEC *levels)
{
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();
	
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
   VOPL_CONTOUR_DATA * pcontourdata = (VOPL_CONTOUR_DATA*)(pdata->dataplot[k]);

	pcontourdata->levels = v_copy(levels, pcontourdata->levels);
	
   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_contourmeshdata(int ix, int iy, const IMAT *NSELMT, const MAT *XYSOMM)
{
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();
	
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
	VOPL_CONTOUR_DATA * pcontourdata = (VOPL_CONTOUR_DATA*)(pdata->dataplot[k]);
	
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    */

   pcontourdata->NSELMT = im_copy(NSELMT, pcontourdata->NSELMT);
   pcontourdata->XYSOMM = m_copy(XYSOMM, pcontourdata->XYSOMM);
	
	pcontourdata->ucdmesh = vopl_ucdmesh_build(pcontourdata->XYSOMM->me, pcontourdata->XYSOMM->m, 
															 pcontourdata->NSELMT->im, pcontourdata->NSELMT->m);
	
   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_contourplotdata(int ix, int iy, const VEC *vec)
{
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();
	
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
	VOPL_CONTOUR_DATA * pcontourdata = (VOPL_CONTOUR_DATA*)(pdata->dataplot[k]);
	
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    */
	
	pcontourdata->VALUES = v_copy(vec, pcontourdata->VALUES);
	
   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_vopl_contourtransientbuildinfo(int ix, int iy)
{
   VOPL_GRAPH_DATA * pdata = pdata_vopl_get_static();
	
   int nx = pdata->nx;
   int ny = pdata->ny;
	
   int k = (ix-1)*ny + (iy-1);
	
	VOPL_CONTOUR_DATA * pcontourdata = (VOPL_CONTOUR_DATA*)(pdata->dataplot[k]);
	
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    */
	
	pcontourdata->build_border_object_flag = 0;
	pcontourdata->build_mesh_object_flag = 0;
	
	pcontourdata->delete_border_object_flag = 0;
	pcontourdata->delete_mesh_object_flag = 0;
	
   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void drawcontours_vopl1D(VOPL_CONTOUR_DATA *pdata)
{	
	color(VOGLEe_COLOR_BLACK);
   clear();
	
   if (pdata->show_contours) callobj(pdata->CONTOURS);
	if (pdata->show_mesh) callobj(pdata->MESH);
	if (pdata->show_border) callobj(pdata->BORDER);
	
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void delete_voplcontour_objects(void * data)
{
   VOPL_CONTOUR_DATA* pdata = (VOPL_CONTOUR_DATA*) data;
	
   printf("delete_objects...\n");
	
   if (pdata->delete_contours_object_flag) delobj(pdata->CONTOURS);
   if (pdata->delete_border_object_flag) delobj(pdata->BORDER);
   if (pdata->delete_mesh_object_flag) delobj(pdata->MESH);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void setup_voplcontour_objects(void * data)
{	
   VOPL_CONTOUR_DATA* pdata = (VOPL_CONTOUR_DATA*) data;
	
   MAT * XYSOMM = pdata->XYSOMM;
   VEC * VALUES = pdata->VALUES;
   IMAT *NSELMT = pdata->NSELMT;
	
   printf("setup_objects...\n");
	
	setwindow( pdata->xmin, pdata->xmax, 'x');
	setwindow( pdata->ymin, pdata->ymax, 'y');
	
   /*
    * Make the mesh
    */
   if (pdata->build_mesh_object_flag)
	{
		makeobj(pdata->MESH);
      {
         color(pdata->mesh_color);
		
		   contour_mesh(XYSOMM->me, VALUES->ve, VALUES->dim, NSELMT->im, NSELMT->m);
      }
      closeobj();
	}
	
   /*
    * make the border
    */
   if (pdata->build_border_object_flag)
	{
		makeobj(pdata->BORDER);
      {
         color(pdata->border_color);
		
		   contour_border(XYSOMM->me, VALUES->ve, VALUES->dim, NSELMT->im, NSELMT->m);
	   }
      closeobj();
	}
	
   /*
    * make the contours
    */
   if (pdata->build_contours_object_flag)
	{
		makeobj(pdata->CONTOURS);
      {
		   if ( pdata->levels )
		   {
			   /*contour(pdata->levels->ve, pdata->levels->dim, XYSOMM->me, VALUES->ve, VALUES->dim, NSELMT->im, NSELMT->m);*/
				contour_with_vopl_ucdmesh(pdata->levels->ve, pdata->levels->dim, VALUES->ve, pdata->ucdmesh);
		   }
		   else 
		   {
		      int nb_levels = pdata->nb_levels;
		
		      VEC *levels = v_get(nb_levels);
			
			   int i, idx = 0;
		      double minvalue = v_min(VALUES, &idx);
		      double maxvalue = v_max(VALUES, &idx);
		
		      for (i=0; i<nb_levels; i++)
		      {
			      levels->ve[i] = minvalue + (maxvalue - minvalue)/(nb_levels+1)*(i+1);
		      }
		
		      contour(levels->ve, nb_levels, XYSOMM->me, VALUES->ve, VALUES->dim, NSELMT->im, NSELMT->m);
		
		      V_FREE(levels);
		   }
		}
   }
   closeobj();
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

