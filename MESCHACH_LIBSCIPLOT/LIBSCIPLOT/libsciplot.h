#ifndef LIBSCIPLOT__H
#define LIBSCIPLOT__H

#include "libcommon.h"
#include "plotter.h"


#define LIBPLOT_AQUA    0 /* do not use this */
#define LIBPLOT_BLACK   1
#define LIBPLOT_BLUE    2
#define LIBPLOT_FUSCHIA 3 /* do not use this */
#define LIBPLOT_GRAY    4
#define LIBPLOT_GREEN   5
#define LIBPLOT_LIME    6 /* do not use this */
#define LIBPLOT_MAROON  7
#define LIBPLOT_NAVY    8
#define LIBPLOT_OLIVE   9  /* do not use this */
#define LIBPLOT_PURPLE  10
#define LIBPLOT_RED     11
#define LIBPLOT_SILVER  12 /* do not use this */
#define LIBPLOT_TEAL    13 /* do not use this */
#define LIBPLOT_WHITE   14
#define LIBPLOT_YELLOW  15


#define LIBPLOT_COLOR_BLACK   -1
#define LIBPLOT_COLOR_WHITE    0
#define LIBPLOT_COLOR_RED      1
#define LIBPLOT_COLOR_GREEN    2
#define LIBPLOT_COLOR_BLUE     3
#define LIBPLOT_COLOR_MAGENTA  4
#define LIBPLOT_COLOR_CYAN     5

#define LIBPLOT_LINEMODE_SOLID          0
#define LIBPLOT_LINEMODE_DOTTED         1
#define LIBPLOT_LINEMODE_DOTDASHED      2
#define LIBPLOT_LINEMODE_SHORTDASHED    3
#define LIBPLOT_LINEMODE_LONGDASHED     4

#define LIBPLOT_FILLSTYLE_EMPTY         0
#define LIBPLOT_FILLSTYLE_SOLID         1
#define LIBPLOT_FILLSTYLE_OUTLINE       2
#define LIBPLOT_FILLSTYLE_HATCHED       3
#define LIBPLOT_FILLSTYLE_CROSS_HATCHED 4

#define LIBPLOT_SYMBOL_VOID                            0
#define LIBPLOT_SYMBOL_DOT                             1
#define LIBPLOT_SYMBOL_PLUS                            2
#define LIBPLOT_SYMBOL_ASTERIX                         3
#define LIBPLOT_SYMBOL_CIRCLE                          4
#define LIBPLOT_SYMBOL_CROSS                           5
#define LIBPLOT_SYMBOL_SQUARE                          6
#define LIBPLOT_SYMBOL_TRIANGLE                        7
#define LIBPLOT_SYMBOL_DIAMOND                         8
#define LIBPLOT_SYMBOL_STAR                            9
#define LIBPLOT_SYMBOL_INVERTED_TRIANGLE              10
#define LIBPLOT_SYMBOL_STARBURST                      11
#define LIBPLOT_SYMBOL_FANCY_PLUS                     12
#define LIBPLOT_SYMBOL_FANCE_CROSS                    13
#define LIBPLOT_SYMBOL_FANCY_SQUARE                   14
#define LIBPLOT_SYMBOL_FANCY_DIAMOND                  15
#define LIBPLOT_SYMBOL_FILLED_CIRCLE                  16
#define LIBPLOT_SYMBOL_FILLED_SQUARE                  17
#define LIBPLOT_SYMBOL_FILLED_TRIANGLE                18
#define LIBPLOT_SYMBOL_FILLED_DIAMOND                 19
#define LIBPLOT_SYMBOL_FILLED_INVERTED_TRIANGLE       20
#define LIBPLOT_SYMBOL_FILLED_FANCY_SQUARE            21
#define LIBPLOT_SYMBOL_FILLED_FANCY_DIAMOND           22
#define LIBPLOT_SYMBOL_HALF_FILLED_CIRCLE             23
#define LIBPLOT_SYMBOL_HALF_FILLED_SQUARE             24
#define LIBPLOT_SYMBOL_HALF_FILLED_TRIANGLE           25
#define LIBPLOT_SYMBOL_HALF_FILLED_DIAMOND            26
#define LIBPLOT_SYMBOL_HALF_FILLED_INVERTED_TRIANGLE  27
#define LIBPLOT_SYMBOL_HALF_FILLED_FANCY_SQUARE       28
#define LIBPLOT_SYMBOL_HALF_FILLED_FANCY_DIAMOND      29
#define LIBPLOT_SYMBOL_OCTAGON                        30
#define LIBPLOT_SYMBOL_FILLED_OCTAGON                 31

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

