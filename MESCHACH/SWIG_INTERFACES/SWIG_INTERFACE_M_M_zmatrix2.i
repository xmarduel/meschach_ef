
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
///////////////////////////////////////////////////////// 
 
#ifndef ZMATRIX2H
#define ZMATRIX2H

#include "zmatrix.h"

//extern ZVEC	*zUsolve(ZMAT *matrix, ZVEC *b, ZVEC *out, double diag);
//extern ZVEC	*zLsolve(ZMAT *matrix, ZVEC *b, ZVEC *out, double diag);
//extern ZVEC	*zUAsolve(ZMAT *U, ZVEC *b, ZVEC *out, double diag);
//extern ZVEC	*zDsolve(ZMAT *A, ZVEC *b, ZVEC *x);
//extern ZVEC	*zLAsolve(ZMAT *L, ZVEC *b, ZVEC *out, double diag);

//extern ZVEC	*zhhvec(ZVEC *,int,Real *,ZVEC *,complex *);
//extern ZVEC	*zhhtrvec(ZVEC *,double,int,ZVEC *,ZVEC *);
//extern ZMAT	*zhhtrrows(ZMAT *,int,int,ZVEC *,double);
//extern ZMAT	*zhhtrcols(ZMAT *,int,int,ZVEC *,double);
//extern ZMAT     *zHfactor(ZMAT *,ZVEC *);
//extern ZMAT     *zHQunpack(ZMAT *,ZVEC *,ZMAT *,ZMAT *);

//extern ZMAT	*zQRfactor(ZMAT *A, ZVEC *diag);
//extern ZMAT	*zQRCPfactor(ZMAT *A, ZVEC *diag, PERM *px);
//extern ZVEC	*_zQsolve(ZMAT *QR, ZVEC *diag, ZVEC *b, ZVEC *x, ZVEC *tmp);
//extern ZMAT	*zmakeQ(ZMAT *QR, ZVEC *diag, ZMAT *Qout);
//extern ZMAT	*zmakeR(ZMAT *QR, ZMAT *Rout);
//extern ZVEC	*zQRsolve(ZMAT *QR, ZVEC *diag, ZVEC *b, ZVEC *x);
//extern ZVEC	*zQRAsolve(ZMAT *QR, ZVEC *diag, ZVEC *b, ZVEC *x);
//extern ZVEC	*zQRCPsolve(ZMAT *QR,ZVEC *diag,PERM *pivot,ZVEC *b,ZVEC *x);
//extern ZVEC	*zUmlt(ZMAT *U, ZVEC *x, ZVEC *out);
//extern ZVEC	*zUAmlt(ZMAT *U, ZVEC *x, ZVEC *out);
//extern double	zQRcondest(ZMAT *QR);

//extern ZMAT	*zLUfactor(ZMAT *A, PERM *pivot);
//extern ZVEC	*zLUsolve(ZMAT *A, PERM *pivot, ZVEC *b, ZVEC *x);
//extern ZVEC	*zLUAsolve(ZMAT *LU, PERM *pivot, ZVEC *b, ZVEC *x);
//extern ZMAT	*zm_inverse(ZMAT *A, ZMAT *out);
//extern double	zLUcondest(ZMAT *LU, PERM *pivot);

//extern void	zgivens(complex, complex, Real *, complex *);
//extern ZMAT	*zrot_rows(ZMAT *A, int i, int k, double c, complex s,
//			   ZMAT *out);
//extern ZMAT	*zrot_cols(ZMAT *A, int i, int k, double c, complex s,
//			   ZMAT *out);
//extern ZVEC	*rot_zvec(ZVEC *x, int i, int k, double c, complex s,
//			  ZVEC *out);
//extern ZMAT	*zschur(ZMAT *A,ZMAT *Q);
/* extern ZMAT	*schur_vecs(ZMAT *T,ZMAT *Q,X_re,X_im) */


#endif
