#ifndef MEF_ASSEMBLAGE_OBJECTS_2D_H
#define MEF_ASSEMBLAGE_OBJECTS_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file assemblage_objects_2D.h
 *
 * Contains all functions for assemblage of matrix , right hand side aso.
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_2D.h"
#include "MESCHACH_EF/INCLUDES/rhs_2D.h"
#include "MESCHACH_EF/INCLUDES/adv_2D.h"


SPMAT*  assemblage2D_matrix_Mass      (const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Mass   );
SPMAT*  assemblage2D_matrix_Stiff1    (const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Stiff1 );
SPMAT*  assemblage2D_matrix_Conv_x    (const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_x );
SPMAT*  assemblage2D_matrix_Conv_y    (const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_y );
SPMAT*  assemblage2D_matrix_M_Stab    (const ELT_2D *elt, const GEOM_2D *geom, SPMAT *M_Stab , const ADV_2D *MyAdvFun );
SPMAT*  assemblage2D_matrix_Conv_x_fun(const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_x , const ADV_2D *MyAdvFun );
SPMAT*  assemblage2D_matrix_Conv_y_fun(const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_y , const ADV_2D *MyAdvFun );
SPMAT*  assemblage2D_matrix_Conv_G_fun(const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_G , const ADV_2D *MyAdvFun );

SPMAT*  assemblage2D_matrix_AUx       (const ELT_2D *elt, const GEOM_2D *geom, const VEC *a, SPMAT *AUx);
SPMAT*  assemblage2D_matrix_AUy       (const ELT_2D *elt, const GEOM_2D *geom, const VEC *a, SPMAT *AUy);
SPMAT*  assemblage2D_matrix_AxU       (const ELT_2D *elt, const GEOM_2D *geom, const VEC *a, SPMAT *AxU);
SPMAT*  assemblage2D_matrix_AyU       (const ELT_2D *elt, const GEOM_2D *geom, const VEC *a, SPMAT *AyU);
SPMAT*  assemblage2D_matrix_A_U       (const ELT_2D *elt, const GEOM_2D *geom, const VEC *a, SPMAT *A_U);

SPMAT*  assemblage2D_matrix_Conv_x_elt2elt1(const ELT_2D *elt2, const GEOM_2D *geom2, const ELT_2D *elt1, const GEOM_2D *geom1, SPMAT *Bx);
SPMAT*  assemblage2D_matrix_Conv_y_elt2elt1(const ELT_2D *elt2, const GEOM_2D *geom2, const ELT_2D *elt1, const GEOM_2D *geom1, SPMAT *By);


VEC*    assemblage2D_vector_fun (const ELT_2D *elt, const GEOM_2D *geom, const RHS_2D *RhsFun      , VEC *RHS);
VEC*    assemblage2D_vector_abx (const ELT_2D *elt, const GEOM_2D *geom, const VEC* a, const VEC* b, VEC *RHS);
VEC*    assemblage2D_vector_aby (const ELT_2D *elt, const GEOM_2D *geom, const VEC* a, const VEC* b, VEC *RHS);
VEC*    assemblage2D_vector_ab  (const ELT_2D *elt, const GEOM_2D *geom, const VEC* a, const VEC* b, VEC *RHS);


SPMAT * assemblage2D_matrix_fromBC(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *MassBC);
VEC   * assemblage2D_vector_fromBC(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, VEC *RHS);

#ifdef __cplusplus
}
#endif

#endif

/*! \fn SPMAT * assemblage2D_matrix_Mass ( const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Mass   );
*
* \param elt   : the finite element
* \param geom  : the geometry
* \param Mass  : the mass matrix (output)
*
* Construct the Mass matrix ( fi, fj)
*/

/*! \fn SPMAT * assemblage2D_matrix_Stiff1 ( const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Stiff1   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Stiff1 : the stiffness matrix (output)
*
* Construct the Stiffness matrix ( Dfi, Dfj)
*/

/*! \fn SPMAT * assemblage2D_matrix_Conv_x ( const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_x   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Conv_x : the convection matrix (output)
*
* Construct the Convection matrix  (fi, DfjDx)
*/

/*! \fn SPMAT * assemblage2D_matrix_Conv_y ( const ELT_2D *elt, const GEOM_2D *geom, SPMAT *Conv_y   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Conv_y : the convection matrix (output)
*
* Construct the Convection matrix  (fi, DfjDy)
*/


