#ifndef MEF_STOKES_H
#define MEF_STOKES_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file algo_stokes.h
 *
 * Contains the stokes algos - dim independant -
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

/* basic structure for iterative methods */

/* type Fun_Rx for functions to get y = R*x --R pressure matrix-- */
typedef VEC  *(*Fun_Rx)(void *,void *,void *,void *,void *,void*, double, VEC *,VEC *);


/* type ITER */
typedef struct ITERSTOKES_DATA {

   int shared_x;   /* if TRUE then x is shared and it will not be free'd */ 
   int shared_b;   /* if TRUE then b is shared and it will not be free'd */
   
   unsigned k;   /* no. of direction (search) vectors; =0 - none */
   int limit;    /* upper bound on the no. of iter. steps */
   int steps;    /* no. of iter. steps done */
   Real eps;     /* accuracy required */
   
   VEC *x;       /* input: initial guess;
		    output: approximate solution */
   VEC *b;       /* right hand side of the equation A*x = b */

   Fun_Rx   Rx; /* function computing y = A*x */
   void *R_par1;        /* parameters for Ax */
   void *R_par2;        /* parameters for Ax */
   void *R_par3;        /* parameters for Ax */
   void *R_par4;        /* parameters for Ax */
   void *R_par5;        /* parameters for Ax */
   void *R_par6;        /* parameters for Ax */
   
   Fun_Rx   Px; /* function computing y = B*x; B - preconditioner */
   void *P_par1;        /* parameters for Bx */
   void *P_par2;        /* parameters for Bx */
   void *P_par3;        /* parameters for Bx */
   void *P_par4;        /* parameters for Bx */
   void *P_par5;        /* parameters for Bx */
   void *P_par6;        /* parameters for Bx */

   Real rho;

   void (*info)     (struct ITERSTOKES_DATA *, double, VEC *,VEC *);
            /* function giving some information for a user;
	       nres - a norm of a residual res */
   
   int  (*stop_crit)(struct ITERSTOKES_DATA *, double, VEC *,VEC *);
           /* stopping criterion:
	      nres - a norm of res;
	      res - residual;
	    if returned value == TRUE then stop;
	    if returned value == FALSE then continue; */

   Real init_res;   /* the norm of the initial residual */

}  ITERSTOKES;

/* type Fun_Stokes_info */
typedef void (*Fun_Stokes_info)   (ITERSTOKES *, double, VEC *,VEC *);
/* type Fun_Stokes_stp_crt */
typedef int  (*Fun_Stokes_stp_crt)(ITERSTOKES *, double, VEC *,VEC *);

/* standard information */
void iter_stokes_std_info     (ITERSTOKES *ip, double nres, VEC *res, VEC *Bres);
/* standard stopping criterion */
int  iter_stokes_std_stop_crit(ITERSTOKES *ip, double nres, VEC *res, VEC *Bres);



#define ISTOKESNULL   (ITERSTOKES *)NULL

/* save free macro */
#define ITERSTOKES_FREE(ip)  (iter_stokes_free(ip), (ip)=(ITERSTOKES *)NULL)


/* get, resize and free ITER variable */
ITERSTOKES *iter_stokes_get(int lenb, int lenx);
int         iter_stokes_free(ITERSTOKES *ip);


/* prototypes from iter.c */

/* different iterative procedures */
VEC  *iter_stokes_cg(ITERSTOKES *ip);
VEC  *iter_stokes_bicgstab(ITERSTOKES *ip);

/******************************************************************************/

int stokes_resol_pressurematrix       (SPMAT *A              , SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps);
int stokes_resol_pressurematrix_factOk(SPMAT *A, SPMAT *LLT_A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps, PERM *Pu, PERM *INVPu);

int stokes_resol_uzawa        (SPMAT *A ,               SPMAT *B, SPMAT *C,                 SPMAT *LLT_PRECOND, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps);
int stokes_resol_uzawa_factOk (SPMAT *A , SPMAT *LLT_A, SPMAT *B, SPMAT *C, SPMAT* PRECOND, SPMAT *LLT_PRECOND, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps, PERM *Pu, PERM *INVPu, PERM *Pp, PERM *INVPp);

int stokes_resol_augmentedlagrangian(SPMAT *A, SPMAT *B, SPMAT *J, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps);


/******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif




