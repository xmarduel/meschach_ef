#ifndef GRAPHICS_1D_LIBSCIPLOT__H
#define GRAPHICS_1D_LIBSCIPLOT__H

/* \file graphics1D_libsciplot.h
 *
 * Declare common defines and enumerations
 * ----------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

typedef int libsciplot_color;
typedef int libsciplot_linestyle;
typedef int libsciplot_fillstyle;

#define END_CURVE_DEF  (-1)


typedef enum
{
   LIBSCIPLOTe_AQUA    ,  /* do not use this */ /*  {"aqua",              0x00, 0xff, 0xff},*/
   LIBSCIPLOTe_BLACK   ,                        /*  {"black",             0x00, 0x00, 0x00},*/
   LIBSCIPLOTe_BLUE    ,                        /*  {"blue",              0x00, 0x00, 0xff},*/
   LIBSCIPLOTe_FUSCHIA ,  /* do not use this */ /*  {"fuchsia",           0xff, 0x00, 0xff},*/
   LIBSCIPLOTe_GRAY    ,                        /*  {"gray",              0x80, 0x80, 0x80},*/
   LIBSCIPLOTe_GREEN   ,                        /*  {"green",             0x00, 0x80, 0x00},*/
   LIBSCIPLOTe_LIME    ,  /* do not use this */ /*  {"lime",              0x00, 0xff, 0x00},*/
   LIBSCIPLOTe_MAROON  ,                        /*  {"maroon",            0x80, 0x00, 0x00},*/
   LIBSCIPLOTe_NAVY    ,                        /*  {"navy",              0x00, 0x00, 0x80},*/
   LIBSCIPLOTe_OLIVE   ,  /* do not use this */ /*  {"olive",             0x80, 0x80, 0x00},*/
   LIBSCIPLOTe_PURPLE  ,                        /*  {"purple",            0x80, 0x00, 0x80},*/
   LIBSCIPLOTe_RED     ,                        /*  {"red",               0xff, 0x00, 0x00},*/
   LIBSCIPLOTe_SILVER  ,  /* do not use this */ /*  {"silver",            0xc0, 0xc0, 0xc0},*/
   LIBSCIPLOTe_TEAL    ,  /* do not use this */ /*  {"teal",              0x00, 0x80, 0x80},*/
   LIBSCIPLOTe_WHITE   ,                        /*  {"white",             0xff, 0xff, 0xff},*/
   LIBSCIPLOTe_YELLOW  ,                        /*  {"yellow",            0xff, 0xff, 0x00},*/

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
  LIBSCIPLOTe_NO_OUTPUT = 0,

   LIBSCIPLOTe_PPM = 1,
   LIBSCIPLOTe_GIF = 2,
   LIBSCIPLOTe_PPS = 3,

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

#define  LIBSCIPLOT_NB_MAX_SUBPLOTS  4

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct LIBSCIPLOT_CURVE_DATA_
{
   int ix,iy; /* panel */

   char   legend[256];
   char x_legend[256];
   char y_legend[256];

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
   int frame_on_top;
   int frame_color;
   int legend_color;

   /* draw itself */
   void (*draw)(const struct LIBSCIPLOT_CURVE_DATA_ * pdata);
   /* it has a default "draw", but we can change it */
   void (*set_drawfunc)(struct LIBSCIPLOT_CURVE_DATA_* pdata, void (*drawfunc)(const struct LIBSCIPLOT_CURVE_DATA_ *mystruct));

} LIBSCIPLOT_CURVE_DATA;

typedef void (*LIBSCIPLOT_DRAW_FUNC)(const LIBSCIPLOT_CURVE_DATA *mystruct);

LIBSCIPLOT_CURVE_DATA *libsciplot_curvedata_new  (int dim);
void                  *libsciplot_curvedata_free (LIBSCIPLOT_CURVE_DATA *in);
LIBSCIPLOT_CURVE_DATA *libsciplot_curvedata_clone(LIBSCIPLOT_CURVE_DATA *in);

void drawscene_libsciplot1D(const LIBSCIPLOT_CURVE_DATA *dataplot );

void set_drawfunc_libsciplot(LIBSCIPLOT_CURVE_DATA* q, LIBSCIPLOT_DRAW_FUNC drawfunc);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct LIBSCIPLOT_GRAPH_DATA_
{
   char driver[16];

   char title[64];
   int  bgcolor;

   int nx;
   int ny;

   int window_size_x;
   int window_size_y;

   /* contains all the datas to plot */
   LIBSCIPLOT_CURVE_DATA *dataplot[LIBSCIPLOT_NB_MAX_SUBPLOTS];

} LIBSCIPLOT_GRAPH_DATA;


LIBSCIPLOT_GRAPH_DATA *libsciplot_graphdata_new  (int nx, int ny);
void                  *libsciplot_graphdata_free (LIBSCIPLOT_GRAPH_DATA *in);
LIBSCIPLOT_GRAPH_DATA *libsciplot_graphdata_clone(LIBSCIPLOT_GRAPH_DATA *in);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* access the static stuff */
LIBSCIPLOT_GRAPH_DATA * pdata_libsciplot_get_static(void);
LIBSCIPLOT_GRAPH_DATA * pdata_libsciplot_set_static(LIBSCIPLOT_GRAPH_DATA *data);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef enum
{
   LIBSCIPLOTe_THREAD_POSIX  = 1,
   LIBSCIPLOTe_THREAD_PYTHON = 2

} LIBSCIPLOT_THREAD_TYPE;


int  graphics1D_libsciplot_set_thread_type(int type); /* used for python driver */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_libsciplot_title(const char *title);
int  graphics1D_libsciplot_bgcolor(const int bgcolor);

int  graphics1D_libsciplot_setbox( int ix , int iy ,  const int hasbox);
int  graphics1D_libsciplot_labelsize(int ix, int iy, double size);
int  graphics1D_libsciplot_legend( int ix , int iy ,  const char *legend);
int  graphics1D_libsciplot_endmarker( int ix, int iy, const int endmarker);
int  graphics1D_libsciplot_framecolor( int ix, int iy, const int framecolor);
int  graphics1D_libsciplot_legendcolor( int ix, int iy, const int legendcolor);
int  graphics1D_libsciplot_setup_window(int ix, int iy, VEC *X, VEC *Y);
int  graphics1D_libsciplot_window( int ix, int iy, double window_xmin , double window_xmax , double window_ymin , double window_ymax );

int  graphics1D_libsciplot_curvedata ( int ix, int iy, int color1, const VEC *X, const VEC *Y);
int  graphics1D_libsciplot_curvedata1( int ix, int iy, int color1, const VEC *X, const VEC *Y, int idx_plot_start, int idx_plot_end );

void graphics1D_libsciplot_set_plotdata_in_queue(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#endif
