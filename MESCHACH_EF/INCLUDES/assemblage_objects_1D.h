#ifndef MEF_ASSEMBLAGE_OBJECTS_1D_H
#define MEF_ASSEMBLAGE_OBJECTS_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file assemblage_objects_1D.h
 *
 * Contains all functions for assemblage of matrix , right hand side vector aso .
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_1D.h"
#include "MESCHACH_EF/INCLUDES/rhs_1D.h"
#include "MESCHACH_EF/INCLUDES/adv_1D.h"


SPMAT * assemblage1D_matrix_Mass  (const ELT_1D *elt, const GEOM_1D *geom, SPMAT *Mass  );
SPMAT * assemblage1D_matrix_Stiff1(const ELT_1D *elt, const GEOM_1D *geom, SPMAT *Stiff1);
SPMAT * assemblage1D_matrix_Stiff2(const ELT_1D *elt, const GEOM_1D *geom, SPMAT *Stiff2);
SPMAT * assemblage1D_matrix_Convec(const ELT_1D *elt, const GEOM_1D *geom, SPMAT *Convec);

SPMAT * assemblage1D_matrix_AUx   (const ELT_1D *elt, const GEOM_1D *geom, const VEC *a, SPMAT *AUx);
SPMAT * assemblage1D_matrix_AxU   (const ELT_1D *elt, const GEOM_1D *geom, const VEC *b, SPMAT *AxU);
SPMAT * assemblage1D_matrix_A_U   (const ELT_1D *elt, const GEOM_1D *geom, const VEC *b, SPMAT *AU );


VEC *   assemblage1D_vector_fun   (const ELT_1D *elt, const GEOM_1D *geom, const RHS_1D *RhsFun     , VEC *RHS);
VEC *   assemblage1D_vector_abx   (const ELT_1D *elt, const GEOM_1D *geom, const VEC* a, const VEC*b, VEC *RHS);
VEC *   assemblage1D_vector_ab    (const ELT_1D *elt, const GEOM_1D *geom, const VEC* a, const VEC*b, VEC *RHS);


SPMAT * assemblage1D_matrix_fromBC(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *MassBC);
VEC   * assemblage1D_vector_fromBC(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS);


#ifdef __cplusplus
}
#endif


#endif


/*! \fn SPMAT * assemblage1D_matrix_Mass ( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Mass   );
*
* \param elt   : the finite element
* \param geom  : the geometry
* \param Mass  : the mass matrix (output)
*
* Construct the Mass matrix ( fi, fj)
*/

/*! \fn SPMAT * assemblage1D_matrix_Stiff1 ( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Stiff1   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Stiff1 : the stiffness matrix (output)
*
* Construct the Stiffness matrix ( Dfi, Dfj)
*/

/*! \fn SPMAT * assemblage1D_matrix_Stiff2 ( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Stiff2   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Stiff2 : the stiffness matrix (output)
*
* Construct the Stiffness matrix (order 2) (DDfi, DDfj)
*/

/*! \fn SPMAT * assemblage1D_matrix_Convec ( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Convec   );
*
* \param elt    : the finite element
* \param geom   : the geometry
* \param Convec : the mass matrix (output)
*
* Construct the Convection matrix  (fi, Dfj)
*/

