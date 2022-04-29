#ifndef MEF_GRAPHICS_OUTPUT_1D__H
#define MEF_GRAPHICS_OUTPUT_1D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file graphics_output_1D.h
 *
 * Contains all the output files functions
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"

/*
 *   1-Dim
 */

void graphics1D_output_vec  (const char *filename, const ELT_1D *elt, const GEOM_1D *geom, const VEC        *SOL);
void graphics1D_output_fun  (const char *filename, const ELT_1D *elt, const GEOM_1D *geom, const FUN_1D     *FUN);

void graphics1D     (const char* format, const ELT_1D *elt, const GEOM_1D *geom, const VEC    *SOL, const char *filename);
void graphics1D_fun (const char* format, const ELT_1D *elt, const GEOM_1D *geom, const FUN_1D *FUN, const char *filename);

#ifdef __cplusplus
}
#endif

#endif
