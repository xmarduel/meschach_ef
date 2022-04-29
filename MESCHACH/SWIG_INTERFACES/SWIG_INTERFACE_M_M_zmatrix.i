
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
///////////////////////////////////////////////////////// 

#ifndef ZMATRIXH
#define ZMATRIXH

#include "matrix.h"


          /*  Type definitions for complex vectors and matrices  */


/* complex definition */
//typedef struct  {
//                Real re,im;
//        } complex;

/* complex vector definition */
typedef struct  {
                %immutable;
                u_int   dim;
                %mutable;
//              u_int max_dim;
//              complex  *ve;
                } ZVEC;

/* complex matrix definition */
typedef struct  {
                %immutable;
                u_int   m, n;
                %mutable;
//              u_int   max_m, max_n, max_size;
//              complex *base;          /* base is base of alloc'd mem */
//              complex **me;
                } ZMAT;

#define ZVNULL  ((ZVEC *)NULL)
#define ZMNULL  ((ZMAT *)NULL)

#define	Z_CONJ		1
#define	Z_NOCONJ	0


/* memory functions */

//int zv_get_vars(int dim,...);
//int zm_get_vars(int m,int n,...);
//int zv_resize_vars(int new_dim,...);
//int zm_resize_vars(int m,int n,...);
//int zv_free_vars(ZVEC **,...);
//int zm_free_vars(ZMAT **,...);


//extern ZMAT	*_zm_copy(ZMAT *in,ZMAT *out,u_int i0,u_int j0);
//extern ZMAT	* zm_move(ZMAT *, int, int, int, int, ZMAT *, int, int);
//extern ZMAT	*zvm_move(ZVEC *, int, ZMAT *, int, int, int, int);
//extern ZVEC	*_zv_copy(ZVEC *in,ZVEC *out,u_int i0);
//extern ZVEC	* zv_move(ZVEC *, int, int, ZVEC *, int);
//extern ZVEC	*zmv_move(ZMAT *, int, int, int, int, ZVEC *, int);
//extern complex	z_finput(FILE *fp);
//extern ZMAT	*zm_finput(FILE *fp,ZMAT *a);
//extern ZVEC *zv_finput(FILE *fp,ZVEC *x);
//extern ZMAT	*zm_add(ZMAT *mat1,ZMAT *mat2,ZMAT *out);
//extern ZMAT	*zm_sub(ZMAT *mat1,ZMAT *mat2,ZMAT *out);
//extern ZMAT	*zm_mlt(ZMAT *A,ZMAT *B,ZMAT *OUT);
//extern ZMAT	*zmma_mlt(ZMAT *A,ZMAT *B,ZMAT *OUT);
//extern ZMAT	*zmam_mlt(ZMAT *A,ZMAT *B,ZMAT *OUT);
//extern ZVEC	*zmv_mlt(ZMAT *A,ZVEC *b,ZVEC *out);
//extern ZMAT	*zsm_mlt(complex scalar,ZMAT *matrix,ZMAT *out);
//extern ZVEC	*zvm_mlt(ZMAT *A,ZVEC *b,ZVEC *out);
//extern ZMAT	*zm_adjoint(ZMAT *in,ZMAT *out);
//extern ZMAT	*zswap_rows(ZMAT *A,int i,int j,int lo,int hi);
//extern ZMAT	*zswap_cols(ZMAT *A,int i,int j,int lo,int hi);
//extern ZMAT	*mz_mltadd(ZMAT *A1,ZMAT *A2,complex s,ZMAT *out);
//extern ZVEC	*zmv_mltadd(ZVEC *v1,ZVEC *v2,ZMAT *A,complex alpha,ZVEC *out);
//extern ZVEC	*zvm_mltadd(ZVEC *v1,ZVEC *v2,ZMAT *A,complex alpha,ZVEC *out);
//extern ZVEC	*zv_zero(ZVEC *x);
//extern ZMAT	*zm_zero(ZMAT *A);
//extern ZMAT	*zm_get(int m,int n);
//extern ZVEC	*zv_get(int dim);
//extern ZMAT	*zm_resize(ZMAT *A,int new_m,int new_n);
//extern complex	_zin_prod(ZVEC *x,ZVEC *y,u_int i0,u_int flag);
//extern ZVEC	*zv_resize(ZVEC *x,int new_dim);
//extern ZVEC	*zv_mlt(complex scalar,ZVEC *vector,ZVEC *out);
//extern ZVEC	*zv_add(ZVEC *vec1,ZVEC *vec2,ZVEC *out);
//extern ZVEC	*zv_mltadd(ZVEC *v1,ZVEC *v2,complex scale,ZVEC *out);
//extern ZVEC	*zv_sub(ZVEC *vec1,ZVEC *vec2,ZVEC *out);

