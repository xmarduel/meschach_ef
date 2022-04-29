
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_ADDS
//
/////////////////////////////////////////////////////////
 
#ifndef MATRIX_ADDS_H
#define MATRIX_ADDS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "matrix.h"
#include "sparse.h"

#define MY_LIST1     1

#define TYPE_IMAT    0
#define TYPE_TENSOR  1

#define NB_STRUCTS1  2



int mem_attach_list1(void);

typedef struct  {
   %immutable;
   u_int   m, n;
   %mutable;
//   u_int   max_m, max_n, max_size;
//   int     **im , *ibase;   /* ibase is base of alloc'd mem */
      } IMAT;

typedef struct  {
   %immutable;
   u_int   l,m,n;
   %mutable;
//   u_int   max_l, max_m, max_n, max_size;
//   Real    ***te;           /* pointer to matrix element */
//   Real    *base, **me;     /* additional pointers */
      } TENSOR;



#define IMNULL  ((IMAT*)NULL)
#define TSNULL  ((TENSOR*)NULL)

int im_free(IMAT *imat) ;
int ts_free(TENSOR *tnsr) ;

/* MACROS */

#define IM_FREE(imat)  ( im_free(imat),  (imat)=(IMAT *)NULL )
#define TS_FREE(tnsr)  ( ts_free(tnsr),  (tnsr)=(TENSOR *)NULL )

#define im_input(im)     xim_finput(stdout,im)
#define ts_input(ts)      ts_finput(stdout,ts)

#define im_output(im)     im_foutput(stdout,im)
#define ts_output(ts)     ts_foutput(stdout,ts)

#define	im_copy(in,out)	  _im_copy(in,out,0,0)
#define	ts_copy(in,out)   _ts_copy(in,out,0,0,0)


/***************************************************************/


/* -------------   New functions on usual structs ----*/

VEC*  v_sadd(real s, const VEC* in, VEC* out);
VEC*  v_ssub(real s, const VEC* in, VEC* out);


IVEC* iv_ones(IVEC* in);
IVEC* iv_rand(IVEC* in);
IVEC* iv_sadd(int s, const IVEC* in, IVEC* out);
IVEC* iv_ssub(int s, const IVEC* in, IVEC* out);
IVEC* iv_smlt(int s, const IVEC* in, IVEC* out);


MAT*  m_sadd(real s, const MAT* in, MAT* out);
MAT*  m_ssub(real s, const MAT* in, MAT* out);



/* -------------   IMAT functions --------------------*/

/* manage memory --------------------------- */
IMAT *im_get(int m, int n);
IMAT *im_resize(IMAT *A, int new_m, int new_n);

/* Entrees-Sorties ------------------------- */
void    im_dump    (FILE *fp, IMAT *a);
IMAT*   xim_finput (FILE *fp, IMAT *a);
IMAT*   iim_finput (FILE *fp, IMAT *a); /* interactive */
IMAT*   bim_finput (FILE *fp, IMAT *a); /* batch       */
void    im_foutput (FILE *fp, IMAT *a);


/* initialisations ------------------------- */
IMAT *im_zero(IMAT *A);
IMAT *im_ident(IMAT *A);
IMAT *im_rand(IMAT *A);
IMAT *im_ones(IMAT *A);

IMAT *im_transp(IMAT* in, IMAT* out);

/* get the data ---------------------------- */
//int   im_get_val(IMAT *,int,int);
//IMAT *im_set_val(IMAT *,int,int,int);
IMAT * _im_copy(IMAT *A, IMAT *out, int i0, int j0);
/* IMAT *  im_copy(IMAT *A, IMAT *out); */ /* as MACRO */

/* operations ------------------------------ */
IMAT *im_add(const IMAT *A, const IMAT *B, IMAT *out);
IMAT *im_sub(const IMAT *A, const IMAT *B, IMAT *out);
IMAT *im_mlt(const IMAT *A, const IMAT *B, IMAT *out);

IMAT *im_smlt(const IMAT *A, int  n,  IMAT *out);
IVEC *im_vmlt(const IMAT *A, IVEC *B, IVEC *out);

