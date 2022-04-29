
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////

#ifndef MEF_APPLY_BOUNDARY_CONDITIONS_1D__H
#define MEF_APPLY_BOUNDARY_CONDITIONS_1D__H


#include "matrix.h"
#include "sparse.h"

#include "finite_elements_1D.h"
#include "geometry_1D.h"
#include "boundary_conditions_1D.h"

void transform1D_matrix_vector_with_bc( ELT_1D *elt , GEOM_1D *geom , BC_1D *BC , SPMAT *A , VEC *RHS );
void transform1D_vector_with_bc       ( ELT_1D *elt , GEOM_1D *geom , BC_1D *BC , SPMAT *A , VEC *RHS );
void transform1D_matrix_with_bc       ( ELT_1D *elt , GEOM_1D *geom , BC_1D *BC , SPMAT *A );


#endif

/*------------------------------------------------------------------------------------------------*/

#ifndef MEF_APPLY_BOUNDARY_CONDITIONS_2D__H
#define MEF_APPLY_BOUNDARY_CONDITIONS_2D__H


#include "matrix.h"
#include "sparse.h"

#include "finite_elements_2D.h"
#include "geometry_2D.h"
#include "boundary_conditions_2D.h"

void transform2D_matrix_vector_with_bc( ELT_2D *elt , GEOM_2D *geom , BC_2D *BC , SPMAT *A , VEC *RHS ); 
void transform2D_vector_with_bc       ( ELT_2D *elt , GEOM_2D *geom , BC_2D *BC , SPMAT *A , VEC *RHS );
void transform2D_matrix_with_bc       ( ELT_2D *elt , GEOM_2D *geom , BC_2D *BC , SPMAT *A );

void   transform2D_stokes_matrix_vector_with_bc      ( ELT_2D *elt , GEOM_2D *geomU , GEOM_2D *geomP, BC_2D *BC, SPMAT *A, VEC *RHS);
VEC*   transform2D_stokes_vector_with_bc             ( ELT_2D *elt , GEOM_2D *geomU , GEOM_2D *geomP, BC_2D *BC, SPMAT *A, VEC *RHS);
SPMAT* transform2D_stokes_matrix_with_bc             ( ELT_2D *elt , GEOM_2D *geomU , GEOM_2D *geomP, BC_2D *BC, SPMAT *A);

void   transform2D_navierstokes_matrix_vector_with_bc( ELT_2D *elt , GEOM_2D *geomU , GEOM_2D *geomP, BC_2D *BC, SPMAT *A, VEC *RHS);
VEC*   transform2D_navierstokes_vector_with_bc       ( ELT_2D *elt , GEOM_2D *geomU , GEOM_2D *geomP, BC_2D *BC, SPMAT *A, VEC *RHS);
SPMAT* transform2D_navierstokes_matrix_with_bc       ( ELT_2D *elt , GEOM_2D *geomU , GEOM_2D *geomP, BC_2D *BC, SPMAT *A);


#endif

/*------------------------------------------------------------------------------------------------*/

#ifndef MEF_APPLY_BOUNDARY_CONDITIONS_3D__H
#define MEF_APPLY_BOUNDARY_CONDITIONS_3D__H


#include "matrix.h"
#include "sparse.h"

#include "finite_elements_3D.h"
#include "geometry_3D.h"
#include "boundary_conditions_3D.h"

void transform3D_matrix_vector_with_bc( ELT_3D *elt , GEOM_3D *geom , BC_3D *BC , SPMAT *A , VEC *RHS ); 
void transform3D_vector_with_bc       ( ELT_3D *elt , GEOM_3D *geom , BC_3D *BC , SPMAT *A , VEC *RHS );
void transform3D_matrix_with_bc       ( ELT_3D *elt , GEOM_3D *geom , BC_3D *BC , SPMAT *A );

void   transform3D_stokes_matrix_vector_with_bc      ( ELT_3D *elt , GEOM_3D *geomU , GEOM_3D *geomP, BC_3D *BC, SPMAT *A, VEC *RHS);
VEC*   transform3D_stokes_vector_with_bc             ( ELT_3D *elt , GEOM_3D *geomU , GEOM_3D *geomP, BC_3D *BC, SPMAT *A, VEC *RHS);
SPMAT* transform3D_stokes_matrix_with_bc             ( ELT_3D *elt , GEOM_3D *geomU , GEOM_3D *geomP, BC_3D *BC, SPMAT *A);

void   transform3D_navierstokes_matrix_vector_with_bc(  ELT_3D *elt ,  GEOM_3D *geomU ,  GEOM_3D *geomP,  BC_3D *BC, SPMAT *A, VEC *RHS);
VEC*   transform3D_navierstokes_vector_with_bc       (  ELT_3D *elt ,  GEOM_3D *geomU ,  GEOM_3D *geomP,  BC_3D *BC, SPMAT *A, VEC *RHS);
SPMAT* transform3D_navierstokes_matrix_with_bc       (  ELT_3D *elt ,  GEOM_3D *geomU ,  GEOM_3D *geomP,  BC_3D *BC, SPMAT *A);

#endif