
#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_svqueue.h"

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static LIBSCIPLOT_GRAPH_DATA *pdata_libsciplot  =  NULL;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

LIBSCIPLOT_GRAPH_DATA * pdata_libsciplot_get_static(void)
{
   return pdata_libsciplot;
}

LIBSCIPLOT_GRAPH_DATA * pdata_libsciplot_set_static(LIBSCIPLOT_GRAPH_DATA *data)
{
   pdata_libsciplot = data;

   return pdata_libsciplot;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

LIBSCIPLOT_CURVE_DATA *libsciplot_curvedata_new  (int dim)
{
   LIBSCIPLOT_CURVE_DATA *out = malloc ( sizeof(LIBSCIPLOT_CURVE_DATA) );

   out->x = NULL;
   out->y = NULL;

   if (dim>0) out->x = v_get(dim);
   if (dim>0) out->y = v_get(dim);

   /* set defaults plot attributes */
   out->draw         = drawscene_libsciplot1D; /* default drawing */
   out->set_drawfunc = set_drawfunc_libsciplot; /* to set a new drawing function */

   out->font         = 1;
   out->labels_size  = 1.0;
   out->color        = LIBSCIPLOTe_COLOR_RED;
   out->withbox      = 1;
   out->adjustscale  = 1;

   out->xmin =  0.0;
   out->xmax =  1.0;
   out->ymin = -1.0;
   out->ymax =  1.0;

   out->frame_on_top = 1;
   out->frame_color  = LIBSCIPLOTe_COLOR_BLACK;
   out->legend_color = LIBSCIPLOTe_COLOR_BLACK;

   out->marker = 0;
	out->end_marker = 0;

   /*
   out->marker      = LIBSCIPLOTe_MARKER_UNDEFINED;
	out->end_marker  = LIBSCIPLOTe_MARKER_UNDEFINED;
    */
   strcpy(out->legend, "solution");
   strcpy(out->x_legend, "(X)");
   strcpy(out->y_legend, "(Y)");

   return out;
}

void *libsciplot_curvedata_free (LIBSCIPLOT_CURVE_DATA *in)
{
   if (in)
   {
      if (in->x) V_FREE(in->x);
      if (in->y) V_FREE(in->y);

      free(in);
   }

   return NULL;
}

LIBSCIPLOT_CURVE_DATA *libsciplot_curvedata_clone(LIBSCIPLOT_CURVE_DATA *in)
{
   LIBSCIPLOT_CURVE_DATA *out = libsciplot_curvedata_new(in->x->dim);

   out->ix = in->ix;
   out->iy = in->iy;

   out->x = v_copy(in->x, out->x);
   out->y = v_copy(in->y, out->y);

   out->font         = in->font;
   out->labels_size  = in->labels_size;

   out->color  = in->color;
	
   out->marker     = in->marker;
	out->end_marker = in->end_marker;

   out->adjustscale = in->adjustscale;

   out->xmin = in->xmin; /* window */
   out->xmax = in->xmax; /* window */

   out->ymin = in->ymin; /* window */
   out->ymax = in->ymax; /* window */

   out->withbox = in->withbox;

   out->frame_color  = in->frame_color;
   out->legend_color = in->legend_color;

   strncpy(out->legend, in->legend, 63);

   out->draw         = in->draw; /* default drawing */
   out->set_drawfunc = in->set_drawfunc; /* to set a new drawing function */

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void set_drawfunc_libsciplot(LIBSCIPLOT_CURVE_DATA* pdata, LIBSCIPLOT_DRAW_FUNC drawfunc)
{
   pdata->draw = drawfunc; /* set the function who draws */
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

LIBSCIPLOT_GRAPH_DATA *libsciplot_graphdata_new(int nx, int ny)
{
   LIBSCIPLOT_GRAPH_DATA *pdata = malloc ( sizeof(LIBSCIPLOT_GRAPH_DATA) );
   int i,j,k;

   pdata->nx = nx;
   pdata->ny = ny;

	for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	{
		k = i*nx + j;
			
		pdata->dataplot[k] = libsciplot_curvedata_new(0);
			
		pdata->dataplot[k]->ix = i+1;
		pdata->dataplot[k]->iy = j+1;
	}

   pdata->bgcolor = LIBSCIPLOTe_WHITE;

	/* set defaults */
   for (k=0; k<nx*ny; k++)
   {
      /* the datas */
      pdata->dataplot[k]->x = NULL;
      pdata->dataplot[k]->y = NULL;
      /* plot attributes */
      pdata->dataplot[k]->labels_size  = 1.0;
      pdata->dataplot[k]->color        = LIBSCIPLOTe_COLOR_RED;
      pdata->dataplot[k]->withbox      = 1;
      pdata->dataplot[k]->adjustscale  = 1;
		
      pdata->dataplot[k]->xmin =  0.0;
      pdata->dataplot[k]->xmax =  1.0;
      pdata->dataplot[k]->ymin = -1.0;
      pdata->dataplot[k]->ymax =  1.0;
		
      pdata->dataplot[k]->marker  = 0;
		pdata->dataplot[k]->end_marker  = 0;
		
      strcpy(pdata->dataplot[k]->legend, "solution");
   }

	
   return pdata;
}

void *libsciplot_graphdata_free (LIBSCIPLOT_GRAPH_DATA *in)
{
   if ( in != NULL)
   {
      int nx = in->nx;
      int ny = in->ny;

      int k;

      for (k=0; k<nx*ny; k++)
      {
         libsciplot_curvedata_free(in->dataplot[k]);
      }

      free(in);
   }

   return NULL;
}

LIBSCIPLOT_GRAPH_DATA *libsciplot_graphdata_clone(LIBSCIPLOT_GRAPH_DATA *in)
{
   int k;
   LIBSCIPLOT_GRAPH_DATA *out;

   int nx ;
   int ny ;

   if ( in == NULL )
   {
      return NULL;
   }

   out = malloc ( sizeof(LIBSCIPLOT_GRAPH_DATA) );

   nx = in->nx;
   ny = in->ny;

   for (k=0; k<nx*ny; k++)
   {
      out->dataplot[k] = libsciplot_curvedata_clone(in->dataplot[k]);
   }

   out->nx = in->nx;
   out->ny = in->ny;

   strncpy(out->driver, in->driver, 15);

   out->window_size_x = in->window_size_x;
   out->window_size_y = in->window_size_y;

   strncpy(out->title, in->title, 63);

   out->bgcolor = in->bgcolor;

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_title(const char *title)
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   strncpy(pdata->title, title, 63);

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_bgcolor(const int bgcolor)
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   pdata->bgcolor = bgcolor;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_setbox(int ix, int iy, const int hasbox)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = pdata_libsciplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->withbox = hasbox;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_libsciplot_endmarker(int ix, int iy, const int marker)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = pdata_libsciplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->end_marker = marker;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_window(int ix,  int iy,  double window_xmin , double window_xmax ,
                                                   double window_ymin , double window_ymax )
{
   LIBSCIPLOT_GRAPH_DATA* pdata = pdata_libsciplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->xmin = window_xmin;
   pdata->dataplot[k]->xmax = window_xmax;
   pdata->dataplot[k]->ymin = window_ymin;
   pdata->dataplot[k]->ymax = window_ymax;

   pdata->dataplot[k]->adjustscale = 0;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_labelsize( int ix,  int iy, double size)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = pdata_libsciplot_get_static();
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

int graphics1D_libsciplot_framecolor( int ix, int iy , const int frame_color  )
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->frame_color = frame_color;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_legendcolor( int ix, int iy , const int legend_color  )
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->legend_color = legend_color;

   return EXIT_SUCCESS;
}
/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_legend( int ix, int iy , const char *legend  )
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   strncpy(pdata->dataplot[k]->legend, legend, 256) ;
   pdata->dataplot[k]->legend[255] = '\0';

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_curvedata(int ix, int iy, int color1, const VEC *X, const VEC *Y)
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   /* set the data in plottingdata structure */
   pdata->dataplot[k]->x = v_copy(X, NULL);
   pdata->dataplot[k]->y = v_copy(Y, NULL);

   pdata->dataplot[k]->color = color1;

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_curvedata1(int ix, int iy, int color, const VEC *X , const VEC *Y, int idx1, int idx2)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = pdata_libsciplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_libsciplot_plotcurves_flush()
    */

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->color  = color;
   pdata->dataplot[k]->adjustscale = 0;

   if ( idx1 == LIBSCIPLOTd_IDX_FIRST && idx2 == LIBSCIPLOTd_IDX_LAST )
   {
      pdata->dataplot[k]->x = v_copy(X, pdata->dataplot[k]->x);
      pdata->dataplot[k]->y = v_copy(Y, pdata->dataplot[k]->y);
   }
   else
   {
      VEC *XX = v_get(idx2-idx1+1);
      VEC *YY = v_get(idx2-idx1+1);
		
      v_move(X, idx1, idx2-idx1+1, XX, 0);
      v_move(Y, idx1, idx2-idx1+1, YY, 0);
		
      pdata->dataplot[k]->x = v_copy(XX, pdata->dataplot[k]->x);
      pdata->dataplot[k]->y = v_copy(YY, pdata->dataplot[k]->y);

      V_FREE(XX);
      V_FREE(YY);
   }

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_libsciplot_set_plotdata_in_queue(void)
{
   SVQueue                 * queue = svqueue_libsciplot_get_static();
   LIBSCIPLOT_GRAPH_DATA   * pdata = pdata_libsciplot_get_static();

   /* set the data in the queue */
   queue->xset(queue, pdata);

   return;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

