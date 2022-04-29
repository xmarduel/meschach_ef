
#include "spooles/Eigen/Bridge.h"

#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper.h"
#include "MESCHACH_SPOOLES/INCLUDES/spooles_wrapper_eigen.h"

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

static DenseMtx* spEIGENspooles_bridge(InpMtx *AA, InpMtx *BB, DV* e_values, EIGENt_SPOOLES_PROBLEM type, EIGENt_SPOOLES_OPTION flag);

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

static DenseMtx* spEIGENspooles_bridge(InpMtx *A, InpMtx *B, DV* e_values, EIGENt_SPOOLES_PROBLEM type, EIGENt_SPOOLES_OPTION flag)
{
#ifdef HAVE_BCSLIB

   DenseMtx *e_vectors; /* the return value -- allocate memory for it ! */

   Bridge    bridge ;

   char      pbtype[4], which[4] ;
   double    lftend, rhtend, center, shfscl ;
   double    c__1 = 1.0, c__4 = 4.0, tolact = 2.309970868130169e-11 ;
   double    eigval[1000], sigma[2];
   double    *evec;
   int       error, fstevl, lfinit, lstevl, mxbksz, msglvl, ncol, ndiscd,
             neig, neigvl, nfound, nnonzeros, nrhs, nrow, prbtyp, rc, 
             retc, rfinit, seed, warnng, i,j ;
   int       c__5 = 5, output = 6 ;
   int       *lanczos_wksp;

   FILE      *msgFile;

   /*--------------------------------------------------------------------*/

   DenseMtx_dimensions(e_vectors, &nrow, &ncol);
   neigvl = ncol; /* neigvl = # of desired eigenvalues */
   
   switch(flag)  /* which eigenvalues to compute */
   {
      case EIGENe_SPOOLES_LOWER    : which[1] = 'L';  break;     /* 'l' or 'L' lowest (smallest magnitude) */
      case EIGENe_SPOOLES_UPPER    : which[1] = 'H';  break;     /* 'h' or 'H' highest (largest magnitude) */
      case EIGENe_SPOOLES_NEAREST_1: which[1] = 'N';  break;     /* 'n' or 'N' nearest to central value */
      case EIGENe_SPOOLES_NEAREST_2: which[1] = 'C';  break;     /* 'c' or 'C' nearest to central value */
      case EIGENe_SPOOLES_ALL      : which[1] = 'A';  break;     /* 'a' or 'A' all eigenvalues in interval */

      default: warning(WARN_UNKNOWN,"spEIGENspooles_bridge"); which[1] = 'L';
   }

   switch(type) /* type of problem */
   {
      case EIGENe_SPOOLES_ORDINARY_SYMMETRIC     : pbtype[1] = 'O';  break;   /* ordinary symmetric eigenproblem */ /* next: K positive semidefinite */
      case EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_1: pbtype[1] = 'V';  break;   /* generalized symmetric problem (K,M) with M positive semidef. (vibration problem) */
      case EIGENe_SPOOLES_GENERALIZED_SYMMETRIC_2: pbtype[1] = 'B';  break;   /* generalized symmetric problem (K,K_s) K_s posibly indefinite (buckling problem) */

      default: warning(WARN_UNKNOWN,"spEIGENspooles_bridge"); pbtype[1] = 'O';
   }

   lfinit = 0;               /* if true, lftend is restriction on lower bound of eigenvalues. if false, no restriction on lower bound */
   lftend = 0.0;             /* left endpoint of interval */
   
   rfinit = 0;               /* if true, rhtend is restriction on upper bound of eigenvalues. if false, no restriction on upper bound */
   rhtend = 1.0e20;          /* right endpoint of interval */

   center = 0.0;             /* center of interval */

   mxbksz = 5;                /* upper bound on block size for Lanczos recurrence */
   shfscl = 0.87332995124806; /* shift scaling parameter, an estimate on the magnitude of the smallest nonzero eigenvalues */



   /*
   ----------------------------------------
   check and set the problem type parameter
   ----------------------------------------
   */
   switch ( pbtype[1] )
   {
      case 'v' : case 'V' : prbtyp = 1 ; break ;
      case 'b' : case 'B' : prbtyp = 2 ; break ;
      case 'o' : case 'O' : prbtyp = 3 ; break ;
   }

   /*
   ----------------------------
   Initialize Lanczos workspace
   ----------------------------
   */
   lanczos_init_ ( &lanczos_wksp ) ;

   /*
   ----------------------------------
   initialize communication structure
   ----------------------------------
   */
   lanczos_set_parm( &lanczos_wksp, "order-of-problem",   &nrow,   &retc );
   lanczos_set_parm( &lanczos_wksp, "accuracy-tolerance", &tolact, &retc );
   lanczos_set_parm( &lanczos_wksp, "max-block-size",     &mxbksz, &retc );
   lanczos_set_parm( &lanczos_wksp, "shift-scale",        &shfscl, &retc );
   lanczos_set_parm( &lanczos_wksp, "message_level",      &msglvl, &retc );
   
   lanczos_set_parm( &lanczos_wksp, "matrix-type", &c__1, &retc );
   lanczos_set_parm( &lanczos_wksp, "matrix-type", &c__4, &retc );
   
   /*
   -----------------------------
   set up the solver environment
   -----------------------------
   */
   rc = Setup((void *) &bridge, &prbtyp, &nrow, &mxbksz, A, B, &seed, &msglvl, msgFile) ;

   if ( rc != 1 ) error(E_UNKNOWN,"spEIGENspooles_bridge"); /* fatal error %d from Setup()", rc) ; */

   /*--------------------------------------------------------------------*/
   /*
   -----------------------------------------------
   invoke eigensolver
   nfound -- # of eigenvalues found and kept
   ndisc  -- # of additional eigenvalues discarded
   -----------------------------------------------
   */

   lanczos_run(&neigvl, &which[1] , &pbtype[1], &lfinit, &lftend, 
               &rfinit, &rhtend, &center, &lanczos_wksp, &bridge, &nfound,
	       &ndiscd, &warnng, &error, Factor, MatMul, Solve ) ;

   /*
   -------------------------
   get eigenvalues and print
   -------------------------
   */

   neig   = nfound + ndiscd ;
   lstevl = nfound ;

   lanczos_eigenvalues (&lanczos_wksp, eigval, &neig, &retc);
 
   fstevl = 1 ;
   if ( nfound == 0 ) fstevl = -1 ;
   if ( ndiscd >  0 ) lstevl = -ndiscd ;

   /*
   -------------------------
   get eigenvectors and print
   -------------------------
   */

   neig = min ( 50, nrow );
   Lncz_ALLOCATE(evec, double, nrow, retc);

   for ( i = 1 ; i <= nfound ; i++ )
   {
      lanczos_eigenvector ( &lanczos_wksp, &i, &i, newToOld, evec, &nrow, &retc ) ;
   }

   /*
   ------------------------
   free the working storage
   ------------------------
   */

   lanczos_free( &lanczos_wksp ) ;

   rc = Cleanup(&bridge) ;

   if ( rc != 1 ) error(E_UNKNOWN,"spEIGENspooles_bridge"); /* error return %d from Cleanup()", rc) ; */


   /*
   ------------------------
   setup the spooles structure
   ------------------------
   */
   for (i=0; i<neigvl; i++)
   {
      e_values->vec[i] = eigval[i];
   }
                        
   for (i=0; i<nrow; i++)
   for (j=0; j<ncol; j++)
   {
      DenseMtx_setRealEntry(e_vectors,i,j, evec[i*ncol + j*nrow]);
   }
   
   /*
   ------------------------
   finito
   ------------------------
   */
   return e_vectors;

#else
   error(E_UNKNOWN,"sp_eigen_spooles_bridge");
   return NULL;
#endif
}

