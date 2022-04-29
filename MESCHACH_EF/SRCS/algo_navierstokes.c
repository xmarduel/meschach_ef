
/** \file algo_navierstokes.c
 *
 * Contains algorithms for NavierStokes
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/algo_stokes.h"
#include "MESCHACH_EF/INCLUDES/algo_navierstokes.h"


/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

static VEC* spPRESSUREMATRIXapply1       (SPMAT *A, SPMAT *LU_A, PERM *P, PERM *INVP, SPMAT *B, PERM *P_LU_A, double rho, VEC* in, VEC *out);
static VEC* spPRESSUREMATRIXapply2       (SPMAT *A, SPMAT *LU_A, PERM *P, PERM *INVP, SPMAT *B, PERM *P_LU_A, double rho, VEC* in, VEC *out);
static VEC* spPRESSUREMATRIXapply1_bandwr(SPMAT *A, SPMAT *LU_A, PERM *P, PERM *INVP, SPMAT *B, PERM *P_LU_A, double rho, VEC* in, VEC *out);
static VEC* spPRESSUREMATRIXapply2_bandwr(SPMAT *A, SPMAT *LU_A, PERM *P, PERM *INVP, SPMAT *B, PERM *P_LU_A, double rho, VEC* in, VEC *out);

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

int navierstokes_resol_uzawa (SPMAT *A, SPMAT *B, SPMAT *C, SPMAT *PRECOND, VEC *VIT, VEC *P, VEC *F, VEC *G)
{
   PARAMS* MyParams = Params_get_staticparam(0);      
      
   BANDWRt_METHOD  bandwr_method     = MyParams->navierstokes_params.uzawa.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->navierstokes_params.uzawa.innerloop_solver.bandwidth_option;
   BANDWRt_MATRIXTYPE bandwr_mtype   = BANDWRe_NONSYM;   
	
   SPMAT *LU_A ;
   SPMAT *LU_P ;

   PERM  *Pu    ;
   PERM  *INVPu ;

   PERM  *Pp    ;
   PERM  *INVPp ;
   
   PERM *PERM_LU_A;
   PERM *PERM_LU_P;

   int rc;

   /* check */
   if ( ! A ) error (E_NULL, "navierstokes_resol_uzawa");
   if ( ! B ) error (E_NULL, "navierstokes_resol_uzawa");
   /*if ( ! C ) error (E_NULL, "navierstokes_resol_uzawa"); */
   if ( ! PRECOND ) error (E_NULL, "navierstokes_resol_uzawa");
   if ( ! VIT ) error (E_NULL, "navierstokes_resol_uzawa");
   if ( ! P ) error (E_NULL, "navierstokes_resol_uzawa");
   if ( ! F ) error (E_NULL, "navierstokes_resol_uzawa");
   if ( ! G ) error (E_NULL, "navierstokes_resol_uzawa");

   /* allocate mem */
   if ( bandwr_method != BANDWRe_NULL )
   {
      Pu    = px_get(VIT->dim);
      INVPu = px_get(VIT->dim);

      Pp    = px_get(P->dim);
      INVPp = px_get(P->dim);
   }

   PERM_LU_A = px_get(VIT->dim);
   PERM_LU_P = px_get(P->dim);

   LU_A = sp_copy(A);

   LU_P = sp_copy(PRECOND);

   
   if ( bandwr_method != BANDWRe_NULL )
   {
      printf("\n... navierstokes_resol_uzawa with bandwidth reduction ...\n");

      /* set the matrix with reduced band width */
      LU_A = sp_dopermutationforbandwr(A      , Pu, INVPu, bandwr_method, bandwr_option , bandwr_mtype , LU_A );

      /* set the matrix with reduced band width */
      LU_P = sp_dopermutationforbandwr(PRECOND, Pp, INVPp, bandwr_method, bandwr_option , bandwr_mtype , LU_P );
   }
   else
   {
      printf("\n... navierstokes_resol_uzawa without bandwidth reduction ...\n");
   }

   spLUfactor(LU_A, PERM_LU_A, 0.5);
   spLUfactor(LU_P, PERM_LU_P, 0.5);


   rc = navierstokes_resol_uzawa_factOk (LU_A, PERM_LU_A, B, C, LU_P, PERM_LU_P, VIT, P,
                                         F, G,
                                         Pu, INVPu, Pp, INVPp);

   

   SP_FREE(LU_A);
   SP_FREE(LU_P);

   PX_FREE(Pu); PX_FREE(INVPu);
   PX_FREE(Pp); PX_FREE(INVPp);

   PX_FREE(PERM_LU_A);
   PX_FREE(PERM_LU_P);

   return rc;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

