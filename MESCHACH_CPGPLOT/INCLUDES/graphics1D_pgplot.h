
#ifndef GRAPHICS_1D_PGPLOT_H
#define GRAPHICS_1D_PGPLOT_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_pgplot.h
 *
 * Contains the routines for plotting with PGPLOT library
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_ADDS/INCLUDES/queue.h"


#define PGPLOT_NB_MAX_SUBPLOTS 4

#define PGPLOT_PIXEL_TO_CENTIMETER(x) ((x)/(40*2.54))  /* machine dependent  */

#define PGPLOT_OUTPUT_FILE    "solution"

	typedef int cpgplot_color;


	typedef enum
	{
		PGPLOTe_COLOR_BLACK           =  0 ,
		PGPLOTe_COLOR_WHITE           =  1 , /* default */
		PGPLOTe_COLOR_RED             =  2 ,
		PGPLOTe_COLOR_GREEN           =  3 ,
		PGPLOTe_COLOR_BLUE1           =  4 ,
		PGPLOTe_COLOR_CYAN            =  5 ,
		PGPLOTe_COLOR_MAGENTA         =  6 ,
		PGPLOTe_COLOR_YELLOW          =  7 ,
		PGPLOTe_COLOR_ORANGE          =  8 ,
		PGPLOTe_COLOR_GREEN_YELLOW    =  9 ,
		PGPLOTe_COLOR_GREEN_CYAN      = 10 ,
		PGPLOTe_COLOR_BLUE_CYAN       = 11 ,
		PGPLOTe_COLOR_BLUE_MAGENTA    = 12 ,
		PGPLOTe_COLOR_RED_MAGEBTA     = 13 ,
		PGPLOTe_COLOR_DARK_GRAY       = 14 ,
		PGPLOTe_COLOR_LIGHT_GRAY      = 15 ,

		PGPLOT_NB_COLORS              = 16

	} PGPLOT_COLOR;


	typedef enum
	{
		PGPLOTe_LINE_STYLE_EMPTY              =  0 , /* no meaning */
		PGPLOTe_LINE_STYLE_SOLID              =  1 ,
		PGPLOTe_LINE_STYLE_LONG_DASHED        =  2 ,
		PGPLOTe_LINE_STYLE_DASHED_DOT         =  3 ,
		PGPLOTe_LINE_STYLE_DOTTED             =  4 ,
		PGPLOTe_LINE_STYLE_DASHED_DOT_DOT_DOT =  5 ,

		PGPLOT_NB_LINE_STYLES =  6

	} PGPLOT_LINE_STYLE;


	typedef enum
	{
		PGPLOTe_FILL_STYLE_EMPTY         =  0 ,
		PGPLOTe_FILL_STYLE_SOLID         =  1 ,
		PGPLOTe_FILL_STYLE_OUTLINE       =  2 ,
		PGPLOTe_FILL_STYLE_HATCHED       =  3 ,
		PGPLOTe_FILL_STYLE_CROSS_HATCHED =  4 ,

		PGPLOT_NB_FILL_STYLES =  5

	} PGPLOT_FILL_STYLE;


	typedef enum
	{
		PGPLOTe_MARKER_UNDEFINED        = -1 ,

		PGPLOTe_MARKER_CUBE1            =  0 ,
		PGPLOTe_MARKER_PIXEL            =  1 ,
		PGPLOTe_MARKER_ADDITION         =  2 ,
		PGPLOTe_MARKER_STAR             =  3 ,
		PGPLOTe_MARKER_CIRCLE           =  4 ,
		PGPLOTe_MARKER_MULTIPLICATION   =  5 ,
		PGPLOTe_MARKER_CUBE2            =  6 ,
		PGPLOTe_MARKER_TRIANGLE         =  7 ,
		PGPLOTe_MARKER_ADDITIONINCIRCLE =  8 ,
		PGPLOTe_MARKER_POINTINCIRCLE    =  9 ,

		PGPLOTe_MARKER_BURG             =  10 ,
		PGPLOTe_MARKER_LOSANGE          =  11 ,
		PGPLOTe_MARKER_CHRISMAS         =  12 ,
		PGPLOTe_MARKER_FILLEDTRIANGLE   =  13 ,
		PGPLOTe_MARKER_CIRCLE0          =  14 ,
		PGPLOTe_MARKER_DAVIDSTAR        =  15 ,
		PGPLOTe_MARKER_FILLEDCUBE       =  16 ,
		PGPLOTe_MARKER_FILLEDCICLE      =  17 ,
		PGPLOTe_MARKER_FILLEDCHRISMAS   =  18 ,
		PGPLOTe_MARKER_BIGCUBE          =  19 ,

		PGPLOTe_MARKER_CIRCLE1          =  20 ,
		PGPLOTe_MARKER_CIRCLE2          =  21 ,
		PGPLOTe_MARKER_CIRCLE3          =  22 ,
		PGPLOTe_MARKER_CIRCLE4          =  23 ,
		PGPLOTe_MARKER_CIRCLE5          =  24 ,
		PGPLOTe_MARKER_CIRCLE6          =  25 ,
		PGPLOTe_MARKER_CIRCLE7          =  26 ,
		PGPLOTe_MARKER_CIRCLE8          =  27 ,
		PGPLOTe_MARKER_ARROWLEFT        =  28 ,
		PGPLOTe_MARKER_ARROWRIGHT       =  29 ,

		PGPLOTe_MARKER_ARROWUP          =  30 ,
		PGPLOTe_MARKER_ARROWDOWN        =  31 ,

		PGPLOTe_NB_MARKERS              =  32

	} PGPLOT_MARKER;


	typedef enum
	{
		PGPLOTe_NO_OUTPUT  = 0,
		PGPLOTe_PPM  = 1,
		PGPLOTe_GIF  = 2,
		PGPLOTe_PPS  = 3

	} PGPLOT_OUTPUT_FORMAT;


