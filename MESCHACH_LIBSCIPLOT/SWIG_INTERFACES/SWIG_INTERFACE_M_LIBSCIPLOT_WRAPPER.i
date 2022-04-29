
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_LIBSCIPLOT-SWIG
//
///////////////////////////////////////////////////////// 

#ifndef LIBSCIPLOT_WRAPPER_H
#define LIBSCIPLOT_WRAPPER_H 

typedef int libsciplot_color_ext;
typedef int libsciplot_color;
typedef int libsciplot_linestyle;
typedef int libsciplot_fillstyle;

typedef enum
{
   LIBSCIPLOTe_AQUA    ,  /*  {"aqua",              0x00, 0xff, 0xff},*/
   LIBSCIPLOTe_BLACK   ,  /*  {"black",             0x00, 0x00, 0x00},*/
   LIBSCIPLOTe_BLUE    ,  /*  {"blue",              0x00, 0x00, 0xff},*/
   LIBSCIPLOTe_FUSCHIA ,  /*  {"fuchsia",           0xff, 0x00, 0xff},*/
   LIBSCIPLOTe_GRAY    ,  /*  {"gray",              0x80, 0x80, 0x80},*/
   LIBSCIPLOTe_GREEN   ,  /*  {"green",             0x00, 0x80, 0x00},*/
   LIBSCIPLOTe_LIME    ,  /*  {"lime",              0x00, 0xff, 0x00},*/
   LIBSCIPLOTe_MAROON  ,  /*  {"maroon",            0x80, 0x00, 0x00},*/
   LIBSCIPLOTe_NAVY    ,  /*  {"navy",              0x00, 0x00, 0x80},*/
   LIBSCIPLOTe_OLIVE   ,  /*  {"olive",             0x80, 0x80, 0x00},*/
   LIBSCIPLOTe_PURPLE  ,  /*  {"purple",            0x80, 0x00, 0x80},*/
   LIBSCIPLOTe_RED     ,  /*  {"red",               0xff, 0x00, 0x00},*/
   LIBSCIPLOTe_SILVER  ,  /*  {"silver",            0xc0, 0xc0, 0xc0},*/
   LIBSCIPLOTe_TEAL    ,  /*  {"teal",              0x00, 0x80, 0x80},*/
   LIBSCIPLOTe_WHITE   ,  /*  {"white",             0xff, 0xff, 0xff},*/
   LIBSCIPLOTe_YELLOW  ,  /*  {"yellow",            0xff, 0xff, 0x00},*/

} LIBSCIPLOT_COLOR_EXT;


typedef enum
{
   LIBSCIPLOTe_COLOR_BLACK   = -1,
   LIBSCIPLOTe_COLOR_WHITE   = 0,
   LIBSCIPLOTe_COLOR_RED     = 1,
   LIBSCIPLOTe_COLOR_GREEN   = 2,
   LIBSCIPLOTe_COLOR_BLUE    = 3,
   LIBSCIPLOTe_COLOR_MAGENTA = 4,
   LIBSCIPLOTe_COLOR_CYAN    = 5

} LIBSCIPLOT_COLOR;


typedef enum
{
   LIBSCIPLOTe_LINEMODE_SOLID       = 0,
   LIBSCIPLOTe_LINEMODE_DOTTED      = 1,
   LIBSCIPLOTe_LINEMODE_DOTDASHED   = 2,
   LIBSCIPLOTe_LINEMODE_SHORTDASHED = 3,
   LIBSCIPLOTe_LINEMODE_LONGDASHED  = 4

} LIBSCIPLOT_LINEMODE;


typedef enum
{
   LIBSCIPLOTe_FILLSTYLE_EMPTY         = 0,
   LIBSCIPLOTe_FILLSTYLE_SOLID         = 1,
   LIBSCIPLOTe_FILLSTYLE_OUTLINE       = 2,
   LIBSCIPLOTe_FILLSTYLE_HATCHED       = 3,
   LIBSCIPLOTe_FILLSTYLE_CROSS_HATCHED = 4

} LIBSCIPLOT_FILLSTYLE;



typedef enum
{
   LIBSCIPLOTe_PPM = 0,
   LIBSCIPLOTe_GIF = 1,
   LIBSCIPLOTe_PPS = 2

} LIBSCIPLOT_OUTPUT;


