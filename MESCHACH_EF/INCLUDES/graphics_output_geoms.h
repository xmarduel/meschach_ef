#ifndef MEF_GRAPHICS_OUTPUT_GEOMS__H
#define MEF_GRAPHICS_OUTPUT_GEOMS__H

#ifdef __cplusplus
extern "C"
{
#endif

/* 
 * Contains all the output files functions
 *
 */

#include "MESCHACH_EF/INCLUDES/geometry.h"


/*
 * geom view (with gnuplot or graph)
 */

void graphics_geom1D_view(const char *format, const GEOM_1D *geom, const char *name_file);
void graphics_geom2D_view(const char *format, const GEOM_2D *geom, const char *name_file);
void graphics_geom3D_view(const char *format, const GEOM_3D *geom, const char *name_file);

#ifdef __cplusplus
}
#endif

#endif
