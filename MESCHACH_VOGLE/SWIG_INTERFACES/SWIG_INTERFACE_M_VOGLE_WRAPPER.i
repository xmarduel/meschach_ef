
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_VOGLE
//
///////////////////////////////////////////////////////// 
 
#ifndef VOGLE_WRAPPER_H
#define VOGLE_WRAPPER_H 


typedef int vogle_color;

typedef enum
{
   VOGLEe_COLOR_BLACK           =  0 , 
   VOGLEe_COLOR_RED             =  1 , /* default */
   VOGLEe_COLOR_GREEN           =  2 ,
   VOGLEe_COLOR_YELLOW          =  3 ,
   VOGLEe_COLOR_BLUE            =  4 ,
   VOGLEe_COLOR_MAGENTA         =  5 ,
   VOGLEe_COLOR_CYAN            =  6 ,
   VOGLEe_COLOR_WHITE           =  7 ,
   
   VOGLE_NB_COLORS              =  8

} VOGLE_COLOR;


typedef enum
{
   VOGLEe_LINE_STYLE_EMPTY              =  0 , /* no meaning */
   VOGLEe_LINE_STYLE_SOLID              =  1 ,
   VOGLEe_LINE_STYLE_LONG_DASHED        =  2 ,
   VOGLEe_LINE_STYLE_DASHED_DOT         =  3 ,
   VOGLEe_LINE_STYLE_DOTTED             =  4 ,
   VOGLEe_LINE_STYLE_DASHED_DOT_DOT_DOT =  5 ,

   VOGLE_NB_LINE_STYLES =  6

} VOGLE_LINE_STYLE;


typedef enum
{
   VOGLEe_FILL_STYLE_EMPTY         =  0 ,
   VOGLEe_FILL_STYLE_SOLID         =  1 ,
   VOGLEe_FILL_STYLE_OUTLINE       =  2 ,
   VOGLEe_FILL_STYLE_HATCHED       =  3 ,
   VOGLEe_FILL_STYLE_CROSS_HATCHED =  4 ,
   
   VOGLE_NB_FILL_STYLES =  5

} VOGLE_FILL_STYLE;


typedef enum
{
   VOGLEe_NO_MARKER            =  0 ,

   VOGLEe_MARKER_CIRCLE        =  1 ,
   VOGLEe_MARKER_CUBE          =  2 ,
   VOGLEe_MARKER_TRIANGLE      =  3 ,
   VOGLEe_MARKER_LOSANGE       =  4 ,
   VOGLEe_MARKER_CHRISMAS      =  5 ,
   VOGLEe_MARKER_CROSS         =  6 ,
   VOGLEe_MARKER_ADDITION      =  7 ,
   VOGLEe_MARKER_MULTIPICATION =  8 ,
   VOGLEe_MARKER_STAR          =  9 ,
   
   VOGLEe_NB_MARKERS           = 10

} VOGLE_MARKER;



typedef enum
{
   VOGLEe_NO_OUTPUT  = 0,
   VOGLEe_PPM  = 1,
   VOGLEe_GIF  = 2,
   VOGLEe_TIFF = 3,
   VOGLEe_XBM  = 4,
	VOGLEe_PNG  = 5,
   VOGLEe_PPS  = 6

} VOGLE_OUTPUT_FORMAT;


typedef enum
{
   VOGLEe_FONT_ASTROLOGY    ,
   VOGLEe_FONT_CURSIVE      ,
   VOGLEe_FONT_FUTURA_L     ,
   VOGLEe_FONT_FUTURA_M     ,
   VOGLEe_FONT_GOTHIC_ENG   ,
   VOGLEe_FONT_GOTHIC_GER   ,
   VOGLEe_FONT_GOTHIC_ITA   ,
   VOGLEe_FONT_GREEK        ,
   VOGLEe_FONT_JAPANESE     ,
   VOGLEe_FONT_MARKERS      ,
   VOGLEe_FONT_MATH_LOW     ,
   VOGLEe_FONT_MATH_UPP     ,
   VOGLEe_FONT_METEOROLOGY  ,
   VOGLEe_FONT_MUSIC        ,
   VOGLEe_FONT_CYRILLIC     ,
   VOGLEe_FONT_SCRIPT       ,
   VOGLEe_FONT_SYMBOLIC     ,
   VOGLEe_FONT_TIMES_G      ,
   VOGLEe_FONT_TIMES_IB     ,
   VOGLEe_FONT_TIMES_I      ,
   VOGLEe_FONT_TIMES_R      ,
   VOGLEe_FONT_TIMES_RB     ,

} VOGLE_FONT;


#define  VOGLEd_IDX_FIRST  (-1)   /**< indicates the range of indices where a vector must be plotted */
#define  VOGLEd_IDX_LAST   (-1)   /**< indicates the range of indices where a vector must be plotted */

#define VOPL_CURVE    1
#define VOPL_CONTOURS 2

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_CURVE_DATA *curvedata_new  ();
void            *curvedata_free (VOPL_CURVE_DATA *in);
VOPL_CURVE_DATA *curvedata_clone(VOPL_CURVE_DATA *in);

void set_drawfunc_voplcurve(VOPL_CURVE_DATA* q, VOPL_CURVE_DRAW_FUNC drawfunc);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_CONTOUR_DATA *contourdata_new  ();
void              *contourdata_free (VOPL_CONTOUR_DATA *in);
VOPL_CONTOUR_DATA *contourdata_clone(VOPL_CONTOUR_DATA *in);

