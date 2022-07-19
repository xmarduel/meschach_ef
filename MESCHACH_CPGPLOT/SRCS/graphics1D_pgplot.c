

/** \file graphics1D_pgplot.c
 *
 * Contains the routines for plotting with PGPLOT library
 *
 */

#ifdef HAVE_CPGPLOT
#include "cpgplot.h"         /* Librairie CPGPLOT      */
#endif


#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_svqueue.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"


/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

static PGPLOT_GRAPH_DATA *pdata_pgplot  =  NULL;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

PGPLOT_GRAPH_DATA * pdata_pgplot_get_static()
{
   return pdata_pgplot;
}

PGPLOT_GRAPH_DATA * pdata_pgplot_set_static(PGPLOT_GRAPH_DATA *data)
{
   pdata_pgplot = data;

   return pdata_pgplot;
}

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

PGPLOT_CURVE_DATA *pgplot_curvedata_new  (int dim)
{
   PGPLOT_CURVE_DATA *out = malloc ( sizeof(PGPLOT_CURVE_DATA) );

   out->x = NULL;
   out->y = NULL;

   if (dim>0) out->x = v_get(dim);
   if (dim>0) out->y = v_get(dim);

   out->font         = 1; /* default */
   out->labels_size  = 1.0; /* default */
   out->draw         = drawscene_pgplot1D; /* default drawing */
   out->set_drawfunc = set_drawfunc_pgplot; /* to set a new drawing function */

   return out;
}

void *pgplot_curvedata_free (PGPLOT_CURVE_DATA *in)
{
   V_FREE(in->x);
   V_FREE(in->y);

   free(in);

   return NULL;
}

