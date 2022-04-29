
/*
 * Contains the functions to matrix and vector assemblage in 2D
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_2D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"


/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

static Real eval_jac_on_face(int f, const ELT_2D *elt, const GEOM_2D *geom, Real face_center[2]);

/* -------------------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_dirichlet_on_mat(int axe, const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A);
static void apply_Bc2D_dirichlet_on_rhs(int axe, const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, const SPMAT *A, VEC *RHS);

/* -------------------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_neumann_on_rhsNS  (int axe, const ELT_2D *elt, const GEOM_2D *geom , const BC_2D *BC , VEC *RHS);
static void apply_Bc2D_robin_on_rhsNS    (int axe, const ELT_2D *elt, const GEOM_2D *geom , const BC_2D *BC , VEC *RHS);
static void apply_Bc2D_dirichlet_on_rhsNS(int axe, const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP , const BC_2D *BC , const SPMAT *A , VEC *RHS);

static void apply_Bc2D_cauchy_on_matNS   (int axe, const ELT_2D *elt, const GEOM_2D *geom , const BC_2D *BC , SPMAT *A);
static void apply_Bc2D_robin_on_matNS    (int axe, const ELT_2D *elt, const GEOM_2D *geom , const BC_2D *BC , SPMAT *A);
static void apply_Bc2D_dirichlet_on_matNS(int axe, const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP , const BC_2D *BC, SPMAT *A);

static void apply_Bc2D_dirichlet_on_rhsNS_P(const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *BC, const SPMAT *A, VEC *RHS);
static void apply_Bc2D_dirichlet_on_matNS_P(const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *BC, SPMAT *A);

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

void transform2D_matrix_vector_with_bc(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A, VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_matrix_vector_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform2D_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform2D_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform2D_matrix_vector_with_bc");

   transform2D_vector_with_bc(elt, geom, BC, A, RHS);
   transform2D_matrix_with_bc(elt, geom, BC, A);
   
   return;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

void transform2D_vector_with_bc(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, const SPMAT *A, VEC *RHS)
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_vector_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform2D_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform2D_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform2D_vector_with_bc");

   axe = BC->BC_dirichlet->current_selected_axe;
   /* conditions aux limites de Dirichlet */
   if ( Bc2D_hasDirichletBC(BC, axe) )
   {
      apply_Bc2D_dirichlet_on_rhs(axe, elt, geom, BC, A, RHS);
   }
   
   return;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

