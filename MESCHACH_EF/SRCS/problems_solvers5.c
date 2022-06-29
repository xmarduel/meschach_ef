


/** \file problems_solvers5.c
 *
 * Contains algorithms for NavierStokes (2D-3D)
 *
 */

#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"
#include "MESCHACH_ADDS/INCLUDES/spmat2formats.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_2D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_2D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_2D.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_3D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_3D.h"
#include "MESCHACH_EF/INCLUDES/algo_stokes.h"

/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"



/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

void  solve2D_navier_stokes(const ELT_2D *elt2 , const ELT_2D *elt1 , const GEOM_2D *MyGeom, const BC_2D *MyBC, const RHS_2D *MyRhsFun)
{
   VEC *U = v_get(MyGeom->NBSOMM);
   VEC *V = v_get(MyGeom->NBSOMM);
   VEC *P = v_get(MyGeom->geomBase->NBSOMM);

   PARAMS* MyParams = Params_get_staticparam(0);


   (void) solve2D_navier_stokes_(elt2 , elt1 , MyGeom, MyBC, MyRhsFun, U, V, P);

   /* ------------------------------ graphics output ----------------------------------------------- */

   if (MyParams->graph_params.SILO) graphics2D_stokes( "silo" , elt2 , MyGeom , U , V , P , "NavierStokes");
   if (MyParams->graph_params.VTK ) graphics2D_stokes( "vtk"  , elt2 , MyGeom , U , V , P , "NavierStokes");

   /* ------------------------------ graphics output ----------------------------------------------- */


   V_FREE(U);
   V_FREE(V);
   V_FREE(P);

   return;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

void  solve3D_navier_stokes(const ELT_3D *elt2 , const ELT_3D *elt1 , const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun)
{
   VEC *U = v_get(MyGeom->NBSOMM);
   VEC *V = v_get(MyGeom->NBSOMM);
   VEC *W = v_get(MyGeom->NBSOMM);
   VEC *P = v_get(MyGeom->geomBase->NBSOMM);

   PARAMS* MyParams = Params_get_staticparam(0);


   (void) solve3D_navier_stokes_(elt2 , elt1 , MyGeom, MyBC, MyRhsFun, U, V, W, P);

   /* ------------------------------ graphics output ----------------------------------------------- */

   if (MyParams->graph_params.SILO) graphics3D_stokes( "silo" , elt2 , MyGeom , U , V , W , P , "NavierStokes");
   if (MyParams->graph_params.VTK ) graphics3D_stokes( "vtk"  , elt2 , MyGeom , U , V , W , P , "NavierStokes");

   /* ------------------------------ graphics output ----------------------------------------------- */


   V_FREE(U);
   V_FREE(V);
   V_FREE(W);
   V_FREE(P);

   return;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

void  solve2D_navier_stokes_(const ELT_2D *elt2 , const ELT_2D *elt1 , const GEOM_2D *MyGeom, const BC_2D *MyBc, const RHS_2D *MyRhsFun ,
                             VEC *U,
                             VEC *V,
                             VEC *P)
{
   SPMAT *Au;
   SPMAT *Bx, *By;
   SPMAT *C = (SPMAT*)NULL; /* stab matrix */

   SPMAT *J;

   SPMAT *A; /* aggregates Au and Av */
   SPMAT *B; /* aggregates Bx and By */

   SPMAT *STOKES;      /* the whole matrix -> get the matrix + rhs with bc with it */
   VEC   *STOKES_RHS;  /* the whole rhs    -> get the matrix + rhs with bc with it */

   SPMAT *STOKES_bc;      /* the whole matrix -> get the matrix + rhs with bc with it */
   VEC   *STOKES_RHS_bc;  /* the whole rhs    -> get the matrix + rhs with bc with it */

   VEC   *VIT; /* aggregates U and V */

   VEC   *F, *Fu, *Fv;
   VEC   *G;


   GEOM_2D   *Geom_p;

   int NBSOMM_U;
   int NBSOMM_P;


   PARAMS* MyParams = Params_get_staticparam(0);

   Real kappa    = MyParams->phys_params.kappa;
	Real Reynolds = 1.0 / kappa;

   Real eps_steps = MyParams->navierstokes_params.eps_steps;
   int  max_steps = MyParams->navierstokes_params.max_steps;
   int  nb_steps  = 0;

   /* check arguments */
   if ( elt2     == ELT_2D_NULL ) error(E_NULL, "solve2D_navier_stokes_");
   if ( elt1     == ELT_2D_NULL ) error(E_NULL, "solve2D_navier_stokes_");
   if ( MyGeom   == GEOM_2D_NULL) error(E_NULL, "solve2D_navier_stokes_");
   if ( MyBc     == BC_2D_NULL  ) error(E_NULL, "solve2D_navier_stokes_");
   if ( MyRhsFun == RHS_2D_NULL ) error(E_NULL, "solve2D_navier_stokes_");
   if ( U        == VNULL       ) error(E_NULL, "solve2D_navier_stokes_");
   if ( V        == VNULL       ) error(E_NULL, "solve2D_navier_stokes_");
   if ( P        == VNULL       ) error(E_NULL, "solve2D_navier_stokes_");


   /* assign geometry for P */
   Geom_p = MyGeom->geomBase ;

   if ( Geom_p == (GEOM_2D *)NULL )
   {
      printf("Achtung, Geom was a \"P1\" geometry, no child ! \n");
      printf("What is then the geometry for the pressure ? \n");
      error(E_UNKNOWN, "solve2D_navierstokes_");
   }


   NBSOMM_U = MyGeom->NBSOMM ;
   NBSOMM_P = Geom_p->NBSOMM;

   Au = sp_get(NBSOMM_U,NBSOMM_U,10); /* stiffness matrix for one velocity component */

   Bx = sp_get(NBSOMM_P,NBSOMM_U,10);
   By = sp_get(NBSOMM_P,NBSOMM_U,10);

   J  = sp_get(NBSOMM_P,NBSOMM_P,10);  /* mass matrix for the pressure */

   C  = sp_get(NBSOMM_P,NBSOMM_P,10);  /* matrix for stabilisation */
   sp_zero(C); /* actually for stabilisation */

   /* ----- assemblage matrix and rhs ------ */

   Au = assemblage2D_matrix_Stiff1( elt2 , MyGeom , Au );
   sp_smlt(Au, kappa, Au);

   J  = assemblage2D_matrix_Mass  ( elt1 , Geom_p , J  );

   Bx = assemblage2D_matrix_Conv_x_elt2elt1( elt2 , MyGeom , elt1 , Geom_p , Bx );
   By = assemblage2D_matrix_Conv_y_elt2elt1( elt2 , MyGeom , elt1 , Geom_p , By );

   /* -- create "big" matrices & vectors -------------------------------------------- */

   STOKES     = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10);
   STOKES_RHS =  v_get(2*NBSOMM_U + NBSOMM_P);

   STOKES_bc     = sp_get(2*NBSOMM_U + NBSOMM_P, 2*NBSOMM_U + NBSOMM_P, 10);
   STOKES_RHS_bc =  v_get(2*NBSOMM_U + NBSOMM_P);


   STOKES = sp_move(Au, 0,0, NBSOMM_U,NBSOMM_U, STOKES,          0,       0);
   STOKES = sp_move(Au, 0,0, NBSOMM_U,NBSOMM_U, STOKES,   NBSOMM_U,NBSOMM_U);

   STOKES = sp_move(Bx, 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,       0);
   STOKES = sp_move(By, 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,NBSOMM_U);

   /* ---------------------------------------------------------------------------------*/

   Rhs2D_setCurrentSelectedAxe((RHS_2D*)MyRhsFun, AXEe_X);
   Fu = v_get(NBSOMM_U);
   Fu = assemblage2D_vector_fun( elt2, MyGeom, MyRhsFun, Fu);

   Rhs2D_setCurrentSelectedAxe((RHS_2D*)MyRhsFun, AXEe_Y);
   Fv = v_get(NBSOMM_U);
   Fv = assemblage2D_vector_fun( elt2, MyGeom, MyRhsFun, Fv);

   STOKES_RHS = v_move(Fu, 0, NBSOMM_U, STOKES_RHS,        0); /* set Fu into STOKES_RHS */
   STOKES_RHS = v_move(Fv, 0, NBSOMM_U, STOKES_RHS, NBSOMM_U); /* set Fv into STOKES_RHS */

   /* ---------------------------------------------------------------------------------*/

   STOKES_bc     = sp_copy2(STOKES, STOKES_bc);
   STOKES_RHS_bc =   v_copy(STOKES_RHS, STOKES_RHS_bc);

   (void)transform2D_stokes_matrix_vector_with_bc( elt2, MyGeom, Geom_p, MyBc, STOKES_bc, STOKES_RHS_bc);

   /* ---------------------------------------------------------------------------------*/

   A  = sp_get(2*NBSOMM_U, 2*NBSOMM_U, 10); sp_zero(A);
   B  = sp_get(  NBSOMM_P, 2*NBSOMM_U, 10); sp_zero(B);

   VIT = v_get(2*NBSOMM_U);  /* [ U , V ] */
   F   = v_get(2*NBSOMM_U);
   G   = v_get(  NBSOMM_P);


   A = sp_move(STOKES_bc,          0,0, 2*NBSOMM_U,2*NBSOMM_U, A, 0,0);
   B = sp_move(STOKES_bc, 2*NBSOMM_U,0,   NBSOMM_P,2*NBSOMM_U, B, 0,0);

   F = v_move(STOKES_RHS_bc,          0,2*NBSOMM_U, F, 0);
   G = v_move(STOKES_RHS_bc, 2*NBSOMM_U,  NBSOMM_P, G, 0);


   /* ---------------------------------------------- */
   /* ----------- call the algorithms -------------- */
   /* ---------------------------------------------- */
   {
      BANDWRt_METHOD  bandwr_method = MyParams->stokes_params.uzawa.rho;
      BANDWRt_OPTION  bandwr_option = MyParams->stokes_params.uzawa.innerloop_solver.bandwidth_method;
      BANDWRt_MATRIXTYPE  bandwr_mtype = BANDWRe_NONSYM;

      int  s,s1,s2;
      double norm;

      SPMAT *LLT_PAPt ;
      SPMAT *LLT_PJPt ;

		SPMAT *PAPt ;
      SPMAT *PJPt ;

      PERM  *Pu    ;
      PERM  *INVPu ;

      PERM  *Pp    ;
      PERM  *INVPp ;

      int rc;

      VEC *VITold = v_get(VIT->dim);

      VEC *UUx = v_get(NBSOMM_U);
      VEC *VUy = v_get(NBSOMM_U);
      VEC *UVx = v_get(NBSOMM_U);
      VEC *VVy = v_get(NBSOMM_U);

      VEC *F_NLI = v_get(VIT->dim);

      /* allocate mem */
      if ( bandwr_option != BANDWRe_NO_OPTION )
      {
         Pu    = px_get(VIT->dim);
         INVPu = px_get(VIT->dim);

         Pp    = px_get(P->dim);
         INVPp = px_get(P->dim);
      }

      PAPt = sp_copy(A);
      PJPt = sp_copy(J);

      if ( bandwr_option != BANDWRe_NO_OPTION )
      {
         printf("\n... solve2D_navier_stokes_ with bandwidth reduction ...\n");

         /* set the matrix with reduced band width */
         PAPt = sp_dopermutationforbandwr(A , Pu, INVPu, bandwr_method, bandwr_option, bandwr_mtype, PAPt );

         /* set the matrix with reduced band width */
         PJPt = sp_dopermutationforbandwr(J , Pp, INVPp, bandwr_method, bandwr_option, bandwr_mtype, PJPt );
      }
      else
      {
         printf("\n... solve2D_navier_stokes_ without bandwidth reduction ...\n");
      }

      LLT_PAPt = sp_copy(PAPt);
      LLT_PJPt = sp_copy(PJPt);

		spCHfactor(LLT_PAPt);
      spCHfactor(LLT_PJPt);

      /* first Stokes */
      if ( strcmp(MyParams->navierstokes_params.outer_method, "UZAWA") == 0 )
      {
         stokes_resol_uzawa_factOk (PAPt, LLT_PAPt, B, C, PJPt, LLT_PJPt, VIT, P, F, G, eps_steps, max_steps, &nb_steps, Pu, INVPu, Pp, INVPp);
      }
      else
      if ( strcmp(MyParams->navierstokes_params.outer_method, "PRESSUREMATRIX") == 0 )
      {
         stokes_resol_pressurematrix_factOk(PAPt, LLT_PAPt, B, VIT, P, F, G, eps_steps, max_steps, &nb_steps, Pu, INVPu);
      }
      else
      {
         printf("method for NavierStokes is -%s-\n", MyParams->navierstokes_params.outer_method );
         error(E_UNKNOWN, "solve2D_navierstokes_");
      }

      if(1) {
      /* then with the non-liner terms */
      for (nb_steps=1 ; nb_steps <= max_steps ; nb_steps++ )
      {
         VITold = v_copy(VIT, VITold);

         /* get back velocity */
         U = v_move(VIT,        0,NBSOMM_U, U,0);
         V = v_move(VIT, NBSOMM_U,NBSOMM_U, V,0);

         /* build the non-linear terms */
         UUx = assemblage2D_vector_abx( elt2 , MyGeom , U , U , UUx );
         VUy = assemblage2D_vector_aby( elt2 , MyGeom , V , U , VUy );

         UVx = assemblage2D_vector_abx( elt2 , MyGeom , U , V , UVx );
         VVy = assemblage2D_vector_aby( elt2 , MyGeom , V , V , VVy );

         STOKES_RHS = v_zero(STOKES_RHS);
         /* build the non-linear rhs */
         for (s=0; s<NBSOMM_U; s++)
         {
            s1 = s;
            s2 = s + NBSOMM_U;

            STOKES_RHS->ve[s1] = Fu->ve[s] - UUx->ve[s] - VUy->ve[s] ;
            STOKES_RHS->ve[s2] = Fv->ve[s] - UVx->ve[s] - VVy->ve[s] ;
         }

         STOKES_RHS_bc = v_copy(STOKES_RHS, STOKES_RHS_bc);

         STOKES_RHS_bc = transform2D_stokes_vector_with_bc( elt2, MyGeom, Geom_p, MyBc, STOKES, STOKES_RHS_bc);

         F = v_move(STOKES_RHS_bc,          0,2*NBSOMM_U, F, 0);
         G = v_move(STOKES_RHS_bc, 2*NBSOMM_U,  NBSOMM_P, G, 0);

         /* ---------------------------------------------- */
         /* ----------- call the algorithms -------------- */
         /* ---------------------------------------------- */

         if ( strcmp(MyParams->navierstokes_params.outer_method, "UZAWA") == 0 )
         {
            rc = stokes_resol_uzawa_factOk (PAPt, LLT_PAPt, B, C, PJPt, LLT_PJPt, VIT, P, F_NLI, G, eps_steps, max_steps, &nb_steps, Pu, INVPu, Pp, INVPp);
         }
         else
         if ( strcmp(MyParams->navierstokes_params.outer_method, "PRESSUREMATRIX") == 0 )
         {
            rc = stokes_resol_pressurematrix_factOk(PAPt, LLT_PAPt, B, VIT, P, F_NLI, G, eps_steps, max_steps, &nb_steps, Pu, INVPu);
         }
         else
         {
            printf("method for NavierStokes is -%s-\n", MyParams->stokes_params.method);
            error(E_UNKNOWN, "solve2D_navierstokes_");
         }

         /* test convergence */
         norm = v_norm2( v_sub(VIT, VITold, VITold) );
         printf("NS iter %d -> diff = %le\n", nb_steps, norm);

         if ( norm < eps_steps )
         {
            break;
         }
      }
      } /* if (0) */

      SP_FREE(PAPt);
      SP_FREE(PJPt);

		SP_FREE(LLT_PAPt);
      SP_FREE(LLT_PJPt);

      PX_FREE(Pu); PX_FREE(INVPu);
      PX_FREE(Pp); PX_FREE(INVPp);

      V_FREE(VITold);

      V_FREE(UUx);
      V_FREE(VUy);
      V_FREE(UVx);
      V_FREE(VVy);

      V_FREE(F_NLI);
   }

   /* get back velocity */
   U = v_move(VIT,        0,NBSOMM_U, U,0);
   V = v_move(VIT, NBSOMM_U,NBSOMM_U, V,0);

   /* ------- free memory ------------------ */

   SP_FREE(Au);
   SP_FREE(J);

   V_FREE(F);
   V_FREE(G);

   SP_FREE(A);
   SP_FREE(B);
   SP_FREE(C);

   SP_FREE(STOKES);
   SP_FREE(STOKES_bc);

   V_FREE(STOKES_RHS);
   V_FREE(STOKES_RHS_bc);

   return;
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

void  solve3D_navier_stokes_(const ELT_3D *elt2 , const ELT_3D *elt1 , const GEOM_3D *MyGeom, const BC_3D *MyBc, const RHS_3D *MyRhsFun ,
                             VEC *U,
                             VEC *V,
                             VEC *W,
                             VEC *P)
{
   SPMAT *Au, *Av, *Aw;
   SPMAT *Bx, *By, *Bz;
   SPMAT *C = (SPMAT*)NULL; /* stab. matrix */
   SPMAT *J;

   SPMAT *A; /* aggregates Au , Av and Aw */
   SPMAT *B; /* aggregates Bx , By and Bz */

   VEC   *VIT; /* agrregates U , V and W  */

   VEC   *Fu;
   VEC   *Fv;
   VEC   *Fw;

   VEC   *F;
   VEC   *G;

   SPMAT *STOKES;      /* the whole matrix -> get the matrix + rhs with bc with it */
   VEC   *STOKES_RHS;  /* the whole rhs    -> get the matrix + rhs with bc with it */

   SPMAT *STOKES_bc;      /* the whole matrix -> get the matrix + rhs with bc with it */
   VEC   *STOKES_RHS_bc;  /* the whole rhs    -> get the matrix + rhs with bc with it */

   int NBSOMM_U = MyGeom->NBSOMM ;
   int NBSOMM_P;


   PARAMS* MyParams = Params_get_staticparam(0);

   Real kappa     = MyParams->phys_params.kappa;
   Real Reynolds  = 1.0 / kappa;

   Real eps_steps = MyParams->navierstokes_params.eps_steps;
   int  max_steps = MyParams->navierstokes_params.max_steps;
   int  nb_steps  = 1;

   GEOM_3D   *Geom_p = MyGeom->geomBase ;

   if ( Geom_p == (GEOM_3D *)NULL )
   {
      printf("Achtung, Geom was a \"P1\" geometry, no child ! \n");
      printf("What is then the geometry for the pressure ? \n");
      error(E_UNKNOWN, "solve3D_stokes");
   }
   else
   {
      NBSOMM_P = Geom_p->NBSOMM;
   }


   Au  = sp_get(NBSOMM_U,NBSOMM_U,10); /* stiffness matrix for the velocity */
   Av  = sp_get(NBSOMM_U,NBSOMM_U,10); /* stiffness matrix for the velocity */
   Aw  = sp_get(NBSOMM_U,NBSOMM_U,10); /* stiffness matrix for the velocity */

   Bx = sp_get(NBSOMM_P,NBSOMM_U,10);
   By = sp_get(NBSOMM_P,NBSOMM_U,10);
   Bz = sp_get(NBSOMM_P,NBSOMM_U,10);

   J  = sp_get(NBSOMM_P,NBSOMM_P,10);  /* mass matrix for the pressure */


   /* ----- assemblage matrix and rhs ------ */

   Au  = assemblage3D_matrix_Stiff1( elt2 , MyGeom , Au );
   sp_smlt(Au, kappa, Au);

   sp_copy2(Au,Av);
   sp_copy2(Au,Aw);

   J  = assemblage3D_matrix_Mass  ( elt1 , Geom_p , J );

   Bx = assemblage3D_matrix_Conv_x_elt2elt1( elt2 , MyGeom , elt1 , Geom_p , Bx );
   By = assemblage3D_matrix_Conv_y_elt2elt1( elt2 , MyGeom , elt1 , Geom_p , By );
   Bz = assemblage3D_matrix_Conv_z_elt2elt1( elt2 , MyGeom , elt1 , Geom_p , Bz );

   /* -- create "big" matrices & vectors -------------------------------------------- */

   STOKES     = sp_get(3*NBSOMM_U + NBSOMM_P, 3*NBSOMM_U + NBSOMM_P, 10);
   STOKES_RHS =  v_get(3*NBSOMM_U + NBSOMM_P);

   STOKES_bc     = sp_get(3*NBSOMM_U + NBSOMM_P, 3*NBSOMM_U + NBSOMM_P, 10);
   STOKES_RHS_bc =  v_get(3*NBSOMM_U + NBSOMM_P);


   STOKES = sp_move(Au, 0,0, NBSOMM_U,NBSOMM_U, STOKES,            0,         0);
   STOKES = sp_move(Av, 0,0, NBSOMM_U,NBSOMM_U, STOKES,   1*NBSOMM_U,1*NBSOMM_U);
   STOKES = sp_move(Aw, 0,0, NBSOMM_U,NBSOMM_U, STOKES,   2*NBSOMM_U,2*NBSOMM_U);

   STOKES = sp_move(Bx, 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,         0);
   STOKES = sp_move(By, 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,1*NBSOMM_U);
   STOKES = sp_move(Bz, 0,0, NBSOMM_P,NBSOMM_U, STOKES, 2*NBSOMM_U,2*NBSOMM_U);

   /* ---------------------------------------------------------------------------------*/

   Rhs3D_setCurrentSelectedAxe((RHS_3D*)MyRhsFun, AXEe_X);
   Fu = v_get(NBSOMM_U);
   Fu = assemblage3D_vector_fun( elt2, MyGeom, MyRhsFun, Fu);

   Rhs3D_setCurrentSelectedAxe((RHS_3D*)MyRhsFun, AXEe_Y);
   Fv = v_get(NBSOMM_U);
   Fv = assemblage3D_vector_fun( elt2, MyGeom, MyRhsFun, Fv);

   Rhs3D_setCurrentSelectedAxe((RHS_3D*)MyRhsFun, AXEe_Z);
   Fw = v_get(NBSOMM_U);
   Fw = assemblage3D_vector_fun( elt2, MyGeom, MyRhsFun, Fw);


   STOKES_RHS = v_move(Fu, 0, NBSOMM_U, STOKES_RHS,          0); /* set Fu into STOKES_RHS */
   STOKES_RHS = v_move(Fv, 0, NBSOMM_U, STOKES_RHS, 1*NBSOMM_U); /* set Fv into STOKES_RHS */
   STOKES_RHS = v_move(Fw, 0, NBSOMM_U, STOKES_RHS, 2*NBSOMM_U); /* set Fw into STOKES_RHS */

   /* ---------------------------------------------------------------------------------*/

   STOKES_bc     = sp_copy2(STOKES, STOKES_bc);
   STOKES_RHS_bc =   v_copy(STOKES_RHS, STOKES_RHS_bc);

   (void)transform3D_stokes_matrix_vector_with_bc( elt2, MyGeom, Geom_p, MyBc, STOKES_bc, STOKES_RHS_bc);

   /* ---------------------------------------------------------------------------------*/

   A  = sp_get(3*NBSOMM_U, 3*NBSOMM_U, 10); sp_zero(A);
   B  = sp_get(  NBSOMM_P, 3*NBSOMM_U, 10); sp_zero(B);

   VIT = v_get(3*NBSOMM_U);  /* [ U , V , W ] */
   F   = v_get(3*NBSOMM_U);
   G   = v_get(  NBSOMM_P);


   A = sp_move(STOKES_bc,          0,0, 3*NBSOMM_U,3*NBSOMM_U, A, 0,0);
   B = sp_move(STOKES_bc, 3*NBSOMM_U,0,   NBSOMM_P,3*NBSOMM_U, B, 0,0);

   F = v_move(STOKES_RHS_bc,          0,3*NBSOMM_U, F, 0);
   G = v_move(STOKES_RHS_bc, 3*NBSOMM_U,  NBSOMM_P, G, 0);





   if ( strcmp(MyParams->navierstokes_params.outer_method, "UZAWA") == 0 )
   {
      stokes_resol_uzawa (A, B, C, J, VIT, P, F, G, eps_steps, max_steps, &nb_steps);
   }
   else
   if ( strcmp(MyParams->navierstokes_params.outer_method, "PRESSUREMATRIX") == 0 )
   {
      stokes_resol_pressurematrix(A, B, VIT, P, F, G, eps_steps, max_steps, &nb_steps);
   }
   else
   {
      error(E_UNKNOWN, "solve3D_navier_stokes_");
   }


   /* get back velocity */
   U = v_move(VIT, 0*NBSOMM_U,NBSOMM_U, U,0);
   V = v_move(VIT, 1*NBSOMM_U,NBSOMM_U, V,0);
   W = v_move(VIT, 2*NBSOMM_U,NBSOMM_U, W,0);

   /* ------- free memory ------------------ */

   SP_FREE(J) ;

   V_FREE(F);
   V_FREE(G);

   SP_FREE(A); SP_FREE(Au); SP_FREE(Av); SP_FREE(Aw);
   SP_FREE(B); SP_FREE(Bx); SP_FREE(By); SP_FREE(Bz);

   V_FREE(Fu);
   V_FREE(Fv);
   V_FREE(Fw);

   SP_FREE(STOKES);
   SP_FREE(STOKES_bc);

   V_FREE(STOKES_RHS);
   V_FREE(STOKES_RHS_bc);

   /* ------------------ end ----------------------- */

   return;
}
