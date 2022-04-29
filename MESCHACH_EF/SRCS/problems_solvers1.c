
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/iter.h"

#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_1D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_1D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_1D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_1D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_mtxpattern.h"
/* "top-level functions" */
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"

/** \file problems_solvers1.c
 *
 * Contains the drivers functions for linear problems in 1D
 *
 */

typedef enum
{
   PRBLMe_LAPLACIAN   = 0 ,
   PRBLMe_BILAPLACIAN = 1 ,
   PRBLMe_ADVECTION   = 2 ,
   PRBLMe_HELMHOLZ    = 3 ,
   PRBLMe_BURGERS     = 4
   
} PRBLMt_TYPE;

static VEC *_solve1D_lin(PRBLMt_TYPE problem, const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun, const ADV_1D *MyAdvFun);
static VEC *_solve1D_nli(PRBLMt_TYPE problem, const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun, const ADV_1D *MyAdvFun);

static void show_matrix(const SPMAT *A, const char *legend);

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve1D_laplacian( const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve1D_laplacian");
   if ( MyGeom   == NULL ) error(E_NULL, "solve1D_laplacian");
   if ( MyBC     == NULL ) error(E_NULL, "solve1D_laplacian");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve1D_laplacian");

   return _solve1D_lin(PRBLMe_LAPLACIAN, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve1D_bilaplacian(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve1D_bilaplacien");
   if ( MyGeom   == NULL ) error(E_NULL, "solve1D_bilaplacien");
   if ( MyBC     == NULL ) error(E_NULL, "solve1D_bilaplacien");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve1D_bilaplacien");

   return _solve1D_lin(PRBLMe_BILAPLACIAN, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve1D_convection_diffusion(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun, const ADV_1D *MyAdvFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve1D_convection_diffusion");
   if ( MyGeom   == NULL ) error(E_NULL, "solve1D_convection_diffusion");
   if ( MyBC     == NULL ) error(E_NULL, "solve1D_convection_diffusion");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve1D_convection_diffusion");
   if ( MyAdvFun == NULL ) error(E_NULL, "solve1D_convection_diffusion");

   return _solve1D_lin(PRBLMe_ADVECTION, MyElt, MyGeom, MyBC, MyRhsFun, MyAdvFun);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve1D_helmholz(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve1D_helmholz");
   if ( MyGeom   == NULL ) error(E_NULL, "solve1D_helmholz");
   if ( MyBC     == NULL ) error(E_NULL, "solve1D_helmholz");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve1D_helmholz");

   return _solve1D_lin(PRBLMe_HELMHOLZ, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* solve1D_burgers(const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun)
{
   /* check */
   if ( MyElt    == NULL ) error(E_NULL, "solve1D_burgers");
   if ( MyGeom   == NULL ) error(E_NULL, "solve1D_burgers");
   if ( MyBC     == NULL ) error(E_NULL, "solve1D_burgers");
   if ( MyRhsFun == NULL ) error(E_NULL, "solve1D_burgers");

   return _solve1D_nli(PRBLMe_BURGERS, MyElt, MyGeom, MyBC, MyRhsFun, NULL);
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

static VEC* _solve1D_lin(PRBLMt_TYPE problem, const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun, const ADV_1D* MyAdvFun)
{
   SPMAT *A    = (SPMAT *)NULL;
   SPMAT *S    = (SPMAT *)NULL;
   SPMAT *M    = (SPMAT *)NULL;
   SPMAT *C    = (SPMAT *)NULL;
   SPMAT *M_BC = (SPMAT *)NULL;
   
   SPMAT *ILU  = (SPMAT *)NULL;
   SPMAT *ICH  = (SPMAT *)NULL;

   PERM  *PIV;

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


   M_BC = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
   
   A = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
   S = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
   M = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);
   C = sp_get(MyGeom->NBSOMM,MyGeom->NBSOMM,10);

   RHS_FUN = v_get(MyGeom->NBSOMM);
   RHS_BC  = v_get(MyGeom->NBSOMM);
   RHS     = v_get(MyGeom->NBSOMM);
   
   SOL = v_get(MyGeom->NBSOMM);
	
   /* ----- assemblage matrix and rhs ------ */

   M_BC = assemblage1D_matrix_fromBC(MyElt, MyGeom, MyBC, M_BC);
   
   switch( problem )
   {
      case PRBLMe_LAPLACIAN:
         S = assemblage1D_matrix_Stiff1(MyElt, MyGeom, S);
         A = sp_add (S, M_BC, A);
         break;
         
      case PRBLMe_BILAPLACIAN:
         S = assemblage1D_matrix_Stiff2(MyElt, MyGeom, S);
         A = sp_add (S, M_BC, A);
         break;
         
      case PRBLMe_ADVECTION:
         S = assemblage1D_matrix_Stiff1(MyElt, MyGeom, S);
         C = assemblage1D_matrix_Convec(MyElt, MyGeom, C);

         S = sp_smlt(S, MyParams->phys_params.kappa, S);
         A = sp_add (S, C, A);
         A = sp_add (A, M_BC, A);

         break;

      case PRBLMe_HELMHOLZ:
         S = assemblage1D_matrix_Stiff1(MyElt, MyGeom, S);
         M = assemblage1D_matrix_Mass  (MyElt, MyGeom, M);

         S = sp_smlt(S, MyParams->phys_params.kappa, S);
         M = sp_smlt(M, MyParams->phys_params.sigma, M);

         sp_output(S);
         sp_output(M);
         
         A = sp_add (S, M, A);
         
         break;

      default:
         error(E_UNKNOWN, "_solve1D_lin");
         return NULL;
   }
   
   RHS_FUN = assemblage1D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN);
   RHS_BC  = assemblage1D_vector_fromBC(MyElt, MyGeom, MyBC, RHS_BC);
   
   RHS = v_add(RHS_FUN, RHS_BC, RHS);
   
	/*
	v_output(RHS);
    printf(".....\n");
	sp_output(A);
	printf(".....\n");
   */
   
   transform1D_matrix_vector_with_bc(MyElt, MyGeom, MyBC, A, RHS);
   
	/*
	v_output(RHS);
    printf(".....\n");
	sp_output(A);
	printf(".....\n");
	*/
	
   /* show matrix with X11 */
   show_matrix(A, "A with BC");

   
   /* ------ solve the system Ax = b   ----- */

   if  ( strcmp(ResolutionMethod, "DIRECT-METHOD") == 0 )
   {     
      ICH = sp_copy(A);

      switch( problem )
      {
         case PRBLMe_LAPLACIAN:
            
            spCHfactor(ICH);  
            spCHsolve(ICH, RHS, SOL);
            break;

         case PRBLMe_BILAPLACIAN:
            spCHfactor(ICH);
            spCHsolve(ICH, RHS, SOL);
            break;

         case PRBLMe_ADVECTION:
            PIV = px_get(ICH->m);

            spLUfactor(ICH, PIV, 0.1);
            spLUsolve(ICH, PIV, RHS, SOL);

            PX_FREE(PIV);
            break;

         case PRBLMe_HELMHOLZ:
            spCHfactor(ICH);
            spCHsolve(ICH, RHS, SOL);
            break;

         default: error(E_UNKNOWN, "_solve1D_lin");
      }
   }

   else  /* Iterativ Method */
   {
      if  ( strcmp(Preconditionning, "ICH") == 0 )
      {     
         ICH = sp_copy(A);
         spICHfactor(ICH);    
      }

      else if ( strcmp(Preconditionning, "ILU") == 0 )
      {
         if ( problem == PRBLMe_LAPLACIAN )
			{
				printf("for problem = \"Laplacien\", the matrix A should be symmetric \n");
				printf(" -> use preferrably the ICH preconditionning + CG \n");
			}
			
         ILU = sp_copy(A);
         spILUfactor(ILU, 0.0);
      }

      else if ( strcmp(Preconditionning, "NULL") == 0 )
      {
         printf("no precond for iterativ method \n");
      }  

      else
      {
         printf("Preconditionning unknown !");
         error(E_UNKNOWN, "solve1D_laplacien");
      }


      ITER_OUTPUT_INFO *info = iter_output_info(MyParams->resol_params.iter_info, MyParams->resol_params.iter_file);

      if ( strcmp(ResolutionMethod, "CG") == 0 )
      {
         iter_xspcg(A, ICH, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
         printf(" cg: # of iter. = %d \n\n",nb_steps);
      }

      else 
		if ( strcmp(ResolutionMethod, "CGS") == 0 )
		{  
			iter_xspcgs(A, ILU, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
			printf(" cgs: # of iter. = %d \n\n",nb_steps);
		}
			
		else 
		if ( strcmp(ResolutionMethod, "BiCGStab") == 0 )
      {  
         iter_xspbicgstab(A, ILU, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
         printf(" bicgstab: # of iter. = %d \n\n",nb_steps);
      }

      else
      {
         printf("Methode inconnue !!\n");
         error(E_UNKNOWN, "solve1D_laplacien");
      }

      MyParams->resol_params.nb_steps = nb_steps;
   }   

   /* info residu */
   {
      VEC *RESIDU;
      RESIDU = v_get(SOL->dim);
      RESIDU = sp_mv_mlt(A, SOL, RESIDU);
      RESIDU = v_sub(RHS, RESIDU,RESIDU);

      printf("|| A.x - f || = %le\n", v_norm2(RESIDU) );

      /*printf("|| S.x || = %le\n", v_norm2(sp_mv_mlt(S,SOL,RESIDU)) );*/
      
      V_FREE(RESIDU);
   }

   /* ------- free memory ------------------ */

   V_FREE(RHS);

   SP_FREE(A);
   SP_FREE(S);
   SP_FREE(M);
   SP_FREE(C);

   if ( strcmp(Preconditionning, "ILU") == 0 ) SP_FREE(ILU);
   if ( strcmp(Preconditionning, "ICH") == 0 ) SP_FREE(ICH);

   return SOL;
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

VEC* _solve1D_nli(PRBLMt_TYPE problem, const ELT_1D *MyElt, const GEOM_1D *MyGeom, const BC_1D *MyBC, const RHS_1D *MyRhsFun, const ADV_1D *MyAdvFun)
{
   SPMAT *S    = (SPMAT *)NULL;
   SPMAT *A    = (SPMAT *)NULL;
   SPMAT *Abc  = (SPMAT *)NULL;
   SPMAT *M_BC = (SPMAT *)NULL;
   SPMAT *ICH  = (SPMAT *)NULL;

   VEC  *SOL;
   VEC  *RHS;

   VEC  *RHS_FUN;
   VEC  *RHS_BC;
   VEC  *RHS_NLI; /* UUx or UU */

   VEC  *Un = VNULL;

   VEC* (*assemblage_nli)( const ELT_1D *Elt , const GEOM_1D *Geom , const VEC* a , const VEC*b , VEC *RHS );
   
   PARAMS* MyParams = Params_get_staticparam(0);

   Real eps_steps = MyParams->resol_params.eps_steps;
   int  max_steps = MyParams->resol_params.max_steps;
   int  nb_steps  = MyParams->resol_params.nb_steps;

   char *ResolutionMethod = MyParams->resol_params.resolution_method;
   char *Preconditionning = MyParams->resol_params.preconditionning;

   int nb_steps_iter_nli = max_steps; // TODO: burgers outer iterations (json)
   int n;
   Real diff;
   
   switch( problem )
   {
      case PRBLMe_BURGERS: assemblage_nli = assemblage1D_vector_abx; break;

      default: error(E_UNKNOWN, "_solve1D_nli");
   }

   A    = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM,10);
   Abc  = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM,10);
   S    = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM,10);
   M_BC = sp_get(MyGeom->NBSOMM, MyGeom->NBSOMM,10);
   
   RHS = v_get(MyGeom->NBSOMM); v_zero(RHS);
   SOL = v_get(MyGeom->NBSOMM); v_zero(SOL);

   RHS_FUN = v_get(MyGeom->NBSOMM);
   RHS_BC  = v_get(MyGeom->NBSOMM);
   RHS_NLI = v_get(MyGeom->NBSOMM);
   
   /* ----- assemblage matrix and rhs ------ */

   /* calculate once for all the stiff matrix */
   S = assemblage1D_matrix_Stiff1( MyElt, MyGeom, S);
   M_BC = assemblage1D_matrix_fromBC(MyElt, MyGeom, MyBC, M_BC);
   
   /* calculate once for all the rhs from the source function */
   RHS_FUN = assemblage1D_vector_fun(MyElt, MyGeom, MyRhsFun, RHS_FUN);
   RHS_BC = assemblage1D_vector_fromBC(MyElt, MyGeom, MyBC, RHS_BC);

   A = sp_add(S, M_BC, A);
   RHS_FUN = v_add(RHS_FUN, RHS_BC, RHS_FUN);
   
   /* bc */
   Abc = sp_copy2(A, Abc);

   /* bc */
   transform1D_matrix_with_bc(MyElt, MyGeom, MyBC, Abc);

   
   /* ------ solve the system Ax = b   ----- */

   if  ( strcmp(ResolutionMethod, "DIRECT-METHOD") == 0 )
   {  
      ICH = sp_copy(Abc);
   
      spCHfactor(ICH);
   }
   else
   if ( strcmp(Preconditionning, "ICH") == 0 )
   {
      ICH = sp_copy(Abc);

      spICHfactor(ICH);    
   }
   else
   if ( strcmp(Preconditionning, "ILU") == 0 )
   {
      printf( "for problem = \"Burgers or xxx \", the matrix A should be symmetric\n");
      printf( " -> use preferrably the ICH preconditionning + CG \n");

      ICH = sp_copy(Abc);

      spICHfactor(ICH);     
   }
   else
   if ( strcmp(Preconditionning, "DIAG") == 0 )
   {
      printf( "not implemented -> NULL precond \n");
   }
   else
   if ( strcmp(Preconditionning, "NULL") == 0 )
   {
      printf( "no precond for iterativ method \n");
   }
   else
   {
      warning(E_UNKNOWN, "_solve1D_nli");
   }


   for (n=0; n<nb_steps_iter_nli; n++)
   {
      Un = v_copy(SOL, Un);

      /*transform1D_vector_with_oldbasis( MyElt, MyGeom , Un );*/
      RHS_NLI = assemblage_nli(MyElt, MyGeom, Un, Un, RHS_NLI);    /*  UUx or UU */
      /*transform1D_vector_with_newbasis( MyElt, MyGeom , RHS_NLI );*/
      

      RHS = v_sub(RHS_FUN, RHS_NLI, RHS);

      transform1D_vector_with_bc(MyElt, MyGeom, MyBC, A, RHS);


      if ( strcmp(ResolutionMethod, "DIRECT-METHOD") == 0 )
      {
         SOL = spCHsolve(ICH, RHS, SOL);
      }
      else
		{
			ITER_OUTPUT_INFO *info = iter_output_info(MyParams->resol_params.iter_info, MyParams->resol_params.iter_file);
			
			if ( strcmp(ResolutionMethod, "CG") == 0 )
         {
            iter_xspcg(A, ICH, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
            printf( "cg: # of iter. = %d\n",nb_steps );
         }
			else
         if ( strcmp(ResolutionMethod, "BiCGStab") == 0 )
         {
            iter_xspbicgstab(A, ICH, RHS, eps_steps, SOL, max_steps, &nb_steps, info);
            printf( "bicgstab: # of iter. = %d\n",nb_steps);
         }
         else
         {
            error(E_UNKNOWN, "_solve1D_nli");
         }

         /* test convergence */
         diff = v_norm2( v_sub(SOL, Un, RHS) );
         printf ("iter=%d -> diff = %le \n", n, diff );

         if ( diff < eps_steps )
         {
            break;
         }
		}
   }

   V_FREE(RHS_FUN);
   V_FREE(RHS_NLI);
   V_FREE(RHS);
   V_FREE(RHS_BC);
   V_FREE(Un);
   
   
   SP_FREE(A);
   SP_FREE(S);
   SP_FREE(M_BC);
   SP_FREE(Abc);

   SP_FREE(ICH);

   return SOL;
}

/*--------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------*/

static void show_matrix(const SPMAT * A, const char * legend)
{
   PARAMS* MyParams = Params_get_staticparam(0);

   if ( MyParams->misc_graphics.ispmatpattern )
   {
      graphics_matrix_pattern( "X11", A, legend );
   }
}

/* ------------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------------- */