void transform2D_matrix_with_bc(const ELT_2D *elt, const GEOM_2D *geom, const BC_2D *BC, SPMAT *A)
{
   int axe;

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_matrix_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform2D_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_matrix_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform2D_matrix_with_bc");

   axe = BC->BC_dirichlet->current_selected_axe;
   /* conditions aux limites de Dirichlet */
   if ( Bc2D_hasDirichletBC(BC, axe) )
   {
      apply_Bc2D_dirichlet_on_mat(axe, elt, geom, BC, A);
   }
      
   sp_compact(A, MACHEPS);
   
   return;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

void transform2D_stokes_matrix_vector_with_bc(const ELT_2D *elt, const GEOM_2D *geomU, const GEOM_2D *geomP, const BC_2D *BC, SPMAT *A, VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_stokes_matrix_vector_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform2D_stokes_matrix_vector_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform2D_stokes_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_stokes_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform2D_stokes_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform2D_stokes_matrix_vector_with_bc");

   transform2D_stokes_vector_with_bc( elt, geomU, geomP, BC, A, RHS );
   transform2D_stokes_matrix_with_bc( elt, geomU, geomP, BC, A );

   return;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

VEC* transform2D_stokes_vector_with_bc( const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP , const BC_2D *BC ,
                                        SPMAT *STOKES , VEC *STOKES_RHS )
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y };
   
   /* check */
   if ( elt        == NULL ) error(E_NULL, "transform2D_stokes_vector_with_bc");
   if ( geomU      == NULL ) error(E_NULL, "transform2D_stokes_vector_with_bc");
   if ( geomP      == NULL ) error(E_NULL, "transform2D_stokes_vector_with_bc");
   if ( BC         == NULL ) error(E_NULL, "transform2D_stokes_vector_with_bc");
   if ( STOKES     == NULL ) error(E_NULL, "transform2D_stokes_vector_with_bc");
   if ( STOKES_RHS == NULL ) error(E_NULL, "transform2D_stokes_vector_with_bc");


   /* conditions aux limites de Neumann ou de Robin pour [U,V] */
   for (K=0; K<2; K++)
   {
      if ( Bc2D_hasNeumannBC(BC, axes[K]) )
      {
         apply_Bc2D_neumann_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }

      if ( Bc2D_hasRobinBC(BC, axes[K]) )
      {
         apply_Bc2D_robin_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }
   }

   /* conditions aux limites de Dirichlet pour [U,V] */
   for (K=0; K<2; K++)
   {
      if ( Bc2D_hasDirichletBC(BC, axes[K]) )
      {
         apply_Bc2D_dirichlet_on_rhsNS(axes[K], elt, geomU, geomP, BC, STOKES, STOKES_RHS);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc2D_dirichlet_on_rhsNS_P(elt, geomU, geomP, BC, STOKES, STOKES_RHS);
   
   /* finito */
   return STOKES_RHS;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

SPMAT* transform2D_stokes_matrix_with_bc( const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP , const BC_2D *BC,
                                          SPMAT *STOKES)
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y };

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_stokes_matrix_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform2D_stokes_matrix_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform2D_stokes_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_stokes_matrix_with_bc");
   if ( STOKES == NULL ) error(E_NULL, "transform2D_stokes_matrix_with_bc");


   /* conditions aux limites de Cauchy ou de Robin */
   for (K=0; K<2; K++)
   {
      /* conditions aux limites de Cauchy */
      if ( Bc2D_hasCauchyBC(BC, axes[K]) )
      {
         apply_Bc2D_cauchy_on_matNS   (axes[K], elt, geomU, BC, STOKES);
      }

      /* conditions aux limites de Robin */
      if ( Bc2D_hasRobinBC(BC, axes[K]) )
      {
         apply_Bc2D_robin_on_matNS   (axes[K], elt, geomU, BC, STOKES);
      }
   }


   /* ----- Transformation de la matrice -------------------------------------*/
   for (K=0; K<2; K++)
   {
      /* conditions aux limites de Dirichlet */
      if ( Bc2D_hasDirichletBC(BC, axes[K]) )
      {
         apply_Bc2D_dirichlet_on_matNS   (axes[K], elt, geomU, geomP, BC, STOKES);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc2D_dirichlet_on_matNS_P(elt , geomU , geomP , BC , STOKES );

   /* finito */
   sp_compact(STOKES, MACHEPS);

   return STOKES;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

void   transform2D_navierstokes_matrix_vector_with_bc( const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP, const BC_2D *BC, SPMAT *A, VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_vector_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_vector_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_vector_with_bc");

   transform2D_navierstokes_vector_with_bc( elt, geomU, geomP, BC, A, RHS );
   transform2D_navierstokes_matrix_with_bc( elt, geomU, geomP, BC, A );
   
   return;
}

/*-----------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------*/

VEC*   transform2D_navierstokes_vector_with_bc ( const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP, const BC_2D *BC, SPMAT *STOKES, VEC *STOKES_RHS)
{
   int K;

   int axes[] = { AXEe_X , AXEe_Y };
   
   /* check */
   if ( elt        == NULL ) error(E_NULL, "transform2D_navierstokes_vector_with_bc");
   if ( geomU      == NULL ) error(E_NULL, "transform2D_navierstokes_vector_with_bc");
   if ( geomP      == NULL ) error(E_NULL, "transform2D_navierstokes_vector_with_bc");
   if ( BC         == NULL ) error(E_NULL, "transform2D_navierstokes_vector_with_bc");
   if ( STOKES     == NULL ) error(E_NULL, "transform2D_navierstokes_vector_with_bc");
   if ( STOKES_RHS == NULL ) error(E_NULL, "transform2D_navierstokes_vector_with_bc");


   /* conditions aux limites de Neumann ou de Robin pour [U,V] */
   for (K=0; K<2; K++)
   {
      /* conditions aux limites de Neumann */
      if ( Bc2D_hasNeumannBC(BC, axes[K]) )
      {
         apply_Bc2D_neumann_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }

      /* conditions aux limites de Robin */
      if ( Bc2D_hasRobinBC(BC, axes[K]) )
      {
         apply_Bc2D_robin_on_rhsNS  (axes[K], elt, geomU, BC, STOKES_RHS);
      }
   }

   /* conditions aux limites de Dirichlet pour [U,V] */
   for (K=0; K<2; K++)
   {
      if ( Bc2D_hasDirichletBC(BC, axes[K]) )
      { 
         apply_Bc2D_dirichlet_on_rhsNS  (axes[K], elt, geomU, geomP, BC, STOKES, STOKES_RHS);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc2D_dirichlet_on_rhsNS_P(elt , geomU , geomP , BC , STOKES , STOKES_RHS);

   /* finito */
   return STOKES_RHS;

}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

SPMAT* transform2D_navierstokes_matrix_with_bc ( const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP, const BC_2D *BC, SPMAT *STOKES)
{
   int K;
   int axes[] = { AXEe_X , AXEe_Y };

   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_with_bc");
   if ( geomU  == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_with_bc");
   if ( geomP  == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_with_bc");
   if ( STOKES == NULL ) error(E_NULL, "transform2D_navierstokes_matrix_with_bc");


   /*
             !  A_11  A_12   Bx� !
             !                   !
   STOKES =  !  A_21  A_22   By� !
             !                   !
             !  Bx    By     0   !
   
   
   */

   /* conditions aux limites de Cauchy */
   for (K=0; K<2; K++)
   {
      /* conditions aux limites de Cauchy */
      if ( Bc2D_hasCauchyBC(BC, axes[K]) ) 
      {
         apply_Bc2D_cauchy_on_matNS   (axes[K], elt, geomU, BC, STOKES);
      }

      /* conditions aux limites de Robin */
      if ( Bc2D_hasRobinBC(BC, axes[K]) )
      {
         apply_Bc2D_robin_on_matNS   (axes[K], elt, geomU, BC, STOKES);
      }
   }


   /* conditions aux limites de Dirichlet -- Check A_KK ----------------------------------- */
   for (K=0; K<2; K++)
   {
      /* conditions aux limites de Dirichlet */
      if ( Bc2D_hasDirichletBC(BC, axes[K]) )
      {
         apply_Bc2D_dirichlet_on_matNS  (axes[K], elt, geomU, geomP, BC, STOKES);
      }
   }

   /* conditions aux limites de Dirichlet pour P */
   apply_Bc2D_dirichlet_on_matNS_P(elt , geomU , geomP , BC , STOKES );

   /* finito */
   sp_compact(STOKES, MACHEPS);

   return STOKES;
   
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

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_dirichlet_on_rhs(int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , const SPMAT * A , VEC *RHS)
{
   int i,idx,idx2;
   Real bcval;
   SPROW *r;
   
   IVEC *REF_S  = geom->REF_S;
   MAT  *XYSOMM = geom->XYSOMM;
   

   /* ----- Transformation du second membre ----------------------------------*/
   for (i=0; i<geom->NBSOMM; i++)
   {
      if ( (REF_S->ive[i] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[i])) )
      {
         RHS->ve[i] = Bc2D_evalFunction1(BC, BC_2De_DIRICHLET, REF_S->ive[i], axe,
                                         XYSOMM->me[i][0],
                                         XYSOMM->me[i][1]);
      }
      else
      {
         r = A->row + i ;
         
         for ( idx=0 ; idx<r->len ; idx++ )
         {
            idx2 = r->elt[idx].col ;

            if ( (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[idx2])) )
            {
               Real bcVal = Bc2D_evalFunction1(BC, BC_2De_DIRICHLET, REF_S->ive[idx2], axe,
                                               XYSOMM->me[idx2][0],
                                               XYSOMM->me[idx2][1]);

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

static void apply_Bc2D_dirichlet_on_mat(int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , SPMAT *A)
{
   int i,idx,idx2;
   SPROW * r;

   IVEC *REF_S = geom->REF_S;


   /* ----- Transformation de la matrice -------------------------------- */
   for (i=0; i<geom->NBSOMM; i++)
   {
      r = A->row + i ;                      /* on se place sur la i1-ieme ligne */

      if ( (REF_S->ive[i] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[i])) ) /*on est sur un sommet "Dirichlet" */
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

            if ( (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[idx2])) )
            {
               r->elt[idx].val = 0.0 ;
            }
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_dirichlet_on_rhsNS_P(const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP , const BC_2D *BC ,
                                               const SPMAT * STOKES , VEC * RHS)
{
   int i,i3,idx,idx2;
   SPROW * r;
   int axe;

   IVEC *REF_S  = geomU->REF_S;
   MAT  *XYSOMM = geomU->XYSOMM;


   for (i=0; i<geomP->NBSOMM; i++)
   {
      i3 = i + 2*geomU->NBSOMM;

      r = STOKES->row + i3 ;

      for ( idx=0 ; idx<r->len ; idx++ )
      {
         idx2 = r->elt[idx].col ;

         axe = -1;

         if ( (0*geomU->NBSOMM <= idx2) && (idx2 < 1*geomU->NBSOMM) )
         {
            axe = AXEe_X;
            /* and idx2 ok */
         }
         else
         if ( (1*geomU->NBSOMM <= idx2) && (idx2 < 2*geomU->NBSOMM) )
         {
            axe = AXEe_Y;
            idx2 -= geomU->NBSOMM;
         }
         else
         {
            error(E_UNKNOWN, "apply_Bc2D_dirichlet_on_PStokesvector");
         }

         if ( (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[idx2])) )
         {
            Real bcVal = Bc2D_evalFunction1(BC, BC_2De_DIRICHLET, REF_S->ive[idx2], axe,
                                            XYSOMM->me[idx2][0],
                                            XYSOMM->me[idx2][1]);

            RHS->ve[i3] -= (r->elt[idx].val) * bcVal ;
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_dirichlet_on_matNS_P(const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP , const BC_2D *BC ,
                                               SPMAT * STOKES )
{
   int i,i3,idx,idx2;
   SPROW * r;
   int axe;

   IVEC *REF_S = geomU->REF_S;
   
   /* ----- Transformation de la matrice -- Check Bx and By  -----------------------------------*/   
   for (i=0; i<geomP->NBSOMM; i++)
   {
      i3 = i + 2*geomU->NBSOMM ;

      r = STOKES->row + i3 ;      /* on se place sur la i3-ieme ligne */

      for (idx=0; idx<r->len; idx++ )      /* REF_S[indice de la colonne] > 0    */
      {
         idx2 = r->elt[idx].col ;

         if ( (0*geomU->NBSOMM <= idx2) && (idx2 < 1*geomU->NBSOMM) )
         {
            axe = AXEe_X;
            /* idx2 ok */;
         }
         else
            if ( (1*geomU->NBSOMM <= idx2) && (idx2 < 2*geomU->NBSOMM) )
            {
               axe = AXEe_Y;
               idx2 -= geomU->NBSOMM;
            }
         else
         {
            error(E_UNKNOWN, "apply_Bc2D_dirichlet_on_PStokesmatrix");
         }

         if ( (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[idx2])) )
         {
            r->elt[idx].val = 0.0 ;
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_neumann_on_rhsNS  (int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , VEC *RHS)
{
   int i,a;
   Real face_center[2],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Neumann */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geom->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geom->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc2D_neumann_on_rhsNS");
   }

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
            RHS->ve[ NSFACE->im[a][i] + K_x_NBSOMM ] += SCM_ar->ve[i] ;
         }
      }
   }

   V_FREE(SCM_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_robin_on_rhsNS    (int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , VEC *RHS)
{
   int i,a;
   Real face_center[2],jac, BCval;
   VEC *SCM_ar = v_get(elt->nb_somm_face); /* integration sur les aretes pour cl de Robin */

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geom->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geom->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc2D_robin_on_rhsNS");
   }


   for( a=0 ; a<geom->NBFACE ; a++ )
   {
      if ( (REF_A->ive[a] > 0) && (BC_2De_ROBIN==Bc2D_getBcType(BC,axe,REF_A->ive[a])) )
      {
         jac   = eval_jac_on_face  ( a , elt , geom , face_center); /* Jacobien de la transformation entre l'arete et [0:1] */
         BCval = Bc2D_evalFunction2(BC, BC_2De_ROBIN, REF_A->ive[a], axe, face_center[0],face_center[1]);

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

static void apply_Bc2D_dirichlet_on_rhsNS(int axe , const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP , const BC_2D *BC , const SPMAT * A , VEC *RHS)
{
   int i,i1,i3,idx,idx2;
   SPROW * r;

   IVEC *REF_S  = geomU->REF_S;
   MAT  *XYSOMM = geomU->XYSOMM;
   
   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geomU->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geomU->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc2D_dirichlet_on_rhsNS");
   }

   /* ----- Transformation du second membre ----------------------------------*/
   for (i=0; i<geomU->NBSOMM; i++)
   {
      i1 = i + K_x_NBSOMM;

      if ( (REF_S->ive[i] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[i])) )
      {
         RHS->ve[i1] = Bc2D_evalFunction1(BC, BC_2De_DIRICHLET, REF_S->ive[i], axe,
                                                 XYSOMM->me[i][0],
                                                 XYSOMM->me[i][1]);
      }
      else
      {
         r = A->row + i1 ;
         
         for ( idx=0 ; idx<r->len ; idx++ )
         {
            idx2 = r->elt[idx].col ;

            if ( idx2 < 1*geomU->NBSOMM )
            {
               if (  (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,AXEe_X,REF_S->ive[idx2])) )
               {
                  Real bcVal = Bc2D_evalFunction1(BC, BC_2De_DIRICHLET, REF_S->ive[idx2], AXEe_X,
                                                  XYSOMM->me[idx2][0],
                                                  XYSOMM->me[idx2][1]);

                  RHS->ve[i1] -= (r->elt[idx].val) * bcVal ;
               }
            }
            else
            if ( idx2 < 2*geomU->NBSOMM )
            {
               idx2 -= geomU->NBSOMM;

               if (  (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,AXEe_Y,REF_S->ive[idx2])) )
               {
                  Real bcVal = Bc2D_evalFunction1(BC, BC_2De_DIRICHLET, REF_S->ive[idx2], AXEe_Y,
                                                     XYSOMM->me[idx2][0],
                                                     XYSOMM->me[idx2][1]);

                  RHS->ve[i1] -= (r->elt[idx].val) * bcVal ;
               }
            }
            else
            if ( idx2 < 2*geomU->NBSOMM + geomP->NBSOMM )
            {
               ;
            }
            else
            {
               error(E_UNKNOWN, "apply_Bc2D_dirichlet_on_rhsNS");
            }
         }
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_cauchy_on_matNS   (int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real face_center[2],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Cauchy */

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geom->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geom->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc2D_cauchy_on_matNS");
   }


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
            sp_add_val(A, NSFACE->im[a][k1] + K_x_NBSOMM,
                          NSFACE->im[a][k2] + K_x_NBSOMM, MAT_ar->me[k1][k2] );
         }
      }
   }

   M_FREE(MAT_ar);
}


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_robin_on_matNS    (int axe, const ELT_2D *elt , const GEOM_2D *geom , const BC_2D *BC , SPMAT *A)
{
   int a;
   int k1,k2;

   IVEC *REF_A  = geom->REF_A;
   IMAT *NSFACE = geom->NSFACE;

   Real face_center[2],jac,BCval;
   MAT *MAT_ar = m_get(elt->nb_somm_face,elt->nb_somm_face); /* integration sur les faces pour cl de Robin */

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geom->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geom->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc2D_robin_on_matNS");
   }

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
            sp_add_val(A, NSFACE->im[a][k1] + K_x_NBSOMM,
                          NSFACE->im[a][k2] + K_x_NBSOMM , MAT_ar->me[k1][k2] );
         }
      }
   }

   M_FREE(MAT_ar);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void apply_Bc2D_dirichlet_on_matNS(int axe , const ELT_2D *elt , const GEOM_2D *geomU , const GEOM_2D *geomP , const BC_2D *BC , SPMAT * A )
{
   int i,i1, idx,idx2;
   SPROW * r;

   IVEC *REF_S  = geomU->REF_S;

   int K_x_NBSOMM;

   switch(axe)
   {
      case AXEe_X: K_x_NBSOMM = 0 * geomU->NBSOMM ; break;
      case AXEe_Y: K_x_NBSOMM = 1 * geomU->NBSOMM ; break;

      default: error(E_UNKNOWN, "apply_Bc2D_dirichlet_on_matNS");
   }

   /* Transformation de la matrice */
   for (i=0; i<geomU->NBSOMM; i++)
   {
      i1 = i + K_x_NBSOMM;

      r = A->row + i1 ;                      /* on se place sur la i1-ieme ligne */

      if ( (REF_S->ive[i] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,axe,REF_S->ive[i])) )  /* somm "Dirichlet" */
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
               if ( (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,AXEe_X,REF_S->ive[idx2])) )
               {
                  r->elt[idx].val = 0.0 ;
               }
            }
            else
            if ( (1*geomU->NBSOMM <= idx2) && (idx2 < 2*geomU->NBSOMM) )  /* Only A_K2 columns */
            {
               idx2 -= geomU->NBSOMM;       /* REF_S[indice de la colonne] > 0    */

               if ( (REF_S->ive[idx2] > 0) && (BC_2De_DIRICHLET==Bc2D_getBcType(BC,AXEe_Y,REF_S->ive[idx2])) )
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




