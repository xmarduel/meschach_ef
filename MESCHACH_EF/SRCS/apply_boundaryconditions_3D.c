
/*
 * Contains the functions to matrix and vector assemblage in 3D
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_3D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static Real eval_jac_on_face ( int f , const ELT_3D *element , const GEOM_3D *geom , Real face_center[3]);

/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_dirichlet_on_mat(int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A);
static void apply_Bc3D_dirichlet_on_rhs(int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , const SPMAT * A , VEC *RHS);

/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_cauchy_on_matNS   (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A);
static void apply_Bc3D_robin_on_matNS    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A);
static void apply_Bc3D_dirichlet_on_matNS(int axe, const ELT_3D *elt , const GEOM_3D *geomU, const GEOM_3D *geomP , const BC_3D *BC , SPMAT * A );

static void apply_Bc3D_neumann_on_rhsNS  (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS);
static void apply_Bc3D_robin_on_rhsNS    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS);
static void apply_Bc3D_dirichlet_on_rhsNS(int axe, const ELT_3D *elt , const GEOM_3D *geomU, const GEOM_3D *geomP , const BC_3D *BC , const SPMAT * A , VEC *RHS);

static void apply_Bc3D_dirichlet_on_rhsNS_P(const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC , const SPMAT * STOKES , VEC * RHS);
static void apply_Bc3D_dirichlet_on_matNS_P(const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC , SPMAT * STOKES );

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void transform3D_matrix_vector_with_bc( const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC ,
                                        SPMAT  *A   , VEC     *RHS              )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");

   transform3D_vector_with_bc( elt, geom, BC, A, RHS );
   transform3D_matrix_with_bc( elt, geom, BC, A);

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void transform3D_vector_with_bc( const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , const SPMAT  *A , VEC  *RHS  )
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform3D_matrix_vector_with_bc");

   axe = BC->BC_dirichlet->current_selected_axe;
   /* conditions aux limites de Dirichlet */
   if ( Bc3D_hasDirichletBC(BC, axe) )
   {
      apply_Bc3D_dirichlet_on_rhs  (axe, elt, geom, BC, A, RHS);
   }

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void transform3D_matrix_with_bc( const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A )
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_matrix_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform3D_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_matrix_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform3D_matrix_with_bc");

   axe = BC->BC_dirichlet->current_selected_axe;
   /* conditions aux limites de Dirichlet */
   if ( Bc3D_hasDirichletBC(BC, axe) )
   {
      apply_Bc3D_dirichlet_on_mat  (axe, elt, geom, BC, A);
   }

   sp_compact(A,MACHEPS);

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void transform3D_stokes_matrix_vector_with_bc( const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC ,
                                               SPMAT *A , VEC *RHS           )
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_stokes_matrix_vector_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform3D_stokes_matrix_vector_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform3D_stokes_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_stokes_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform3D_stokes_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform3D_stokes_matrix_vector_with_bc");

   transform3D_stokes_vector_with_bc( elt, geomU, geomP, BC, A, RHS );
   transform3D_stokes_matrix_with_bc( elt, geomU, geomP, BC, A );

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC* transform3D_stokes_vector_with_bc( const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC ,
                                        SPMAT *STOKES , VEC *STOKES_RHS         )
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y , AXEe_Z };

   /* check */
   if ( elt        == NULL ) error(E_NULL, "transform3D_stokes_vector_with_bc");
   if ( geomU      == NULL ) error(E_NULL, "transform3D_stokes_vector_with_bc");
   if ( geomP      == NULL ) error(E_NULL, "transform3D_stokes_vector_with_bc");
   if ( BC         == NULL ) error(E_NULL, "transform3D_stokes_vector_with_bc");
   if ( STOKES     == NULL ) error(E_NULL, "transform3D_stokes_vector_with_bc");
   if ( STOKES_RHS == NULL ) error(E_NULL, "transform3D_stokes_vector_with_bc");


   for (K=0; K<3; K++)
   {
      if ( Bc3D_hasNeumannBC(BC, axes[K]) )  /* conditions aux limites de Neumann pour [U,V,W] */
      {
         apply_Bc3D_neumann_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }

      if ( Bc3D_hasRobinBC(BC, axes[K]) )  /* conditions aux limites de Robin pour [U,V,W] */
      {
         apply_Bc3D_robin_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }

      if (  Bc3D_hasDirichletBC(BC, axes[K])  ) /* conditions aux limites de Dirichlet of [U,V,W] */
      {
         apply_Bc3D_dirichlet_on_rhsNS  (axes[K], elt, geomU, geomP, BC, STOKES, STOKES_RHS);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc3D_dirichlet_on_rhsNS_P(elt , geomU , geomP , BC , STOKES , STOKES_RHS);

   /* finito */
   return STOKES_RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* transform3D_stokes_matrix_with_bc( const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC,
                                          SPMAT *STOKES)
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y , AXEe_Z };


   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_stokes_matrix_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform3D_stokes_matrix_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform3D_stokes_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_stokes_matrix_with_bc");
   if ( STOKES == NULL ) error(E_NULL, "transform3D_stokes_matrix_with_bc");


   /* ----- Transformation de la matrice -- conditions aux limites de Cauchy */
   for (K=0; K<3; K++)
   {
      if ( Bc3D_hasCauchyBC(BC, axes[K]) )  /* conditions aux limites de Cauchy */
      {
         apply_Bc3D_cauchy_on_matNS  (axes[K], elt, geomU, BC, STOKES);
      }

      if ( Bc3D_hasRobinBC(BC, axes[K]) )  /* conditions aux limites de Robin */
      {
         apply_Bc3D_robin_on_matNS  (axes[K], elt, geomU, BC, STOKES);
      }
   }


   /* ----- Transformation de la matrice -- conditions aux limites de Dirichlet */
   for (K=0; K<3; K++)
   {
      if ( Bc3D_hasDirichletBC(BC, axes[K]) )  /* conditions aux limites de Dirichlet */
      {
         apply_Bc3D_dirichlet_on_matNS  (axes[K], elt, geomU, geomP, BC, STOKES);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc3D_dirichlet_on_matNS_P(elt , geomU , geomP , BC , STOKES );

   /* finito */
   sp_compact(STOKES, MACHEPS);

   return STOKES;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void   transform3D_navierstokes_matrix_vector_with_bc( const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP, const BC_3D *BC, SPMAT *A, VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_vector_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_vector_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_vector_with_bc");

   transform3D_navierstokes_vector_with_bc( elt, geomU, geomP, BC, A, RHS );
   transform3D_navierstokes_matrix_with_bc( elt, geomU, geomP, BC, A );

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC*   transform3D_navierstokes_vector_with_bc       ( const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP, const BC_3D *BC, SPMAT *STOKES, VEC *STOKES_RHS)
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y , AXEe_Z };

   /* check */
   if ( elt        == NULL ) error(E_NULL, "transform3D_navierstokes_vector_with_bc");
   if ( geomU      == NULL ) error(E_NULL, "transform3D_navierstokes_vector_with_bc");
   if ( geomP      == NULL ) error(E_NULL, "transform3D_navierstokes_vector_with_bc");
   if ( BC         == NULL ) error(E_NULL, "transform3D_navierstokes_vector_with_bc");
   if ( STOKES     == NULL ) error(E_NULL, "transform3D_navierstokes_vector_with_bc");
   if ( STOKES_RHS == NULL ) error(E_NULL, "transform3D_navierstokes_vector_with_bc");


   for (K=0; K<3; K++)
   {
      if ( Bc3D_hasNeumannBC(BC, axes[K]) )  /* conditions aux limites de Neumann pour [U,V,W] */
      {
         apply_Bc3D_neumann_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }

      if ( Bc3D_hasRobinBC(BC, axes[K]) )  /* conditions aux limites de Robin pour [U,V,W] */
      {
         apply_Bc3D_robin_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }

      if ( Bc3D_hasDirichletBC(BC, axes[K]) ) /* conditions aux limites de Dirichlet of [U,V,W] */
      {
         apply_Bc3D_dirichlet_on_rhsNS( axes[K] , elt , geomU , geomP , BC , STOKES , STOKES_RHS);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc3D_dirichlet_on_rhsNS_P(elt , geomU , geomP , BC , STOKES , STOKES_RHS);

   /* finito */
   return STOKES_RHS;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* transform3D_navierstokes_matrix_with_bc ( const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP, const BC_3D *BC, SPMAT *STOKES)
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y , AXEe_Z };

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_with_bc");
   if ( STOKES == NULL ) error(E_NULL, "transform3D_navierstokes_matrix_with_bc");


   /*
             !  A_11  A_12  A_13  Bx� !
             !                        !
   STOKES =  !  A_21  A_22  A_23  By� !
             !                        !
             !  A_31  A_32  A_33  Bz� !
             !                        !
             !  Bx    By    Bz    0   !


   */

   /* conditions aux limites de Cauchy or de Robin */
   for (K=0; K<3; K++)
   {
      if ( Bc3D_hasCauchyBC(BC, axes[K]) )  /* conditions aux limites de Cauchy */
      {
         apply_Bc3D_cauchy_on_matNS  (axes[K], elt, geomU, BC, STOKES);
      }

      if ( Bc3D_hasRobinBC(BC, axes[K]) )  /* conditions aux limites de Robin */
      {
         apply_Bc3D_robin_on_matNS  (axes[K], elt, geomU, BC, STOKES);
      }
   }


   /* ----- Transformation de la matrice -- Check A_kj -----------------------------------*/

   for (K=0; K<3; K++)
   {
      if ( Bc3D_hasDirichletBC(BC, axes[K]) ) /* conditions aux limites de Cauchy */
      {
          apply_Bc3D_dirichlet_on_matNS( axes[K] , elt , geomU , geomP , BC , STOKES );
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc3D_dirichlet_on_matNS_P(elt , geomU , geomP , BC , STOKES );


   /* finito */
   sp_compact(STOKES, MACHEPS);

   return STOKES;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

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

static void apply_Bc3D_dirichlet_on_rhs(int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , const SPMAT * A , VEC *RHS)
{
   int i,idx,idx2;
   Real bcval;
   SPROW *r;

   IVEC *REF_S  = geom->REF_S;
   MAT  *XYSOMM = geom->XYSOMM;


   /* ----- Transformation du second membre ----------------------------------*/
   for (i=0; i<geom->NBSOMM; i++)
   {
      if ( (REF_S->ive[i] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,axe,REF_S->ive[i])) )
      {
         RHS->ve[i] = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[i], axe,
                                         XYSOMM->me[i][0],
                                         XYSOMM->me[i][1],
                                         XYSOMM->me[i][2]);
      }
      else
      {
         r = A->row + i ;

         for ( idx=0 ; idx<r->len ; idx++ )
         {
            idx2 = r->elt[idx].col ;

            if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,axe,REF_S->ive[idx2])) )
            {
               Real bcVal = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[idx2], axe,
                                               XYSOMM->me[idx2][0],
                                               XYSOMM->me[idx2][1],
                                               XYSOMM->me[idx2][2]);

               RHS->ve[i] -= (r->elt[idx].val) * bcVal;
            }
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_dirichlet_on_mat(int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A)
{
   int i,idx,idx2;
   SPROW * r;

   IVEC *REF_S = geom->REF_S;

   /* ----- Transformation de la matrice -------------------------------- */
   for (i=0; i<geom->NBSOMM; i++)
   {
      r = A->row + i ;                      /* on se place sur la i1-ieme ligne */

      if ( (REF_S->ive[i] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC, axe, REF_S->ive[i])) ) /*on est sur un sommet "Dirichlet" */
      {
         /* for (idx=0; idx<r->len; idx++ ) r->elt[idx].val = 0.0 ; */
         sprow_smlt(r, 0.0, 0, r, TYPE_SPROW); /* on met 0.0 sur toute la ligne */
         sprow_set_val(r,i, 1.0);              /* sauf 1 sur la diagonale       */
      }
      else
      {
         for (idx=0; idx<r->len; idx++ )  /* sur cette ligne, on met 0.0 la ou  */
         {
            idx2 = r->elt[idx].col ;  /* REF_S[indice de la colonne] > 0  AND really a dirichlet bc  */

            if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC, axe, REF_S->ive[idx2])) )
            {
               r->elt[idx].val = 0.0 ;
            }
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_dirichlet_on_rhsNS_P(const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC ,
                                               const SPMAT * STOKES , VEC * RHS)
{
   int i,i3,idx,idx2;
   SPROW * r;
   int axe;

   IVEC *REF_S  = geomU->REF_S;
   MAT  *XYSOMM = geomU->XYSOMM;


   for (i=0; i<geomP->NBSOMM; i++)
   {
      i3 = i + 3*geomU->NBSOMM;

      r = STOKES->row + i3 ;

      for ( idx=0 ; idx<r->len ; idx++ )
      {
         idx2 = r->elt[idx].col ;

         axe = -1;

         if ( (0*geomU->NBSOMM <= idx2) &&(idx2 < 1*geomU->NBSOMM) )
         {
            axe = AXEe_X;
            /* and idx2 ok */
         }
         else
         if ( (1*geomU->NBSOMM <= idx2) &&(idx2 < 2*geomU->NBSOMM) )
         {
            axe = AXEe_Y;
            idx2 -= 1*geomU->NBSOMM;
         }
         else
         if ( (2*geomU->NBSOMM <= idx2) &&(idx2 < 3*geomU->NBSOMM) )
         {
            axe = AXEe_Z;
            idx2 -= 2*geomU->NBSOMM;
         }
         else
         {
            error(E_UNKNOWN, "apply_Bc3D_dirichlet_on_rhsNS_P");
         }

         if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,axe,REF_S->ive[idx2])) )
         {
            Real bcVal = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[idx2], axe,
                                            XYSOMM->me[idx2][0],
                                            XYSOMM->me[idx2][1],
                                            XYSOMM->me[idx2][2]);

            RHS->ve[i3] -= (r->elt[idx].val) * bcVal ;
         }
      }
   }

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_dirichlet_on_matNS_P(const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC ,
                                               SPMAT * STOKES )
{
   int i,i3,idx,idx2;
   SPROW * r;
   int axe;

   IVEC *REF_S = geomU->REF_S;

   /* ----- Transformation de la matrice -- Check Bx , By and Bz  -----------------------------------*/
   for (i=0; i<geomP->NBSOMM; i++)
   {
      i3 = i + 3*geomU->NBSOMM ;

      r = STOKES->row + i3 ;      /* on se place sur la i-ieme ligne */

      for (idx=0; idx<r->len; idx++ )      /* REF_S[indice de la colonne] > 0    */
      {
         idx2 = r->elt[idx].col ;

         if ( (0*geomU->NBSOMM <= idx2) &&(idx2 < 1*geomU->NBSOMM) )
         {
            axe = AXEe_X;
            /* idx2 ok */;
         }
         else
         if ( (1*geomU->NBSOMM <= idx2) &&(idx2 < 2*geomU->NBSOMM) )
         {
            axe = AXEe_Y;
            idx2 -= 1*geomU->NBSOMM;
         }
         else
         if ( (2*geomU->NBSOMM <= idx2) &&(idx2 < 3*geomU->NBSOMM) )
         {
            axe = AXEe_Z;
            idx2 -= 2*geomU->NBSOMM;
         }
         else
         {
            error(E_UNKNOWN, "apply_Bc3D_dirichlet_on_matNS_P");
         }

         if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,axe,REF_S->ive[idx2])) )
         {
            r->elt[idx].val = 0.0 ;
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_neumann_on_rhsNS  (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS)
{
   int i,a;
   Real centrum[3],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Neumann */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   int K = 0;
   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K = 0; break;
      case AXEe_Y: K = 1; break;
      case AXEe_Z: K = 2; break;

      default: error(E_UNKNOWN, "apply_Bc3D_neumann_on_rhsNS");
   }

   K_x_NBSOMM = K * geom->NBSOMM;



   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_3De_NEUMANN==Bc3D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , centrum);
         BCval = Bc3D_evalFunction1(BC, BC_3De_NEUMANN, REF_A->ive[a], axe, centrum[0],centrum[1],centrum[2]);

         sv_mlt(  BCval * jac , elt->eltDM1->VEC_I, SCM_ar);

         for(i=0; i<elt->nb_somm_face; i++)
         {
            RHS->ve[ NSFACE->im[a][i] + K_x_NBSOMM ] += SCM_ar->ve[i] ;
         }
      }
   }

   V_FREE(SCM_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_robin_on_rhsNS    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , VEC *RHS)
{
   int i,a;
   Real centrum[3],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Robin */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   int K = 0;
   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K = 0; break;
      case AXEe_Y: K = 1; break;
      case AXEe_Z: K = 2; break;

      default: error(E_UNKNOWN, "apply_Bc3D_robin_on_rhsNS");
   }

   K_x_NBSOMM = K * geom->NBSOMM;


   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_3De_ROBIN==Bc3D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , centrum); /* Jacobien de la transformation entre la face et [0:1] */
         BCval = Bc3D_evalFunction2(BC, BC_3De_ROBIN, REF_A->ive[a], axe, centrum[0],centrum[1],centrum[2]);

         sv_mlt(  BCval * jac , elt->eltDM1->VEC_I, SCM_ar);

         for(i=0; i<elt->nb_somm_face; i++)
         {
            RHS->ve[ NSFACE->im[a][i] + K_x_NBSOMM ] += SCM_ar->ve[i] ;
         }
      }
   }

   V_FREE(SCM_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_dirichlet_on_rhsNS(int axe , const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC , const SPMAT * A , VEC *RHS)
{
   int i,i1, idx,idx2;
   SPROW *r;

   IVEC *REF_S  = geomU->REF_S;
   MAT  *XYSOMM = geomU->XYSOMM;

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0*geomU->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1*geomU->NBSOMM ; break;
      case AXEe_Z: K_x_NBSOMM = 2*geomU->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc3D_dirichlet_on_rhsNS");
   }


   /* process */
   for (i=0; i<geomU->NBSOMM; i++)
   {
      i1 = i + K_x_NBSOMM;

      if ( (REF_S->ive[i] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,axe,REF_S->ive[i])) )
      {
         RHS->ve[i1] = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[i], axe,
                                                 XYSOMM->me[i][0],
                                                 XYSOMM->me[i][1],
                                                 XYSOMM->me[i][2]);
      }
      else
      {
         r = A->row + i1 ;

         for ( idx=0 ; idx<r->len ; idx++ )
         {
            idx2 = r->elt[idx].col ;

            if ( idx2 < 1*geomU->NBSOMM )
            {
               if (  (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,AXEe_X,REF_S->ive[idx2])) )
               {
                  Real bcVal = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[idx2], AXEe_X,
                                                  XYSOMM->me[idx2][0],
                                                  XYSOMM->me[idx2][1],
                                                  XYSOMM->me[idx2][2]);

                  RHS->ve[i1] -= (r->elt[idx].val) * bcVal ;
               }
            }
            else
            if ( idx2 < 2*geomU->NBSOMM )
            {
               idx2 -= 1*geomU->NBSOMM;

               if (  (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,AXEe_Y,REF_S->ive[idx2])) )
               {
                  Real bcVal = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[idx2], AXEe_Y,
                                                   XYSOMM->me[idx2][0],
                                                   XYSOMM->me[idx2][1],
                                                   XYSOMM->me[idx2][2]);

                  RHS->ve[i1] -= (r->elt[idx].val) * bcVal ;
               }
            }
            else
            if ( idx2 < 3*geomU->NBSOMM )
            {
               idx2 -= 2*geomU->NBSOMM;

               if (  (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC,AXEe_Z,REF_S->ive[idx2])) )
               {
                  Real bcVal = Bc3D_evalFunction1(BC, BC_3De_DIRICHLET, REF_S->ive[idx2], AXEe_Z,
                                                   XYSOMM->me[idx2][0],
                                                   XYSOMM->me[idx2][1],
                                                   XYSOMM->me[idx2][2]);

                  RHS->ve[i1] -= (r->elt[idx].val) * bcVal ;
               }
            }
            else
            if ( idx2 < 3*geomU->NBSOMM + geomP->NBSOMM )
            {
               ;
            }
            else
            {
               error(E_UNKNOWN, "apply_Bc3D_dirichlet_on_rhsNS");
            }
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_dirichlet_on_matNS(int axe , const ELT_3D *elt , const GEOM_3D *geomU , const GEOM_3D *geomP , const BC_3D *BC , SPMAT * A )
{
   int i,i1,idx,idx2;
   SPROW *r;

   IVEC *REF_S  = geomU->REF_S;

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0*geomU->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1*geomU->NBSOMM ; break;
      case AXEe_Z: K_x_NBSOMM = 2*geomU->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc3D_dirichlet_on_matNS");
   }


   /* process */
   for (i=0; i<geomU->NBSOMM; i++)
   {
      i1 = i + K_x_NBSOMM;

      r = A->row + i1 ;                      /* on se place sur la i1-ieme ligne */

      if ( (REF_S->ive[i] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC, axe, REF_S->ive[i])) )  /* sommet "Dirichlet" */
      {
         /* for (idx=0; idx<r->len; idx++ ) r->elt[idx].val = 0.0 ; */
         sprow_smlt(r, 0.0, 0, r, TYPE_SPROW); /* on met 0.0 sur toute la ligne */
         sprow_set_val(r,i1, 1.0);              /* sauf 1 sur la diagonale       */
      }
      else
      {
         for (idx=0; idx<r->len; idx++ )  /* sur cette ligne, on met 0.0 la ou  */
         {
            idx2 = r->elt[idx].col;       /* REF_S[indice de la colonne] > 0    */

            if ( (0*geomU->NBSOMM <= idx2) && (idx2 < 1*geomU->NBSOMM) )  /* Only A_K1 columns */
            {
               if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC, AXEe_X, REF_S->ive[idx2])) )
               {
                  r->elt[idx].val = 0.0 ;
               }
            }
            else
            if ( (1*geomU->NBSOMM <= idx2) && (idx2 < 2*geomU->NBSOMM) )  /* Only A_K2 columns */
            {
               idx2 -= 1*geomU->NBSOMM;       /* REF_S[indice de la colonne] > 0    */

               if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC, AXEe_Y, REF_S->ive[idx2])) )
               {
                  r->elt[idx].val = 0.0 ;
               }
            }
            else
            if ( (2*geomU->NBSOMM <= idx2) && (idx2 < 3*geomU->NBSOMM) )  /* Only A_K3 columns */
            {
               idx2 -= 2*geomU->NBSOMM;       /* REF_S[indice de la colonne] > 0    */

               if ( (REF_S->ive[idx2] > 0) && (BC_3De_DIRICHLET==Bc3D_getBcType(BC, AXEe_Z, REF_S->ive[idx2])) )
               {
                  r->elt[idx].val = 0.0 ;
               }
            }
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_cauchy_on_matNS   (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real centrum[3],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Cauchy */

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geom->NBSOMM; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geom->NBSOMM; break;
      case AXEe_Z: K_x_NBSOMM = 2 * geom->NBSOMM; break;

      default: error(E_UNKNOWN, "apply_Bc3D_cauchy_on_matNS");
   }


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
            sp_add_val(A, NSFACE->im[a][k1] + K_x_NBSOMM,
                          NSFACE->im[a][k2] + K_x_NBSOMM, MAT_ar->me[k1][k2] );
         }
      }
   }

   M_FREE(MAT_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc3D_robin_on_matNS    (int axe, const ELT_3D *elt , const GEOM_3D *geom , const BC_3D *BC , SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real centrum[3],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Robin */

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geom->NBSOMM; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geom->NBSOMM; break;
      case AXEe_Z: K_x_NBSOMM = 2 * geom->NBSOMM; break;

      default: error(E_UNKNOWN, "apply_Bc3D_robin_on_matNS");
   }


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
            sp_add_val(A, NSFACE->im[a][k1] + K_x_NBSOMM,
                          NSFACE->im[a][k2] + K_x_NBSOMM , MAT_ar->me[k1][k2] );
         }
      }
   }

   M_FREE(MAT_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/


/*
 sv_mlt(  BCval * jac , elt->eltDM1->VEC_I, SCM_ar);


 if ( (strcmp(elt->name_ef,"P1")==0)  || (strcmp(elt->name_ef,"P1b")==0) )
 {
    SCM_ar->ve[0] = BCval*0.5*jac;
    SCM_ar->ve[1] = BCval*0.5*jac;
 }
 else
 if ( strcmp(elt->name_ef,"P2")==0 )
 {
    SCM_ar->ve[0] = BCval*1.0/6.0*jac;
    SCM_ar->ve[1] = BCval*4.0/6.0*jac;
    SCM_ar->ve[2] = BCval*1.0/6.0*jac;
 }
 else
 if ( strcmp(elt->name_ef,"P3")==0 )
 {
    SCM_ar->ve[0] = BCval*3.0/24.0*jac;
    SCM_ar->ve[1] = BCval*9.0/24.0*jac;
    SCM_ar->ve[2] = BCval*9.0/24.0*jac;
    SCM_ar->ve[3] = BCval*3.0/24.0*jac;
 }
 else
 {
    error(E_UNKNOWN, "transform2D_vector_with_bc");
 }
 */

