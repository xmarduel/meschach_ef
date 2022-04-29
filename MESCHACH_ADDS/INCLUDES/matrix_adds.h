
#ifndef MATRIX_ADDS_H
#define MATRIX_ADDS_H

/** \file matrix_adds.h
 *
 * extentions to MESCHACH LIBRARY for usual structs 
 *
 * ------------------------------------------------------------------------- */

#ifdef __cplusplus
extern "C"
{
#endif

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#define MY_LIST1     1

#define TYPE_IMAT    0
#define TYPE_TENSOR  1

#define NB_STRUCTS1  2



int mem_attach_list1(void);

typedef struct  {
   u_int   m, n;
   u_int   max_m, max_n, max_size;
   int     **im , *ibase;   /* ibase is base of alloc'd mem */
      } IMAT;

typedef struct  {
   u_int   l,m,n;
   u_int   max_l, max_m, max_n, max_size;
   Real    ***te;           /* pointer to tensor element */
   Real    *base, **me;     /* additional pointers */
      } TENSOR;




#define IMNULL  ((IMAT*)NULL)
#define TSNULL  ((TENSOR*)NULL)

int im_free(IMAT *imat) ;
int ts_free(TENSOR *tensor) ;

/* MACROS */

#define IM_FREE(imat)  ( im_free(imat),  (imat)=(IMAT *)NULL )
#define TS_FREE(tnsr)  ( ts_free(tnsr),  (tnsr)=(TENSOR *)NULL )

#define im_input(im)     xim_finput(stdout,im)
#define ts_input(tnsr)   ts_finput(stdout,TENSOR)

#define im_output(im)     im_foutput(stdout,im)
#define m3_output(TENSOR)  m3_foutput(stdout,TENSOR)

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

real m_star(const MAT* m1, const MAT* m2);

/* tensor product between two vectors */
MAT *tensor_product(const VEC *U, const VEC *V, MAT *UV);

/* trandformation of a matrix with a lincomb of coefficients */
MAT* m_lincomb_transform( MAT* inout, const MAT *lincomb_coeffs);

/* -------------   IMAT functions --------------------*/

/* manage memory --------------------------- */
IMAT *im_get(int m, int n);
IMAT *im_resize(IMAT *A, int new_m, int new_n);

/* Entrees-Sorties ------------------------- */
void    im_dump    (FILE *fp, const IMAT *a);
void    im_foutput (FILE *fp, const IMAT *a);

IMAT*   xim_finput (FILE *fp, IMAT *a);
IMAT*   iim_finput (FILE *fp, IMAT *a); /* interactive */
IMAT*   bim_finput (FILE *fp, IMAT *a); /* batch       */

/* initialisations ------------------------- */
IMAT *im_zero(IMAT *A);
IMAT *im_ident(IMAT *A);
IMAT *im_rand(IMAT *A);
IMAT *im_ones(IMAT *A);

IMAT *im_transp(const IMAT* in, IMAT* out);

/* get the data ---------------------------- */
/*int   im_get_val(IMAT *,int,int);*/
/*IMAT *im_set_val(IMAT *,int,int,int);*/
IMAT * _im_copy(const IMAT *A, IMAT *out, int i0, int j0);
/* IMAT *  im_copy(const IMAT *A, IMAT *out); */ /* as MACRO */

/* operations ------------------------------ */
IMAT *im_add(const IMAT *A, const IMAT *B, IMAT *out);
IMAT *im_sub(const IMAT *A, const IMAT *B, IMAT *out);
IMAT *im_mlt(const IMAT *A, const IMAT *B, IMAT *out);

IMAT *im_sadd(int s, const IMAT* in, IMAT* out);
IMAT *im_ssub(int s, const IMAT* in, IMAT* out);

IMAT *im_smlt(const IMAT *A, int  n,        IMAT *out);
IVEC *im_vmlt(const IMAT *A, const IVEC *B, IVEC *out);


/* ---------------  TENSOR functions --------------- */

/* manage memory --------------------------- */
TENSOR *ts_get(int l, int m, int n); 

/* Entrees-Sorties ------------------------- */
void    ts_dump   (FILE *fp, const TENSOR *a);
void    ts_foutput(FILE *fp, const TENSOR *a);

TENSOR*  ts_finput (FILE *fp, TENSOR *a);
/*TENSOR*  its_finput(FILE *fp, TENSOR *a); */ /* interactive */
/*TENSOR*  bts_finput(FILE *fp, TENSOR *a); */ /* batch       */

/* initialisations ------------------------- */
TENSOR *ts_zero(TENSOR *A);
TENSOR *ts_ident(TENSOR *A);
TENSOR* ts_rand(TENSOR *A);

/* get the data ---------------------------- */
/*Real  ts_get_val(TENSOR *,int,int,int); */      /* as MACRO */
/*TENSOR *ts_set_val(TENSOR *,int,int,int,Real); */ /* as MACRO */
TENSOR * _ts_copy(const TENSOR *A, TENSOR *out, int i0, int j0 , int k0 );
/* TENSOR *ts_copy(const TENSOR *A, TENSOR *out); */ /* as MACRO */

/* operations on TENSOR ---------------------- */
TENSOR *ts_add(const TENSOR *A, const TENSOR *B, TENSOR *out);
TENSOR *ts_sub(const TENSOR *A, const TENSOR *B, TENSOR *out);

VEC    *tsm_mlt(const TENSOR *tensor, const MAT *mat, VEC *out);
MAT    *tsv_mlt1(const TENSOR *tensor, const VEC *vect,  MAT *out);
MAT    *tsv_mlt2(const TENSOR *tensor, const VEC *vect,  MAT *out);
TENSOR *sts_mlt(Real scalar, const TENSOR *in, TENSOR *out);



/* Entry level access to data structures */
#ifdef DEBUG

/* returns A[i][j] */
#define	im_get_val(A,i,j)	(((i) < 0 || (i) >= (A)->m ||  (j) < 0 || (j) >= (A)->n) ? \
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
static char *my_names1[NB_STRUCTS1] = { "IMAT"   ,
                                        "TENSOR"  };

static int (*my_frees1[NB_STRUCTS1])(void *) = { (int (*)(void *))im_free    ,
                                                 (int (*)(void *))ts_free    };

static MEM_ARRAY my_tnums1[NB_STRUCTS1]; /* initialised to zeros */
/**********************************************************************/


#ifdef __cplusplus
}
#endif

#endif
