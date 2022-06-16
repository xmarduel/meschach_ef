
#ifndef MEF_MISCELLANEOUS_3D_H
#define MEF_MISCELLANEOUS_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file miscellaneous_3D.h
 * 
 * ----------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
   
#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"

/* ------------------------------------------------------------------------ */

VEC *build_vec_ef_from_function3D         (const ELT_3D *elt, const GEOM_3D *geom, const FUN_3D* fun, VEC *vec);
VEC *build_vec_ef_from_function3Dtransient(const ELT_3D *elt, const GEOM_3D *geom, const FUN_3D* fun, VEC *vec, Real tps);

/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif
