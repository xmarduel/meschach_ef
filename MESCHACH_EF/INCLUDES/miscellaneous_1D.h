
#ifndef MEF_MISCELLANEOUS_1D_H
#define MEF_MISCELLANEOUS_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file miscellaneous_2D.h
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/rhs_1D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"

/* ------------------------------------------------------------------------ */

VEC* build_vec_from_function1D          (const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D *dfun, VEC *vec);
VEC* build_vec_from_function1Dtransient (const ELT_1D *MyElt, const GEOM_1D *MyGeom, const FUN_1D *fun, const FUN_1D *dfun, VEC *vec, Real tps);

/* ------------------------------------------------------------------------ */

VEC* get_vector_for_plotting_1D (const ELT_1D *MyElt, const GEOM_1D *MyGeom, const VEC *vec);

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
