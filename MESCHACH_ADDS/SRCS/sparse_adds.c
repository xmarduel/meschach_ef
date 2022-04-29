
/** \file sparse_adds.c
 *
 * extentions of MESCHACH LIBRARY for usual structs 
 *
 */

#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse2.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"

#include "MESCHACH_ADDS/INCLUDES/spmat2formats.h"


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* sp_add_val -- adds the (i,j) entry of the sparse matrix A */
Real  sp_add_val(SPMAT *A, int i, int j, Real val)
{ 
   SPROW        *r;
   int  idx, idx2, new_len;
   
   if ( A == SMNULL )      error(E_NULL,"sp_add_val");
   if ( i < 0 || i >= A->m || j < 0 || j >= A->n )    error(E_SIZES,"sp_add_val");
   
   r = A->row+i;
   idx = sprow_idx(r,j);
   /* printf("sp_add_val: idx = %d\n",idx); */
   if ( idx >= 0 )
   {
      r->elt[idx].val += val; return val;
   }
   /* else */
   if ( idx < -1 )
   {
      /* Note: this destroys the column & diag access paths */
      A->flag_col = A->flag_diag = FALSE;
      /* shift & insert new value */
      idx = -(idx+2);   /* this is the intended insertion index */
      if ( r->len >= r->maxlen )
      {
         r->len = r->maxlen;
         new_len = max(2*r->maxlen+1,5);
         if (mem_info_is_on())
         {
            mem_bytes(TYPE_SPMAT,A->row[i].maxlen*sizeof(row_elt),
               new_len*sizeof(row_elt));
         }
         
         r->elt = RENEW(r->elt,new_len,row_elt);
         if ( ! r->elt )        /* can't allocate */
            error(E_MEM,"sp_add_val");
         r->maxlen = 2*r->maxlen+1;
      }
      for ( idx2 = r->len-1; idx2 >= idx; idx2-- )
         MEM_COPY((char *)(&(r->elt[idx2])),
         (char *)(&(r->elt[idx2+1])),sizeof(row_elt));
         /************************************************************
         if ( idx < r->len )
         MEM_COPY((char *)(&(r->elt[idx])),(char *)(&(r->elt[idx+1])),
         (r->len-idx)*sizeof(row_elt));
      ************************************************************/
      r->len++;
      r->elt[idx].col = j;
      return r->elt[idx].val = val;
   }
   /* else -- idx == -1, error in index/matrix! */
   return 0.0;
} 

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC *spLDLsolve(const SPMAT *L, const VEC *b, VEC *out)
{
   int	i, j_idx, n, scan_idx, scan_row;
   SPROW	*row;
   row_elt	*elt;
   Real	diag_val, sum, *out_ve;
   
   if ( L == SMNULL || b == VNULL )     error(E_NULL  ,"spLDLsolve");
   if ( L->m != L->n )                  error(E_SQUARE,"spLDLsolve");
   if ( b->dim != L->m )                error(E_SIZES,"spLDLsolve");

   if ( ! L->flag_col  )    sp_col_access((SPMAT*)L);
   if ( ! L->flag_diag )    sp_diag_access((SPMAT*)L);
   
   
   out = v_copy(b, out);
   out_ve = out->ve;
   
   
   /* forward substitution: solve LDL.x=b for x */
   n = L->n;
   for ( i = 0; i < n; i++ )
   {
      sum = out_ve[i];
      row = &(L->row[i]);
      elt = row->elt;
      for ( j_idx = 0; j_idx < row->len; j_idx++, elt++ )
      {
         if ( elt->col >= i )
            break;
         sum -= elt->val*out_ve[elt->col];
      }
      if ( row->diag >= 0 )
      {
         out_ve[i] = sum;   /* new from Xav : */
         /*LDL^T : L unit inferior */
         /* out_ve[i] = sum/(row->elt[row->diag].val);*/
         /* This as to be skipped  */
      }
      else
      {
         error(E_SING,"spLDLsolve forwards");
      }
   }
   
   /* diagonal substitution */
   for ( i=0; i<n; i++ )
      if ( sp_get_val(L,i,i) == 0.0 )
         error(E_SING,"LDLsolve diagonal");
      else
         out_ve[i] = out_ve[i]/sp_get_val(L,i,i); 
   
   /* backward substitution: solve L^T.out = x for out */
   for ( i = n-1; i >= 0; i-- )
   {
      sum = out_ve[i];
      row = &(L->row[i]);
      /* Note that row->diag >= 0 by above loop */
      elt = &(row->elt[row->diag]);
      diag_val = elt->val;
      /* scan down column */
      scan_idx = elt->nxt_idx;
      scan_row = elt->nxt_row;
      while ( scan_row >= 0 /* && scan_idx >= 0 */ )
      {
         row = &(L->row[scan_row]);
         elt = &(row->elt[scan_idx]);
         sum -= elt->val*out_ve[scan_row];
         scan_idx = elt->nxt_idx;
         scan_row = elt->nxt_row;
      }
      out_ve[i] = sum;                /* new from Xav : */
      /* LDL^T : L unit inferior */
      /* out_ve[i] = sum/diag_val;*/  /* This as to be skipped   */
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *dense2sp(const MAT *A, SPMAT *out)
{
   int i,j;
   
   if ( A == MNULL || out == SMNULL )              error(E_NULL ,"dense2sp");
   if ( ( A->m != out->m )||( A->n != out->n ) )   error(E_SIZES,"dense2sp");
   
   for (i=0; i<A->m; i++)
   for (j=0; j<A->n; j++)
   {
      if ( fabs(A->me[i][j]) > MACHEPS )
      {
         sp_set_val(out,i,j, A->me[i][j]);
      }
   }
   
   sp_compact(out,MACHEPS);
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_insert_submat(SPMAT *A, int i0, int j0, const MAT *B)
{
   int i,j;

   /* We insert B in A at indices (i0,j0) of A */
   if ( A==(SPMAT *)NULL )      error(E_NULL  ,"sp_insert_submat");
   if ( B==(MAT *)NULL   )      error(E_NULL  ,"sp_insert_submat");
   if ( A->m < (B->m + i0) )    error(E_BOUNDS,"sp_insert_submat");
   if ( A->n < (B->n + j0) )    error(E_BOUNDS,"sp_insert_submat");
   if ( (i0 < 0)||(j0 < 0) )    error(E_BOUNDS,"sp_insert_submat");

   /* only set B in the sub-block of A */
   for (i=0; i<B->m; i++)
   for (j=0; j<B->n; j++)
   {
      sp_set_val(A, i+i0,j+j0, B->me[i][j]);
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *m_get_subsp( MAT *A , int k0, int k1, const SPMAT *B )
{
   int i,j;
   int size = k1-k0;

   /* check args ? */
   if ( A == NULL ) error(E_NULL,"m_get_subsp");
   if ( B == NULL ) error(E_NULL,"m_get_subsp");
   /* check size */
   if ( size <= 0 )   error(E_NEG  ,"m_get_subsp");
   if ( k1   > B->m ) error(E_SIZES,"m_get_subsp");
   if ( k1   > B->n ) error(E_SIZES,"m_get_subsp");
   if ( size > A->m ) error(E_SIZES,"m_get_subsp");
   if ( size > A->n ) error(E_SIZES,"m_get_subsp");

   /* store the values of the matrix for the first and last P rows and columns */
   for (i=0; i<size; i++)
   for (j=0; j<size; j++)
   {
      A->me[i][j] = sp_get_val(B, i+k0, j+k0);  /* rows and columns */
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_transpose_rows(SPMAT *A, int k0, int k1)
{
   int i,j;

   int col;
   Real val;

   SPROW * r;

   /* check args ? */
   if ( A   == NULL ) error(E_NULL,"sp_transpose_rows");
   /* check size */
   if ( k0 > k1   )   error(E_NEG  ,"sp_transpose_rows");
   if ( k1 > A->m )   error(E_SIZES,"sp_transpose_rows");
   if ( k1 > A->n )   error(E_SIZES,"sp_transpose_rows");

   /* read value in the rows, set them in the columns */
   for (i=k0; i<k1; i++)
   {
      r = A->row + i ; /* the i-th row */

      for (j=0; j<r->len; j++)
      {
         val = r->elt[j].val;
         col = r->elt[j].col;

         sp_set_val(A,col,i, val);
      }
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_transp(const SPMAT *in, SPMAT *out)
{
   int	i, idx;
   int	in_situ;
   SPROW   *r;
   SPMAT   *TMP;

   if ( in == (SPMAT *)NULL )           error(E_NULL   ,"sp_transp");
   if ( in == out && in->n != in->m )   error(E_INSITU2,"sp_transp");

   in_situ = ( in == out );

   if ( out == (SPMAT *)NULL )
   {
      out = sp_get(in->n,in->m,2); /* 1-> bug ! */
      sp_zero(out);
   }
   else
   {
      if (!in_situ) sp_zero(out);
   }
   if (  (in->n != out->m)||(in->m != out->n) )   error(E_SIZES,"sp_transp");

   
   if ( in != out )  /* not in-situ */
   {
      for ( i = 0; i < in->m; i++ )
      {
         r = in->row +i ;
         for (idx=0; idx<r->len; idx++ )
         {
            sp_set_val( out, r->elt[idx].col,i, r->elt[idx].val );
         }
      }
   }
   else
   {
      TMP = sp_get(in->n, in->m, 2);
      for ( i = 0; i < in->m; i++ )
      {
         r = in->row +i ;
         for (idx=0; idx<r->len; idx++ )
         {
            sp_set_val(TMP, r->elt[idx].col,i, r->elt[idx].val);
         }
      }
      out = sp_copy2(TMP,out);
      SP_FREE(TMP);
   }

   return sp_compact(out,MACHEPS);   
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_m_mlt(const SPMAT *A, const SPMAT *B, SPMAT *C)     /* C = A.B   */
{
   u_int	i, j, idx,col;
   SPROW  *r;
   Real sum;
   
   if ( A==(SPMAT *)NULL || B==(SPMAT *)NULL )      error(E_NULL  ,"sp_m_mlt");
   if ( A->n != B->m )                              error(E_SIZES ,"sp_m_mlt");
   if ( A == C || B == C )                          error(E_INSITU,"sp_m_mlt");
   
   
   if ( C==(SPMAT *)NULL )
   {
      C = sp_get(A->m,B->n,5);
   }
   else
   {
      if ( (A->m != C->m)||(B->n != C->n) ) error(E_SIZES,"sp_m_mlt");
   }

   /* reset C */
   sp_zero(C);


   for ( i=0; i<A->m; i++ )
   {
      r = A->row +i;
      for ( j=0; j<B->n; j++ )
      {
         sum = 0.0;
         for ( idx=0; idx<r->len; idx++ )
         {
            col = r->elt[idx].col;
            sum += sp_get_val(A,i,col)*sp_get_val(B,col,j);
         }
         if ( fabs(sum) > MACHEPS ) sp_set_val(C,i,j,sum);
      }
   }
   
   sp_compact(C,MACHEPS);
   
   return C;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_mmtr_mlt(const SPMAT *A, const SPMAT *B, SPMAT *C)  /* C = A.B^T */
{
   u_int   i, j;
   SPROW  *r1,*r2;
   Real    sum;
   
   if ( A==(SPMAT *)NULL || B==(SPMAT *)NULL )      error(E_NULL  ,"sp_mmtr_mlt");
   if ( A->m != B->m )                              error(E_SIZES ,"sp_mmtr_mlt");
   if ( A == C || B == C )                          error(E_INSITU,"sp_mmtr_mlt");
   
   
   if ( C==(SPMAT *)NULL )
   {
      C = sp_get(A->m,B->n,5);
   }
   else
   {
      if ( (A->m != C->m)||(B->n != C->n) ) error(E_SIZES,"sp_mmtr_mlt");
      
      sp_zero(C);
   }
   
   /* reset C */
   sp_zero(C);
   
   for ( i=0; i<A->m; i++ )
   {
      r1 = A->row +i;
      for ( j=0; j<B->m; j++ )
      {
         r2 = B->row + j;
         sum = sprow_ip(r1,r2,A->m);
         
         sp_set_val(C,i,j,sum);
      }
   }
   
   sp_compact(C,MACHEPS);
   
   return C;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_mtrm_mlt(const SPMAT *A, const SPMAT *B, SPMAT *C)  /* C = A^T.B */
{
   SPMAT *AT;   /* AT = A^T */
   
   if ( A==(SPMAT *)NULL || B==(SPMAT *)NULL )      error(E_NULL  ,"sp_mtrm_mlt");
   if ( A->n != B->n )                              error(E_SIZES ,"sp_mtrm_mlt");
   if ( A == C || B == C )                          error(E_INSITU,"sp_mtrm_mlt");
   
   AT = sp_transp(A, NULL);
   
   sp_m_mlt(AT,B,C);      /* C = A^T.B */
   
   SP_FREE(AT);
   
   return C;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_zero_subsp(SPMAT *A, int i0, int j0, int m, int n)
{
   int   i,idx;
   SPROW *r;

   /* We set the sub-block of A at point(i0;j0) of size (m;n) to zero */
   if ( A==(SPMAT *)NULL )                  error(E_NULL  ,"sp_zero_subsp");
   if ( (i0 < 0) || (A->m < (i0 + m)) )     error(E_BOUNDS,"sp_zero_subsp");
   if ( (j0 < 0) || (A->n < (j0 + n)) )     error(E_BOUNDS,"sp_zero_subsp");

   /* set the sub-block of A to zero */
   for (i=i0; i<(i0+m); i++)
   {
      r = A->row + i;

      /* reach the block first column */
      for (idx=0; idx<r->len; idx++)
      {
         if (r->elt[idx].col < j0 )
            continue;
         else
            break;
      }

      /* set zero in the sub-block columns range */
      for ( ; idx<r->len; idx++)
      {
         if (r->elt[idx].col < (j0+n) )
         {
            r->elt[idx].val = 0.0;
         }
         else
         {
            break;
         }
      }
   }

   /* finito */
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_insert_subsp(SPMAT *A, int i0, int j0, const SPMAT *B)
{
   /* We insert B in A at indices (i0,j0) of A */
   
   if ( A==(SPMAT *)NULL )      error(E_NULL  ,"sp_insert_subsp");
   if ( B==(SPMAT *)NULL )      error(E_NULL  ,"sp_insert_subsp");
   if ( A->m < (B->m + i0) )    error(E_BOUNDS,"sp_insert_subsp");
   if ( A->n < (B->n + j0) )    error(E_BOUNDS,"sp_insert_subsp");
   if ( (i0 < 0)||(j0 < 0) )    error(E_BOUNDS,"sp_insert_subsp");

   /* set the sub-block of A to zero */
   A = sp_zero_subsp(A, i0, j0, B->m, B->n);

   /* now only add B in the sub-block of A */
   return sp_add_subsp(A, i0, j0, B);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_add_subsp(SPMAT *A, int i0, int j0, const SPMAT *B)
{
   int   i,idx;
   SPROW *r;
   /* We add B in A at indices (i0,j0) of A */

   if ( A==(SPMAT *)NULL )      error(E_NULL  ,"sp_add_subsp");
   if ( B==(SPMAT *)NULL )      error(E_NULL  ,"sp_add_subsp");
   if ( A->m < (B->m + i0) )    error(E_BOUNDS,"sp_add_subsp");
   if ( A->n < (B->n + j0) )    error(E_BOUNDS,"sp_add_subsp");
   if ( (i0 < 0)||(j0 < 0) )    error(E_BOUNDS,"sp_add_subsp");
   
   for (i=0; i<B->m; i++)
   {
      r = B->row + i;
      for (idx=0; idx<r->len; idx++)
      {
         sp_add_val(A, i0+i, j0+r->elt[idx].col, r->elt[idx].val );
      }
   }
   
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_insert_subsp_block(SPMAT *A, int p, int q, const SPMAT *B)
{
   /* A(K*MX,K*NX) is a block matrix and B must be B(MX,NX) */
   /* We insert B in A at block (p,q) of the submatrices of A (each block is of size of B) */

   if ( A==(SPMAT *)NULL )           error(E_NULL  ,"sp_insert_subsp_block");
   if ( B==(SPMAT *)NULL )           error(E_NULL  ,"sp_insert_subsp_block");
   if ( A->m < (p+1)*B->m )          error(E_BOUNDS,"sp_insert_subsp_block");
   if ( A->n < (q+1)*B->n )          error(E_BOUNDS,"sp_insert_subsp_block");
   if ( ( p < 0 ) || ( q < 0 ) )     error(E_BOUNDS,"sp_insert_subsp_block");

   return sp_insert_subsp(A, p*B->m, q*B->n, B);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_add_subsp_block(SPMAT *A, int p, int q, const SPMAT *B)
{
   /* A(K*MX,K*NX) is a block matrix and B must be B(MX,NX) */
   /* We add B in A at block (p,q) of the submatrices of A (each block is of size of B) */

   if ( A==(SPMAT *)NULL || B==(SPMAT *)NULL )     error(E_NULL  ,"sp_add_subsp_block");
   if ( A->m < (p+1)*B->m )                        error(E_BOUNDS,"sp_add_subsp_block");
   if ( A->n < (q+1)*B->n )                        error(E_BOUNDS,"sp_add_subsp_block");
   if ( ( p < 0 ) || ( q < 0 ) )                   error(E_BOUNDS,"sp_add_subsp_block");

   return sp_add_subsp(A, p*B->m, q*B->n, B);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_move(const SPMAT *A, int i0, int j0, int m0 , int n0 , SPMAT *B, int i1, int j1)
{
   /* We insert in B at indices (i1,j1) the submatrix of A which starts at (i0,j0) of dim (m0,n0) (cf m_move) */
   /* It is more general than sp_insert_subsp */
   int   i,idx;
   SPROW *r;
   
   if ( A==(SPMAT *)NULL || B==(SPMAT *)NULL )      error(E_NULL  ,"sp_move");
   if ( ( m0 + i0 > A->m )||( n0 + j0 > A->n ) )    error(E_BOUNDS,"sp_move");
   if ( ( i0 < 0 )||( j0 < 0 ) )                    error(E_BOUNDS,"sp_move");
   if ( ( m0 + i1 > B->m )||( n0 + j1 > B->n ) )    error(E_BOUNDS,"sp_move");
   if ( ( i1 < 0 )||( j1 < 0 ) )                    error(E_BOUNDS,"sp_move");   
   
   /* first set the sub-matrix of B to zero */
   sp_zero_subsp(B, i1, j1, m0, n0);

   /* then insert the part of A in it */
   for (i=i0; i<i0+m0; i++)
   {
      r =  A->row + i;

      /* reach the column n0 of A */
      for (idx=0; idx<r->len; idx++)
      {
         if (r->elt[idx].col < j0 )
            continue;
         else
            break;
      }

      /* continue to loop  in the row ... */
      for (    ; idx<r->len; idx++)
      {
         if ( r->elt[idx].col < (n0+j0) )
         {
            sp_set_val(B, i-i0+i1, r->elt[idx].col -j0+j1, r->elt[idx].val );
         }
         else /* we are here with column index bigger than n0+j0 : quit for this row */
         {
            break;
         }
      }
   }
   
   return B;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT *sp_madd(const SPMAT *A, int i0, int j0, int m0 , int n0 , SPMAT *B, int i1, int j1)
{
   int   i,idx;
   SPROW *r;
   /* We add in B at indices (i1,j1) the submatrix of A which starts at (i0,j0) of dim (m0,n0) */
   /* It is more general than sp_add_subsp */

   if ( A==(SPMAT *)NULL || B==(SPMAT *)NULL )      error(E_NULL  ,"sp_madd");
   if ( ( m0 + i0 > A->m )||( n0 + j0 > A->n ) )    error(E_BOUNDS,"sp_madd");
   if ( ( i0 < 0 )||( j0 < 0 ) )                    error(E_BOUNDS,"sp_madd");
   if ( ( m0 + i1 > B->m )||( n0 + j1 > B->n ) )    error(E_BOUNDS,"sp_madd");
   if ( ( i1 < 0 )||( j1 < 0 ) )                    error(E_BOUNDS,"sp_madd");
   
   
   for (i=i0; i<i0+m0; i++)
   {
      r =  A->row + i;

      /* reach the column n0 of A */
      for (idx=0; idx<r->len; idx++)
      {
         if (r->elt[idx].col < j0 )
            continue;
         else
            break;
      }

      /* continue to loop  in the row ... */
      for (    ; idx<r->len; idx++)
      {
         if ( r->elt[idx].col < (n0+j0) )
         {
            sp_add_val(B, i-i0+i1, r->elt[idx].col -j0+j1, r->elt[idx].val );
         }
         else /* we are here with column index bigger than n0+j0 : quit for this row */
         {
            break;
         }
      }
   }
   
   return B;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* px_sp_cols -- perform : out = A * M(P) i.e. permute columns of matrix A with Pt
	-- May NOT be in situ */
SPMAT *px_sp_cols(const PERM *P, const SPMAT *A, SPMAT* out)
{
   SPMAT *At, *Ot;
   PERM  *Pt;
   
   if ( ! A || ! P )        error(E_NULL  ,"px_sp_cols");
   if ( P->size != A->n )   error(E_SIZES ,"px_sp_cols");
   if ( A == out )          error(E_INSITU,"px_sp_cols");

   if ( out != NULL )
   {
      if ( A->m != out->m )    error(E_SIZES ,"px_sp_cols");
      if ( A->n != out->n )    error(E_SIZES ,"px_sp_cols");
   }
   else
   {
      out = sp_get(A->m,A->n,2);      
   }

   Pt = px_get(P->size);
   Pt = px_inv(P, Pt);

   At = sp_transp(A, NULL);
   
   Ot = px_sp_rows(Pt, At, NULL);

   out = sp_transp(Ot, out);
   
   /* free memory */
   SP_FREE(Ot);
   SP_FREE(At);

   PX_FREE(Pt);
   
   /* finito */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* px_sp_rows -- performs : out = A * (P) i.e. permute rows of matrix A with P
	-- May NOT be in situ */
SPMAT *px_sp_rows(const PERM *P, const SPMAT *A, SPMAT* out)
{
   int i;
   
   if ( ! A || ! P )        error(E_NULL  ,"px_sp_rows");
   if ( P->size != A->m )   error(E_SIZES ,"px_sp_rows");
   if ( A == out )          error(E_INSITU,"px_sp_rows");

   if ( out != NULL )
   {
      if ( A->m != out->m ) error(E_SIZES ,"px_sp_rows");
      if ( A->n != out->n ) error(E_SIZES ,"px_sp_rows");
   }
   else
   {
      out = sp_get(A->m,A->n,2);
   }
   
   for (i=0; i<A->m; i++)
   {
      sprow_copy2(&(A->row[P->pe[i]]),&(out->row[i]), TYPE_SPMAT);
   }
   
   /* end */
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT* sp_ident(SPMAT* A)
{
   int i;

   if ( A == (SPMAT*)NULL ) error(E_NULL,  "sp_ident");
   if ( A->m != A->n )      error(E_SQUARE,"sp_ident");

   /* init */
   sp_zero(A);
   sp_compact(A, 0.0);

   for (i=0; i<A->m; i++)
   {
      sp_set_val(A, i,i, 1.0);
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT* sp_eye(SPMAT* A)
{
   int i,size;

   if ( A == (SPMAT*)NULL ) error(E_NULL,  "sp_eye");

   /* init */
   sp_zero(A);
   sp_compact(A, 0.0);

   size = ( A->m > A->n ) ? A->m : A->n ;
   
   for (i=0; i<size; i++)
   {
      sp_set_val(A, i,i, 1.0);
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT* sp_tridiag(SPMAT* A, real lower_val, real diag_val, real upper_val)
{
   int i;

   if ( A == (SPMAT*)NULL ) error(E_NULL,  "sp_tri");
   if ( A->m != A->n )      error(E_SQUARE,"sp_tri");

   /* init */
   sp_zero(A);
   sp_compact(A, 0.0);

   for (i=0; i<A->m-1; i++)
   {
      sp_set_val(A, i+1,i, lower_val);
      sp_set_val(A, i  ,i, diag_val );
      sp_set_val(A, i  ,i+1, upper_val);
   }
   i = A->m - 1;
   sp_set_val(A, i, i, diag_val );

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPMAT* sp_ones(SPMAT* A)
{
   int i,j;

   if ( A == (SPMAT*)NULL ) error(E_NULL,  "sp_ones");

   /* init */
   sp_zero(A);
   sp_compact(A, 0.0);

   for (i=0; i<A->m; i++)
   for (j=0; j<A->n; j++)
   {
      sp_set_val(A, i,j, 1.0);
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */


/************************************************************************/
/*----------------------------------------------------------------------*/
/*                      *** ILUT preconditioner ***                     */
/*							                */
/*   Yousef Saad's incomplete LU factorization with dual truncation     */
/*   mechanism (numerical and positional dropping).  This is modified	*/
/*   only in order to use a different calling sequence, and users    	*/
/*   should refer to the SPARSKIT documentation for more information on	*/
/*   the routine.						        */
/*								        */
/*----------------------------------------------------------------------*/

void static qsplit( Real *a, int *ind, int n, int ncut);


SPMAT *spILUTfactor(SPMAT *A, SPMAT *ILUT, int lfil, Real tol)
{
   int n;
   int nnz;
   
   SPROW *r;
   
   /* Structures for CRS storage of A */
   VEC  *avalues;
   IVEC *col_ind;
   IVEC *row_ptr;
   
   /* 2 vectors to store ILUT in Modified Sparse Row format */
   VEC  *alu;
   IVEC *jlu; 
   IVEC *ju;
   
   int iwk;
   
   /* working vectors */
   VEC  *wu, *wl;
   IVEC *jr, *jwu, *jwl;
   
   
   int k,j,j1,j2,jj,i,ii,ju0,jpos,len,jrow,nl,lenu0,lenu,lenl0,lenl;
   Real t,tnorm,s,fact;
   
   /* some tests ----------------------------------------------- */
   if ( A==(SPMAT *)NULL || ILUT==(SPMAT *)NULL )
      error(E_NULL,"spILUTfactor");
   
   if ( A == ILUT )  error(E_INSITU,"spILUTfactor");
   
   if (lfil < 0) error(E_NEG,"spILUTfactor");
   /* ---------------------------------------------------------- */
   
   n   = A->m;
   nnz = sp_nonzeros(A);
   
   avalues =  v_get(nnz);
   col_ind = iv_get(nnz);
   row_ptr = iv_get(A->m +1);
   
   /* transform SPMAT *A format into Compressed Row Storage */
   spmat2crs(A , avalues , col_ind , row_ptr , 'F');
   
   
   /* for the size of the ILUT matrix, we choose a quite large dimension */
   /* namely the one of a total fill-in as for LU factorisation of A     */
   iwk = 0;
   for (i=0; i<A->m; i++)
   {
      r = A->row + i ;
      iwk += ( r->elt[r->len -1].col - r->elt[0].col +1);
   }
   iwk *= 1 /*2*/ ; /* well, it is possible it is a little bit less */
   /* 
    * printf("in spILUTfactor, we allocate iwk = %d at the beginning \n",iwk);
    */
   
   printf("allocate alu with %d reals ", iwk);
   alu  =  v_get(iwk); /* to be perhaps resized */ printf(" ... done \n");
   printf("allocate jlu with %d ints  ", iwk);
   jlu  = iv_get(iwk); /* to be perhaps resized */ printf(" ... done \n");
   ju   = iv_get(A->m);      /* DIM OK !        */
   
   
   /* a few working vectors */
   wu = v_get(A->m +1);
   wl = v_get(A->m   );
   
   jr  = iv_get(A->m);
   jwu = iv_get(A->m);
   jwl = iv_get(A->m);
   
   
   
   /* -------------------- JETZT GEHT-S LOS ! ------------------------ */
   
   /* ---------------------------------------------------------------- */
   /* initialize ju0 (points to next element to be added to alu,jlu)   */
   /* and pointer.                                                     */
   
   ju0 = n+2;
   jlu->ive[0] = ju0;
   
   /* integer Real pointer array. */
   
   iv_zero(jr);
   
   /*--------------------------------------------------------------------*/
   /* beginning of main loop.                                            */
   /*--------------------------------------------------------------------*/
   for (ii=1; ii<=n; ii++)
   {
      j1 = row_ptr->ive[ii-1];        /* row_ptr in FORTRAN notation */
      j2 = row_ptr->ive[ii  ] - 1 ;
      tnorm = 0.0;
      
      for (k=j1-1; k<j2; k++)
      {
         tnorm = tnorm+fabs(avalues->ve[k]);
      }
      if (tnorm == 0) error(E_SING,"spILUTfactor");
      
      tnorm = tnorm/(Real)(j2-j1+1);
      
      /*--- unpack L-part and U-part of row of A in arrays wl, wu -- */
      
      lenu =  1;
      lenl =  0;
      jwu->ive[0] = ii;
      wu->ve[0] = 0.0;
      jr->ive[ii-1] = 1;
      
      for (j=j1-1; j<j2; j++)
      {
         k = col_ind->ive[j];  /* col_ind in FORTRAN notation */
         t = avalues->ve[j];
         if ( (fabs(t) < tol*tnorm) && ( k != ii) ) continue;
         
         if (k < ii)
         {
            lenl++;
            jwl->ive[lenl-1] = k;
            wl->ve[lenl-1] = t;
            jr->ive[k-1] = lenl;
         }
         else if (k == ii)
         {
            wu->ve[0] = t;
         }
         else
         {
            lenu++ ;
            jwu->ive[lenu-1] = k;
            wu->ve[lenu-1] = t;
            jr->ive[k-1] = lenu;
         }
      }
      
      tnorm = tnorm/(Real)(j2-j1+1);
      lenl0 = lenl;
      lenu0 = lenu;
      jj = 0;
      nl = 0;
      
      /*-------------------------------------------------------------------*/
      /*---------------------- eliminate previous rows --------------------*/
      /*-------------------------------------------------------------------*/
etiq1:
      jj++ ;
      if (jj > lenl) goto etiq2;
      
      /*-------------------------------------------------------------------*/
      /*in order to do the elimination in the correct order we need to     */
      /*exchange the current row number with the one that has              */
      /*smallest column number, among jj,jj+1,...,lenl.                    */
      /*-------------------------------------------------------------------*/
      jrow = jwl->ive[jj-1];
      k = jj ;
      
      /* determine smallest column index */
      
      for (j=jj; j<lenl; j++)
      {
         if (jwl->ive[j] < jrow)
         {
            jrow = jwl->ive[j] ;
            k = j+1 ;
         }
      }
      
      /* exchange in jwl */
      
      if (k != jj)
      {
         j = jwl->ive[jj-1];
         jwl->ive[jj-1] = jwl->ive[k-1];
         jwl->ive[k-1] = j;
         
         /* exchange in jr */
         
         jr->ive[jrow-1] = jj;
         jr->ive[j-1] = k;
         
         /* exchange in wl */
         
         s = wl->ve[jj-1];
         wl->ve[jj-1] = wl->ve[k-1];
         wl->ve[k-1] = s;
      }
      
      if (jrow >= ii) goto etiq2;
      
      /*--------get the multiplier for row to be eliminated: jrow */
      fact = wl->ve[jj-1]*alu->ve[jrow-1];
      /* zero out element in row by setting jr->ive[jrow) = 0     */
      jr->ive[jrow-1] = 0 ;
      if (fabs(fact)*wu->ve[n+2-jrow-1] <= tol*tnorm) goto etiq1;
      
      /*--------------------------------------------------------*/
      /*------------ combine current row and row jrow ----------*/
      /*--------------------------------------------------------*/
      for (k=ju->ive[jrow-1]; k<=jlu->ive[jrow+1-1]-1; k++)
      {
         s = fact*alu->ve[k-1];
         j = jlu->ive[k-1];
         jpos = jr->ive[j-1];
         
         /* if fill-in element is small then disregard: */
         
         if ( (fabs(s) < tol*tnorm)&&(jpos==0) ) continue;
         
         if (j >= ii)
         {
            /* dealing with upper part. */
            if (jpos==0)
            {
               /* this is a fill-in element */
               lenu++ ;
               if (lenu > n) error(E_SING,"spILUTfactor");
               jwu->ive[lenu-1] = j ;
               jr->ive[j-1] = lenu ;
               wu->ve[lenu-1] = - s ;
            }
            else
            {
               /* no fill-in element */
               wu->ve[jpos-1] -= s ;
            }
         }
         else
         {
            /* dealing with lower part. */
            if (jpos==0)
            {
               /* this is a fill-in element */
               lenl++ ;
               if (lenl>n) error(E_SING,"spILUTfactor");
               jwl->ive[lenl-1] = j;
               jr->ive[j-1] = lenl;
               wl->ve[lenl-1] = - s;
            }
            else
            {
               /* no fill-in element */
               wl->ve[jpos-1] -= s ;
            }
         }
      }
      
      nl++ ;
      wl->ve[nl-1] = fact ;
      jwl->ive[nl-1]  = jrow ;
      
      goto etiq1 ;
      
      /*-------------------------------------------------------*/
      /*------------ update l-matrix --------------------------*/
      /*-------------------------------------------------------*/
etiq2 : len = min(nl,lenl0+lfil);
        
        qsplit(&(wl->ve[0]), &(jwl->ive[0]), nl, len);
        
        for (k=0; k<len; k++)
        {
           if (ju0 > iwk)
           {
              iwk *= 2;
              printf("resize alu with %d reals ", iwk);
              v_resize(alu,iwk);  printf(" ...done \n ");
              printf("resize jlu with %d ints  ", iwk);
              iv_resize(jlu,iwk); printf(" ...done \n ");
           }
           alu->ve[ju0-1]  =  wl->ve[k]   ;
           jlu->ive[ju0-1] =  jwl->ive[k] ;
           ju0++ ;
        }
        
        /* save pointer to beginning of row ii of U */
        
        ju->ive[ii-1] = ju0 ;
        
        /* reset double-pointer jr to zero (L-part - except first */
        /* jj-1 elements which have already been reset)           */
        for (k=jj-1; k<lenl; k++)
        {
           jr->ive[jwl->ive[k]-1] = 0 ;
        }
        len = min(lenu,lenu0+lfil+1); /* +1 added 11/23/94 */
        /* len = min0(lenu,lfil) */
        qsplit (&(wu->ve[1]), &(jwu->ive[1]), lenu-1, len);
        
        /*----------------------------------------------------------*/
        /*------------ update u-matrix -----------------------------*/
        /*----------------------------------------------------------*/
        t = fabs(wu->ve[0]);
        if (len + ju0 > iwk)
        {
           iwk *= 2;
           v_resize(alu,iwk);
           iv_resize(jlu,iwk);
        }
        for (k=1; k<len; k++)
        {
           jlu->ive[ju0-1] = jwu->ive[k];
           alu->ve[ju0-1]  = wu->ve[k];
           t +=  fabs(wu->ve[k]);
           ju0++ ;
        }
        
        /* save norm in wu (backwards). Norm is in fact average abs value */
        
        wu->ve[n+1-ii] = t / (Real)(len+1);
        
        /* store inverse of diagonal element of u */
        
        if (wu->ve[0] == 0.0) wu->ve[0] = (0.0001 + tol)*tnorm;
        
        alu->ve[ii-1] = 1.0/wu->ve[0];
        
        /* update pointer to beginning of next row of U. */
        
        jlu->ive[ii] = ju0;
        
        /* reset double-pointer jr to zero (U-part) */
        
        for (k=0; k<lenu; k++)  jr->ive[jwu->ive[k]-1] = 0;
        
        /*---------------------------------------------------------------*/
        /*    end main loop                                              */
        /*---------------------------------------------------------------*/
        
  }
  
  
  
  iwk = jlu->ive[A->m] -1;
  
  v_resize(alu,iwk);
  iv_resize(jlu,iwk);
  
  
  /* we obtain B in the Modified Row Storage */
  
  sp_zero(ILUT); sp_compact(ILUT, 0.0);
  
  mrs2spmat(ILUT , alu , jlu );
  
  
  /* free memory */
  V_FREE(avalues);
  IV_FREE(col_ind);
  IV_FREE(row_ptr);
  
  V_FREE(wu);
  V_FREE(wl);
  
  IV_FREE(jr);
  IV_FREE(jwl);
  IV_FREE(jwu);
  
  V_FREE(alu);
  IV_FREE(jlu);
  IV_FREE(ju);
  
  return ILUT ;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void static qsplit( Real *a, int *ind, int n, int ncut)
{
   /*-----------------------------------------------------------------------*/
   /*  does a quick-sort split of a real array.                             */
   /*  on input a(1:n). is a real array                                     */
   /*  on output a(1:n) is permuted such that its elements satisfy:         */
   /*                                                                       */
   /*  abs(a(i)) >= abs(a(ncut)) for i < ncut and                           */
   /*  abs(a(i)) <= abs(a(ncut)) for i > ncut                               */
   /*                                                                       */
   /*  ind(1:n) is an integer array which permuted in the same way as a     */
   /*  -------------------------------------------------------------------- */
   Real tmp, abskey;
   int  itmp, first, last, mid, j;
   
   first = 0;
   last = n-1;
   
   ncut--;       /* add by Xav to fit C indexes */
   
   if ( (ncut < first) || (ncut > last) ) return;
   
   while (1)        /* there is a "return" statement inside the loop */
   {
      mid = first;
      abskey = fabs(a[mid]);
      
      for (j=first+1; j<=last; j++)
      {
         if ( fabs(a[j]) > abskey )
         {
            mid = mid+1;
            /* interchange  */
            tmp = a[mid];
            itmp = ind[mid];
            a[mid] = a[j];
            ind[mid] = ind[j];
            a[j]  = tmp;
            ind[j] = itmp;
         }
      }
      
      /* interchange */
      
      tmp = a[mid];
      a[mid] = a[first];
      a[first]  = tmp;
      
      itmp = ind[mid];
      ind[mid] = ind[first];
      ind[first] = itmp;
      
      /* test for while loop */
      
      if (mid == ncut) return;
      
      if (mid > ncut)
      {
         last = mid-1;
      }
      else
      {
         first = mid+1;
      }
   }
   
   return;
}


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* spILUsolve -- solve A.x = b using factored matrix A from spILUfactor()
-- no permutation in the arguments
-- returns x
-- may not be in-situ */
VEC	*spILUsolve(const SPMAT *A, const VEC *b, VEC *x)
{
   static PERM *Id = PNULL;
   Id = px_resize(Id, A->m);
   
   MEM_STAT_REG(Id, TYPE_PERM);
      
   if ( ! A )  error(E_NULL,"spILUsolve");
   if ( ! b )  error(E_NULL,"spILUsolve");
   if ( b == x )  error(E_INSITU,"spILUsolve");
   
   spLUsolve(A,Id,b,x);
   
   return x;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT* m_sp_mmm_invL_A_invLt(const SPMAT *LLT, const SPMAT *A, MAT *M)
{
   /*

    M = inv(L).A.inv(Lt) 

        -> H = A.inv(Lt)  -> H.Lt = A   -> calculate H

        -> M = inv(L).H   -> L.M  = H   -> calculate M
   */

   int i,j,k;

   MAT *H;

   real val0 = 0.0;
   real val1 = 0.0;
   real val2 = 0.0;

   /* check */
   if ( A   == NULL) error(E_NULL,"m_sp_mmm_invL_A_invLt");
   if ( LLT == NULL) error(E_NULL,"m_sp_mmm_invL_A_invLt");
   if ( A->m != A->n ) error(E_SQUARE,"m_sp_mmm_invL_A_invLt");
   if ( M == NULL)
   {
      M = m_get(A->m, A->n);
   }
   else
   {
      if ( (M->m != A->m)||(M->n != A->n) )  error(E_SIZES,"m_sp_mmm_invL_A_invLt");
   }
   
   /* start */
   H = m_get(A->m, A->n);
   H = m_zero(H);
   /* eval H */
   for (j=0; j<A->n; j++)
   for (i=j; i<A->m; i++)
   {
      val0 = 0.0;

      for (k=0; k<j; k++)
      {
         val0 +=  H->me[i][k] * sp_get_val(LLT,j,k) ;
      }

      H->me[i][j] = 1.0/(sp_get_val(LLT,j,j)) * ( sp_get_val(A,i,j) - val0 ) ;
   }

   M = m_zero(M);
   /* eval C */
   for (i=0; i<A->m; i++)
   for (j=0; j<=i;   j++)
   {
      val1 = 0.0;
      val2 = 0.0;

      for (k=0; k<j; k++)
      {
         val1 +=  sp_get_val(LLT,i,k) * M->me[j][k] ;
      }
      for (k=j; k<i; k++)
      {
         val2 +=  sp_get_val(LLT,i,k) * M->me[k][j] ;
      }

      M->me[i][j] = 1.0/(sp_get_val(LLT,i,i)) * ( H->me[i][j] - val1 - val2 ) ;
   }

   /* C is symmetric : fill the upper part now */
   for (i=0;   i<A->m; i++)
   for (j=i+1; j<A->n; j++)
   {
      M->me[i][j] = M->me[j][i] ;
   }

   /* clean */
   M_FREE(H);

   /* finito */
   return M;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT* m_spLTsolve_vectorset(const SPMAT *LLT, MAT *M)  /* is done In-Situ */
{
   VEC *x_vec;
   VEC *y_vec;

   int k;

   int		i, scan_idx, scan_row;
   SPROW	*row;
   row_elt	*elt;
   Real         diag_val, sum;
   Real         norm_x_vec;

   real *x;
   real *y;
   
   /* check */
   if ( ! LLT )  error(E_NULL,"spLTsolve_vectorset");
   if ( ! M   )  error(E_NULL,"spLTsolve_vectorset");

   /* start */
   if ( ! LLT->flag_col )  sp_col_access((SPMAT*)LLT);
   if ( ! LLT->flag_diag ) sp_diag_access((SPMAT*)LLT);

   x_vec = v_get(LLT->m);
   y_vec = v_get(LLT->m);
   
   for (k=0; k<LLT->n; k++)
   {
      y_vec = get_col(M, k, y_vec);

      x_vec = v_copy(y_vec,x_vec);

      x = x_vec->ve;
      y = y_vec->ve;

      /* backward substitution: solve L^T.out = x for out */
      for ( i = LLT->n -1; i >= 0; i-- )
      {
         sum = y[i];

         row = &(LLT->row[i]);
         /* Note that row->diag >= 0 by above loop */
         elt = &(row->elt[row->diag]);
         diag_val = elt->val;

         /* scan down column */
         scan_idx = elt->nxt_idx;
         scan_row = elt->nxt_row;
         while ( scan_row >= 0 /* && scan_idx >= 0 */ )
         {
            row = &(LLT->row[scan_row]);
            elt = &(row->elt[scan_idx]);

            sum -= elt->val*x[scan_row];
            scan_idx = elt->nxt_idx;
            scan_row = elt->nxt_row;
         }

         x[i] = sum/diag_val;
      }

      norm_x_vec = v_norm2(x_vec); sv_mlt(1.0/norm_x_vec, x_vec, x_vec);

      set_col(M, k, x_vec);
   }

   /* clean */
   V_FREE(x_vec);
   V_FREE(y_vec);
   
   /* finito */
   return M;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* sp_bandw -- the band width of A */
int sp_bandw(const SPMAT *A)
{
   int i, idx1, idx2, bandw = 0;
   int tmp;
   int lb,ub;
   
   SPROW *r;

   if ( ! A )  error(E_NULL,"sp_bandw");

   for (i=0; i<A->m; i++ )
   {
      r = A->row + i ;

      if ( r->len > 0 )
      {
         idx1 = r->elt[0].col;
         idx2 = r->elt[r->len-1].col;

         lb = abs(i-idx1);
         ub = abs(i-idx2);

         tmp = ( lb > ub ? lb : ub );

         if ( tmp > bandw )
         {
            bandw = tmp;
         }
      }
   }

   return bandw;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* sp_profile -- the profile of A */
int sp_profile(const SPMAT *A)
{
   int i, prof = 0;
   SPROW *r;

   if ( ! A )  error(E_NULL,"sp_profile");

   for (i=0; i<A->m; i++ )
   {
      int prof_i = 0;
      
      r = A->row + i ;
      
      if ( r->len == 0 )
      {
         /* error(E_SING,"sp_profile"); */
         prof_i = 0;
      }
      else
      if ( r->len == 1 )
      {
         prof_i = abs( i - r->elt[0].col );
      }
      else
      {
         int idx1 = r->elt[0].col;
         int idx2 = r->elt[r->len-1].col;
      
         if ( ( idx1 < i )&&(idx2 > i) )
         {
            /*
            prof_i = i - idx1 > idx2 -i ? i - idx1 : idx2 - i ;
            */
            prof_i = idx2 - idx1 ;
         }
         else
         {
            if ( idx1 < i )
               prof_i = i - idx1 ;
            else
               prof_i = idx2 - i ; 
         }
      }

      prof += prof_i;
   }

   return prof;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* sp_nonzeros -- counts non-zeros in SPMAT* A */
int sp_nonzeros(const SPMAT *A)
{
   int cnt, i;
   
   if ( ! A )  error(E_NULL,"sp_nonzeros");

   cnt = 0;
   for (i=0; i<A->m; i++ ) cnt += A->row[i].len;
   
   return cnt;
}

/* sp_nonzeros_symmetric_part -- counts non-zeros in SPMAT* A (only upper part with A symmetrix) */
int sp_nonzeros_symmetric_part(const SPMAT *A)
{
   int cnt = 0;
   int i,j;

   if ( ! A )  error(E_NULL,"sp_nonzeros_symmetric_part");

   for(i=0; i<A->m; i++)
   {
      SPROW *r = A->row +i;
      
      for (j=0; j<r->len; j++)
      {
         if ( r->elt[j].col < i )
         {
            continue;
         }
         if ( fabs(r->elt[j].val) > MACHEPS )
         {
            cnt++;
         }
      }
   }
   
   return cnt;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

SPROW *sprow_lincomb(int s, SPROW **allrows, Real *coeffs, int type_sprow, SPROW *out) /* out <- sum_i s[i].x[i] */
{
   int i,k;
   SPROW  *tmp[10];

   /* check */
   if ( ! out )  error(E_NULL,"sprow_lincomb");
   if ( ! coeffs )  error(E_NULL,"sprow_lincomb");
   if ( ! allrows )  error(E_NULL,"sprow_lincomb");
      
   /* alloc mem */
   for (i=0; i<s; i++)
   {
      tmp[i] = sprow_get(10);
   }

   /* start the loop*/
   sprow_smlt  (allrows[0], coeffs[0], 0, tmp[0], type_sprow);

   /* the loop */
   for (k=1; k<s; k++)
   {
      sprow_mltadd(tmp[k-1], allrows[k], coeffs[k], 0, tmp[k], type_sprow);
   }

   sprow_copy2(tmp[s-1], out, type_sprow);
   

   /* free mem */
   for (i=0; i<s; i++)
   {
      SPROW_FREE(tmp[i]);
   }

   /* finito */
   return out;
}


SPROW *sprow_linlist(SPROW *out, int type_sprow, SPROW *v1, real a1, ...)  /* out <- s1.x1 + s2.x2 + ... */
{
   error(E_NULL  ,"sprow_linlist"); /* ToDo */

   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

