
#ifndef GRAPHICS_1D_VOGLE_H
#define GRAPHICS_1D_VOGLE_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics1D_vogle.h
 *
 * Contains the routines for plotting with VOPL library
 *
 * ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_VOGLE/INCLUDES/graphics_vogle.h"


#define NB_MAX_SUBPLOTS 4

#define VOPL_CURVE    1
#define VOPL_CONTOURS 2

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

/* base struct for CURVE & CONTOURS - all common stuff is here - */

typedef struct VOPL_PLOT_DATA_
{
	int plot_type; /* VOPL_CURVE or VOPL_CONTOURS */

	int ix,iy; /* (sub)panel indices */

	double xmin, xmax; /* window */
   double ymin, ymax; /* window */

	char legend[64];

	double labels_size;
   int    font;

	/* drawing function */
	void (*draw)(void * pdata);
	/* it has a default "draw", but we can change it */
	void (*set_drawfunc)(void* pdata, void (*drawfunc)(void *plotdata));

} VOPL_PLOT_DATA;

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct VOPL_CURVE_DATA_
{
   int plot_type; /* VOPL_CURVE */

   int ix,iy; /* panel */

   double xmin, xmax; /* window */
   double ymin, ymax; /* window */

   char legend[64];

   double labels_size;
   int    font;

   /* draw itself */
   void (*draw)(const struct VOPL_CURVE_DATA_ * pdata);
   /* it has a default "draw", but we can change it */
   void (*set_drawfunc)(struct VOPL_CURVE_DATA_ * pdata, void (*drawfunc)(const struct VOPL_CURVE_DATA_ *curvedata));

   VEC * x;
   VEC * y;

   int color;

   int marker;
   int end_marker;

   int adjustscale;

   int withbox;

} VOPL_CURVE_DATA;

typedef void (*VOPL_CURVE_DRAW_FUNC)(const VOPL_CURVE_DATA *curvedata);

VOPL_CURVE_DATA *curvedata_new  (void);
void            *curvedata_free (VOPL_CURVE_DATA *in);
VOPL_CURVE_DATA *curvedata_clone(VOPL_CURVE_DATA *in);

void drawcurve_vopl1D(const VOPL_CURVE_DATA *dataplot );

void set_drawfunc_voplcurve(VOPL_CURVE_DATA* q, VOPL_CURVE_DRAW_FUNC drawfunc);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

struct VOPL_CONTOUR_DATA;
typedef void (*VOPL_CONTOUR_DRAW_FUNC)(struct VOPL_CONTOUR_DATA *pdata);

void delete_voplcontour_objects(void * data);
void setup_voplcontour_objects(void * data);


typedef struct VOPL_CONTOUR_DATA_
{
   int plot_type; /* VOPL_CONTOURS */

   int ix,iy; /* panel */

   double xmin, xmax; /* window */
   double ymin, ymax; /* window */

   char legend[64];

   double labels_size;
   int    font;

   /* drawing function */
   void (*draw)(const struct VOPL_CONTOUR_DATA_ * pdata);
   /* it has a default "draw", but we can change it */
   void (*set_drawfunc)(struct VOPL_CONTOUR_DATA_* pdata, void (*drawfunc)(const struct VOPL_CONTOUR_DATA_ *curvedata));

   /* the raw values */
   VEC  *VALUES;
   IMAT *NSELMT;
   MAT  *XYSOMM;
   /* the raw values */

   /* vogle objects id */
   int BORDER;
   int MESH;
   int CONTOURS;

   /* flags */
   int    show_border;
   int    show_mesh;
   int    show_contours;

   int mesh_color;
   int border_color;

   /* number of levels to draw */
   int nb_levels;
   /* and the levels */
   VEC *levels;

   /* governs which object to delete */
   int delete_border_object_flag;
   int delete_mesh_object_flag;
   int delete_contours_object_flag;

   /* governs which object to build */
   int build_border_object_flag;
   int build_mesh_object_flag;
   int build_contours_object_flag;

   /* for transient problem, store once for all the full mesh info */
   void *ucdmesh;

   /**/
   VOPL_CONTOUR_DRAW_FUNC setup_objects;
   VOPL_CONTOUR_DRAW_FUNC delete_objects;

} VOPL_CONTOUR_DATA;

VOPL_CONTOUR_DATA  *contourdata_new(void);
void               *contourdata_free (VOPL_CONTOUR_DATA *in);
VOPL_CONTOUR_DATA  *contourdata_clone(VOPL_CONTOUR_DATA *in);

