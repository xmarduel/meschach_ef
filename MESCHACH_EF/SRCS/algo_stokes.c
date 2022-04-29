
/** \file algo_stokes.c
 *
 * Contains algorithms for Stokes
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"
#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/algo_stokes.h"

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static VEC* spPRESSUREMATRIXapply1       (SPMAT *A, PERM *Pa, PERM *INVPa, SPMAT *B, SPMAT *LLT_A, SPMAT *unused, double rho, VEC* in, VEC *out);
static VEC* spPRESSUREMATRIXapply2       (SPMAT *A, PERM *Pa, PERM *INVPa, SPMAT *B, SPMAT *LLT_A, SPMAT *unused, double rho, VEC* in, VEC *out);
static VEC* spPRESSUREMATRIXapply1_bandwr(SPMAT *A, PERM *Pa, PERM *INVPa, SPMAT *B, SPMAT *LLT_A, SPMAT *unused, double rho, VEC* in, VEC *out);
static VEC* spPRESSUREMATRIXapply2_bandwr(SPMAT *A, PERM *Pa, PERM *INVPa, SPMAT *B, SPMAT *LLT_A, SPMAT *unused, double rho, VEC* in, VEC *out);

static VEC* spAUGMLAGRMATRIXapply1       (SPMAT *J, PERM *Pj, PERM *INVPj, SPMAT *B, SPMAT *LLT_J, SPMAT *A, double rho, VEC* in, VEC *out);
static VEC* spAUGMLAGRMATRIXapply1_bandwr(SPMAT *J, PERM *Pj, PERM *INVPj, SPMAT *B, SPMAT *LLT_J, SPMAT *A, double rho, VEC* in, VEC *out);
static VEC* spAUGMLAGRMATRIXapply2       (SPMAT *J, PERM *Pj, PERM *INVPj, SPMAT *B, SPMAT *LLT_J, SPMAT *A, double rho, VEC* in, VEC *out);
static VEC* spAUGMLAGRMATRIXapply2_bandwr(SPMAT *J, PERM *Pj, PERM *INVPj, SPMAT *B, SPMAT *LLT_J, SPMAT *A, double rho, VEC* in, VEC *out);

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

int stokes_resol_uzawa (SPMAT *A, SPMAT *B, SPMAT *C, SPMAT *PRECOND, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps)
{
	
   /*
	 
	 A*U_kp1 = F - Bt*P
	 
	 P*(P_kp1 - P_k) = rho.B*U_kp1     0 < rho < 2*kappa
	 
	*/
	
   PARAMS* MyParams = Params_get_staticparam(0);      

   BANDWRt_METHOD  bandwr_method     = MyParams->stokes_params.uzawa.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->stokes_params.uzawa.innerloop_solver.bandwidth_option;
   BANDWRt_MATRIXTYPE  bandwr_mtype  = BANDWRe_NONSYM;

   SPMAT *PAPt ;
   SPMAT *PJPt ;
	
	SPMAT *LLT_PAPt ;
   SPMAT *LLT_PJPt ;

   PERM  *Pu    = NULL;
   PERM  *INVPu = NULL;

   PERM  *Pp    = NULL;
   PERM  *INVPp = NULL;

   int rc;
   
   /* check */
   if ( ! A ) error (E_NULL, "stokes_resol_uzawa");
   if ( ! B ) error (E_NULL, "stokes_resol_uzawa");
   /*if ( ! C ) error (E_NULL, "stokes_resol_uzawa"); */
   if ( ! PRECOND ) error (E_NULL, "stokes_resol_uzawa");
   if ( ! U ) error (E_NULL, "stokes_resol_uzawa");
   if ( ! P ) error (E_NULL, "stokes_resol_uzawa");
   if ( ! F ) error (E_NULL, "stokes_resol_uzawa");
   if ( ! G ) error (E_NULL, "stokes_resol_uzawa");


   PAPt = sp_copy(A);
	
   PJPt = sp_copy(PRECOND);
	

   if ( bandwr_option != BANDWRe_NO_OPTION )
   {
      printf("\n... stokes_resol_uzawa with bandwidth reduction ...\n");
		
		/* allocate mem */
		Pu    = px_get(U->dim);
      INVPu = px_get(U->dim);

      Pp    = px_get(P->dim);
      INVPp = px_get(P->dim);

      printf("profile before = %d\n", sp_profile(A) );
      /* set the matrix with reduced band width */
      PAPt = sp_dopermutationforbandwr(A      , Pu, INVPu, bandwr_method, bandwr_option , bandwr_mtype, PAPt );
      printf("profile after = %d\n", sp_profile(PAPt) );
    
      /* set the matrix with reduced band width */
      PJPt = sp_dopermutationforbandwr(PRECOND, Pp, INVPp, bandwr_method, bandwr_option , bandwr_mtype, PJPt );
   }
   else
   {
      printf("\n... stokes_resol_uzawa without bandwidth reduction ...\n");
   }
   
	if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
   {
      LLT_PAPt = sp_copy(PAPt);
		spCHfactor(LLT_PAPt);
		
		LLT_PJPt = sp_copy(PJPt);
		spCHfactor(LLT_PJPt);
   }
   if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "ITERATIV-METHOD") == 0 )
   {
      LLT_PAPt = sp_copy(PAPt);
		spICHfactor(LLT_PAPt);
		
		LLT_PJPt = sp_copy(PJPt);
		spICHfactor(LLT_PJPt);
   }

   rc = stokes_resol_uzawa_factOk(PAPt, LLT_PAPt, B, C, PJPt, LLT_PJPt, U, P, 
									  	    F, G,
										    tol, max_steps, steps,
										    Pu, INVPu, Pp, INVPp );

   SP_FREE(PAPt);
   SP_FREE(PJPt);
	
	SP_FREE(LLT_PAPt);
   SP_FREE(LLT_PJPt);


   PX_FREE(Pu); PX_FREE(INVPu);
   PX_FREE(Pp); PX_FREE(INVPp);

   return rc;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