//extern ZVEC	*zv_map(complex (*f)(complex),ZVEC *x,ZVEC *out);
//extern ZVEC	*_zv_map(complex (*f)(void *,complex),void *params,ZVEC *x,ZVEC *out);

//extern ZVEC	*zv_lincomb(int n,ZVEC *v[],complex a[],ZVEC *out);
//extern ZVEC	*zv_linlist(ZVEC *out,ZVEC *v1,complex a1,...);
//extern ZVEC	*zv_star(ZVEC *x1, ZVEC *x2, ZVEC *out);
//extern ZVEC	*zv_slash(ZVEC *x1, ZVEC *x2, ZVEC *out);
//extern int	zm_free(ZMAT *mat);
//extern int	zv_free(ZVEC *vec);

//extern ZVEC	*zv_rand(ZVEC *x);
//extern ZMAT	*zm_rand(ZMAT *A);

//extern ZVEC	*zget_row(ZMAT *A, int i, ZVEC *out);
//extern ZVEC	*zget_col(ZMAT *A, int j, ZVEC *out);
//extern ZMAT	*zset_row(ZMAT *A, int i, ZVEC *in);
//extern ZMAT	*zset_col(ZMAT *A, int j, ZVEC *in);

//extern ZVEC	*px_zvec(PERM *pi, ZVEC *in, ZVEC *out);
//extern ZVEC	*pxinv_zvec(PERM *pi, ZVEC *in, ZVEC *out);

//extern void	__zconj__(complex zp[], int len);
//extern complex	__zip__(complex zp1[],complex zp2[],int len,int flag);
//extern void	__zmltadd__(complex zp1[],complex zp2[],
//			    complex s,int len,int flag);
//extern void	__zmlt__(complex zp[],complex s,complex out[],int len);
//extern void	__zadd__(complex zp1[],complex zp2[],complex out[],int len);
//extern void	__zsub__(complex zp1[],complex zp2[],complex out[],int len);
//extern void	__zzero__(complex zp[],int len);
//extern void	z_foutput(FILE *fp,complex z);
//extern void     zm_foutput(FILE *fp,ZMAT *a);
//extern void     zv_foutput(FILE *fp,ZVEC *x);


//extern void     zm_dump(FILE *fp,ZMAT *a);
//extern void     zv_dump(FILE *fp,ZVEC *x);

//extern complex  zv_sum(ZVEC* x);
//extern double	_zv_norm1(ZVEC *x, VEC *scale);
//extern double	_zv_norm2(ZVEC *x, VEC *scale);
//extern double	_zv_norm_inf(ZVEC *x, VEC *scale);
//extern double	zm_norm1(ZMAT *A);
//extern double	zm_norm_inf(ZMAT *A);
//extern double	zm_norm_frob(ZMAT *A);

/* from 'zmatio' */
//extern  ZMAT     *bzm_finput(FILE *fp,ZMAT *mat);
//extern  ZMAT     *izm_finput(FILE *fp,ZMAT *mat);
//extern  ZVEC     *izv_finput(FILE *fp,ZVEC *vec);
//extern  ZVEC     *bzv_finput(FILE *fp,ZVEC *vec);
//extern  char	*setzformat(char *f_string);


//complex	zmake(double real, double imag);
//double	zabs(complex z);
//complex zadd(complex z1,complex z2);
//complex zsub(complex z1,complex z2);
//complex	zmlt(complex z1,complex z2);
//complex	zinv(complex z);
//complex	zdiv(complex z1,complex z2);
//complex	zsqrt(complex z);
//complex	zexp(complex z);
//complex	zlog(complex z);
//complex	zconj(complex z);
//complex	zneg(complex z);
//void	zgivens(complex x,complex y,Real *c,complex *s);





//#define	zv_copy(x,y)	_zv_copy(x,y,0)
//#define	zm_copy(A,B)	_zm_copy(A,B,0,0)

//#define	z_input()	z_finput(stdin)
//#define	zv_input(x)	zv_finput(stdin,x)
//#define	zm_input(A)	zm_finput(stdin,A)
//#define	z_output(z)	z_foutput(stdout,z)
//#define	zv_output(x)	zv_foutput(stdout,x)
//#define	zm_output(A)	zm_foutput(stdout,A)

//#define	ZV_FREE(x)	( zv_free(x), (x) = ZVNULL )
//#define	ZM_FREE(A)	( zm_free(A), (A) = ZMNULL )

//#define	zin_prod(x,y)	_zin_prod(x,y,0,Z_CONJ)

//#define	zv_norm1(x)	_zv_norm1(x,VNULL)
//#define	zv_norm2(x)	_zv_norm2(x,VNULL)
//#define	zv_norm_inf(x)	_zv_norm_inf(x,VNULL)


#endif
