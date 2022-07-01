
/*
 * Contains the functions to matrix and vector assemblage in 1D
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_1D.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"


/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

static void apply_Bc1D_dirichlet_on_matrix(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A);
static void apply_Bc1D_dirichlet_on_matrix_SPLINES(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A);

static void apply_Bc1D_dirichlet_on_vector(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, const SPMAT *A, VEC *RHS);
static void apply_Bc1D_dirichlet_on_vector_SPLINES(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, const SPMAT *A, VEC *RHS);

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

void transform1D_matrix_vector_with_bc(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A, VEC *RHS)
{
   /* check */
   if ( elt    == NULL ) error(E_NULL, "transform1D_matrix_vector_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform1D_matrix_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform1D_matrix_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform1D_matrix_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform1D_matrix_vector_with_bc");

   transform1D_vector_with_bc(elt, geom, BC, A, RHS );
   transform1D_matrix_with_bc(elt, geom, BC, A);

   return ;
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

void transform1D_vector_with_bc(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, const SPMAT *A, VEC *RHS)
{
   /*
    conditions limites de Dirichlet - le vecteur est change en consequense
    */

   /* check pointer */
   if ( elt    == NULL ) error(E_NULL, "transform1D_vector_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform1D_vector_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform1D_vector_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform1D_vector_with_bc");
   if ( RHS    == NULL ) error(E_NULL, "transform1D_vector_with_bc");

   /* verification compatibilite des dimension */
   if ( A->m != RHS->dim )         error(E_SIZES, "transform1D_vector_with_bc");
   //if ( A->m != geom->REF_S->dim ) error(E_SIZES, "transform1D_vector_with_bc");
   //if ( A->m != geom->XSOMM->dim ) error(E_SIZES, "transform1D_vector_with_bc");

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      if ( Bc1D_hasDirichletBC(BC, AXEe_X) )
      {
         apply_Bc1D_dirichlet_on_vector(elt, geom, BC, A, RHS);
      }
   }

   return;
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

void transform1D_matrix_with_bc(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A)
{
   /*
    conditions limites de Dirichlet - la matrice est changee en consequense
   */

   /* check pointer */
   if ( elt    == NULL ) error(E_NULL, "transform1D_matrix_with_bc");
   if ( geom   == NULL ) error(E_NULL, "transform1D_matrix_with_bc");
   if ( BC     == NULL ) error(E_NULL, "transform1D_matrix_with_bc");
   if ( A      == NULL ) error(E_NULL, "transform1D_matrix_with_bc");

   /* verification compatibilite des dimension */
   //if ( A->m != geom->REF_S->dim ) error(E_SIZES, "transform1D_matrix_with_bc");


   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      if ( Bc1D_hasDirichletBC(BC, AXEe_X) )
      {
         apply_Bc1D_dirichlet_on_matrix(elt, geom, BC, A);
      }
   }

   sp_compact(A, MACHEPS);

   return;
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

static void apply_Bc1D_dirichlet_on_matrix(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A)
{
   if ( strcmp(elt->name_ef, "S2") == 0 )
   {
      return apply_Bc1D_dirichlet_on_matrix_SPLINES(elt, geom, BC, A);
   }
   if ( strcmp(elt->name_ef, "S3") == 0 )
   {
      return apply_Bc1D_dirichlet_on_matrix_SPLINES(elt, geom, BC, A);
   }
   if ( strcmp(elt->name_ef, "S4") == 0 )
   {
      return apply_Bc1D_dirichlet_on_matrix_SPLINES(elt, geom, BC, A);
   }
   if ( strcmp(elt->name_ef, "S5") == 0 )
   {
      return apply_Bc1D_dirichlet_on_matrix_SPLINES(elt, geom, BC, A);
   }

   int i,idx,idx2;
   SPROW *r;

   for(i=0; i<A->m; i++)
   {
      if ( (geom->REF_S->ive[i] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[i])) )
      {
         r = A->row + i ;
         for (idx=0; idx<r->len; idx++ ) r->elt[idx].val = 0.0 ; /* 0 on the whole row */
         sprow_set_val(r,i, 1.0);    /* but 1 on the diagonal element */
      }
      else
      {
         r = A->row + i ;
         for (idx=0; idx<r->len; idx++ )
         {
            idx2 = r->elt[idx].col ;

            if ( (geom->REF_S->ive[idx2] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[idx2])) )
            {
               r->elt[idx].val = 0.0 ;
            }
         }
      }
   }
}