typedef enum
{
    LIBSCIPLOTe_SYMBOL_VOID                        =    0,
    LIBSCIPLOTe_SYMBOL_DOT                         =    1,
    LIBSCIPLOTe_SYMBOL_PLUS                        =    2,
    LIBSCIPLOTe_SYMBOL_ASTERIX                     =    3,
    LIBSCIPLOTe_SYMBOL_CIRCLE                      =    4,
    LIBSCIPLOTe_SYMBOL_CROSS                       =    5,
    LIBSCIPLOTe_SYMBOL_SQUARE                      =    6,
    LIBSCIPLOTe_SYMBOL_TRIANGLE                    =    7,
    LIBSCIPLOTe_SYMBOL_DIAMOND                     =    8,
    LIBSCIPLOTe_SYMBOL_STAR                        =    9,
    LIBSCIPLOTe_SYMBOL_INVERTED_TRIANGLE           =   10,
    LIBSCIPLOTe_SYMBOL_STARBURST                   =   11,
    LIBSCIPLOTe_SYMBOL_FANCY_PLUS                  =   12,
    LIBSCIPLOTe_SYMBOL_FANCY_CROSS                 =   13,
    LIBSCIPLOTe_SYMBOL_FANCY_SQUARE                =   14,
    LIBSCIPLOTe_SYMBOL_FANCY_DIAMOND               =   15,
    LIBSCIPLOTe_SYMBOL_FILLED_CIRCLE               =   16,
    LIBSCIPLOTe_SYMBOL_FILLED_SQUARE               =   17,
    LIBSCIPLOTe_SYMBOL_FILLED_TRIANGLE             =   18,
    LIBSCIPLOTe_SYMBOL_FILLED_DIAMOND              =   19,
    LIBSCIPLOTe_SYMBOL_FILLED_INVERTED_TRIANGLE    =   20,
    LIBSCIPLOTe_SYMBOL_FILLED_FANCY_SQUARE         =   21,
    LIBSCIPLOTe_SYMBOL_FILLED_FANCY_DIAMOND        =   22,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_CIRCLE          =   23,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_SQUARE          =   24,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_TRIANGLE        =   25,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_DIAMOND         =   26,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_INVERTED_TRIANGLE=  27,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_FANCY_SQUARE     =  28,
    LIBSCIPLOTe_SYMBOL_HALF_FILLED_FANCY_DIAMOND    =  29,
    LIBSCIPLOTe_SYMBOL_OCTAGON                      =  30,
    LIBSCIPLOTe_SYMBOL_FILLED_OCTAGON               =  31,

} LIBSCIPLOT_SYMBOL;  



#define  LIBSCIPLOTd_IDX_FIRST  (-1)   /**< indicates the range of indices where a vector must be plotted */
#define  LIBSCIPLOTd_IDX_LAST   (-1)   /**< indicates the range of indices where a vector must be plotted */


void graphics1D_libsciplot_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny);
int  graphics1D_libsciplot_finalize(void);

int  graphics1D_libsciplot_title(const char *title);
int  graphics1D_libsciplot_bgcolor(const int bgcolor);

int  graphics1D_libsciplot_setbox( int ix , int iy ,  const int hasbox);
int  graphics1D_libsciplot_labelsize(int ix, int iy, double size);
int  graphics1D_libsciplot_legend( int ix , int iy ,  const char *legend);
int  graphics1D_libsciplot_framecolor( int ix, int iy, const int framecolor);
int  graphics1D_libsciplot_legendcolor( int ix, int iy, const int legendcolor);
int  graphics1D_libsciplot_endmarker( int ix, int iy, const int endmarker);
int  graphics1D_libsciplot_setup_window(int ix, int iy, VEC *X, VEC *Y);
int  graphics1D_libsciplot_window( int ix, int iy, double window_xmin , double window_xmax , double window_ymin , double window_ymax );

int  graphics1D_libsciplot_curvedata ( int ix, int iy, int color1, const VEC *X, const VEC *Y);
int  graphics1D_libsciplot_curvedata1( int ix, int iy, int color1, const VEC *X, const VEC *Y, int idx_plot_start, int idx_plot_end );

int  graphics1D_libsciplot_plotcurve();
int  graphics1D_libsciplot_draw_all(void);


void graphics1D_libsciplot_plotcurves_start(void);
void graphics1D_libsciplot_plotcurves_flush(void);
void graphics1D_libsciplot_finalize_transient(void);


// a function to be called from python in a PYTHON thread

%inline %{

void Py_libsciplot_curve_with_xt_toolkit(void)
{
        /* set-up the queue with the data to plot */
        graphics1D_libsciplot_set_plotdata_in_queue();
        
        Py_BEGIN_ALLOW_THREADS
        
        graphics1D_libsciplot_set_thread_type(LIBSCIPLOTe_THREAD_PYTHON);
        /* call the plotting stuff ( Xt window ) */
        graphics1D_libsciplot_curve_with_xt_toolkit( svqueue_libsciplot_get_static() );
    
        Py_END_ALLOW_THREADS
        
        /* return only when killing the Xt window */
        return; 
}

%}

#endif