/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */

MAT* sp_eigen_spooles(const SPMAT *A, const SPMAT *B, VEC *eigen_values, EIGENt_SPOOLES_PROBLEM type, EIGENt_SPOOLES_OPTION flag)
{
   MAT *eigen_vectors = NULL; /* the return value */

   DV       *e_values;
   DenseMtx *e_vectors;

   InpMtx *AA;
   InpMtx *BB;
   
   /* check */
   if ( eigen_values  == NULL ) error(E_NULL,"sp_eigen_spooles");
   if ( A             == NULL ) error(E_NULL,"sp_eigen_spooles");
   if ( B             == NULL ) error(E_NULL,"sp_eigen_spooles");
   
   if ( A->m          != eigen_values->dim ) error(E_SIZES,"sp_eigen_spooles");
   
   if ( A->m != B->m ) error(E_SIZES,"sp_eigen_spooles");
   if ( A->n != B->n ) error(E_SIZES,"sp_eigen_spooles");

   
   /* convert  meschach to spooles */
   e_values  = v_meschach_to_v_spooles(eigen_values,  NULL);

   AA = s_meschach_to_s_spooles(A, NULL);
   BB = s_meschach_to_s_spooles(B, NULL);
   
   /* main routine */
   e_vectors = spEIGENspooles_bridge(AA, BB, e_values, type, flag);
   /* main routine */

   /* convert back to meschach */
   eigen_values  = v_spooles_to_v_meschach(e_values, eigen_values);
   eigen_vectors = m_spooles_to_m_meschach(e_vectors, eigen_vectors);

   /* clean */
   InpMtx_free(AA);
   InpMtx_free(BB);

   DenseMtx_free(e_vectors);
   DV_free(e_values);
   
   /* finito*/
   return eigen_vectors;
}


/* ------------------------------------------------------------------------------------- */
/* ------------------------------------------------------------------------------------- */
