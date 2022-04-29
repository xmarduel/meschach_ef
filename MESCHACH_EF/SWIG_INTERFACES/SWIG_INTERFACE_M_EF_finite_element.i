
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////
 
#ifndef FINITE_ELEMENTS_H
#define FINITE_ELEMENTS_H


#define MY_LIST2   2

#define TYPE_ELT_1D    0
#define TYPE_ELT_2D    1
#define TYPE_ELT_3D    2

#include "finite_elements_1D.h"
#include "finite_elements_2D.h"
#include "finite_elements_3D.h"


#define  NB_ERRS2         2
#define  NB_WARNS2        1

//static  char *my_err_mesg2[NB_ERRS2] =
//{
//   "unknown fe error",                /* 0 */
//   "finite element not implemented"   /* 1 */
//};

//static char *my_warn_mesg2[NB_WARNS2] = 
//{
//   "unknown warning"		        /* 0 */
//};

#define	 E_EF_UNKNOWN	  0
#define	 E_EF_NOTIMPL	  1

#define  W_EF_UNKNOWN     0


int mem_attach_list2(void);
int err_attach_list2(void);
int warn_attach_list2(void);
int err_warn_attach_lists2(void);

#endif



#ifndef FINITE_ELEMENTS_1D_H
#define FINITE_ELEMENTS_1D_H



#include "matrix.h"
#include "matrix_adds.h"

typedef struct {
    %immutable;
    int      dim;             /**< dimension; here dim = 1               */
    char*    name_ef     ;     /**< nom de l'element : P1 , P2 , H3 ...   */
    int      nb_somm_cell ;     /**< nb de fonctions de base de l'element  */
    %mutable;
//  Real     (**f_base)();    /**< fonctions de base de l'element        */
//  Real     (**dfdx_base)();   /**< derivees des fonctions de bases     */
//  Real     (**ddfdxx_base)(); /**< derivees secondes                   */
//  MAT*     MAT_Masse;      /**< Mass Matrix                   */
//  MAT*     MAT_Rigid1;     /**< Rigidity matrix first order   */
//  MAT*     MAT_Rigid2;     /**< Rigidity matrix second order  */
//  MAT*     MAT_Convection; /**< Convection matrix Ni.dNj      */
//  VEC*     VEC_Scmbr;      /**< Second Membre (Source cste)   */
//  TENSOR*   TENSOR_Conv3D_1; /**< Ni.dNj.Nk                     */
//  TENSOR*   TENSOR_Conv3D_2; /**< dNi.Nj.Nk                     */
//  TENSOR*   TENSOR_Conv3D_3; /**< Ni.Nj.dNk                     */ 
//  TENSOR*   TENSOR_Mass3D  ; /**< Ni.Nj.Nk                      */
//
//  MAT*     MAT_PermBasis;     /* basis permutation to fit BC   */
//
//  /* only for "S2", "S3" or "S5" non-uniform */
//  SPLINES_1D *splines_def; /* definition of all functions on the mesh : will depend on the geometry */

      } ELT_1D;   /* --- to INITIALIZE in "elt1D_get"  ---- */
 

#define ELT1DNULL  ((ELT_1D *)NULL)
 
int elt1D_free(ELT_1D *) ; 

#define ELT_1D_FREE(elt)  ( elt1D_free(elt ), (elt )=(ELT_1D  *)NULL )

ELT_1D  *elt1D_get(const char *type);

/*-------------------------------------------------------------------*/
/* some methods                                                      */
/*-------------------------------------------------------------------*/
void elt1D_foutput(FILE* fp, ELT_1D* elt);
void elt1D_fdump(FILE* fp, ELT_1D* elt);

#define elt1D_output(elt) elt1D_foutput(stdout, elt);
#define elt1D_dump(elt)   elt1D_fdump(stdout, elt);

/*------------------------------------------------------------------- */
/* all basis functions   : DON'T INSERT THEM !                        */
/*------------------------------------------------------------------- */




#endif
#ifndef FINITE_ELEMENTS_2D_H
#define FINITE_ELEMENTS_2D_H


#include "matrix.h"


