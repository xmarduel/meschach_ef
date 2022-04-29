
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
/////////////////////////////////////////////////////////
 
#ifndef MATLAB_DEF
#define MATLAB_DEF

#include "matrix.h"
#include "zmatrix.h"

/* structure required by MATLAB */
//typedef struct {
//	long    type;   /* matrix type */
//	long    m;      /* # rows */
//	long    n;      /* # cols */
//	long    imag;   /* is complex? */
//	long    namlen; /* length of variable name */
//		} matlab;

/* macros for matrix storage type */
#define INTEL   0       /* for 80x87 format */
#define PC      INTEL
#define MOTOROLA        1       /* 6888x format */
#define SUN     MOTOROLA
#define APOLLO  MOTOROLA
/* #define MAC     MOTOROLA */
#define VAX_D   2
#define VAX_G   3

#define COL_ORDER       0
#define ROW_ORDER       1

#define DOUBLE_PREC  0       /* double precision */
#define SINGLE_PREC  1       /* single precision */
#define INT_32  2       /* 32 bit integers (signed) */
#define INT_16  3       /* 16 bit integers (signed) */
#define INT_16u 4       /* 16 bit integers (unsigned) */
/* end of macros for matrix storage type */

#ifndef MACH_ID
#define MACH_ID         MOTOROLA
#endif

#define ORDER           ROW_ORDER

#if REAL == DOUBLE
#define PRECISION       DOUBLE_PREC
#elif REAL == FLOAT
#define PRECISION  	SINGLE_PREC
#endif


/* prototypes */

MAT *m_save(FILE *,MAT *,const char *);
MAT *m_load(FILE *,char **);
VEC *v_save(FILE *,VEC *,const char *);
double d_save(FILE *,double,const char *);

/* complex variant */

extern ZMAT	*zm_save(FILE *fp,ZMAT *A,const char *name);
extern ZVEC	*zv_save(FILE *fp,ZVEC *x,const char *name);
extern complex	z_save(FILE *fp,complex z,const char *name);
extern ZMAT	*zm_load(FILE *fp,char **name);



#endif
