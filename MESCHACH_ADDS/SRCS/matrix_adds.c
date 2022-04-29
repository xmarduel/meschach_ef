
/** \file matrix_adds.c
 *
 * extentions to MESCHACH LIBRARY for usual structs 
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
 

/* local variables */
static char line[MAXLINE];


/* ------------------------------------------------------------------------------------------- */
/* --------------------- easy memory administration  for list 1 ------------------------------ */

int mem_attach_list1(void)
{
   return mem_attach_list(MY_LIST1, 2, my_names1, my_frees1, my_tnums1);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* im_get -- gets an mxn integer matrix (in IMAT form) by dynamic memory */
/*                                                            allocation */
IMAT   *im_get(int m, int n)
{
   IMAT  *imatrix;
   int  i;
   
   if (m < 0 || n < 0)   error(E_NEG,"im_get");
   
   if ( (imatrix=NEW(IMAT)) == (IMAT *)NULL )
   {
      error(E_MEM,"im_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_IMAT,0,sizeof(IMAT),MY_LIST1);
      mem_numvar_list(TYPE_IMAT,1,MY_LIST1);
   }
   
   imatrix->m = imatrix->max_m = m;
   imatrix->n = imatrix->max_n = n;
   imatrix->max_size = m*n;
   
   if ((imatrix->ibase = NEW_A(m*n,int)) == (int *)NULL )
   {
      free(imatrix);
      error(E_MEM,"im_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_IMAT,0,m*n*sizeof(int),MY_LIST1);
   }
    
   /* Create pointers */
   if ( (imatrix->im = NEW_A(m,int *)) == (int **)NULL )
   {
      free(imatrix->ibase);     free(imatrix);
      error(E_MEM,"im_get");
   }
   else if  (mem_info_is_on())
   {
      mem_bytes_list(TYPE_IMAT,0,m*sizeof(int *),MY_LIST1);
   }
   
   /* set up pointers */
   for ( i=0; i<m; i++ ) imatrix->im[i] = &(imatrix->ibase[i*n]);
     
   return (imatrix);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* im_free -- returns IMAT & asoociated memory back to memory heap */
int      im_free(IMAT *imat)
{
   int  m,n;
   
   if ( imat==(IMAT *)NULL || (int)(imat->m) < 0 || (int)(imat->n) < 0 )
      /* don't trust it */
      return (-1);
   
   m=imat->m;
   n=imat->n;
   
   if ( imat->ibase != (int *)NULL )
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_IMAT,m*n*sizeof(int),0,MY_LIST1);
      }
      free((char *)(imat->ibase));
   }
   
   if ( imat->im != (int **)NULL )
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_IMAT,m*sizeof(int *),0,MY_LIST1);
      }
      free((char *)(imat->im));
   }
   
   if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_IMAT,sizeof(IMAT),0,MY_LIST1);
      mem_numvar_list(TYPE_IMAT,-1,MY_LIST1);
   }
   
   free((char *)imat);   
   
   return (0);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* _im_copy -- copy an imatrix into new area */
IMAT *_im_copy(const IMAT *inn, IMAT *out, int i0 , int j0)
{
   u_int	i,j;
   
   if ( inn == IMNULL )     error(E_NULL,"im_copy");
   if ( inn == out )        return (out);
   if ( out==IMNULL || out->m != inn->m || out->n != inn->n )
      out = im_resize(out,inn->m,inn->n);
   
   for ( i=i0; i < inn->m; i++ )
   for ( j=j0; j < inn->n; j++ )
      out->im[i][j] = inn->im[i][j];
      
   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* im_resize -- resize an imatrix into new area */
IMAT *im_resize(IMAT *A, int new_m , int new_n)
{
   int	i;
   int	new_max_m, new_max_n, new_size, old_m, old_n;
   
   if (new_m < 0 || new_n < 0)    error(E_NEG,"im_resize");
   
   if ( ! A )  return im_get(new_m,new_n);
   
   /* nothing was changed */
   if (new_m == A->m && new_n == A->n)  return A;
   
   old_m = A->m;
   old_n = A->n;
   
   if ( new_m > A->max_m )
   {
      /* re-allocate A->im */
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_IMAT,A->max_m*sizeof(int *),
            new_m*sizeof(int *),MY_LIST1);
      }
      
      A->im = RENEW(A->im,new_m,int *);
      if ( ! A->im ) error(E_MEM,"im_resize");
   }
   
   new_max_m = max(new_m,A->max_m);
   new_max_n = max(new_n,A->max_n);
   
   
   new_size = new_max_m*new_max_n;
   
   if ( new_size > A->max_size )
   {  /* re-allocate A->ibase */
      if (mem_info_is_on()) {
         mem_bytes_list(TYPE_IMAT,A->max_m*A->max_n*sizeof(int),
            new_size*sizeof(int),MY_LIST1);
      }
      
      A->ibase = RENEW(A->ibase,new_size,int);
      if ( ! A->ibase )	error(E_MEM,"im_resize");
      
      A->max_size = new_size;
   }
   
   /* now set up A->im[i] */
   for ( i = 0; i < new_m; i++ ) A->im[i] = &(A->ibase[i*new_n]);
   
   /* now shift data in matrix */
   if ( old_n > new_n )
   {
      for ( i = 1; i < min(old_m,new_m); i++ )
         MEM_COPY((char *)&(A->ibase[i*old_n]),
         (char *)&(A->ibase[i*new_n]),
         sizeof(int)*new_n);
   }
   else if ( old_n < new_n )
   {
      for ( i = (int)(min(old_m,new_m))-1; i > 0; i-- )
      {   /* copy & then zero extra space */
         MEM_COPY((char *)&(A->ibase[i*old_n]),
            (char *)&(A->ibase[i*new_n]),
            sizeof(int)*old_n);
         /* __zero__(&(A->ibase[i*new_n+old_n]),(new_n-old_n));*/
      }
      /* __zero__(&(A->ibase[old_n]),(new_n-old_n));*/
      A->max_n = new_n;
   }
   
   /* zero out the new rows.. */
   for ( i = old_m; i < new_m; i++ )
      /* __zero__(&(A->ibase[i*new_n]),new_n)*/;
         
   A->max_m = new_max_m;
   A->max_n = new_max_n;
   A->max_size = A->max_m*A->max_n;
   A->m = new_m;	
   A->n = new_n;
   
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