typedef struct {
    %immutable;
    int     dim;             /* dimension; here dim = 2                  */
    char*   name_ef     ;     /* nom de l'element : P1 , P2 , Q1 ...      */
    int     nb_somm_cell ;     /* nb de fonctions de base de l'element     */
    %mutable;
//  Real    (**f_base)();    /* fonctions de base de l'element           */
//  Real    (**dfdx_base)(); /* derivees des fonctions de base           */
//  Real    (**dfdy_base)(); /* derivees des fonctions de base           */
//  int     nb_somm_face ;   /* nb de fonctions de base de l'arete       */
//  Real    (**f_aret)();    /* fonctions de base sur les aretes         */
//  MAT*    MAT_Masse;
//  MAT*    MAT_Rigid_xx;    /* The three rigidity matrices  */
//  MAT*    MAT_Rigid_xy;    /*                              */
//  MAT*    MAT_Rigid_yy;    /*                              */
//  MAT*    MAT_Conv_x;      /* The two Convection matrices  */
//  MAT*    MAT_Conv_y;      /*                              */
//  MAT*    MAT_Conv_x_P1dP2;
//  MAT*    MAT_Conv_y_P1dP2;
//  MAT*    MAT_LssLss[3];   /* Matrices to compute the stabilisation term */
//  MAT*    MAT_Ls_Ls_[6];   /* Matrices to compute the stabilisation term */
//  MAT*    MAT_Ls_Lss[6];   /* Matrices to compute the stabilisation term */
//  MAT*    MAT_LssLs_[6];   /* Matrices to compute the stabilisation term */
//
//  ELT_1D* eltDM1;          /* Same element on the dimenson -1            */          
//
      } ELT_2D;            /* ---- a INITIALISER dans "elt2D_get" ------ */
 

#define ELT2DNULL  ((ELT_2D *)NULL)
 
int elt2D_free(ELT_2D *) ; 

#define ELT_2D_FREE(elt)  ( elt2D_free(elt ), (elt )=(ELT_2D  *)NULL )

ELT_2D  *elt2D_get(const char* type);



#endif



#ifndef FINITE_ELEMENTS_3D_H
#define FINITE_ELEMENTS_3D_H


#include "matrix.h"

 
typedef struct {
    %immutable;
    int     dim;             /* dimension; here dim = 3                  */
    char*   name_ef     ;     /* nom de l'element : P1 , P2 , Q1 ...      */
    int     nb_somm_cell ;     /* nb de fonctions de base de l'element     */
    %mutable;
//  Real    (**f_base)();    /* fonctions de base de l'element           */
//  Real    (**dfdx_base)(); /* derivees des fonctions de base           */
//  Real    (**dfdy_base)(); /* derivees des fonctions de base           */
//  Real    (**dfdz_base)(); /* derivees des fonctions de base           */
//  int     nb_somm_face ;   /* nb de fonctions de base de l'arete       */
//  Real    (**f_aret)();    /* fonctions de base sur les aretes         */
//  MAT*    MAT_Masse;
//  MAT*    MAT_Rigid_xx;    /* The six rigidity matrices     */
//  MAT*    MAT_Rigid_yy;    /*                               */
//  MAT*    MAT_Rigid_zz;    /*                               */
//  MAT*    MAT_Rigid_xy;    /*                               */
//  MAT*    MAT_Rigid_xz;    /*                               */
//  MAT*    MAT_Rigid_yz;    /*                               */
//  MAT*    MAT_Conv_x;      /* The three Convection matrices */
//  MAT*    MAT_Conv_y;      /*                               */
//  MAT*    MAT_Conv_z;      /*                               */
//  MAT*    MAT_Conv_x_P1dP2;
//  MAT*    MAT_Conv_y_P1dP2;
//  MAT*    MAT_Conv_z_P1dP2;
//
//  ELT_2D* eltDM1;          /* Same element on the dimenson -1            */
//
      } ELT_3D;            /* ---- a INITIALISER dans "elt3D_get" ------ */
 

#define ELT3DNULL  ((ELT_3D *)NULL)
 
int elt3D_free(ELT_3D *) ; 

#define ELT_3D_FREE(elt)  ( elt3D_free(elt ), (elt )=(ELT_3D  *)NULL )

ELT_3D  *elt3D_get(const char* type);



#endif




//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{

#undef	ELT_1D_FREE
#undef  ELT_2D_FREE
#undef	ELT_3D_FREE


void ELT_1D_FREE(ELT_1D* elt)
{ 
   elt1D_free(elt);
   (elt)=(ELT_1D *)NULL;
}
void ELT_2D_FREE(ELT_2D* elt)
{ 
   elt2D_free(elt);
   (elt)=(ELT_2D *)NULL;
}
void ELT_3D_FREE(ELT_3D* elt)
{ 
   elt3D_free(elt);
   (elt)=(ELT_3D *)NULL;
}


///////////////////////////////////////////////////// 
#undef elt1D_output
void elt1D_output(ELT_1D* elt)
{
	if ( elt == NULL ) error(E_NULL, "elt1D_output");
		
	elt1D_foutput(stdout, elt);
}

#undef elt1D_dump
void elt1D_dump(ELT_1D* elt)
{
	if ( elt == NULL ) error(E_NULL, "elt1D_dump");
		
	elt1D_fdump(stdout, elt);
}
/////////////////////////////////////////////////////

%}
