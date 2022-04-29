
#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Command.h>
#endif

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_svqueue.h"
#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot_stationnary.h"

#ifdef HAVE_LIBSCIPLOT
#include "libsciplot.h"
#endif

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void * graphics1D_libsciplot_curve_with_xt_toolkit(void *pdata); /* function sent to the thread */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

static Multigrapher*  MULTIGRAPHER;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_init_multigrapher(const char *driver, Display * display, Window *pwindow, int window_size_x, int window_size_y)
{
   int size_x_window_pixel = window_size_x;
   int size_y_window_pixel = window_size_y;

   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   char *display_type;
   char bg_color[16];
   char bitmap_size[16];
   /* strings to pass to libplot */
   if (pdata)
   {
      switch(pdata->bgcolor)
      {
          case LIBSCIPLOTe_GREEN  : strcpy(bg_color, "green");  break; 
          case LIBSCIPLOTe_RED    : strcpy(bg_color, "red");    break; 
          case LIBSCIPLOTe_BLUE   : strcpy(bg_color, "blue");   break; 
          case LIBSCIPLOTe_YELLOW : strcpy(bg_color, "yellow"); break; 
          case LIBSCIPLOTe_WHITE  : strcpy(bg_color, "white");  break; 
          case LIBSCIPLOTe_BLACK  : strcpy(bg_color, "black");  break; 
          default:                  strcpy(bg_color, "white");  break; 
      }
   }
   else
   {
      strcpy(bg_color, "white");
   }

   /* strings to pass to libplot */
   if ( strcmp(driver,"X") == 0 )
   {
      display_type = strdup("Xdrawable");
   }
   else
   if ( strcmp(driver,"GIF") == 0 )
   {
      display_type = strdup("GIF");
   }
   else
   if ( strcmp(driver,"PNM") == 0 )
   {
      display_type = strdup("PNM");
   }
   else
   {
      display_type = strdup("Xdrawable");
   }
   
   snprintf(bitmap_size, 15, "%dx%d", size_x_window_pixel, size_y_window_pixel);

                         
   /* call to LIBSCIPLOT */
   MULTIGRAPHER = libsciplot_open(display_type, display, pwindow, bg_color, bitmap_size);
   
   if ( MULTIGRAPHER == NULL)
   {
      return EXIT_FAILURE;
   }
   else
   {
      return EXIT_SUCCESS;
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_libsciplot_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny)
{
   /* create a queue , set it in a static data  */
   SVQueue *queue = svqueue_new(libsciplot_graphdata_clone, libsciplot_graphdata_free);

   svqueue_libsciplot_set_static(queue);
   
   /* create the container for the data to plot , set it in a static data  */
   LIBSCIPLOT_GRAPH_DATA *pdata = libsciplot_graphdata_new(nx,ny);

   strncpy(pdata->driver, driver, 15);

   pdata->window_size_x = window_size_x;
   pdata->window_size_y = window_size_y;

   pdata_libsciplot_set_static(pdata);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_finalize(void)
{
   /*
   libsciplot_end(MULTIGRAPHER);

   MULTIGRAPHER = NULL;
   */
   
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_subp(int nx, int ny)
{
   libsciplot_subp(MULTIGRAPHER, nx, ny); /* plotting window divided in nx.ny sub-windows */
   
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_erase(void)
{
   libsciplot_erase(MULTIGRAPHER);
   
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_flush(void)
{
   libsciplot_flush(MULTIGRAPHER);
   
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_setup_window(int ix, int iy, VEC *X, VEC *Y)
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k = (ix-1)*ny + (iy-1);

   double xmin, xmax, ymin, ymax;
   
   /* window dimensions evaluated  throught the vectors to plot the window size */
   libsciplot_ewin( MULTIGRAPHER, &xmin, &xmax, &ymin, &ymax, X->ve, Y->ve, X->dim );

   printf("libsciplot_ewin -> %f %f %f %f \n", xmin, xmax, ymin, ymax);

   /* and set them into MyPlotParams */
   pdata->dataplot[k]->xmin = xmin;
   pdata->dataplot[k]->xmax = xmax;
   pdata->dataplot[k]->ymin = ymin;
   pdata->dataplot[k]->ymax = ymax;

   /* end */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_libsciplot_plotcurve()
{
   int rc;

   /* set-up a new thread */
   pthread_t  thread;

   /* init data send to the new thread */
   SVQueue            * queue = svqueue_libsciplot_get_static();

   /* fill the queue */
   graphics1D_libsciplot_set_plotdata_in_queue();

   /* Create and run the new thread for the graphics */
   rc = pthread_create(&thread, NULL, graphics1D_libsciplot_curve_with_xt_toolkit, queue );
   if (rc)
   {
      printf("ERROR; return code from pthread_create() is %d\n", rc); return EXIT_FAILURE;
   }

   /* wait until the data is cloned */
   queue->xset(queue, NULL);

   /* finito */
   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int graphics1D_libsciplot_draw_all(void)
{
   LIBSCIPLOT_GRAPH_DATA* pdata = pdata_libsciplot_get_static();

   /*
    *  This "only" copies the new data into our structure "pdata_static"
    *
    * The real plot takes place when we call the function graphics1D_libsciplot_plotcurves_flush()
    */

   int nx = pdata->nx;
   int ny = pdata->ny;

   int k;

   for (k=0; k<nx*ny; k++)
   {
      if ( pdata->dataplot[k]->x != NULL ) pdata->dataplot[k]->draw( pdata->dataplot[k] );
   }

   return EXIT_SUCCESS;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void drawscene_libsciplot1D(const LIBSCIPLOT_CURVE_DATA *dataplot )
{
   LIBSCIPLOT_GRAPH_DATA * pdata = pdata_libsciplot_get_static();

   int bgcolor = pdata->bgcolor; 

   int dim = dataplot->x->dim;

   set_graph_parameters_default ( MULTIGRAPHER );

   libsciplot_bgcolor(MULTIGRAPHER, bgcolor);

   libsciplot_panl(MULTIGRAPHER, dataplot->ix, dataplot->iy);

   libsciplot_sch(MULTIGRAPHER, dataplot->labels_size/30.0);

   libsciplot_vstd(MULTIGRAPHER);

   if (dataplot->adjustscale == 1 )
   {
      graphics1D_libsciplot_setup_window(dataplot->ix, dataplot->iy, dataplot->x, dataplot->y);
   }
   
   libsciplot_swin(MULTIGRAPHER, dataplot->xmin, dataplot->xmax, dataplot->ymin, dataplot->ymax);

   /*
    *	Set box arounf the plot
    */
   if ( dataplot->withbox > 0 )
   {
      libsciplot_framecolor(MULTIGRAPHER, dataplot->frame_color);
      libsciplot_box(MULTIGRAPHER, "ABCNST", 0.0, 0, "ABCNST", 0.0, 0);
   }
   
   /*
    *   Set color
    */
   libsciplot_sci(MULTIGRAPHER, dataplot->legend_color);
   /*
    *	Set legend
    */
   libsciplot_lab(MULTIGRAPHER, dataplot->x_legend, dataplot->y_legend, dataplot->legend );

   /*
    *	Set color
    */
   libsciplot_sci(MULTIGRAPHER, dataplot->color);
   /*
    *	Draw the Graph
    */
   libsciplot_line(MULTIGRAPHER, dim, dataplot->x->ve, dataplot->y->ve);

   /*
    *	Draw the marker
    */
   if ( dataplot->end_marker >= 0 )
   {
      libsciplot_sci(MULTIGRAPHER, LIBSCIPLOTe_COLOR_RED);
      libsciplot_pt1(MULTIGRAPHER, dataplot->x->ve[dim-1], dataplot->y->ve[dim-1], dataplot->end_marker);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
