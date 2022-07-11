
#ifndef GRAPHICS_2D_VOGLE_H
#define GRAPHICS_2D_VOGLE_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics2D_vogle.h
 *
 * Contains the routines for plotting with VOGLE library
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle.h"

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

typedef struct _VOGLE_SURFACE_MOTION_ACTION
{
   int MOTION_X_ROTATE;
   int MOTION_Y_ROTATE;

   int MOTION_X_TRANSLATE;
   int MOTION_Y_TRANSLATE;

   double MOTION_SCALE_FACTOR;

} VOGLE_SURFACE_MOTION_ACTION;


/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

typedef struct _VOGLE_SURFACE_VIEW
{
   double SCAL ;

   double TX;
   double TY;
   double TZ;

   double ROTX;
   double ROTY;

   /* to center to rotation axe in the middle of the surface */
   double BB_XMIN ; double BB_XMAX ;
   double BB_YMIN ; double BB_YMAX ;
   double BB_ZMIN ; double BB_ZMAX ;

} VOGLE_SURFACE_VIEW;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef void (*VOGLE_DRAW_FUNC)(void /*VOGLE_SURFACE_DATA*/*mystruct);

typedef struct VOGLE_SURFACE_DATA_
{
   char driver[64];

   int window_size_x;
   int window_size_y;

   char title[64];

   /* the raw values */
   VEC  *VALUES;
   IMAT *NSELMT;
   MAT  *XYSOMM;
   /* the raw values */

   /* vogle data properties */
   int surface_color;
   double labels_size;

   /* vogle motion attr. */
   VOGLE_SURFACE_MOTION_ACTION motion;

   /* vogle view attr. */
   VOGLE_SURFACE_VIEW view;

   /* vogle objects id */
   int SURFACE;
   int BOUNDINGBOX;
   int MINIAXES;

   /* flags */
   int    show_backface;
   int    show_boundingbox;
   int    show_miniref;

   /* drawing function */
   VOGLE_DRAW_FUNC draw;
   /* it has a default "draw", but we can change it */
   void (*set_drawfunc)(struct VOGLE_SURFACE_DATA_* pdata, VOGLE_DRAW_FUNC drawfunc);

   /**/
   VOGLE_DRAW_FUNC setup_view;
   VOGLE_DRAW_FUNC setup_objects;
   VOGLE_DRAW_FUNC delete_objects;

} VOGLE_SURFACE_DATA;

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

VOGLE_SURFACE_DATA *surfacedata_new();
void               *surfacedata_free (VOGLE_SURFACE_DATA *in);
VOGLE_SURFACE_DATA *surfacedata_clone(VOGLE_SURFACE_DATA *in);

void set_drawfunc_vogle(VOGLE_SURFACE_DATA* q, VOGLE_DRAW_FUNC drawfunc);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/* access the static stuff */
VOGLE_SURFACE_DATA * pdata_vogle_get_static(void);
VOGLE_SURFACE_DATA * pdata_vogle_set_static(VOGLE_SURFACE_DATA *data);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

void graphics2D_vogle_initialize(char *driver, int window_size_x, int window_size_y);

char *graphics2D_vogle_title(const char *title);

int  graphics2D_vogle_store_surfacedata(const IMAT *NSELMT, const MAT *XYSOMM);
int  graphics2D_vogle_store_plotdata(const VEC *U_o);

/* ------------------------------------------------------------------------------------ */

void graphics2D_vogle_set_plotdata_in_queue(void);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