IMAT*  im_sadd(int s, const IMAT* in, IMAT* out);
IMAT*  im_ssub(int s, const IMAT* in, IMAT* out);

/* --------------- Operations sur les TENSOR --------------- */

/* manage memory --------------------------- */
TENSOR *ts_get(int l, int m, int n); 

/* Entrees-Sorties ------------------------- */
void    ts_dump   (FILE *fp, TENSOR *a);
TENSOR*  ts_finput (FILE *fp, TENSOR *a);
//TENSOR*  its_finput(FILE *fp, TENSOR *a); /* interactive */
//TENSOR*  bts_finput(FILE *fp, TENSOR *a); /* batch       */
void    ts_foutput(FILE *fp, TENSOR *a);

/* initialisations ------------------------- */
TENSOR *ts_zero(TENSOR *A);
TENSOR *ts_ident(TENSOR *A);
TENSOR* ts_rand(TENSOR *A);

/* get the data ---------------------------- */
//Real  ts_get_val(TENSOR *,int,int,int);
//TENSOR *ts_set_val(TENSOR *,int,int,int,Real);
TENSOR * _ts_copy(TENSOR *A, TENSOR *out, int i0, int j0 , int k0 );
/* TENSOR *ts_copy(TENSOR *A, TENSOR *out); */ /* as MACRO */

/* operations on TENSOR ---------------------- */
TENSOR *ts_add(const TENSOR *A, const TENSOR *B, TENSOR *out);
TENSOR *ts_sub(const TENSOR *A, const TENSOR *B, TENSOR *out);

VEC   *tsm_mlt(const TENSOR *tensor, const MAT *mat, VEC *out);
MAT   *tsv_mlt1(const TENSOR *tensor, const VEC *vec, MAT *out);
MAT   *tsv_mlt2(const TENSOR *tensor, const VEC *vec, MAT *out);
TENSOR *sts_mlt(Real scalar, const TENSOR *in, TENSOR *out);

/* tensor product between two vectors */
MAT *tensor_product(const VEC *U, const VEC *V, MAT *UV);

/* trandformation of a matrix with a lincomb of coefficients */
MAT* m_lincomb_transform(MAT* inout, const MAT *lincomb_coeffs);

/* Entry level access to data structures */
#ifdef DEBUG

/* returns A[i][j] */
#define	im_get_val(A,i,j)	(((i) < 0 || (i) >= (A)->m || (j) < 0 || (j) >= (A)->n) ? \
			 error(E_BOUNDS,"im_entry"), 0.0 : (A)->im[i][j] )

/* A[i][j] <- val */
#define	im_set_val(A,i,j,val) ((A)->im[i][j] = ((i) < 0 || (i) >= (A)->m || (j) < 0 || (j) >= (A)->n) ? \
                         error(E_BOUNDS,"im_set_val"), 0.0 : (val) )

/* returns A[i][j][k] */
#define	ts_get_val(A,i,j,k)	(((i) < 0 || (i) >= (A)->l || (j) < 0 || (j) >= (A)->m) || (k) < 0 || (k) >= (A)->n) ? \
			 error(E_BOUNDS,"ts_entry"), 0.0 : (A)->te[i][j][k] )

/* A[i][j][k] <- val */
#define	ts_set_val(A,i,j,k,val) ((A)->te[i][j][k] = ((i) < 0 || (i) >= (A)->l || (j) < 0 || (j) >= (A)->m) || (k) < 0 || (k) >= (A)->n) ? \
                         error(E_BOUNDS,"ts_set_val"), 0.0 : (val) )

#else


/* returns A[i][j] */
#define	im_get_val(A,i,j)       ( (A)->im[i][j] ) )

/* A[i][j] <- val */
#define	im_set_val(A,i,j,val)	( (A)->im[i][j]  = (val) )

/* returns A[i][j][k] */
#define	ts_get_val(A,i,j,k)     ( (A)->te[i][j][k] ) )

