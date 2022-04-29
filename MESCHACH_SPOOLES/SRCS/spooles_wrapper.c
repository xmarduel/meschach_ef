
#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper.h"


/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

DV *v_meschach_to_v_spooles(const VEC *v, DV* dv)
{
   int i;
   
   if ( v == NULL ) error(E_NULL,"v_meschach_to_v_spooles");

   if ( dv == NULL )
   {
      dv = DV_new();
      DV_setSize(dv, v->dim);  
   }
   else
   {
      if ( v->dim != dv->size ) error(E_SIZES, "v_meschach_to_v_spooles");
   }

   /* fill */
   for (i=0; i<v->dim; i++)
   {
      DV_setEntry (dv, i, v->ve[i]);
   }

   return dv;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

DenseMtx *v_meschach_to_m_spooles(const VEC *v, DenseMtx *M)
{
   int i;

   if ( v == NULL ) error(E_NULL,"v_meschach_to_m_spooles");

   if ( M == NULL )
   {
      M = DenseMtx_new();
      DenseMtx_init(M, SPOOLES_REAL, 0,0, v->dim,1 , 1,v->dim);
   }
   else
   {
      int m,n;

      DenseMtx_dimensions (M,&m,&n);

      if ( m != v->dim ) error(E_SIZES, "v_meschach_to_m_spooles");
      if ( n != 1      ) error(E_SIZES, "v_meschach_to_m_spooles");
   }


   /* fill */
   for (i=0; i<v->dim; i++)
   {
      DenseMtx_setRealEntry(M,i,0, v->ve[i]);
   }

   return M;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

DenseMtx *m_meschach_to_m_spooles(const MAT *A, DenseMtx *M)
{
   int i,j;

   if ( A == NULL ) error(E_NULL,"m_meschach_to_m_spooles");

   if ( M == NULL )
   {
      M = DenseMtx_new();
      DenseMtx_init(M, SPOOLES_REAL, 0,0, A->m,A->n , 1,A->m);
   }
   else
   {
      int m,n;

      DenseMtx_dimensions (M,&m,&n);

      if ( m != A->m ) error(E_SIZES, "m_meschach_to_m_spooles");
      if ( n != A->n ) error(E_SIZES, "m_meschach_to_m_spooles");
   }

   for (i=0; i<A->m; i++)
   for (j=0; j<A->n; j++)
   {
      DenseMtx_setRealEntry(M,i,j, A->me[i][j]);
   }

   return M;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

InpMtx *s_meschach_to_s_spooles(const SPMAT *A, InpMtx *B)
{
   int i,j;

   int idx;
   Real val;
   SPROW *r;
   
   if ( A == NULL ) error(E_NULL,"s_meschach_to_s_spooles");

   if ( B == NULL )
   {
      B = InpMtx_new();
      InpMtx_init(B, INPMTX_BY_ROWS, SPOOLES_REAL, 0, 0);
   }
   else
   {
      int m,n;

      m = IV_max(&(B->ivec1IV));
      n = IV_max(&(B->ivec2IV));

      if ( m != A->m ) error(E_SIZES, "s_meschach_to_s_spooles");
      if ( n != A->n ) error(E_SIZES, "s_meschach_to_s_spooles");

      /* set to zero */
      InpMtx_clearData(B);
      /* set to zero */
   }

   for (i=0; i<A->m; i++)
   {
      r = A->row + i;

      for (j=0; j<r->len; j++)
      {
         idx = r->elt[j].col ;
         val = r->elt[j].val ;

         InpMtx_inputRealEntry(B,i,idx, val);
      }
   }

   return B;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

InpMtx *s_meschach_to_s_spooles_sym(const SPMAT *A, InpMtx *B)
{
   int i,j;

   int idx;
   Real val;
   SPROW *r;

   if ( A == NULL ) error(E_NULL,"s_meschach_to_s_spooles_sym");

   if ( B == NULL )
   {
      B = InpMtx_new();
      InpMtx_init(B, INPMTX_BY_ROWS, SPOOLES_REAL, 0, 0);
   }
   else
   {
      int m,n;

      m = IV_max(&(B->ivec1IV));
      n = IV_max(&(B->ivec2IV));

      if ( m != A->m ) error(E_SIZES, "s_meschach_to_s_spooles_sym");
      if ( n != A->n ) error(E_SIZES, "s_meschach_to_s_spooles_sym");

      /* set to zero */
      InpMtx_clearData(B);
      /* set to zero */
   }

   for (i=0; i<A->m; i++)
   {
      r = A->row + i;

      for (j=0; j<r->len; j++)
      {
         idx = r->elt[j].col ;
         val = r->elt[j].val ;

         if ( idx> i ) break; /* only the lower part */
         
         InpMtx_inputRealEntry(B,i,idx, val);
      }
   }

   return B;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

VEC *v_spooles_to_v_meschach(const DV *dv, VEC *v)
{
   if ( dv == NULL ) error(E_NULL,"v_spooles_to_v_meschach");
   if ( dv->size <= 0 ) error(E_SIZES,"v_spooles_to_v_meschach");

   if ( v != NULL )
   {
      if ( v->dim != dv->size ) error(E_SIZES, "v_spooles_to_v_meschach");
   }
   else
   {
      v = v_get(dv->size);
   }

   /*
   for (i=0; i<v->dim; i++)
   {
      val = dv->vec[i];

      v_set_val(v,i, val);
   }
   */

   MEM_COPY( &(dv->vec[0]), &(v->ve[0]), v->dim*sizeof(Real) );

   return v;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

VEC *m_spooles_to_v_meschach(const DenseMtx *M, VEC *v)
{
   if ( M == NULL ) error(E_NULL,"m_spooles_to_v_meschach");
   if ( v == NULL ) error(E_NULL,"m_spooles_to_v_meschach");

   /* retrieve v from M */
   /*
   for (i=0; i<DV_size(x); i++)
   {
      v->ve[i] = M->entries[i]);
   }
   */
   
   MEM_COPY( M->entries, v->ve, v->dim*sizeof(Real) );

   return v;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

MAT *m_spooles_to_m_meschach(const DenseMtx *A, MAT *M)
{
   int m,n;
   int i,j;
   Real val;

   if ( A == NULL ) error(E_NULL,"m_spooles_to_m_meschach");

   DenseMtx_dimensions (A,&m,&n);

   if ( M != NULL )
   {
      if ( m != M->m ) error(E_SIZES, "m_spooles_to_m_meschach");
      if ( n != M->n ) error(E_SIZES, "m_spooles_to_m_meschach");
   }
   else
   {
      M = m_get(m,n);
   }
   
   for (i=0; i<M->m; i++)
   for (j=0; j<M->n; j++)
   {
      val = A->entries[i*A->inc1 + j*A->inc2];

      m_set_val(M,i,j, val);
   }

   return M;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

SPMAT *s_spooles_to_s_meschach(const InpMtx *A, SPMAT *S)
{
   int m,n,idx;
   int    *ivec1;
   int    *ivec2;
   double *dvec ;

   if ( A == NULL ) error(E_NULL,"s_spooles_to_s_meschach");

   m = IV_max(&(A->ivec1IV));
   n = IV_max(&(A->ivec2IV));

   if ( S != NULL )
   {
      if ( (m+1) != S->m ) error(E_SIZES, "s_spooles_to_s_meschach");
      if ( (n+1) != S->n ) error(E_SIZES, "s_spooles_to_s_meschach");

      sp_zero(S);
   }
   else
   {
      S = sp_get(m+1,n+1,2);
   }


   ivec1 = InpMtx_ivec1(A) ;
   ivec2 = InpMtx_ivec2(A) ;

   dvec  = InpMtx_dvec(A)  ;

   if ( A->nent != IV_size(&(A->ivec1IV)) ) error(E_SIZES,"s_spooles_to_s_meschach");
   if ( A->nent != IV_size(&(A->ivec2IV)) ) error(E_SIZES,"s_spooles_to_s_meschach");
   if ( A->nent != DV_size(&(A->dvecDV))  ) error(E_SIZES,"s_spooles_to_s_meschach");

   for ( idx = 0 ; idx < A->nent ; idx++ )
   {
      sp_set_val(S, ivec1[idx], ivec2[idx], dvec[idx]);
   }

   return S;
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

