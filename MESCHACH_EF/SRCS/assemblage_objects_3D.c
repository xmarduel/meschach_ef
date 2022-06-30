
/*
 * Contains the functions to matrix and vector assemblage in 3D
 *
 */

#define  SQR(x)  (x)*(x)
#define  CUB(x)  (x)*(x)*(x)


#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"      /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"  /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/rhs.h"                  /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/assemblage_objects_3D.h"
#include "MESCHACH_EF/INCLUDES/triangle_integration_gauss_3D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"


typedef enum
{
   ASSEMBLAGEe_MASS  = 0,
   ASSEMBLAGEe_STIFF = 1,
   ASSEMBLAGEe_CONVx = 2,
   ASSEMBLAGEe_CONVy = 3,
   ASSEMBLAGEe_CONVz = 4,

   ASSEMBLAGEe_CONV_X_FUN = 5,
   ASSEMBLAGEe_CONV_Y_FUN = 6,
   ASSEMBLAGEe_CONV_Z_FUN = 7,
   ASSEMBLAGEe_CONV_G_FUN = 8,

   ASSEMBLAGEe_AUx   =  9,
   ASSEMBLAGEe_AUy   = 10,
   ASSEMBLAGEe_AUz   = 11,
   ASSEMBLAGEe_AxU   = 12,
   ASSEMBLAGEe_AyU   = 13,
   ASSEMBLAGEe_AzU   = 14,
   ASSEMBLAGEe_A_U   = 15,

   ASSEMBLAGEe_ABx   = 16,
   ASSEMBLAGEe_ABy   = 17,
   ASSEMBLAGEe_ABz   = 18,
   ASSEMBLAGEe_A_B   = 19,

   ASSEMBLAGEe_STOKES_Bx = 20,
   ASSEMBLAGEe_STOKES_By = 21,
   ASSEMBLAGEe_STOKES_Bz = 22

} ASSEMBLAGEt_TYPE;


static SPMAT * _assemblage_matrix1a( const ASSEMBLAGEt_TYPE type , const ELT_3D *elt , const GEOM_3D *geom , SPMAT *A  );
static SPMAT * _assemblage_matrix1b( const ASSEMBLAGEt_TYPE type , const ELT_3D *elt , const GEOM_3D *geom , SPMAT *A , const ADV_3D *MyAdvFun );

static MAT * _systel_matrix_Mass  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el );
static MAT * _systel_matrix_Stiff1( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el );
static MAT * _systel_matrix_Conv_x( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el );
static MAT * _systel_matrix_Conv_y( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el );
static MAT * _systel_matrix_Conv_z( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el );

static MAT * _systel_matrix_Conv_x_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el , const ADV_3D *MyAdvFun );
static MAT * _systel_matrix_Conv_y_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el , const ADV_3D *MyAdvFun );
static MAT * _systel_matrix_Conv_z_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el , const ADV_3D *MyAdvFun );
static MAT * _systel_matrix_Conv_G_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el , const ADV_3D *MyAdvFun );


static SPMAT * _assemblage_matrix2( const ASSEMBLAGEt_TYPE type , const ELT_3D *element , const GEOM_3D *geom,  const VEC *a , SPMAT *A );

static TENSOR * _systel_matrix_AUx( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AUy( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AUz( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AxU( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AyU( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AzU( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_A_U( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el );


static SPMAT * _assemblage_matrix3( const ASSEMBLAGEt_TYPE type , const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                                                  const ELT_3D *elt1 , const GEOM_3D *geom1 , SPMAT *B_ );

static MAT* _systel_matrix_Conv_x_elt2elt1( u_int e , const ELT_3D *elt2 , const GEOM_3D *Mygeom2 , const ELT_3D *elt1 , const GEOM_3D *Mygeom1 , MAT *Bx_el );
static MAT* _systel_matrix_Conv_y_elt2elt1( u_int e , const ELT_3D *elt2 , const GEOM_3D *Mygeom2 , const ELT_3D *elt1 , const GEOM_3D *Mygeom1 , MAT *By_el );
static MAT* _systel_matrix_Conv_z_elt2elt1( u_int e , const ELT_3D *elt2 , const GEOM_3D *Mygeom2 , const ELT_3D *elt1 , const GEOM_3D *Mygeom1 , MAT *Bz_el );


static VEC * _assemblage_vector2( const ASSEMBLAGEt_TYPE type , const ELT_3D *element , const GEOM_3D *geom,  const VEC *a ,  const VEC *b, VEC *RHS );

static VEC * _systel_vector_fun ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , const RHS_3D *RhsFun        , VEC *Rhs_el );
static VEC * _systel_vector_abx ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );
static VEC * _systel_vector_aby ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );
static VEC * _systel_vector_abz ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );
static VEC * _systel_vector_ab  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );

static Real eval_jac_on_e    ( int e , const ELT_3D *element , const GEOM_3D *geom );
static Real eval_jac_on_e_   ( int e , const ELT_3D *element , const GEOM_3D *geom , Real transformation[3][3]);

static Real element_size     ( int e , const ELT_3D *element , const GEOM_3D *geom , Real center[3]);



static Real eval_jac_on_face ( int f , const ELT_3D *element , const GEOM_3D *geom , Real face_center[3]);

static void apply_Bc3D_cauchy_on_mat   (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A);
static void apply_Bc3D_robin_on_mat    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A);