int stokes_resol_uzawa_factOk (SPMAT *PAPt, SPMAT *LLT_PAPt, SPMAT *B, SPMAT *C, SPMAT* PJPt, SPMAT *LLT_PJPt, VEC *U, VEC *P,
							   	    VEC *F, VEC *G,
										 Real tol, int max_steps, int *steps,
                               PERM  *Pu, PERM  *INVPu, PERM  *Pp, PERM  *INVPp )
{
   /*
	 
	A*U_kp1 = F - Bt*P
	 
	P*(P_kp1 - P_k) = rho.B*U_kp1     0 < rho < 2*kappa
	 
	*/
   
	PARAMS* MyParams = Params_get_staticparam(0);      
   Real rho         = MyParams->stokes_params.uzawa.rho;  /* condition on rho follows */

	BANDWRt_METHOD  bandwr_method     = MyParams->stokes_params.uzawa.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->stokes_params.uzawa.innerloop_solver.bandwidth_option;

	int inner_step;
	
   Real ieps_steps = MyParams->stokes_params.uzawa.innerloop_solver.eps_steps;
	int  imax_steps = MyParams->stokes_params.uzawa.innerloop_solver.max_steps;
	
	
   Real residu = 1. ;

   VEC *oldU, *oldP;
   VEC *R   , *R1  ;
   VEC *tmpU;

   /* check */
   if ( ! PAPt ) error (E_NULL, "stokes_resol_uzawas_factOk");
	if ( ! LLT_PAPt ) error (E_NULL, "stokes_resol_uzawas_factOk");
   if ( ! B ) error (E_NULL, "stokes_resol_uzawas_factOk");
   /*if ( ! C ) error (E_NULL, "stokes_resol_uzawas_factOk"); */
   if ( ! PJPt ) error (E_NULL, "stokes_resol_uzawas_factOk");
	if ( ! LLT_PJPt ) error (E_NULL, "stokes_resol_uzawas_factOk");
   if ( ! U ) error (E_NULL, "stokes_resol_uzawas_factOk");
   if ( ! P ) error (E_NULL, "stokes_resol_uzawas_factOk");
   if ( ! F ) error (E_NULL, "stokes_resol_uzawas_factOk");
   if ( ! G ) error (E_NULL, "stokes_resol_uzawas_factOk");

   /* allocate mem */
   oldU = v_get(U->dim);
   oldP = v_get(P->dim);

   R = v_get(G->dim);
   R1= v_get(G->dim);

   tmpU = v_get(U->dim);
	v_zero(tmpU);


   for (*steps=1 ; *steps <= max_steps ; (*steps)++ )
   {
      oldP = v_copy(P, oldP);
      oldU = v_copy(U, oldU);

      /* U_kp1 : A*U_kp1 = F - Bt*P */
      v_sub(F, sp_vm_mlt(B,P,tmpU), tmpU);
      
      if ( bandwr_option != BANDWRe_NO_OPTION )
      {
         if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
			{
				spCHsolve_bandwr(LLT_PAPt, Pu, INVPu, tmpU, U);
			}
			if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "ITERATIV-METHOD") == 0 ) 
			{
				iter_xspcg_bandwr(PAPt, LLT_PAPt, Pu, INVPu, tmpU, ieps_steps, U, imax_steps, &inner_step, NULL);
			}
      }
      else
      {
         if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
			{
				spCHsolve(LLT_PAPt, tmpU, U);
			}
			if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "ITERATIV-METHOD") == 0 )
			{
				iter_xspcg(PAPt, LLT_PAPt, tmpU, ieps_steps, U, imax_steps, &inner_step, NULL);
			}
      }

		/* P_kp1 : P*(P_kp1 - P_k) = rho.B*U_kp1 - G */ 
      R = sp_mv_mlt(B, U, R);
      R = v_sub(R, G, R);

      /* >>> xm */
      if ( bandwr_option != BANDWRe_NO_OPTION )
      {
         if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
			{
				spCHsolve_bandwr(LLT_PJPt, Pp, INVPp, R, R1);
			}
			if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "ITERATIV-METHOD") == 0 )
			{
				iter_xspcg_bandwr(PJPt, LLT_PJPt, Pp, INVPp, R, ieps_steps, R1, imax_steps, &inner_step, NULL);
			}
      }
      else
      {
         if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "DIRECT-METHOD") == 0 )
			{
				spCHsolve(LLT_PJPt, R, R1);
			}
			if ( strcmp(MyParams->stokes_params.uzawa.innerloop_method, "ITERATIV-METHOD") == 0 )
			{
				iter_xspcg(PJPt, LLT_PJPt, R, ieps_steps, R1, imax_steps, &inner_step, NULL);
			}
			
      }
      /* <<< xm */

      /* p_kp1 = p_k + rho*r */
      v_add(P, sv_mlt(rho,R1,R), P);


      printf("iter %05d :", *steps);

      printf("norm(p-oldp)= %le   ", v_norm2(v_sub(P, oldP, oldP)) ); 
      printf("norm(u-oldu)= %le \n", v_norm2(v_sub(U, oldU, oldU)) );

      residu = v_norm2(R);

      if (residu <= tol && (*steps)>2)
      {
         V_FREE(oldU);    V_FREE(oldP);
         V_FREE(R);       V_FREE(R1);  V_FREE(tmpU);

         return EXIT_SUCCESS;
      }
   }

   V_FREE(oldU);    V_FREE(oldP);
   V_FREE(R);       V_FREE(R1);  V_FREE(tmpU);

   return EXIT_FAILURE;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*
 * spPRESSUREMATRIXapply1 : perform  out = R*in with R = B*inv(A)*Bt
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with cholesky factorisation )
 *  3-  o3 = out = B*o2
 *
 * Note : A unused - LLT_A is the Choleski factorisation of A
 * Note : Pa and INVPa unused
 *
 */
