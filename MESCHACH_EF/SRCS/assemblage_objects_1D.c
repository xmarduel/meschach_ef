
/*
 * Contains the functions to matrix and vector assemblage in 1D
 *
 */

#define  SQR(x)  (x)*(x)
#define  CUB(x)  (x)*(x)*(x)

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"  /* for errors definitions */
#include "MESCHACH_EF/INCLUDES/assemblage_objects_1D.h"
#include "MESCHACH_EF/INCLUDES/triangle_integration_gauss_1D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

typedef enum
{
   ASSEMBLAGEe_MASS   = 0,
   ASSEMBLAGEe_STIFF1 = 1,
   ASSEMBLAGEe_STIFF2 = 2,
   ASSEMBLAGEe_CONVEC = 3,

   ASSEMBLAGEe_AUx    = 4,
   ASSEMBLAGEe_AxU    = 5,
   ASSEMBLAGEe_A_U    = 6,

   ASSEMBLAGEe_ABx    = 7,
   ASSEMBLAGEe_A_B    = 8,

   
   ASSEMBLAGEe_VECTOR_UUx   =  9,
   ASSEMBLAGEe_VECTOR_UU    = 10,

} ASSEMBLAGEt_TYPE;



static SPMAT * _assemblage_matrix1( const ASSEMBLAGEt_TYPE type , const ELT_1D *elt, const GEOM_1D *geom , SPMAT *A  );

static MAT * _systel_matrix_Mass  ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , MAT* M_el );
static MAT * _systel_matrix_Stiff1( u_int e , const ELT_1D *elt , const GEOM_1D *geom , MAT* M_el );
static MAT * _systel_matrix_Stiff2( u_int e , const ELT_1D *elt , const GEOM_1D *geom , MAT* M_el );
static MAT * _systel_matrix_Convec( u_int e , const ELT_1D *elt , const GEOM_1D *geom , MAT* M_el );


static SPMAT * _assemblage_matrix2( const ASSEMBLAGEt_TYPE type , const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , SPMAT *A  );
static VEC   * _assemblage_vector2( const ASSEMBLAGEt_TYPE type , const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , const VEC *b , VEC *RHS);

static TENSOR * _systel_matrix_AUx ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_AxU ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , TENSOR* M_el );
static TENSOR * _systel_matrix_A_U ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , TENSOR* M_el );

static VEC * _systel_vector_fun   ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , const RHS_1D *RhsFun        , VEC *Rhs_el );
static VEC * _systel_vector_fun_o ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , const RHS_1D *RhsFun        , VEC *Rhs_el , const MAT *fbase_k_ksi_m);
static VEC * _systel_vector_abx   ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , const VEC* a , const VEC* b , VEC *Rhs_el );
static VEC * _systel_vector_ab    ( u_int e , const ELT_1D *elt , const GEOM_1D *geom , const VEC* a , const VEC* b , VEC *Rhs_el );


static Real eval_jac_on_e  ( int e , const ELT_1D *elt , const GEOM_1D *geom );
static void eval_x1x2_on_e ( int e , const ELT_1D *elt , const GEOM_1D *geom , Real *x1 , Real *x2 );


static SPMAT* transform1D_matrix_with_newbasis_bf ( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *A );
static VEC*   transform1D_vector_with_newbasis_bf ( const ELT_1D *elt , const GEOM_1D *geom , VEC *RHS );

static VEC*   vector_cbf_to_bbf                   ( const ELT_1D *elt , const GEOM_1D *geom , const VEC * RHS_cbf , VEC *RHS_bbf );

static void apply_Bc1D_cauchy_on_matrix   (const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A);
static void apply_Bc1D_robin_on_matrix    (const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A);

static void apply_Bc1D_neumann_on_vector  (const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS);
static void apply_Bc1D_robin_on_vector    (const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS);

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_Mass( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Mass )
{
   /* check */
   if ( elt  == NULL ) error(E_NULL, "assemblage1D_matrix_Mass");
   if ( geom == NULL ) error(E_NULL, "assemblage1D_matrix_Mass");
   if ( Mass == NULL ) error(E_NULL, "assemblage1D_matrix_Mass");
   
   /* delegate */
   return _assemblage_matrix1 (ASSEMBLAGEe_MASS , elt , geom , Mass );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_Stiff1( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Stiff1 )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_matrix_Stiff1");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_matrix_Stiff1");
   if ( Stiff1 == NULL ) error(E_NULL, "assemblage1D_matrix_Stiff1");

   /* delegate */
   return _assemblage_matrix1 (ASSEMBLAGEe_STIFF1 , elt , geom , Stiff1 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_Stiff2( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Stiff2 )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_matrix_Stiff2");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_matrix_Stiff2");
   if ( Stiff2 == NULL ) error(E_NULL, "assemblage1D_matrix_Stiff2");

   /* delegate */
   return _assemblage_matrix1 (ASSEMBLAGEe_STIFF2 , elt , geom , Stiff2 );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_Convec( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *Convec )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_matrix_Convec");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_matrix_Convec");
   if ( Convec == NULL ) error(E_NULL, "assemblage1D_matrix_Convec");

   /* delegate */
   return _assemblage_matrix1 (ASSEMBLAGEe_CONVEC , elt , geom , Convec );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT * assemblage1D_matrix_fromBC(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *MassDM1_BC)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_matrix_fromBC");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_matrix_fromBC");
   if ( BC     == NULL ) error(E_NULL, "assemblage1D_matrix_fromBC");
   if ( MassDM1_BC == NULL ) error(E_NULL, "assemblage1D_matrix_fromBC");
   
   /*
    conditions limites de Cauchy/Robin - la matrice est changee en consequense
    */
   
   /* verification compatibilite des dimension */
   if ( MassDM1_BC->m != geom->REF_S->dim ) error(E_SIZES, "assemblage1D_matrix_fromBC");
   
   
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      if ( Bc1D_hasCauchyBC(BC, AXEe_X) )
      {
         apply_Bc1D_cauchy_on_matrix(elt, geom, BC, MassDM1_BC);
      }
      
      if ( Bc1D_hasRobinBC(BC, AXEe_X) )
      {
         apply_Bc1D_robin_on_matrix(elt, geom, BC, MassDM1_BC);
      }
   }
   
   sp_compact(MassDM1_BC, MACHEPS);
   
   return MassDM1_BC;
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