#define  PGPLOTd_IDX_FIRST  (-1)   /**< indicates the range of indices where a vector must be plotted */
#define  PGPLOTd_IDX_LAST   (-1)   /**< indicates the range of indices where a vector must be plotted */


/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


typedef struct PGPLOT_CURVE_DATA_
{
   int ix,iy; /* panel */

   char   legend[64];
   char x_legend[64];
   char y_legend[64];

   VEC * x;
   VEC * y;

   int    font;
   double labels_size;

   int  color;

   int marker;
   int end_marker;

   int adjustscale;

   double xmin, xmax; /* window */
   double ymin, ymax; /* window */

   int withbox;

   /* draw itself */
   void (*draw)(const struct PGPLOT_CURVE_DATA_ * pdata);
   /* it has a default "draw", but we can change it */
   void (*set_drawfunc)(struct PGPLOT_CURVE_DATA_* pdata, void (*drawfunc)(const struct PGPLOT_CURVE_DATA_ * pdata)) ;

} PGPLOT_CURVE_DATA;

typedef void (*PGPLOT_DRAW_FUNC)(const PGPLOT_CURVE_DATA *mystruct);

PGPLOT_CURVE_DATA *pgplot_curvedata_new  (int dim);
void              *pgplot_curvedata_free (PGPLOT_CURVE_DATA *in);
PGPLOT_CURVE_DATA *pgplot_curvedata_clone(PGPLOT_CURVE_DATA *in);

void drawscene_pgplot1D(const PGPLOT_CURVE_DATA *dataplot );

void set_drawfunc_pgplot(PGPLOT_CURVE_DATA* q, PGPLOT_DRAW_FUNC drawfunc);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct PGPLOT_GRAPH_DATA_
{
   char driver[16];

   char title[64];

   int nx;
   int ny;

   int window_size_x;
   int window_size_y;

   /* contains all the datas to plot */
   PGPLOT_CURVE_DATA *dataplot[PGPLOT_NB_MAX_SUBPLOTS];

} PGPLOT_GRAPH_DATA;


PGPLOT_GRAPH_DATA *pgplot_graphdata_new  (int nx, int ny);
void              *pgplot_graphdata_free (PGPLOT_GRAPH_DATA *in);
PGPLOT_GRAPH_DATA *pgplot_graphdata_clone(PGPLOT_GRAPH_DATA *in);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* access the static stuff */
PGPLOT_GRAPH_DATA * pdata_pgplot_get_static(void);
PGPLOT_GRAPH_DATA * pdata_pgplot_set_static(PGPLOT_GRAPH_DATA *data);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_cpgplot_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny);

