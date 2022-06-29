
/*
 * Contains the functions to matrix and vector assemblage in 2D
 *
 */


#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"      /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"  /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/rhs.h"                  /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/assemblage_objects_2D.h"
#include "MESCHACH_EF/INCLUDES/triangle_integration_gauss_2D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

typedef enum
{
   ASSEMBLAGEe_MASS   = 0,
   ASSEMBLAGEe_STIFF1 = 1,
   ASSEMBLAGEe_CONV_X = 2,
   ASSEMBLAGEe_CONV_Y = 3,

   ASSEMBLAGEe_STAB       = 4,
   ASSEMBLAGEe_CONV_X_FUN = 5,
   ASSEMBLAGEe_CONV_Y_FUN = 6,
   ASSEMBLAGEe_CONV_G_FUN = 7,

   ASSEMBLAGEe_AUx    =  8,
   ASSEMBLAGEe_AUy    =  9,
   ASSEMBLAGEe_AxU    = 10,
   ASSEMBLAGEe_AyU    = 11,
   ASSEMBLAGEe_A_U    = 12,

   ASSEMBLAGEe_ABx    = 13,
   ASSEMBLAGEe_ABy    = 14,
   ASSEMBLAGEe_A_B    = 15

} ASSEMBLAGEt_TYPE;

static Real eval_jac_on_face( int f , const ELT_2D *elt , const GEOM_2D *geom , Real face_center[2]);

static SPMAT * _assemblage_matrix1a( const ASSEMBLAGEt_TYPE type , const ELT_2D *elt , const GEOM_2D *geom , SPMAT *A );
static SPMAT * _assemblage_matrix1b( const ASSEMBLAGEt_TYPE type , const ELT_2D *elt , const GEOM_2D *geom , SPMAT *A , const ADV_2D *MyAdvFun );

static MAT * _systel_matrix_Mass      ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el );
static MAT * _systel_matrix_Stiff1    ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el );
static MAT * _systel_matrix_Conv_x    ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el );
static MAT * _systel_matrix_Conv_y    ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el );
static MAT * _systel_matrix_M_Stab    ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el , const ADV_2D *MyAdvFun );
static MAT * _systel_matrix_Conv_x_fun( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el , const ADV_2D *MyAdvFun );
static MAT * _systel_matrix_Conv_y_fun( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el , const ADV_2D *MyAdvFun );
static MAT * _systel_matrix_Conv_G_fun( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el , const ADV_2D *MyAdvFun );

static SPMAT * _assemblage_matrix2( const ASSEMBLAGEt_TYPE type , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , SPMAT *A  );

static TENSOR * _systel_matrix_AUx ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AUy ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AxU ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AyU ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_A_U ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , TENSOR* M_el );

static MAT * _systel_matrix_Conv_x_elt2elt1( u_int e , const ELT_2D *elt2 , const GEOM_2D *Mygeom2 , const ELT_2D *elt1 , const GEOM_2D *Mygeom1 , MAT *Bx_el );
static MAT * _systel_matrix_Conv_y_elt2elt1( u_int e , const ELT_2D *elt2 , const GEOM_2D *Mygeom2 , const ELT_2D *elt1 , const GEOM_2D *Mygeom1 , MAT *By_el );

static VEC * _systel_vector_fun   ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D* RhsFun      , VEC *Rhs_el );
static VEC * _systel_vector_fun_o ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D *RhsFun      , VEC *Rhs_el , const MAT *fbase_k_ksi_m);
static VEC * _systel_vector_Stab( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D *RhsFun        , VEC *Stab_el);
static VEC * _systel_vector_abx ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );
static VEC * _systel_vector_aby ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );
static VEC * _systel_vector_ab  ( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *Rhs_el );

static Real eval_jac_on_e   ( int e , const ELT_2D *element , const GEOM_2D *geom );
static Real eval_jac_on_e_  ( int e , const ELT_2D *element , const GEOM_2D *geom , Real transformation[2][2]);

static Real element_size    ( int e , const ELT_2D *element , const GEOM_2D *geom , Real center[2]);


static void apply_Bc2D_cauchy_on_mat   (int axe , const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , SPMAT *A);
static void apply_Bc2D_robin_on_mat    (int axe , const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , SPMAT *A);

