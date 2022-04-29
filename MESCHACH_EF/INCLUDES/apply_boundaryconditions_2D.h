#ifndef MEF_APPLY_BOUNDARY_CONDITIONS_2D__H
#define MEF_APPLY_BOUNDARY_CONDITIONS_2D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file apply_boundaryconditions_2D.h
 *
 * Contains all functions for applying BC on built matrices and rhs
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_2D.h"


void transform2D_matrix_vector_with_bc(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A, VEC *RHS);
void transform2D_matrix_with_bc       (const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A);
void transform2D_vector_with_bc       (const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, const SPMAT *A, VEC *RHS);


void   transform2D_stokes_matrix_vector_with_bc      (const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *BC, SPMAT *A, VEC *RHS);
VEC*   transform2D_stokes_vector_with_bc             (const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *BC, SPMAT *A, VEC *RHS);
SPMAT* transform2D_stokes_matrix_with_bc             (const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *BC, SPMAT *A);

void   transform2D_navierstokes_matrix_vector_with_bc(const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *bc, SPMAT *A, VEC *RHS);
VEC*   transform2D_navierstokes_vector_with_bc       (const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *bc, SPMAT *A, VEC *RHS);
SPMAT* transform2D_navierstokes_matrix_with_bc       (const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *bc, SPMAT *A);

#ifdef __cplusplus
}
#endif

#endif
