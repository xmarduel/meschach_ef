
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////
 
#ifndef GRAPHICS_OUTPUT_H
#define GRAPHICS_OUTPUT_H

#include "finite_elements.h"
#include "geometry.h"
#include "boundary_conditions.h"
#include "functions_structs.h"


/*
 *   1-Dim
 */

void graphics1D_output_vec  (const char *filename, ELT_1D *element, GEOM_1D *Geom, VEC        *SOL );
void graphics1D_output_fun  (const char *filename, ELT_1D *element, GEOM_1D *Geom, FUN_1D     *FUN );

void graphics1D             (const char* format, ELT_1D *element , GEOM_1D *Geom , VEC    *SOL , const char *filename);
void graphics1D_fun         (const char* format, ELT_1D *element , GEOM_1D *Geom , FUN_1D *FUN , const char *filename);

/*
 *   2-Dim
 */

void graphics2D_set   (const char *format, const ELT_2D *element , const GEOM_2D *Geom , const int nb, const MAT *SOL , const char *filename);

void graphics2D       (const char* format, ELT_2D *element , GEOM_2D *Geom , VEC    *SOL , const char *filename);
void graphics2D_fun   (const char* format, ELT_2D *element , GEOM_2D *Geom , FUN_2D *FUN , const char *filename);
/* for stokes */
void graphics2D_stokes    (const char* format, ELT_2D *element , GEOM_2D *Geom , VEC     *u, VEC     *v, VEC     *p, const char *filename);
void graphics2D_stokes_fun(const char* format, ELT_2D *element , GEOM_2D *Geom , FUN_2D *Fu, FUN_2D *Fv, FUN_2D *Fp, const char *filename);

/*
 *   3-Dim
 */

void graphics3D       (const char* format, ELT_3D *element , GEOM_3D *Geom , VEC    *SOL , const char *filename);
void graphics3D_fun   (const char* format, ELT_3D *element , GEOM_3D *Geom , FUN_3D *FUN , const char *filename);
/* for stokes */
void graphics3D_stokes       (const char* format, ELT_3D *element , GEOM_3D *Geom , VEC     *u, VEC     *v, VEC     *w, VEC     *p, const char *filename);
void graphics3D_stokes_fun   (const char* format, ELT_3D *element , GEOM_3D *Geom , FUN_3D *Fu, FUN_3D *Fv, FUN_3D *Fw, FUN_3D *Fp, const char *filename);

/*
 * matrix patterns (with gnuplot or graph)
 */

void graphics_matrix_pattern(const char* format, SPMAT* A, const char* filename);

/*
 * geom view (with gnuplot or graph)
 */

void graphics_geom1D_view(const char *format, GEOM_1D * Geom, const char *filename);
void graphics_geom2D_view(const char *format, GEOM_2D * Geom, const char *filename);
void graphics_geom3D_view(const char *format, GEOM_3D * Geom, const char *filename);


#endif