static void apply_Bc1D_cauchy_on_matrix(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A)
{
   int a,idx2;
   Real BCval;
   
   for(a=0; a<geom->NBFACE; a++)
   {
      if ( (geom->REF_A->ive[a] > 0) && (BC_1De_CAUCHY==Bc1D_getBcType(BC, AXEe_X, geom->REF_A->ive[a])) )
      {
         idx2 = geom->NSFACE->im[a][0];
         
         BCval = Bc1D_evalFunction1(BC, BC_1De_CAUCHY, geom->REF_A->ive[a], AXEe_X, geom->XSOMM->ve[idx2]);
         
         sp_add_val(A, idx2, idx2, BCval);
      }
   }
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

static void apply_Bc1D_robin_on_matrix(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A)
{
   int a,idx2;
   Real BCval;
   
   for(a=0; a<geom->NBFACE; a++)
   {
      if ( (geom->REF_A->ive[a] > 0) && (BC_1De_ROBIN==Bc1D_getBcType(BC, AXEe_X, geom->REF_A->ive[a])) )
      {
         idx2 = geom->NSFACE->im[a][0];
         
         BCval = Bc1D_evalFunction1(BC, BC_1De_ROBIN, geom->REF_A->ive[a], AXEe_X, geom->XSOMM->ve[idx2]);
         
         sp_add_val(A, idx2, idx2, BCval);
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT* _assemblage_matrix1( ASSEMBLAGEt_TYPE type , const ELT_1D *elt , const GEOM_1D *geom , SPMAT *A  )
{
   int e; int i,j;
   
   MAT *A_el;
   MAT* (*_systel_matrix_)( u_int e , const ELT_1D *elt , const GEOM_1D *geom , MAT* M_el );

   switch( type )
   {
      case ASSEMBLAGEe_MASS  : _systel_matrix_ = _systel_matrix_Mass   ; break;
      case ASSEMBLAGEe_STIFF1: _systel_matrix_ = _systel_matrix_Stiff1 ; break;
      case ASSEMBLAGEe_STIFF2: _systel_matrix_ = _systel_matrix_Stiff2 ; break;
      case ASSEMBLAGEe_CONVEC: _systel_matrix_ = _systel_matrix_Convec ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix1");
   }

   /* alloc mem */
   A_el = m_get(elt->nb_somm_cell, elt->nb_somm_cell);

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


   if ( geom->periodicity == NON_PERIODIC_MESHe )
	{
		if ( strcmp(elt->name_ef, "S2") == 0 )
		{
         transform1D_matrix_with_newbasis_bf( elt , geom , A );
      }
      else
	   if ( strcmp(elt->name_ef, "S3") == 0 )
      {
         transform1D_matrix_with_newbasis_bf( elt , geom , A );
      }
      else
      if ( strcmp(elt->name_ef, "S4") == 0 )
      {
         transform1D_matrix_with_newbasis_bf( elt , geom , A );
      }
      else
	   if ( strcmp(elt->name_ef, "S5") == 0 )
      {
         transform1D_matrix_with_newbasis_bf( elt , geom , A );
      }
	}
   
   return sp_compact(A, MACHEPS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Mass( u_int  e , const ELT_1D *element , const GEOM_1D *geom , MAT *Mass_el )
{
   Real jac ;
   
   /* initialisation */
   m_copy(element->MAT_I_I, Mass_el);
   
   /* calcul jacobien on this element */
   jac = eval_jac_on_e ( e , element , geom );
      
   sm_mlt( jac, Mass_el, Mass_el);
   
   if ( strcmp(element->name_ef,"H3") == 0  )
   {
      int i,j;
         
      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      {
         Mass_el->me[i][2*j+1] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      {
         Mass_el->me[2*i+1][j] *= jac;
      }
   }
   
   /* with numerical integration  with the basis functions */
   /*
   m_zero(Mass_el);
    
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
      {
         Mass_el->me[i][j]  += w1D[m]*
             element-> f_base[i](ksi1D[m])*
             element-> f_base[j](ksi1D[m]);
      }
   }
   sm_mlt( jac, Mass_el, Mass_el);
   */
   

   return Mass_el;
} 

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Stiff1( u_int  e , const ELT_1D *element , const GEOM_1D *geom , MAT *Stiff1_el )
{
   Real jac, un_sur_jac;
   
   /* initialisation */
   m_copy(element->MAT_x_x, Stiff1_el);

   /* calcul jacobien on this element */ 
   jac = eval_jac_on_e ( e , element , geom );
   
   un_sur_jac = 1.0/jac ;
   
   sm_mlt(un_sur_jac, Stiff1_el, Stiff1_el);
   
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      int i,j;
   
      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      {
         Stiff1_el->me[i][2*j+1] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      {
         Stiff1_el->me[2*i+1][j] *= jac;
      }
   }

   /* with numerical integration  with the basis functions */
   /*
   m_zero(Stiff1_el);

   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
      {
         Stiff1_el->me[i][j]  += w1D[m]*
             element-> dfdx_base[i](ksi1D[m])*
             element-> dfdx_base[j](ksi1D[m]);
      }
   }
   sm_mlt( un_sur_jac, Stiff1_el, Stiff1_el);
   m_output(Stiff1_el);
   */
   
   return Stiff1_el;
} 

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Stiff2( u_int  e , const ELT_1D *element , const GEOM_1D *geom , MAT *Stiff2_el )
{
   Real jac, un_sur_jac, un_sur_jac3;
   
   /* initialisation */
   m_copy(element->MAT_xx_xx, Stiff2_el);
   
   /* calcul jacobien on this element */
   jac = eval_jac_on_e ( e , element , geom );
   
   un_sur_jac  = 1.0 / jac ;
   un_sur_jac3 = CUB(un_sur_jac);
   
   sm_mlt(un_sur_jac3, Stiff2_el, Stiff2_el);
   
   
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      int i,j;
   
      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      {
         Stiff2_el->me[i][2*j+1] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      {
         Stiff2_el->me[2*i+1][j] *= jac;
      }
   }
   
   return Stiff2_el;
} 

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static MAT* _systel_matrix_Convec( u_int  e , const ELT_1D *element , const GEOM_1D *geom , MAT *Conv_el )
{
   /* initialisation */
   m_copy(element->MAT_I_x, Conv_el);
   
   /* with numerical integration  with the basis functions */
   /*
   m_zero(Conv_el);

   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   {
      for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
      {
         Conv_el->me[i][j]  += w1D[m]*
           element-> f_base[i](ksi1D[m])*
           element-> dfdx_base[j](ksi1D[m]);
      }
   }
   */
      
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      int i,j;
   
      /* calcul jacobien on this element */
      Real jac = eval_jac_on_e ( e , element , geom );

      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      {
         Conv_el->me[i][2*j+1] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      {
         Conv_el->me[2*i+1][j] *= jac;
      }
   }
   
   return Conv_el;
} 

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_AUx( const ELT_1D *elt , const GEOM_1D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage1D_matrix_AUx :                                 */
   /*   calculate the matrix A from the term a.Ux in the PDE        */
   /*   with vector a = SUM( a_p*PHI_p )                            */
   /*               U = SUM( u_j.PHI_j )                            */
   /*   => A_ij = (SUM a_p*PHI_p)*dPHI_j*PHI_i                      */
   /*                                                               */
   /* Rappel :  TENSOR_I_x_I[i][j][k] = INTEGRAL( Ni.dNj.Nk )        */

   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_matrix_AUx");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_matrix_AUx");
   if ( a      == NULL ) error(E_NULL, "assemblage1D_matrix_AUx");
   if ( A      == NULL ) error(E_NULL, "assemblage1D_matrix_AUx");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_AUx , elt , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_AxU( const ELT_1D *elt , const GEOM_1D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage_matrix_AxU :                                   */
   /*   calculate the matrix A from the term a_x.U in the PDE       */
   /*   with vector a = SUM( a_p*PHI_p )                            */
   /*               U = SUM( u_j.PHI_j )                            */
   /*   => A_ij = (SUM a_p*dPHI_p)*PHI_j*PHI_i                      */
   /*                                                               */
   /* Rappel :  TENSOR_x_I_I[p][j][i] = INTEGRAL( dNp.Nj.Ni )        */

   /* check */
   if ( elt   == NULL ) error(E_NULL, "assemblage1D_matrix_AxU");
   if ( geom  == NULL ) error(E_NULL, "assemblage1D_matrix_AxU");
   if ( a     == NULL ) error(E_NULL, "assemblage1D_matrix_AxU");
   if ( A     == NULL ) error(E_NULL, "assemblage1D_matrix_AxU");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_AxU , elt , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* assemblage1D_matrix_A_U( const ELT_1D *elt , const GEOM_1D * geom, const VEC *a , SPMAT  *A )
{
   /* in  assemblage1D_matrix_A_U :                                 */
   /*   calculate the matrix A from the term a.U in the PDE         */
   /*   with vector a = SUM( a_p*PHI_p )                            */
   /*               U = SUM( u_j.PHI_j )                            */
   /*   => A_ij = (SUM a_p*PHI_p)*PHI_j*PHI_i                       */
   /*                                                               */
   /* Rappel : TENSOR_I_I_I->me[i][j][k] = INT(Ni*Nj*Nk)             */

   /* check */
   if ( elt   == NULL ) error(E_NULL, "assemblage1D_matrix_A_U");
   if ( geom  == NULL ) error(E_NULL, "assemblage1D_matrix_A_U");
   if ( a     == NULL ) error(E_NULL, "assemblage1D_matrix_A_U");
   if ( A     == NULL ) error(E_NULL, "assemblage1D_matrix_A_U");

   /* delegate */
   return _assemblage_matrix2( ASSEMBLAGEe_A_U , elt , geom , a , A  );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT * _assemblage_matrix2( ASSEMBLAGEt_TYPE type , const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , SPMAT *A  )
{
   u_int e;
   u_int i,j,k;

   VEC *a_in_bbf = v_copy(a, NULL); /* a is iven in cl_bf , a_in_bbf is in "basics" bf */

   MAT   *MAT_el;
   TENSOR *TENSOR_el;

   TENSOR* (*_systel_matrix_)( u_int e , const ELT_1D *elt , const GEOM_1D *geom , TENSOR* M_el );

   switch( type )
   {
      case ASSEMBLAGEe_AUx  : _systel_matrix_ = _systel_matrix_AUx ; break;
      case ASSEMBLAGEe_AxU  : _systel_matrix_ = _systel_matrix_AxU ; break;
      case ASSEMBLAGEe_A_U  : _systel_matrix_ = _systel_matrix_A_U ; break;

      default: error(E_UNKNOWN, "_assemblage_matrix2");
   }

   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */
   a_in_bbf = vector_cbf_to_bbf(elt, geom, a, a_in_bbf);
   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */

   /* mem alloc */
   MAT_el    = m_get(elt->nb_somm_cell, elt->nb_somm_cell);
   TENSOR_el = ts_get(elt->nb_somm_cell, elt->nb_somm_cell, elt->nb_somm_cell);

   /* initialisation de A */
   sp_zero(A);

   /* fill */
   for( e=0 ; e<geom->NBELMT ; e++ )
   { 
      TENSOR_el = _systel_matrix_( e , elt , geom , TENSOR_el );

      m_zero(MAT_el);  /* init for each element "e" */

      for( i=0 ; i<elt->nb_somm_cell ; i++)
      for( j=0 ; j<elt->nb_somm_cell ; j++) 
      {
         for (k=0; k<elt->nb_somm_cell ; k++)
         {
            MAT_el->me[i][j] += a_in_bbf->ve[geom->NSELMT->im[e][k]]*TENSOR_el->te[k][j][i] ;
         }

         sp_add_val( A , geom->NSELMT->im[e][i] , geom->NSELMT->im[e][j] , MAT_el->me[i][j] );
      }
   }

   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */
   A = transform1D_matrix_with_newbasis_bf(elt , geom , A);
   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */

   /* clean */
   V_FREE(a_in_bbf);

   M_FREE(MAT_el) ;
   TS_FREE(TENSOR_el) ;

   /* finito */
   return A;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_AUx( u_int  e , const ELT_1D *element , const GEOM_1D *geom , TENSOR *TENSOR_el )
{
   /* initialisation de TENSOR_el : indep. of mesh if not "H3" */
   ts_copy(element->TENSOR_I_x_I, TENSOR_el);

   /*
   
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  += w1D[m]* 
                                          element->f_base[i]   (ksi1D[m]) *
                                          element->dfdx_base[j](ksi1D[m]) *
                                          element->f_base[k]   (ksi1D[m]) ;
      }
   }
   */
   
   /* only if element is "H3" */
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      int i,j,k;
      Real jac;

      /* calcul jacobien on this element */
      jac = eval_jac_on_e( e , element , geom );

      for (i=0; i<4; i++)
      for (j=0; j<4; j++)
      for (k=0; k<2; k++)
      {
         TENSOR_el->te[i][j][2*k+1] *= jac;
      }
      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      for (k=0; k<4; k++)
      {
         TENSOR_el->te[i][2*j+1][k] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      for (k=0; k<4; k++)
      {
         TENSOR_el->te[2*i+1][j][k] *= jac;
      }
   }

   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_AxU( u_int  e , const ELT_1D *element , const GEOM_1D *geom , TENSOR  *TENSOR_el  )
{
   /* initialisation de TENSOR_el : indep. of mesh if not "H3" */
   ts_copy(element->TENSOR_x_I_I, TENSOR_el);

   /* only if element is "H3"  */
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      int i,j,k;
      Real jac;

      /* calcul jacobien on this element */
      jac = eval_jac_on_e( e , element , geom );

      for (i=0; i<4; i++)
      for (j=0; j<4; j++)
      for (k=0; k<2; k++)
      {
         TENSOR_el->te[i][j][2*k+1] *= jac;
      }
      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      for (k=0; k<4; k++)
      {
         TENSOR_el->te[i][2*j+1][k] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      for (k=0; k<4; k++)
      {
         TENSOR_el->te[2*i+1][j][k] *= jac;
      }
   }

   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static TENSOR* _systel_matrix_A_U( u_int  e , const ELT_1D *element , const GEOM_1D *geom , TENSOR  *TENSOR_el  )
{
   Real jac;

   /* initialisation de TENSOR_el : indep. of mesh if not "H3" */
   ts_copy(element->TENSOR_I_I_I, TENSOR_el);

   /*
   for (i=0; i<element->nb_somm_cell; i++)
   for (j=0; j<element->nb_somm_cell; j++)
   for (k=0; k<element->nb_somm_cell; k++)
   {
      for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
      {
         TENSOR_el->me[i][j][k]  -= w1D[m]*
               element->f_base[i](ksi1D[m]) *
               element->f_base[j](ksi1D[m]) *
               element->f_base[k](ksi1D[m]) ;
      }
   }
    */
   
   /* calcul jacobien on this element */
   jac = eval_jac_on_e ( e , element , geom );

   sts_mlt( jac, TENSOR_el, TENSOR_el);


   /* only if element is "H3"  */
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      int i,j,k;

      for (i=0; i<4; i++)
      for (j=0; j<4; j++)
      for (k=0; k<2; k++)
      {
         TENSOR_el->te[i][j][2*k+1] *= jac;
      }
      for (i=0; i<4; i++)
      for (j=0; j<2; j++)
      for (k=0; k<4; k++)
      {
         TENSOR_el->te[i][2*j+1][k] *= jac;
      }
      for (i=0; i<2; i++)
      for (j=0; j<4; j++)
      for (k=0; k<4; k++)
      {
         TENSOR_el->te[2*i+1][j][k] *= jac;
      }
   }

   return TENSOR_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage1D_vector_fun( const ELT_1D *elt , const GEOM_1D *geom , const RHS_1D *RhsFun, VEC *RHS )
{
   int e; int i; int k; int m;
   VEC *RHS_el;
   MAT *fbase_k_ksi_m;
   
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_vector_fun");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_vector_fun");
   if ( RhsFun == NULL ) error(E_NULL, "assemblage1D_vector_fun");
   if ( RHS    == NULL ) error(E_NULL, "assemblage1D_vector_fun");
   
   /* initialisation of RHS */
   v_zero(RHS);
   
	/* optimize if NULL function */
	if ( Rhs1D_is_zero(RhsFun) )
	{
		return RHS;
	}
	
	RHS_el = v_get(elt->nb_somm_cell);
	
   /* init fbase_k_ksi_m */
   fbase_k_ksi_m = m_get(elt->nb_somm_cell, NBPTS_GAUSS_1D_TRI);
   
   for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
   {
      for (k=0; k<elt->nb_somm_cell; k++)
      {
         fbase_k_ksi_m->me[k][m] = elt->f_base[k](ksi1D[m]) ;
      }
   }

   
   /* setup RHS */
   for(e=0; e<geom->NBELMT; e++)
   { 
      /*RHS_el = _systel_vector_fun( e , elt , geom , RhsFun , RHS_el );*/
      RHS_el = _systel_vector_fun_o( e , elt , geom , RhsFun , RHS_el , fbase_k_ksi_m);
      
      for (i=0; i<elt->nb_somm_cell; i++)
      {
         RHS->ve[geom->NSELMT->im[e][i]] += RHS_el->ve[i] ;
      }
   }

   V_FREE(RHS_el) ;
   M_FREE(fbase_k_ksi_m);

   if ( geom->periodicity == NON_PERIODIC_MESHe )
	{
	    if ( (strcmp(elt->name_ef,"S2") == 0) )
       {
           transform1D_vector_with_newbasis_bf( elt , geom , RHS );
       }
       else
	    if ( (strcmp(elt->name_ef,"S3") == 0) )
       {
           transform1D_vector_with_newbasis_bf( elt , geom , RHS );
       }
       else
       if ( (strcmp(elt->name_ef,"S4") == 0) )
       {
          transform1D_vector_with_newbasis_bf( elt , geom , RHS );
       }
       else
	    if ( (strcmp(elt->name_ef,"S5") == 0) )
       {
           transform1D_vector_with_newbasis_bf( elt , geom , RHS );
       }
	}
   
   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC * assemblage1D_vector_fromBC(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS)
{
   /* check pointer */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_vector_fromBC");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_vector_fromBC");
   if ( BC     == NULL ) error(E_NULL, "assemblage1D_vector_fromBC");
   if ( RHS    == NULL ) error(E_NULL, "assemblage1D_vector_fromBC");
   
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      if ( Bc1D_hasNeumannBC(BC, AXEe_X) )
      {
         apply_Bc1D_neumann_on_vector(elt, geom, BC, RHS);
      }
      
      if ( Bc1D_hasRobinBC(BC, AXEe_X) )
      {
         apply_Bc1D_robin_on_vector(elt, geom, BC, RHS);
      }
   }
   
   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC * _systel_vector_fun( u_int  e , const ELT_1D *elt , const GEOM_1D *geom , const RHS_1D *RhsFun , VEC *Rhs_el )
{
   int  k,m,i;
   Real xx[NBPTS_GAUSS_1D_TRI];
   
   Real x1,x2 ;
   Real w_m__F_x_m;
   
   Real jac ;

   /* initialisation Rhs_el */
   v_zero(Rhs_el);
   
   /* x1 & x2 */
   eval_x1x2_on_e ( e , elt , geom , &x1 , &x2 );

   /* calcul jacobien */
   jac = eval_jac_on_e ( e , elt , geom );
   /*printf("jac = %lf %lf %lf\n", jac,x1,x2);*/
   
   for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
   {
      xx[m] = x1 + (x2-x1)*ksi1D[m] ;

      w_m__F_x_m = w1D[m]*Rhs1D_evalFunction( RhsFun, 0, AXEe_X, xx[m] );
         
      for (k=0; k<elt->nb_somm_cell; k++)
      {
         Rhs_el->ve[k] += w_m__F_x_m * elt->f_base[k](ksi1D[m]) ;
      }
   }

   /* If the Source is a constant function : */
   /*
   SCM_el = v_smlt(elt->VEC_I, Rhs1D_evalFunction( RhsFun, 0, axe, 0.0), SCM_el);
   */
    
   /* post -process for H3 */
   if ( strcmp(elt->name_ef,"H3") == 0 )
   {
      for (i=0; i<2; i++)
      {
         Rhs_el->ve[2*i+1] *= jac;
      }
   }
   /* post -process for H3 */
   
   sv_mlt(jac , Rhs_el, Rhs_el);

   return Rhs_el;
} 
 
/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_fun_o( u_int  e , const ELT_1D *elt , const GEOM_1D *geom , const RHS_1D *RhsFun , VEC *Rhs_el , const MAT * fbase_k_ksi_m )
{
   int  k,m,i;
   Real xx[NBPTS_GAUSS_1D_TRI];
   
   Real x1,x2 ;
   Real w_m__F_x_m;
   
   Real jac ;

   /* initialisation Rhs_el */
   v_zero(Rhs_el);
   
   /* x1 & x2 */
   eval_x1x2_on_e ( e , elt , geom , &x1 , &x2 );

   /* calcul jacobien */
   jac = eval_jac_on_e ( e , elt , geom );
   /*printf("jac = %lf %lf %lf\n", jac,x1,x2);*/
   
   for (m=0; m<NBPTS_GAUSS_1D_TRI; m++)
   {
      xx[m] = x1 + (x2-x1)*ksi1D[m] ;

      w_m__F_x_m = w1D[m]*Rhs1D_evalFunction( RhsFun, 0, AXEe_X, xx[m] );
         
      for (k=0; k<elt->nb_somm_cell; k++)
      {
         Rhs_el->ve[k] += w_m__F_x_m * fbase_k_ksi_m->me[k][m] ;
      }
   }

   /* If the Source is a constant function : */
   /*
   SCM_el = v_smlt(elt->VEC_I, Rhs1D_evalFunction( RhsFun, 0, axe, 0.0), SCM_el);
   */
    
   /* post -process for H3 */
   if ( strcmp(elt->name_ef,"H3") == 0 )
   {
      for (i=0; i<2; i++)
      {
         Rhs_el->ve[2*i+1] *= jac;
      }
   }
   /* post -process for H3 */
   
   sv_mlt(jac , Rhs_el, Rhs_el);

   return Rhs_el;
} 

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage1D_vector_abx( const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , const VEC *b , VEC *RHS )
{   
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_vector_abx");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_vector_abx");
   if ( a      == NULL ) error(E_NULL, "assemblage1D_vector_abx");
   if ( b      == NULL ) error(E_NULL, "assemblage1D_vector_abx");
   if ( RHS    == NULL ) error(E_NULL, "assemblage1D_vector_abx");

   /* delegate */
   return _assemblage_vector2(ASSEMBLAGEe_VECTOR_UUx , elt , geom , a , b , RHS );
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* assemblage1D_vector_ab( const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , const VEC *b , VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "assemblage1D_vector_ab");
   if ( geom   == NULL ) error(E_NULL, "assemblage1D_vector_ab");
   if ( a      == NULL ) error(E_NULL, "assemblage1D_vector_ab");
   if ( b      == NULL ) error(E_NULL, "assemblage1D_vector_ab");
   if ( RHS    == NULL ) error(E_NULL, "assemblage1D_vector_ab");

   /* delegate */
   return _assemblage_vector2(ASSEMBLAGEe_VECTOR_UU  , elt , geom , a , b , RHS);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC * _assemblage_vector2( const ASSEMBLAGEt_TYPE type, const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , const VEC *b , VEC *RHS)
{
   int e; int i;

   VEC *a_in_bbf = v_copy(a, NULL); /* a is iven in cl_bf , a_in_bbf is in "basics" bf */
   VEC *b_in_bbf = v_copy(b, NULL); /* b is iven in cl_bf , b_in_bbf is in "basics" bf */

   VEC *RHS_el;

   VEC* (*_systel_vector_)( u_int e , const ELT_1D *elt , const GEOM_1D *geom , const VEC* a , const VEC* b , VEC* M_el );

   switch( type )
   {
      case ASSEMBLAGEe_VECTOR_UUx  : _systel_vector_ = _systel_vector_abx   ; break;
      case ASSEMBLAGEe_VECTOR_UU   : _systel_vector_ = _systel_vector_ab    ; break;

      default: error(E_UNKNOWN, "_assemblage_vector2");
   }

   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */
   a_in_bbf = vector_cbf_to_bbf(elt, geom, a, a_in_bbf);
   b_in_bbf = vector_cbf_to_bbf(elt, geom, b, b_in_bbf);
   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */
   
   /* alloc mem */
   RHS_el = v_get(elt->nb_somm_cell);

   /* initialisation of RHS */
   v_zero(RHS);
   
   for(e=0; e<geom->NBELMT; e++)
   { 
      RHS_el = _systel_vector_( e , elt , geom , a_in_bbf , b_in_bbf , RHS_el );
      
      for (i=0; i<elt->nb_somm_cell; i++)
      { 
         RHS->ve[geom->NSELMT->im[e][i]] += RHS_el->ve[i] ;
      }
   }

   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */
   RHS = transform1D_vector_with_newbasis_bf(elt , geom , RHS);
   /* in case of "S2", "S3", "S4" or "S5" and non-periodic bc */
   
   /* free mem */
   V_FREE(a_in_bbf);
   V_FREE(b_in_bbf);

   V_FREE(RHS_el) ;
   
   /**/
   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_abx( u_int  e , const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , const VEC *b, VEC *Rhs_el )
{
   int i,j;
    
   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   
   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                              elt->nb_somm_cell,
                              elt->nb_somm_cell);
   
   /* init */
   _systel_matrix_AUx( e , elt , geom , TENSOR_el );
   
   /* We write the componant of U.Ux in a matrix  */
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
       ab->me[i][j] = a->ve[geom->NSELMT->im[e][i]]*
                      b->ve[geom->NSELMT->im[e][j]];
   }
      
   /* Final Calculus : SCM_NL_e = TENSOR*ab */
   tsm_mlt(TENSOR_el, ab, Rhs_el);
   
   M_FREE(ab);
   TS_FREE(TENSOR_el);
   
   return Rhs_el;
} 

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC* _systel_vector_ab ( u_int  e , const ELT_1D *elt , const GEOM_1D *geom , const VEC *a , const VEC *b , VEC *Rhs_el )
{
   int i,j;
    
   MAT* ab = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   
   TENSOR *TENSOR_el = ts_get(elt->nb_somm_cell,
                              elt->nb_somm_cell,
                              elt->nb_somm_cell);
   
   /* init */
   _systel_matrix_A_U( e , elt , geom , TENSOR_el );
   
   /* We write the componant of U.Ux in a matrix */
   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
       ab->me[i][j] = a->ve[geom->NSELMT->im[e][i]]*
                      b->ve[geom->NSELMT->im[e][j]];
   }
      
   /* Final Calculus : SCM_NL_e = TENSOR*ab */
   tsm_mlt(TENSOR_el, ab, Rhs_el);
      
   M_FREE(ab);
   TS_FREE(TENSOR_el);

   return Rhs_el;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static SPMAT* transform1D_matrix_with_newbasis_bf( const ELT_1D *elt , const GEOM_1D *geom , SPMAT *A )
{
   int i;
   
   /* check pointer */
   if ( elt    == NULL ) error(E_NULL, "transform1D_matrix_with_newbasis_bf");
   if ( geom   == NULL ) error(E_NULL, "transform1D_matrix_with_newbasis_bf");
   if ( A      == NULL ) error(E_NULL, "transform1D_matrix_with_newbasis_bf");
   
   /* ----- only for S3 and S5  ----------------------------------------------------*/
   if ( !( (strcmp(elt->name_ef,"S2") == 0) || (strcmp(elt->name_ef,"S3") == 0) || (strcmp(elt->name_ef,"S4") == 0) || (strcmp(elt->name_ef,"S5") == 0) )  )
   {
      return A;
   }

   /* ----- Transformation de la matrice -------------------------------------------*/
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      /*         tr
       *    A = T . A . T            ( T is geom->BASEFUNC_BASICS_to_CL )
       */

      SPMAT *TMP = sp_get(A->m, A->n, 5);
      
      TMP = sp_mtrm_mlt(geom->BASEFUNC_BASICS_to_CL, A, TMP);
      A   = sp_m_mlt   (TMP, geom->BASEFUNC_BASICS_to_CL, A);

      /* clean */
      SP_FREE(TMP);
   }

   return A; 
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC * transform1D_vector_with_newbasis_bf( const ELT_1D *elt , const GEOM_1D *geom , VEC *RHS )
{
   int i,k;

   /* check pointer */
   if ( elt    == NULL ) error(E_NULL, "transform1D_vector_with_newbasis_bf");
   if ( geom   == NULL ) error(E_NULL, "transform1D_vector_with_newbasis_bf");
   if ( RHS    == NULL ) error(E_NULL, "transform1D_vector_with_newbasis_bf");

   
   if ( !( (strcmp(elt->name_ef,"S2")==0) || (strcmp(elt->name_ef,"S3")==0) || (strcmp(elt->name_ef,"S4") == 0) || (strcmp(elt->name_ef,"S5")==0) ) )
   {
      return RHS;
   }

   /* non periodic: perform CL */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      /*           tr
       *    RHS = T . RHS            ( T is geom->BASEFUNC_BASICS_to_CL )
       */

      VEC *TMP = v_get(RHS->dim);

      TMP = sp_vm_mlt(geom->BASEFUNC_BASICS_to_CL, RHS, TMP);

      RHS = v_copy(TMP, RHS);

      /* clean */
      V_FREE(TMP);
   }
   
   return RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC * vector_cbf_to_bbf(  const ELT_1D *elt , const GEOM_1D *geom , const VEC * RHS_cbf , VEC *RHS_bbf )
{
   char *ef = elt->name_ef;
   
   if ( !( (strcmp(ef,"S2")==0) || (strcmp(ef,"S3")==0) || (strcmp(ef,"S4")==0) || (strcmp(ef,"S5")==0) ) )
   {
      return RHS_bbf;
   }

   /* non periodic: perform CL */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      /*           
       *    RHS_bbf = T . RHS_cbf            ( T is geom->BASEFUNC_BASICS_to_CL )
       */

      RHS_bbf = sp_mv_mlt(geom->BASEFUNC_BASICS_to_CL, RHS_cbf, RHS_bbf);
   }

   return RHS_bbf;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

Real eval_jac_on_e ( int e , const ELT_1D *element , const GEOM_1D *geom )
{
   Real jac;
   Real x1,x2;
   
   /* calculus the limits of the element e */
   eval_x1x2_on_e ( e , element , geom  , &x1 , &x2 );
   
   /* calculus of the jacobian */
   jac = fabs(x2 - x1) ;
   
   return jac;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void eval_x1x2_on_e ( int e , const ELT_1D *elt , const GEOM_1D *geom , Real *x1 , Real *x2 )
{
   /* calcul x1 et x2 on the element e */
   /*
    should i put in the struct ELT_1D the indices idx1 & idx2 ?
    */
   
   if ( strcmp(elt->name_ef,"P1") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][0] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][1] ] ;
   }
   else
   if ( strcmp(elt->name_ef,"P2") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][0] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][2] ] ;
   }
   else
   if ( strcmp(elt->name_ef,"P3") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][0] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][3] ] ;
   } 
   else
   if ( strcmp(elt->name_ef,"H3") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][0] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][2] ] ;
   }
   else
   if ( strcmp(elt->name_ef,"S2") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][1] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][2] ] ;
   }
   else
   if ( strcmp(elt->name_ef,"S3") == 0 ) 
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][1] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][2] ] ;
   }
   else
   if ( strcmp(elt->name_ef,"S4") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][2] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][3] ] ;
   }
   else
   if ( strcmp(elt->name_ef,"S5") == 0 )
   {
      *x1 = geom->XSOMM->ve[ geom->NSELMT->im[e][2] ] ;
      *x2 = geom->XSOMM->ve[ geom->NSELMT->im[e][3] ] ;
   }
   else
   {
      error(E_UNKNOWN, "eval_x1x2_on_e");
   }

   /* Periodicity : x2 "must" be grather than x1 */
   if ( (e==geom->NBELMT-1) && ( geom->periodicity == PERIODIC_MESHe ) )
   {
      Real X_RIGHT = geom->X_RIGHT;
      Real X_LEFT  = geom->X_LEFT;
      
      *x2 += (X_RIGHT-X_LEFT) ;
   }
   
   /*printf(" x1 = %lf  -- x2 = %lf \n",*x1,*x2);*/

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc1D_robin_on_vector(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS)
{
   int a,idx2;
   
   for(a=0 ; a<geom->NBFACE; a++)
   {
      if ( (geom->REF_A->ive[a] > 0) && (BC_1De_ROBIN==Bc1D_getBcType(BC, AXEe_X, geom->REF_A->ive[a])) )
      {
         idx2 = geom->NSFACE->im[a][0];
         
         RHS->ve[idx2] += Bc1D_evalFunction2(BC, BC_1De_ROBIN, geom->REF_A->ive[a], AXEe_X, geom->XSOMM->ve[idx2]);
      }
   }
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

static void apply_Bc1D_neumann_on_vector(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, VEC *RHS)
{
   int a,idx2;
   
   for(a=0; a<geom->NBFACE; a++)
   {
      if ( (geom->REF_A->ive[a] > 0) && (BC_1De_NEUMANN==Bc1D_getBcType(BC, AXEe_X, geom->REF_A->ive[a])) )
      {
         idx2 = geom->NSFACE->im[a][0];
         
         RHS->ve[idx2] += Bc1D_evalFunction1(BC, BC_1De_NEUMANN, geom->REF_A->ive[a], AXEe_X, geom->XSOMM->ve[idx2]);
      }
   }
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