static VEC* spPRESSUREMATRIXapply1(SPMAT *A, PERM *Pa, PERM *INVPa,
                                   SPMAT *B,
                                   SPMAT *LLT_A,
                                   SPMAT *unused,
                                   double rho, VEC* in, VEC *out)
{
  static VEC *o1 = VNULL;
  static VEC *o2 = VNULL;

  o1 = v_resize(o1, B->n);
  o2 = v_resize(o2, B->n);

  MEM_STAT_REG(o1,TYPE_VEC);
  MEM_STAT_REG(o2,TYPE_VEC);
  
  o1  = sp_vm_mlt(B, in, o1);
  o2  = spCHsolve(LLT_A, o1, o2);
  out = sp_mv_mlt(B, o2, out);
  
  return out;
}

/*
 * spPRESSUREMATRIXapply2 : perform  out = R*in with R = B*inv(A)*Bt
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with an PCG iterativ method )
 *  3-  o3 = out = B*o2
 *
 * Note : A is used - ICH_A is the Incomplete Choleski factorisation of A
 * Note : Pa and INVPa unused
 *
 */
static VEC* spPRESSUREMATRIXapply2(SPMAT *A, PERM *Pa, PERM *INVPa,
                                   SPMAT *B,
                                   SPMAT *ICH_A,
                                   SPMAT *unused,
                                   double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   PARAMS* MyParams = Params_get_staticparam(0);

   Real eps_steps    = MyParams->stokes_params.pressurematrix.innerloop_solver.eps_steps;
   int  max_steps    = MyParams->stokes_params.pressurematrix.innerloop_solver.max_steps;
   int  steps;
   
   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   o1  = sp_vm_mlt(B, in, o1);
   iter_xspcg(A, ICH_A, o1 /* in */, eps_steps, o2 /* out */, max_steps, &steps, NULL);
   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*
 * spPRESSUREMATRIXapply1_bandwr : perform  out = R*in with R = B*inv(A)*Bt  with A permutated for bandwidth reduction
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with cholesky factorisation + permutations)
 *  3-  o3 = out = B*o2
 *
 * Note : A unused - LLT_A is the Choleski factorisation of A
 * Note : Pa and INVPa are used
 *
 */
static VEC* spPRESSUREMATRIXapply1_bandwr(SPMAT *A, PERM *Pa, PERM *INVPa,
                                          SPMAT *B,
                                          SPMAT *LLT_A,
                                          SPMAT *unused,
                                          double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);
  
   o1  = sp_vm_mlt(B, in, o1);
   o2  = spCHsolve_bandwr(LLT_A, Pa, INVPa, o1, o2);
   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*
 * spPRESSUREMATRIXapply2_bandwr : perform  out = R*in with R = B*inv(A)*Bt  with A permutated for bandwidth reduction
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with PCG factorisation )
 *  3-  o3 = out = B*o2
 *
 * Note : A is used - LLT_A is the Incomplete Choleski factorisation of A
 * Note : Pa and INVPa are used
 *
 */
