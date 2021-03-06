
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_factor.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_3D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_mtxpattern.h"
/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"

/** \file problems_solvers3.c
 *
 * Contains the drivers functions for linear problems in 3D
 *
 */

typedef enum
{
   PRBLMe_3D_LAPLACIAN   = 0 ,
   PRBLMe_3D_BILAPLACIAN = 1 ,
   PRBLMe_3D_ADVECTION   = 2 ,
   PRBLMe_3D_HELMHOLZ    = 3 ,
   PRBLMe_3D_BURGERS     = 4

} PRBLMt_3D_TYPE;

static VEC *_solve3D_lin(PRBLMt_3D_TYPE problem, const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, const ADV_3D *MyAdvFun);
static VEC *_solve3D_nli(PRBLMt_3D_TYPE problem, const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, const ADV_3D *MyAdvFun);

static void dump_matrix(const SPMAT *A, const char *header);
static void dump_vector(const VEC   *b, const char *header);

static void show_matrix(const SPMAT *A, const char *legend);

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve3D_laplacian(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve3D_laplacian");
   if ( MyGeom   == NULL ) error(E_NULL, "solve3D_laplacian");
   if ( MyBC     == NULL ) error(E_NULL, "solve3D_laplacian");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve3D_laplacian");

   return _solve3D_lin(PRBLMe_3D_LAPLACIAN, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve3D_bilaplacian(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve3D_bilaplacien");
   if ( MyGeom   == NULL ) error(E_NULL, "solve3D_bilaplacien");
   if ( MyBC     == NULL ) error(E_NULL, "solve3D_bilaplacien");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve3D_bilaplacien");

   return _solve3D_lin(PRBLMe_3D_BILAPLACIAN, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve3D_convection_diffusion(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, const ADV_3D *MyAdvFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve3D_convection_diffusion");
   if ( MyGeom   == NULL ) error(E_NULL, "solve3D_convection_diffusion");
   if ( MyBC     == NULL ) error(E_NULL, "solve3D_convection_diffusion");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve3D_convection_diffusion");
   if ( MyAdvFun == NULL ) error(E_NULL, "solve3D_convection_diffusion");

   return _solve3D_lin(PRBLMe_3D_ADVECTION, MyElt, MyGeom, MyBC, MyRhsFun, MyAdvFun);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve3D_helmholz(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve3D_helmholz");
   if ( MyGeom   == NULL ) error(E_NULL, "solve3D_helmholz");
   if ( MyBC     == NULL ) error(E_NULL, "solve3D_helmholz");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve3D_helmholz");

   return _solve3D_lin(PRBLMe_3D_HELMHOLZ, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve3D_burgers(const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve3D_burgers");
   if ( MyGeom   == NULL ) error(E_NULL, "solve3D_burgers");
   if ( MyBC     == NULL ) error(E_NULL, "solve3D_burgers");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve3D_burgers");

   return _solve3D_nli(PRBLMe_3D_BURGERS, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/* ---------------------------------------------------------------------------------------------------------------- */
/* ---------------------------------------------------------------------------------------------------------------- */

static VEC* _solve3D_lin(PRBLMt_3D_TYPE problem, const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, const ADV_3D *MyAdvFun)
{
   SPMAT *A;
   SPMAT *C;
   SPMAT *ILU = (SPMAT *)NULL ;
   SPMAT *ICH = (SPMAT *)NULL ;

   SPMAT *STIFF1;
   SPMAT *CONV_X;
   SPMAT *CONV_Y;
   SPMAT *CONV_Z;
   SPMAT *M_BC;

   VEC  *SOL;

   VEC  *RHS;
   VEC  *RHS_FUN;
   VEC  *RHS_BC;

   PARAMS* MyParams = Params_get_staticparam(0);

   Real eps_steps = MyParams->resol_params.eps_steps;
   int  max_steps = MyParams->resol_params.max_steps;
   int  nb_steps  = MyParams->resol_params.nb_steps;

   char *ResolutionMethod = MyParams->resol_params.resolution_method;
   char *Preconditionning = MyParams->resol_params.preconditionning;

   int  bandwr_method     = MyParams->resol_params.bandwidth_method;
   int  bandwr_option     = MyParams->resol_params.bandwidth_option;


   switch( problem )
   {
      case PRBLMe_3D_LAPLACIAN:
         A = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
         STIFF1 = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         M_BC = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
         break;

      case PRBLMe_3D_BILAPLACIAN:
         A = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         STIFF1 = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         M_BC = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
         break;

      case PRBLMe_3D_ADVECTION:
         A = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);

         STIFF1 = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         CONV_X = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         CONV_Y = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         CONV_Z = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         M_BC = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
         break;

      case PRBLMe_3D_HELMHOLZ:
         A = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         C = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
         M_BC = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM, 10);
         break;

      default:
         error(E_UNKNOWN, "_solve3D_lin");
         return NULL;
   }


   RHS_FUN = v_get(MyGeom->NBSOMM);
   RHS_BC  = v_get(MyGeom->NBSOMM);
   RHS     = v_get(MyGeom->NBSOMM);

   SOL     = v_get(MyGeom->NBSOMM);


   /* ----- assemblage matrix and rhs ------ */

   M_BC = assemblage3D_matrix_fromBC(MyElt, MyGeom, MyBC, M_BC);

   switch( problem )
   {
      case PRBLMe_3D_LAPLACIAN:
         STIFF1 = assemblage3D_matrix_Stiff1(MyElt, MyGeom, STIFF1);
         A = sp_add(STIFF1, M_BC, A);
         break;

      case PRBLMe_3D_BILAPLACIAN:
         error(E_UNKNOWN, "_solve3D_lin");
         /*STIFF1 = assemblage3D_matrix_Stiff2(MyElt, MyGeom, STIFF1);*/
         break;

      case PRBLMe_3D_ADVECTION:
         STIFF1 = assemblage3D_matrix_Stiff1 ( MyElt, MyGeom , STIFF1 );
         CONV_X = assemblage3D_matrix_Conv_x ( MyElt, MyGeom , CONV_X );
         CONV_Y = assemblage3D_matrix_Conv_y ( MyElt, MyGeom , CONV_Y );
         CONV_Z = assemblage3D_matrix_Conv_z ( MyElt, MyGeom , CONV_Z );

         A = sp_smlt(STIFF1, MyParams->phys_params.kappa , A);
         A = sp_add(A, CONV_X, A);
         A = sp_add(A, CONV_Y, A);
         A = sp_add(A, CONV_Z, A);
         A = sp_add(A, M_BC, A);

         SP_FREE(STIFF1);
         SP_FREE(CONV_X);
         SP_FREE(CONV_Y);
         SP_FREE(CONV_Z);

         break;

      case PRBLMe_3D_HELMHOLZ:
         A = assemblage3D_matrix_Stiff1( MyElt, MyGeom , A );
         C = assemblage3D_matrix_Mass  ( MyElt, MyGeom , C );

         A = sp_smlt(A, MyParams->phys_params.kappa, A);
         C = sp_smlt(C, MyParams->phys_params.sigma, C);

         A = sp_add (A, C, A);

         SP_FREE(C);

         break;

      default:
         error(E_UNKNOWN, "_solve3D_lin");
         return NULL;
   }


   RHS_FUN = assemblage3D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN);
   RHS_BC   = assemblage3D_vector_fromBC(MyElt, MyGeom, MyBC, RHS_BC);

   RHS = v_add(RHS_FUN, RHS_BC, RHS);

   /* dump */
   dump_matrix(A  , "# MATRIX :");
   dump_vector(RHS, "# RHS    :");
   show_matrix(A  , "A after assemblage" );

   transform3D_matrix_vector_with_bc(MyElt, MyGeom, MyBC, A, RHS);

   /* dump */
   dump_matrix(A  , "# MATRIX + CL :");
   dump_vector(RHS, "# RHS    + CL :");
   show_matrix(A  , "A with CL" );


   /* ------ solve the system Ax = b   ----- */

   if  ( strcmp(ResolutionMethod, "DIRECT-METHOD") == 0 )
   {
      switch( problem )
      {
         case PRBLMe_3D_LAPLACIAN:
         case PRBLMe_3D_BILAPLACIAN:
         case PRBLMe_3D_HELMHOLZ:

            if ( bandwr_method == BANDWRe_NULL)
            {
               SPMAT *LLT_A = sp_copy(A);

               spCHfactor(LLT_A);
               spCHsolve(LLT_A, RHS, SOL);

               SP_FREE(LLT_A);
            }
            else
            if ( bandwr_method == BANDWRe_MESCHACH )
            {
               spCHresolution_meschach(A, RHS, SOL);
            }
            else
            if ( bandwr_method == BANDWRe_AMD )
            {
               spCHresolution_amd(A, RHS, SOL);
            }
            else
            if ( bandwr_method == BANDWRe_SPOOLES )
            {
               spCHresolution_spooles(A, RHS, SOL);
            }
            else
            {
               error(E_UNKNOWN, "_solve3D_lin");
            }

            break;


         case PRBLMe_3D_ADVECTION:

            if ( bandwr_method == BANDWRe_NULL)
            {
               SPMAT *LU_A = sp_copy(A);
               PERM *PIV = px_get(LU_A->m);

               spLUfactor(LU_A, PIV, 0.1);
               spLUsolve(LU_A, PIV, RHS, SOL);

               PX_FREE(PIV);
               SP_FREE(LU_A);
            }
            else
            if ( bandwr_method == BANDWRe_MESCHACH )
            {
               spLUresolution_meschach(A, RHS, SOL);
            }
            else
            if ( bandwr_method == BANDWRe_AMD )
            {
               spLUresolution_amd(A, RHS, SOL);
            }
            else
            if ( bandwr_method == BANDWRe_SPOOLES )
            {
               spLUresolution_spooles(A, RHS, SOL);
            }
            else
            {
               error(E_UNKNOWN, "_solve3D_lin");
            }

            break;


         default: error(E_UNKNOWN, "_solve3D_lin");
      }

      /* info residu */
      {
         VEC *RESIDU;
         RESIDU = v_get(SOL->dim);
         RESIDU = sp_mv_mlt(A, SOL, RESIDU);
         RESIDU = v_sub(RHS, RESIDU, RESIDU);

         printf("norm res = %le\n", v_norm2(RESIDU) );

         V_FREE(RESIDU);
      }

   }

   else  /* Iterativ Method */
   {
      if  ( strcmp(Preconditionning, "ICH") == 0 )
      {
         ICH = sp_copy(A);
         spICHfactor(ICH);
      }
      else
      if ( strcmp(Preconditionning, "ILU") == 0 )
      {
         if ( problem == PRBLMe_3D_LAPLACIAN )
			{
				printf("for problem = \"Laplacien\", the matrix A should be symmetric \n");
            printf(" -> use preferrably the LLT preconditionning + CG \n");
			}

         ILU = sp_copy(A);
         spILUfactor(ILU, 0.0);
      }
      else
      if ( strcmp(Preconditionning, "NULL") == 0 )
      {
         printf("no precond for iterativ method \n");
      }
      else
      {
         printf("Preconditionning unknown !");
         error(E_UNKNOWN, "solve3D_laplacien");
      }

      ITER_OUTPUT_INFO *info = iter_output_info(MyParams->resol_params.iter_info, MyParams->resol_params.iter_file);

      if ( strcmp(ResolutionMethod, "CG") == 0 )
      {
         iter_xspcg(A, ICH, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
         printf(" cg: # of iter. = %d \n\n", nb_steps);
      }
		else
		if ( strcmp(ResolutionMethod, "CGS") == 0 )
		{
			iter_xspcgs(A, ILU, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
			printf(" cgs: # of iter. = %d \n\n", nb_steps);
		}
      else
      if ( strcmp(ResolutionMethod, "BiCGStab") == 0 )
      {
         iter_xspbicgstab(A, ILU, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
         printf(" bicgstab: # of iter. = %d \n\n", nb_steps);
      }
      else
      {
         printf("Methode inconnue !!\n");
         error(E_UNKNOWN, "solve3D_laplacien");
      }

      MyParams->resol_params.nb_steps = nb_steps;
   }

   /* -----------------------------------------------FIN ETAPE 5 -------
   DEBUT ETAPE 6 ---------------------------------------------------- */

   V_FREE(RHS);

   SP_FREE(A);

   if ( strcmp(Preconditionning, "ILU") == 0 ) SP_FREE(ILU);
   if ( strcmp(Preconditionning, "ICH") == 0 ) SP_FREE(ICH);

   return SOL;
}

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

static VEC *_solve3D_nli(PRBLMt_3D_TYPE problem, const ELT_3D *MyElt, const GEOM_3D *MyGeom, const BC_3D *MyBC, const RHS_3D *MyRhsFun, const ADV_3D *MyAdvFun)
{
   return NULL;
}

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

static void dump_matrix(const SPMAT *A, const char *header)
{
   PARAMS* MyParams = Params_get_staticparam(0);

   if (MyParams->logger != NULL)
   {
      fprintf   (MyParams->logger, "%s\n\n", header);
      sp_foutput(MyParams->logger, A);
   }
}

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

static void dump_vector(const VEC *b, const char *header)
{
   PARAMS* MyParams = Params_get_staticparam(0);

   if (MyParams->logger != NULL)
   {
      fprintf  (MyParams->logger, "%s\n\n", header);
      v_foutput(MyParams->logger, b);
   }
}

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */

static void show_matrix(const SPMAT *A, const char *legend)
{
   PARAMS* MyParams = Params_get_staticparam(0);

   if ( MyParams->misc_graphics.ispmatpattern )
   {
      graphics_matrix_pattern("X11", A, legend);
   }
}

/* ------------------------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------------------------ */



