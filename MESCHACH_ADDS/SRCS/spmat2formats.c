
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/spmat2formats.h"

/** \file spmat2formats.c
 *
 * Contains routines to transform SPMAT to "FORTRAN" sparse arrays for sparse matrices
 *
 *
 * Transform the SPMAT meschach format to Compressed Row Storage
 *                                        Compressed Column Storage
 *                                        Modified Row Storage
 *
 */


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  spmat2crs(const SPMAT *A , VEC *a , IVEC *col_ind , IVEC *row_ptr , int type /* 'C' or 'F' */)
{
   int i,idx,nnz,curr;
   SPROW *r;
   int incr = 0;
   
   /* tests NULL structures */
   if ( A       == (SPMAT *)NULL ) error(E_NULL,"spmat2crs");
   if ( a       == (VEC   *)NULL ) error(E_NULL,"spmat2crs");
   if ( col_ind == (IVEC  *)NULL ) error(E_NULL,"spmat2crs");
   if ( row_ptr == (IVEC  *)NULL ) error(E_NULL,"spmat2crs");
   /* test length of row_ptr */
   if (row_ptr->dim != A->m +1) error(E_SIZES,"spmat2crs");
   
   /* resize a and col_ind with nnz(A) if neccesary */
   nnz = sp_nonzeros(A); /* printf("nnz(A) = %d \n",nnz); */
   /* resize if needed */
   if (a->dim       < nnz )  v_resize(a      , nnz);
   if (col_ind->dim < nnz ) iv_resize(col_ind, nnz);
   
   switch(type)
   {
      case 'C': incr = 0; break;
      case 'F': incr = 1; break;
      default: error(E_UNKNOWN,"crs2spmat");
   }

   /* init */
   curr = 0;
   row_ptr->ive[0] = incr;
   
   for (i=0; i<A->m; i++)
   {
      r = A->row + i ;
      
      /* test */
      if ( a->dim < curr + r->len )
      {
         error(E_SIZES,"spmat2crs");
      }
      
      /* fill */
      for (idx=0; idx<r->len; idx++)
      {
         a->ve[curr]        =  r->elt[idx].val ;
         col_ind->ive[curr] =  r->elt[idx].col + incr;
         
         curr++;
      }
      
      row_ptr->ive[i+1] = curr+incr;
   }
   
   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  crs2spmat(SPMAT *A , const VEC *a , const IVEC *col_ind , const IVEC *row_ptr , int type /* 'C' or 'F' */)
{
   int i,j,idx;
   int incr = 0;
   
   /* test length of row_ptr */
   if ( A == (SPMAT *)NULL )    error(E_NULL,"crs2spmat");
   if ( col_ind == (IVEC *)NULL ) error(E_NULL,"crs2spmat");
   if ( row_ptr == (IVEC *)NULL ) error(E_NULL,"crs2spmat");
   /* test length of row_ptr */
   if (row_ptr->dim != A->m +1) error(E_SIZES,"crs2spmat");
   
   /* init */
   sp_zero(A);
   
   switch(type)
   {
      case 'C': incr = 0; break;
      case 'F': incr = 1; break;
      default: error(E_UNKNOWN,"crs2spmat");
   }

   /* fill */
   for (i=0; i<A->m; i++)
   for (idx=0; idx<(row_ptr->ive[i+1]-row_ptr->ive[i]); idx++)
   {
      j = row_ptr->ive[i]+idx-incr ;
      sp_set_val(A,i,col_ind->ive[j]-incr, a->ve[j]);
   }
      
   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  spmat2ccs(const SPMAT *A , VEC *a , IVEC *row_ind , IVEC *col_ptr , int type /* 'C' or 'F' */)
{
   SPMAT *At;
   
   /* tests NULL structures */
   if ( A      ==(SPMAT *)NULL ) error(E_NULL,"spmat2ccs");
   if ( a      ==(VEC   *)NULL ) error(E_NULL,"spmat2ccs");
   if ( row_ind==(IVEC  *)NULL ) error(E_NULL,"spmat2ccs");
   if ( col_ptr==(IVEC  *)NULL ) error(E_NULL,"spmat2ccs");


   /* consider the transposed of A */
   At = sp_transp(A, NULL);

   spmat2crs(At, a, row_ind, col_ptr, type);

   /* clean */
   SP_FREE(At);
   
   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  ccs2spmat(SPMAT *A , const VEC *a , const IVEC *row_ind , const IVEC *col_ptr , int type /* 'C' or 'F' */)
{
   int i,j,idx;
   int incr = 0;
   
   /* test length of col_ptr */
   if ( A == (SPMAT *)NULL )    error(E_NULL,"ccs2spmat");
   /* test length of col_ptr */
   if (col_ptr->dim != A->m +1) error(E_SIZES,"ccs2spmat");

   /* init */
   sp_zero(A);

   switch(type)
   {
      case 'C': incr = 0; break;
      case 'F': incr = 1; break;
      default: error(E_UNKNOWN,"ccs2spmat");
   }

   /* fill */
   for (i=0; i<A->m; i++)
   for (idx=0; idx<(col_ptr->ive[i+1]-col_ptr->ive[i]); idx++)
   {
      j = col_ptr->ive[i]+idx-incr ;
      sp_set_val(A, row_ind->ive[j]-incr,i, a->ve[j]);
   }

   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  spmat2mrs(const SPMAT *A , VEC *a , IVEC *col_ind )
{
   error(E_UNKNOWN,"spmat2mrs");   
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  mrs2spmat( SPMAT *A , const VEC *a , const IVEC *col_ind )
{
   int i,k,idx;
   
   /* tests NULL structures */
   if ( A==(SPMAT *)NULL ) error(E_NULL,"mrs2spmat");
   if ( a==(VEC   *)NULL ) error(E_NULL,"mrs2spmat");
   if ( col_ind==(IVEC *)NULL ) error(E_NULL,"mrs2spmat");
   

   printf("fill an SPMAT* from mrs data ... \n");
   
   sp_zero(A);
   
   /* the diagonal */
   for (i=0; i<A->m; i++)  sp_set_val(A,i,i, 1.0/a->ve[i] );
   
   k = A->m +1;  /* k goes throught the tab "a" and "col_ind" */
   /* non diag elements */
   for (i=0; i<A->m; i++)
   for (idx=0; idx<col_ind->ive[i+1]-col_ind->ive[i]; idx++)
   {
      sp_set_val(A,i,col_ind->ive[k]-1, a->ve[k]);
         
      k++;
   }
      
   printf("... done \n");
      
   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

