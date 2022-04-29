#ifndef MEF_ASSEMBLAGE_OBJECTS_3D_H
#define MEF_ASSEMBLAGE_OBJECTS_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file assemblage_objects_3D.h
 *
 * Contains all functions for assemblage of matrix , right hand side  aso .
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_3D.h"
#include "MESCHACH_EF/INCLUDES/rhs_3D.h"
#include "MESCHACH_EF/INCLUDES/adv_3D.h"


SPMAT*  assemblage3D_matrix_Mass      (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Mass   );
SPMAT*  assemblage3D_matrix_Stiff1    (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Stiff1 );
SPMAT*  assemblage3D_matrix_Conv_x    (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_x );
SPMAT*  assemblage3D_matrix_Conv_y    (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_y );
SPMAT*  assemblage3D_matrix_Conv_z    (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_z );
SPMAT*  assemblage3D_matrix_Conv_x_fun(const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_x , const ADV_3D *MyAdvFun);
SPMAT*  assemblage3D_matrix_Conv_y_fun(const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_y , const ADV_3D *MyAdvFun);
SPMAT*  assemblage3D_matrix_Conv_z_fun(const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_z , const ADV_3D *MyAdvFun);
SPMAT*  assemblage3D_matrix_Conv_G_fun(const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_G , const ADV_3D *MyAdvFun);

SPMAT*  assemblage3D_matrix_AUx(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *AUx);
SPMAT*  assemblage3D_matrix_AUy(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *AUy);
SPMAT*  assemblage3D_matrix_AUz(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *AUz);
SPMAT*  assemblage3D_matrix_AxU(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *AxU);
SPMAT*  assemblage3D_matrix_AyU(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *AyU);
SPMAT*  assemblage3D_matrix_AzU(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *AzU);
SPMAT*  assemblage3D_matrix_A_U(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, SPMAT *A_U);

SPMAT*  assemblage3D_matrix_Conv_x_elt2elt1(const ELT_3D *elt2, const GEOM_3D *geom2, const ELT_3D *elt1, const GEOM_3D *geom1, SPMAT *Bx);
SPMAT*  assemblage3D_matrix_Conv_y_elt2elt1(const ELT_3D *elt2, const GEOM_3D *geom2, const ELT_3D *elt1, const GEOM_3D *geom1, SPMAT *By);
SPMAT*  assemblage3D_matrix_Conv_z_elt2elt1(const ELT_3D *elt2, const GEOM_3D *geom2, const ELT_3D *elt1, const GEOM_3D *geom1, SPMAT *Bz);

VEC*    assemblage3D_vector_fun(const ELT_3D *elt, const GEOM_3D *geom, const RHS_3D *RhsFun      , VEC *RHS);
VEC*    assemblage3D_vector_abx(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, const VEC *b, VEC *RHS);
VEC*    assemblage3D_vector_aby(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, const VEC *b, VEC *RHS);
VEC*    assemblage3D_vector_abz(const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, const VEC *b, VEC *RHS);
VEC*    assemblage3D_vector_ab (const ELT_3D *elt, const GEOM_3D *geom, const VEC *a, const VEC *b, VEC *RHS);

SPMAT * assemblage3D_matrix_fromBC(const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *BC, SPMAT *MassBC);
VEC   * assemblage3D_vector_fromBC(const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *BC, VEC *RHS);

#ifdef __cplusplus
}
#endif

#endif

/*! \fn SPMAT *  assemblage3D_matrix_Mass (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Mass   );
* 
* \param elt   : the finite element
* \param geom  : the geometry
* \param Mass  : the mass matrix (output)
*
* Construct the Mass matrix ( fi, fj)
*/

/*! \fn SPMAT * assemblage3D_matrix_Stiff1 (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Stiff1   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Stiff1 : the stiffness matrix (output)
*
* Construct the Stiffness matrix ( Dfi, Dfj)
*/

/*! \fn SPMAT * assemblage3D_matrix_Conv_x (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_x   );
* 
* \param elt    : the finite element
* \param geom   : the geometry
* \param Conv_x : the convection matrix (output)
*
* Construct the Convection matrix  (fi, DfjDx)
*/

/*! \fn SPMAT * assemblage3D_matrix_Conv_y (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_y   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Conv_y : the convection matrix (output)
*
* Construct the Convection matrix  (fi, DfjDy)
*/


/*! \fn SPMAT * assemblage3D_matrix_Conv_z (const ELT_3D *elt, const GEOM_3D *geom, SPMAT *Conv_z   );
* 
* \param elt    : the finite element
* \param geom   : the geometry
* \param Conv_z : the convection matrix (output)
*
* Construct the Convection matrix  (fi, DfjDz)
*/

/*! \fn VEC * assemblage3D_vector_fun (const ELT_3D *elt, const GEOM_3D *geom, const RHS_3D *RhsFun , VEC *RHS  );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param RhsFun : the convection matrix
* \param RHS    : the returned vector (output)
*
* Construct the vector (fi * RhsFun) [i=0..NBSOMM]
*/