static void apply_Bc1D_dirichlet_on_matrix_SPLINES(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, SPMAT *A)
{
   int i,idx,idx2;
   SPROW *r;

   for(i=0; i<A->m; i++)
   {
      int node_i = geom->SPLINES_DOF_TO_REF_S->ive[i];

      if ( (node_i > -1) && (geom->REF_S->ive[node_i] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[node_i])) )
      {
         r = A->row + i ;
         for (idx=0; idx<r->len; idx++ ) r->elt[idx].val = 0.0 ; /* 0 on the whole row */
         sprow_set_val(r,i, 1.0);    /* but 1 on the diagonal element */
      }
      else
      {
         r = A->row + i ;
         for (idx=0; idx<r->len; idx++ )
         {
            idx2 = r->elt[idx].col ;

            int node_idx2 = geom->SPLINES_DOF_TO_REF_S->ive[idx2];

            if ( (node_idx2 > -1) && (geom->REF_S->ive[node_idx2] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[node_idx2])) )
            {
               r->elt[idx].val = 0.0 ;
            }
         }
      }
   }
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

static void apply_Bc1D_dirichlet_on_vector(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, const SPMAT *A, VEC *RHS)
{
   if (strcmp(elt->name_ef, "S2") == 0 )
   {
      return apply_Bc1D_dirichlet_on_vector_SPLINES(elt, geom, BC, A, RHS);
   }
   if (strcmp(elt->name_ef, "S3") == 0 )
   {
      return apply_Bc1D_dirichlet_on_vector_SPLINES(elt, geom, BC, A, RHS);
   }
   if (strcmp(elt->name_ef, "S4") == 0 )
   {
      return apply_Bc1D_dirichlet_on_vector_SPLINES(elt, geom, BC, A, RHS);
   }
   if (strcmp(elt->name_ef, "S5") == 0 )
   {
      return apply_Bc1D_dirichlet_on_vector_SPLINES(elt, geom, BC, A, RHS);
   }

   int i,idx,idx2;
   SPROW *r;

   for (i=0; i<geom->NBSOMM; i++)
   {
      if ( (geom->REF_S->ive[i] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[i])) )
      {
         Real BCVal = Bc1D_evalFunction1(BC, BC_1De_DIRICHLET, geom->REF_S->ive[i] , AXEe_X , geom->XSOMM->ve[i] );

         RHS->ve[i] = BCVal;
      }
      else
      {
         r = A->row + i ;

         for ( idx=0 ; idx<r->len ; idx++ )
         {
            idx2 = r->elt[idx].col ;

            if ( (geom->REF_S->ive[idx2] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[idx2])) )
            {
               Real BCVal = Bc1D_evalFunction1(BC, BC_1De_DIRICHLET, geom->REF_S->ive[idx2], AXEe_X, geom->XSOMM->ve[idx2] ) ;

               RHS->ve[i] -= (r->elt[idx].val) * BCVal ;
            }
         }
      }
   }
}

static void apply_Bc1D_dirichlet_on_vector_SPLINES(const ELT_1D *elt, const GEOM_1D *geom, const BC_1D *BC, const SPMAT *A, VEC *RHS)
{
   // NX segment -> NBSOMM = NX + 1
   //            -> NB_DOF = NX + 2 (S2)
   //            -> NB_DOF = NX + 3 (S3)
   //            -> NB_DOF = NX + 4 (S4)
   //            -> NB_DOF = NX + 5 (S5)

   int i,idx,idx2;
   SPROW *r;

   for (i=0; i<geom->NB_DOF; i++)
   {
      int node_i = geom->SPLINES_DOF_TO_REF_S->ive[i];

      if ( (node_i > -1) && (geom->REF_S->ive[node_i] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[node_i])) )
      {
            Real BCVal = Bc1D_evalFunction1(BC, BC_1De_DIRICHLET, geom->REF_S->ive[node_i] , AXEe_X , geom->XSOMM->ve[node_i] );

            RHS->ve[i] = BCVal;
      }
      else
      {
         r = A->row + i ;

         for ( idx=0 ; idx<r->len ; idx++ )
         {
            idx2 = r->elt[idx].col ;

            int node_idx2 = geom->SPLINES_DOF_TO_REF_S->ive[idx2];

            if ( node_idx2 >= 0 && (geom->REF_S->ive[node_idx2] > 0) && (BC_1De_DIRICHLET == Bc1D_getBcType(BC, AXEe_X, geom->REF_S->ive[node_idx2])) )
            {
               Real BCVal = Bc1D_evalFunction1(BC, BC_1De_DIRICHLET, geom->REF_S->ive[node_idx2], AXEe_X, geom->XSOMM->ve[node_idx2] ) ;

               RHS->ve[i] -= (r->elt[idx].val) * BCVal ;
            }
         }
      }
   }
}

/* ----------------------------------------------------------------------------------------------------------- */
/* ----------------------------------------------------------------------------------------------------------- */