static void apply_Bc2D_neumann_on_rhs  (int axe , const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , VEC *RHS);
static void apply_Bc2D_robin_on_rhs    (int axe , const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , VEC *RHS);

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_Mass( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Mass )
{
   /* check */
   if ( elt  == NULL ) error(E_NULL, "assemblage2D_matrix_Mass");
   if ( geom == NULL ) error(E_NULL, "assemblage2D_matrix_Mass");
   if ( Mass == NULL ) error(E_NULL, "assemblage2D_matrix_Mass");

   /* delegate */
   return _assemblage_matrix1a (ASSEMBLAGEe_MASS , elt , geom , Mass );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_Stiff1( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Stiff1 )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_matrix_Stiff1");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_matrix_Stiff1");
   if ( Stiff1 == NULL ) error(E_NULL, "assemblage2D_matrix_Stiff1");

   /* delegate */
   return _assemblage_matrix1a (ASSEMBLAGEe_STIFF1 , elt , geom , Stiff1 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_Conv_x( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_x )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x");
   if ( Conv_x == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x");

   /* delegate */
   return _assemblage_matrix1a (ASSEMBLAGEe_CONV_X , elt , geom , Conv_x );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_Conv_y( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_y )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y");
   if ( Conv_y == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y");

   /* delegate */
   return _assemblage_matrix1a (ASSEMBLAGEe_CONV_Y , elt , geom , Conv_y );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_M_Stab( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *M_Stab , const ADV_2D *MyAdvFun )
{
   PARAMS* MyParams = Params_get_staticparam(0);
   char* method_stab;

   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage2D_matrix_M_Stab");
   if ( geom     == NULL ) error(E_NULL, "assemblage2D_matrix_M_Stab");
   if ( M_Stab   == NULL ) error(E_NULL, "assemblage2D_matrix_M_Stab");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage2D_matrix_M_Stab");

   /* check method */
   method_stab = MyParams->adv_params.method;

   if ( strncmp(method_stab,"GALS",4)==0 )
   {
      MyParams->adv_params.rho_stab = 1.0;
   }
   else
   if ( strncmp(method_stab,"SUPG",4)==0 )
   {
      MyParams->adv_params.rho_stab = 0.0;
   }
   else
   if ( strncmp(method_stab,"DWG",3)==0 )
   {
      MyParams->adv_params.rho_stab = -1.0;
   }
   else
   {
      error(E_UNKNOWN, "assemblage2D_matrix_M_Stab");
   }


   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_STAB , elt , geom , M_Stab , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage2D_matrix_Conv_x_fun( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_x , const ADV_2D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_fun");
   if ( Conv_x   == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_X_FUN , elt , geom , Conv_x , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage2D_matrix_Conv_y_fun( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_y , const ADV_2D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_fun");
   if ( Conv_y   == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_Y_FUN , elt , geom , Conv_y , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT*  assemblage2D_matrix_Conv_G_fun( const ELT_2D *elt , const GEOM_2D *geom , SPMAT *Conv_G , const ADV_2D *MyAdvFun )
{
   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_G_fun");
   if ( geom     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_G_fun");
   if ( Conv_G   == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_G_fun");
   if ( MyAdvFun == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_G_fun");

   /* delegate */
   return _assemblage_matrix1b (ASSEMBLAGEe_CONV_G_FUN , elt , geom , Conv_G , MyAdvFun );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT * assemblage2D_matrix_fromBC(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *MassBC)
{
   int axe;

   /* check */
   if ( elt      == NULL ) error(E_NULL, "assemblage2D_matrix_fromBC");
   if ( geom     == NULL ) error(E_NULL, "assemblage2D_matrix_fromBC");
   if ( BC       == NULL ) error(E_NULL, "assemblage2D_matrix_fromBC");
   if ( MassBC   == NULL ) error(E_NULL, "assemblage2D_matrix_fromBC");

   axe = BC->BC_cauchy->current_selected_axe;
   /* conditions aux limites de Cauchy */
   if ( Bc2D_hasCauchyBC(BC, axe) )
   {
      apply_Bc2D_cauchy_on_mat(axe, elt, geom, BC, MassBC);
   }

   axe = BC->BC_robin->current_selected_axe;
   /* conditions aux limites de Robin */
   if ( Bc2D_hasRobinBC(BC, axe) )
   {
      apply_Bc2D_robin_on_mat(axe, elt, geom, BC, MassBC);
   }

   return MassBC;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT* _assemblage_matrix1a( ASSEMBLAGEt_TYPE type , const ELT_2D *elt , const GEOM_2D *geom , SPMAT *A )
{
   int e; int i,j;

   MAT *A_el;
   MAT* (*_systel_matrix_)( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el );

   /* check */
   if ( elt  == NULL ) error(E_NULL, "_assemblage_matrix1a");
   if ( geom == NULL ) error(E_NULL, "_assemblage_matrix1a");
   if ( A    == NULL ) error(E_NULL, "_assemblage_matrix1a");

   switch( type )
   {
      case ASSEMBLAGEe_MASS  : _systel_matrix_ = _systel_matrix_Mass   ; break;
      case ASSEMBLAGEe_STIFF1: _systel_matrix_ = _systel_matrix_Stiff1 ; break;
      case ASSEMBLAGEe_CONV_X: _systel_matrix_ = _systel_matrix_Conv_x ; break;
      case ASSEMBLAGEe_CONV_Y: _systel_matrix_ = _systel_matrix_Conv_y ; break;

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

static SPMAT* _assemblage_matrix1b( const ASSEMBLAGEt_TYPE type , const ELT_2D *elt , const GEOM_2D *geom , SPMAT *A , const ADV_2D *MyAdvFun )
{
   int e; int i,j;

   MAT *A_el;
   MAT* (*_systel_matrix_)( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* M_el , const ADV_2D *MyAdvFun );

   /* check */
   if ( elt  == NULL ) error(E_NULL, "_assemblage_matrix1b");
   if ( geom == NULL ) error(E_NULL, "_assemblage_matrix1b");
   if ( A    == NULL ) error(E_NULL, "_assemblage_matrix1b");

   switch( type )
   {
      case ASSEMBLAGEe_STAB:       _systel_matrix_ = _systel_matrix_M_Stab     ; break;
      case ASSEMBLAGEe_CONV_X_FUN: _systel_matrix_ = _systel_matrix_Conv_x_fun ; break;
      case ASSEMBLAGEe_CONV_Y_FUN: _systel_matrix_ = _systel_matrix_Conv_y_fun ; break;
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

static MAT* _systel_matrix_Mass( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , MAT *Mass_el )
{
   Real jacobien;

   /* initialisation Mass_el */
   m_zero(Mass_el);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e ( e, elt, geom );

   /* calcul Mass_el on triangle number "e" */
   Mass_el = sm_mlt(jacobien, elt->MAT_I_I,Mass_el);


   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         Mass_el->me[i][j]  -= w2D[m]*jacobien
            elt->f_base[i](ksi2D[m],eta2D[m])*
            elt->f_base[j](ksi2D[m],eta2D[m])  ;
      }
   }
   */

   return Mass_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Stiff1( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , MAT *Stiff1_el )
{
   /*
                  2        2
    c_xx = [ ksi_x  + ksi_y  ] * jacobien


    c_xy = [ ksi_x.eta_x  + ksi_y.eta_y ] * jacobien

                  2         2
    c_yy = [ eta_x  +  eta_y  ] * jacobien

   */

   MAT *work_el;

   Real jacobien ;

   Real transformation[2][2];
   Real ksi_x, eta_x;
   Real ksi_y, eta_y;

   Real c_xx, c_xy, c_yy ;


   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation Stiff_el */
   m_zero(Stiff1_el);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];/* (y3-y1)*un_sur_jacobien*/ ;
   eta_x =  transformation[0][1];/*-(y2-y1)*un_sur_jacobien*/ ;
   ksi_y =  transformation[1][0];/*-(x3-x1)*un_sur_jacobien*/ ;
   eta_y =  transformation[1][1];/* (x2-x1)*un_sur_jacobien*/ ;

   c_xx = jacobien * ( ksi_x*ksi_x + ksi_y*ksi_y ) ;
   c_xy = jacobien * ( ksi_x*eta_x + ksi_y*eta_y ) ;
   c_yy = jacobien * ( eta_x*eta_x + eta_y*eta_y ) ;

   /* calcul Stiff1_el on triangle number "e" */
   Stiff1_el = m_add(Stiff1_el, sm_mlt(c_xx,elt->MAT_x_x,           work_el), Stiff1_el);
   Stiff1_el = m_add(Stiff1_el, sm_mlt(c_xy,elt->MAT_x_y__plus__y_x,work_el), Stiff1_el);
   Stiff1_el = m_add(Stiff1_el, sm_mlt(c_yy,elt->MAT_y_y,           work_el), Stiff1_el);

   /* or with numerical integration  with the basis functions : */
   /*
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         Stiff1_el->me[i][j]  -= w2D[m]*(c_xx*
                           elt->dfdx_base[i](ksi2D[m],eta2D[m])*elt->dfdx_base[j](ksi2D[m],eta2D[m])   +
                                        c_xy*
                          (elt->dfdx_base[i](ksi2D[m],eta2D[m])*elt->dfdy_base[j](ksi2D[m],eta2D[m])+
                           elt->dfdy_base[i](ksi2D[m],eta2D[m])*elt->dfdx_base[j](ksi2D[m],eta2D[m]))  +
                                        c_yy*
                           elt->dfdy_base[i](ksi2D[m],eta2D[m])*elt->dfdy_base[j](ksi2D[m],eta2D[m])   ) ;
      }
   }
   */

   /* free memory */
   M_FREE(work_el);

   return Stiff1_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Conv_x( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , MAT *Conv_x_el )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[2][2];
   Real ksi_x, eta_x;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];/* (y3-y1)*un_sur_jacobien*/ ;
   eta_x =  transformation[0][1];/*-(y2-y1)*un_sur_jacobien*/ ;

   /* initialisation Conv_x_el */
   m_zero(Conv_x_el);

   /* we have */
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*ksi_x),elt->MAT_I_x,work_el), Conv_x_el);
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*eta_x),elt->MAT_I_y,work_el), Conv_x_el);


  /*
                              _  _           _  _
  Conv_x_el = |Jac|.bx.(ksi_x.Ni_Nxj + eta_x.Ni_Nyj) ;
  Conv_y_el = |Jac|.by.(ksi_y.Ni_Nxj + eta_y.Ni_Nyj) ;

  */


  /* or with numerical integration  with the basis functions : */
  /*
  for (i=0; i<elt->nb_somm_cell; i++)
  for (j=0; j<elt->nb_somm_cell; j++)
  {
    for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
    {
      Conv_x_el->me[i][j]  += w2D[m]*jacobien*
                           elt->f_base[i](ksi2D[m],eta2D[m])*   (
                           ksi_x*elt->dfdx_base[j](ksi2D[m],eta2D[m]) +
                           eta_x*elt->dfdy_base[j](ksi2D[m],eta2D[m]) )  ;
    }
  }
  */

  /* free memory */
  M_FREE(work_el);

  return Conv_x_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Conv_y( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , MAT *Conv_y_el )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[2][2];
   Real ksi_y,eta_y;


   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* get the transformation parameters */
   ksi_y =  transformation[1][0];/*-(x3-x1)*un_sur_jacobien*/ ;
   eta_y =  transformation[1][1];/* (x2-x1)*un_sur_jacobien*/ ;

   /* initialisation Conv_x_el */
   m_zero(Conv_y_el);

   /* we have */
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*ksi_y),elt->MAT_I_x,work_el), Conv_y_el);
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*eta_y),elt->MAT_I_y,work_el), Conv_y_el);


  /* or with numerical integration  with the basis functions : */
  /*
  for (i=0; i<element->nb_somm_cell; i++)
  for (j=0; j<element->nb_somm_cell; j++)
  {
    for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
    {
      MAT_Cy_el->me[i][j]  += w2D[m]*jacobien*
                                 elt->f_base[i]   (ksi2D[m],eta2D[m])*
                           (
                           ksi_y*elt->dfdx_base[j](ksi2D[m],eta2D[m]) +
                           eta_y*elt->dfdy_base[j](ksi2D[m],eta2D[m])
                           )  ;

    }
  }
  */

  /* free memory */
  M_FREE(work_el);

  return Conv_y_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_x_fun( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* Conv_x_el , const ADV_2D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[2][2];
   Real ksi_x, eta_x;

   Real cell_size;
   Real cell_center[2];
   Real b1;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* calcul cell center */
   cell_size = element_size( e, elt, geom , cell_center);

   b1 = Adv2D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, cell_center[0], cell_center[1] );

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];/* (y3-y1)*un_sur_jacobien*/ ;
   eta_x =  transformation[0][1];/*-(y2-y1)*un_sur_jacobien*/ ;

   /* initialisation Conv_x_el */
   m_zero(Conv_x_el);

   /* we have */
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*ksi_x),elt->MAT_I_x,work_el), Conv_x_el);
   Conv_x_el = m_add(Conv_x_el, sm_mlt( (jacobien*eta_x),elt->MAT_I_y,work_el), Conv_x_el);

   Conv_x_el = sm_mlt( b1, Conv_x_el, Conv_x_el);

   /*                          _  _           _  _
   Conv_x_el = |Jac|.bx.(ksi_x.Ni_Nxj + eta_x.Ni_Nyj) ;
   */

   /* or with numerical integration  with the basis functions : */
   /*
    for (i=0; i<elt->nb_somm_cell; i++)
    for (j=0; j<elt->nb_somm_cell; j++)
    {
       for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
       {
          Conv_x_el->me[i][j]  += w2D[m]*jacobien*
          elt->f_base[i](ksi2D[m],eta2D[m])*   (
                                                ksi_x*elt->dfdx_base[j](ksi2D[m],eta2D[m]) +
                                                eta_x*elt->dfdy_base[j](ksi2D[m],eta2D[m]) )  ;
       }
    }
    */

   /* free memory */
   M_FREE(work_el);

   return Conv_x_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_y_fun( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* Conv_y_el , const ADV_2D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[2][2];
   Real ksi_y,eta_y;

   Real cell_size;
   Real cell_center[2];
   Real b2;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   /* calcul cell center */
   cell_size = element_size( e, elt, geom , cell_center);

   b2 = Adv2D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Y, cell_center[0], cell_center[1] );

   /* get the transformation parameters */
   ksi_y =  transformation[1][0];/*-(x3-x1)*un_sur_jacobien*/ ;
   eta_y =  transformation[1][1];/* (x2-x1)*un_sur_jacobien*/ ;

   /* initialisation Conv_y_el */
   m_zero(Conv_y_el);

   /* we have */
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*ksi_y),elt->MAT_I_x,work_el), Conv_y_el);
   Conv_y_el = m_add(Conv_y_el, sm_mlt( (jacobien*eta_y),elt->MAT_I_y,work_el), Conv_y_el);

   Conv_y_el = sm_mlt( b2, Conv_y_el, Conv_y_el);

   /* or with numerical integration  with the basis functions : */
   /*
    for (i=0; i<element->nb_somm_cell; i++)
    for (j=0; j<element->nb_somm_cell; j++)
    {
       for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
       {
          MAT_Cy_el->me[i][j]  += w2D[m]*jacobien*
          elt->f_base[i]   (ksi2D[m],eta2D[m])*
          (
           ksi_y*elt->dfdx_base[j](ksi2D[m],eta2D[m]) +
           eta_y*elt->dfdy_base[j](ksi2D[m],eta2D[m])
           )  ;

       }
    }
    */

   /* free memory */
   M_FREE(work_el);

   return Conv_y_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_G_fun( u_int e , const ELT_2D *elt , const GEOM_2D *geom , MAT* Conv_G_el , const ADV_2D *MyAdvFun )
{
   MAT *work_el;

   Real jacobien ;

   Real transformation[2][2];
   Real d_ksi,d_eta;
   int row;

   Real cell_size;
   Real cell_center[2];
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

   bG = Adv2D_evalFunction(MyAdvFun, 0, axe1, axe2, axe3, cell_center[0], cell_center[1] );

   /* get the transformation parameters */
   switch(axe2)
   {
      case AXEe_X: row = 0; break;
      case AXEe_Y: row = 1; break;
      default: error(E_UNKNOWN, "_systel_matrix_Conv_G_fun");
   }

   d_ksi =  transformation[row][0];  /* axe2 = AXEe_X -> d_ksi = ksi_x , d_eta = eta_x  */
   d_eta =  transformation[row][1];  /* axe2 = AXEe_Y -> d_ksi = ksi_y , d_eta = eta_y  */

   /* initialisation Conv_G_el */
   m_zero(Conv_G_el);

   /* we have */
   Conv_G_el = m_add(Conv_G_el, sm_mlt( (jacobien*d_ksi),elt->MAT_I_x,work_el), Conv_G_el);
   Conv_G_el = m_add(Conv_G_el, sm_mlt( (jacobien*d_eta),elt->MAT_I_y,work_el), Conv_G_el);

   Conv_G_el = sm_mlt( bG, Conv_G_el, Conv_G_el);

   /* free memory */
   M_FREE(work_el);

   return Conv_G_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_M_Stab( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , MAT *M_Stab_el , const ADV_2D *MyAdvFun )
{
   PARAMS* MyParams = Params_get_staticparam(0);
   int k;

   MAT *work_el;

   Real kappa  = MyParams->phys_params.kappa;
   Real sigma  = MyParams->phys_params.sigma;
   Real cell_size;
   Real rho_stab = MyParams->adv_params.rho_stab;
   Real delta    = MyParams->adv_params.delta;

   Real jacobien , un_sur_jacobien , kappa_jacobien , kappa_kappa_jacobien ;

   Real transformation[2][2];
   Real ksi_x, eta_x;
   Real ksi_y, eta_y;
   Real cell_center[2];

   Real coeff_MAT_D__D__[3] ;
   Real coeff_MAT_DD_DD_[9] ;
   Real coeff_MAT_DD_D__[6] ;
   Real coeff_MAT_D__DD_[6] ;

   MAT* MAT_D__D__[3];
   MAT* MAT_DD_DD_[9];
   MAT* MAT_D__DD_[6];
   MAT* MAT_DD_D__[6];

   Real b1 = 0.0;
   Real b2 = 0.0;
   Real b_max;

   Real coeff_stab;

   /* alloc memory */
   work_el = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   /* initialisation arrays of MAT* (members of "elt") */
   MAT_D__D__[0] = elt->MAT_x_x;
   MAT_D__D__[1] = elt->MAT_x_y__plus__y_x;
   MAT_D__D__[2] = elt->MAT_y_y;

   MAT_DD_DD_[0] = elt->MAT_xx_xx;
   MAT_DD_DD_[1] = elt->MAT_xx_xy;
   MAT_DD_DD_[2] = elt->MAT_xx_yy;
   MAT_DD_DD_[3] = elt->MAT_xy_xx;
   MAT_DD_DD_[4] = elt->MAT_xy_xy;
   MAT_DD_DD_[5] = elt->MAT_xy_yy;
   MAT_DD_DD_[6] = elt->MAT_yy_xx;
   MAT_DD_DD_[7] = elt->MAT_yy_xy;
   MAT_DD_DD_[8] = elt->MAT_yy_yy;

   MAT_DD_D__[0] = elt->MAT_xx_x;
   MAT_DD_D__[1] = elt->MAT_xx_y;
   MAT_DD_D__[2] = elt->MAT_xy_x;
   MAT_DD_D__[3] = elt->MAT_xy_y;
   MAT_DD_D__[4] = elt->MAT_yy_x;
   MAT_DD_D__[5] = elt->MAT_yy_y;

   MAT_D__DD_[0] = elt->MAT_x_xx;
   MAT_D__DD_[1] = elt->MAT_y_xx;
   MAT_D__DD_[2] = elt->MAT_x_xy;
   MAT_D__DD_[3] = elt->MAT_y_xy;
   MAT_D__DD_[4] = elt->MAT_x_yy;
   MAT_D__DD_[5] = elt->MAT_y_yy;


   /* calcul jacobien */
   cell_size = element_size( e, elt, geom , cell_center);

   jacobien = eval_jac_on_e_ ( e, elt, geom, transformation );

   un_sur_jacobien      = 1.0 / jacobien ;
   kappa_jacobien       = kappa * jacobien ;
   kappa_kappa_jacobien = kappa * kappa * jacobien ;

   b1 = Adv2D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_X, cell_center[0], cell_center[1] );
   b2 = Adv2D_evalFunction(MyAdvFun, 0, AXEe_X, AXEe_X, AXEe_Y, cell_center[0], cell_center[1] );

   b_max = ( fabs(b1) > fabs(b2) ? fabs(b1) : fabs(b2) );

   if ( b_max < kappa )
   {
      b_max = kappa ;
   }

   /* get the transformation parameters */
   ksi_x =  transformation[0][0]; /*  (y3-y1)*un_sur_jacobien */
   eta_x =  transformation[0][1]; /* -(y2-y1)*un_sur_jacobien */
   ksi_y =  transformation[1][0]; /* -(x3-x1)*un_sur_jacobien */
   eta_y =  transformation[1][1]; /*  (x2-x1)*un_sur_jacobien */


   /*                                       Nx * Nx         Nx * Ny  + Ny * Nx                Ny * Nv            */
   coeff_MAT_D__D__[0] = jacobien * ( b1*b1*ksi_x*ksi_x + 2*b1*b2*ksi_x*ksi_y               + b2*b2*ksi_y*ksi_y ) ;
   coeff_MAT_D__D__[1] = jacobien * ( b1*b1*ksi_x*eta_x +   b1*b2*(ksi_x*eta_y+ksi_y*eta_x) + b2*b2*ksi_y*eta_y ) ;
   coeff_MAT_D__D__[2] = jacobien * ( b1*b1*eta_x*eta_x + 2*b1*b2*eta_x*eta_y               + b2*b2*eta_y*eta_y ) ;

   /*                                         Nx * Nxx                 Ny *  Nyy                Ny *  Nxx                Nx * Nyy              */
   coeff_MAT_D__DD_[0] = kappa_jacobien * (   b1*ksi_x*ksi_x*ksi_x +   b2*ksi_y*ksi_y*ksi_y +   b2*ksi_y*ksi_x*ksi_x +   b1*ksi_x*ksi_y*ksi_y ) ;/* u_uu */
   coeff_MAT_D__DD_[1] = kappa_jacobien * (   b1*eta_x*ksi_x*ksi_x +   b2*eta_y*ksi_y*ksi_y +   b2*eta_y*ksi_x*ksi_x +   b1*eta_x*ksi_y*ksi_y ) ;/* v_uu */
   coeff_MAT_D__DD_[2] = kappa_jacobien * ( 2*b1*ksi_x*ksi_x*eta_x + 2*b2*ksi_y*ksi_y*eta_y + 2*b2*ksi_y*ksi_x*eta_x + 2*b1*ksi_x*ksi_y*eta_y ) ;/* u_uv */
   coeff_MAT_D__DD_[3] = kappa_jacobien * ( 2*b1*eta_x*ksi_x*eta_x + 2*b2*eta_y*ksi_y*eta_y + 2*b2*eta_y*ksi_x*eta_x + 2*b1*eta_x*ksi_y*eta_y ) ;/* v_uv */
   coeff_MAT_D__DD_[4] = kappa_jacobien * (   b1*ksi_x*eta_x*eta_x +   b2*ksi_y*eta_y*eta_y +   b2*ksi_y*eta_x*eta_x +   b1*ksi_x*eta_y*eta_y ) ;/* u_vv */
   coeff_MAT_D__DD_[5] = kappa_jacobien * (   b1*eta_x*eta_x*eta_x +   b2*eta_y*eta_y*eta_y +   b2*eta_y*eta_x*eta_x +   b1*eta_x*eta_y*eta_y ) ;/* v_vv */

   coeff_MAT_DD_D__[0] = coeff_MAT_D__DD_[0] ;
   coeff_MAT_DD_D__[1] = coeff_MAT_D__DD_[1] ;
   coeff_MAT_DD_D__[2] = coeff_MAT_D__DD_[2] ;
   coeff_MAT_DD_D__[3] = coeff_MAT_D__DD_[3] ;
   coeff_MAT_DD_D__[4] = coeff_MAT_D__DD_[4] ;
   coeff_MAT_DD_D__[5] = coeff_MAT_D__DD_[5] ;

   /*                                               Nxx  * Nxx                  Nyy *  Nyy                   Nxx *  Nyy                  Nyy * Nxx         */
   coeff_MAT_DD_DD_[0] = kappa_kappa_jacobien * (   ksi_x*ksi_x*ksi_x*ksi_x +   ksi_y*ksi_y*ksi_y*ksi_y +    ksi_x*ksi_x*ksi_y*ksi_y +   ksi_y*ksi_y*ksi_x*ksi_x ) ;
   coeff_MAT_DD_DD_[1] = kappa_kappa_jacobien * ( 2*ksi_x*ksi_x*ksi_x*ksi_x + 2*ksi_y*ksi_y*ksi_y*eta_y +  2*ksi_x*ksi_x*ksi_y*eta_y + 2*ksi_y*ksi_y*ksi_x*eta_x ) ;
   coeff_MAT_DD_DD_[2] = kappa_kappa_jacobien * (   ksi_x*ksi_x*eta_x*eta_x +   ksi_y*ksi_y*eta_y*eta_y +    ksi_x*ksi_x*eta_y*eta_y +   ksi_y*ksi_y*eta_x*eta_x ) ;
   coeff_MAT_DD_DD_[3] = kappa_kappa_jacobien * ( 2*ksi_x*eta_x*ksi_x*ksi_x + 2*ksi_y*eta_y*ksi_y*ksi_y +  2*ksi_x*eta_x*ksi_y*ksi_y + 2*ksi_y*eta_y*ksi_x*ksi_x ) ;
   coeff_MAT_DD_DD_[4] = kappa_kappa_jacobien * ( 4*ksi_x*eta_x*ksi_x*eta_x + 4*ksi_y*eta_y*ksi_y*eta_y +  4*ksi_x*eta_x*ksi_y*eta_y + 4*ksi_y*eta_y*ksi_x*eta_x ) ;
   coeff_MAT_DD_DD_[5] = kappa_kappa_jacobien * ( 2*ksi_x*eta_x*eta_x*eta_x + 2*ksi_y*eta_y*eta_y*eta_y +  2*ksi_x*eta_x*eta_y*eta_y + 2*ksi_y*eta_y*eta_x*eta_x ) ;
   coeff_MAT_DD_DD_[6] = kappa_kappa_jacobien * (   eta_x*eta_x*ksi_x*ksi_x +   eta_y*eta_y*ksi_y*ksi_y +    eta_x*eta_x*ksi_y*ksi_y +   eta_y*eta_y*ksi_x*ksi_x ) ;
   coeff_MAT_DD_DD_[7] = kappa_kappa_jacobien * ( 2*eta_x*eta_x*ksi_x*eta_x + 2*eta_y*eta_y*ksi_y*eta_y +  2*eta_x*eta_x*ksi_y*eta_y + 2*eta_y*eta_y*ksi_x*eta_x ) ;
   coeff_MAT_DD_DD_[8] = kappa_kappa_jacobien * (   eta_x*eta_x*eta_x*eta_x +   eta_y*eta_y*eta_y*eta_y +    eta_x*eta_x*eta_y*eta_y +   eta_y*eta_y*eta_x*eta_x ) ;

   /* MATRICE DE STABILISATION */

   /* initialisation M_Stab_el */
   m_zero(M_Stab_el);

   for (k=0; k<3; k++)
   {
      M_Stab_el = m_add(M_Stab_el, sm_mlt(coeff_MAT_D__D__[k], MAT_D__D__[k], work_el), M_Stab_el);
   }


   for (k=0; k<9; k++)
   {
      M_Stab_el = m_add(M_Stab_el, sm_mlt( rho_stab * coeff_MAT_DD_DD_[k], MAT_DD_DD_[k], work_el), M_Stab_el);
   }

   for (k=0; k<6; k++)
   {
      M_Stab_el = m_add(M_Stab_el, sm_mlt( -            coeff_MAT_D__DD_[k] , MAT_D__DD_[k], work_el), M_Stab_el);
      M_Stab_el = m_add(M_Stab_el, sm_mlt( - rho_stab * coeff_MAT_DD_D__[k] , MAT_DD_D__[k], work_el), M_Stab_el);
   }

   coeff_stab = delta * ( cell_size / b_max ) ;

   M_Stab_el = sm_mlt(coeff_stab, M_Stab_el, M_Stab_el);


   /* free memory */
   M_FREE(work_el);



   /* or with numerical integration  with the basis functions : */
   /*
   {
   int i,j,m;

   MAT * work_el0 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el1 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el2 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el3 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el4 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el5 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el6 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el7 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el8 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   MAT * work_el10 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el11 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el12 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el13 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el14 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el15 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   MAT * work_el20 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el21 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el22 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el23 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el24 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   MAT * work_el25 = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         work_el0->me[i][j]  += w2D[m]*elt->d2fdxx_base[i](ksi2D[m],eta2D[m])*elt->d2fdxx_base[j](ksi2D[m],eta2D[m])  ;
         work_el1->me[i][j]  += w2D[m]*elt->d2fdxx_base[i](ksi2D[m],eta2D[m])*elt->d2fdxy_base[j](ksi2D[m],eta2D[m])  ;
         work_el2->me[i][j]  += w2D[m]*elt->d2fdxx_base[i](ksi2D[m],eta2D[m])*elt->d2fdyy_base[j](ksi2D[m],eta2D[m])  ;
         work_el3->me[i][j]  += w2D[m]*elt->d2fdxy_base[i](ksi2D[m],eta2D[m])*elt->d2fdxx_base[j](ksi2D[m],eta2D[m])  ;
         work_el4->me[i][j]  += w2D[m]*elt->d2fdxy_base[i](ksi2D[m],eta2D[m])*elt->d2fdxy_base[j](ksi2D[m],eta2D[m])  ;
         work_el5->me[i][j]  += w2D[m]*elt->d2fdxy_base[i](ksi2D[m],eta2D[m])*elt->d2fdyy_base[j](ksi2D[m],eta2D[m])  ;
         work_el6->me[i][j]  += w2D[m]*elt->d2fdyy_base[i](ksi2D[m],eta2D[m])*elt->d2fdxx_base[j](ksi2D[m],eta2D[m])  ;
         work_el7->me[i][j]  += w2D[m]*elt->d2fdyy_base[i](ksi2D[m],eta2D[m])*elt->d2fdxy_base[j](ksi2D[m],eta2D[m])  ;
         work_el8->me[i][j]  += w2D[m]*elt->d2fdyy_base[i](ksi2D[m],eta2D[m])*elt->d2fdyy_base[j](ksi2D[m],eta2D[m])  ;

         work_el10->me[i][j]  += w2D[m]*elt->dfdx_base[i](ksi2D[m],eta2D[m])*elt->d2fdxx_base[j](ksi2D[m],eta2D[m])  ;
         work_el11->me[i][j]  += w2D[m]*elt->dfdy_base[i](ksi2D[m],eta2D[m])*elt->d2fdxx_base[j](ksi2D[m],eta2D[m])  ;
         work_el12->me[i][j]  += w2D[m]*elt->dfdx_base[i](ksi2D[m],eta2D[m])*elt->d2fdxy_base[j](ksi2D[m],eta2D[m])  ;
         work_el13->me[i][j]  += w2D[m]*elt->dfdy_base[i](ksi2D[m],eta2D[m])*elt->d2fdxy_base[j](ksi2D[m],eta2D[m])  ;
         work_el14->me[i][j]  += w2D[m]*elt->dfdx_base[i](ksi2D[m],eta2D[m])*elt->d2fdyy_base[j](ksi2D[m],eta2D[m])  ;
         work_el15->me[i][j]  += w2D[m]*elt->dfdy_base[i](ksi2D[m],eta2D[m])*elt->d2fdyy_base[j](ksi2D[m],eta2D[m])  ;

         work_el20->me[i][j]  += w2D[m]*elt->d2fdxx_base[i](ksi2D[m],eta2D[m])*elt->dfdx_base[j](ksi2D[m],eta2D[m])  ;
         work_el21->me[i][j]  += w2D[m]*elt->d2fdxx_base[i](ksi2D[m],eta2D[m])*elt->dfdy_base[j](ksi2D[m],eta2D[m])  ;
         work_el22->me[i][j]  += w2D[m]*elt->d2fdxy_base[i](ksi2D[m],eta2D[m])*elt->dfdx_base[j](ksi2D[m],eta2D[m])  ;
         work_el23->me[i][j]  += w2D[m]*elt->d2fdxy_base[i](ksi2D[m],eta2D[m])*elt->dfdy_base[j](ksi2D[m],eta2D[m])  ;
         work_el24->me[i][j]  += w2D[m]*elt->d2fdyy_base[i](ksi2D[m],eta2D[m])*elt->dfdx_base[j](ksi2D[m],eta2D[m])  ;
         work_el25->me[i][j]  += w2D[m]*elt->d2fdyy_base[i](ksi2D[m],eta2D[m])*elt->dfdy_base[j](ksi2D[m],eta2D[m])  ;
      }
   }

   m_sub(work_el0, MAT_DD_DD_[0], MAT_DD_DD_[0]);
   m_sub(work_el1, MAT_DD_DD_[1], MAT_DD_DD_[1]);
   m_sub(work_el2, MAT_DD_DD_[2], MAT_DD_DD_[2]);
   m_sub(work_el3, MAT_DD_DD_[3], MAT_DD_DD_[3]);
   m_sub(work_el4, MAT_DD_DD_[4], MAT_DD_DD_[4]);
   m_sub(work_el5, MAT_DD_DD_[5], MAT_DD_DD_[5]);
   m_sub(work_el6, MAT_DD_DD_[6], MAT_DD_DD_[6]);
   m_sub(work_el7, MAT_DD_DD_[7], MAT_DD_DD_[7]);
   m_sub(work_el8, MAT_DD_DD_[8], MAT_DD_DD_[8]);

   m_sub(work_el10, MAT_D__DD_[0], MAT_D__DD_[0]);
   m_sub(work_el11, MAT_D__DD_[1], MAT_D__DD_[1]);
   m_sub(work_el12, MAT_D__DD_[2], MAT_D__DD_[2]);
   m_sub(work_el13, MAT_D__DD_[3], MAT_D__DD_[3]);
   m_sub(work_el14, MAT_D__DD_[4], MAT_D__DD_[4]);
   m_sub(work_el15, MAT_D__DD_[5], MAT_D__DD_[5]);

   m_sub(work_el20, MAT_DD_D__[0], MAT_DD_D__[0]);
   m_sub(work_el21, MAT_DD_D__[1], MAT_DD_D__[1]);
   m_sub(work_el22, MAT_DD_D__[2], MAT_DD_D__[2]);
   m_sub(work_el23, MAT_DD_D__[3], MAT_DD_D__[3]);
   m_sub(work_el24, MAT_DD_D__[4], MAT_DD_D__[4]);
   m_sub(work_el25, MAT_DD_D__[5], MAT_DD_D__[5]);

   for (m=0; m<9; m++)
   {
      for (i=0; i<elt->nb_somm_cell; i++)
      for (j=0; j<elt->nb_somm_cell; j++)
      {
         if ( fabs(MAT_DD_DD_[m]->me[i][j]) < 1.0e-5 )  MAT_DD_DD_[m]->me[i][j] = 0.0;
      }

      printf(" m = %d \n",m);
      m_output(MAT_DD_DD_[m]);
   }

   for (m=0; m<6; m++)
   {
      for (i=0; i<elt->nb_somm_cell; i++)
      for (j=0; j<elt->nb_somm_cell; j++)
      {
         if ( fabs(MAT_D__DD_[m]->me[i][j]) < 1.0e-5 )  MAT_D__DD_[m]->me[i][j] = 0.0;
      }

      printf(" m = %d \n",m);
      m_output(MAT_D__DD_[m]);
   }

   for (m=0; m<6; m++)
   {
      for (i=0; i<elt->nb_somm_cell; i++)
      for (j=0; j<elt->nb_somm_cell; j++)
      {
         if ( fabs(MAT_DD_D__[m]->me[i][j]) < 1.0e-5 )  MAT_DD_D__[m]->me[i][j] = 0.0;
      }

      printf(" m = %d \n",m);
      m_output(MAT_DD_D__[m]);
   }

   }
   */

   return M_Stab_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_AUx( const ELT_2D *element , const GEOM_2D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage2D_matrix_AUx :                                 */
   /*   calculate the matrix A from the term "a.Ux" in the PDE      */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*dPHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_AxU :                                 */
   /*   calculate the matrix A from the term "a_x.U" in the PDE     */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*dPHI_p)*PHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_A_U :                                 */
   /*   calculate the matrix A from the term "a.U" in the PDE       */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*PHI_j*PHI_i                       */


   /* check */
   if ( element == NULL ) error(E_NULL, "assemblage2D_matrix_AUx");
   if ( geom    == NULL ) error(E_NULL, "assemblage2D_matrix_AUx");
   if ( a       == NULL ) error(E_NULL, "assemblage2D_matrix_AUx");
   if ( A       == NULL ) error(E_NULL, "assemblage2D_matrix_AUx");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_AUx , element , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_AUy( const ELT_2D *element , const GEOM_2D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage2D_matrix_AUy :                                 */
   /*   calculate the matrix A from the term "a.Uy" in the PDE      */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*dPHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_AxU :                                 */
   /*   calculate the matrix A from the term "a_y.U" in the PDE     */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*dPHI_p)*PHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_A_U :                                 */
   /*   calculate the matrix A from the term "a.U" in the PDE       */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*PHI_j*PHI_i                       */

   /* check */
   if ( element == NULL ) error(E_NULL, "assemblage2D_matrix_AUy");
   if ( geom    == NULL ) error(E_NULL, "assemblage2D_matrix_AUy");
   if ( a       == NULL ) error(E_NULL, "assemblage2D_matrix_AUy");
   if ( A       == NULL ) error(E_NULL, "assemblage2D_matrix_AUy");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_AUy , element , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_AxU( const ELT_2D *element , const GEOM_2D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage2D_matrix_AUx :                                 */
   /*   calculate the matrix A from the term "a.Ux" in the PDE      */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*dPHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_AxU :                                 */
   /*   calculate the matrix A from the term "a_x.U" in the PDE     */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*dPHI_p)*PHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_A_U :                                 */
   /*   calculate the matrix A from the term "a.U" in the PDE       */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*PHI_j*PHI_i                       */

   /* check */
   if ( element == NULL ) error(E_NULL, "assemblage2D_matrix_AxU");
   if ( geom    == NULL ) error(E_NULL, "assemblage2D_matrix_AxU");
   if ( a       == NULL ) error(E_NULL, "assemblage2D_matrix_AxU");
   if ( A       == NULL ) error(E_NULL, "assemblage2D_matrix_AxU");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_AxU , element , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_AyU( const ELT_2D *element , const GEOM_2D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage2D_matrix_AUx :                                 */
   /*   calculate the matrix A from the term "a.Ux" in the PDE      */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*dPHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_AxU :                                 */
   /*   calculate the matrix A from the term "a_x.U" in the PDE     */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*dPHI_p)*PHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_A_U :                                 */
   /*   calculate the matrix A from the term "a.U" in the PDE       */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*PHI_j*PHI_i                       */

   /* check */
   if ( element == NULL ) error(E_NULL, "assemblage2D_matrix_AyU");
   if ( geom    == NULL ) error(E_NULL, "assemblage2D_matrix_AyU");
   if ( a       == NULL ) error(E_NULL, "assemblage2D_matrix_AyU");
   if ( A       == NULL ) error(E_NULL, "assemblage2D_matrix_AyU");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_AyU , element , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage2D_matrix_A_U( const ELT_2D *element , const GEOM_2D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage2D_matrix_AUx :                                 */
   /*   calculate the matrix A from the term "a.Ux" in the PDE      */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*dPHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_AxU :                                 */
   /*   calculate the matrix A from the term "a_x.U" in the PDE     */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*dPHI_p)*PHI_j*PHI_i                      */
   /*                                                               */
   /* in  assemblage2D_matrix_A_U :                                 */
   /*   calculate the matrix A from the term "a.U" in the PDE       */
   /*   with vector "a" given : a = SUM( a_p.PHI_p)                 */
   /*               "U"       : U = SUM( u_j.PHI_j)                 */
   /*   => A_ij = (SUM a_p*PHI_p)*PHI_j*PHI_i                       */

   /* check */
   if ( element == NULL ) error(E_NULL, "assemblage2D_matrix_A_U");
   if ( geom    == NULL ) error(E_NULL, "assemblage2D_matrix_A_U");
   if ( a       == NULL ) error(E_NULL, "assemblage2D_matrix_A_U");
   if ( A       == NULL ) error(E_NULL, "assemblage2D_matrix_A_U");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_A_U , element , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT* _assemblage_matrix2( ASSEMBLAGEt_TYPE type , const ELT_2D *element , const GEOM_2D *geom , const VEC *a , SPMAT *A  )
{
   u_int e;
   u_int i,j,k;

   MAT   *MAT_el;
   TENSOR *TENSOR_el ;

   TENSOR* (*_systel_matrix_)( u_int e , const ELT_2D *elt , const GEOM_2D *geom , TENSOR* M_el );

   switch( type )
   {
      case ASSEMBLAGEe_AUx  : _systel_matrix_ = _systel_matrix_AUx ; break;
      case ASSEMBLAGEe_AUy  : _systel_matrix_ = _systel_matrix_AUy ; break;
      case ASSEMBLAGEe_AxU  : _systel_matrix_ = _systel_matrix_AxU ; break;
      case ASSEMBLAGEe_AyU  : _systel_matrix_ = _systel_matrix_AyU ; break;
      case ASSEMBLAGEe_A_U  : _systel_matrix_ = _systel_matrix_A_U ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix2");
   }

   /* check */
   if ( element == NULL ) error(E_NULL, "_assemblage_matrix2");
   if ( geom    == NULL ) error(E_NULL, "_assemblage_matrix2");
   if ( a       == NULL ) error(E_NULL, "_assemblage_matrix2");
   if ( A       == NULL ) error(E_NULL, "_assemblage_matrix2");

   /* mem alloc */
   MAT_el    =  m_get(element->nb_somm_cell, element->nb_somm_cell);
   TENSOR_el = ts_get(element->nb_somm_cell, element->nb_somm_cell, element->nb_somm_cell);

   /* initialisation de A */
   sp_zero(A);

   /* fill */
   for( e=0 ; e<geom->NBELMT ; e++ )
   {
      TENSOR_el = _systel_matrix_( e , element , geom , TENSOR_el );

      m_zero(MAT_el);  /* init for each element "e" */

      for( i=0 ; i<element->nb_somm_cell ; i++)
      for( j=0 ; j<element->nb_somm_cell ; j++)
      {
         for (k=0; k<element->nb_somm_cell ; k++)
         {
            MAT_el->me[i][j] += a->ve[geom->NSELMT->im[e][k]]*TENSOR_el->te[k][j][i];
         }

         sp_add_val( A, geom->NSELMT->im[e][i] , geom->NSELMT->im[e][j] , MAT_el->me[i][j] );
      }
   }

   M_FREE(MAT_el) ;
   TS_FREE(TENSOR_el) ;

   return sp_compact(A, MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_AUx( u_int  e , const ELT_2D *element , const GEOM_2D *geom , TENSOR  *TENSOR_el  )
{
   Real jacobien ;

   Real transformation[2][2];
   Real ksi_x, eta_x;


   TENSOR  *work_el = ts_get(element->nb_somm_cell,element->nb_somm_cell,element->nb_somm_cell);


   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, element, geom, transformation) ;

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];/* (y3-y1)*un_sur_jacobien*/ ;
   eta_x =  transformation[0][1];/*-(y2-y1)*un_sur_jacobien*/ ;

   /* init */
   ts_zero(TENSOR_el);

   /* we have */
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*ksi_x),element->TENSOR_I_x_I,work_el), TENSOR_el);
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*eta_x),element->TENSOR_I_y_I,work_el), TENSOR_el);

   /*
   or with numerical integration  with the basis functions :
   */
   /*
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  += w2D[m]* ( ksi_x*
            element->f_base[i]   (ksi2D[m],eta2D[m]) *
            element->dfdx_base[j](ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) +
                                             eta_x*
            element->f_base[i]   (ksi2D[m],eta2D[m]) *
            element->dfdy_base[j](ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) );
      }
   }
   TENSOR_el = sts_mlt( jacobien, TENSOR_el, TENSOR_el);
   */

   TS_FREE(work_el);

   /* end */
   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_AUy( u_int  e , const ELT_2D *element , const GEOM_2D *geom , TENSOR  *TENSOR_el  )
{
   Real jacobien ;

   Real transformation[2][2];
   Real ksi_y, eta_y ;

   TENSOR  *work_el = ts_get(element->nb_somm_cell,element->nb_somm_cell,element->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, element, geom, transformation) ;

   /* get the transformation parameters */
   ksi_y =  transformation[1][0];/*-(x3-x1)*un_sur_jacobien*/ ;
   eta_y =  transformation[1][1];/* (x2-x1)*un_sur_jacobien*/ ;

   /* init */
   ts_zero(TENSOR_el);

   /* we have */
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*ksi_y),element->TENSOR_I_x_I,work_el), TENSOR_el);
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*eta_y),element->TENSOR_I_y_I,work_el), TENSOR_el);

   /*
   or with numerical integration  with the basis functions :
   */
   /*
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  += w2D[m]*( ksi_y*
            element->f_base[i]   (ksi2D[m],eta2D[m]) *
            element->dfdx_base[j](ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) +
                                            eta_y*
            element->f_base[i]   (ksi2D[m],eta2D[m]) *
            element->dfdy_base[j](ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) );
      }
   }
   TENSOR_el = sts_mlt( jacobien, TENSOR_el, TENSOR_el);
   */

   TS_FREE(work_el);

   /* end */
   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_AxU( u_int  e , const ELT_2D *element , const GEOM_2D *geom , TENSOR  *TENSOR_el  )
{
   Real jacobien ;

   Real transformation[2][2];
   Real ksi_x,eta_x;

   TENSOR  *work_el = ts_get(element->nb_somm_cell,element->nb_somm_cell,element->nb_somm_cell);


   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, element, geom, transformation ) ;

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];/* (y3-y1)*un_sur_jacobien*/ ;
   eta_x =  transformation[0][1];/*-(y2-y1)*un_sur_jacobien*/ ;

   /* init */
   ts_zero(TENSOR_el);

   /* we have */
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*ksi_x),element->TENSOR_x_I_I,work_el), TENSOR_el);
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*eta_x),element->TENSOR_y_I_I,work_el), TENSOR_el);

   /*
   or with numerical integration  with the basis functions :
   */
   /*
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  += w2D[m]* ( ksi_x*
            element->dfdx_base[i](ksi2D[m],eta2D[m]) *
            element->f_base[j]   (ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) +
                                             eta_x*
            element->dfdy_base[i](ksi2D[m],eta2D[m]) *
            element->f_base[j]   (ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) );
      }
   }
   TENSOR_el = sts_mlt( jacobien, TENSOR_el, TENSOR_el);
   */

   TS_FREE(work_el);

   /* end */
   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_AyU( u_int  e , const ELT_2D *element , const GEOM_2D *geom , TENSOR  *TENSOR_el  )
{
   Real jacobien  ;

   Real transformation[2][2];
   Real ksi_y,eta_y ;

   TENSOR  *work_el = ts_get(element->nb_somm_cell,element->nb_somm_cell,element->nb_somm_cell);

   /* calcul jacobien */
   jacobien = eval_jac_on_e_ ( e, element, geom, transformation );

   /* get the transformation parameters */
   ksi_y =  transformation[1][0];/*-(x3-x1)*un_sur_jacobien*/ ;
   eta_y =  transformation[1][1];/* (x2-x1)*un_sur_jacobien*/ ;

   /* init */
   ts_zero(TENSOR_el);

   /* we have  */
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*ksi_y),element->TENSOR_x_I_I,work_el), TENSOR_el);
   TENSOR_el = ts_add(TENSOR_el, sts_mlt( (jacobien*eta_y),element->TENSOR_y_I_I,work_el), TENSOR_el);

   /*
   or with numerical integration  with the basis functions :
   */
   /*
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  += w2D[m]* ( ksi_y*
            element->dfdx_base[i](ksi2D[m],eta2D[m]) *
            element->f_base[j]   (ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) +
                                             eta_y*
            element->dfdy_base[i](ksi2D[m],eta2D[m]) *
            element->f_base[j]   (ksi2D[m],eta2D[m]) *
            element->f_base[k]   (ksi2D[m],eta2D[m]) );
      }
   }
   sts_mlt( jacobien, TENSOR_el, TENSOR_el);
   */

   TS_FREE(work_el);

   /* end */
   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_A_U( u_int  e , const ELT_2D *element , const GEOM_2D *geom , TENSOR  *TENSOR_el  )
{
   Real jacobien;

   /* calcul jacobien on this element */
   jacobien = eval_jac_on_e ( e , element , geom );

   /* init */
   ts_zero(TENSOR_el);

   /* we have */
   TENSOR_el = sts_mlt( jacobien, element->TENSOR_I_I_I, TENSOR_el);

   /*
   or with numerical integration  with the basis functions :
   */
   /*
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  += w2D[m]*jacobien*
            element->f_base[i](ksi2D[m],eta2D[m]) *
            element->f_base[j](ksi2D[m],eta2D[m]) *
            element->f_base[k](ksi2D[m],eta2D[m]) ;
      }
   }
   */


   /* end */
   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage2D_vector_fun( const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D *RhsFun , VEC *RHS )
{
   int e,i,m,k;
   VEC *RHS_el;
   MAT *fbase_k_ksieta_m;

   /* check */
   if ( elt    == ELT_2D_NULL  ) error(E_NULL, "assemblage2D_vector_Rhs");
   if ( geom   == GEOM_2D_NULL ) error(E_NULL, "assemblage2D_vector_Rhs");
   if ( RhsFun == RHS_2D_NULL  ) error(E_NULL, "assemblage2D_vector_Rhs");
   if ( RHS    == VNULL        ) error(E_NULL, "assemblage2D_vector_Rhs");

   /* start */
   RHS_el = v_get(elt->nb_somm_cell);

   /* init fbase_k_ksi_m */
   fbase_k_ksieta_m = m_get(elt->nb_somm_cell, NBPTS_GAUSS_2D_TRI);

   for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
   {
      for (k=0; k<elt->nb_somm_cell; k++)
      {
         fbase_k_ksieta_m->me[k][m] = elt->f_base[k](ksi2D[m], eta2D[m]) ;
      }
   }


   /* initialisation of Mass */
   v_zero(RHS);

   for(e=0; e<geom->NBELMT; e++)
   {
      /*RHS_el = _systel_vector_fun( e , elt , geom , RhsFun , RHS_el );*/
      RHS_el = _systel_vector_fun_o( e , elt , geom , RhsFun , RHS_el , fbase_k_ksieta_m);

      for (i=0; i<elt->nb_somm_cell; i++)
      {
         RHS->ve[geom->NSELMT->im[e][i]] += RHS_el->ve[i] ;
      }
   }

   V_FREE(RHS_el) ;
   M_FREE(fbase_k_ksieta_m);

   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC * assemblage2D_vector_fromBC(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, VEC *RHS)
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_vector_fromBC");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_vector_fromBC");
   if ( BC     == NULL ) error(E_NULL, "assemblage2D_vector_fromBC");
   if ( RHS    == NULL ) error(E_NULL, "assemblage2D_vector_fromBC");

   axe = BC->BC_neumann->current_selected_axe;
   /* conditions aux limites de Neumann */
   if ( Bc2D_hasNeumannBC(BC, axe) )
   {
      apply_Bc2D_neumann_on_rhs  (axe, elt, geom, BC, RHS);
   }

   axe = BC->BC_robin->current_selected_axe;
   /* conditions aux limites de Robin */
   if ( Bc2D_hasRobinBC(BC, axe) )
   {
      apply_Bc2D_robin_on_rhs  (axe, elt, geom, BC, RHS);
   }

   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_fun( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D *RhsFun , VEC *Rhs_el )
{
   int  m,k;
   Real xx[NBPTS_GAUSS_2D_TRI], yy[NBPTS_GAUSS_2D_TRI];

   Real x1,x2,x3 ;
   Real y1,y2,y3 ;

   Real jacobien ;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   int axe = RhsFun->current_selected_axe;
   /* check */
   if ( (axe != AXEe_X) && (axe != AXEe_Y) ) error6(E_RHS_WRONGAXENUMBER, "systel_vector_Rhs");

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   /* calcul jacobien */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;

   jacobien = (y3-y1)*(x2-x1) - (x3-x1)*(y2-y1) ;


   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         xx[m] = x1 + (x2-x1)*ksi2D[m] + (x3-x1)*eta2D[m] ;
         yy[m] = y1 + (y2-y1)*ksi2D[m] + (y3-y1)*eta2D[m] ;

         Rhs_el->ve[k] += w2D[m]* /* jacobien sp���ter ! */
            Rhs2D_evalFunction(RhsFun, 0/*ref_e*/, axe, xx[m],yy[m]) * elt->f_base[k](ksi2D[m],eta2D[m]) ;
      }
   }

   Rhs_el = sv_mlt(jacobien, Rhs_el, Rhs_el); /* don't forget the jacobien !*/

   /* If the Source is a constant function : */
   /*
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      Rhs_el->ve[k] = Rhs2D_evalFunction( RhsFun, 0, axe, 0.0,0.0) * jacobien * elt->VEC_I->ve[k] ;
   }
   */

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_fun_o( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D *RhsFun , VEC *Rhs_el , const MAT *fbase_k_ksieta_m  )
{
   int  m,k;
   Real xx[NBPTS_GAUSS_2D_TRI], yy[NBPTS_GAUSS_2D_TRI];

   Real x1,x2,x3 ;
   Real y1,y2,y3 ;

   Real jacobien ;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   int axe = RhsFun->current_selected_axe;
   /* check */
   if ( (axe != AXEe_X) && (axe != AXEe_Y) ) error6(E_RHS_WRONGAXENUMBER, "systel_vector_Rhs");

   /* initialisation Rhs_el */
   v_zero(Rhs_el);

   /* calcul jacobien */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;

   jacobien = (y3-y1)*(x2-x1) - (x3-x1)*(y2-y1) ;


   for (k=0; k<elt->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         xx[m] = x1 + (x2-x1)*ksi2D[m] + (x3-x1)*eta2D[m] ;
         yy[m] = y1 + (y2-y1)*ksi2D[m] + (y3-y1)*eta2D[m] ;

         Rhs_el->ve[k] += w2D[m]* /* jacobien spater ! */
            Rhs2D_evalFunction(RhsFun, 0/*ref_e*/, axe, xx[m],yy[m]) * fbase_k_ksieta_m->me[k][m] ;
      }
   }

   Rhs_el = sv_mlt(jacobien, Rhs_el, Rhs_el); /* don't forget the jacobien !*/

   /* If the Source is a constant function : */
   /*
   for (k=0; k<elt->nb_somm_cell; k++)
   {
      Rhs_el->ve[k] = Rhs2D_evalFunction( RhsFun, 0, axe, 0.0,0.0) * jacobien * elt->VEC_I->ve[k] ;
   }
   */

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC *_systel_vector_Stab( u_int e , const ELT_2D *elt , const GEOM_2D *geom , const RHS_2D *RhsFun, VEC *Stab_el )
{
   error(E_UNKNOWN, "_systel_vector_Stab");
   return NULL;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage2D_vector_abx( const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *RHS )
{
   int e; int i;

   VEC *RHS_el;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( a      == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( b      == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( RHS    == NULL ) error(E_NULL, "assemblage2D_vector_abx");

   /* alloc mem */
   RHS_el = v_get(elt->nb_somm_cell);

   /* initialisation of RHS */
   v_zero(RHS);

   for(e=0; e<geom->NBELMT; e++)
   {
      RHS_el = _systel_vector_abx( e , elt , geom , a , b , RHS_el );

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

VEC* assemblage2D_vector_aby( const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *RHS )
{
   int e; int i;

   VEC *RHS_el;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( a      == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( b      == NULL ) error(E_NULL, "assemblage2D_vector_abx");
   if ( RHS    == NULL ) error(E_NULL, "assemblage2D_vector_abx");

   /* alloc mem */
   RHS_el = v_get(elt->nb_somm_cell);

   /* initialisation of RHS */
   v_zero(RHS);

   for(e=0; e<geom->NBELMT; e++)
   {
      RHS_el = _systel_vector_aby( e , elt , geom , a , b , RHS_el );

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

static VEC* _systel_vector_abx( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b, VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                              elt->nb_somm_cell,
                              elt->nb_somm_cell);

   /* init */
   _systel_matrix_AUx( e , elt , geom , TENSOR_el );

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

static VEC* _systel_vector_aby( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b, VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                              elt->nb_somm_cell,
                              elt->nb_somm_cell);

   /* init */
   _systel_matrix_AUy( e , elt , geom , TENSOR_el );

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

VEC* assemblage2D_vector_ab( const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *RHS)
{
   int e; int i;

   VEC *RHS_el;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage2D_vector_ab");
   if ( geom   == NULL ) error(E_NULL, "assemblage2D_vector_ab");
   if ( a      == NULL ) error(E_NULL, "assemblage2D_vector_ab");
   if ( b      == NULL ) error(E_NULL, "assemblage2D_vector_ab");
   if ( RHS    == NULL ) error(E_NULL, "assemblage2D_vector_ab");

   /* alloc mem */
   RHS_el = v_get(elt->nb_somm_cell);

   /* initialisation of RHS */
   v_zero(RHS);

   for(e=0; e<geom->NBELMT; e++)
   {
      RHS_el = _systel_vector_ab( e , elt , geom , a , b , RHS_el );

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

static VEC* _systel_vector_ab ( u_int  e , const ELT_2D *elt , const GEOM_2D *geom , const VEC *a , const VEC *b , VEC *Rhs_el )
{
   int i,j;

   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                              elt->nb_somm_cell,
                              elt->nb_somm_cell);

   /* init */
   _systel_matrix_A_U( e , elt , geom , TENSOR_el );

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

SPMAT *assemblage2D_matrix_Conv_x_elt2elt1( const ELT_2D *elt2 , const GEOM_2D *geom2 ,
                                            const ELT_2D *elt1 , const GEOM_2D *geom1 , SPMAT *Bx )
{
   int e;
   int i,j;

   MAT *Bx_el;

   /* check */
   if ( elt2      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_elt2elt1");
   if ( geom2     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_elt2elt1");
   if ( elt1      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_elt2elt1");
   if ( geom1     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_elt2elt1");
   if ( Bx        == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_x_elt2elt1");
   /* test compatibility geom1 and geom 2 */
   if ( geom1->NBELMT != geom2->NBELMT )  error(E_SIZES, "assemblage2D_matrix_Conv_x_elt2elt1");

   /* alloc mem */
   Bx_el = m_get(elt1->nb_somm_cell, elt2->nb_somm_cell);

   /* initialisation of Bx   */
   sp_zero(Bx);

   for(e=0; e<geom1->NBELMT; e++)
   {
      /* printf("triangle num %d \n",e); */
      Bx_el = _systel_matrix_Conv_x_elt2elt1( e , elt2 , geom2 , elt1 , geom1 , Bx_el );

      for (i=0; i<elt1->nb_somm_cell; i++)
      for (j=0; j<elt2->nb_somm_cell; j++)
      {
         /*
         printf("(i,j)_el = (%d,%d) \n",i,j);
         printf("(i,j)    = (%d,%d) \n",geom->geomBase->NSELMT->im[e][i],geom->NSELMT->im[e][j]);
         */

         sp_add_val( Bx   , geom1->NSELMT->im[e][i] , geom2->NSELMT->im[e][j] ,
                     Bx_el->me[i][j] );
      }
   }

   M_FREE(Bx_el) ;

   return sp_compact(Bx,MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT *assemblage2D_matrix_Conv_y_elt2elt1( const ELT_2D *elt2 , const GEOM_2D *geom2 ,
                                            const ELT_2D *elt1 , const GEOM_2D *geom1 , SPMAT *By )
{
   int e;
   int i,j;

   MAT *By_el;

   /* check */
   if ( elt2      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_elt2elt1");
   if ( geom2     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_elt2elt1");
   if ( elt1      == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_elt2elt1");
   if ( geom1     == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_elt2elt1");
   if ( By        == NULL ) error(E_NULL, "assemblage2D_matrix_Conv_y_elt2elt1");
   /* test compatibility geom1 and geom 2 */
   if ( geom1->NBELMT != geom2->NBELMT )  error(E_SIZES, "assemblage2D_matrix_Conv_y_elt2elt1");

   /* alloc mem */
   By_el = m_get(elt1->nb_somm_cell, elt2->nb_somm_cell);

   /* initialisation of By   */
   sp_zero(By);

   for(e=0; e<geom1->NBELMT; e++)
   {
      By_el = _systel_matrix_Conv_y_elt2elt1( e , elt2 , geom2 , elt1 , geom1 , By_el );

      for (i=0; i<elt1->nb_somm_cell; i++)
      for (j=0; j<elt2->nb_somm_cell; j++)
      {
         sp_add_val( By   , geom1->NSELMT->im[e][i] , geom2->NSELMT->im[e][j] ,
                     By_el->me[i][j] );
      }
   }

   M_FREE(By_el) ;

   return sp_compact(By,MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_x_elt2elt1( u_int e , const ELT_2D *elt2 , const GEOM_2D *geom2 ,
                                                       const ELT_2D *elt1 , const GEOM_2D *geom1 , MAT *Bx_el )
{
   Real jacobien ;

   Real transformation[2][2];
   Real ksi_x, eta_x;

   MAT* work_el = m_get(elt1->nb_somm_cell,elt2->nb_somm_cell);


   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt1, geom1, transformation) ;

   /* get the transformation parameters */
   ksi_x =  transformation[0][0];/* (y3-y1)*un_sur_jacobien*/ ;
   eta_x =  transformation[0][1];/*-(y2-y1)*un_sur_jacobien*/ ;

   /* init Bx_el */
   m_zero(Bx_el);

   /* with exact integration */
   Bx_el = m_add(Bx_el,  sm_mlt( (jacobien*ksi_x), elt2->MAT_I_x_PM1dP,work_el), Bx_el);
   Bx_el = m_add(Bx_el,  sm_mlt( (jacobien*eta_x), elt2->MAT_I_y_PM1dP,work_el), Bx_el);

   /* with numerical integration  with the basis functions */
   /*
   for (i=0; i<elt1->nb_somm_cell; i++)
   for (j=0; j<elt2->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         Bx_el->me[i][j]  -= w2D[m]*jacobien*
                           elt1-> f_base[i](ksi2D[m],eta2D[m])* (
                              ksi_x*elt2->dfdx_base[j](ksi2D[m],eta2D[m]) +
                              eta_x*elt2->dfdy_base[j](ksi2D[m],eta2D[m]) );
      }
   }

   for (i=0; i<elt1->nb_somm_cell; i++)
   for (j=0; j<elt2->nb_somm_cell; j++)
   {
      if ( fabs(Bx_el->me[i][j]) < 0.0001 )
      {
         Bx_el->me[i][j] = 0.0;
      }
   }

   m_output(Bx_el);

   exit(0);
   */

   M_FREE(work_el);

   return Bx_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT * _systel_matrix_Conv_y_elt2elt1( u_int e , const ELT_2D *elt2 , const GEOM_2D *geom2 ,
                                                       const ELT_2D *elt1 , const GEOM_2D *geom1 , MAT *By_el )
{
   Real jacobien ;

   Real transformation[2][2];
   Real ksi_y, eta_y ;

   MAT* work_el = m_get(elt1->nb_somm_cell,elt2->nb_somm_cell);

   /* calcul jacobien on elt "e" */
   jacobien = eval_jac_on_e_ ( e, elt1, geom1, transformation );

   /* get the transformation parameters */
   ksi_y =  transformation[1][0];/*-(x3-x1)*un_sur_jacobien*/ ;
   eta_y =  transformation[1][1];/* (x2-x1)*un_sur_jacobien*/ ;

   /* init By_el */
   m_zero(By_el);

   /* with exact integration */
   By_el = m_add(By_el, sm_mlt( (jacobien*ksi_y), elt2->MAT_I_x_PM1dP,work_el), By_el);
   By_el = m_add(By_el, sm_mlt( (jacobien*eta_y), elt2->MAT_I_y_PM1dP,work_el), By_el);

   /* with numerical integration  with the basis functions */
   /*
   for (i=0; i<elt1->nb_somm_cell; i++)
   for (j=0; j<elt2->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_2D_TRI; m++)
      {
         By_el->me[i][j]  -= w2D[m]*jacobien*
                           elt1-> f_base[i](ksi2D[m],eta2D[m])* (
                              ksi_y*elt2->dfdx_base[j](ksi2D[m],eta2D[m]) +
                              eta_y*elt2->dfdy_base[j](ksi2D[m],eta2D[m]) );
      }
   }

   for (i=0; i<elt1->nb_somm_cell; i++)
   for (j=0; j<elt2->nb_somm_cell; j++)
   {
      if ( fabs(By_el->me[i][j]) < 0.0001 )
      {
         By_el->me[i][j] = 0.0;
      }
   }

   m_output(By_el);

   exit(0);
   */

   M_FREE(work_el);

   return By_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real element_size( int e, const ELT_2D *elt, const GEOM_2D *geom , Real center[2] )
{
   Real elt_size;

   Real x1,x2,x3 ;
   Real y1,y2,y3 ;

   Real lenAB ;
   Real lenAC ;
   Real lenBC ;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   /* calcul jacobien */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;


   lenAB = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ;
   lenAC = (x3-x1)*(x3-x1) + (y3-y1)*(y3-y1) ;
   lenBC = (x2-x3)*(x2-x3) + (y2-y3)*(y2-y3) ;

   elt_size = ( lenAB > lenAC ? lenAB : lenAC );
   elt_size = ( lenBC > elt_size ? lenBC : elt_size );

   /* center */
   center[0] = ( x1+x2+x3 )/3.0;
   center[1] = ( y1+y2+y3 )/3.0;

   /* return */
   return sqrt(elt_size);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real eval_jac_on_e  ( int e , const ELT_2D *element , const GEOM_2D *geom )
{
   /*                   _
   TRANSFORMATION DE  T  a  Ti : (Ksi,eta) coords on reference triangle :

   x = x1 + (x2-x1)*ksi + (x3-x1)*eta ;
   y = y1 + (y2-y1)*ksi + (y3-y1)*eta ;

   _
   TRANSFORMATION DE  Ti  a  T :

   ksi = [ c1 + (y3-y1)x - (x3-x1)y ]/jacobien
   eta = [ c2 - (y2-y1)x + (x2-x1)y ]/jacobien

   avec jacobien = (x2-x1)(y3-y1) - (x3-x1)(y2-y1) > 0


   d'ou :

   ksi_x =  (y3-y1)/jacobien        ksi_y = -(x3-x1)/jacobien

   eta_x = -(y2-y1)/jacobien        eta_y =  (x2-x1)/jacobien
   */

   Real x1,x2,x3 ;
   Real y1,y2,y3 ;

   Real jacobien ;

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   /* calcul jacobien */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;

   jacobien = (y3-y1)*(x2-x1) - (x3-x1)*(y2-y1) ;

#ifdef _DEBUG
   assert( jacobien > 0.0 );
#endif

   return jacobien;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real eval_jac_on_e_ ( int e , const ELT_2D *element , const GEOM_2D *geom , Real transformation[2][2])
{
   /*
                                   [ ksi_x , eta_x  ]
    transformation is a 2x2 matrix [ ksi_y , eta_y  ]

    */

   Real x1,x2,x3 ;
   Real y1,y2,y3 ;

   Real jacobien , un_sur_jacobien ;

   /*
   Real ksi_x, eta_x;
   Real ksi_y, eta_y;
   */

   MAT*  XYSOMM = geom->XYSOMM; /* alias */
   IMAT* NSELMT = geom->NSELMT; /* alias */

   /* calcul jacobien */
   x1 = XYSOMM->me[ NSELMT->im[e][0] ][0] ;
   x2 = XYSOMM->me[ NSELMT->im[e][1] ][0] ;
   x3 = XYSOMM->me[ NSELMT->im[e][2] ][0] ;

   y1 = XYSOMM->me[ NSELMT->im[e][0] ][1] ;
   y2 = XYSOMM->me[ NSELMT->im[e][1] ][1] ;
   y3 = XYSOMM->me[ NSELMT->im[e][2] ][1] ;

   jacobien = (y3-y1)*(x2-x1) - (x3-x1)*(y2-y1) ;

#ifdef _DEBUG
   assert ( jacobien > 0.0 ) ;
#endif

   un_sur_jacobien = 1.0 / jacobien ;

   transformation[0][0] = /* ksi_x */   (y3-y1)*un_sur_jacobien ;
   transformation[0][1] = /* eta_x */  -(y2-y1)*un_sur_jacobien ;
   transformation[1][0] = /* ksi_y */  -(x3-x1)*un_sur_jacobien ;
   transformation[1][1] = /* eta_y */   (x2-x1)*un_sur_jacobien ;


   return jacobien;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_cauchy_on_mat   (int axe, const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real face_center[2],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Cauchy */

   /* ----- Transformation de la matrice -------------------------------- */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] != 0) && (BC_2De_CAUCHY==Bc2D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , face_center); /* Jacobien de la transformation entre l'arete et [0:1] */
         BCval = Bc2D_evalFunction1(BC, BC_2De_CAUCHY, REF_A->ive[a], axe, face_center[0],face_center[1]);

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

static void apply_Bc2D_robin_on_mat    (int axe, const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real face_center[2],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Robin */

   /* ----- Transformation de la matrice -------------------------------- */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] != 0) && (BC_2De_ROBIN==Bc2D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , face_center); /* Jacobien de la transformation entre l'arete et [0:1] */
         BCval = Bc2D_evalFunction1(BC, BC_2De_ROBIN, REF_A->ive[a], axe, face_center[0],face_center[1]);

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

static void apply_Bc2D_neumann_on_rhs  (int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , VEC *RHS)
{
   int i,a;
   Real face_center[2],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Neumann */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   /* process */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_2De_NEUMANN==Bc2D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , face_center); /* Jacobien de la transformation entre l'arete et [0:1] */

         BCval = Bc2D_evalFunction1(BC, BC_2De_NEUMANN, REF_A->ive[a], axe, face_center[0],face_center[1]);


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

static void apply_Bc2D_robin_on_rhs    (int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , VEC *RHS)
{
   int i,a;
   Real face_center[2],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Robin */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   /* process */
   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_2De_ROBIN==Bc2D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , face_center); /* Jacobien de la transformation entre l'arete et [0:1] */
         BCval = Bc2D_evalFunction2(BC, BC_2De_ROBIN, REF_A->ive[a], axe, face_center[0],face_center[1]);

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

static Real eval_jac_on_face  ( int f , const ELT_2D *element , const GEOM_2D *geom , Real face_center[2])
{
   Real x1,y1,x2,y2;
   /*
    Egde (AB in R2 <-> Base edge in R1
    */
   Real jacobien ;

   MAT  *XYSOMM = geom->XYSOMM;
   IMAT *NSELMT = geom->NSELMT;
   IMAT *NSFACE = geom->NSFACE;

   /* calcul jacobien */
   int D = element->nb_somm_face -1;  /* nombre de sommet de l'arete -1 */

   x1 = XYSOMM->me[NSFACE->im[f][0]][0];   /* car NSFACE commemce a ZERO */
   y1 = XYSOMM->me[NSFACE->im[f][0]][1];

   x2 = XYSOMM->me[NSFACE->im[f][D]][0];
   y2 = XYSOMM->me[NSFACE->im[f][D]][1];

   face_center[0] = (x1+x2)/2.0;
   face_center[1] = (y1+y2)/2.0;

   jacobien = sqrt( (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) ); /* Jacobien de la transformation entre l'arete et [0:1] */

   return jacobien;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