static VEC* spPRESSUREMATRIXapply2_bandwr(SPMAT *A, PERM *Pa, PERM *INVPa,
                                          SPMAT *B,
                                          SPMAT *ICH_A,
                                          SPMAT *unused,
                                          double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   static VEC *o1perm = VNULL;
   static VEC *o2perm = VNULL;

   PARAMS* MyParams = Params_get_staticparam(0);

   int  eps_steps    = MyParams->stokes_params.pressurematrix.innerloop_solver.eps_steps;
   int  max_steps    = MyParams->stokes_params.pressurematrix.innerloop_solver.max_steps;
   int  steps;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   o1perm = v_resize(o1perm, B->n);
   o2perm = v_resize(o2perm, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   MEM_STAT_REG(o1perm,TYPE_VEC);
   MEM_STAT_REG(o2perm,TYPE_VEC);

   o1  = sp_vm_mlt(B, in, o1);

   o1perm = px_vec(Pa, o1, o1perm);     /* the permutated matrix must be st.  P * A * P'  */
   iter_xspcg(A, ICH_A, o1perm /* in */, eps_steps, o2perm /* out */, max_steps, &steps, NULL); 
   o2 = px_vec(INVPa, o2perm, o2);      /* the permutated matrix must be st.  P * A * P'  */

   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int stokes_resol_pressurematrix(SPMAT *A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps)
{
   PARAMS* MyParams   = Params_get_staticparam(0);
   
   BANDWRt_METHOD  bandwr_method     = MyParams->stokes_params.pressurematrix.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->stokes_params.pressurematrix.innerloop_solver.bandwidth_option;
   BANDWRt_MATRIXTYPE  bandwr_mtype  = BANDWRe_NONSYM;

   int rc;

   /*
   A*U + Bt*P = F
   B*U        = G
   
   U = inv(A)[ F - Bt*P ]
   B*U = - B*inv(A)*Bt*P + B*inv(A)*F = G
   
   B*inv(A)*Bt*P = B*inv(A)*F - G
   
   Solve R*P = H  with CG : R = B*inv(A)*Bt ;
                            H = B*inv(A)*F - G ;
   */

   SPMAT *PAPt  ;
   SPMAT *LLT_A ;
   
   PERM *Pu    = px_get(U->dim);
   PERM *INVPu = px_get(U->dim);


   printf("bandwr_method = %d \n", bandwr_method);
   printf("bandwr_option = %d \n", bandwr_option);
   
   if ( bandwr_method != BANDWRe_NULL )
   {
       printf("profile before = %d\n", sp_profile(A) );
       printf("bandw before = %d\n", sp_bandw(A) );
       PAPt  = sp_copy(A);
       PAPt = sp_dopermutationforbandwr(A, Pu, INVPu, bandwr_method, bandwr_option , bandwr_mtype, PAPt );
       printf("profile after = %d\n", sp_profile(PAPt) );
       printf("bandw   after = %d\n", sp_bandw(PAPt) );
   }
   else
   {
       PAPt  = sp_copy(A);
   }

   LLT_A = sp_copy(PAPt); /* copy for factorisation */

   if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
   {
      spCHfactor(LLT_A);
   }
   if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
   {
      spICHfactor(LLT_A);
   }

   /* main algo */
   rc = stokes_resol_pressurematrix_factOk(PAPt, LLT_A, B, U, P, F, G, tol, max_steps, steps, Pu, INVPu );
   
   /* free mem */
   SP_FREE(PAPt);
   SP_FREE(LLT_A);

   PX_FREE(Pu);
   PX_FREE(INVPu);

   /* finito */
   return rc;   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int stokes_resol_pressurematrix_factOk(SPMAT *A, SPMAT *LLT_A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G,
                                       Real tol, int max_steps, int *steps,
                                       PERM *Pu , PERM *INVPu )
{
   PARAMS* MyParams   = Params_get_staticparam(0);      
   int  bandwr_method = MyParams->stokes_params.pressurematrix.innerloop_solver.bandwidth_method;  

   /*
   A*U + Bt*P = F
   B*U        = G
   
   U = inv(A)[ F - Bt*P ]
   B*U = - B*inv(A)*Bt*P + B*inv(A)*F = G
   
   B*inv(A)*Bt*P = B*inv(A)*F - G
   
   Solve R*P = H  with CG : R = B*inv(A)*Bt ;
   H = B*inv(A)*F - G ;
   */

   int inner_step;

   Real ieps_steps = MyParams->stokes_params.pressurematrix.innerloop_solver.eps_steps;
	int  imax_steps = MyParams->stokes_params.pressurematrix.innerloop_solver.max_steps;
	
	VEC *Ptmp;
   VEC *Utmp;

	
   ITERSTOKES *ip;

   /* eval the rhs H = B*inv(A)*F - G */
	 
	Ptmp = v_get(P->dim);
   Utmp = v_get(U->dim);

   Ptmp = sv_mlt(-1.0, G, Ptmp);

   if ( bandwr_method == BANDWRe_NULL ) /* no band width reduction with permutation */
   {
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         Utmp = spCHsolve(LLT_A, F, Utmp);
      }
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         iter_xspcg(A, LLT_A, F, ieps_steps, Utmp, imax_steps, &inner_step, NULL);
      }
   }
   else                                /* yes, permutation for band width reduction */
   {
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         Utmp = spCHsolve_bandwr(LLT_A, Pu, INVPu, F, Utmp);
      }
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         iter_xspcg_bandwr(A, LLT_A, Pu, INVPu, F, ieps_steps, Utmp, imax_steps, &inner_step, NULL);
      }
   }

   Ptmp = sp_mv_mlt(B, Utmp, Ptmp);
   Ptmp = v_sub(Ptmp, G, Ptmp);

   ip = iter_stokes_get(0,0);

   if ( bandwr_method != BANDWRe_NULL )
   {
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spPRESSUREMATRIXapply1_bandwr;
      }
      else
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spPRESSUREMATRIXapply2_bandwr;
      }
   }
   else
   {
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spPRESSUREMATRIXapply1;
      }
      else
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spPRESSUREMATRIXapply2;
      }
   }

   ip->R_par1 = (void *)A;
   ip->R_par2 = (void *)Pu;
   ip->R_par3 = (void *)INVPu;
   ip->R_par4 = (void *)B;
   ip->R_par5 = (void *)LLT_A;
   ip->R_par6 = NULL;
  
   ip->Px = (Fun_Rx) NULL; /* preconditionning */


   ip->info      = iter_stokes_std_info ;
   ip->stop_crit = iter_stokes_std_stop_crit ;

   ip->b     = Ptmp;
   ip->eps   = ieps_steps;
   ip->limit = imax_steps;
   ip->x     = P;

   iter_stokes_cg(ip);

   P = ip->x;
   if (steps) *steps = ip->steps;

   ip->shared_x = ip->shared_b = TRUE;
   iter_stokes_free(ip);   /* release only ITERSTOKES structure */


   /* and the velocity */
   sp_vm_mlt(B,P,Utmp);

   if ( bandwr_method == BANDWRe_NULL ) /* no band width reduction with permutation */
   {
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         U = spCHsolve(LLT_A, v_sub(F, Utmp, Utmp), U);
      }
      else
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         iter_xspcg(A, LLT_A, v_sub(F, Utmp, Utmp) , ieps_steps, U , imax_steps, &inner_step, NULL);
      }
   }
   else /* yes, permutation for band width reduction */
   {
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         U = spCHsolve_bandwr(LLT_A, Pu, INVPu, v_sub(F, Utmp, Utmp), U);
      }
      else
      if ( strcmp(MyParams->stokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         iter_xspcg_bandwr(A, LLT_A, Pu, INVPu, v_sub(F, Utmp, Utmp), ieps_steps, U, imax_steps, &inner_step, NULL);
      }
   }

   V_FREE(Ptmp);
   V_FREE(Utmp);

   return EXIT_SUCCESS;   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*
 * spAUGMLAGRMATRIXapply1 : perform  out = R*in with R = A + p*Bt*inv(J)*B
 *
 *  1-  o1 = B*in
 *  2-  o2 = inv(J)*o1 ( ie J*o2 = o1 : solve with cholesky factorisation )
 *  3-  o3 = p*Bt*o2
 *
 *  4-  out = A*in + o3
 *
 */
