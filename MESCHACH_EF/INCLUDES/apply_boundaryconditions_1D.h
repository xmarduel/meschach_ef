#ifndef MEF_APPLY_BOUNDARY_CONDITIONS_1D__H
#define MEF_APPLY_BOUNDARY_CONDITIONS_1D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file apply_boundaryconditions_1D.h
 *
 * Contains all functions for applying BC on built matrices and rhs
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_1D.h"

   
void transform1D_matrix_vector_with_bc(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A, VEC *RHS);
void transform1D_matrix_with_bc       (const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A);
void transform1D_vector_with_bc       (const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, const SPMAT *A, VEC *RHS);


#ifdef __cplusplus
}
#endif


#endif


