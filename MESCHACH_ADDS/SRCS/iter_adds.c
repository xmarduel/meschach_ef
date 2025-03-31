/** \file iter_adds.c
 *
 * extentions to MESCHACH LIBRARY for usual structs : improved iterativ methods + better I/O
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

ITER_OUTPUT_INFO* iter_output_info(ITER_INFOe iter_output_type, const char* output_filename)
{
	ITER_OUTPUT_INFO* iter_output_info = malloc(sizeof(ITER_OUTPUT_INFO));
	
	iter_output_info->iter_output_type  = iter_output_type;
	iter_output_info->output_filename   = strdup(output_filename);
		
	return iter_output_info;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* a new standard information : no output residuals in stdout */
void iter_null_info(ITER *ip, Real nres, VEC *res, VEC *Bres)
{
   return;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* a new standard information : output residuals in stdout (=iter_std_info") */
void iter_std1_info(ITER *ip, Real nres, VEC *res, VEC *Bres)
{
   /* check input */
   if ( !ip )  error(E_NULL, "iter_std1_info");
   
   /* write the residuals in the file fp_iter , named ITER_FILE */
   if (nres >= 0.0)
   {
      fprintf(stdout, " %d   %g \n", ip->steps, nres);
   }
   else 
   {
      fprintf(stdout, "# %d   %g (WARNING !!! should be >= 0) \n", ip->steps, nres);
   }
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* a new standard information : output residuals in a file */
void iter_std2_info(ITER *ip, Real nres, VEC *res, VEC *Bres)
{
#ifdef MESCHACH__ITER_EXTENSIONS
   /* check input */
   if ( !ip )  error(E_NULL, "iter_std2_info");
   /* check file */
   if ( !ip->fp ) error(E_NULL, "iter_std2_info");
	
   /* write the residuals in the file fp_iter , named ITER_FILE */
   if (nres >= 0.0)
   {
      fprintf(ip->fp, " %d   %g \n", ip->steps, nres);
   }
   else 
   {
      fprintf(ip->fp, "# %d   %g (WARNING !!! should be >= 0) \n", ip->steps, nres);
   }
#endif
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* a new standard information : output residuals in a file + stdout */
void iter_std3_info(ITER *ip, Real nres, VEC *res, VEC *Bres)
{
#ifdef MESCHACH__ITER_EXTENSIONS
   /* check input */
   if ( !ip )  error(E_NULL, "iter_std3_info");
   /* check file */
   if ( !ip->fp ) error(E_NULL, "iter_std3_info");
   
   /* write the residuals in the file fp_iter , named ITER_FILE */
   /* write the residuals in the standart output                */
   if (nres >= 0.0)
   {
      fprintf(ip->fp, " %d   %g \n", ip->steps, nres);
      fprintf(stdout, " %d   %g \n", ip->steps, nres);
   }
   else 
   {
      fprintf(ip->fp, "# %d   %g (WARNING !!! should be >= 0) \n", ip->steps, nres);
      fprintf(stdout, "# %d   %g (WARNING !!! should be >= 0) \n", ip->steps, nres);
   }
#endif
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

void iter_setup(ITER *ip, ITER_OUTPUT_INFO *iter_output_info)
{
	switch(iter_output_info->iter_output_type)
	{
		case ITER_INFOe_NONE:
	   {
			ip->info = iter_null_info;
			ip->fp   = NULL;
		}
			break;
			
	   case ITER_INFOe_STDOUT:
	   {
		   ip->info = iter_std1_info;
		   ip->fp   = NULL;
	   }
			break;
			
#ifdef MESCHACH__ITER_EXTENSIONS
	   case ITER_INFOe_FILE:
	   {
	      ip->info = iter_std2_info;
		   ip->fp   = fopen(iter_output_info->output_filename, "w");
	   }
			break;
			
		case ITER_INFOe_ALL:
	   {
	      ip->info = iter_std3_info;
		   ip->fp   = fopen(iter_output_info->output_filename, "w");
	   }
			break;
	}
#endif
}

void iter_clean(ITER *ip, ITER_OUTPUT_INFO *iter_output_info)
{
	switch(iter_output_info->iter_output_type)
	{
		case ITER_INFOe_NONE:
	   case ITER_INFOe_STDOUT:
         break;
			
#ifdef MESCHACH__ITER_EXTENSIONS
	   case ITER_INFOe_FILE:
		case ITER_INFOe_ALL:
	      if ( ip->fp != NULL ) {	fclose(ip->fp); ip->fp = NULL; }
			break;
	}
#endif
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* 
 Biconjugate gradients Stabilized method;
 */
VEC  *iter_bicgstab(ITER *ip)
{
   static VEC *s = VNULL;
   static VEC *p = VNULL;
   static VEC *r = VNULL; static VEC *rr = VNULL;
   static VEC *v = VNULL;
   static VEC *t = VNULL;
   
   static VEC *z2= VNULL;  /* additional vec if (ip->B) */
   static VEC *z1= VNULL;  /* additional vec if (ip->B) */
   
   Real rho,old_rho=0.0,omega=0.0,alpha=0.0,beta;
   Real inner, nres;
   Real inner_b = sqrt(fabs(in_prod(ip->b,ip->b)));
   
   VEC *ss;
   VEC *pp;
	
   if ( ip->eps <= 0.0 )  ip->eps = MACHEPS;
   
   s = v_resize(s,ip->b->dim);
   p = v_resize(p,ip->b->dim);
   r = v_resize(r,ip->b->dim);   rr = v_resize(rr,ip->b->dim);
   v = v_resize(v,ip->b->dim);
   t = v_resize(t,ip->b->dim);
   
   MEM_STAT_REG(s,TYPE_VEC);
   MEM_STAT_REG(p,TYPE_VEC);
   MEM_STAT_REG(r,TYPE_VEC);   MEM_STAT_REG(rr,TYPE_VEC);
   MEM_STAT_REG(v,TYPE_VEC);
   MEM_STAT_REG(t,TYPE_VEC);
   
   
   if (ip->Bx != (Fun_Ax)NULL)
   {
      z1 = v_resize(z1,ip->b->dim);
      MEM_STAT_REG(z1,TYPE_VEC);
      ss = z1;
   }
   else ss = s;
   
   if (ip->Bx != (Fun_Ax)NULL)
   {
      z2 = v_resize(z2,ip->b->dim);
      MEM_STAT_REG(z2,TYPE_VEC);
      pp = z2;
   }
   else pp = p;
   
   
   /* initialisation : all residuals scaled with |b| */
   ip->steps = 0; nres = 1.0;
   if (ip->info) ip->info(ip,nres,r,VNULL);
   if (ip->stop_crit(ip,nres,r,VNULL)) goto fin;
   
   
   v_zero(ip->x);
   
   r = (ip->Ax)(ip->A_par,ip->x,r);
   r = v_sub(ip->b,r,r);
   
   rr = v_copy(r,rr);
   
   
   for ( ip->steps = 1; ip->steps <= ip->limit; ip->steps++ )
   {
      rho = in_prod(rr,r);
      if ( rho==0.0 ) error(E_SING, "iter_bicgstab");
      
      if ( ip->steps==1 )
      {
         p = v_copy(r,p);
      }
      else
      {
         beta = ( rho/old_rho)*(alpha/omega);
         
         p = v_mltadd(p,v,-omega,p);
         p = v_mltadd(r,p, beta ,p);
      }
      
      if ( ip->Bx ) (ip->Bx)(ip->B_par,p,pp);    /* solve B*pp = p */
      
      v = (ip->Ax)(ip->A_par,pp,v);               /* v = A*pp */
      alpha = rho/in_prod(rr,v);
      s = v_mltadd(r,v,-alpha,s);
      
      if ( fabs(sqrt(in_prod(s,s)))/inner_b < ip->eps )
      {
         nres=fabs(sqrt(in_prod(s,s)))/inner_b;
         ip->x = v_mltadd(ip->x,pp,alpha,ip->x);
         if (ip->info) ip->info(ip,nres,r,VNULL);
         if (ip->stop_crit(ip,nres,r,VNULL) )
            break;
      }
      
      if ( ip->Bx ) (ip->Bx)(ip->B_par,s,ss);    /* solve B*ss = s */
      t = (ip->Ax)(ip->A_par,ss,t);               /* t = A*ss */
      omega = in_prod(t,s)/in_prod(t,t);
      
      ip->x = v_mltadd(ip->x,pp,alpha,ip->x);
      ip->x = v_mltadd(ip->x,ss,omega,ip->x);
      r = v_mltadd(s,t,-omega,r);
      inner = in_prod(r,r);
      /*nres = sqrt(fabs(inner));*/
      nres = sqrt(fabs(inner))/inner_b;
      
      if (ip->info) ip->info(ip,nres,r,VNULL);
      if (ip->stop_crit(ip,nres,r,VNULL)) break;
      
      old_rho = rho;
   }
   
fin:
   
   return ip->x;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* iter_xspbicgstab -- a simple interface to iter_bicgstab() with sparse matrix.
 data structures
 -- assumes that ILU contains the LU factorisation of the
 actual preconditioner;
 use always as follows:
 x = iter_spbicgstab(A,ILU,b,eps,x,limit,steps);
 or 
 x = iter_spbicgstab(A,ILU,b,eps,VNULL,limit,steps);
 In the second case the solution vector is created.
 */
VEC  *iter_xspbicgstab(const SPMAT *A, const SPMAT *ILU, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{	
   ITER *ip;
	
   /* check input */
   if ( !A   )  error(E_NULL, "iter_spbicgstab");
   if ( !b   )  error(E_NULL, "iter_spbicgstab");
	
   ip = iter_get(0,0);
	
	/* new: customize iter */
	if (iter_output_info) iter_setup(ip, iter_output_info);
	/* new: customize iter */
	
   ip->stop_crit = iter_std_stop_crit;
	
   ip->Ax = (Fun_Ax) sp_mv_mlt;
   ip->A_par = (void *)A;
   if( ILU )
   {
      ip->Bx = (Fun_Ax) spILUsolve; /* spILUsolve = spLUsolve without      */
      ip->B_par = (void *)ILU;      /*            permutation in arguments */
   }
   else
   {
      ip->Bx = (Fun_Ax) NULL;
      ip->B_par = NULL;
   }
   ip->b = (VEC *)b;
   ip->eps = eps;
   ip->limit = limit;
   ip->x = x;
	
   iter_bicgstab(ip);
	
   x = ip->x;
   if (steps) *steps = ip->steps;
   ip->shared_x = ip->shared_b = TRUE;
	
	/* new: customize iter */
	if (iter_output_info) iter_clean(ip, iter_output_info);
	/* new: customize iter */
	
   iter_free(ip);   /* release only ITER structure */
	
   return x;		
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC  *iter_xspcg(const SPMAT *A, const SPMAT *LLT, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{	
   ITER *ip;

	/* check input */
   if ( !A )  error(E_NULL, "iter_xspcg");
   if ( !b )  error(E_NULL, "iter_xspcg");
   
   
   ip = iter_get(0,0);
	
	/* new: customize iter */
	if (iter_output_info) iter_setup(ip, iter_output_info);
	/* new: customize iter */
	
   ip->Ax = (Fun_Ax) sp_mv_mlt;
   ip->A_par = (void *)A;
   if( LLT ) {
		ip->Bx = (Fun_Ax) spCHsolve;
		ip->B_par = (void *)LLT;
   }
   else {
		ip->Bx = (Fun_Ax) NULL;
		ip->B_par = NULL;
   }
	
   ip->b = (VEC *)b;
   ip->eps = eps;
   ip->limit = limit;
   ip->x = x;
	
   iter_cg(ip);
	
   x = ip->x;
   if (steps) *steps = ip->steps;
   ip->shared_x = ip->shared_b = TRUE;
	
	/* new: customize iter */
	if (iter_output_info) iter_clean(ip, iter_output_info);
	/* new: customize iter */
	
   iter_free(ip);   /* release only ITER structure */
	
   return x;		
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC *iter_xspcgs(const SPMAT *A, const SPMAT *B, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{	
   ITER *ip;
	
	/* check input */
   if ( !A   )  error(E_NULL, "iter_xspcgs");
   if ( !b   )  error(E_NULL, "iter_xspcgs");
   
   
   ip = iter_get(0,0);
	
	/* new: customize iter */
	if (iter_output_info) iter_setup(ip, iter_output_info);
	/* new: customize iter */
	
   ip->Ax = (Fun_Ax) sp_mv_mlt;
   ip->A_par = (void *) A;
   if (B) {
      ip->Bx = (Fun_Ax) sp_mv_mlt;
      ip->B_par = (void *) B;
   }
   else {
      ip->Bx = (Fun_Ax) NULL;
      ip->B_par = NULL;
   }
	
   ip->limit = limit;
   ip->b = (VEC *)b;
   ip->eps = eps;
   ip->x = x;
	
	VEC *r0 = v_get(b->dim);
	v_zero(r0);
	
   iter_cgs(ip,r0);
	
   x = ip->x;
   if (steps) *steps = ip->steps;
   ip->shared_x = ip->shared_b = TRUE;   
   
	/* new: customize iter */
	if (iter_output_info) iter_clean(ip, iter_output_info);
	/* new: customize iter */
	
	iter_free(ip);   /* release only ITER structure */
	
	V_FREE(r0);
	
   return x;		
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC *iter_xspgmres(const SPMAT *A, const SPMAT *B, const VEC *b, Real eps, VEC *x, int k, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{
   ITER *ip;

	/* check input */
   if ( !A )  error(E_NULL, "iter_xspgmres");
   if ( !b )  error(E_NULL, "iter_xspgmres");
   
   
   ip = iter_get(0,0);
	
	/* new: customize iter */
	if (iter_output_info) iter_setup(ip, iter_output_info);
	/* new: customize iter */
	
   ip->Ax = (Fun_Ax) sp_mv_mlt;
   ip->A_par = (void *) A;
   if (B) {
      ip->Bx = (Fun_Ax) spILUsolve;
      ip->B_par = (void *) B;
   }
   else {
      ip->Bx = (Fun_Ax) NULL;
      ip->B_par = NULL;
   }
	
   ip->k = k;
   ip->limit = limit;
   ip->b = (VEC *)b;
   ip->eps = eps;
   ip->x = x;
	
   iter_gmres(ip);
	
   x = ip->x;
   if (steps) *steps = ip->steps;
   ip->shared_x = ip->shared_b = TRUE;
   
	/* new: customize iter */
	if (iter_output_info) iter_clean(ip, iter_output_info);
	/* new: customize iter */
	
	iter_free(ip);   /* release only ITER structure */
	
   return x;		
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* iter_xspmgcr - a simple interface to iter_mgcr */
/* no preconditioner */
VEC *iter_xspmgcr(const SPMAT *A, const SPMAT *B, const VEC *b, Real eps, VEC *x, int k, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{
   ITER *ip;
   
	/* check input */
   if ( !A )  error(E_NULL, "iter_xspmgcr");
   if ( !b )  error(E_NULL, "iter_xspmgcr");
   
   
   ip = iter_get(0,0);
	
	/* new: customize iter */
	if (iter_output_info) iter_setup(ip, iter_output_info);
	/* new: customize iter */
	
   ip->Ax = (Fun_Ax) sp_mv_mlt;
   ip->A_par = (void *) A;
   
   if (B)
   {
      ip->Bx = (Fun_Ax) sp_mv_mlt;
      ip->B_par = (void *) B;
   }
   else
   {
      ip->Bx = (Fun_Ax) NULL;
      ip->B_par = NULL;
   }
   
   ip->k = k;
   ip->limit = limit;
   ip->b = (VEC *)b;
   ip->eps = eps;
   ip->x = x;
   
   iter_mgcr(ip);
   
   x = ip->x;
   if (steps) *steps = ip->steps;
   ip->shared_x = ip->shared_b = TRUE;
   
	/* new: customize iter */
	if (iter_output_info) iter_clean(ip, iter_output_info);
	/* new: customize iter */
	
	iter_free(ip);   /* release only ITER structure */
   
   return x;		
}


/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC *iter_xspcg_bandwr      (const SPMAT *A, const SPMAT *LLT, const PERM *P, const PERM *INVP, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{
   VEC *b_perm = v_get(b->dim);
   VEC *x_perm = v_get(x->dim);
   
   /* check input */
   if ( A == SMNULL )   error(E_NULL, "iter_xspcg_bandwr");
   if ( LLT == SMNULL ) error(E_NULL, "iter_xspcg_bandwr");
   if ( P == PNULL )    error(E_NULL, "iter_xspcg_bandwr");
   if ( INVP == PNULL ) error(E_NULL, "iter_xspcg_bandwr");
   if ( b == VNULL )    error(E_NULL, "iter_xspcg_bandwr");
   if ( x == VNULL )    error(E_NULL, "iter_xspcg_bandwr");
   
   b_perm = px_vec(P, b, b_perm);     /* the permutated matrix must be st.  P * A * P'  */
   iter_xspcg(A, LLT, b_perm /* in */, eps, x_perm /* out */, limit, steps, iter_output_info);
   x = px_vec(INVP, x_perm, x);      /* the permutated matrix must be st.  P * A * P'  */
	
   /* clean */
   V_FREE(b_perm);
   V_FREE(x_perm);
   
   /* finito */
   return x;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC  *iter_xspbicgstab_bandwr(const SPMAT *A, const SPMAT *LLT, const PERM *P, const PERM *INVP, const VEC *b, Real eps, VEC *x, int limit, int *steps, ITER_OUTPUT_INFO* iter_output_info)
{
   VEC *b_perm = v_get(b->dim);
   VEC *x_perm = v_get(x->dim);
	
   /* check input */
   if ( A == SMNULL )   error(E_NULL, "iter_xspbicgstab_bandwr");
   if ( LLT == SMNULL ) error(E_NULL, "iter_xspbicgstab_bandwr");
   if ( P == PNULL )    error(E_NULL, "iter_xspbicgstab_bandwr");
   if ( INVP == PNULL ) error(E_NULL, "iter_xspbicgstab_bandwr");
   if ( b == VNULL )    error(E_NULL, "iter_xspbicgstab_bandwr");
   if ( x == VNULL )    error(E_NULL, "iter_xspbicgstab_bandwr");
   
   b_perm = px_vec(P, b, b_perm);     /* the permutated matrix must be st.  P * A * P'  */
   iter_xspbicgstab(A, LLT, b_perm /* in */, eps, x_perm /* out */, limit, steps, iter_output_info);
   x = px_vec(INVP, x_perm, x);      /* the permutated matrix must be st.  P * A * P'  */
	
   /* clean */
   V_FREE(b_perm);
   V_FREE(x_perm);
	
   /* finito */
   return x;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