static VEC* spAUGMLAGRMATRIXapply1(SPMAT *J, PERM *Pj, PERM *INVPj,
                                   SPMAT *B,
                                   SPMAT *LLT_J,
                                   SPMAT *A,
                                   double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;
   static VEC *o3 = VNULL;

   o1 = v_resize(o1, B->m);
   o2 = v_resize(o2, B->m);
   o3 = v_resize(o3, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   o1  = sp_mv_mlt(B, in, o1);
   o2  = spCHsolve(LLT_J, o1, o2);
   o3  = sp_vm_mlt(B, o2, o3);
   o3  = sv_mlt(rho, o3, o3);

   out = v_add(o3, sp_mv_mlt(A, in, out), out);

   /*
   printf("spAUGMLAGRMATRIXapply1 \n");
   */
   
   return out;
}

/*
 * spAUGMLAGRMATRIXapply2 : perform  out = R*in with R = A + p*Bt*inv(J)*B
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(J)*o1 ( ie J*o2 = o1 : solve with an PCG iterativ method )
 *  3-  o3 = out = B*o2
 *
 *
 *  4-  out = A*in + o3
 *
 * Note : J is used - ICH_J is the Incomplete Choleski factorisation of J
 *
 */
static VEC* spAUGMLAGRMATRIXapply2(SPMAT *J, PERM *P, PERM *INVP,
                                   SPMAT *B,
                                   SPMAT *ICH_J,
                                   SPMAT *A,
                                   double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;
   static VEC *o3 = VNULL;
   
   PARAMS* MyParams = Params_get_staticparam(0);

   Real eps_steps    = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps;
   int  max_steps    = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.max_steps;
   int  steps;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);
   o3 = v_resize(o3, B->n);
   
   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);
   MEM_STAT_REG(o3,TYPE_VEC);
   
   o1  = sp_vm_mlt(B, in, o1);
   iter_xspcg(J, ICH_J, o1 /* in */, eps_steps, o2 /* out */, max_steps, &steps, NULL);
   o3  = sp_vm_mlt(B, o2, o3);
   o3  = sv_mlt(rho, o3, o3);

   out = v_add(o3, sp_mv_mlt(A, in, out), out);

   /*
   printf("spAUGMLAGRMATRIXapply2 \n");
   */
   
   return out;
}


/*
 * spAUGMLAGRMATRIXapply1_bandwr : perform  out = R*in with R = A + p*Bt*inv(J)*B  with bandwidth reduction
 *
 *  1-  o1 = B*in
 *  2-  o2 = inv(J)*o1 ( ie J*o2 = o1 : solve with cholesky factorisation )
 *  3-  o3 = rho*Bt*o2
 *
 *  4-  o3 += A*in
 *
 * Note : J is unused - LLT_J complete Cholesky factorisation of J
 *
 */
static VEC* spAUGMLAGRMATRIXapply1_bandwr(SPMAT *J, PERM *Pj, PERM *INVPj,
                                          SPMAT *B,
                                          SPMAT *LLT_J,
                                          SPMAT *A,
                                          double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;
   static VEC *o3 = VNULL;

   o1 = v_resize(o1, B->m);
   o2 = v_resize(o2, B->m);
   o3 = v_resize(o3, B->m);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);
   MEM_STAT_REG(o3,TYPE_VEC);

   o1  = sp_mv_mlt(B, in, o1);
   o2  = spCHsolve_bandwr(LLT_J, Pj, INVPj, o1, o2);
   o3  = sp_vm_mlt(B, o2, o3);
   o3  = sv_mlt(rho, o3, o3);

   out = v_add(o3, sp_mv_mlt(A, in, out), out);

   /*
   printf("spAUGMLAGRMATRIXapply1_bandwr \n");
   */
   
   return out;
}