void drawcontours_vopl1D(const VOPL_CONTOUR_DATA *pdata);

void set_drawfunc_voplcontours(VOPL_CONTOUR_DATA* q, VOPL_CONTOUR_DRAW_FUNC drawfunc);

/* ------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------ */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef struct VOPL_GRAPH_DATA_
{
   char driver[16];

   char title[64];

   int nx;
   int ny;

   int window_size_x;
   int window_size_y;

   /* contains all the datas to plot */
   VOPL_PLOT_DATA *dataplot[NB_MAX_SUBPLOTS]; /* curve or contours */

} VOPL_GRAPH_DATA;


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

void graphics1D_vopl_initialize(const char * driver, int window_size_x, int window_size_y, int nx, int ny, int vopl_plot_type);

char *graphics1D_vopl_title    (const char * title);

int  graphics1D_vopl_legend    (int ix, int iy, const char *legend);
int  graphics1D_vopl_window    (int ix, int iy, double window_xmin , double window_xmax , double window_ymin , double window_ymax );
int  graphics1D_vopl_labelsize (int ix, int iy, double size);
int  graphics1D_vopl_font      (int ix, int iy, int font_id);

/* -------------------------------------------------------------------------------------- */

int  graphics1D_vopl_endmarker  (int ix, int iy, int end_marker);  /* for VOPL_CURVE */
int  graphics1D_vopl_setbox     (int ix, int iy, int hasbox);      /* for VOPL_CURVE */
int  graphics1D_vopl_curvedata  (int ix, int iy, int color1, const VEC *X, const VEC *Y); /* for VOPL_CURVE */
int  graphics1D_vopl_curvedata1 (int ix, int iy, int color1, const VEC *X, const VEC *Y, int idx1, int idx2); /* for VOPL_CURVE */

/* -------------------------------------------------------------------------------------- */

int  graphics1D_vopl_nblevels   (int ix, int iy, int nb_levels); /* for VOPL_CONTOURS */
int  graphics1D_vopl_levels     (int ix, int iy, VEC *levels); /* for VOPL_CONTOURS */
int  graphics1D_vopl_contourmeshdata(int ix, int iy, const IMAT *NSELMT, const MAT *XYSOMM); /* for VOPL_CONTOURS */
int  graphics1D_vopl_contourplotdata(int ix, int iy, const VEC *SOL); /* for VOPL_CONTOURS */

int  graphics1D_vopl_contourtransientbuildinfo(int ix, int iy); /* for VOPL_CONTOURS */

/* -------------------------------------------------------------------------------------- */

void graphics1D_vopl_set_plotdata_in_queue(void);

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

typedef enum
{
   VOPLe_THREAD_POSIX  = 1,
   VOPLe_THREAD_PYTHON = 2

} VOPL_THREAD_TYPE;


int  graphics1D_vopl_set_thread_type(int type); /* used for python driver */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/


#ifdef __cplusplus
}
#endif


#endif


/*! \struct VOPL_CURVE_DATA
* \brief the structure embedded in the VOPL_GRAPH_DATA structure
*
* contains the data to be plotted + attributes how the datas are plotted (vopl "draw" function)
*/

/*! \struct VOPL_CONTOURS_DATA
 * \brief the structure embedded in the VOPL_GRAPH_DATA structure
 *
 * contains the data to be plotted + attributes how the datas are plotted (vopl "draw" function)
 */

/*! \struct VOPL_GRAPH_DATA
* \brief the structure embedded in the SVQueue structure
*
* Store all the datas for plotting in the "plotting" thread
*/

/*! \fn graphics1D_vopl_curvedata1(int ix, int iy, int color1, const VEC *X1, const VEC *Y1 , int idx_plot_start, int idx_plot_end );
*  \brief call to the drawing routine for transient problems
*  \param ix : sub-panel idx
*  \param iy : sub-panel idx
*  \param color1     : draw attribute: the color
*  \param X1 : the datas to be plot
*  \param Y1 : the datas to be plot
*  \param idx_plot_start : the first index of the part of the vector that will be drawned
*  \param idx_plot_end   : the last  index of the part of the vector that will be drawned
*
*  The function update the structure THREAD_SYNCHRONIZATION_STRUC and thus  must be called after
*  ini_thread_synchro_datas() and between graphics1D_vopl_plotcurves_start() and
*  graphics1D_vopl_plotcurves_flush().
*/