PGPLOT_CURVE_DATA *pgplot_curvedata_clone(PGPLOT_CURVE_DATA *in)
{
   PGPLOT_CURVE_DATA *out = pgplot_curvedata_new(in->x->dim);

   out->ix = in->ix;
   out->iy = in->iy;

   out->x = v_copy(in->x, out->x);
   out->y = v_copy(in->y, out->y);

   out->font         = in->font;
   out->labels_size  = in->labels_size;

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

void set_drawfunc_pgplot(PGPLOT_CURVE_DATA* pdata, PGPLOT_DRAW_FUNC drawfunc)
{
   pdata->draw = drawfunc; /* set the function who draws */
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

PGPLOT_GRAPH_DATA *pgplot_graphdata_new(int nx, int ny)
{
   PGPLOT_GRAPH_DATA *pdata = malloc ( sizeof(PGPLOT_GRAPH_DATA) );
   int i,j,k;

   pdata->nx = nx;
   pdata->ny = ny;

   for (i=0; i<nx; i++)
	for (j=0; j<ny; j++)
	{
		k = i*nx + j;
		
		pdata->dataplot[k] = pgplot_curvedata_new(0);
		
		pdata->dataplot[k]->ix = i+1;
		pdata->dataplot[k]->iy = j+1;
	}
	
	/* set defaults */
   for (k=0; k<nx*ny; k++)
   {
      /* the datas */
      pdata->dataplot[k]->x = NULL;
      pdata->dataplot[k]->y = NULL;
      /* plot attributes */
      pdata->dataplot[k]->labels_size  = 1.5;
      pdata->dataplot[k]->color        = PGPLOTe_COLOR_RED;
      pdata->dataplot[k]->withbox      = 1;
      pdata->dataplot[k]->adjustscale  = 1;
		
      pdata->dataplot[k]->xmin =  0.0;
      pdata->dataplot[k]->xmax =  1.0;
      pdata->dataplot[k]->ymin = -1.0;
      pdata->dataplot[k]->ymax =  1.0;
		
      pdata->dataplot[k]->marker      = PGPLOTe_MARKER_UNDEFINED;
		pdata->dataplot[k]->end_marker  = PGPLOTe_MARKER_UNDEFINED;
		
      strcpy(pdata->dataplot[k]->legend, "solution");
   }	

   return pdata;
}

void *pgplot_graphdata_free (PGPLOT_GRAPH_DATA *in)
{
   if ( in != NULL)
   {
      int nx = in->nx;
      int ny = in->ny;

      int k;

      for (k=0; k<nx*ny; k++)
      {
         pgplot_curvedata_free(in->dataplot[k]);
      }

      free(in);
   }

   return NULL;
}

PGPLOT_GRAPH_DATA *pgplot_graphdata_clone(PGPLOT_GRAPH_DATA *in)
{
   int k;
   PGPLOT_GRAPH_DATA *out;

   int nx ;
   int ny ;

   if ( in == NULL )
   {
      return NULL;
   }

   out = malloc ( sizeof(PGPLOT_GRAPH_DATA) );
   
   nx = in->nx;
   ny = in->ny;

   for (k=0; k<nx*ny; k++)
   {
      out->dataplot[k] = pgplot_curvedata_clone(in->dataplot[k]);
   }

   out->nx = in->nx;
   out->ny = in->ny;

   strncpy(out->driver, in->driver, 15);

   out->window_size_x = in->window_size_x;
   out->window_size_y = in->window_size_y;
   
   strncpy(out->title, in->title, 63);

   return out;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void drawscene_pgplot1D(const PGPLOT_CURVE_DATA *dataplot )
{
   int i;
   float *xr = malloc( dataplot->x->dim * sizeof(float) ); /* dataplot->x is double */
   float *yr = malloc( dataplot->y->dim * sizeof(float) ); /* dataplot->y is double */
   int dim;

   dim = dataplot->x->dim;

   /*
    * get the points to plot in "float".
    */
   for(i=0; i<dim; i++)
   {
      xr[i] = dataplot->x->ve[i];
      yr[i] = dataplot->y->ve[i];
   }

   
   cpgpanl(dataplot->ix,dataplot->iy);
   cpgeras();
   
   cpgsci(PGPLOTe_COLOR_GREEN);

   cpgsch(dataplot->labels_size);

   cpgvstd();
   cpgswin(dataplot->xmin, dataplot->xmax, dataplot->ymin, dataplot->ymax);

   /*
    *	Set box arounf the plot
    */
   cpgbox("BCNST", 0.0, 0, "BCNMST", 0.0, 0);

   /*
    *	Set legend
    */
   cpglab("(x)", "(y)", dataplot->legend);
   
   /*
    *	Set color
    */
   cpgsci(dataplot->color);

   /*
    *	Draw the Graph
    */
   cpgline(dim, xr, yr);

   /*
    *	Draw the end marker
    */
   if ( dataplot->end_marker >= 0 )
   {
      cpgsci(PGPLOTe_COLOR_RED);
      cpgpt1(xr[dim-1], yr[dim-1], dataplot->end_marker);
   }

   /*
    * finito
    */
   free(xr);
   free(yr);
}

/* ----------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------- */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_cpgplot_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny)
{
   int k;
	
   /* create a queue , set it in a static data  */
   SVQueue *queue = svqueue_new(pgplot_graphdata_clone, pgplot_graphdata_free);
	
   svqueue_pgplot_set_static(queue);
	
   /* create the container for the data to plot , set it in a static data  */
   PGPLOT_GRAPH_DATA *pdata = pgplot_graphdata_new(nx,ny);
	
   strncpy(pdata->driver, driver, 15);
	
   pdata->window_size_x = window_size_x;
   pdata->window_size_y = window_size_y;
	
   pdata_pgplot_set_static(pdata);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

char *graphics1D_cpgplot_title(const char *title)
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
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

int graphics1D_cpgplot_window(int ix,  int iy,  double window_xmin , double window_xmax ,
                                                double window_ymin , double window_ymax )
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
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

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_cpgplot_font( int ix,  int iy, const int font_id     )
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->font = font_id;

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_cpgplot_setbox( int ix,  int iy, const int hasbox     )
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->withbox = hasbox;

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_cpgplot_legend(int ix, int iy, const char *legend)
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   strncpy(pdata->dataplot[k]->legend, legend, 63);

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_cpgplot_labelsize(int ix, int iy, const double size)
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->labels_size = size;

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_cpgplot_endmarker(int ix, int iy, const int marker)
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */
   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->end_marker = marker;

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_cpgplot_curvedata(int ix, int iy, int color1, const VEC *X, const VEC *Y)
{
   PGPLOT_GRAPH_DATA * pdata = pdata_pgplot_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   /* set the data in plottingdata structure */
   pdata->dataplot[k]->x = v_copy(X, NULL);
   pdata->dataplot[k]->y = v_copy(Y, NULL);

   pdata->dataplot[k]->color = color1;

   /**/
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_cpgplot_curvedata1(int ix, int iy, int color, const VEC *X , const VEC *Y, int idx1, int idx2)
{
   PGPLOT_GRAPH_DATA* pdata = pdata_pgplot_get_static();
   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_pgplot_plotcurves_flush()
    */

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   pdata->dataplot[k]->color  = color;
   pdata->dataplot[k]->adjustscale = 0;

   if ( idx1 == PGPLOTd_IDX_FIRST && idx2 == PGPLOTd_IDX_LAST )
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


   /* finito */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_cpgplot_set_plotdata_in_queue(void)
{
   SVQueue            * queue = svqueue_pgplot_get_static();
   PGPLOT_GRAPH_DATA  * pdata = pdata_pgplot_get_static();
	
   /* set the data in the queue */
   queue->xset(queue, pdata);
	
   /**/
   return;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_cpgplot_finalize(void)
{
   cpgend();
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