/*
 * spAUGMLAGRMATRIXapply2_bandwr : perform  out = R*in with R = A + p*Bt*inv(J)*B  with bandwidth reduction
 *
 *  1-  o1 = B*in
 *  2-  o2 = inv(J)*o1 ( ie J*o2 = o1 : solve with cholesky factorisation )
 *  3-  o3 = rho*Bt*o2
 *
 *  4-  o3 += A*in
 *
 * Note : J is used - ICH_J complete Cholesky factorisation of J
 *
 */
static VEC* spAUGMLAGRMATRIXapply2_bandwr(SPMAT *J, PERM *Pj, PERM *INVPj,
                                          SPMAT *B,
                                          SPMAT *ICH_J,
                                          SPMAT *A,
                                          double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;
   static VEC *o3 = VNULL;

   static VEC *o1perm = VNULL;
   static VEC *o2perm = VNULL;

   PARAMS* MyParams = Params_get_staticparam(0);

   int  eps_steps    = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps;
   int  max_steps    = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.max_steps;
   int  steps;

   
   o1 = v_resize(o1, B->m);
   o2 = v_resize(o2, B->m);
   o3 = v_resize(o3, B->m);

   o1perm = v_resize(o1perm, B->m);
   o2perm = v_resize(o2perm, B->m);


   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);
   MEM_STAT_REG(o3,TYPE_VEC);

   MEM_STAT_REG(o1perm,TYPE_VEC);
   MEM_STAT_REG(o2perm,TYPE_VEC);

   o1  = sp_mv_mlt(B, in, o1);

   o1perm = px_vec(Pj, o1, o1perm);     /* the permutated matrix must be st.  P * A * P'  */
   iter_xspcg(J, ICH_J, o1perm /* in */, eps_steps, o2perm /* out */, max_steps, &steps, NULL);
   o2 = px_vec(INVPj, o2perm, o2);      /* the permutated matrix must be st.  P * A * P'  */

   o3  = sp_vm_mlt(B, o2, o3);
   o3  = sv_mlt(rho, o3, o3);

   out = v_add(o3, sp_mv_mlt(A, in, out), out);

   /*
   printf("spAUGMLAGRMATRIXapply2_bandwr \n");
   */
   
   return out;
}


