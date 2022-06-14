
/*
 * Contains the implementation of the finite element structure in 1 dimension
 *
 */


#define  SQR(x)  (x)*(x)
#define  CUB(x)  (x)*(x)*(x)
#define  QUA(x)  (x)*(x)*(x)*(x)
#define  CIN(x)  (x)*(x)*(x)*(x)*(x)

#include "MESCHACH/INCLUDES/matrix.h"          /* Librairie Meschach    */
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"     /* for the 3D arrays     */

#include "MESCHACH_EF/INCLUDES/finite_elements.h"


/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_P1(ELT_1D *elt);
static void elt1D_get_P2(ELT_1D *elt);
static void elt1D_get_P3(ELT_1D *elt);
static void elt1D_get_H3(ELT_1D *elt);
static void elt1D_get_S2(ELT_1D *elt);
static void elt1D_get_S3(ELT_1D *elt);
static void elt1D_get_S4(ELT_1D *elt);
static void elt1D_get_S5(ELT_1D *elt);

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

ELT_1D *elt1D_get(const char *type)
{
   ELT_1D *elt;

   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/

   if ( (elt=NEW(ELT_1D)) == (ELT_1D *)NULL )
   {
      error(E_MEM, "elt_1D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_ELT_1D, 0, sizeof(ELT_1D), MY_LIST2);
      mem_numvar_list(TYPE_ELT_1D, 1, MY_LIST2);
   }

   elt->dim         = 1 ;
   elt->name_ef     = strdup(type) ;

   elt->splines_def = NULL;

   /* ------------------------------------------------------------------------------------------- */
   /* ------------------------------------------------------------------------------------------- */

   if ( strcmp(elt->name_ef,"P1") == 0 )
   {
      elt1D_get_P1(elt);
   }
   else
   if ( strcmp(elt->name_ef,"P2") == 0  )
   {
      elt1D_get_P2(elt);
   }
   else
   if ( strcmp(elt->name_ef,"P3") == 0 )
   {
      elt1D_get_P3(elt);
   }
   else
   if ( strcmp(elt->name_ef,"H3") == 0 )
   {
      elt1D_get_H3(elt);
   }
   else
   if ( strcmp(elt->name_ef,"S2") == 0 )
   {
      elt1D_get_S2(elt);
   }
   else
   if ( strcmp(elt->name_ef,"S3") == 0 )
   {
      elt1D_get_S3(elt);
   }
   else
   if ( strcmp(elt->name_ef,"S4") == 0 )
   {
      elt1D_get_S4(elt);
   }
   else
   if ( strcmp(elt->name_ef,"S5") == 0 )
   {
      elt1D_get_S5(elt);
   }
   else
   {
      error2(E_EF_NOTIMPL, "elt_get");
   }

   return (elt);
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_P1(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element P1 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_P1_mtx.h"
   /* Declaration de toutes les matrices pour l' element P1 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 2 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(2, sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(2, sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(2, sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_P1 ;
   elt->f_base[1] = F1_1D_P1 ;

   elt->dfdx_base[0] = DFDX0_1D_P1 ;
   elt->dfdx_base[1] = DFDX1_1D_P1 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_P1 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_P1 ;


   elt->MAT_I_I = m_get(2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P1[i][j];
   }

   elt->MAT_I_x = m_get(2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_P1[i][j];
   }

   elt->MAT_x_x   = m_get(2,2);
   elt->MAT_xx_xx = m_get(2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_P1[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_P1[i][j];
   }

   elt->MAT_I_xx = m_get(2,2);
   elt->MAT_x_xx = m_get(2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_P1[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_P1[i][j];
   }



   elt->VEC_I  = v_get(2);
   elt->VEC_x  = v_get(2);
   elt->VEC_xx = v_get(2);
   for (i=0; i<2; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__P1[i];
      elt->VEC_x->ve[i]  = VEC_x__P1[i];
      elt->VEC_xx->ve[i] = VEC_xx_P1[i];
   }

   elt->TENSOR_I_x_I = ts_get(2,2,2);
   elt->TENSOR_x_I_I = ts_get(2,2,2);
   elt->TENSOR_I_I_x = ts_get(2,2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   for (k=0; k<2; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_P1[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_P1[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_P1[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(2,2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   for (k=0; k<2; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_P1[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = (MAT*)NULL;
   elt->MAT_FuncBasis_CL_R = (MAT*)NULL;
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_P2(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element P2 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_P2_mtx.h"
   /* Declaration de toutes les matrices pour l' element P2 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 3 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(3, sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(3, sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(3, sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_P2 ;
   elt->f_base[1] = F1_1D_P2 ;
   elt->f_base[2] = F2_1D_P2 ;

   elt->dfdx_base[0] = DFDX0_1D_P2 ;
   elt->dfdx_base[1] = DFDX1_1D_P2 ;
   elt->dfdx_base[2] = DFDX2_1D_P2 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_P2 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_P2 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_P2 ;


   elt->MAT_I_I = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P2[i][j];
   }

   elt->MAT_I_x = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_P2[i][j];
   }

   elt->MAT_x_x   = m_get(3,3);
   elt->MAT_xx_xx = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_P2[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_P2[i][j];
   }

   elt->MAT_I_xx = m_get(3,3);
   elt->MAT_x_xx = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_P2[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_P2[i][j];
   }



   elt->VEC_I  = v_get(3);
   elt->VEC_x  = v_get(3);
   elt->VEC_xx = v_get(3);
   for (i=0; i<3; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__P2[i];
      elt->VEC_x->ve[i]  = VEC_x__P2[i];
      elt->VEC_xx->ve[i] = VEC_xx_P2[i];
   }

   elt->TENSOR_I_x_I = ts_get(3,3,3);
   elt->TENSOR_x_I_I = ts_get(3,3,3);
   elt->TENSOR_I_I_x = ts_get(3,3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   for (k=0; k<3; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_P2[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_P2[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_P2[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(3,3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   for (k=0; k<3; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_P2[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = (MAT*)NULL;
   elt->MAT_FuncBasis_CL_R = (MAT*)NULL;
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_P3(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element P3 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_P3_mtx.h"
   /* Declaration de toutes les matrices pour l' element P3 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 4 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_P3 ;
   elt->f_base[1] = F1_1D_P3 ;
   elt->f_base[2] = F2_1D_P3 ;
   elt->f_base[3] = F3_1D_P3 ;

   elt->dfdx_base[0] = DFDX0_1D_P3 ;
   elt->dfdx_base[1] = DFDX1_1D_P3 ;
   elt->dfdx_base[2] = DFDX2_1D_P3 ;
   elt->dfdx_base[3] = DFDX3_1D_P3 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_P3 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_P3 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_P3 ;
   elt->ddfdxx_base[3] = DDFDXX3_1D_P3 ;

   elt->MAT_I_I = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P3[i][j];
   }

   elt->MAT_I_x = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_P3[i][j];
   }

   elt->MAT_x_x   = m_get(4,4);
   elt->MAT_xx_xx = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_P3[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_P3[i][j];
   }

   elt->MAT_I_xx = m_get(4,4);
   elt->MAT_x_xx = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_P3[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_P3[i][j];
   }



   elt->VEC_I  = v_get(4);
   elt->VEC_x  = v_get(4);
   elt->VEC_xx = v_get(4);
   for (i=0; i<4; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__P3[i];
      elt->VEC_x->ve[i]  = VEC_x__P3[i];
      elt->VEC_xx->ve[i] = VEC_xx_P3[i];
   }

   elt->TENSOR_I_x_I = ts_get(4,4,4);
   elt->TENSOR_x_I_I = ts_get(4,4,4);
   elt->TENSOR_I_I_x = ts_get(4,4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   for (k=0; k<4; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_P3[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_P3[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_P3[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(4,4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   for (k=0; k<4; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_P3[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = (MAT*)NULL;
   elt->MAT_FuncBasis_CL_R = (MAT*)NULL;
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_H3(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element H3 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_H3_mtx.h"
   /* Declaration de toutes les matrices pour l' element H3 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 4 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_H3 ;
   elt->f_base[1] = F1_1D_H3 ;
   elt->f_base[2] = F2_1D_H3 ;
   elt->f_base[3] = F3_1D_H3 ;

   elt->dfdx_base[0] = DFDX0_1D_H3 ;
   elt->dfdx_base[1] = DFDX1_1D_H3 ;
   elt->dfdx_base[2] = DFDX2_1D_H3 ;
   elt->dfdx_base[3] = DFDX3_1D_H3 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_H3 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_H3 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_H3 ;
   elt->ddfdxx_base[3] = DDFDXX3_1D_H3 ;

   elt->MAT_I_I = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_H3[i][j];
   }

   elt->MAT_I_x = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_H3[i][j];
   }

   elt->MAT_x_x   = m_get(4,4);
   elt->MAT_xx_xx = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_H3[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_H3[i][j];
   }

   elt->MAT_I_xx = m_get(4,4);
   elt->MAT_x_xx = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_H3[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_H3[i][j];
   }



   elt->VEC_I  = v_get(4);
   elt->VEC_x  = v_get(4);
   elt->VEC_xx = v_get(4);
   for (i=0; i<4; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__H3[i];
      elt->VEC_x->ve[i]  = VEC_x__H3[i];
      elt->VEC_xx->ve[i] = VEC_xx_H3[i];
   }

   elt->TENSOR_I_x_I = ts_get(4,4,4);
   elt->TENSOR_x_I_I = ts_get(4,4,4);
   elt->TENSOR_I_I_x = ts_get(4,4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   for (k=0; k<4; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_H3[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_H3[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_H3[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(4,4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   for (k=0; k<4; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_H3[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = (MAT*)NULL;
   elt->MAT_FuncBasis_CL_R = (MAT*)NULL;
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_S2(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element S2 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_S2_mtx.h"
   /* Declaration de toutes les matrices pour l' element S2 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 3 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(3,sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(3,sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(3,sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_S2 ;
   elt->f_base[1] = F1_1D_S2 ;
   elt->f_base[2] = F2_1D_S2 ;

   elt->dfdx_base[0] = DFDX0_1D_S2 ;
   elt->dfdx_base[1] = DFDX1_1D_S2 ;
   elt->dfdx_base[2] = DFDX2_1D_S2 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_S2 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_S2 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_S2 ;

   elt->MAT_I_I = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_S2[i][j];
   }

   elt->MAT_I_x = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_S2[i][j];
   }

   elt->MAT_x_x   = m_get(3,3);
   elt->MAT_xx_xx = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_S2[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_S2[i][j];
   }

   elt->MAT_I_xx = m_get(3,3);
   elt->MAT_x_xx = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_S2[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_S2[i][j];
   }


   elt->VEC_I  = v_get(3);
   elt->VEC_x  = v_get(3);
   elt->VEC_xx = v_get(3);
   for (i=0; i<3; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__S2[i];
      elt->VEC_x->ve[i]  = VEC_x__S2[i];
      elt->VEC_xx->ve[i] = VEC_xx_S2[i];
   }

   elt->TENSOR_I_x_I = ts_get(3,3,3);
   elt->TENSOR_x_I_I = ts_get(3,3,3);
   elt->TENSOR_I_I_x = ts_get(3,3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   for (k=0; k<3; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_S2[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_S2[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_S2[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(3,3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   for (k=0; k<3; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_S2[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = m_get(2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   {
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S2[i][j];
   }
   elt->MAT_FuncBasis_CL_R = m_get(2,2);
   for (i=0; i<2; i++)
   for (j=0; j<2; j++)
   {
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S2[1-i][1-j];
   }
         
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_S3(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element S3 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_S3_mtx.h"
   /* Declaration de toutes les matrices pour l' element S3 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 4 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(4,sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_S3 ;
   elt->f_base[1] = F1_1D_S3 ;
   elt->f_base[2] = F2_1D_S3 ;
   elt->f_base[3] = F3_1D_S3 ;

   elt->dfdx_base[0] = DFDX0_1D_S3 ;
   elt->dfdx_base[1] = DFDX1_1D_S3 ;
   elt->dfdx_base[2] = DFDX2_1D_S3 ;
   elt->dfdx_base[3] = DFDX3_1D_S3 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_S3 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_S3 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_S3 ;
   elt->ddfdxx_base[3] = DDFDXX3_1D_S3 ;

   elt->MAT_I_I = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_S3[i][j];
   }

   elt->MAT_I_x = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_S3[i][j];
   }

   elt->MAT_x_x   = m_get(4,4);
   elt->MAT_xx_xx = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_S3[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_S3[i][j];
   }

   elt->MAT_I_xx = m_get(4,4);
   elt->MAT_x_xx = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_S3[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_S3[i][j];
   }


   elt->VEC_I  = v_get(4);
   elt->VEC_x  = v_get(4);
   elt->VEC_xx = v_get(4);
   for (i=0; i<4; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__S3[i];
      elt->VEC_x->ve[i]  = VEC_x__S3[i];
      elt->VEC_xx->ve[i] = VEC_xx_S3[i];
   }

   elt->TENSOR_I_x_I = ts_get(4,4,4);
   elt->TENSOR_x_I_I = ts_get(4,4,4);
   elt->TENSOR_I_I_x = ts_get(4,4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   for (k=0; k<4; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_S3[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_S3[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_S3[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(4,4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   for (k=0; k<4; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_S3[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S3_a[i][j];
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S3_b[i][j];
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S3_c[i][j];
   }
   elt->MAT_FuncBasis_CL_R = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S3_a[2-i][2-j];
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S3_b[2-i][2-j];
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S3_c[2-i][2-j];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_S4(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element S4 ... */
#include "MESCHACH_EF/INCLUDES/ef1D_S4_mtx.h"
   /* Declaration de toutes les matrices pour l' element S4 ... */
   
   u_int i,j,k;
   
   elt->nb_somm_cell = 5 ;
   
   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(5,sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(5,sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(5,sizeof(Real (*)(Real)) );
   
   elt->f_base[0] = F0_1D_S4 ;
   elt->f_base[1] = F1_1D_S4 ;
   elt->f_base[2] = F2_1D_S4 ;
   elt->f_base[3] = F3_1D_S4 ;
   elt->f_base[4] = F4_1D_S4 ;
   
   elt->dfdx_base[0] = DFDX0_1D_S4 ;
   elt->dfdx_base[1] = DFDX1_1D_S4 ;
   elt->dfdx_base[2] = DFDX2_1D_S4 ;
   elt->dfdx_base[3] = DFDX3_1D_S4 ;
   elt->dfdx_base[4] = DFDX4_1D_S4 ;
   
   elt->ddfdxx_base[0] = DDFDXX0_1D_S4 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_S4 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_S4 ;
   elt->ddfdxx_base[3] = DDFDXX3_1D_S4 ;
   elt->ddfdxx_base[4] = DDFDXX4_1D_S4 ;

   elt->MAT_I_I = m_get(5,5);
   for (i=0; i<5; i++)
      for (j=0; j<5; j++)
      {
         elt->MAT_I_I->me[i][j] = MAT_Masse_S4[i][j];
      }
   
   elt->MAT_I_x = m_get(5,5);
   for (i=0; i<5; i++)
      for (j=0; j<5; j++)
      {
         elt->MAT_I_x->me[i][j] = MAT_Convection_S4[i][j];
      }
   
   elt->MAT_x_x   = m_get(5,5);
   elt->MAT_xx_xx = m_get(5,5);
   for (i=0; i<5; i++)
      for (j=0; j<5; j++)
      {
         elt->MAT_x_x->me[i][j]   = MAT_Rigid1_S4[i][j];
         elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_S4[i][j];
      }
   
   elt->MAT_I_xx = m_get(5,5);
   elt->MAT_x_xx = m_get(5,5);
   for (i=0; i<5; i++)
      for (j=0; j<5; j++)
      {
         elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_S4[i][j];
         elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_S4[i][j];
      }
   
   
   elt->VEC_I  = v_get(5);
   elt->VEC_x  = v_get(5);
   elt->VEC_xx = v_get(5);
   for (i=0; i<5; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__S4[i];
      elt->VEC_x->ve[i]  = VEC_x__S4[i];
      elt->VEC_xx->ve[i] = VEC_xx_S4[i];
   }
   
   elt->TENSOR_I_x_I = ts_get(5,5,5);
   elt->TENSOR_x_I_I = ts_get(5,5,5);
   elt->TENSOR_I_I_x = ts_get(5,5,5);
   for (i=0; i<5; i++)
      for (j=0; j<5; j++)
         for (k=0; k<5; k++)
         {
            elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_S4[i][j][k];
            elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_S4[j][i][k];
            elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_S4[i][k][j];
         }
   
   elt->TENSOR_I_I_I = ts_get(5,5,5);
   for (i=0; i<5; i++)
      for (j=0; j<5; j++)
         for (k=0; k<5; k++)
         {
            elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_S4[i][j][k];
         }
   
   elt->MAT_FuncBasis_CL_L = m_get(4,4);
   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
      {
         elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S4_a[i][j];
         elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S4_b[i][j];
         elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S4_c[i][j];
      }
   elt->MAT_FuncBasis_CL_R = m_get(4,4);
   for (i=0; i<4; i++)
      for (j=0; j<4; j++)
      {
         elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S4_a[3-i][3-j];
         elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S4_b[3-i][3-j];
         elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S4_c[3-i][3-j];
      }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt1D_get_S5(ELT_1D *elt)
{
   /* Declaration de toutes les matrices pour l' element S5 ... */
   #include "MESCHACH_EF/INCLUDES/ef1D_S5_mtx.h"
   /* Declaration de toutes les matrices pour l' element S5 ... */

   u_int i,j,k;

   elt->nb_somm_cell = 6 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real))calloc(6,sizeof(Real (*)(Real)) );
   elt->dfdx_base   = (Real (**)(Real))calloc(6,sizeof(Real (*)(Real)) );
   elt->ddfdxx_base = (Real (**)(Real))calloc(6,sizeof(Real (*)(Real)) );

   elt->f_base[0] = F0_1D_S5 ;
   elt->f_base[1] = F1_1D_S5 ;
   elt->f_base[2] = F2_1D_S5 ;
   elt->f_base[3] = F3_1D_S5 ;
   elt->f_base[4] = F4_1D_S5 ;
   elt->f_base[5] = F5_1D_S5 ;

   elt->dfdx_base[0] = DFDX0_1D_S5 ;
   elt->dfdx_base[1] = DFDX1_1D_S5 ;
   elt->dfdx_base[2] = DFDX2_1D_S5 ;
   elt->dfdx_base[3] = DFDX3_1D_S5 ;
   elt->dfdx_base[4] = DFDX4_1D_S5 ;
   elt->dfdx_base[5] = DFDX5_1D_S5 ;

   elt->ddfdxx_base[0] = DDFDXX0_1D_S5 ;
   elt->ddfdxx_base[1] = DDFDXX1_1D_S5 ;
   elt->ddfdxx_base[2] = DDFDXX2_1D_S5 ;
   elt->ddfdxx_base[3] = DDFDXX3_1D_S5 ;
   elt->ddfdxx_base[4] = DDFDXX4_1D_S5 ;
   elt->ddfdxx_base[5] = DDFDXX5_1D_S5 ;

   elt->MAT_I_I = m_get(6,6);
   for (i=0; i<6; i++)
   for (j=0; j<6; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_S5[i][j];
   }

   elt->MAT_I_x = m_get(6,6);
   for (i=0; i<6; i++)
   for (j=0; j<6; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Convection_S5[i][j];
   }

   elt->MAT_x_x   = m_get(6,6);
   elt->MAT_xx_xx = m_get(6,6);
   for (i=0; i<6; i++)
   for (j=0; j<6; j++)
   {
      elt->MAT_x_x->me[i][j]   = MAT_Rigid1_S5[i][j];
      elt->MAT_xx_xx->me[i][j] = MAT_Rigid2_S5[i][j];
   }

   elt->MAT_I_xx = m_get(6,6);
   elt->MAT_x_xx = m_get(6,6);
   for (i=0; i<6; i++)
   for (j=0; j<6; j++)
   {
      elt->MAT_I_xx->me[i][j] = MAT_Stab_I_xx_S5[i][j];
      elt->MAT_x_xx->me[i][j] = MAT_Stab_x_xx_S5[i][j];
   }


   elt->VEC_I  = v_get(6);
   elt->VEC_x  = v_get(6);
   elt->VEC_xx = v_get(6);
   for (i=0; i<6; i++)
   {
      elt->VEC_I->ve[i]  = VEC_I__S5[i];
      elt->VEC_x->ve[i]  = VEC_x__S5[i];
      elt->VEC_xx->ve[i] = VEC_xx_S5[i];
   }

   elt->TENSOR_I_x_I = ts_get(6,6,6);
   elt->TENSOR_x_I_I = ts_get(6,6,6);
   elt->TENSOR_I_I_x = ts_get(6,6,6);
   for (i=0; i<6; i++)
   for (j=0; j<6; j++)
   for (k=0; k<6; k++)
   {
      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_Convection_S5[i][j][k];
      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_Convection_S5[j][i][k];
      elt->TENSOR_I_I_x->te[i][j][k] = TENSOR_Convection_S5[i][k][j];
   }

   elt->TENSOR_I_I_I = ts_get(6,6,6);
   for (i=0; i<6; i++)
   for (j=0; j<6; j++)
   for (k=0; k<6; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Masse_S5[i][j][k];
   }

   elt->MAT_FuncBasis_CL_L = m_get(5,5);
   for (i=0; i<5; i++)
   for (j=0; j<5; j++)
   {
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S5_a[i][j];
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S5_b[i][j];
      elt->MAT_FuncBasis_CL_L->me[i][j] = MAT_PermBasis_S5_c[i][j];
   }
   elt->MAT_FuncBasis_CL_R = m_get(5,5);
   for (i=0; i<5; i++)
   for (j=0; j<5; j++)
   {
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S5_a[4-i][4-j];
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S5_b[4-i][4-j];
      elt->MAT_FuncBasis_CL_R->me[i][j] = MAT_PermBasis_S5_c[4-i][4-j];
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int elt1D_free(ELT_1D *elt)
{
   if ( elt == (ELT_1D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_ELT_1D, sizeof(ELT_1D), 0, MY_LIST2);
         mem_numvar_list(TYPE_ELT_1D, -1, MY_LIST2);
      }

      free(elt->name_ef);

      V_FREE(elt->VEC_I);
      V_FREE(elt->VEC_x);
      V_FREE(elt->VEC_xx);
      
      M_FREE(elt->MAT_I_I);
      
      M_FREE(elt->MAT_I_x);
      M_FREE(elt->MAT_x_x);
      M_FREE(elt->MAT_xx_xx);
      M_FREE(elt->MAT_x_xx);
      M_FREE(elt->MAT_I_xx);
      
      TS_FREE(elt->TENSOR_I_x_I);
      TS_FREE(elt->TENSOR_x_I_I);
      TS_FREE(elt->TENSOR_I_I_x);
      
      TS_FREE(elt->TENSOR_I_I_I);
      

      M_FREE(elt->MAT_FuncBasis_CL_L);
      M_FREE(elt->MAT_FuncBasis_CL_R);

      free(elt->f_base);
      free(elt->dfdx_base);
      free(elt->ddfdxx_base);
      
      /* free the memory of the element */
      free(elt);
            
      return EXIT_SUCCESS;
   }
   
}


/*-------------------------------------------------------------------*/
/* some methods                                                      */
/*-------------------------------------------------------------------*/
void elt1D_foutput(FILE* fp, ELT_1D* elt)
{
   if ( elt == NULL ) error(E_NULL, "elt1D_foutput");
   if ( fp  == NULL ) error(E_NULL, "elt1D_foutput");

   fprintf(fp, "ELT: \n");
   fprintf(fp, "\tdim = %d\n", elt->dim);
   fprintf(fp, "\tname = %s\n", elt->name_ef);
   
   return;
}

void elt1D_fdump(FILE* fp, ELT_1D* elt)
{
   if ( elt == NULL ) error(E_NULL, "elt1D_foutput");
   if ( fp  == NULL ) error(E_NULL, "elt1D_foutput");

   fprintf(fp, "ELT: \n");
   fprintf(fp, "\tdim = %d\n", elt->dim);
   fprintf(fp, "\tname = %s\n", elt->name_ef);
      
   fprintf(fp, "\tMAT_Masse:");
   m_foutput(fp, elt->MAT_I_I);
   
   fprintf(fp, "\tMAT_Rigid1:");
   m_foutput(fp, elt->MAT_x_x);

   fprintf(fp, "\tbase functions:");
   
   return;
}


/*  ------------------------------------------------- */
/*  Definition des fonctions de base - 1D -  -------- */
/*  ------------------------------------------------- */

Real F0_1D_P0( Real x )          /* -- Element P0 --- */
{
   Real val;
   return val = 1.0 ;
}


Real F0_1D_P1( Real x )          /* -- Element P1 --- */ 
{
   Real val;
   return val = 1.0 - x ;
}
Real F1_1D_P1( Real x )
{
   Real val;
   return val = x ;
}


Real F0_1D_P2( Real x )          /* -- Element P2 --- */
{
   Real val;
   return val = 2.0*(1.0-x)*(0.5-x);
}
Real F1_1D_P2( Real x )
{
   Real val;
   return val = 4.0*(x)*(1.0-x);
}
Real F2_1D_P2( Real x )
{
   Real val;
   return val = 2.0*(x)*(x-0.5);
}


Real F0_1D_P3( Real x )          /* -- Element P3 --- */
{
   Real val;
   return val = 9.0/2.0*(1.0-x)*(1.0/3.0-x)*(2.0/3.0-x);
}
Real F1_1D_P3( Real x )
{
   Real val;
   return val = 27.0/2.0*(x)*(1.0-x)*(2.0/3.0-x);
}
Real F2_1D_P3( Real x )
{
   Real val;
   return val = 27.0/2.0*(x)*(x-1.0/3.0)*(1.0-x);
}
Real F3_1D_P3( Real x )
{
   Real val;
   return val = 9.0/2.0*(x)*(x-1.0/3.0)*(x-2.0/3.0);
}


Real F0_1D_H3( Real x )          /* -- Element H3 --- */
{
   Real val;
   return val = (x-1.0)*(x-1.0)*(2.0*x+1.0);
}
Real F1_1D_H3( Real x )
{
   Real val;
   return val = x*(x-1.0)*(x-1.0);
}
Real F2_1D_H3( Real x )
{
   Real val;
   return val = x*x*(-2.0*x+3.0);
}
Real F3_1D_H3( Real x )
{
   Real val;
   return val = x*x*(x-1.0);
}


Real F0_1D_S2( Real x )          /* -- Element S2 --- */
{
   Real val;
   return val = SQR(1.0-x);
}
Real F1_1D_S2( Real x )
{
   Real val;
   return val = 3.0 - 2.0*x - 2.0*SQR(1.0 - x);
}
Real F2_1D_S2( Real x )
{
   Real val;
   return val = x*x;
}



Real F0_1D_S3( Real x )          /* -- Element S3 --- */
{
   Real val;
   Real tmp = 1.0-x;
   return val = CUB(tmp);
}
Real F1_1D_S3( Real x )
{
   Real val;
   Real tmp = 1.0-x;
   return val = 4.0 -3*x + 3*SQR(tmp) - 3*CUB(tmp);
}
Real F2_1D_S3( Real x )
{
   Real val;
   return val = 1.0 +3.0*x + 3*SQR(x) - 3.0*CUB(x); 
}
Real F3_1D_S3( Real x )
{
   Real val;
   return val = CUB(x);
}


//B1(t) =    (1-t)**4
//B2(t) = -4*(1-t)**4 +  4*(1-t)**3 + 6*(1-t)**2 + 4*(1-t)**1 + 1
//B3(t) =  6*t**4 - 12*t**3 - 6*t**2 + 12*t**1 + 11
//B4(t) = -4*t**4 +  4*t**3 + 6*t**2 +  4*t**1 + 1
//B5(t) =    t**4

Real F0_1D_S4( Real x )          /* -- Element S4 --- */
{
   Real val;
   return val = QUA(1-x);
}
Real F1_1D_S4( Real x )
{
   Real val;
   return val = -4*QUA(1-x) +  4*CUB(1-x) + 6*SQR(1-x) + 4*(1-x) + 1;
}
Real F2_1D_S4( Real x )
{
   Real val;
   return val = 6*QUA(x) - 12*CUB(x) - 6*SQR(x) + 12*x + 11;
}
Real F3_1D_S4( Real x )
{
   Real val;
   return val = -4*QUA(x)+  4*CUB(x) + 6*SQR(x) +  4*x + 1;
}
Real F4_1D_S4( Real x )
{
   Real val;
   return val = QUA(x);
}


Real F0_1D_S5( Real x )          /* -- Element S5 --- */
{
   Real val;
   return val = CIN(1.0-x);
}
Real F1_1D_S5( Real x )
{
   Real val;
   return val = 26.0 -50.0*x +20.0*SQR(x) +20.0*CUB(x) -20.0*QUA(x) + 5.0*CIN(x);
}
Real F2_1D_S5( Real x )
{
   Real val;
   return val = 66.0         -60.0*SQR(x)              +30.0*QUA(x) -10.0*CIN(x);
}
Real F3_1D_S5( Real x )
{
   Real val;
   return val = 10.0*CIN(x) -20.0*QUA(x) -20.0*CUB(x) +20.0*SQR(x) +50.0*x +26.0;
}
Real F4_1D_S5( Real x )
{
   Real val;
   return val = -5.0*CIN(x) + 5.0*QUA(x) +10.0*CUB(x) +10.0*SQR(x) + 5.0*x + 1.0;
}
Real F5_1D_S5( Real x )
{
   Real val;
   return val =      CIN(x);
}


/*  Derivees -------------------------------------- */

Real DFDX0_1D_P0( Real x )     /* -- Element P0 --- */
{
   Real val;
   return val = 0.0 ;
}


Real DFDX0_1D_P1( Real x )     /* -- Element P1 --- */ 
{
   Real val;
   return val = -1.0 ;
}
Real DFDX1_1D_P1( Real x )
{
   Real val;
   return val = 1.0 ;
}


Real DFDX0_1D_P2( Real x )    /* -- Element P2 --- */
{
   Real val;
   return val = 4.0*x - 3.0;
}
Real DFDX1_1D_P2( Real x )
{
   Real val;
   return val = -8.0*x + 4.0 ;
}
Real DFDX2_1D_P2( Real x )
{
   Real val;
   return val = 4.0*x - 1.0 ;
}


Real DFDX0_1D_P3( Real x )     /* -- Element P3 --- */
{
   Real val;
   return val =  -5.5 + 18.0*x - 13.5*x*x ;
}
Real DFDX1_1D_P3( Real x )
{
   Real val;
   return val = 9.0 - 45.0*x + 40.5*x*x ;
}
Real DFDX2_1D_P3( Real x )
{
   Real val;
   return val = - 4.5 + 36.0*x - 40.5*x*x ;
}
Real DFDX3_1D_P3( Real x )
{
   Real val;
   return val =  1.0 - 9.0*x + 13.5*x*x ;
}


Real DFDX0_1D_H3( Real x )     /* -- Element H3 --- */
{
   Real val;
   return val = 6.0*x*(x-1.0);
}
Real DFDX1_1D_H3( Real x )
{
   Real val;
   return val = (x-1.0)*(3.0*x-1.0);
}
Real DFDX2_1D_H3( Real x )
{
   Real val;
   return val = 2.0*x*(-3.0*x+3.0);
}
Real DFDX3_1D_H3( Real x )
{
   Real val;
   return val = x*(3.0*x-2.0);
}


Real DFDX0_1D_S2( Real x )          /* -- Element S2 --- */
{
   Real val;
   return val = -2.0 + 2.0*x;
}
Real DFDX1_1D_S2( Real x )
{
   Real val;
   return val = 2.0 - 4.0*x;
}
Real DFDX2_1D_S2( Real x )
{
   Real val;
   return val = 2.0*x;
}



Real DFDX0_1D_S3( Real x )     /* -- Element S3 --- */
{
   Real val;
   return val = -3.0*SQR(1.0-x);
}
Real DFDX1_1D_S3( Real x )
{
   Real val;
   return val = -9.0 + 6.0*x + 9.0*SQR(1.0-x);
}
Real DFDX2_1D_S3( Real x )
{
   Real val;
   return val = 3.0 + 6.0*x - 9.0*SQR(x);
}
Real DFDX3_1D_S3( Real x )
{
   Real val;
   return val = 3.0*SQR(x);
}




// val = QUA(1-x);
// val = -4*QUA(1-x) +  4*CUB(1-x) + 6*SQR(1-x) + 4*(1-x) + 1;
// val = 6*QUA(x) - 12*CUB(x) - 6*SQR(x) + 12*x + 11;
// val = -4*QUA(x)+  4*CUB(x) + 6*SQR(x) +  4*x + 1;
// val = QUA(x);



Real DFDX0_1D_S4( Real x )     /* -- Element S4 --- */
{
   Real val;
   return val = -4*CUB(1-x);
}
Real DFDX1_1D_S4( Real x )
{
   Real val;
   return val =  16*CUB(1-x) - 12*SQR(1-x) - 12*(1-x) - 4;
}
Real DFDX2_1D_S4( Real x )
{
   Real val;
   return val = 24*CUB(x) - 36*SQR(x) - 12*x + 12;
}
Real DFDX3_1D_S4( Real x )
{
   Real val;
   return val = -16*CUB(x)+  12*SQR(x) + 12*x +  4;
}
Real DFDX4_1D_S4( Real x )
{
   Real val;
   return val = 4*CUB(x);
}



Real DFDX0_1D_S5( Real x )     /* -- Element S5 --- */
{
   Real val;
   return val = -5.0*QUA(1.0-x);
}
Real DFDX1_1D_S5( Real x )
{
   Real val;
   return val = -50.0 + 40.0*x + 60.0*SQR(x) - 80.0*CUB(x)  + 25.0*QUA(x);
}
Real DFDX2_1D_S5( Real x )
{
   Real val;
   return val = - 120.0*x + 120.0*CUB(x)  - 50.0*QUA(x);
}
Real DFDX3_1D_S5( Real x )
{
   Real val;
   return val = 50.0*QUA(x) - 80.0*CUB(x) - 60.0*SQR(x) + 40.0*x + 50.0;
}
Real DFDX4_1D_S5( Real x )
{
   Real val;
   return val =-25.0*QUA(x) + 20.0*CUB(x) + 30.0*SQR(x) + 20.0*x + 5.0;
}
Real DFDX5_1D_S5( Real x )
{
   Real val;
   return val = 5.0*QUA(x);
}

/*  Derivees secondes -------------------------------- */

Real DDFDXX0_1D_P0( Real x )      /* -- Element P0 --- */
{
   Real val;
   return val = 0.0 ;
}


Real DDFDXX0_1D_P1( Real x )      /* -- Element P1 --- */ 
{
   Real val;
   return val = 0.0 ;
}
Real DDFDXX1_1D_P1( Real x )
{
   Real val;
   return val = 0.0 ;
}


Real DDFDXX0_1D_P2( Real x )     /* -- Element P2 --- */
{
   Real val;
   return val = 4.0 ;
}
Real DDFDXX1_1D_P2( Real x )
{
   Real val;
   return val = -8.0 ;
}
Real DDFDXX2_1D_P2( Real x )
{
   Real val;
   return val = 4.0 ;
}


Real DDFDXX0_1D_P3( Real x )     /* -- Element P3 --- */
{
   Real val;
   return val =  18.0 - 27.0*x ;
}
Real DDFDXX1_1D_P3( Real x )
{
   Real val;
   return val =  - 27.0 + 81.0*x ;
}
Real DDFDXX2_1D_P3( Real x )
{
   Real val;
   return val =  36.0 - 81.0*x ;
}
Real DDFDXX3_1D_P3( Real x )
{
   Real val;
   return val =  - 9.0 + 27.0*x ;
}



Real DDFDXX0_1D_H3( Real x )    /* -- Element H3 --- */
{
   Real val;
   return val = 12.0*x - 6.0 ;
}
Real DDFDXX1_1D_H3( Real x )
{
   Real val;
   return val =  6.0*x - 4.0 ;
}
Real DDFDXX2_1D_H3( Real x )
{
   Real val;
   return val = -12.0*x + 6.0 ;
}
Real DDFDXX3_1D_H3( Real x )
{
   Real val;
   return val =  6.0*x - 2.0 ;
}


Real DDFDXX0_1D_S2( Real x )          /* -- Element S2 --- */
{
   Real val;
   return val = 2.0;
}
Real DDFDXX1_1D_S2( Real x )
{
   Real val;
   return val = -4.0;
}
Real DDFDXX2_1D_S2( Real x )
{
   Real val;
   return val = 2.0;
}



Real DDFDXX0_1D_S3( Real x )    /* -- Element S3 --- */
{
   Real val;
   return val = 6.0 -6.0*x ;
}
Real DDFDXX1_1D_S3( Real x )
{
   Real val;
   return val =  -12.0 + 18.0*x ;
}
Real DDFDXX2_1D_S3( Real x )
{
   Real val;
   return val = 6.0 - 18.0*x ;
}
Real DDFDXX3_1D_S3( Real x )
{
   Real val;
   return val =  6.0*x ;
}



//val = -4*CUB(1-x);
//val = +16*CUB(1-x) -  12*SQR(1-x) - 12*(1-x) - 4;
//val = 24*CUB(x) - 36*SQR(x) - 12*x + 12;
//val = -16*CUB(x)+  12*SQR(x) + 12*x +  4;
//val = 4*CUB(x);

Real DDFDXX0_1D_S4( Real x )    /* -- Element S4 --- */
{
   Real val;
   return val = 12*SQR(1-x);
}
Real DDFDXX1_1D_S4( Real x )
{
   Real val;
   return val = -16*3*SQR(1-x) + 24*(1-x) + 12;
}
Real DDFDXX2_1D_S4( Real x )
{
   Real val;
   return val = 24*3*SQR(x) - 72*x - 12;
}
Real DDFDXX3_1D_S4( Real x )
{
   Real val;
   return val = -16*3*SQR(x)+  24*x + 12;
}
Real DDFDXX4_1D_S4( Real x )
{
   Real val;
   return val = 12*SQR(x);
}



Real DDFDXX0_1D_S5( Real x )    /* -- Element S5 --- */
{
   Real val;
   return val =  20.0*CUB(1.0-x);
}
Real DDFDXX1_1D_S5( Real x )
{
   Real val;
   return val = 40.0 + 120.0*x - 240.0*SQR(x) + 100.0*CUB(x);
}
Real DDFDXX2_1D_S5( Real x )
{
   Real val;
   return val =  - 120.0 + 360.0*SQR(x) - 200.0*CUB(x);
}
Real DDFDXX3_1D_S5( Real x )
{
   Real val;
   return val = 200.0*CUB(x) - 240.0*SQR(x) - 120.0*x + 40.0;
}
Real DDFDXX4_1D_S5( Real x )
{
   Real val;
   return val =  - 100.0*CUB(x) + 60.0*SQR(x) + 60.0*x + 20.0;
}
Real DDFDXX5_1D_S5( Real x )
{
   Real val;
   return val = 20.0*CUB(x);
}
/* ------------------------------------------------------- */





