
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_CPGPLOT-SWIG
//
/////////////////////////////////////////////////////////
 
#ifndef CPGPLOT_WRAPPER_H
#define CPGPLOT_WRAPPER_H 

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


/////////////////////////////////////////////////////////

SVQueue * svqueue_pgplot_get_static(void);
SVQueue * svqueue_pgplot_set_static(SVQueue* queue);

void graphics1D_cpgplot_set_plotdata_in_queue (void);

void graphics1D_cpgplot_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny);
void graphics1D_cpgplot_finalize(void);

int  graphics1D_cpgplot_plotcurve();

void graphics1D_cpgplot_plotcurves_start(void);
void graphics1D_cpgplot_plotcurves_flush(void);
void graphics1D_cpgplot_finalize_transient(void);



char *graphics1D_cpgplot_title(const char *title);

int graphics1D_cpgplot_window     ( int ix, int iy, double window_xmin , double window_xmax , double window_ymin , double window_ymax );
int graphics1D_cpgplot_setbox     ( int ix, int iy, const int hasbox);
int graphics1D_cpgplot_font       ( int ix, int iy, const int font_id);
int graphics1D_cpgplot_legend     ( int ix, int iy, const char *legend);
int graphics1D_cpgplot_labelsize  ( int ix, int iy, const double size);
int graphics1D_cpgplot_endmarker  ( int ix, int iy, const int endmarker);
int graphics1D_cpgplot_curvedata  ( int ix, int iy, int color1, const VEC *X, const VEC *Y);
int graphics1D_cpgplot_curvedata1 ( int ix, int iy, int color1, const VEC *X, const VEC *Y, int idx_plot_start, int idx_plot_end );
                                     



// a function to be called from python in a PYTHON thread

%inline %{

void Py_pgplot_curve_with_xt_toolkit(void)
{
   /* set-up the queue with the data to plot */
   graphics1D_cpgplot_set_plotdata_in_queue();
        
   Py_BEGIN_ALLOW_THREADS
        
   graphics1D_cpgplot_set_thread_type(PGPLOTe_THREAD_PYTHON);
   /* call the plotting stuff ( Xt window ) */
   pgplot_curve_with_xt_toolkit( svqueue_pgplot_get_static() );
    
   Py_END_ALLOW_THREADS
        
   /* return only when killing the Xt window */
   return; 
}

%}

#endif
