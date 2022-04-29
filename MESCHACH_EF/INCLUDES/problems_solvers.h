#ifndef MEF_PROBLEMS_SOLVER_H
#define MEF_PROBLEMS_SOLVER_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file problems_solvers.h
 * 
 * Contains solvers algorithms for
 * - laplacian
 * - bilaplacian
 * - convection-diffusion
 * - helmholtz
 * - burgers
 *
 * - stokes
 * - navier-stokes
 * ----------------------------------------------------------------------- */

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/rhs.h"
#include "MESCHACH_EF/INCLUDES/adv.h"

VEC*  solve1D_laplacian           (const ELT_1D *elt , const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun );
VEC*  solve1D_bilaplacian         (const ELT_1D *elt , const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun );
VEC*  solve1D_convection_diffusion(const ELT_1D *elt , const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun , const ADV_1D *MyAdvFun );
VEC*  solve1D_helmholz            (const ELT_1D *elt , const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun );
VEC*  solve1D_burgers             (const ELT_1D *elt , const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun );

VEC*  solve2D_laplacian           (const ELT_2D *elt , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun );
VEC*  solve2D_helmholz            (const ELT_2D *elt , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun );
VEC*  solve2D_convection_diffusion(const ELT_2D *elt , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun , const ADV_2D *MyAdvFun );

VEC*  solve3D_laplacian           (const ELT_3D *elt , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun );
VEC*  solve3D_helmholz            (const ELT_3D *elt , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun );
VEC*  solve3D_convection_diffusion(const ELT_3D *elt , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun , const ADV_3D *MyAdvFun );


void  solve2D_stokes        (const ELT_2D *elt2 , const ELT_2D *elt1 , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun );
void  solve2D_stokes_       (const ELT_2D *elt2 , const ELT_2D *elt1 , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun, VEC *U, VEC *V, VEC *P );
void  solve2D_navier_stokes (const ELT_2D *elt2 , const ELT_2D *elt1 , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun );
void  solve2D_navier_stokes_(const ELT_2D *elt2 , const ELT_2D *elt1 , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun, VEC *U, VEC *V, VEC *P );

void  solve3D_stokes        (const ELT_3D *elt2 , const ELT_3D *elt1 , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun );
void  solve3D_stokes_       (const ELT_3D *elt2 , const ELT_3D *elt1 , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, VEC *U, VEC *V, VEC *W, VEC *P );
void  solve3D_navier_stokes (const ELT_3D *elt2 , const ELT_3D *elt1 , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun );
void  solve3D_navier_stokes_(const ELT_3D *elt2 , const ELT_3D *elt1 , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, VEC *U, VEC *V, VEC *W, VEC *P );

#ifdef __cplusplus
}
#endif

#endif