Multigrapher*  libsciplot_open(char *display_type, void* display, void *pwindow, char *bg_color, char *bitmap_size);

/* PGPLOT similar functions style */

void libsciplot_env         (Multigrapher* multigrapher, double  xmin, double  xmax, double  ymin, double  ymax, int justif, int axis_style);
void libsciplot_swin        (Multigrapher* multigrapher, double  xmin, double  xmax, double  ymin, double  ymax);
void libsciplot_ewin        (Multigrapher* multigrapher, double *xmin, double *xmax, double *ymin, double *ymax, double *vec_x, double *vec_y, int no_of_points);
void libsciplot_svp         (Multigrapher* multigrapher, double  xmin, double  xmax, double  ymin, double  ymax);
void libsciplot_sci         (Multigrapher* multigrapher, int color);
void libsciplot_sls         (Multigrapher* multigrapher, int line_style);
void libsciplot_sfs         (Multigrapher* multigrapher, int fill_style);
void libsciplot_rect        (Multigrapher* multigrapher, double xmin, double xmax, double ymin, double ymax);
void libsciplot_text        (Multigrapher* multigrapher, double xmin, double ymin, char *label);
void libsciplot_curs        (Multigrapher* multigrapher, double *x, double  *y, char *ch);            /* TODO */
void libsciplot_page        (Multigrapher* multigrapher);                                             /* TODO */
void libsciplot_end         (Multigrapher* multigrapher);
void libsciplot_pap         (Multigrapher* multigrapher, int size_window_x, int size_window_y);       /* TODO */
void libsciplot_ask         (Multigrapher* multigrapher, int ask);                                    /* TODO */
void libsciplot_checkcursor (Multigrapher* multigrapher);                                             /* TODO */
void libsciplot_sch         (Multigrapher* multigrapher, double size_label);
void libsciplot_reset       (Multigrapher* multigrapher);
void libsciplot_subp        (Multigrapher* multigrapher, int nx, int ny);
void libsciplot_panl        (Multigrapher* multigrapher, int i,int j);
void libsciplot_vstd        (Multigrapher* multigrapher);
void libsciplot_box         (Multigrapher* multigrapher, const char *xstyle, double x, int xo, const char *ystyle, double y, int yo);
void libsciplot_framecolor  (Multigrapher* multigrapher, int color);
void libsciplot_titlefont   (Multigrapher* multigrapher, const char * fontname);
void libsciplot_axisfont    (Multigrapher* multigrapher, const char * fontname);
void libsciplot_axisfont    (Multigrapher* multigrapher, const char * fontname);
void libsciplot_lab         (Multigrapher* multigrapher, const char *xlabel, const char *ylabel, const char *legend );
void libsciplot_eras        (Multigrapher* multigrapher);
void libsciplot_pt1         (Multigrapher* multigrapher, double x, double y, int marker);
void libsciplot_line        (Multigrapher* multigrapher, int len, double *x, double *y);
void libsciplot_pt          (Multigrapher* multigrapher, int len, double *x, double *y, int marker );
void libsciplot_erase       (Multigrapher* multigrapher);
void libsciplot_flush       (Multigrapher* multigrapher);

void libsciplot_bgcolor  (Multigrapher* multigrapher, int color );

Point* libsciplot_fillpoints_for_array_bounds_eval(double *x, double *y, int len);
Point* libsciplot_fillpoints_for_lines    (double *x, double *y, int len, int linemode);
Point* libsciplot_fillpoints_for_points   (double *x, double *y, int len, int symbol, int linemode);
Point* libsciplot_fillpoints_for_rectangle(double x1, double x2, double y1, double y2);

#endif