int navierstokes_resol_uzawa_factOk (SPMAT *LU_A, PERM *PERM_LU_A, SPMAT *B, SPMAT *C, SPMAT *LU_P, PERM *PERM_LU_P, VEC *U, VEC *P,
                                     VEC *F, VEC *G,
                                     PERM  *Pu, PERM  *INVPu, PERM  *Pp, PERM  *INVPp )
{
   PARAMS* MyParams = Params_get_staticparam(0);      
   Real rho         = MyParams->navierstokes_params.uzawa.rho;  /* condition on rho follows */

   BANDWRt_METHOD  bandwr_method     = MyParams->navierstokes_params.uzawa.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->navierstokes_params.uzawa.innerloop_solver.bandwidth_option;

	Real tol      = MyParams->navierstokes_params.uzawa.innerloop_solver.eps_steps;
	int max_steps = MyParams->navierstokes_params.uzawa.innerloop_solver.max_steps;
	int steps     = 1;
	
   Real residu = 1. ;

   VEC *oldU, *oldP;
   VEC *R   , *R1  ;
   VEC *tmpU;

   /* check */
   if ( ! LU_A ) error (E_NULL, "navierstokes_resol_uzawa_factOk");
   if ( ! B ) error (E_NULL, "navierstokes_resol_uzawa_factOk");
   /*if ( ! C ) error (E_NULL, "navierstokes_resol_uzawa_factOk"); */
   if ( ! LU_P ) error (E_NULL, "navierstokes_resol_uzawa_factOk");
   if ( ! U ) error (E_NULL, "navierstokes_resol_uzawa_factOk");
   if ( ! P ) error (E_NULL, "navierstokes_resol_uzawa_factOk");
   if ( ! F ) error (E_NULL, "navierstokes_resol_uzawa_factOk");
   if ( ! G ) error (E_NULL, "navierstokes_resol_uzawa_factOk");

   /* allocate mem */
   oldU = v_get(U->dim);
   oldP = v_get(P->dim);

   R = v_get(G->dim);
   R1= v_get(G->dim);

   tmpU = v_get(U->dim);


   for (steps=1 ; steps <= max_steps ; steps++ )
   {
      oldP = v_copy(P, oldP);
      oldU = v_copy(U, oldU);


      v_sub(F, sp_vm_mlt(B,P,tmpU), tmpU);
      /*int status_ms = m_solver(a, ap, u, f - b.trans_mult(p));*/  /* A*U = F - Bt*P    -> U */  
      if ( bandwr_method != BANDWRe_NULL )
      {
         spLUsolve_bandwr(LU_A, PERM_LU_A, Pu, INVPu, tmpU, U);  
      }
      else
      {
         spLUsolve(LU_A, PERM_LU_A, tmpU, U); 
      }

      /*Vector r = I.solve(b*u - g + c*p);*/                        /* PRECOND*P = B*U + C*P - G  -> P */
      R = sp_mv_mlt(B, U, R);
      R = v_sub(R, G, R);
      /* R = v_add(R, sp_mv_mlt(C,P,tmpP), R); */

      /* >>> xm */
      if ( bandwr_method != BANDWRe_NULL )
      {
         spLUsolve_bandwr(LU_P, PERM_LU_P, Pp, INVPp, R, R1);
      }
      else
      {
         spLUsolve(LU_P, PERM_LU_P, R, R1);
      }
      /* <<< xm */

      /*p = p + rho*r;*/
      v_add(P, sv_mlt(rho,R1,R), P);


      if ( steps%10 == 0 )
      {
         printf("iter %05d :", steps);

         printf("norm(p-oldp)= %le   ", v_norm2(v_sub(P, oldP, oldP)) ); 
         printf("norm(u-oldu)= %le \n", v_norm2(v_sub(U, oldU, oldU)) );
      }
      
      residu = v_norm2(R);

      if (residu <= tol && steps > 2)
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


/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

int navierstokes_resol_pressurematrix       (SPMAT *A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G)
{
   PARAMS* MyParams   = Params_get_staticparam(0);

   BANDWRt_METHOD  bandwr_method     = MyParams->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_method;
   BANDWRt_OPTION  bandwr_option     = MyParams->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_option;
   BANDWRt_MATRIXTYPE bandwr_mtype   = BANDWRe_NONSYM;

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

   SPMAT *LU_A = sp_copy(A);
   PERM  *PERM_LU_A = px_get(U->dim);

   PERM *Pu    = px_get(U->dim);
   PERM *INVPu = px_get(U->dim);

   printf("bandwr_method = %d \n", bandwr_method);

   if ( bandwr_method != BANDWRe_NULL )
   {
      LU_A = sp_dopermutationforbandwr(A, Pu, INVPu, bandwr_method, bandwr_option , bandwr_mtype , LU_A );
   }

   spLUfactor(LU_A, PERM_LU_A, 0.5);

   /* main algo */
   rc = navierstokes_resol_pressurematrix_factOk(A, LU_A, PERM_LU_A, B, U, P, F, G, Pu, INVPu );

   /* free mem */
   SP_FREE(LU_A);

   PX_FREE(Pu);
   PX_FREE(INVPu);

   PX_FREE(PERM_LU_A);
   
   /* finito */
   return rc;  
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*
 * perform  out = R*in with R = B*inv(A)*Bt
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with lu factorisation )
 *  3-  o3 = out = B*o2
 *
 */
static VEC* spPRESSUREMATRIXapply1(SPMAT *A, SPMAT *LU_A, PERM *P, PERM *INVP,
                                   SPMAT *B,
                                   PERM  *PERM_LU_A,
                                   double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   o1  = sp_vm_mlt(B, in, o1);
   o2  = spLUsolve(LU_A, PERM_LU_A, o1, o2);
   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*
 * perform  out = R*in with R = B*inv(A)*Bt
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with BiCGStab + Precond iterativ method )
 *  3-  o3 = out = B*o2
 *
 */
static VEC* spPRESSUREMATRIXapply2(SPMAT *A, SPMAT *ILU_A, PERM *P, PERM *INVP,
                                   SPMAT *B,
                                   PERM  *PERM_LU_A,
                                   double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   PARAMS* MyParams = Params_get_staticparam(0);

   Real eps_steps    = MyParams->resol_params.eps_steps;
   int  max_steps    = MyParams->resol_params.max_steps;
   int  steps;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   o1  = sp_vm_mlt(B, in, o1);
   o2  = iter_xspbicgstab(A, ILU_A, o1, eps_steps, o2, max_steps, &steps, NULL);
   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*
 * perform  out = R*in with R = B*inv(A)*Bt  with bandwidth reduction
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : solve with lu factorisation )
 *  3-  o3 = out = B*o2
 *
 */
static VEC* spPRESSUREMATRIXapply1_bandwr(SPMAT *A, SPMAT *LU_A, PERM *P, PERM *INVP,
                                          SPMAT *B,
                                          PERM  *PERM_LU_A,
                                          double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   o1  = sp_vm_mlt(B, in, o1);
   o2  = spLUsolve_bandwr(LU_A, PERM_LU_A, P, INVP, o1, o2);
   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*
 * perform  out = R*in with R = B*inv(A)*Bt  with bandwidth reduction
 *
 *  1-  o1 = Bt*in
 *  2-  o2 = inv(A)*o1 ( ie A*o2 = o1 : Solve with BiCGStab + Precond iterativ method )
 *  3-  o3 = out = B*o2
 *
 */
static VEC* spPRESSUREMATRIXapply2_bandwr(SPMAT *A, SPMAT *ILU_A, PERM *P, PERM *INVP,
                                          SPMAT *B,
                                          PERM  *PERM_LU_A,
                                          double rho, VEC* in, VEC *out)
{
   static VEC *o1 = VNULL;
   static VEC *o2 = VNULL;

   PARAMS* MyParams = Params_get_staticparam(0);

   Real eps_steps    = MyParams->resol_params.eps_steps;
   int  max_steps    = MyParams->resol_params.max_steps;
   int  steps;

   o1 = v_resize(o1, B->n);
   o2 = v_resize(o2, B->n);

   MEM_STAT_REG(o1,TYPE_VEC);
   MEM_STAT_REG(o2,TYPE_VEC);

   o1  = sp_vm_mlt(B, in, o1);
   iter_xspbicgstab_bandwr(A, ILU_A, P, INVP, o1, eps_steps, o2, max_steps, &steps, NULL);
   out = sp_mv_mlt(B, o2, out);

   return out;
}

/*------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------*/

int navierstokes_resol_pressurematrix_factOk(SPMAT *A, SPMAT *LU_A , PERM *PERM_LU_A, SPMAT *B, VEC *U, VEC *P, VEC *F, VEC *G,
                                             PERM *Pu, PERM *INVPu)
{
   PARAMS* MyParams   = Params_get_staticparam(0);
   
   int  bandwr_method     = MyParams->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_method;
   int  bandwr_option     = MyParams->navierstokes_params.pressurematrix.innerloop_solver.bandwidth_option;

	Real tol      = MyParams->navierstokes_params.pressurematrix.innerloop_solver.eps_steps;
	int max_steps = MyParams->navierstokes_params.pressurematrix.innerloop_solver.max_steps;
	int steps     = 1;
	
	int inner_step = 1;
   /*
    A*U + Bt*P = F
    B*U        = G

    U = inv(A)[ F - Bt*P ]
    B*U = - B*inv(A)*Bt*P + B*inv(A)*F = G

    B*inv(A)*Bt*P = B*inv(A)*F - G

    Solve R*P = H  with CG : R = B*inv(A)*Bt ;
    H = B*inv(A)*F - G ;
    */

   VEC *Ptmp;
   VEC *Utmp;

   ITERSTOKES *ip;

   Ptmp = v_get(P->dim);
   Utmp = v_get(U->dim);

   Ptmp = sv_mlt(-1.0, G, Ptmp);

   if ( bandwr_method == BANDWRe_NULL )
   {
		if ( strcmp(MyParams->navierstokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
			Utmp = spLUsolve(LU_A, PERM_LU_A, F, Utmp);
		}
		if ( strcmp(MyParams->navierstokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
         Utmp = spLUsolve(LU_A, PERM_LU_A, F, Utmp);
			printf("navier stokes : stokes pressurematrix resolution: innerloop only with direct method");
			//iter_xspcg(A, LLT_A, F, tol, Utmp, max_steps, &inner_step, NULL);
      }
   }
   else
   {
      if ( strcmp(MyParams->navierstokes_params.pressurematrix.innerloop_method, "DIRECT-METHOD") == 0 )
      {
			Utmp = spLUsolve_bandwr(LU_A, PERM_LU_A, Pu, INVPu, F, Utmp);
		}
		if ( strcmp(MyParams->navierstokes_params.pressurematrix.innerloop_method, "ITERATIV-METHOD") == 0 )
      {
			Utmp = spLUsolve_bandwr(LU_A, PERM_LU_A, Pu, INVPu, F, Utmp);
			printf("navier stokes : stokes pressurematrix resolution: innerloop only with direct method");
			//iter_xspcg_bandwr(A, LLT_A, Pu, INVPu, F, tol, Utmp, max_steps, &inner_step, NULL);
      }
   }

   Ptmp = sp_mv_mlt(B, Utmp, Ptmp);
   Ptmp = v_sub(Ptmp, G, Ptmp);

   ip = iter_stokes_get(0,0);

   if ( bandwr_method != BANDWRe_NULL )
   {
      ip->Rx = (Fun_Rx) spPRESSUREMATRIXapply1_bandwr;
   }
   else
   {
      ip->Rx = (Fun_Rx) spPRESSUREMATRIXapply1;
   }

   ip->R_par1 = (void *)A;
   ip->R_par2 = (void *)LU_A;
   ip->R_par3 = (void *)Pu;
   ip->R_par4 = (void *)INVPu;
   ip->R_par5 = (void *)B;
   ip->R_par6 = (void *)PERM_LU_A;

   ip->rho = 0.0;

   ip->Px = (Fun_Rx) NULL; /* preconditionning */


   ip->info      = iter_stokes_std_info ;
   ip->stop_crit = iter_stokes_std_stop_crit ;

   ip->b     = Ptmp;
   ip->eps   = tol;
   ip->limit = max_steps;
   ip->x     = P;

   iter_stokes_bicgstab(ip);

   P = ip->x;
   steps = ip->steps;

   ip->shared_x = ip->shared_b = TRUE;
   iter_stokes_free(ip);   /* release only ITERSTOKES structure */


   /* and the velocity */
   sp_vm_mlt(B,P,Utmp);

   if ( bandwr_method != BANDWRe_NULL )
   {
      spLUsolve_bandwr(LU_A, PERM_LU_A, Pu, INVPu, v_sub(F, Utmp, Utmp), U);
   }
   else
   {
      spLUsolve(LU_A, PERM_LU_A, v_sub(F, Utmp, Utmp), U);
   }

   /* clean */
   V_FREE(Ptmp);
   V_FREE(Utmp);
   
   /* finito */
   return EXIT_SUCCESS; 
}


