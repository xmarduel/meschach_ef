
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////

#ifndef MEF_ASSEMBLAGE_OBJECTS_1D_H
#define MEF_ASSEMBLAGE_OBJECTS_1D_H


#include "matrix.h"
#include "sparse.h"

#include "finite_elements_1D.h"
#include "geometry_1D.h"
#include "rhs_1D.h"
#include "adv_1D.h"

SPMAT*  assemblage1D_matrix_Mass  ( ELT_1D *elt , GEOM_1D *geom , SPMAT *Mass   );
SPMAT*  assemblage1D_matrix_Stiff1( ELT_1D *elt , GEOM_1D *geom , SPMAT *Stiff1 );
SPMAT*  assemblage1D_matrix_Stiff2( ELT_1D *elt , GEOM_1D *geom , SPMAT *Stiff2 ); 
SPMAT*  assemblage1D_matrix_Convec( ELT_1D *elt , GEOM_1D *geom , SPMAT *Convec );

SPMAT*  assemblage1D_matrix_AUx   ( ELT_1D *elt , GEOM_1D *geom , VEC *a , SPMAT *AUx );
SPMAT*  assemblage1D_matrix_AxU   ( ELT_1D *elt , GEOM_1D *geom , VEC *b , SPMAT *AxU );
SPMAT*  assemblage1D_matrix_A_U   ( ELT_1D *elt , GEOM_1D *geom , VEC *b , SPMAT *A_U );


VEC*    assemblage1D_vector_fun   ( ELT_1D *elt , GEOM_1D *geom , RHS_1D *RhsFun , VEC *RHS );
VEC*    assemblage1D_vector_abx   ( ELT_1D *elt , GEOM_1D *geom , VEC* a, VEC *b , VEC *RHS );
VEC*    assemblage1D_vector_ab    ( ELT_1D *elt , GEOM_1D *geom , VEC* a, VEC *b , VEC *RHS );

SPMAT * assemblage1D_matrix_fromBC(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *MassBC);
VEC   * assemblage1D_vector_fromBC(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS);

#endif

#ifndef MEF_ASSEMBLAGE_OBJECTS_2D_H
#define MEF_ASSEMBLAGE_OBJECTS_2D_H


#include "matrix.h"
#include "sparse.h"

#include "finite_elements_2D.h"
#include "geometry_2D.h"
#include "rhs_2D.h"
#include "adv_2D.h"

SPMAT*  assemblage2D_matrix_Mass      ( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Mass   );
SPMAT*  assemblage2D_matrix_Stiff1    ( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Stiff1 );
SPMAT*  assemblage2D_matrix_Conv_x    ( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_x );
SPMAT*  assemblage2D_matrix_Conv_y    ( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_y );
SPMAT*  assemblage2D_matrix_M_Stab    ( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *M_Stab , const ADV_2D *MyAdvFun ); 
SPMAT*  assemblage2D_matrix_Conv_x_fun( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_x , const ADV_2D *MyAdvFun );
SPMAT*  assemblage2D_matrix_Conv_y_fun( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_y , const ADV_2D *MyAdvFun );
SPMAT*  assemblage2D_matrix_Conv_G_fun( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_G , const ADV_2D *MyAdvFun );

SPMAT*  assemblage2D_matrix_AUx ( ELT_2D *elt , GEOM_2D *geom, VEC *a , SPMAT *AUx );
SPMAT*  assemblage2D_matrix_AUy ( ELT_2D *elt , GEOM_2D *geom, VEC *a , SPMAT *AUy );
SPMAT*  assemblage2D_matrix_AxU ( ELT_2D *elt , GEOM_2D *geom, VEC *a , SPMAT *AxU );
SPMAT*  assemblage2D_matrix_AyU ( ELT_2D *elt , GEOM_2D *geom, VEC *a , SPMAT *AyU );
SPMAT*  assemblage2D_matrix_A_U ( ELT_2D *elt , GEOM_2D *geom, VEC *a , SPMAT *A_U );

SPMAT*  assemblage2D_matrix_Conv_x_elt2elt1( ELT_2D *elt2 , GEOM_2D *Mygeom2 , ELT_2D *elt1 , GEOM_2D *Mygeom1 , SPMAT *Bx );
SPMAT*  assemblage2D_matrix_Conv_y_elt2elt1( ELT_2D *elt2 , GEOM_2D *Mygeom2 , ELT_2D *elt1 , GEOM_2D *Mygeom1 , SPMAT *By );