char *graphics1D_cpgplot_title    (const char *title);

int graphics1D_cpgplot_window     ( int ix, int iy, double window_xmin , double window_xmax , double window_ymin , double window_ymax );

int graphics1D_cpgplot_setbox     ( int ix, int iy, const int hasbox);

int graphics1D_cpgplot_font       ( int ix, int iy, const int  font_id);
int graphics1D_cpgplot_legend     ( int ix, int iy, const char *legend);
int graphics1D_cpgplot_labelsize  ( int ix, int iy, const double size);
int graphics1D_cpgplot_endmarker  ( int ix, int iy, const int marker);
int graphics1D_cpgplot_curvedata  ( int ix, int iy, int color1, const VEC *X, const VEC *Y);
int graphics1D_cpgplot_curvedata1 ( int ix, int iy, int color1, const VEC *X, const VEC *Y, int idx_plot_start, int idx_plot_end );

void graphics1D_cpgplot_set_plotdata_in_queue (void);
void graphics1D_cpgplot_finalize(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef enum
{
   PGPLOTe_THREAD_POSIX  = 1,
   PGPLOTe_THREAD_PYTHON = 2

} PGPLOT_THREAD_TYPE;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_cpgplot_set_thread_type(int type);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#ifdef __cplusplus
}
#endif


#endif

/*! \enum PGPLOT_COLOR
 * \brief
 *
 * list all the colors used in the PGPLOT library
 */

/*! \enum PGPLOT_LINE_STYLE
 * \brief
 *
 * list all the line-styles used in the PGPLOT library
 */

/*! \enum PGPLOT_FILL_STYLE
 * \brief
 *
 * list all the fill-styles used in the PGPLOT library
 */

/*! \enum PGPLOT_MARKER
 * \brief
 *
 * list all the marker-types used in the PGPLOT library
 */

/*! \fn char *graphics1D_cpgplot_title(const char *title)
*
*  \brief set in the static structure the data passed in arguments
*
*  \param title  : the "title" of the plot, used to name a hardcopy of the plot (gif, ppm, pps)
*/

/*! \fn int graphics1D_cpgplot_window( int ix,  int iy,
*                                      double window_xmin ,
*                                      double window_xmax ,
*                                      double window_ymin ,
*                                      double window_ymax )
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param window_xmin    : the "window" bounds of the plot
*  \param window_xmax    : the "window" bounds of the plot
*  \param window_ymin    : the "window" bounds of the plot
*  \param window_ymax    : the "window" bounds of the plot
*/

/*! \fn int graphics1D_cpgplot_setbox   ( int ix,  int iy, int hasbox       )
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param hasbox         : if the (sub)plot has the box or not (not used yet)
*/

/*! \fn int graphics1D_cpgplot_legend   ( int ix,  int iy, const char *legend)
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param legend         : the "legend" of the (sub)plot
*/

/*! \fn int graphics1D_cpgplot_labelsize( int ix,  int iy, const double size)
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param size           : the size of the legend of the (sub)plot
*/

/*! \fn int graphics1D_cpgplot_font( int ix,  int iy, const int font_id)
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param font_id        : the type of font used for the (sub)plot
*/

/*! \fn int graphics1D_cpgplot_endmarker( int ix,  int iy, const int marker)
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param marker         : type of the marker plotted at the end of the curve if > 0
*/

/*! \fn int  graphics1D_cpgplot_curvedata1( int ix, int iy, int color1 , const VEC *X, const VEC *Y,
*                                                int idx_plot_start   ,
*                                                int idx_plot_end     )
*
*  \brief set in the static structure the data passed in arguments
*
*  \param ix             : index of the sub-panel
*  \param iy             : index of the sub-panel
*  \param color1         : colot of the cuve to plot
*  \param X              : the vector to plot
*  \param Y              : the vector to plot
*  \param idx_plot_start : range of the indexes of the vectors X1 and Y1 to be plotted
*  \param idx_plot_end   : range of the indexes of the vectors X1 and Y1 to be plotted
*/

