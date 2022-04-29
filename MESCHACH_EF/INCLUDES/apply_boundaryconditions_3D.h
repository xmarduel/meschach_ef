#ifndef MEF_APPLY_BOUNDARY_CONDITIONS_3D__H
#define MEF_APPLY_BOUNDARY_CONDITIONS_3D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file apply_boundaryconditions_3D.h
 *
 * Contains all functions for applying BC on built matrices and rhs
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_3D.h"


void transform3D_matrix_vector_with_bc  (const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *bc , SPMAT *A, VEC *RHS);
void transform3D_matrix_with_bc         (const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *bc , SPMAT *A);
void transform3D_vector_with_bc         (const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *bc , const SPMAT *A, VEC *RHS);

void   transform3D_stokes_matrix_vector_with_bc      (const ELT_3D *elt, const GEOM_3D *geomU, const GEOM_3D *geomP, const BC_3D *bc, SPMAT *A, VEC *RHS);
VEC*   transform3D_stokes_vector_with_bc             (const ELT_3D *elt, const GEOM_3D *geomU, const GEOM_3D *geomP, const BC_3D *bc, SPMAT *A, VEC *RHS);
SPMAT* transform3D_stokes_matrix_with_bc             (const ELT_3D *elt, const GEOM_3D *geomU, const GEOM_3D *geomP, const BC_3D *bc, SPMAT *A);

void   transform3D_navierstokes_matrix_vector_with_bc(const ELT_3D *elt, const GEOM_3D *geomU, const GEOM_3D *geomP, const BC_3D *bc, SPMAT *A, VEC *RHS);
VEC*   transform3D_navierstokes_vector_with_bc       (const ELT_3D *elt, const GEOM_3D *geomU, const GEOM_3D *geomP, const BC_3D *bc, SPMAT *A, VEC *RHS);
SPMAT* transform3D_navierstokes_matrix_with_bc       (const ELT_3D *elt, const GEOM_3D *geomU, const GEOM_3D *geomP, const BC_3D *bc, SPMAT *A);

#ifdef __cplusplus
}
#endif

#endif