int stokes_resol_augmentedlagrangian(SPMAT *A, SPMAT *B, SPMAT *J, VEC *U, VEC *P, VEC *F, VEC *G, Real tol, int max_steps, int *steps)
{
   PARAMS* MyParams   = Params_get_staticparam(0);      
   BANDWRt_METHOD  bandwr_method     = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.bandwidth_option;
   BANDWRt_MATRIXTYPE  bandwr_mtype  = BANDWRe_NONSYM;

   double  rho        = MyParams->stokes_params.augmentedlagrangian.rho;
   double  res1, res2;
   
	/*
   A*U + Bt*P = F
   B*U        = G
   
   J*( P_kp1 - P_k ) = rho.B*U_kp1
   (A + rho.Bt*INVJ*B)*U_kp1 = f - Bt*P_k
    
   */

   VEC *P_k, *P_kp1, *P_tmp, *P_tmp2;
   VEC *U_k, *U_kp1, *U_tmp, *U_tmp2;

   SPMAT *PAPt;
	SPMAT *LLT_PAPt;

   PERM *Pe    = px_get(U->dim);
   PERM *INVPe = px_get(U->dim);

   SPMAT *PJPt;
	SPMAT *LLT_PJPt;

   PERM *Pj    = px_get(P->dim);
   PERM *INVPj = px_get(P->dim);


   ITERSTOKES *ip;
   
   PAPt = sp_copy(A);
   PJPt = sp_copy(J);

   if ( bandwr_option != BANDWRe_NO_OPTION )
   {
      PAPt = sp_dopermutationforbandwr(A, Pe, INVPe, bandwr_method, bandwr_option , bandwr_mtype, PAPt );
      PJPt = sp_dopermutationforbandwr(J, Pj, INVPj, bandwr_method, bandwr_option , bandwr_mtype, PJPt );
   }

	LLT_PAPt = sp_copy(PAPt);
   LLT_PJPt = sp_copy(PJPt);
	
   spCHfactor(LLT_PAPt);
   spCHfactor(LLT_PJPt);

   ip = iter_stokes_get(0,0);

   if ( bandwr_method == BANDWRe_NULL ) /* no band width reduction with permutation */
   {
      if ( strcmp(MyParams->stokes_params.augmentedlagrangian.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spAUGMLAGRMATRIXapply1;
      }
      if ( strcmp(MyParams->stokes_params.augmentedlagrangian.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spAUGMLAGRMATRIXapply2;
      }
   }
   else                                /* yes, permutation for band width reduction */
   {
      if ( strcmp(MyParams->stokes_params.augmentedlagrangian.innerloop_method, "DIRECT-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spAUGMLAGRMATRIXapply1_bandwr;
      }
      if ( strcmp(MyParams->stokes_params.augmentedlagrangian.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         ip->Rx = (Fun_Rx) spAUGMLAGRMATRIXapply2_bandwr;
      }
   }

   ip->R_par1 = (void *)J;
   ip->R_par2 = (void *)Pj;
   ip->R_par3 = (void *)INVPj;
   ip->R_par4 = (void *)B;
   ip->R_par5 = (void *)LLT_PJPt;
   ip->R_par6 = (void *)A;
  
   ip->rho    = rho;

   ip->Px = (Fun_Rx) NULL; /* preconditionning */
   

   ip->info      = iter_stokes_std_info ;
   ip->stop_crit = iter_stokes_std_stop_crit ;

   U_k = v_get(U->dim);
   P_k = v_get(P->dim);

   U_kp1 = v_get(U->dim);
   P_kp1 = v_get(P->dim);

   U_tmp = v_get(U->dim); U_tmp2 = v_get(U->dim);
   P_tmp = v_get(P->dim); P_tmp2 = v_get(P->dim);

   ip->eps   = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.eps_steps;
   ip->limit = MyParams->stokes_params.augmentedlagrangian.innerloop_solver.max_steps;
   ip->x     = U_kp1;

   for (*steps=1 ; *steps <= max_steps ; (*steps)++ )
   {
      /* eval U_kp1 */
      ip->b     = v_sub(F, sp_vm_mlt(B, P_k, U_tmp), U_tmp2);
      iter_stokes_cg(ip);

      /* check convergence */
      U_kp1 = ip->x;

      /* eval P_kp1 */
      spCHsolve_bandwr( LLT_PJPt, Pj, INVPj, sp_mv_mlt(B, U_kp1, P_tmp2), P_tmp);
      sv_mlt(rho, P_tmp, P_tmp);
      P_kp1 = v_add(P_tmp, P_k, P_kp1);

      res1 = v_norm2( v_sub(P_k, P_kp1, P_tmp) );
      res2 = v_norm2( v_sub(U_k, U_kp1, U_tmp) );
      printf("\t\t\t->res1,res2 = %le,%le (iter = %d )\n", res1,res2, *steps);
      
      if ( res2 < tol )
      {
         U = v_copy(U_kp1, U);
         P = v_copy(P_kp1, P);

         return EXIT_SUCCESS;
      }

      U_k = v_copy(U_kp1, U_k);
      P_k = v_copy(P_kp1, P_k);
   }

   if (steps) *steps = ip->steps;

   ip->shared_x = ip->shared_b = TRUE;
   iter_stokes_free(ip);   /* release only ITERSTOKES structure */


   return EXIT_FAILURE;  
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC  *iter_stokes_cg(ITERSTOKES *ip)
{
   static VEC *r = VNULL, *p = VNULL, *q = VNULL, *z = VNULL;
   Real	alpha, beta, inner, old_inner, nres;
   VEC *rr;   /* rr == r or rr == z */
   
   if (ip == ISTOKESNULL)
     error(E_NULL, "iter_stokes_cg");
   if (!ip->Rx || !ip->b)
     error(E_NULL, "iter_stokes_cg");
   if ( ip->x == ip->b )
     error(E_INSITU, "iter_stokes_cg");
   if (!ip->stop_crit)
     error(E_NULL, "iter_stokes_cg");
   
   if ( ip->eps <= 0.0 )
     ip->eps = MACHEPS;
   
   r = v_resize(r,ip->b->dim);
   p = v_resize(p,ip->b->dim);
   q = v_resize(q,ip->b->dim);
   
   MEM_STAT_REG(r,TYPE_VEC);
   MEM_STAT_REG(p,TYPE_VEC);
   MEM_STAT_REG(q,TYPE_VEC);
   
   if (ip->Px != (Fun_Rx)NULL)
   {
      z = v_resize(z,ip->b->dim);
      MEM_STAT_REG(z,TYPE_VEC);
      rr = z;
   }
   else
   {
      rr = r;
   }

   if (ip->x != VNULL)
   {
      if (ip->x->dim != ip->b->dim)
      {
         error(E_SIZES, "iter_stokes_cg");
      }
      
      (ip->Rx)(ip->R_par1,ip->R_par2,ip->R_par3,ip->R_par4,ip->R_par5,ip->R_par6, ip->rho, ip->x,p);    		/* p = A*x */
      v_sub(ip->b,p,r);		 		/* r = b - A*x */
   }
   else
   {  /* ip->x == 0 */
      ip->x = v_get(ip->b->dim);
      ip->shared_x = FALSE;
      v_copy(ip->b,r);
   }
   
   old_inner = 0.0;
   for ( ip->steps = 0; ip->steps <= ip->limit; ip->steps++ )
   {
      if ( ip->Px )
      {
         (ip->Px)(ip->P_par1,ip->P_par2,ip->P_par3,ip->P_par4,ip->P_par5,ip->R_par6, ip->rho, r,rr);		/* rr = B*r */
      }

      inner = in_prod(rr,r);
      nres = sqrt(fabs(inner));

      if (ip->info) ip->info(ip,nres,r,rr);
      if ( ip->stop_crit(ip,nres,r,rr) ) break;
      
      if ( ip->steps )	/* if ( ip->steps > 0 ) ... */
      {
	      beta = inner/old_inner;
	      p = v_mltadd(rr,p,beta,p);
      }
      else		/* if ( ip->steps == 0 ) ... */
      {
	      beta = 0.0;
	      p = v_copy(rr,p);
	      old_inner = 0.0;
      }

      (ip->Rx)(ip->R_par1,ip->R_par2,ip->R_par3,ip->R_par4,ip->R_par5,ip->R_par6, ip->rho, p,q);     /* q = A*p */
      
      alpha = inner/in_prod(p,q);
      v_mltadd(ip->x,p,alpha,ip->x);
      v_mltadd(r,q,-alpha,r);
      old_inner = inner;
   }
   
   return ip->x;
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */

VEC  *iter_stokes_bicgstab(ITERSTOKES *ip)
{
   static VEC *s = VNULL;
   static VEC *p = VNULL;
   static VEC *r = VNULL; static VEC *rr = VNULL;
   static VEC *v = VNULL;
   static VEC *t = VNULL;

   static VEC *z2= VNULL;  /* additional vec if (ip->B) */
   static VEC *z1= VNULL;  /* additional vec if (ip->B) */

   Real rho,old_rho,omega,alpha,beta;
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


   if (ip->Px != (Fun_Rx)NULL) {
      z1 = v_resize(z1,ip->b->dim);
      MEM_STAT_REG(z1,TYPE_VEC);
      ss = z1;
   }
   else ss = s;

   if (ip->Px != (Fun_Rx)NULL) {
      z2 = v_resize(z2,ip->b->dim);
      MEM_STAT_REG(z2,TYPE_VEC);
      pp = z2;
   }
   else pp = p;


   /* initialisation : all residuals scaled with |b| */
   ip->steps = 0; nres = 1.0;
   if (ip->info) ip->info(ip,nres,r,VNULL);
   if (ip->stop_crit(ip,nres,r,VNULL)) goto fin;


   /*v_zero(ip->x);*/

   r = (ip->Rx)(ip->R_par1,ip->R_par2,ip->R_par3,ip->R_par4,ip->R_par5,ip->R_par6, ip->rho, ip->x,r);    		/* p = A*x */
   r = v_sub(ip->b,r,r);

   rr = v_copy(r,rr);


   for ( ip->steps = 1; ip->steps <= ip->limit; ip->steps++ )
   {
      rho = in_prod(rr,r);
      if ( rho==0.0 ) error(E_SING, "iter_stokes_bicgstab");

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

      if ( ip->Px ) (ip->Px)(ip->P_par1,ip->P_par2,ip->P_par3,ip->P_par4,ip->P_par5,ip->P_par6, ip->rho, p,pp);    /* solve B*pp = p */

      v = (ip->Rx)(ip->R_par1,ip->R_par2,ip->R_par3,ip->R_par4,ip->R_par5,ip->R_par6, ip->rho, pp,v);    		/* v = A*pp */
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

      if ( ip->Px ) (ip->Px)(ip->P_par1,ip->P_par2,ip->P_par3,ip->P_par4,ip->P_par5,ip->P_par6, ip->rho, s,ss);     /* solve B*ss = s */

      t = (ip->Rx)(ip->R_par1,ip->R_par2,ip->R_par3,ip->R_par4,ip->R_par5,ip->R_par6, ip->rho, ss,t);    		 /* t = A*ss */
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

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/* iter_stokes_get - create a new structure pointing to ITERSTOKES */

ITERSTOKES *iter_stokes_get(int lenb, int lenx)
{
   ITERSTOKES *ip;

   if ((ip = NEW(ITERSTOKES)) == (ITERSTOKES *) NULL)
   {
      error(E_MEM, "iter_stokes_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes(TYPE_ITER,0,sizeof(ITERSTOKES));
      mem_numvar(TYPE_ITER,1);
   }

   /* default values */
   ip->shared_x = FALSE;
   ip->shared_b = FALSE;
   ip->k = 0;
   ip->limit = ITER_LIMIT_DEF;
   ip->eps = ITER_EPS_DEF;
   ip->steps = 0;

   if (lenb > 0) ip->b = v_get(lenb);
   else ip->b = (VEC *)NULL;

   if (lenx > 0) ip->x = v_get(lenx);
   else ip->x = (VEC *)NULL;

   ip->R_par1 = NULL;
   ip->R_par2 = NULL;
   ip->R_par3 = NULL;
   ip->R_par4 = NULL;
   ip->R_par5 = NULL;
   ip->R_par6 = NULL;
   ip->Rx = (Fun_Rx)NULL;
   ip->P_par1 = NULL;
   ip->P_par2 = NULL;
   ip->P_par3 = NULL;
   ip->P_par4 = NULL;
   ip->P_par5 = NULL;
   ip->P_par6 = NULL;
   ip->Px = (Fun_Rx)NULL;

   ip->info      = NULL;
   ip->stop_crit = NULL; /* to set */

   ip->init_res = 1.0;
   
   return ip;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/* iter_stokes_free - release memory */
int iter_stokes_free(ITERSTOKES *ip)
{
   if (ip == (ITERSTOKES *)NULL)
   {
      return -1;
   }
   if (mem_info_is_on())
   {
      mem_bytes(TYPE_ITER,sizeof(ITERSTOKES),0);
      mem_numvar(TYPE_ITER,-1);
   }

   if ( !ip->shared_x && ip->x != NULL ) v_free(ip->x);
   if ( !ip->shared_b && ip->b != NULL ) v_free(ip->b);

   free((char *)ip);

   return 0;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/* standard information */
void iter_stokes_std_info(ITERSTOKES *ip, double nres, VEC *res, VEC *Bres)
{
   if (nres >= 0.0)
     printf(" %d. residual = %g\n",ip->steps,nres);
   else 
     printf(" %d. residual = %g (WARNING !!! should be >= 0) \n",
	    ip->steps,nres);
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/* standard stopping criterion */
int iter_stokes_std_stop_crit(ITERSTOKES *ip, double nres, VEC *res, VEC *Bres)
{
   /* save initial value of the residual in init_res */
   if (ip->steps == 0)
     ip->init_res = fabs(nres);

   /* standard stopping criterium */
   if (nres <= ip->init_res*ip->eps) return TRUE; 
   return FALSE;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