/* A[i][j][k] <- val */
#define	ts_set_val(A,i,j,k,val) ( (A)->te[i][j][k] = (val) )

#endif



/**********************************************************************/
//static char *my_names1[] = { "IMAT"   ,
//                             "TENSOR"  };

//static int (*my_frees1[])(void *) = { (int (*)(void *))im_free    ,
//                                      (int (*)(void *))ts_free    };

//static MEM_ARRAY my_tnums1[NB_STRUCTS1]; /* initialised to zeros */
/**********************************************************************/



//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{

#define IM_FREE(imat)  ( im_free(imat),  (imat)=(IMAT *)NULL )
#define TS_FREE(tnsr)  ( ts_free(tnsr),  (tnsr)=(TENSOR *)NULL )

#undef  IM_FREE
#undef  TS_FREE

void IM_FREE(IMAT* imat)
{ 
   im_free(imat);
   (imat)=(IMAT *)NULL;
}
void TS_FREE(TENSOR* tnsr)
{ 
   ts_free(tnsr);
  (tnsr)=(TENSOR *)NULL;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int     im_dim1(IMAT *A)
{
   if ( A == IMNULL ) error(E_NULL,"im_dim2"); return A->m;
}

int     im_dim2(IMAT *A)
{
   if ( A == IMNULL ) error(E_NULL,"im_dim2"); return A->n;
}

int   v_dim(VEC* v)
{
   if ( v  == VNULL ) error(E_NULL,"v_dim"); return v->dim;
}

int   iv_dim(IVEC* in)
{
   if ( in  == IVNULL ) error(E_NULL,"iv_dim"); return in->dim;
}

int     m_dim1(const MAT *A)
{
   if ( A  == MNULL ) error(E_NULL,"m_dim1");  return A->m;
}

int     m_dim2(const MAT *A)
{
   if ( A  == MNULL ) error(E_NULL,"m_dim2");  return A->n;
}

int px_size(PERM* px)
{
   if ( px  == PNULL ) error(E_NULL,"px_size"); return px->size;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

int im_check(IMAT* imat)
{
   return 1; //if ( ! imat ) error(E_NULL,"im_check");
}
int iv_check(IVEC* ivec)
{
   return 1; //if ( ! imat ) error(E_NULL,"iv_check");
}
int m_check(MAT* imat)
{
   return 1; //if ( ! imat ) error(E_NULL,"m_check");
}
int v_check(VEC* vec)
{
   return 1; //if ( ! imat ) error(E_NULL,"v_check");
}
int px_check(PERM* px)
{
   return 1; //if ( ! imat ) error(E_NULL,"px_check");
}


%}

/* returns A[i][j] */
//#define	im_get_val(A,i,j) ( (A)->im[i][j] ) )
%inline %{
#undef im_get_val
int im_get_val(IMAT *A, int i, int j)
{
	return ( A->im[i][j] );
}

/* A[i][j] <- val */
//#define	im_set_val(A,i,j,val)	((A)->im[i][j]  = (val) )
#undef im_set_val
void im_set_val(IMAT *A, int i, int j, int val)
{
	A->im[i][j]  = val ;
}

%}

%inline %{

#undef im_copy
IMAT *im_copy(IMAT *A, IMAT *out)
{
	return _im_copy(A, out, 0,0) ;
}


/* im_null -- gets an NULL integer matrix (for python)   */
IMAT   *im_null(void)
{
  return (IMAT*)NULL;
}



/* returns A[i][j][k] */
#undef ts_entry
real ts_entry(TENSOR *A, int i, int j, int k)
{
  return ( A->te[i][j][k]  );
}

/* A[i][j][k] <- val */
#undef ts_set_val
void ts_set_val( TENSOR *A, int i, int j, int k, real val)
{
  A->te[i][j][k] = val ;
}


/* ts_null -- gets a NULL Tensor (for python)   */
TENSOR   *ts_null(void)
{
  TENSOR  *tensor = TSNULL;
  return tensor;
}

%}



#ifdef __cplusplus
}
#endif

#endif
