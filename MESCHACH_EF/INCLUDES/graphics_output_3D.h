#ifndef MEF_GRAPHICS_OUTPUT_3D__H
#define MEF_GRAPHICS_OUTPUT_3D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_output_3D.h
 *
 * Contains all the output files functions\n
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"


/*
 *   3-Dim
 */

void graphics3D       (const char* format, const ELT_3D *elt, const GEOM_3D *Geom, const VEC    *SOL, const char *name_file);
void graphics3D_fun   (const char* format, const ELT_3D *elt, const GEOM_3D *Geom, const FUN_3D *FUN, const char *name_file);
/* for stokes */
void graphics3D_stokes    (const char* format, const ELT_3D *elt, const GEOM_3D *geom, const VEC     *u, const VEC     *v, const VEC     *w, const VEC     *p, const char *name_file);
void graphics3D_stokes_fun(const char* format, const ELT_3D *elt, const GEOM_3D *geom, const FUN_3D *Fu, const FUN_3D *Fv, const FUN_3D *Fw, const FUN_3D *Fp, const char *name_file);

#ifdef __cplusplus
}
#endif

#endif