static const char    *iformat = "%6d ";
/* xim_finput -- input of a IMAT (there is already an "im_finput" for MAT struct) */
IMAT*  xim_finput(FILE *fp, IMAT *imat)
{
   if ( isatty(fileno(fp)) )
      return iim_finput(fp,imat);
   else
      return bim_finput(fp,imat);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* imi_finput -- interactive input of an integer matrix */
IMAT     *iim_finput(FILE *fp, IMAT *imat)
{
   char       c;
   u_int      i, j, m, n, dynamic;
   /* dynamic set to TRUE if memory allocated here */
   
   /* get matrix size */
   if ( imat != (IMAT *)NULL && imat->m<MAXDIM && imat->n<MAXDIM )
   {
      m = imat->m;     n = imat->n;     dynamic = FALSE;
   }
   else
   {
      dynamic = TRUE;
      do
      {
         fprintf(stderr,"IMatrix: rows cols:");
         
         if ( fgets(line,MAXLINE,fp)==NULL )
         {
            error(E_INPUT,"iim_finput");
         }
      } while ( sscanf(line,"%u%u",&m,&n)<2 || m>MAXDIM || n>MAXDIM );
      
      imat = im_get(m,n);
   }
   
   /* input elements */
   for ( i=0; i<m; i++ )
   {
redo:
      fprintf(stderr,"row %u:\n",i);
      for ( j=0; j<n; j++ )
      do
      {
redo2:
         fprintf(stderr,"entry (%u,%u): ",i,j);
         if ( !dynamic )
            fprintf(stderr,"old %14.9d new: ",
            imat->im[i][j]);
         if ( fgets(line,MAXLINE,fp)==NULL )
            error(E_INPUT,"imi_finput");
         if ( (*line == 'b' || *line == 'B') && j > 0 )
         {
            j--;
            dynamic = FALSE;
            goto redo2;
         }
         if ( (*line == 'f' || *line == 'F') && j < n-1 )
         {
            j++;
            dynamic = FALSE;
            goto redo2;
         }
      }
      while ( *line=='\0' || sscanf(line,"%d",&imat->im[i][j])<1 );

      fprintf(stderr,"Continue: ");
      fscanf(fp,"%c",&c);
      if ( c == 'n' || c == 'N' )
      {  
         dynamic = FALSE;
         goto redo;
      }
      if ( (c == 'b' || c == 'B') /* && i > 0 */ )
      {
         if ( i > 0 )
            i--;
         dynamic = FALSE;
         goto redo;
      }
   }
   
   return (imat);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* bmi_finput -- batch-file input of an integer matrix */
IMAT     *bim_finput(FILE *fp, IMAT *imat)
{
   u_int      i,j,m,n,dummy;
   int        io_code;

   /* get dimension */
   skipjunk(fp);
   if ((io_code=fscanf(fp,"IntMatrix: %u by %u",&m,&n)) < 2 || m>MAXDIM || n>MAXDIM )
      error(io_code==EOF ? E_EOF : E_FORMAT,"bim_finput");

   /* allocate memory if necessary */
   if ( imat==(IMAT *)NULL )
      imat = im_resize(imat,m,n);

   /* get entries */
   for ( i=0; i<m; i++ )
   {
      skipjunk(fp);
      if ( fscanf(fp," row %u:",&dummy) < 1 )
         error(E_FORMAT,"bim_finput");
      for ( j=0; j<n; j++ )
      {
         if ((io_code=fscanf(fp,"%d",&imat->im[i][j])) < 1 )
	         error(io_code==EOF ? 7 : 6,"bim_finput");
      }
   }

   return (imat);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  im_dump(FILE *fp, const IMAT *a)
{
   int  i,j,tmp;
   
   if ( a == (IMAT *)NULL )
   { 
      fprintf(fp,"IntMatrix: NULL\n");   return;
   }
   fprintf(fp,"IntMatrix: %d by %d @ 0x%lx\n",a->m,a->n,(long)a);
   fprintf(fp,"\tmax_m = %d, max_n = %d, max_size = %d\n",
	     a->max_m, a->max_n, a->max_size);
   if ( a->im == (int **)NULL )
   {
      fprintf(fp,"NULL\n");           return;  
   }
   fprintf(fp,"a->im @ 0x%lx\n",(long)(a->im));
   fprintf(fp,"a->ibase @ 0x%lx\n",(long)(a->ibase));
   for ( i=0; i<a->m; i++ )   /* for each row... */
   {
      fprintf(fp,"row %u: @ 0x%lx ",i,(long)(a->im[i]));
      for ( j=0, tmp=2; j<a->n; j++, tmp++ )
      { /* for each col in row... */
         fprintf(fp,iformat,a->im[i][j]);
         if ( ! (tmp % 5) )       putc('\n',fp);
      }
      if ( tmp % 5 != 1 )   putc('\n',fp);
   }
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* im_foutput -- output of a IMAT */
void    im_foutput(FILE *fp, const IMAT *imat)
{
   u_int      i, j, tmp;
   
   if ( imat == (IMAT *)NULL )
   { 
      fprintf(fp,"IntMatrix: NULL\n");
      return;
   }
   
   fprintf(fp,"IntMatrix: %d by %d\n",imat->m,imat->n);
   if ( imat->im == (int **)NULL )
   {
      fprintf(fp,"NULL\n"); 
      return;
   }
   
   for ( i=0; i<imat->m; i++ )                  /* for each row ... */
   {
      fprintf(fp,"row %u: ",i);
      for ( j=0, tmp=2; j<imat->n; j++, tmp++ )  /* for each col in row... */
      {
         fprintf(fp,iformat,imat->im[i][j]);
         if ( ! (tmp % 8) )       putc('\n',fp);
      }
      if ( tmp % 8 != 1 )   putc('\n',fp);
   }
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* im_zero -- zero the matrix A */
IMAT	*im_zero(IMAT	*A)
{
   int i,j, A_m, A_n;
   int **A_im;
   
   if ( A == IMNULL ) error(E_NULL,"im_zero");
   
   A_m  = A->m;
   A_n  = A->n;
   A_im = A->im;
   
   for ( i=0; i<A_m; i++ )
   for ( j=0; j<A_n; j++ ) A_im[i][j] = 0;
      
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* mat_id -- set A to being closest to identity matrix as possible
-- i.e. A[i][j] == 1 if i == j and 0 otherwise */
  IMAT	*im_ident(IMAT *A)
{
   int	i, size;
   
   if ( A == IMNULL ) error(E_NULL,"im_ident");
   
   im_zero(A);
   
   size = min(A->m,A->n);
   for ( i=0; i<size; i++ )A->im[i][i] = 1;
   
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT	*im_rand(IMAT *A)
{
   int i,j;
   Real val;
   
   if ( A == IMNULL ) error(E_NULL,"im_rand");

   for (i=0; i<A->m; i++)
   for (j=0; j<A->n; j++)
   {
      val = mrand()*1000 ;

      A->im[i][j] = (int)val;
   }
      
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* im_ones -- */
IMAT	*im_ones(IMAT *A)
{
   int	i,j;
   
   if ( A == IMNULL ) error(E_NULL,"im_ones");
   
   for ( i=0; i<A->m; i++ )
   for ( j=0; j<A->n; j++ )  A->im[i][j] = 1;
      
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT *im_transp(const IMAT* A, IMAT* B)
{
   int i,j;

   if ( A == IMNULL ) error(E_NULL,"im_transp");
   if ( B == IMNULL )
   {
      B = im_get(A->n, A->m);
   }
   else
   {
      if ( A->m != B->n ) error(E_SIZES,"im_transp");
      if ( A->n != B->m ) error(E_SIZES,"im_transp");
   }

   for (i=0; i<A->m; i++)
   for (j=0; j<A->n; j++)
   {
      B->im[j][i] = A->im[i][j];
   }
      
   return B;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT *im_add(const IMAT *A, const IMAT *B, IMAT *out)
{
   int i,j;
   
   if ( A == IMNULL ) error(E_NULL,"im_add");
   if ( B == IMNULL ) error(E_NULL,"im_add");
   
   if ( (A->m != B->m) ||  (A->n != B->n) ) error(E_SIZES,"im_add");
   
   
   if ( out == IMNULL ) out = im_get(A->m,A->n);
   
   if ( out != IMNULL && ( (out->m != A->m)||(out->n != A->n)) )
      error(E_SIZES,"im_add");
   
   for ( i = 0; i < A->m; i++ )
   for ( j = 0; j < A->n; j++ )
   {
      out->im[i][j] = A->im[i][j] + B->im[i][j];
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT *im_sub(const IMAT *A, const IMAT *B, IMAT *out)
{
   int i,j;
   
   if ( A == IMNULL ) error(E_NULL,"im_sub");
   if ( B == IMNULL ) error(E_NULL,"im_sub");
   
   if ( (A->m != B->m) ||  (A->n != B->n) ) error(E_SIZES,"im_sub");
   
   
   if ( out == IMNULL ) out = im_get(A->m,A->n);
   
   if ( out != IMNULL && ( (out->m != A->m)||(out->n != A->n)) )
      error(E_SIZES,"im_sub");
   
   for ( i = 0; i < A->m; i++ )
   for ( j = 0; j < A->n; j++ )
   {
      out->im[i][j] = A->im[i][j] - B->im[i][j];
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT *im_mlt(const IMAT *A, const IMAT *B, IMAT *out)
{
   int i,j,k;
   
   if ( A == IMNULL ) error(E_NULL,"im_mlt");
   if ( B == IMNULL ) error(E_NULL,"im_mlt");
   
   if ( (A->n != B->m) ) error(E_SIZES,"im_mlt");
   
   
   if ( out == IMNULL ) out = im_get(A->m,B->n);
   
   if ( out != IMNULL && ((out->m != A->m)||(out->n != B->n)) )
      error(E_SIZES,"im_mlt");
   
   for ( i = 0; i < A->m; i++ )
   for ( j = 0; j < B->n; j++ )
   { 
      out->im[i][j] = 0;
      for ( k = 0 ; k < A->n ; k++ )
      {
         out->im[i][j] += A->im[i][k] * B->im[k][j];
      }
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT *im_smlt(const IMAT *A, int  n,  IMAT *out)
{
   int i,j;
   
   if ( A == IMNULL ) error(E_NULL,"im_smlt");
   
   if ( out != IMNULL && ((out->m != A->m)||(out->n != out->n)) )      error(E_SIZES,"im_smlt");
   
   if ( out == IMNULL ) out = im_get(A->m,A->n);
   
   
   for ( i = 0; i < A->m; i++ )
   for ( j = 0; j < A->n; j++ )
   { 
      out->im[i][j] = n * A->im[i][j];
   }
      
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IVEC *im_vmlt(const IMAT *A, const IVEC *V, IVEC *out)
{
   int i,j;
   
   if ( A == IMNULL ) error(E_NULL,"im_vmlt");
   if ( V == IVNULL ) error(E_NULL,"im_vmlt");
   
   if ( (A->n != V->dim) ) error(E_SIZES,"im_vmlt");
   
   if ( out != IVNULL && (out->dim != A->m) )    error(E_SIZES,"im_vmlt");

   
   if ( out == IVNULL ) out = iv_get(A->m);
   
   for ( i = 0; i < A->m; i++ )
   { 
      out->ive[i] = 0;
      for ( j = 0 ; j < A->n ; j++ )
      {
         out->ive[i] += A->im[i][j] * V->ive[j];
      }
   }
   
   return out;
}

/* ********************************************************************* */
/* *************************  TENSOR STUFF  **************************** */
/* ********************************************************************* */

TENSOR  *ts_get(int l, int m, int n) /* gets an lxmxn Real tensor by dynamic memory allocation */
{
   TENSOR  *tensor;
   int  i,j;

   if (l < 0 || m < 0 || n < 0)   error(E_NEG,"ts_get");
   
   if ( (tensor=NEW(TENSOR)) == (TENSOR *)NULL )
   {
      error(E_MEM,"ts_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_TENSOR,0,sizeof(TENSOR),MY_LIST1);
      mem_numvar_list(TYPE_TENSOR,1,MY_LIST1);
   }
   
   tensor->l = tensor->max_l = l;
   tensor->m = tensor->max_m = m;
   tensor->n = tensor->max_n = n;
   tensor->max_size = l*m*n;
   
   /* Memory allocation */
   if ( (tensor->base = NEW_A(l*m*n,Real)) == (Real *)NULL )
   {
      free(tensor);
      error(E_MEM,"ts_get");    
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_TENSOR,0,l*m*n*sizeof(Real),MY_LIST1);
   }
   
   /* Create pointers */
   if ( (tensor->te = NEW_A(l,Real **)) == (Real ***)NULL )
   {
      free(tensor->base);  free(tensor);
      error(E_MEM,"ts_get");
   }
   else if  (mem_info_is_on())
   {
      mem_bytes_list(TYPE_TENSOR,0,l*sizeof(Real **),MY_LIST1);    
   }
   
   /* Create pointers */
   if ( (tensor->me = NEW_A(l*m,Real *)) == (Real **)NULL )
   {
      free(tensor->base);
      free(tensor->te);  free(tensor);
      error(E_MEM,"ts_get");
   }
   else if  (mem_info_is_on())
   {
      mem_bytes_list(TYPE_TENSOR,0,l*m*sizeof(Real *),MY_LIST1);    
   }
   
   /*set up pointers */
   for (j=0; j<l*m; j++)
   {
      tensor->me[j] = &(tensor->base[j*n]);
   }
   for (i=0; i<l; i++)
   {    
      tensor->te[i] = &(tensor->me[i*m]);
   }
   
   return (tensor);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int ts_free(TENSOR *tensor) /* returns TENSOR & asoociated memory back to memory heap */
{
   int  l,m,n;
   
   if ( tensor==(TENSOR *)NULL || (int)(tensor->l) < 0 || (int)(tensor->m) < 0 || (int)(tensor->n) < 0 )
      /* don't trust it */
      return (-1);
   
   l=tensor->max_l;
   m=tensor->max_m;
   n=tensor->max_n;
   
   if ( tensor->base != (Real *)NULL )
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_TENSOR,l*m*n*sizeof(Real),0,MY_LIST1);
      }
      free((char *)(tensor->base));
   }
   
   if ( tensor->me != (Real  **)NULL )
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_TENSOR,l*m*sizeof(Real *),0,MY_LIST1);
      }
      free((char *)(tensor->me));
   }
   
   if ( tensor->te   != (Real ***)NULL ) 
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_TENSOR,l*sizeof(Real **),0,MY_LIST1);
      }
      free((char *)(tensor->te));
   }
   
   
   if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_TENSOR,sizeof(TENSOR),0,MY_LIST1);
      mem_numvar_list(TYPE_TENSOR,-1,MY_LIST1);
   }
   
   free((char *)tensor);   
   
   return (0);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR *_ts_copy(const TENSOR *inn, TENSOR *out, int i0, int j0, int k0) /* copies tensor into new area */
{
   u_int	i,j,k;
   
   if ( inn==TSNULL )      error(E_NULL,"ts_copy");

   if ( inn==out )
   {
      return (out);
   }
   if ( out==TSNULL )
   {
      out = ts_get(inn->l,inn->m,inn->n);
   }
   else
   if ( out->l != inn->l || out->m != inn->m || out->n != inn->n )
   {
      ts_free(out);
      out = ts_get(inn->l,inn->m,inn->n);
   }
   
   if ( i0==0 && j0==0 && k0==0 )
   {
      memmove( out->base, inn->base, sizeof(Real)*(inn->l*inn->m*inn->n) );
   }
   else
   {
      for ( i=i0; i<inn->l; i++ )
      for ( j=j0; j<inn->m; j++ )
      for ( k=k0; k<inn->n; k++ )
      {
         out->te[i][j][k] = inn->te[i][j][k];
      }
   }
      
   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/*static char    *lformat = "%12.6lf"; */
TENSOR*  ts_finput(FILE *fp, TENSOR *ts)
{
   /* difficult .. */
   error(E_INPUT,"ts_finput");

   return (TENSOR*)NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void  ts_dump(FILE *fp, const TENSOR *ts)
{
   int  i,j,k;
   MAT *mat;
   
   if ( ts == (TENSOR *)NULL )
   { 
      fprintf(fp,"Tensor: NULL\n");
      return;
   }
   fprintf(fp,"Tensor: %d by %d by %d @ 0x%lx\n",ts->l,ts->m,ts->n,(long)ts);
   fprintf(fp,"\tmax_l = %d, max_m = %d, max_n = %d, max_size = %d\n", ts->max_l, ts->max_m, ts->max_n, ts->max_size);
   if ( ts->te == (Real ***)NULL )
   {
      fprintf(fp,"NULL\n");
      return;  
   }
   fprintf(fp,"ts->te @ 0x%lx\n",(long)(ts->te));
   fprintf(fp,"ts->base @ 0x%lx\n",(long)(ts->base));
   
   mat = m_get(ts->m,ts->n);
   for (i=0; i<ts->l; i++)   /* for each matrix... */
   {
      fprintf(fp,"matrix %u: @ 0x%lx ",i,(long)(ts->te[i]));
      for (j=0; j<ts->m; j++)
      for (k=0; k<ts->n; k++)
      {
         mat->me[j][k] = ts->te[i][j][k];
      }
      m_foutput(fp, mat);
   }
   
   M_FREE(mat);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void ts_foutput(FILE *fp, const TENSOR *tensor) /* output of a TENSOR */
{
   u_int      i,j,k;
   MAT *mat;
   
   if ( tensor == (TENSOR *)NULL )
   { 
      fprintf(fp,"Tensor: NULL\n");
      return;
   }
   
   mat = m_get(tensor->m,tensor->n);
   
   fprintf(fp,"Tensor: %d by %d by %d\n",tensor->l,tensor->m,tensor->n);
   
   if ( tensor->te == (Real ***)NULL )
   {
      fprintf(fp,"NULL\n"); 
      return;
   }
   
   for ( i=0; i<tensor->l; i++ )                  /* for each row ... */
   {
      for (j=0; j<tensor->m; j++)
      for (k=0; k<tensor->n; k++)
      {
         mat->me[j][k] = tensor->te[i][j][k];
      }
      m_foutput(fp,mat);
   }
   
   M_FREE(mat);
   
   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR *ts_zero(TENSOR *A)  /* zero the tensor */
{
   int i,j,k, A_l, A_m, A_n;
   
   if ( A == TSNULL )   error(E_NULL,"ts_zero");
   
   A_l = A->l;
   A_m = A->m;
   A_n = A->n;
   
   for ( i=0; i<A_l; i++ )
   for ( j=0; j<A_m; j++ )
   for ( k=0; k<A_n; k++ )
   {
      A->te[i][j][k]= 0.0;
   }

   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR	*ts_ident(TENSOR *A)
{
   int i,size,tmp;
   
   if ( A == TSNULL ) error(E_NULL,"ts_ident");
   
   ts_zero(A);
   
   tmp  = min(A->l,A->m);
   size = min(tmp,A->n);
   
   for (i=0; i<size; i++) A->te[i][i][i] = 1.0;
   
   return A;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR* ts_rand(TENSOR* ts)
{
   int  i,j,k;

   if ( ts  == TSNULL ) error(E_NULL,"ts_rand");

   for ( i = 0; i < ts->l; i++ )
   for ( j = 0; j < ts->m; j++ )
   for ( k = 0; k < ts->n; k++ )
   {
      ts->te[i][j][k] = mrand() ;
   }

   return ts;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR *ts_add(const TENSOR *A, const TENSOR *B, TENSOR *out)
{
   int i,j,k;
   
   if ( A == TSNULL ) error(E_NULL,"ts_add");
   if ( B == TSNULL ) error(E_NULL,"ts_add");
   
   if ( (A->l != B->l) || (A->m != B->m) ||  (A->n != B->n) )
      error(E_SIZES,"ts_add");
   
   
   if ( out == TSNULL ) out = ts_get(A->l,A->m,A->n);
   
   if ( out != TSNULL && 
      (  (out->l != A->l)||(out->m != A->m)||(out->n != A->n)) )
      error(E_SIZES,"ts_add");
   
   for ( i=0; i<A->l; i++ )
   for ( j=0; j<A->m; j++ )
   for ( k=0; k<A->n; k++ )
   {
      out->te[i][j][k] = A->te[i][j][k] + B->te[i][j][k];
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR *ts_sub(const TENSOR *A, const TENSOR *B, TENSOR *out)
{
   int i,j,k;
   
   if ( A == TSNULL ) error(E_NULL,"ts_sub");
   if ( B == TSNULL ) error(E_NULL,"ts_sub");
   
   if ( (A->l != B->l) || (A->m != B->m) ||  (A->n != B->n) )
      error(E_SIZES,"ts_sub");
   
   
   if ( out == TSNULL ) out = ts_get(A->l,A->m,A->n);
   
   if ( out != TSNULL && 
      (  (out->l != A->l)||(out->m != A->m)||(out->n != A->n)) )
      error(E_SIZES,"ts_sub");
   
   for ( i = 0; i < A->l; i++ )
   for ( j = 0; j < A->m; j++ )
   for ( k = 0; k < A->n; k++ )
   {
      out->te[i][j][k] = A->te[i][j][k] - B->te[i][j][k];
   }

   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC *tsm_mlt(const TENSOR *tensor, const MAT *mat, VEC *out)
{
   int i,j,k;

   real ***te;
   real **me;

   /* test compatibilites */
   if ( !tensor )  error(E_NULL,"tsm_mlt");
   
   if ( tensor->l != out->dim )
   {
      error(E_SIZES,"tsm_mlt");
   }
   if ( (tensor->m != mat->m) || (tensor->n != mat->n) )
   {
      error(E_SIZES,"tsm_mlt");
   }

   te = tensor->te;
   me = mat->me;

   /* initialisation */
   v_zero(out);
   
   /* multiplication */
   for (k=0; k<tensor->n; k++)
   {
      for (i=0; i<mat->m; i++)
      for (j=0; j<mat->n; j++)
      {
         out->ve[k] += te[i][j][k]*me[i][j]; 
      }
   }

   return out;
} 

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *tsv_mlt1(const TENSOR *tensor, const VEC *vec, MAT *out)
{
   /* We multiply the 1st componant of a TENSOR with a vector */
   
   int i,j,k;
   
   real ***te;
   real *ve;
   
   /* test compatibilites */
   if ( !tensor )                                         error(E_NULL ,"tsv_mlt1");
   if ( tensor->l != vec->dim )                           error(E_SIZES,"tsv_mlt1");
   if ( (tensor->m != out->m) || (tensor->n != out->n) )  error(E_SIZES,"tsv_mlt1");
   
   te = tensor->te;
   ve = vec->ve;
   
   /* initialisation */
   m_zero(out);

   /* multiplication */
   for (j=0; j<out->m; j++)
   for (k=0; k<out->n; k++)
   {
      for (i=0; i<tensor->l; i++)
      {
         out->me[j][k] += te[i][j][k] * ve[i]; 
      }
   }
   
   return out;
} 

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *tsv_mlt2(const TENSOR *tensor, const VEC *vec, MAT *out)
{
   /* We multiply the 2nd componant of a TENSOR with a vector */
   
   int i,j,k;
   
   real ***te;
   real *ve;
   
   /* test compatibilites */
   if ( !tensor )                                         error(E_NULL ,"tsv_mlt2");
   if ( tensor->m != vec->dim )                           error(E_SIZES,"tsv_mlt2");
   if ( (tensor->l != out->m) || (tensor->n != out->n) )  error(E_SIZES,"tsv_mlt2");
   
   te = tensor->te;
   ve = vec->ve;
   
   /* initialisation */
   m_zero(out);

   /* multiplication */
   for (i=0; i<out->m; i++)
   for (k=0; k<out->n; k++)
   {
      for (j=0; j<tensor->m; j++)
      {
         out->me[i][k] += te[i][j][k] * ve[j]; 
      }
   }
   
   return out;
} 

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

TENSOR *sts_mlt(Real scalar, const TENSOR *tensor, TENSOR *out) /* scalar-tensor multiply -- may be in-situ */
{
   u_int	i,j,k, l,m,n;
   
   real ***te;
   
   if ( (tensor==TSNULL)||(out==TSNULL) )   error(E_NULL,"sts_mlt");
   
   l = tensor->l;
   m = tensor->m;
   n = tensor->n;
   
   te = tensor->te;

   for ( i=0; i<l; i++ )
   for ( j=0; j<m; j++ )
   for ( k=0; k<n; k++ )
      out->te[i][j][k] = scalar * te[i][j][k];
         
   return (out);
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT *tensor_product(const VEC *U, const VEC *V, MAT *UV)
{
   int i,j;
   /* tensor product between 2 vectors, results a matrix */
   
   if ( UV==(MAT *)NULL || U==(VEC *)NULL || V==(VEC *)NULL )   error(E_NULL ,"tensor_product");
   if ( (U->dim != UV->m) || (V->dim != UV->n ) )               error(E_SIZES,"tensor_product");
   
   for (i=0; i<U->dim; i++)
   for (j=0; j<V->dim; j++)
   {
      UV->me[i][j] = U->ve[i]*V->ve[j];
   }
   
   return UV;
}

/* --------------------------------------------------------------- */
/* --------------------------------------------------------------- */

VEC*  v_sadd(real s, const VEC* in, VEC* out)
{
   int i;
   
   if ( in  == VNULL ) error(E_NULL,"v_sadd");
   if ( out == VNULL ) error(E_NULL,"v_sadd");
   
   if ( (in->dim != out->dim) ) error(E_SIZES,"v_sadd");
   
   for ( i = 0; i < in->dim; i++ )
   { 
      out->ve[i] = in->ve[i] + s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC*  v_ssub(real s, const VEC* in, VEC* out)
{
   int i;
   
   if ( in  == VNULL ) error(E_NULL,"v_ssub");
   if ( out == VNULL ) error(E_NULL,"v_ssub");
   
   if ( (in->dim != out->dim) ) error(E_SIZES,"v_ssub");
   
   for ( i = 0; i < in->dim; i++ )
   { 
      out->ve[i] = in->ve[i] - s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IVEC* iv_ones(IVEC* in)
{
   int i;
   
   if ( in  == IVNULL ) error(E_NULL,"iv_ones");
   
   for ( i = 0; i < in->dim; i++ )
   { 
      in->ive[i] = 1 ;
   }
   
   return in;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IVEC* iv_rand(IVEC* in)
{
   int  i;
   VEC  *vec;

   if ( in  == IVNULL ) error(E_NULL,"iv_rand");
   
   vec = v_get(in->dim);
   v_rand(vec);
   
   vec = sv_mlt(100.0, vec, vec);
      
   for ( i = 0; i < in->dim; i++ )
   { 
      in->ive[i] = (int)vec->ve[i] ;
   }
   
   return in;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IVEC* iv_sadd(int s, const IVEC* in, IVEC* out)
{
   int i;
   
   if ( in  == IVNULL ) error(E_NULL,"iv_sadd");
   if ( out == IVNULL ) error(E_NULL,"iv_sadd");
   
   if ( (in->dim != out->dim) ) error(E_SIZES,"iv_sadd");
   
   for ( i = 0; i < in->dim; i++ )
   { 
      out->ive[i] = in->ive[i] + s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IVEC* iv_ssub(int s, const IVEC* in, IVEC* out)
{
   int i;
   
   if ( in  == IVNULL ) error(E_NULL,"iv_ssub");
   if ( out == IVNULL ) error(E_NULL,"iv_ssub");
   
   if ( (in->dim != out->dim) ) error(E_SIZES,"iv_ssub");
   
   for ( i = 0; i < in->dim; i++ )
   { 
      out->ive[i] = in->ive[i] - s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IVEC* iv_smlt(int s, const IVEC* in, IVEC* out)
{
   int i;
   
   if ( in  == IVNULL ) error(E_NULL,"iv_ssub");
   if ( out == IVNULL ) error(E_NULL,"iv_ssub");
   
   if ( (in->dim != out->dim) ) error(E_SIZES,"iv_ssub");
   
   for ( i = 0; i < in->dim; i++ )
   { 
      out->ive[i] = in->ive[i] * s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT*  m_sadd(real s, const MAT* in, MAT* out)
{
   int i,j;
   
   if ( in  == MNULL ) error(E_NULL,"m_sadd");
   if ( out == MNULL ) error(E_NULL,"m_sadd");
   
   if ( (in->m != out->m) ) error(E_SIZES,"m_sadd");
   if ( (in->n != out->n) ) error(E_SIZES,"m_sadd");
   
   for ( i = 0; i < in->m; i++ )
   for ( j = 0; j < in->n; j++ )
   { 
      out->me[i][j] = in->me[i][j] + s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT*  m_ssub(real s, const MAT* in, MAT* out)
{
   int i,j;
   
   if ( in  == MNULL ) error(E_NULL,"m_ssub");
   if ( out == MNULL ) error(E_NULL,"m_ssub");
   
   if ( (in->m != out->m) ) error(E_SIZES,"m_ssub");
   if ( (in->n != out->n) ) error(E_SIZES,"m_ssub");
   
   for ( i = 0; i < in->m; i++ )
   for ( j = 0; j < in->n; j++ )
   { 
      out->me[i][j] = in->me[i][j] - s ;
   }
   
   return out;
}

/*
real  in_prod(VEC* v1, VEC *v2)
{
   return _in_prod(v1,v2, 0);
}
*/

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

real m_star(const MAT* m1, const MAT* m2)
{
   int i,j;
   real out = 0.0;

   real **m1me;
   real **m2me;

   if ( m1 == MNULL ) error(E_NULL,"m_star");
   if ( m2 == MNULL ) error(E_NULL,"m_star");

   if ( (m1->m != m2->m) ) error(E_SIZES,"m_star");
   if ( (m1->n != m2->n) ) error(E_SIZES,"m_star");

   m1me = m1->me;
   m2me = m2->me;

   for ( i = 0; i < m1->m; i++ )
   for ( j = 0; j < m1->n; j++ )
   {
      out += m1me[i][j]*m2me[i][j];
   }

   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT*  im_sadd(int s, const IMAT* in, IMAT* out)
{
   int i,j;
   
   if ( in  == IMNULL ) error(E_NULL,"im_sadd");
   if ( out == IMNULL ) error(E_NULL,"im_sadd");
   
   if ( (in->m != out->m) ) error(E_SIZES,"im_sadd");
   if ( (in->n != out->n) ) error(E_SIZES,"im_sadd");
   
   for ( i = 0; i < in->m; i++ )
   for ( j = 0; j < in->n; j++ )
   { 
      out->im[i][j] = in->im[i][j]  + s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

IMAT*  im_ssub(int s, const IMAT* in, IMAT* out)
{
   int i,j;
   
   if ( in  == IMNULL ) error(E_NULL,"im_ssub");
   if ( out == IMNULL ) error(E_NULL,"im_ssub");
   
   if ( (in->m != out->m) ) error(E_SIZES,"im_ssub");
   if ( (in->n != out->n) ) error(E_SIZES,"im_ssub");
   
   for ( i = 0; i < in->m; i++ )
   for ( j = 0; j < in->n; j++ )
   { 
      out->im[i][j] = in->im[i][j]  - s ;
   }
   
   return out;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

MAT* m_lincomb_transform( MAT* inout, const MAT *lincomb_coeffs)
{
   int i,j,k,l;
   MAT * tmp ;

   /* check. the matrix must be carree */
   if ( inout==(MAT *)NULL )               error(E_NULL ,"m_lincomb_transform");
   if ( lincomb_coeffs==(MAT *)NULL   )    error(E_NULL ,"m_lincomb_transform");
   if ( inout->m != lincomb_coeffs->m )    error(E_SIZES,"m_lincomb_transform");
   if ( inout->n != lincomb_coeffs->n )    error(E_SIZES,"m_lincomb_transform");
   if ( inout->m != lincomb_coeffs->n )    error(E_SIZES,"m_lincomb_transform");

   /* alloc mem */
   tmp = m_get(inout->m, inout->n);

   /* process */
   for (i=0; i<inout->m; i++)
   for (j=0; j<inout->n; j++)
   {
      tmp->me[i][j] = 0.0; /* init */

      for (k=0; k<inout->m; k++)
      for (l=0; l<inout->n; l++)
      {
         tmp->me[i][j] += lincomb_coeffs->me[i][k]*lincomb_coeffs->me[j][l]*inout->me[k][l] ;
               /*
                * ex :  b12 = NewPhi_1*NewPhi_2
                *           = (k10.Phi0 + k11.Phi1 + k12.Phi2)*
                *             (k20.Phi0 + k21.Phi1 + k22.Phi2)
                *           = k10.k20.Phi0*Phi0     + ...      ( k10.k20.a00 )
                *             k11.k20.Phi1*Phi0     + ...      ( k11.k20.a10 )
                *             k12.k20.Phi2*Phi0     + ...      ( k12.k20.a20 )
                *             k10.k21.Phi0*Phi1     + ...      ( k10.k21.a01 )
                *             k11.k21.Phi1*Phi1     + ...      ( k11.k21.a11 )
                *             k12.k21.Phi2*Phi1     + ...      ( k12.k21.a21 )
                *             k10.k22.Phi0*Phi2     + ...      ( k10.k22.a02 )
                *             k11.k22.Phi1*Phi2     + ...      ( k11.k22.a12 )
                *             k12.k22.Phi2*Phi2                ( k12.k22.a22 )
                *
                */
      }
   }

   inout = m_copy(tmp, inout);

   /* free mem */
   M_FREE(tmp);

   /* finito */
   return inout;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */
