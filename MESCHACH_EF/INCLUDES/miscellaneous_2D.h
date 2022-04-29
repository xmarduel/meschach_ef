
#ifndef MEF_MISCELLANEOUS_2D_H
#define MEF_MISCELLANEOUS_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file miscellaneous_2D.h
 *
 * ----------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"

/* ------------------------------------------------------------------------ */

VEC *build_vec_from_function2D         (ELT_2D *elt, GEOM_2D *geom, FUN_2D* fun, VEC *vec);
VEC *build_vec_from_function2Dtransient(ELT_2D *elt, GEOM_2D *geom, FUN_2D* fun, VEC *vec, Real tps);

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