static void apply_Bc3D_neumann_on_rhs  (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS);
static void apply_Bc3D_robin_on_rhs    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS);


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage3D_matrix_Mass( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Mass )
{
   /* check */
   if ( elt  == NULL ) error(E_NULL, "assemblage3D_matrix_Mass");
   if ( geom == NULL ) error(E_NULL, "assemblage3D_matrix_Mass");
   if ( Mass == NULL ) error(E_NULL, "assemblage3D_matrix_Mass");

   /* delegate */
   return _assemblage_matrix1a( ASSEMBLAGEe_MASS , elt , geom , Mass);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage3D_matrix_Stiff1( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Stiff1 )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_matrix_Stiff1");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_matrix_Stiff1");
   if ( Stiff1 == NULL ) error(E_NULL, "assemblage3D_matrix_Stiff1");

   /* delegate */
   return _assemblage_matrix1a( ASSEMBLAGEe_STIFF , elt , geom , Stiff1);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage3D_matrix_Conv_x( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_x )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x");
   if ( Conv_x == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x");

   /* delegate */
   return _assemblage_matrix1a( ASSEMBLAGEe_CONVx , elt , geom , Conv_x);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage3D_matrix_Conv_y( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_y )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y");
   if ( Conv_y == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y");

   /* delegate */
   return _assemblage_matrix1a( ASSEMBLAGEe_CONVy , elt , geom, Conv_y);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage3D_matrix_Conv_z( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_z )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z");
   if ( Conv_z == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z");

   /* delegate */
   return _assemblage_matrix1a( ASSEMBLAGEe_CONVz , elt , geom , Conv_z);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_Conv_x_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_x , const ADV_3D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_fun");
   if ( Conv_x   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_X_FUN , elt , geom , Conv_x , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_Conv_y_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_y , const ADV_3D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");
   if ( Conv_y   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_Y_FUN , elt , geom , Conv_y , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_Conv_z_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_z , const ADV_3D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");
   if ( Conv_z   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_Z_FUN , elt , geom , Conv_z , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_Conv_G_fun( const ELT_3D *elt , const GEOM_3D *geom , SPMAT *Conv_G , const ADV_3D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_G_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_G_fun");
   if ( Conv_G   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_G_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_G_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_G_FUN , elt , geom , Conv_G , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT* _assemblage_matrix1a( ASSEMBLAGEt_TYPE type , const ELT_3D *elt , const GEOM_3D *geom , SPMAT *A  )
{
   int e; int i,j;

   MAT *A_el;
   MAT* (*_systel_matrix_)( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el );

   /* check */
   if ( elt  == NULL ) error(E_NULL, "_assemblage_matrix1a");
   if ( geom == NULL ) error(E_NULL, "_assemblage_matrix1a");
   if ( A    == NULL ) error(E_NULL, "_assemblage_matrix1a");

   switch( type )
   {
      case ASSEMBLAGEe_MASS : _systel_matrix_ = _systel_matrix_Mass   ; break;
      case ASSEMBLAGEe_STIFF: _systel_matrix_ = _systel_matrix_Stiff1 ; break;
      case ASSEMBLAGEe_CONVx: _systel_matrix_ = _systel_matrix_Conv_x ; break;
      case ASSEMBLAGEe_CONVy: _systel_matrix_ = _systel_matrix_Conv_y ; break;
      case ASSEMBLAGEe_CONVz: _systel_matrix_ = _systel_matrix_Conv_z ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix1a");
   }

   /* alloc mem */
   A_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation of A */
   sp_zero(A);

   for(e=0; e<geom->NBELMT; e++)
   {
      A_el = _systel_matrix_( e , elt , geom , A_el );

      for (i=0; i<elt->nb_somm_cell; i++)
      for (j=0; j<elt->nb_somm_cell; j++)
      {
         sp_add_val( A , geom->NSELMT->im[e][i] , geom->NSELMT->im[e][j] , A_el->me[i][j] );
      }
   }

   M_FREE(A_el) ;

   return sp_compact(A, MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT* _assemblage_matrix1b( const ASSEMBLAGEt_TYPE type , const ELT_3D *elt , const GEOM_3D *geom , SPMAT *A , const ADV_3D *MyAdvFun )
{
   int e; int i,j;

   MAT *A_el;
   MAT* (*_systel_matrix_)( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* M_el , const ADV_3D *MyAdvFun );

   /* check */
   if ( elt  == NULL ) error(E_NULL, "_assemblage_matrix1b");
   if ( geom == NULL ) error(E_NULL, "_assemblage_matrix1b");
   if ( A    == NULL ) error(E_NULL, "_assemblage_matrix1b");

   switch( type )
   {
      case ASSEMBLAGEe_CONV_X_FUN: _systel_matrix_ = _systel_matrix_Conv_x_fun ; break;
      case ASSEMBLAGEe_CONV_Y_FUN: _systel_matrix_ = _systel_matrix_Conv_y_fun ; break;
      case ASSEMBLAGEe_CONV_Z_FUN: _systel_matrix_ = _systel_matrix_Conv_z_fun ; break;
      case ASSEMBLAGEe_CONV_G_FUN: _systel_matrix_ = _systel_matrix_Conv_G_fun ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix1b");
   }

   /* alloc mem */
   A_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation of A */
   sp_zero(A);

   for(e=0; e<geom->NBELMT; e++)
   {
      A_el = _systel_matrix_( e , elt , geom , A_el , MyAdvFun );

      for (i=0; i<elt->nb_somm_cell; i++)
      for (j=0; j<elt->nb_somm_cell; j++)
      {
         sp_add_val( A , geom->NSELMT->im[e][i] , geom->NSELMT->im[e][j] , A_el->me[i][j] );
      }
   }

   M_FREE(A_el) ;

   return sp_compact(A, MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Mass( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , MAT *Mass_el )
{
   Real jacobien ;

   /* initialisation Mass_el */
   m_zero(Mass_el);

   /* calcul jacobien on elt "e" */
   jacobien =  eval_jac_on_e( e, elt, geom );

   /* calcul Mass_el on triangle number "e" */
   Mass_el = sm_mlt(jacobien, elt->MAT_I_I, Mass_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         Mass_el->me[i][j]  -= w3D[m]*jacobien*
            elt->f_base[i](ksi3D[m],eta3D[m],psi3D[m])*
            elt->f_base[j](ksi3D[m],eta3D[m],psi3D[m])  ;
      }
   }
   */

   return Mass_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Stiff1( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , MAT *Stiff1_el )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];

   Real ksi_x, eta_x, psi_x,
        ksi_y, eta_y, psi_y,
        ksi_z, eta_z, psi_z;

   Real c_xx, c_yy, c_zz, c_xy, c_xz, c_yz;


/*
              2        2         2
   a = [ ksi_x  + ksi_y   + ksi_z  ] * jacobien    (Nksi*Nksi)

              2         2        2
   b = [ eta_x  +  eta_y  + eta_z  ] * jacobien    (Neta*Neta)

              2         2        2
   c = [ psi_x  +  psi_y  + psi_z  ] * jacobien    (Npsi*Npsi)


   d = [ ksi_x.eta_x  + ksi_y.eta_y + ksi_z.eta_z ] * jacobien   (Nksi*Neta + Neta*Nksi)


   e = [ ksi_x.psi_x  + ksi_y.psi_y + ksi_z.psi_z ] * jacobien   (Nksi*Npsi + Npsi*Nksi)


   f = [ eta_x.psi_x  + eta_y.psi_y + eta_z.psi_z ] * jacobien   (Neta*Npsi + Npsi*Neta)

*/


   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell, elt->nb_somm_cell);

   /* jacobien */
   jacobien = eval_jac_on_e_( e, elt, geom, transformation );

   /*
                                   [ ksi_x , eta_x , psi_x ]
    transformation is a 3x3 matrix [ ksi_y , eta_y , psi_y ]
                                   [ ksi_z , eta_z , psi_z ]

    */

   ksi_x = transformation[0][0]; /*printf("tetra %d : ksi_x = %lf \n", e, ksi_x);*/
   ksi_y = transformation[1][0]; /*printf("tetra %d : ksi_y = %lf \n", e, ksi_y);*/
   ksi_z = transformation[2][0]; /*printf("tetra %d : ksi_z = %lf \n", e, ksi_z);*/

   eta_x = transformation[0][1]; /*printf("tetra %d : eta_x = %lf \n", e, eta_x);*/
   eta_y = transformation[1][1]; /*printf("tetra %d : eta_y = %lf \n", e, eta_y);*/
   eta_z = transformation[2][1]; /*printf("tetra %d : eta_z = %lf \n", e, eta_z);*/

   psi_x = transformation[0][2]; /*printf("tetra %d : psi_x = %lf \n", e, psi_x);*/
   psi_y = transformation[1][2]; /*printf("tetra %d : psi_y = %lf \n", e, psi_y);*/
   psi_z = transformation[2][2]; /*printf("tetra %d : psi_z = %lf \n", e, psi_z);*/


   c_xx = ( ksi_x*ksi_x  +  ksi_y*ksi_y  +  ksi_z*ksi_z  ) * jacobien; /*printf("c_xy = %lf \n", c_xx);*/
   c_yy = ( eta_x*eta_x  +  eta_y*eta_y  +  eta_z*eta_z  ) * jacobien; /*printf("c_yy = %lf \n", c_yy);*/
   c_zz = ( psi_x*psi_x  +  psi_y*psi_y  +  psi_z*psi_z  ) * jacobien; /*printf("c_zz = %lf \n", c_zz);*/

   c_xy = ( ksi_x*eta_x  +  ksi_y*eta_y  +  ksi_z*eta_z  ) * jacobien ; /*printf("c_xy = %lf \n", c_xy);*/
   c_xz = ( ksi_x*psi_x  +  ksi_y*psi_y  +  ksi_z*psi_z  ) * jacobien ; /*printf("c_xz = %lf \n", c_xz);*/
   c_yz = ( eta_x*psi_x  +  eta_y*psi_y  +  eta_z*psi_z  ) * jacobien ; /*printf("c_yz = %lf \n", c_yz);*/

   /* initialisation Stiff_el */
   m_zero(Stiff1_el);

   Stiff1_el = m_add(Stiff1_el,sm_mlt(c_xx, elt->MAT_x_x, work_el), Stiff1_el);
   Stiff1_el = m_add(Stiff1_el,sm_mlt(c_yy, elt->MAT_y_y, work_el), Stiff1_el);
   Stiff1_el = m_add(Stiff1_el,sm_mlt(c_zz, elt->MAT_z_z, work_el), Stiff1_el);

   Stiff1_el = m_add(Stiff1_el,sm_mlt(c_xy, elt->MAT_x_y__plus__y_x, work_el), Stiff1_el);
   Stiff1_el = m_add(Stiff1_el,sm_mlt(c_xz, elt->MAT_x_z__plus__z_x, work_el), Stiff1_el);
   Stiff1_el = m_add(Stiff1_el,sm_mlt(c_yz, elt->MAT_y_z__plus__z_y, work_el), Stiff1_el);

   /*m_foutput(stdout, Stiff1_el);*/

   /* or with numerical integration  with the basis functions : */
   /*
   m_zero(work_el);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         work_el->me[i][j]  += w3D[m]*(
          c_xx*  elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m])  +
          c_yy*  elt->dfdy_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m])  +
          c_zz*  elt->dfdz_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m])  +
          c_xy*( elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m])  +
                 elt->dfdy_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) )+
          c_xz*( elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m])  +
                 elt->dfdz_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) )+
          c_yz*( elt->dfdy_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m])  +
                 elt->dfdz_base[i](ksi3D[m],eta3D[m],psi3D[m])*elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) )
                                       ) ;
      }
   }

   m_sub(work_el,Stiff1_el,work_el);
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      if ( fabs( work_el->me[i][j] ) < 1.0e-5 )
      {
         work_el->me[i][j] = 0.0;
      }
   }

   m_foutput(stdout, work_el);
   */

   /* free memory */
   M_FREE(work_el);


   return Stiff1_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Conv_x( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , MAT *Conv_x_el )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real ksi_x, eta_x, psi_x ;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation Conv_x_el */
   m_zero(Conv_x_el);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_( e, elt, geom, transformation );

   /* the transformation */
   ksi_x = transformation[0][0];
   eta_x = transformation[0][1];
   psi_x = transformation[0][2];


   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*ksi_x),elt->MAT_I_x,work_el), Conv_x_el);
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*eta_x),elt->MAT_I_y,work_el), Conv_x_el);
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*psi_x),elt->MAT_I_z,work_el), Conv_x_el);

   /*
   Conv_x_el = |Jac|.bx.(ksi_x.N_Nx + eta_x.N_Ny + psi_x.N_Nz) ;
   Conv_y_el = |Jac|.by.(ksi_y.N_Nx + eta_y.N_Ny + psi_y.N_Nz) ;
   Conv_z_el = |Jac|.bz.(ksi_z.N_Nx + eta_z.N_Ny + psi_z.N_Nz) ;
   */


   /* or with numerical integration  with the basis functions : */
   /*
   m_zero(work_el);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         work_el->me[i][j]  += w3D[m]*jacobien*
                                 elt->f_base[i](ksi3D[m],eta3D[m],psi3D[m])*   (
                           ksi_x*elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m]) +
                           eta_x*elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
                           psi_x*elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m])
                                                                                ) ;
       }
   }

   m_sub(work_el,Conv_x_el,work_el);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      if ( fabs( work_el->me[i][j] ) < 1.0e-5 )
      {
         work_el->me[i][j] = 0.0;
      }
   }

   m_foutput(stdout, work_el);
   */

   /* free memory */
   M_FREE(work_el);

   return Conv_x_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Conv_y( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , MAT *Conv_y_el )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real ksi_y, eta_y, psi_y ;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation Conv_x_el */
   m_zero(Conv_y_el);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_( e, elt, geom, transformation );

   /* the transformation */
   ksi_y = transformation[1][0];
   eta_y = transformation[1][1];
   psi_y = transformation[1][2];


   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*ksi_y),elt->MAT_I_x,work_el), Conv_y_el);
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*eta_y),elt->MAT_I_y,work_el), Conv_y_el);
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*psi_y),elt->MAT_I_z,work_el), Conv_y_el);

   /* or with numerical integration  with the basis functions :
   m_zero(work_el);

   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   {
     for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
     {
        xx[m] = x1 + (x2-x1)*ksi3D[m] + (x3-x1)*eta3D[m] + (x4-x1)*psi3D[m];
        yy[m] = y1 + (y2-y1)*ksi3D[m] + (y3-y1)*eta3D[m] + (y4-y1)*psi3D[m] ;
        zz[m] = z1 + (z2-z1)*ksi3D[m] + (z3-z1)*eta3D[m] + (z4-z1)*psi3D[m] ;

        work_el->me[i][j]  += w[m]*jacobien*
           by(xx[m],yy[m],zz[m])*
           element->f_base[i](ksi3D[m],eta3D[m],psi3D[m])*   (
           ksi_y*element->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m]) +
           eta_y*element->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
           psi_y*element->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) ) ;
      }
   }

   m_sub(work_el,Conv_y_el,work_el);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
         if ( fabs( work_el->me[i][j] ) < 1.0e-5 )
         {
            work_el->me[i][j] = 0.0;
         }
   }

   m_foutput(stdout, work_el);
   */

   /* free memory */
   M_FREE(work_el);

   return Conv_y_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Conv_z( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , MAT *Conv_z_el )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real ksi_z, eta_z, psi_z ;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation Conv_x_el */
   m_zero(Conv_z_el);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_( e, elt, geom, transformation );

   /* the transformation */
   ksi_z = transformation[2][0];
   eta_z = transformation[2][1];
   psi_z = transformation[2][2];


   Conv_z_el = m_add(Conv_z_el, sm_mlt( (jacobien*ksi_z),elt->MAT_I_x,work_el), Conv_z_el);
   Conv_z_el = m_add(Conv_z_el, sm_mlt( (jacobien*eta_z),elt->MAT_I_y,work_el), Conv_z_el);
   Conv_z_el = m_add(Conv_z_el, sm_mlt( (jacobien*psi_z),elt->MAT_I_z,work_el), Conv_z_el);

   /* or with numerical integration  with the basis functions :
   m_zero(work_el);

   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         work_el->me[i][j]  += w[m]*jacobien*
            by(xx[m],yy[m],zz[m])*
            element->f_base[i](ksi[m],eta[m],psi[m])*   (
          ksi_z*element->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m]) +
          eta_z*element->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
          psi_z*element->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) ) ;
      }
   }

   m_sub(work_el,Conv_z_el,work_el);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
         if ( fabs( work_el->me[i][j] ) < 1.0e-5 )
         {
            work_el->me[i][j] = 0.0;
         }
   }

   m_foutput(stdout, work_el);
   */

   /* free memory */
   M_FREE(work_el);

   return Conv_z_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_x_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* Conv_x_el , const ADV_3D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real ksi_x, eta_x, psi_x;

   Real cell_size;
   Real cell_center[3];
   Real b1;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* calcul cell center */
   cell_size = element_size( e, elt, geom , cell_center);

   b1 = Adv3D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, cell_center[0], cell_center[1], cell_center[2] );

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];
   eta_x =  transformation[0][1];
   psi_x =  transformation[0][2];

   /* initialisation Conv_x_el */
   m_zero(Conv_x_el);

   /* we have */
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*ksi_x),elt->MAT_I_x,work_el), Conv_x_el);
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*eta_x),elt->MAT_I_y,work_el), Conv_x_el);
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*psi_x),elt->MAT_I_z,work_el), Conv_x_el);

   Conv_x_el = sm_mlt( b1, Conv_x_el, Conv_x_el);

   /*
                               _  _           _  _           _  _
   Conv_x_el = |Jac|.bx.(ksi_x.Ni_Nxj + eta_x.Ni_Nyj + psi_x.Ni_Nzj) ;
   Conv_y_el = |Jac|.by.(ksi_y.Ni_Nxj + eta_y.Ni_Nyj + psi_y.Ni_Nzj)) ;
   Conv_z_el = |Jac|.bz.(ksi_z.Ni_Nxj + eta_z.Ni_Nyj + psi_z.Ni_Nzj)) ;
   */


   /* free memory */
   M_FREE(work_el);

   return Conv_x_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_y_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* Conv_y_el , const ADV_3D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real ksi_y, eta_y, psi_y;

   Real cell_size;
   Real cell_center[3];
   Real b2;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* calcul cell center */
   cell_size = element_size( e, elt, geom , cell_center);

   b2 = Adv3D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Y, cell_center[0], cell_center[1], cell_center[2] );

   /* get the transformation parameters */
   ksi_y =  transformation[1][0];
   eta_y =  transformation[1][1];
   psi_y =  transformation[1][2];

   /* initialisation Conv_y_el */
   m_zero(Conv_y_el);

   /* we have */
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*ksi_y),elt->MAT_I_x,work_el), Conv_y_el);
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*eta_y),elt->MAT_I_y,work_el), Conv_y_el);
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*psi_y),elt->MAT_I_z,work_el), Conv_y_el);

   Conv_y_el = sm_mlt( b2, Conv_y_el, Conv_y_el);

   /*
    _  _           _  _           _  _
    Conv_x_el = |Jac|.bx.(ksi_x.Ni_Nxj + eta_x.Ni_Nyj + psi_x.Ni_Nzj) ;
    Conv_y_el = |Jac|.by.(ksi_y.Ni_Nxj + eta_y.Ni_Nyj + psi_y.Ni_Nzj)) ;
    Conv_z_el = |Jac|.bz.(ksi_z.Ni_Nxj + eta_z.Ni_Nyj + psi_z.Ni_Nzj)) ;
    */


   /* free memory */
   M_FREE(work_el);

   return Conv_y_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_z_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* Conv_z_el , const ADV_3D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real ksi_z, eta_z, psi_z;

   Real cell_size;
   Real cell_center[3];
   Real b3;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* calcul cell center */
   cell_size = element_size( e, elt, geom , cell_center);

   b3 = Adv3D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Z, cell_center[0], cell_center[1], cell_center[2] );

   /* get the transformation parameters */
   ksi_z =  transformation[2][0];
   eta_z =  transformation[2][1];
   psi_z =  transformation[2][2];

   /* initialisation Conv_z_el */
   m_zero(Conv_z_el);

   /* we have */
   Conv_z_el = m_add(Conv_z_el, sm_mlt( (jacobien*ksi_z),elt->MAT_I_x,work_el), Conv_z_el);
   Conv_z_el = m_add(Conv_z_el, sm_mlt( (jacobien*eta_z),elt->MAT_I_y,work_el), Conv_z_el);
   Conv_z_el = m_add(Conv_z_el, sm_mlt( (jacobien*psi_z),elt->MAT_I_z,work_el), Conv_z_el);

   Conv_z_el = sm_mlt( b3, Conv_z_el, Conv_z_el);

   /*
    _  _           _  _           _  _
    Conv_x_el = |Jac|.bx.(ksi_x.Ni_Nxj + eta_x.Ni_Nyj + psi_x.Ni_Nzj) ;
    Conv_y_el = |Jac|.by.(ksi_y.Ni_Nxj + eta_y.Ni_Nyj + psi_y.Ni_Nzj)) ;
    Conv_z_el = |Jac|.bz.(ksi_z.Ni_Nxj + eta_z.Ni_Nyj + psi_z.Ni_Nzj)) ;
    */


   /* free memory */
   M_FREE(work_el);

   return Conv_z_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_G_fun( u_int e , const ELT_3D *elt , const GEOM_3D *geom , MAT* Conv_G_el , const ADV_3D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[3][3];
   Real d_ksi, d_eta, d_psi;
   int row;

   Real cell_size;
   Real cell_center[3];
   Real bG;

   /*AXES_IDX*/int axe1;
   /*AXES_IDX*/int axe2;
   /*AXES_IDX*/int axe3;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* calcul cell center */
   cell_size = element_size( e, elt, geom , cell_center);

   /* read the current selected axis from the Adv structure */
   axe1 = MyAdvFun->current_selected_axe1;
   axe2 = MyAdvFun->current_selected_axe2;
   axe3 = MyAdvFun->current_selected_axe3;

   bG = Adv3D_evalFunction(MyAdvFun, 0, axe1, axe2, axe3, cell_center[0], cell_center[1], cell_center[2] );

   /* get the transformation parameters */
   switch(axe2)
   {
      case AXEe_X: row = 0; break;
      case AXEe_Y: row = 1; break;
      case AXEe_Z: row = 2; break;
      default: error(E_UNKNOWN, "_systel_matrix_Conv_G_fun");
   }

   d_ksi =  transformation[row][0];
   d_eta =  transformation[row][1];
   d_psi =  transformation[row][2];

   /* initialisation Conv_x_el */
   m_zero(Conv_G_el);

   /* we have */
   Conv_G_el = m_add(Conv_G_el, sm_mlt( (jacobien*d_ksi),elt->MAT_I_x,work_el), Conv_G_el);
   Conv_G_el = m_add(Conv_G_el, sm_mlt( (jacobien*d_eta),elt->MAT_I_y,work_el), Conv_G_el);
   Conv_G_el = m_add(Conv_G_el, sm_mlt( (jacobien*d_psi),elt->MAT_I_z,work_el), Conv_G_el);

   Conv_G_el = sm_mlt( bG, Conv_G_el, Conv_G_el);

   /*
    _  _           _  _           _  _
    Conv_x_el = |Jac|.bx.(ksi_x.Ni_Nxj + eta_x.Ni_Nyj + psi_x.Ni_Nzj) ;
    Conv_y_el = |Jac|.by.(ksi_y.Ni_Nxj + eta_y.Ni_Nyj + psi_y.Ni_Nzj)) ;
    Conv_z_el = |Jac|.bz.(ksi_z.Ni_Nxj + eta_z.Ni_Nyj + psi_z.Ni_Nzj)) ;
    */


   /* free memory */
   M_FREE(work_el);

   return Conv_G_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_AUx   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *AUx )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_AUx");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_AUx");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_AUx");
   if ( AUx     == NULL ) error(E_NULL, "assemblage3D_matrix_AUx");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_AUx, elt, geom, a, AUx );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_AUy   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *AUy )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_AUy");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_AUy");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_AUy");
   if ( AUy     == NULL ) error(E_NULL, "assemblage3D_matrix_AUy");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_AUy, elt, geom, a, AUy );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_AUz   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *AUz )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_AUz");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_AUz");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_AUz");
   if ( AUz     == NULL ) error(E_NULL, "assemblage3D_matrix_AUz");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_AUz, elt, geom, a, AUz );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_AxU   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *AxU )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_AxU");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_AxU");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_AxU");
   if ( AxU     == NULL ) error(E_NULL, "assemblage3D_matrix_AxU");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_AxU, elt, geom, a, AxU );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_AyU   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *AyU )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_AyU");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_AyU");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_AyU");
   if ( AyU     == NULL ) error(E_NULL, "assemblage3D_matrix_AyU");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_AyU, elt, geom, a, AyU );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_AzU   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *AzU )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_AzU");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_AzU");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_AzU");
   if ( AzU     == NULL ) error(E_NULL, "assemblage3D_matrix_AzU");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_AzU, elt, geom, a, AzU );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage3D_matrix_A_U   ( const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *A_U )
{
   /* check */
   if ( elt     == NULL ) error(E_NULL, "assemblage3D_matrix_A_U");
   if ( geom    == NULL ) error(E_NULL, "assemblage3D_matrix_A_U");
   if ( a       == NULL ) error(E_NULL, "assemblage3D_matrix_A_U");
   if ( A_U     == NULL ) error(E_NULL, "assemblage3D_matrix_A_U");

   /* delegate */
   return _assemblage_matrix2 ( ASSEMBLAGEe_A_U, elt, geom, a, A_U );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT * _assemblage_matrix2( ASSEMBLAGEt_TYPE type , const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , SPMAT *A )
{
   u_int e;
   u_int i,j,k;

   MAT   *A_el;
   TENSOR *TENSOR_el;

   TENSOR* (*_systel_matrix_)( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M3D_el );

   /* check */
   if ( elt     == NULL ) error(E_NULL, "_assemblage_matrix2");
   if ( geom    == NULL ) error(E_NULL, "_assemblage_matrix2");
   if ( a       == NULL ) error(E_NULL, "_assemblage_matrix2");
   if ( A       == NULL ) error(E_NULL, "_assemblage_matrix2");

   switch( type )
   {
      case ASSEMBLAGEe_AUx : _systel_matrix_ = _systel_matrix_AUx ; break;
      case ASSEMBLAGEe_AUy : _systel_matrix_ = _systel_matrix_AUy ; break;
      case ASSEMBLAGEe_AUz : _systel_matrix_ = _systel_matrix_AUz ; break;
      case ASSEMBLAGEe_AxU : _systel_matrix_ = _systel_matrix_AxU ; break;
      case ASSEMBLAGEe_AyU : _systel_matrix_ = _systel_matrix_AyU ; break;
      case ASSEMBLAGEe_AzU : _systel_matrix_ = _systel_matrix_AzU ; break;
      case ASSEMBLAGEe_A_U : _systel_matrix_ = _systel_matrix_A_U ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix2");
   }

   /* mem alloc */
   A_el      =   m_get(elt->nb_somm_cell, elt->nb_somm_cell);
   TENSOR_el  = ts_get(elt->nb_somm_cell, elt->nb_somm_cell, elt->nb_somm_cell);

   /* initialisation de A */
   sp_zero(A);

   /* fill */
   for( e=0 ; e<geom->NBELMT ; e++ )
   {
      TENSOR_el = _systel_matrix_( e , elt , geom , TENSOR_el );

      m_zero(A_el);  /* init for each element "e" */

      for( i=0 ; i<elt->nb_somm_cell ; i++)
      for( j=0 ; j<elt->nb_somm_cell ; j++)
      {
         for (k=0; k<elt->nb_somm_cell ; k++)
         {
            A_el->me[i][j] += a->ve[geom->NSELMT->im[e][k]]*TENSOR_el->te[k][j][i] ;
         }

         sp_add_val( A, geom->NSELMT->im[e][i] , geom->NSELMT->im[e][j] , A_el->me[i][j] );
      }
   }

   M_FREE(A_el) ;
   TS_FREE(TENSOR_el) ;

   return A;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_AUx  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien ;

   Real transformation[3][3];
   Real ksi_x, eta_x, psi_x ;

   TENSOR *work_el;

   /* mem alloc */
   work_el = ts_get(elt->nb_somm_cell,elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* transformation parameters */
   ksi_x = transformation[0][0];
   eta_x = transformation[0][1];
   psi_x = transformation[0][2];

   /* init */
   M_el = ts_zero(M_el);

   ts_add( M_el, sts_mlt( ksi_x*jacobien, elt->TENSOR_I_x_I, work_el), M_el);
   ts_add( M_el, sts_mlt( eta_x*jacobien, elt->TENSOR_I_y_I, work_el), M_el);
   ts_add( M_el, sts_mlt( psi_x*jacobien, elt->TENSOR_I_z_I, work_el), M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   jacobien = 1.0;

   ksi_x = 1.0;
   eta_x = 1.0;
   psi_x = 1.0;

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
       {
          M_el->me[i][j][k]  += w3D[m]* ( ksi_x*jacobien*
                            ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                              elt->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                              elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                        + eta_x*jacobien*
                            ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                              elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                              elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                        + psi_x*jacobien*
                            ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                              elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                              elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) ) );
       }
   }
   sts_mlt( jacobien, M_el, M_el );
   m3_foutput(stdout, M_el);
   exit(0);
   */

   /* free mem */
   TS_FREE(work_el);

   /* return */
   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_AUy  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien ;

   Real transformation[3][3];
   Real ksi_y, eta_y, psi_y ;

   TENSOR *work_el;

   /* mem alloc */
   work_el = ts_get(elt->nb_somm_cell,elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* transformation parameters */
   ksi_y = transformation[1][0];
   eta_y = transformation[1][1];
   psi_y = transformation[1][2];

   /* init */
   ts_zero(M_el);

   ts_add( M_el, sts_mlt( ksi_y*jacobien, elt->TENSOR_I_x_I, work_el), M_el);
   ts_add( M_el, sts_mlt( eta_y*jacobien, elt->TENSOR_I_y_I, work_el), M_el);
   ts_add( M_el, sts_mlt( psi_y*jacobien, elt->TENSOR_I_z_I, work_el), M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         M_el->me[i][j][k]  += w3D[m]* ( ksi_y*jacobien
                           ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                        + eta_y*jacobien
                           ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                        + psi_y*jacobien
                           ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) ) );
      }
   }
   exit(0);
   */

   /* free mem */
   TS_FREE(work_el);

   /* return */
   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_AUz  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien ;

   Real transformation[3][3];
   Real ksi_z, eta_z, psi_z ;

   TENSOR *work_el;

   /* mem alloc */
   work_el = ts_get(elt->nb_somm_cell,elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* transformation parameters */
   ksi_z = transformation[2][0];
   eta_z = transformation[2][1];
   psi_z = transformation[2][2];


   /* init */
   ts_zero(M_el);

   ts_add( M_el, sts_mlt( ksi_z*jacobien, elt->TENSOR_I_x_I, work_el), M_el);
   ts_add( M_el, sts_mlt( eta_z*jacobien, elt->TENSOR_I_y_I, work_el), M_el);
   ts_add( M_el, sts_mlt( psi_z*jacobien, elt->TENSOR_I_z_I, work_el), M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         M_el->me[i][j][k]  += w3D[m]* ( ksi_z*jacobien
                           ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + eta_z*jacobien
                           ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + psi_z*jacobien
                           ( elt->f_base[i]   (ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                             elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) ) );
      }
   }
   exit(0);
   */

   /* free mem */
   TS_FREE(work_el);

   /* return */
   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_AxU  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien  ;

   Real transformation[3][3];
   Real ksi_x, eta_x, psi_x ;

   TENSOR *work_el;

   /* mem alloc */
   work_el = ts_get(elt->nb_somm_cell,elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* transformation parameters */
   ksi_x = transformation[0][0];
   eta_x = transformation[0][1];
   psi_x = transformation[0][2];

   /* init */
   ts_zero(M_el);

   ts_add( M_el, sts_mlt( ksi_x*jacobien, elt->TENSOR_x_I_I, work_el), M_el);
   ts_add( M_el, sts_mlt( eta_x*jacobien, elt->TENSOR_y_I_I, work_el), M_el);
   ts_add( M_el, sts_mlt( psi_x*jacobien, elt->TENSOR_z_I_I, work_el), M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         M_el->me[i][j][k]  += w3D[m]* ( ksi_x*
                                 ( elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + eta_x*
                                 ( elt->dfdy_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + psi_x*
                                 ( elt->dfdz_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) ) );
       }
    }

   sts_mlt( jacobien, M_el, M_el );
   exit(0);
   */

   /* free mem */
   TS_FREE(work_el);

   /* return */
   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_AyU  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien  ;

   Real transformation[3][3];
   Real ksi_y, eta_y, psi_y ;

   TENSOR *work_el;

   /* mem alloc */
   work_el = ts_get(elt->nb_somm_cell,elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* transformation parameters */
   ksi_y = transformation[1][0];
   eta_y = transformation[1][1];
   psi_y = transformation[1][2];


   /* init */
   ts_zero(M_el);

   ts_add( M_el, sts_mlt( ksi_y*jacobien, elt->TENSOR_x_I_I, work_el), M_el);
   ts_add( M_el, sts_mlt( eta_y*jacobien, elt->TENSOR_y_I_I, work_el), M_el);
   ts_add( M_el, sts_mlt( psi_y*jacobien, elt->TENSOR_z_I_I, work_el), M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         M_el->me[i][j][k]  += w3D[m]* ( ksi_y*jacobien
                                 ( elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + eta_y*jacobien
                                 ( elt->dfdy_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + psi_y*jacobien
                                 ( elt->dfdz_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) ) );
      }
   }
   exit(0);
   */

   /* free mem */
   TS_FREE(work_el);

   /* return */
   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_AzU  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien  ;

   Real transformation[3][3];
   Real ksi_z, eta_z, psi_z ;

   TENSOR *work_el;

   /* mem alloc */
   work_el = ts_get(elt->nb_somm_cell,elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* transformation parameters */
   ksi_z = transformation[2][0];
   eta_z = transformation[2][1];
   psi_z = transformation[2][2];


   /* init */
   ts_zero(M_el);

   ts_add( M_el, sts_mlt( ksi_z*jacobien, elt->TENSOR_x_I_I, work_el), M_el);
   ts_add( M_el, sts_mlt( eta_z*jacobien, elt->TENSOR_y_I_I, work_el), M_el);
   ts_add( M_el, sts_mlt( psi_z*jacobien, elt->TENSOR_z_I_I, work_el), M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         M_el->me[i][j][k]  += w3D[m]* ( ksi_z*jacobien
                                 ( elt->dfdx_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + eta_z*jacobien
                                 ( elt->dfdy_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) )
                                       + psi_z*jacobien
                                 ( elt->dfdz_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[j]   (ksi3D[m],eta3D[m],psi3D[m]) *
                                   elt->f_base[k]   (ksi3D[m],eta3D[m],psi3D[m]) ) );
      }
   }
   exit(0);
   */

   /* free mem */
   TS_FREE(work_el);

   /* return */
   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR*  _systel_matrix_A_U  ( u_int e , const ELT_3D *elt , const GEOM_3D *geom , TENSOR* M_el )
{
   Real jacobien  ;

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e ( e, elt, geom );

   /* init */
   ts_zero(M_el);

   /* exact integration */
   M_el = sts_mlt( jacobien, elt->TENSOR_I_I_I, M_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         M_el->me[i][j][k]  += w3D[m]* (  elt->f_base[i](ksi3D[m],eta3D[m],psi3D[m]) *
                                          elt->f_base[j](ksi3D[m],eta3D[m],psi3D[m]) *
                                          elt->f_base[k](ksi3D[m],eta3D[m],psi3D[m]) ) ;
      }
   }
   */
   /* don't forget the jacobian */
   /*sts_mlt( jacobien, M_el, M_el );*/


   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      if ( fabs( M_el->me[i][j][k] ) < 1.0e-8 )  M_el->me[i][j][k] = 0.0;
   }
   m3_foutput(stdout, M_el);
   exit(0);
   */

   return M_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage3D_vector_fun( const ELT_3D *elt , const GEOM_3D *geom , const RHS_3D *RhsFun , VEC *RHS )
{
   int e; int i;

   VEC *RHS_el;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_vector_Rhs");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_vector_Rhs");
   if ( RhsFun == NULL ) error(E_NULL, "assemblage3D_vector_Rhs");
   if ( RHS    == NULL ) error(E_NULL, "assemblage3D_vector_Rhs");

   /* initialisation of Mass */
   v_zero(RHS);

   
   if (( RhsFun->Fun[0][0].type == FUN_C_STATIONNARY && RhsFun->Fun[0][0].phi_xyz == Zero3D) ||
       ( RhsFun->Fun[0][0].type == FUN_C_TRANSIENT && RhsFun->Fun[0][0].phi_xyzt == Zero3D_Transient))
   {
      return RHS;
   }
   

   /* mem alloc */
   RHS_el = v_get(elt->nb_somm_cell);
   
   for(e=0; e<geom->NBELMT; e++)
   {
      RHS_el = _systel_vector_fun( e , elt , geom , RhsFun , RHS_el );

      for (i=0; i<elt->nb_somm_cell; i++)
      {
         RHS->ve[geom->NSELMT->im[e][i]] += RHS_el->ve[i] ;
      }
   }

   V_FREE(RHS_el) ;

   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_fun( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , const RHS_3D *RhsFun , VEC *Rhs_el )
{
   int  m,k;

   Real xx[NBPTS_GAUSS_3D_TRI],
        yy[NBPTS_GAUSS_3D_TRI],
        zz[NBPTS_GAUSS_3D_TRI];

   Real x1,x2,x3,x4 ;
   Real y1,y2,y3,y4 ;
   Real z1,z2,z3,z4 ;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   Real jacobien ;


   int axe = RhsFun->current_selected_axe;
   /* check */
   if ( (axe != AXEe_X) && (axe != AXEe_Y) && (axe != AXEe_Z) ) error6(E_RHS_WRONGAXENUMBER, "_systel_vector_Rhs");

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e ( e, elt, geom );

   /* get coordinates of the tetrahedra on elt "e" */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;
   x4 = XYSOMM->me[ NSELMT->im[e][3] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;
   y4 = XYSOMM->me[ NSELMT->im[e][3] ][1] ;

   z1 = XYSOMM->me[ NSELMT->im[e][0] ][2] ;
   z2 = XYSOMM->me[ NSELMT->im[e][1] ][2] ;
   z3 = XYSOMM->me[ NSELMT->im[e][2] ][2] ;
   z4 = XYSOMM->me[ NSELMT->im[e][3] ][2] ;

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
   {
      xx[m] = x1 + (x2-x1)*ksi3D[m] + (x3-x1)*eta3D[m] + (x4-x1)*psi3D[m] ;
      yy[m] = y1 + (y2-y1)*ksi3D[m] + (y3-y1)*eta3D[m] + (y4-y1)*psi3D[m] ;
      zz[m] = z1 + (z2-z1)*ksi3D[m] + (z3-z1)*eta3D[m] + (z4-z1)*psi3D[m] ;

      for (k=0; k<elt->nb_somm_cell; k++)
      {
         Rhs_el->ve[k] += w3D[m]*
            Rhs3D_evalFunction( RhsFun,0/*ref_e*/, axe, xx[m],yy[m],zz[m])* /* jacobien later */
            elt->f_base[k](ksi3D[m],eta3D[m],psi3D[m]) ;
      }
   }

   Rhs_el = sv_mlt(jacobien, Rhs_el, Rhs_el);

   /* If the Source is a constant function : */
   /*
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      SCM_el->ve[k] = Rhs3D_evalFunction( RhsFun, 0, axe, 0.0,0.0,0.0) * jacobien * elt->VEC_Scmbr->ve[k] ;
   }
   */

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC*    assemblage3D_vector_abx( const ELT_3D *elt , const GEOM_3D *geom , const VEC* a , const VEC* b , VEC *RHS )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_vector_abx");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_vector_abx");
   if ( a      == NULL ) error(E_NULL, "assemblage3D_vector_abx");
   if ( b      == NULL ) error(E_NULL, "assemblage3D_vector_abx");
   if ( RHS    == NULL ) error(E_NULL, "assemblage3D_vector_abx");

   /* delegate */
   return _assemblage_vector2( ASSEMBLAGEe_ABx , elt , geom , a , b ,  RHS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage3D_vector_aby( const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *RHS )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_vector_aby");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_vector_aby");
   if ( a      == NULL ) error(E_NULL, "assemblage3D_vector_aby");
   if ( b      == NULL ) error(E_NULL, "assemblage3D_vector_aby");
   if ( RHS    == NULL ) error(E_NULL, "assemblage3D_vector_aby");

   /* delegate */
   return _assemblage_vector2( ASSEMBLAGEe_ABy , elt , geom , a , b ,  RHS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage3D_vector_abz( const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *RHS )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_vector_abz");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_vector_abz");
   if ( a      == NULL ) error(E_NULL, "assemblage3D_vector_abz");
   if ( b      == NULL ) error(E_NULL, "assemblage3D_vector_abz");
   if ( RHS    == NULL ) error(E_NULL, "assemblage3D_vector_abz");

   /* delegate */
   return _assemblage_vector2( ASSEMBLAGEe_ABz , elt , geom , a , b ,  RHS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage3D_vector_ab( const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_vector_ab");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_vector_ab");
   if ( a      == NULL ) error(E_NULL, "assemblage3D_vector_ab");
   if ( b      == NULL ) error(E_NULL, "assemblage3D_vector_ab");
   if ( RHS    == NULL ) error(E_NULL, "assemblage3D_vector_ab");

   /* delegate */
   return _assemblage_vector2( ASSEMBLAGEe_A_B , elt , geom , a , b ,  RHS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC * _assemblage_vector2( ASSEMBLAGEt_TYPE type , const ELT_3D *elt , const GEOM_3D *geom, const VEC *a , const VEC *b, VEC *RHS )
{
   int e; int i;

   VEC *RHS_el;
   VEC* (*_systel_vector_)( u_int e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b, VEC *Rhs_el );

   /* check */
   if ( elt    == NULL ) error(E_NULL, "_assemblage_vector2");
   if ( geom   == NULL ) error(E_NULL, "_assemblage_vector2");
   if ( a      == NULL ) error(E_NULL, "_assemblage_vector2");
   if ( b      == NULL ) error(E_NULL, "_assemblage_vector2");
   if ( RHS    == NULL ) error(E_NULL, "_assemblage_vector2");

   switch( type )
   {
      case ASSEMBLAGEe_ABx : _systel_vector_ = _systel_vector_abx ; break;
      case ASSEMBLAGEe_ABy : _systel_vector_ = _systel_vector_aby ; break;
      case ASSEMBLAGEe_ABz : _systel_vector_ = _systel_vector_abz ; break;
      case ASSEMBLAGEe_A_B : _systel_vector_ = _systel_vector_ab  ; break;

      default: error(E_UNKNOWN, "_assemblage_vector2");
   }

   /* alloc mem */
   RHS_el = v_get(elt->nb_somm_cell);

   /* initialisation of RHS */
   v_zero(RHS);

   for(e=0; e<geom->NBELMT; e++)
   {
      RHS_el = _systel_vector_( e , elt , geom , a , b , RHS_el );

      for (i=0; i<elt->nb_somm_cell; i++)
      {
         RHS->ve[geom->NSELMT->im[e][i]] += RHS_el->ve[i] ;
      }
   }

   V_FREE(RHS_el) ;

   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_abx( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b, VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                             elt->nb_somm_cell,
                             elt->nb_somm_cell);

   /* init */
   TENSOR_el = _systel_matrix_AUx( e , elt , geom , TENSOR_el );

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   /* We write the componant of U.Ux at time step "n-q"  */
   /* in a matrix called Componants_Unmq_MAT2D           */
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      ab->me[i][j] = a->ve[geom->NSELMT->im[e][i]]*
                     b->ve[geom->NSELMT->im[e][j]];
   }

   /* Final Calculus : SCM_NL_e = TENSOR*ab_MAT2D   */
   tsm_mlt(TENSOR_el, ab, Rhs_el);

   M_FREE(ab);
   TS_FREE(TENSOR_el);

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_aby( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b, VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                             elt->nb_somm_cell,
                             elt->nb_somm_cell);

   /* init */
   TENSOR_el = _systel_matrix_AUy( e , elt , geom , TENSOR_el );

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   /* We write the componant of U.Ux at time step "n-q"  */
   /* in a matrix called Componants_Unmq_MAT2D           */
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      ab->me[i][j] = a->ve[geom->NSELMT->im[e][i]]*
                     b->ve[geom->NSELMT->im[e][j]];
   }

   /* Final Calculus : SCM_NL_e = TENSOR*ab_MAT2D   */
   tsm_mlt(TENSOR_el ,ab, Rhs_el);

   M_FREE(ab);
   TS_FREE(TENSOR_el);

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_abz( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b, VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                             elt->nb_somm_cell,
                             elt->nb_somm_cell);

   /* init */
   TENSOR_el = _systel_matrix_AUz( e , elt , geom , TENSOR_el );

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   /* We write the componant of U.Ux at time step "n-q"  */
   /* in a matrix called Componants_Unmq_MAT2D           */
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      ab->me[i][j] = a->ve[geom->NSELMT->im[e][i]]*
                     b->ve[geom->NSELMT->im[e][j]];
   }

   /* Final Calculus : SCM_NL_e = TENSOR*ab_MAT2D   */
   tsm_mlt(TENSOR_el ,ab, Rhs_el);

   M_FREE(ab);
   TS_FREE(TENSOR_el);

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_ab ( u_int  e , const ELT_3D *elt , const GEOM_3D *geom , const VEC *a , const VEC *b , VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                              elt->nb_somm_cell,
                              elt->nb_somm_cell);

   /* init */
   TENSOR_el = _systel_matrix_A_U( e , elt , geom , TENSOR_el );

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   /* We write the componant of U.Ux at time step "n-q"  */
   /* in a matrix called Componants_Unmq_MAT2D           */
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      ab->me[i][j] = a->ve[geom->NSELMT->im[e][i]]*
                     b->ve[geom->NSELMT->im[e][j]];
   }

   /* Final Calculus : SCM_NL_e = TENSOR*ab_MAT2D   */
   tsm_mlt(TENSOR_el, ab, Rhs_el);

   M_FREE(ab);
   TS_FREE(TENSOR_el);

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT *_assemblage_matrix3( ASSEMBLAGEt_TYPE type , const ELT_3D *elt2 , const GEOM_3D *geom2 , const ELT_3D *elt1 , const GEOM_3D *geom1 , SPMAT *B_ )
{
   int e;
   int i,j;

   MAT *B_el;
   MAT* (*_systel_matrix_)( u_int e , const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                      const ELT_3D *elt1 , const GEOM_3D *geom1 , MAT *B_el );

   /* check */
   if ( elt2    == NULL ) error(E_NULL, "_assemblage_matrix3");
   if ( geom2   == NULL ) error(E_NULL, "_assemblage_matrix3");
   if ( elt1    == NULL ) error(E_NULL, "_assemblage_matrix3");
   if ( geom1   == NULL ) error(E_NULL, "_assemblage_matrix3");
   if ( B_      == NULL ) error(E_NULL, "_assemblage_matrix3");
   /* test compatibility geom1 and geom 2 */
   if ( geom1->NBELMT != geom2->NBELMT ) error(E_SIZES, "_assemblage_matrix3");

   switch( type )
   {
      case ASSEMBLAGEe_STOKES_Bx : _systel_matrix_ = _systel_matrix_Conv_x_elt2elt1 ; break;
      case ASSEMBLAGEe_STOKES_By : _systel_matrix_ = _systel_matrix_Conv_y_elt2elt1 ; break;
      case ASSEMBLAGEe_STOKES_Bz : _systel_matrix_ = _systel_matrix_Conv_z_elt2elt1 ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix3");
   }

   /* mem alloc */
   B_el = m_get(elt1->nb_somm_cell,elt2->nb_somm_cell);

   /* initialisation of Bx   */
   sp_zero(B_);

   for(e=0; e<geom1->NBELMT; e++)
   {
      /* printf("triangle num %d \n",e); */

      B_el = _systel_matrix_( e , elt2 , geom2 , elt1 , geom1 , B_el );

      for (i=0; i<elt1->nb_somm_cell; i++)
      for (j=0; j<elt2->nb_somm_cell; j++)
      {
         /*
         printf("(i,j)_el = (%d,%d) \n",i,j);
         printf("(i,j)    = (%d,%d) \n",geom->geomBase->NSELMT->im[e][i],geom->NSELMT->im[e][j]);
         */

         sp_add_val( B_  , geom1->NSELMT->im[e][i] , geom2->NSELMT->im[e][j] , B_el->me[i][j] );
      }
   }


   M_FREE(B_el) ;

   return sp_compact(B_, MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT *assemblage3D_matrix_Conv_x_elt2elt1( const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                            const ELT_3D *elt1 , const GEOM_3D *geom1 , SPMAT *Bx )
{
   /* check */
   if ( elt2    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_elt2elt1");
   if ( geom2   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_elt2elt1");
   if ( elt1    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_elt2elt1");
   if ( geom1   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_elt2elt1");
   if ( Bx      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_x_elt2elt1");
   /* test compatibility geom1 and geom 2 */
   if ( geom1->NBELMT != geom2->NBELMT ) error(E_SIZES, "assemblage3D_matrix_Conv_x_elt2elt1");

   /* delegate */
   return _assemblage_matrix3( ASSEMBLAGEe_STOKES_Bx , elt2 , geom2 , elt1 , geom1 , Bx );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT *assemblage3D_matrix_Conv_y_elt2elt1( const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                            const ELT_3D *elt1 , const GEOM_3D *geom1 , SPMAT *By )
{
   /* check */
   if ( elt2    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_elt2elt1");
   if ( geom2   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_elt2elt1");
   if ( elt1    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_elt2elt1");
   if ( geom1   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_elt2elt1");
   if ( By      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_y_elt2elt1");
   /* test compatibility geom1 and geom 2 */
   if ( geom1->NBELMT != geom2->NBELMT ) error(E_SIZES, "assemblage3D_matrix_Conv_y_elt2elt1");

   /* delegate */
   return _assemblage_matrix3( ASSEMBLAGEe_STOKES_By , elt2 , geom2 , elt1 , geom1 , By );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT *assemblage3D_matrix_Conv_z_elt2elt1( const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                            const ELT_3D *elt1 , const GEOM_3D *geom1 , SPMAT *Bz )
{
   /* check */
   if ( elt2    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z_elt2elt1");
   if ( geom2   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z_elt2elt1");
   if ( elt1    == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z_elt2elt1");
   if ( geom1   == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z_elt2elt1");
   if ( Bz      == NULL ) error(E_NULL, "assemblage3D_matrix_Conv_z_elt2elt1");
   /* test compatibility geom1 and geom 2 */
   if ( geom1->NBELMT != geom2->NBELMT ) error(E_SIZES, "assemblage3D_matrix_Conv_z_elt2elt1");

   /* delegate */
   return _assemblage_matrix3( ASSEMBLAGEe_STOKES_Bz , elt2 , geom2 , elt1 , geom1 , Bz );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MAT* _systel_matrix_Conv_x_elt2elt1( u_int e , const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                               const ELT_3D *elt1 , const GEOM_3D *geom1 , MAT *Bx_el )
{
   Real jacobien ;

   Real transformation[3][3];
   Real ksi_x, eta_x, psi_x ;

   MAT* work_el = m_get(elt1->nb_somm_cell,elt2->nb_somm_cell);


   /* init Bx_el */
   m_zero(Bx_el);

   /* calcul jacobien on elt "e" */
   jacobien =  eval_jac_on_e_ ( e, elt1, geom1, transformation );

   /* transformation parameters */
   ksi_x = transformation[0][0]; /*printf("tri %d : ksi_x = %lf \n", e, ksi_x);*/
   eta_x = transformation[0][1]; /*printf("tri %d : eta_x = %lf \n", e, eta_x);*/
   psi_x = transformation[0][2]; /*printf("tri %d : psi_x = %lf \n", e, psi_x);*/

   /* init Bx_el */
   m_zero(Bx_el);

   /* with exact integration */
   Bx_el = m_add(Bx_el, sm_mlt( (jacobien*ksi_x), elt2->MAT_I_x_PM1dP,work_el), Bx_el);
   Bx_el = m_add(Bx_el, sm_mlt( (jacobien*eta_x), elt2->MAT_I_y_PM1dP,work_el), Bx_el);
   Bx_el = m_add(Bx_el, sm_mlt( (jacobien*psi_x), elt2->MAT_I_z_PM1dP,work_el), Bx_el);

   /* with numerical integration  with the basis functions */
   /*
   m_zero(work_el);

   for (i=0; i<elt1->nb_somm_cell; i++)
   for (j=0; j<elt2->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
      {
         work_el->me[i][j]  += w3D[m]*jacobien*
             elt1-> f_base[i](ksi3D[m],eta3D[m],psi3D[m])* (
                  ksi_x*elt2->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
                  eta_x*elt2->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
                  psi_x*elt2->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) );
       }
   }

   m_sub(Bx_el,work_el,work_el);

   for (i=0; i<elt1->nb_somm_cell; i++)
   for (j=0; j<elt2->nb_somm_cell; j++)
   {
      if ( fabs(work_el->me[i][j]) < 1.0e-5 )
      {
         work_el->me[i][j] = 0.0;
      }
   }
   m_foutput(stdout, work_el);
   */

   M_FREE(work_el);

   return Bx_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MAT* _systel_matrix_Conv_y_elt2elt1( u_int e , const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                               const ELT_3D *elt1 , const GEOM_3D *geom1 , MAT *By_el )
{
   Real jacobien ;

   Real transformation[3][3];
   Real ksi_y, eta_y, psi_y ;

   MAT* work_el = m_get(elt1->nb_somm_cell,elt2->nb_somm_cell);


   /* calcul jacobien on elt "e" */
   jacobien =  eval_jac_on_e_ ( e, elt1, geom1, transformation );

   /* transformation parameters */
   ksi_y = transformation[1][0]; /*printf("tri %d : ksi_y = %lf \n", e, ksi_y);*/
   eta_y = transformation[1][1]; /*printf("tri %d : eta_y = %lf \n", e, eta_y);*/
   psi_y = transformation[1][2]; /*printf("tri %d : psi_y = %lf \n", e, psi_y);*/

   /* init By_el */
   m_zero(By_el);

   /* with exact integration */
   By_el = m_add(By_el, sm_mlt( (jacobien*ksi_y), elt2->MAT_I_x_PM1dP,work_el), By_el);
   By_el = m_add(By_el, sm_mlt( (jacobien*eta_y), elt2->MAT_I_y_PM1dP,work_el), By_el);
   By_el = m_add(By_el, sm_mlt( (jacobien*psi_y), elt2->MAT_I_z_PM1dP,work_el), By_el);

   /* with numerical integration  with the basis functions */
   /*
    m_zero(work_el);

    for (i=0; i<elt1->nb_somm_cell; i++)
    for (j=0; j<elt2->nb_somm_cell; j++)
    {
       for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
       {
          work_el->me[i][j]  += w3D[m]*jacobien*
          elt1-> f_base[i](ksi3D[m],eta3D[m],psi3D[m])* (
               ksi_y*elt2->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
               eta_y*elt2->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
               psi_y*elt2->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) );
       }
    }

    m_sub(By_el,work_el,work_el);

    for (i=0; i<elt1->nb_somm_cell; i++)
    for (j=0; j<elt2->nb_somm_cell; j++)
    {
       if ( fabs(work_el->me[i][j]) < 1.0e-5 )
       {
          work_el->me[i][j] = 0.0;
       }
    }
    m_foutput(stdout, work_el);
    */

   M_FREE(work_el);

   return By_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

MAT* _systel_matrix_Conv_z_elt2elt1( u_int e , const ELT_3D *elt2 , const GEOM_3D *geom2 ,
                                               const ELT_3D *elt1 , const GEOM_3D *geom1 , MAT *Bz_el )
{
   Real jacobien ;

   Real transformation[3][3];
   Real ksi_z, eta_z, psi_z ;

   MAT* work_el = m_get(elt1->nb_somm_cell,elt2->nb_somm_cell);


   /* calcul jacobien on elt "e" */
   jacobien =  eval_jac_on_e_ ( e, elt1, geom1, transformation );

   /* transformation parameters */
   ksi_z = transformation[2][0]; /*printf("tri %d : ksi_z = %lf \n", e, ksi_z);*/
   eta_z = transformation[2][1]; /*printf("tri %d : eta_z = %lf \n", e, eta_z);*/
   psi_z = transformation[2][2]; /*printf("tri %d : psi_z = %lf \n", e, psi_z);*/


   /* init Bz_el */
   m_zero(Bz_el);

   /* with exact integration */
   Bz_el = m_add(Bz_el, sm_mlt( (jacobien*ksi_z), elt2->MAT_I_x_PM1dP,work_el), Bz_el);
   Bz_el = m_add(Bz_el, sm_mlt( (jacobien*eta_z), elt2->MAT_I_y_PM1dP,work_el), Bz_el);
   Bz_el = m_add(Bz_el, sm_mlt( (jacobien*psi_z), elt2->MAT_I_z_PM1dP,work_el), Bz_el);

   /* with numerical integration  with the basis functions */
   /*
    m_zero(work_el);

    for (i=0; i<elt1->nb_somm_cell; i++)
    for (j=0; j<elt2->nb_somm_cell; j++)
    {
       for (m=0; m<NBPTS_GAUSS_3D_TRI; m++)
       {
          work_el->me[i][j]  += w3D[m]*jacobien*
          elt1-> f_base[i](ksi3D[m],eta3D[m],psi3D[m])* (
               ksi_z*elt2->dfdx_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
               eta_z*elt2->dfdy_base[j](ksi3D[m],eta3D[m],psi3D[m]) +
               psi_z*elt2->dfdz_base[j](ksi3D[m],eta3D[m],psi3D[m]) );
       }
    }

    m_sub(Bz_el,work_el,work_el);

    for (i=0; i<elt1->nb_somm_cell; i++)
    for (j=0; j<elt2->nb_somm_cell; j++)
    {
       if ( fabs(work_el->me[i][j]) < 1.0e-5 )
       {
          work_el->me[i][j] = 0.0;
       }
    }
    m_foutput(stdout, work_el);
    */

   M_FREE(work_el);

   return Bz_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real eval_jac_on_e  ( int e , const ELT_3D *element , const GEOM_3D *geom )
{
   /*                    _

   TRANSFORMATION DE  T  a  Ti : (ksi,eta.psi) coords on reference triangle :

   x = x1 + (x2-x1)*ksi + (x3-x1)*eta + (x4-x1)*psi ;
   y = y1 + (y2-y1)*ksi + (y3-y1)*eta + (y4-y1)*psi;
   z = z1 + (z2-z1)*ksi + (z3-z1)*eta + (z4-z1)*psi;

   _
   TRANSFORMATION DE  Ti  a  T :

   ksi = [ c1  + det(A11).x  - det(A21).y  + det(A31).z  ]/jacobien
   eta = [ c2  - det(A12).x  + det(A22).y  - det(A32).z  ]/jacobien
   psi = [ c2  + det(A13).x  - det(A23).y  + det(A33).z  ]/jacobien


   JACOBIEN :

   det(A) = + (x2-x1)[(y3-y1)(z4-z1) - (z3-z1)(y4-y1)]
            - (y2-y1)[(x3-x1)(z4-z1) - (z3-z1)(x4-x1)]
            + (z2-z1)[(x3-x1)(y4-y1) - (y3-y1)(x4-x1)]     > 0


   NOTE :

   AIJ = A without line I and column J

   */

   Real x1,x2,x3,x4 ;
   Real y1,y2,y3,y4 ;
   Real z1,z2,z3,z4 ;

   Real jacobien ;

   Real A_11, A_12, A_13;
   Real A_21, A_22, A_23;
   Real A_31, A_32, A_33;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   /* calcul jacobien on elt "e" */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;
   x4 = XYSOMM->me[ NSELMT->im[e][3] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;
   y4 = XYSOMM->me[ NSELMT->im[e][3] ][1] ;

   z1 = XYSOMM->me[ NSELMT->im[e][0] ][2] ;
   z2 = XYSOMM->me[ NSELMT->im[e][1] ][2] ;
   z3 = XYSOMM->me[ NSELMT->im[e][2] ][2] ;
   z4 = XYSOMM->me[ NSELMT->im[e][3] ][2] ;


   A_11 = (y3-y1)*(z4-z1) - (z3-z1)*(y4-y1);
   A_21 = (x3-x1)*(z4-z1) - (z3-z1)*(x4-x1);
   A_31 = (x3-x1)*(y4-y1) - (y3-y1)*(x4-x1);

   A_12 = (y2-y1)*(z4-z1) - (z2-z1)*(y4-y1);
   A_22 = (x2-x1)*(z4-z1) - (z2-z1)*(x4-x1);
   A_32 = (x2-x1)*(y4-y1) - (y2-y1)*(x4-x1);

   A_13 = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1);
   A_23 = (x2-x1)*(z3-z1) - (z2-z1)*(x3-x1);
   A_33 = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);


   jacobien = (x2-x1) * A_11
            - (y2-y1) * A_21
            + (z2-z1) * A_31 ;

   assert( jacobien > 0.0 );

   return jacobien;
}

static Real eval_jac_on_e_ ( int e , const ELT_3D *element , const GEOM_3D *geom , Real transformation[3][3])
{
   /*
                                   [ ksi_x , eta_x , psi_x ]
    transformation is a 3x3 matrix [ ksi_y , eta_y , psi_y ]
                                   [ ksi_z , eta_z , psi_z ]

   */

   Real x1,x2,x3,x4 ;
   Real y1,y2,y3,y4 ;
   Real z1,z2,z3,z4 ;

   Real jacobien, un_sur_jacobien ;

   Real A_11, A_12, A_13;
   Real A_21, A_22, A_23;
   Real A_31, A_32, A_33;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   /* calcul jacobien on elt "e" */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;
   x4 = XYSOMM->me[ NSELMT->im[e][3] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;
   y4 = XYSOMM->me[ NSELMT->im[e][3] ][1] ;

   z1 = XYSOMM->me[ NSELMT->im[e][0] ][2] ;
   z2 = XYSOMM->me[ NSELMT->im[e][1] ][2] ;
   z3 = XYSOMM->me[ NSELMT->im[e][2] ][2] ;
   z4 = XYSOMM->me[ NSELMT->im[e][3] ][2] ;


   A_11 = (y3-y1)*(z4-z1) - (z3-z1)*(y4-y1);
   A_21 = (x3-x1)*(z4-z1) - (z3-z1)*(x4-x1);
   A_31 = (x3-x1)*(y4-y1) - (y3-y1)*(x4-x1);

   A_12 = (y2-y1)*(z4-z1) - (z2-z1)*(y4-y1);
   A_22 = (x2-x1)*(z4-z1) - (z2-z1)*(x4-x1);
   A_32 = (x2-x1)*(y4-y1) - (y2-y1)*(x4-x1);

   A_13 = (y2-y1)*(z3-z1) - (z2-z1)*(y3-y1);
   A_23 = (x2-x1)*(z3-z1) - (z2-z1)*(x3-x1);
   A_33 = (x2-x1)*(y3-y1) - (y2-y1)*(x3-x1);


   jacobien = (x2-x1) * A_11
            - (y2-y1) * A_21
            + (z2-z1) * A_31 ;

   assert( jacobien > 0.0 );

   un_sur_jacobien = 1.0 / jacobien ;

   /* the transformation */
   transformation[0][0] = /* ksi_x */   A_11 * un_sur_jacobien;
   transformation[1][0] = /* ksi_y */ - A_21 * un_sur_jacobien;
   transformation[2][0] = /* ksi_z */   A_31 * un_sur_jacobien;

   transformation[0][1] = /* eta_x */ - A_12 * un_sur_jacobien;
   transformation[1][1] = /* eta_y */   A_22 * un_sur_jacobien;
   transformation[2][1] = /* eta_z */ - A_32 * un_sur_jacobien;

   transformation[0][2] = /* psi_x */   A_13 * un_sur_jacobien;
   transformation[1][2] = /* psi_y */ - A_23 * un_sur_jacobien;
   transformation[2][2] = /* psi_z */   A_33 * un_sur_jacobien;


   /* return */
   return jacobien;

   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>  test */
   /*
    ksi = 1.0;
    eta = 2.0;
    psi = 3.0;

    x = a_11*ksi + a_12*eta + a_13*psi ;
    y = a_21*ksi + a_22*eta + a_23*psi ;
    z = a_31*ksi + a_32*eta + a_33*psi ;

    j1 = (    (A_11)*x  - (A_21)*y  + (A_31)*z  ) * un_sur_jacobien;
    j2 = (  - (A_12)*x  + (A_22)*y  - (A_32)*z  ) * un_sur_jacobien;
    j3 = (    (A_13)*x  - (A_23)*y  + (A_33)*z  ) * un_sur_jacobien;
    */

   /*printf( "j1=%lf  j2=%lf  j3=%lf \n", j1,j2,j3);*/
   /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< test end */
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real element_size( int e, const ELT_3D *elt, const GEOM_3D *geom , Real center[3] )
{
   Real elt_size;
   Real elt_size1;
   Real elt_size2;

   Real x1,x2,x3,x4 ;
   Real y1,y2,y3,y4 ;
   Real z1,z2,z3,z4 ;

   Real surfABC ;
   Real surfABD ;
   Real surfACD ;
   Real surfBCD ;

   MAT*  XYZSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT  = geom->NSELMT; /* alias */

   /* calcul jacobien */
   x1 = XYZSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYZSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYZSOMM->me[ NSELMT->im[e][2] ][0] ;
   x4 = XYZSOMM->me[ NSELMT->im[e][3] ][0] ;

   y1 = XYZSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYZSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYZSOMM->me[ NSELMT->im[e][2] ][1] ;
   y4 = XYZSOMM->me[ NSELMT->im[e][3] ][1] ;

   z1 = XYZSOMM->me[ NSELMT->im[e][0] ][2] ;
   z2 = XYZSOMM->me[ NSELMT->im[e][1] ][2] ;
   z3 = XYZSOMM->me[ NSELMT->im[e][2] ][2] ;
   z4 = XYZSOMM->me[ NSELMT->im[e][3] ][2] ;


   surfABC = fabs( (y3-y1)*(x2-x1) - (x3-x1)*(y2-y1) );
   surfABD = fabs( (y4-y1)*(x2-x1) - (x4-x1)*(y2-y1) );
   surfACD = fabs( (y4-y1)*(x3-x1) - (x4-x1)*(y3-y1) );
   surfBCD = fabs( (y4-y2)*(x3-x2) - (x4-x2)*(y3-y2) );

   elt_size1 = ( surfABC > surfABD ? surfABC : surfABD );
   elt_size2 = ( surfACD > surfBCD ? surfACD : surfBCD );
   elt_size  = ( elt_size1 > elt_size2 ? elt_size1 : elt_size2 );

   /* center */
   center[0] = ( x1+x2+x3+x4 )/4.0;
   center[1] = ( y1+y2+y3+y4 )/4.0;
   center[2] = ( z1+z2+z3+z4 )/4.0;

   /* return */
   return sqrt(elt_size);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT * assemblage3D_matrix_fromBC(const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *BC, SPMAT *MassBC)
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_matrix_fromBC");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_matrix_fromBC");
   if ( BC     == NULL ) error(E_NULL, "assemblage3D_matrix_fromBC");
   if ( MassBC == NULL ) error(E_NULL, "assemblage3D_matrix_fromBC");

   axe = BC->BC_cauchy->current_selected_axe;
   /* conditions aux limites de Cauchy */
   if ( Bc3D_hasCauchyBC(BC, axe) )
   {
      apply_Bc3D_cauchy_on_mat  (axe, elt, geom, BC, MassBC);
   }

   axe = BC->BC_robin->current_selected_axe;
   /* conditions aux limites de Robin */
   if ( Bc3D_hasRobinBC(BC, axe) )
   {
      apply_Bc3D_robin_on_mat  (axe, elt, geom, BC, MassBC);
   }

   return sp_compact(MassBC, MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC   * assemblage3D_vector_fromBC(const ELT_3D *elt, const GEOM_3D *geom, const BC_3D *BC, VEC *RHS)
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage3D_vector_fromBC");
   if ( geom   == NULL ) error(E_NULL, "assemblage3D_vector_fromBC");
   if ( BC     == NULL ) error(E_NULL, "assemblage3D_vector_fromBC");
   if ( RHS    == NULL ) error(E_NULL, "assemblage3D_vector_fromBC");

   axe = BC->BC_neumann->current_selected_axe;
   /* conditions aux limites de Neumann */
   if ( Bc3D_hasNeumannBC(BC, axe) )
   {
      apply_Bc3D_neumann_on_rhs  (axe, elt, geom, BC, RHS);
   }

   axe = BC->BC_robin->current_selected_axe;
   /* conditions aux limites de Robin */
   if ( Bc3D_hasRobinBC(BC, axe) )
   {
      apply_Bc3D_robin_on_rhs  (axe, elt, geom, BC, RHS);
   }

   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real eval_jac_on_face  ( int f , const ELT_3D *element , const GEOM_3D *geom , Real face_center[3] )
{
   static int count = 0;
   /*
    Triangle (AB;AC) in R3 <-> Base triangle in R2
    */

   Real A[3], B[3], C[3];
   Real AB[3], AC[3], ABcrossAC[3], norm_ABcrossAC ;

   Real transformation[3][3];
   Real A_11, A_21, A_31;

   Real jacobien ;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSFACE = geom->NSFACE; /* alias */

   /* calcul jacobien */
   A[0] = XYSOMM->me[NSFACE->im[f][0]][0] ;
   A[1] = XYSOMM->me[NSFACE->im[f][0]][1] ;
   A[2] = XYSOMM->me[NSFACE->im[f][0]][2] ;

   B[0] = XYSOMM->me[NSFACE->im[f][1]][0] ;
   B[1] = XYSOMM->me[NSFACE->im[f][1]][1] ;
   B[2] = XYSOMM->me[NSFACE->im[f][1]][2] ;

   C[0] = XYSOMM->me[NSFACE->im[f][2]][0] ;
   C[1] = XYSOMM->me[NSFACE->im[f][2]][1] ;
   C[2] = XYSOMM->me[NSFACE->im[f][2]][2] ;

   face_center[0] = (A[0]+B[0]+C[0])/3.0 ;
   face_center[1] = (A[1]+B[1]+C[1])/3.0 ;
   face_center[2] = (A[2]+B[2]+C[2])/3.0 ;

   AB[0] = B[0] - A[0] ;
   AB[1] = B[1] - A[1] ;
   AB[2] = B[2] - A[2] ;

   AC[0] = C[0] - A[0] ;
   AC[1] = C[1] - A[1] ;
   AC[2] = C[2] - A[2] ;

   ABcrossAC[0] = AB[1]*AC[2] - AB[2]*AC[1];
   ABcrossAC[1] = AB[2]*AC[0] - AB[0]*AC[2];
   ABcrossAC[2] = AB[0]*AC[1] - AB[1]*AC[0];

   norm_ABcrossAC = sqrt( ABcrossAC[0]*ABcrossAC[0] + ABcrossAC[1]*ABcrossAC[1] + ABcrossAC[2]*ABcrossAC[2] );

   /*
    printf("A = [ %lf - %lf - %lf ] \n", A[0],A[1],A[2]);
    printf("B = [ %lf - %lf - %lf ] \n", B[0],B[1],B[2]);
    printf("C = [ %lf - %lf - %lf ] \n", C[0],C[1],C[2]);
    */

   /*
    printf("norm_ABcrossAC = %lf \n", norm_ABcrossAC);
    */

#ifdef _DEBUG
   assert( norm_ABcrossAC > 0.0 );
#endif

   ABcrossAC[0] /= norm_ABcrossAC;
   ABcrossAC[1] /= norm_ABcrossAC;
   ABcrossAC[2] /= norm_ABcrossAC;

   transformation[0][0] = AB[0];
   transformation[1][0] = AB[1];
   transformation[2][0] = AB[2];

   transformation[0][1] = AC[0];
   transformation[1][1] = AC[1];
   transformation[2][1] = AC[2];

   transformation[0][2] = ABcrossAC[0];
   transformation[1][2] = ABcrossAC[1];
   transformation[2][2] = ABcrossAC[2];

   A_11 = transformation[1][1]*transformation[2][2] - transformation[2][1]*transformation[1][2] ;
   A_21 = transformation[0][1]*transformation[2][2] - transformation[2][1]*transformation[0][2] ;
   A_31 = transformation[0][1]*transformation[1][2] - transformation[1][1]*transformation[0][2] ;


   jacobien = transformation[0][0] * A_11  -
   transformation[1][0] * A_21  +
   transformation[2][0] * A_31  ;

#ifdef _DEBUG
   assert( jacobien > 0.0 );
#endif

   /*
    printf("jacobien for face No %d  = %lf (count = %d)\n", f,jacobien, ++count);
    */

   return jacobien;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_neumann_on_rhs  (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS)
{
   int i,a;
   Real centrum[3],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Neumann */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   /* process */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_3De_NEUMANN==Bc3D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , centrum);
         BCval = Bc3D_evalFunction1(BC, BC_3De_NEUMANN, REF_A->ive[a], axe, centrum[0],centrum[1],centrum[2]);

         sv_mlt(  BCval * jac , elt->eltDM1->VEC_I, SCM_ar);

         for(i=0; i<elt->nb_somm_face; i++)
         {
            RHS->ve[ NSFACE->im[a][i] ] += SCM_ar->ve[i] ;
         }
      }
   }

   V_FREE(SCM_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_robin_on_rhs    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS)
{
   int i,a;
   Real centrum[3],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Robin */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   /* process */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_3De_ROBIN==Bc3D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , centrum); /* Jacobien de la transformation entre la face et [0:1] */
         BCval = Bc3D_evalFunction2(BC, BC_3De_ROBIN, REF_A->ive[a], axe, centrum[0],centrum[1],centrum[2]);

         sv_mlt(  BCval * jac , elt->eltDM1->VEC_I, SCM_ar);

         for(i=0; i<elt->nb_somm_face; i++)
         {
            RHS->ve[ NSFACE->im[a][i] ] += SCM_ar->ve[i] ;
         }
      }
   }

   V_FREE(SCM_ar);
}


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_cauchy_on_mat   (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real centrum[3],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Cauchy */


   /* ----- Transformation de la matrice -------------------------------- */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] != 0) && (BC_3De_CAUCHY==Bc3D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , centrum); /* Jacobien de la transformation entre la face et [0:1] */
         BCval = Bc3D_evalFunction1(BC, BC_3De_CAUCHY, REF_A->ive[a], axe, centrum[0],centrum[1],centrum[2]);

         sm_mlt(BCval * jac , elt->eltDM1->MAT_I_I, MAT_ar);

         for (k1=0; k1<elt->nb_somm_face; k1++)
         for (k2=0; k2<elt->nb_somm_face; k2++)
         {
            sp_add_val(A, NSFACE->im[a][k1] ,
                       NSFACE->im[a][k2] , MAT_ar->me[k1][k2] );
         }
      }
   }

   M_FREE(MAT_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_robin_on_mat    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real centrum[3],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Robin */

   /* ----- Transformation de la matrice -------------------------------- */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] != 0) && (BC_3De_ROBIN==Bc3D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , centrum); /* Jacobien de la transformation entre l'arete et [0:1] */
         BCval = Bc3D_evalFunction1(BC, BC_3De_ROBIN, REF_A->ive[a], axe, centrum[0],centrum[1], centrum[2]);

         sm_mlt(BCval * jac , elt->eltDM1->MAT_I_I, MAT_ar);

         for (k1=0; k1<elt->nb_somm_face; k1++)
         for (k2=0; k2<elt->nb_somm_face; k2++)
         {
            sp_add_val(A, NSFACE->im[a][k1] ,
                       NSFACE->im[a][k2] , MAT_ar->me[k1][k2] );
         }
      }
   }

   M_FREE(MAT_ar);
}

