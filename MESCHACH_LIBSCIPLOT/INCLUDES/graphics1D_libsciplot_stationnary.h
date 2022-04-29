#ifndef GRAPHICS_1D_LIBSCIPLOT_STATIONNARY_H
#define GRAPHICS_1D_LIBSCIPLOT_STATIONNARY_H


#ifdef HAVE_X11
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#endif

#include "MESCHACH_LIBSCIPLOT/INCLUDES/graphics1D_libsciplot.h"

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

int  graphics1D_libsciplot_init_multigrapher(const char *driver, Display * display, Window *pwindow, int window_size_x, int window_size_y);

int  graphics1D_libsciplot_erase(void);
int  graphics1D_libsciplot_flush(void);
int  graphics1D_libsciplot_subp(int nx, int ny);

void graphics1D_libsciplot_initialize(const char *driver, int window_size_x, int window_size_y, int nx, int ny );
int  graphics1D_libsciplot_finalize(void);

int  graphics1D_libsciplot_plotcurve();
int  graphics1D_libsciplot_draw_all(void);

void graphics1D_libsciplot_set_plotdata_in_queue(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

#endif
