
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
/////////////////////////////////////////////////////////
 
#ifndef MATRIX2H
#define MATRIX2H

#include "matrix.h"

/* Unless otherwise specified, factorisation routines overwrite the
   matrix that is being factorised */


                
extern	MAT	*BKPfactor(MAT *A,PERM *pivot,PERM *blocks),
              
		*CHfactor(MAT *A),
                
                *LUfactor(MAT *A,PERM *pivot),
                
		*QRfactor(MAT *A,VEC *diag),
               
		*QRCPfactor(MAT *A,VEC *diag,PERM *pivot),
               
		*LDLfactor(MAT *A), 
                
                *Hfactor(MAT *A,VEC *diag1,VEC *diag2),
               
                *MCHfactor(MAT *A,double tol),
		*m_inverse(MAT *A,MAT *out);

                
extern	double	LUcondest(MAT *A,PERM *pivot),
                
                QRcondest(MAT *A);


extern	MAT	*makeQ(MAT *A,VEC *diag,MAT *Qout),
               
		*makeR(MAT *A,MAT *Rout),
                
		*makeHQ(MAT *A,VEC *diag1,VEC *diag2,MAT *Qout),
               
		*makeH(MAT *A,MAT *Hout);

                
extern	MAT	*LDLupdate(MAT *A,VEC *u,double alpha),
                
		*QRupdate(MAT *Q,MAT *R,VEC *u,VEC *v);



extern	VEC	*BKPsolve(MAT *A,PERM *pivot,PERM *blocks,VEC *b,VEC *x),
		*CHsolve(MAT *A,VEC *b,VEC *x),
		*LDLsolve(MAT *A,VEC *b,VEC *x),
		*LUsolve(MAT *A,PERM *pivot,VEC *b,VEC *x),
		*_Qsolve(MAT *A,VEC *,VEC *,VEC *, VEC *),
		*QRsolve(MAT *A,VEC *,VEC *b,VEC *x),
    		*QRTsolve(MAT *A,VEC *,VEC *b,VEC *x),


		*Usolve(MAT *A,VEC *b,VEC *x,double diag_val),
		*Lsolve(MAT *A,VEC *b,VEC *x,double diag_val),
		*Dsolve(MAT *A,VEC *b,VEC *x),
		*LTsolve(MAT *A,VEC *b,VEC *x,double diag_val),
		*UTsolve(MAT *A,VEC *b,VEC *x,double diag_val),
                *LUTsolve(MAT *A,PERM *,VEC *,VEC *),
                *QRCPsolve(MAT *QR,VEC *diag,PERM *pivot,VEC *b,VEC *x);

extern  BAND    *bdLUfactor(BAND *A,PERM *pivot),
                *bdLDLfactor(BAND *A);
extern  VEC     *bdLUsolve(BAND *A,PERM *pivot,VEC *b,VEC *x),
                *bdLDLsolve(BAND *A,VEC *b,VEC *x);



extern	VEC	*hhvec(VEC *,u_int,Real *,VEC *,Real *);
extern	VEC	*hhtrvec(VEC *,double,u_int,VEC *,VEC *);
extern	MAT	*hhtrrows(MAT *,u_int,u_int,VEC *,double);
extern	MAT	*hhtrcols(MAT *,u_int,u_int,VEC *,double);

extern	void	givens(double,double,Real *,Real *);
extern	VEC	*rot_vec(VEC *,u_int,u_int,double,double,VEC *); /* in situ */
extern	MAT	*rot_rows(MAT *,u_int,u_int,double,double,MAT *); /* in situ */
extern	MAT	*rot_cols(MAT *,u_int,u_int,double,double,MAT *); /* in situ */


/* eigenvalue routines */

              
extern	VEC	*trieig(VEC *a,VEC *b,MAT *Q),
                 
		*symmeig(MAT *A,MAT *Q,VEC *out);

              
extern	MAT	*schur(MAT *A,MAT *Q);
        
extern	void	schur_evals(MAT *A,VEC *re_part,VEC *im_part);
          
extern	MAT	*schur_vecs(MAT *T,MAT *Q,MAT *X_re,MAT *X_im);


/* singular value decomposition */

        
VEC	*bisvd(VEC *a,VEC *b,MAT *U,MAT *V),
              
	*svd(MAT *A,MAT *U,MAT *V,VEC *out);

/* matrix powers and exponent */
MAT  *_m_pow(MAT *,int,MAT *,MAT *);
MAT  *m_pow(MAT *,int, MAT *);
MAT  *m_exp(MAT *,double,MAT *);
MAT  *_m_exp(MAT *,double,MAT *,int *,int *);
MAT  *m_poly(MAT *,VEC *,MAT *);

/* FFT */
void fft(VEC *,VEC *);
void ifft(VEC *,VEC *);


#endif