VEC*    assemblage2D_vector_fun( ELT_2D *elt , GEOM_2D *geom , RHS_2D *RhsFun , VEC *RHS );
VEC*    assemblage2D_vector_abx( ELT_2D *elt , GEOM_2D *geom , VEC* a, VEC *b , VEC *RHS );
VEC*    assemblage2D_vector_aby( ELT_2D *elt , GEOM_2D *geom , VEC* a, VEC *b , VEC *RHS );
VEC*    assemblage2D_vector_ab ( ELT_2D *elt , GEOM_2D *geom , VEC* a, VEC *b , VEC *RHS );

SPMAT * assemblage2D_matrix_fromBC(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *MassBC);
VEC   * assemblage2D_vector_fromBC(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, VEC *RHS);

/*------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------*/

#endif

#ifndef MEF_ASSEMBLAGE_OBJECTS_3D_H
#define MEF_ASSEMBLAGE_OBJECTS_3D_H


#include "matrix.h"
#include "sparse.h"

#include "finite_elements_3D.h"
#include "geometry_3D.h"
#include "rhs_3D.h"
#include "adv_3D.h"

SPMAT*  assemblage3D_matrix_Mass      ( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Mass   );
SPMAT*  assemblage3D_matrix_Stiff1    ( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Stiff1 );
SPMAT*  assemblage3D_matrix_Conv_x    ( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_x );
SPMAT*  assemblage3D_matrix_Conv_y    ( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_y );
SPMAT*  assemblage3D_matrix_Conv_z    ( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_z );
SPMAT*  assemblage3D_matrix_Conv_x_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_x , const ADV_3D *MyAdvFun );
SPMAT*  assemblage3D_matrix_Conv_y_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_y , const ADV_3D *MyAdvFun );
SPMAT*  assemblage3D_matrix_Conv_z_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_y , const ADV_3D *MyAdvFun );
SPMAT*  assemblage3D_matrix_Conv_G_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_G , const ADV_3D *MyAdvFun );

SPMAT*  assemblage3D_matrix_AUx   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *AUx );
SPMAT*  assemblage3D_matrix_AUy   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *AUy );
SPMAT*  assemblage3D_matrix_AUz   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *AUz );
SPMAT*  assemblage3D_matrix_AxU   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *AxU );
SPMAT*  assemblage3D_matrix_AyU   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *AyU );
SPMAT*  assemblage3D_matrix_AzU   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *AzU );
SPMAT*  assemblage3D_matrix_A_U   ( ELT_3D *elt , GEOM_3D *geom, VEC *a , SPMAT *A_U );

SPMAT*  assemblage3D_matrix_Conv_x_elt2elt1( ELT_3D *elt2 , GEOM_3D *Mygeom2 , ELT_3D *elt1 , GEOM_3D *Mygeom1 , SPMAT *Bx );
SPMAT*  assemblage3D_matrix_Conv_y_elt2elt1( ELT_3D *elt2 , GEOM_3D *Mygeom2 , ELT_3D *elt1 , GEOM_3D *Mygeom1 , SPMAT *By );
SPMAT*  assemblage3D_matrix_Conv_z_elt2elt1( ELT_3D *elt2 , GEOM_3D *Mygeom2 , ELT_3D *elt1 , GEOM_3D *Mygeom1 , SPMAT *By );

VEC*    assemblage3D_vector_fun( ELT_3D *elt , GEOM_3D *geom , RHS_3D *RhsFun , VEC *RHS );
VEC*    assemblage3D_vector_abx( ELT_3D *elt , GEOM_3D *geom , VEC* a , VEC*b , VEC *RHS );
VEC*    assemblage3D_vector_aby( ELT_3D *elt , GEOM_3D *geom , VEC* a , VEC*b , VEC *RHS );
VEC*    assemblage3D_vector_abz( ELT_3D *elt , GEOM_3D *geom , VEC* a , VEC*b , VEC *RHS );
VEC*    assemblage3D_vector_ab ( ELT_3D *elt , GEOM_3D *geom , VEC* a , VEC*b , VEC *RHS );

SPMAT * assemblage3D_matrix_fromBC(const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *BC, SPMAT *MassBC);
VEC   * assemblage3D_vector_fromBC(const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *BC, VEC *RHS);

#endif