void set_drawfunc_voplcontours(VOPL_CONTOUR_DATA* q, VOPL_CONTOUR_DRAW_FUNC drawfunc);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOPL_GRAPH_DATA *vopl_graphdata_new  (int nx, int ny, int vopl_plot_type);
void            *vopl_graphdata_free (VOPL_GRAPH_DATA *in);
VOPL_GRAPH_DATA *vopl_graphdata_clone(VOPL_GRAPH_DATA *in);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* access the static stuff */
VOPL_GRAPH_DATA * pdata_vopl_get_static(void);
VOPL_GRAPH_DATA * pdata_vopl_set_static(VOPL_GRAPH_DATA *data);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_vopl_plotcurve();
int  graphics1D_vopl_plotcontour();

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void graphics1D_vopl_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny, int vopl_plot_type);

void graphics1D_vopl_plotcurves_start(void);
void graphics1D_vopl_plotcurves_flush(void);
void graphics1D_vopl_finalize_transient(void);

char *graphics1D_vopl_title   (const char *title);

int  graphics1D_vopl_font     (int ix, int iy, int font_id);
int  graphics1D_vopl_legend   (int ix, int iy, const char *legend);
int  graphics1D_vopl_labelsize(int ix, int iy, const double size);
int  graphics1D_vopl_window   (int ix, int iy, double window_xmin, double window_xmax, double window_ymin, double window_ymax );

int  graphics1D_vopl_setbox   (int ix, int iy, int hasbox);
int  graphics1D_vopl_endmarker(int ix, int iy, int endmarker);

int  graphics1D_vopl_curvedata  (int ix, int iy, int color1, const VEC *X, const VEC *Y);
int  graphics1D_vopl_curvedata1 (int ix, int iy, int color1, const VEC *X, const VEC *Y, int idx1, int idx2);

int  graphics1D_vopl_nblevels   (int ix, int iy, int nb_levels);
int  graphics1D_vopl_levels     (int ix, int iy, VEC *levels);
int  graphics1D_vopl_contourmeshdata(int ix, int iy, const IMAT *NSELMT, const MAT *XYSOMM);
int  graphics1D_vopl_contourplotdata(int ix, int iy, const VEC *SOL);
int  graphics1D_vopl_contourtransientbuildinfo(int ix, int iy);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

VOGLE_SURFACE_DATA *surfacedata_new  (int   surface_color  ,
                                      const VEC *VALUES    ,
                                      const IMAT *NSELMT   ,
                                      const MAT *XYSOMM);
                                      
void               *surfacedata_free (VOGLE_SURFACE_DATA *in);
VOGLE_SURFACE_DATA *surfacedata_clone(VOGLE_SURFACE_DATA *in);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

void graphics2D_vogle_initialize(const char *driver, int window_size_x, int window_size_y);

char *graphics2D_vogle_title(const char *title);

int  graphics2D_vogle_store_surfacedata(const IMAT *NSELMT, const MAT *XYSOMM);
int  graphics2D_vogle_store_plotdata(const VEC *SOL);
int  graphics2D_vogle_plotsurface();

void graphics2D_vogle_plotsurface_start(void);
void graphics2D_vogle_plotsurface_flush(void);
void graphics2D_vogle_finalize_transient(void);

void graphics1D_vopl_plotcurves_start(void);
void graphics1D_vopl_plotcurves_flush(void);

void graphics1D_vopl_plotcontours_start(void);
void graphics1D_vopl_plotcontours_flush(void);

void graphics1D_vopl_finalize_transient(void);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

// a function to be called from python in a PYTHON thread

%inline %{

void Py_vopl_curve_with_xt_toolkit(void)
{
        /* set-up the queue with the data to plot */
        graphics1D_vopl_set_plotdata_in_queue();
        
        Py_BEGIN_ALLOW_THREADS
        
        graphics1D_voplcurve_set_thread_type(VOPLe_THREAD_PYTHON);
        /* call the plotting stuff ( Xt window ) */
        vopl_curve_with_xt_toolkit( svqueue_vogle_get_static() );
    
        Py_END_ALLOW_THREADS
        
        /* return only when killing the Xt window */
        return; 
}
	
void Py_vopl_contour_with_xt_toolkit(void)
{
		/* set-up the queue with the data to plot */
		graphics1D_vopl_set_plotdata_in_queue();
		
		Py_BEGIN_ALLOW_THREADS
		
		graphics1D_voplcontour_set_thread_type(VOPLe_THREAD_PYTHON);
		/* call the plotting stuff ( Xt window ) */
		vopl_contour_with_xt_toolkit( svqueue_vogle_get_static() );
		
		Py_END_ALLOW_THREADS
		
		/* return only when killing the Xt window */
		return; 
}

void Py_vopl_contours_with_xt_toolkit_transient(void)
{
		/* set-up the queue with the data to plot */
		graphics1D_vopl_set_plotdata_in_queue();
		
		Py_BEGIN_ALLOW_THREADS
		
		graphics1D_voplcontour_set_thread_type(VOPLe_THREAD_PYTHON);
		/* call the plotting stuff ( Xt window ) */
		vopl_contours_with_xt_toolkit_transient( svqueue_vogle_get_static() );
		
		Py_END_ALLOW_THREADS
		
		/* return only when killing the Xt window */
		return; 
}
	
	
%}



#endif
