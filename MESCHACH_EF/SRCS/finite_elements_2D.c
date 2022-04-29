
/* Contains the structure of the finite element in 2 dimensions
 *
 */


#include "MESCHACH/INCLUDES/matrix.h"          /* Librairie Meschach    */
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"     /* for the 3D arrays     */

#include "MESCHACH_EF/INCLUDES/finite_elements.h"

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt2D_get_P1 (ELT_2D *elt);
static void elt2D_get_P2 (ELT_2D *elt);
static void elt2D_get_P3 (ELT_2D *elt);
static void elt2D_get_P1b(ELT_2D *elt);

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

ELT_2D *elt2D_get(const char *type)
{
   ELT_2D *elt;
   
   int    i,j;

   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/
   
   if ( (elt=NEW(ELT_2D)) == (ELT_2D *)NULL )
   {
      error(E_MEM, "ELT_2D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_ELT_2D, 0, sizeof(ELT_2D), MY_LIST2);
      mem_numvar_list(TYPE_ELT_2D, 1, MY_LIST2);
   }
   
   /* initialize also now the eltDM1 */
   if ( strcmp(type,"P1" )==0 ) elt->eltDM1 = elt1D_get("P1");
   if ( strcmp(type,"P2" )==0 ) elt->eltDM1 = elt1D_get("P2");
   if ( strcmp(type,"P3" )==0 ) elt->eltDM1 = elt1D_get("P3");
   if ( strcmp(type,"P1b")==0 ) elt->eltDM1 = elt1D_get("P1");

   /* and initialize our fe */
   elt->dim     = 2;
   elt->name_ef = strdup(type) ;
   
   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/
   
   if (strcmp(elt->name_ef,"P1") == 0)
   {
      elt2D_get_P1(elt);    
   }
   else
   if (strcmp(elt->name_ef,"P1b") == 0)
   {
      elt2D_get_P1b(elt);
   }
   else
   if (strcmp(elt->name_ef,"P2") == 0)
   {
      elt2D_get_P2(elt);
   }
   else
   if (strcmp(elt->name_ef,"P3") == 0)
   {
      elt2D_get_P3(elt);
   }
   else
   {
      error2(E_EF_NOTIMPL, "elt2D_get");
   }

   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/
   
   /* set frequently used matrices */
   elt->MAT_x_y__plus__y_x = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      elt->MAT_x_y__plus__y_x->me[i][j] = elt->MAT_x_y->me[i][j] + elt->MAT_y_x->me[i][j] ;
   }

   /* and with it the datas on the faces */
   elt->nb_somm_face = elt->eltDM1->nb_somm_cell ;
   
   /* elt->f_face  pointe vers (elt->nb_somm_face) fonctions de bases */
   elt->f_face       = (Real (**)(Real))calloc(elt->nb_somm_face,sizeof(Real (*)(Real)) );
   
   for (i=0; i<elt-> nb_somm_face; i++)
   {
      elt->f_face[i] = elt->eltDM1->f_base[i];
   }
   
   return (elt);
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt2D_get_P1 (ELT_2D *elt)
{
   /* Declaration de toutes les matrices pour l' element P1 ... */
   #include "MESCHACH_EF/INCLUDES/ef2D_P1_mtx.h"
   /* Declaration de toutes les matrices pour l' element P1 ... */
   
   int    i,j,k;
   int    DoF = 3;

   elt->nb_somm_cell = DoF ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxx_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdyy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );

   elt->f_base[0] = F0_2D_P1 ;
   elt->f_base[1] = F1_2D_P1 ;
   elt->f_base[2] = F2_2D_P1 ;

   elt->dfdx_base[0] = DFDX0_2D_P1 ;
   elt->dfdx_base[1] = DFDX1_2D_P1 ;
   elt->dfdx_base[2] = DFDX2_2D_P1 ;
   elt->dfdy_base[0] = DFDY0_2D_P1 ;
   elt->dfdy_base[1] = DFDY1_2D_P1 ;
   elt->dfdy_base[2] = DFDY2_2D_P1 ;

   elt->d2fdxx_base[0] = D2FDXX0_2D_P1 ;
   elt->d2fdxx_base[1] = D2FDXX1_2D_P1 ;
   elt->d2fdxx_base[2] = D2FDXX2_2D_P1 ;
   elt->d2fdxy_base[0] = D2FDXY0_2D_P1 ;
   elt->d2fdxy_base[1] = D2FDXY1_2D_P1 ;
   elt->d2fdxy_base[2] = D2FDXY2_2D_P1 ;
   elt->d2fdyy_base[0] = D2FDYY0_2D_P1 ;
   elt->d2fdyy_base[1] = D2FDYY1_2D_P1 ;
   elt->d2fdyy_base[2] = D2FDYY2_2D_P1 ;

   elt->VEC_I  = v_get(DoF);
   elt->VEC_x  = v_get(DoF);
   elt->VEC_y  = v_get(DoF);
   elt->VEC_xx = v_get(DoF);
   elt->VEC_xy = v_get(DoF);
   elt->VEC_yy = v_get(DoF);
   for (i=0; i<DoF; i++)
   {
      elt->VEC_I->ve[i]  = VEC_Masse_P1[i];

      elt->VEC_x->ve[i]  = VEC_Dx___P1[i];
      elt->VEC_y->ve[i]  = VEC_Dy___P1[i];

      elt->VEC_xx->ve[i] = VEC_DxDx_P1[i];
      elt->VEC_xy->ve[i] = VEC_DxDy_P1[i];
      elt->VEC_yy->ve[i] = VEC_DyDy_P1[i];
   }


   elt->MAT_I_I = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P1[i][j];
   }

   elt->MAT_x_x = m_get(DoF,DoF);
   elt->MAT_x_y = m_get(DoF,DoF);
   elt->MAT_y_x = m_get(DoF,DoF);
   elt->MAT_y_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P1[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P1[1][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P1[2][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P1[3][i][j];
   }

   elt->MAT_I_x = m_get(DoF,DoF);
   elt->MAT_I_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P1[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P1[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(3,3);
   elt->MAT_I_y_PM1dP = m_get(3,3);
   for (i=0; i<3; i++)
   for (j=0; j<3; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P1dP1[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P1dP1[i][j];
   }

   elt->TENSOR_I_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_x_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_y_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_x_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_y_I_I = ts_get(DoF,DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   for (k=0; k<DoF; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P1[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P1[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P1[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P1[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P1[j][i][k];
   }

   elt->MAT_xx_xx = m_get(DoF,DoF);
   elt->MAT_xx_xy = m_get(DoF,DoF);
   elt->MAT_xx_yy = m_get(DoF,DoF);
   elt->MAT_xy_xx = m_get(DoF,DoF);
   elt->MAT_xy_xy = m_get(DoF,DoF);
   elt->MAT_xy_yy = m_get(DoF,DoF);
   elt->MAT_yy_xx = m_get(DoF,DoF);
   elt->MAT_yy_xy = m_get(DoF,DoF);
   elt->MAT_yy_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_xx->me[i][j] = TENSOR_DD_DD_P1[0][i][j];
      elt->MAT_xx_xy->me[i][j] = TENSOR_DD_DD_P1[1][i][j];
      elt->MAT_xx_yy->me[i][j] = TENSOR_DD_DD_P1[2][i][j];
      elt->MAT_xy_xx->me[i][j] = TENSOR_DD_DD_P1[3][i][j];
      elt->MAT_xy_xy->me[i][j] = TENSOR_DD_DD_P1[4][i][j];
      elt->MAT_xy_yy->me[i][j] = TENSOR_DD_DD_P1[5][i][j];
      elt->MAT_yy_xx->me[i][j] = TENSOR_DD_DD_P1[6][i][j];
      elt->MAT_yy_xy->me[i][j] = TENSOR_DD_DD_P1[7][i][j];
      elt->MAT_yy_yy->me[i][j] = TENSOR_DD_DD_P1[8][i][j];
   }

   elt->MAT_xx_x = m_get(DoF,DoF);
   elt->MAT_xx_y = m_get(DoF,DoF);
   elt->MAT_xy_x = m_get(DoF,DoF);
   elt->MAT_xy_y = m_get(DoF,DoF);
   elt->MAT_yy_x = m_get(DoF,DoF);
   elt->MAT_yy_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_x->me[i][j] = TENSOR_DD_D__P1[0][i][j];
      elt->MAT_xx_y->me[i][j] = TENSOR_DD_D__P1[1][i][j];
      elt->MAT_xy_x->me[i][j] = TENSOR_DD_D__P1[2][i][j];
      elt->MAT_xy_y->me[i][j] = TENSOR_DD_D__P1[3][i][j];
      elt->MAT_yy_x->me[i][j] = TENSOR_DD_D__P1[4][i][j];
      elt->MAT_yy_y->me[i][j] = TENSOR_DD_D__P1[5][i][j];
   }

   elt->MAT_x_xx = m_get(DoF,DoF);
   elt->MAT_y_xx = m_get(DoF,DoF);
   elt->MAT_x_xy = m_get(DoF,DoF);
   elt->MAT_y_xy = m_get(DoF,DoF);
   elt->MAT_x_yy = m_get(DoF,DoF);
   elt->MAT_y_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_xx->me[i][j] = TENSOR_DD_D__P1[0][j][i];
      elt->MAT_y_xx->me[i][j] = TENSOR_DD_D__P1[1][j][i];
      elt->MAT_x_xy->me[i][j] = TENSOR_DD_D__P1[2][j][i];
      elt->MAT_y_xy->me[i][j] = TENSOR_DD_D__P1[3][j][i];
      elt->MAT_x_yy->me[i][j] = TENSOR_DD_D__P1[4][j][i];
      elt->MAT_y_yy->me[i][j] = TENSOR_DD_D__P1[5][j][i];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt2D_get_P2 (ELT_2D *elt)
{
   /* Declaration de toutes les matrices pour l' element P2 ... */
   #include "MESCHACH_EF/INCLUDES/ef2D_P2_mtx.h"
   /* Declaration de toutes les matrices pour l' element P2 ... */

   int    i,j,k;
   int    DoF = 6;

   elt-> nb_somm_cell = DoF ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxx_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdyy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );

   elt->f_base[0] = F0_2D_P2 ;
   elt->f_base[1] = F1_2D_P2 ;
   elt->f_base[2] = F2_2D_P2 ;
   elt->f_base[3] = F3_2D_P2 ;
   elt->f_base[4] = F4_2D_P2 ;
   elt->f_base[5] = F5_2D_P2 ;

   elt->dfdx_base[0] = DFDX0_2D_P2 ;
   elt->dfdx_base[1] = DFDX1_2D_P2 ;
   elt->dfdx_base[2] = DFDX2_2D_P2 ;
   elt->dfdx_base[3] = DFDX3_2D_P2 ;
   elt->dfdx_base[4] = DFDX4_2D_P2 ;
   elt->dfdx_base[5] = DFDX5_2D_P2 ;
   elt->dfdy_base[0] = DFDY0_2D_P2 ;
   elt->dfdy_base[1] = DFDY1_2D_P2 ;
   elt->dfdy_base[2] = DFDY2_2D_P2 ;
   elt->dfdy_base[3] = DFDY3_2D_P2 ;
   elt->dfdy_base[4] = DFDY4_2D_P2 ;
   elt->dfdy_base[5] = DFDY5_2D_P2 ;

   elt->d2fdxx_base[0] = D2FDXX0_2D_P2 ;
   elt->d2fdxx_base[1] = D2FDXX1_2D_P2 ;
   elt->d2fdxx_base[2] = D2FDXX2_2D_P2 ;
   elt->d2fdxx_base[3] = D2FDXX3_2D_P2 ;
   elt->d2fdxx_base[4] = D2FDXX4_2D_P2 ;
   elt->d2fdxx_base[5] = D2FDXX5_2D_P2 ;
   elt->d2fdxy_base[0] = D2FDXY0_2D_P2 ;
   elt->d2fdxy_base[1] = D2FDXY1_2D_P2 ;
   elt->d2fdxy_base[2] = D2FDXY2_2D_P2 ;
   elt->d2fdxy_base[3] = D2FDXY3_2D_P2 ;
   elt->d2fdxy_base[4] = D2FDXY4_2D_P2 ;
   elt->d2fdxy_base[5] = D2FDXY5_2D_P2 ;
   elt->d2fdyy_base[0] = D2FDYY0_2D_P2 ;
   elt->d2fdyy_base[1] = D2FDYY1_2D_P2 ;
   elt->d2fdyy_base[2] = D2FDYY2_2D_P2 ;
   elt->d2fdyy_base[3] = D2FDYY3_2D_P2 ;
   elt->d2fdyy_base[4] = D2FDYY4_2D_P2 ;
   elt->d2fdyy_base[5] = D2FDYY5_2D_P2 ;

   elt->VEC_I  = v_get(DoF);
   elt->VEC_x  = v_get(DoF);
   elt->VEC_y  = v_get(DoF);
   elt->VEC_xx = v_get(DoF);
   elt->VEC_xy = v_get(DoF);
   elt->VEC_yy = v_get(DoF);
   for (i=0; i<DoF; i++)
   {
      elt->VEC_I->ve[i] = VEC_Masse_P2[i];

      elt->VEC_x->ve[i]  = VEC_Dx___P2[i];
      elt->VEC_y->ve[i]  = VEC_Dy___P2[i];

      elt->VEC_xx->ve[i] = VEC_DxDx_P2[i];
      elt->VEC_xy->ve[i] = VEC_DxDy_P2[i];
      elt->VEC_yy->ve[i] = VEC_DyDy_P2[i];
   }


   elt->MAT_I_I = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P2[i][j];
   }

   elt->MAT_x_x = m_get(DoF,DoF);
   elt->MAT_x_y = m_get(DoF,DoF);
   elt->MAT_y_x = m_get(DoF,DoF);
   elt->MAT_y_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P2[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P2[1][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P2[2][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P2[3][i][j];
   }

   elt->MAT_I_x = m_get(DoF,DoF);
   elt->MAT_I_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P2[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P2[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(3,6);
   elt->MAT_I_y_PM1dP = m_get(3,6);
   for (i=0; i<3; i++)
   for (j=0; j<6; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P1dP2[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P1dP2[i][j];
   }

   elt->TENSOR_I_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_x_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_y_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_x_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_y_I_I = ts_get(DoF,DoF,DoF);
   for (i=0; i<DoF ; i++)
   for (j=0; j<DoF ; j++)
   for (k=0; k<DoF ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P2[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P2[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P2[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P2[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P2[j][i][k];
   }


   elt->MAT_xx_xx = m_get(DoF,DoF);
   elt->MAT_xx_xy = m_get(DoF,DoF);
   elt->MAT_xx_yy = m_get(DoF,DoF);
   elt->MAT_xy_xx = m_get(DoF,DoF);
   elt->MAT_xy_xy = m_get(DoF,DoF);
   elt->MAT_xy_yy = m_get(DoF,DoF);
   elt->MAT_yy_xx = m_get(DoF,DoF);
   elt->MAT_yy_xy = m_get(DoF,DoF);
   elt->MAT_yy_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_xx->me[i][j] = TENSOR_DD_DD_P2[0][i][j];
      elt->MAT_xx_xy->me[i][j] = TENSOR_DD_DD_P2[1][i][j];
      elt->MAT_xx_yy->me[i][j] = TENSOR_DD_DD_P2[2][i][j];
      elt->MAT_xy_xx->me[i][j] = TENSOR_DD_DD_P2[3][i][j];
      elt->MAT_xy_xy->me[i][j] = TENSOR_DD_DD_P2[4][i][j];
      elt->MAT_xy_yy->me[i][j] = TENSOR_DD_DD_P2[5][i][j];
      elt->MAT_yy_xx->me[i][j] = TENSOR_DD_DD_P2[6][i][j];
      elt->MAT_yy_xy->me[i][j] = TENSOR_DD_DD_P2[7][i][j];
      elt->MAT_yy_yy->me[i][j] = TENSOR_DD_DD_P2[8][i][j];
   }
      
   elt->MAT_xx_x = m_get(DoF,DoF);
   elt->MAT_xx_y = m_get(DoF,DoF);
   elt->MAT_xy_x = m_get(DoF,DoF);
   elt->MAT_xy_y = m_get(DoF,DoF);
   elt->MAT_yy_x = m_get(DoF,DoF);
   elt->MAT_yy_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_x->me[i][j] = TENSOR_DD_D__P2[0][i][j];
      elt->MAT_xx_y->me[i][j] = TENSOR_DD_D__P2[1][i][j];
      elt->MAT_xy_x->me[i][j] = TENSOR_DD_D__P2[2][i][j];
      elt->MAT_xy_y->me[i][j] = TENSOR_DD_D__P2[3][i][j];
      elt->MAT_yy_x->me[i][j] = TENSOR_DD_D__P2[4][i][j];
      elt->MAT_yy_y->me[i][j] = TENSOR_DD_D__P2[5][i][j];
   }

   elt->MAT_x_xx = m_get(DoF,DoF);
   elt->MAT_y_xx = m_get(DoF,DoF);
   elt->MAT_x_xy = m_get(DoF,DoF);
   elt->MAT_y_xy = m_get(DoF,DoF);
   elt->MAT_x_yy = m_get(DoF,DoF);
   elt->MAT_y_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_xx->me[i][j] = TENSOR_DD_D__P2[0][j][i];
      elt->MAT_y_xx->me[i][j] = TENSOR_DD_D__P2[1][j][i];
      elt->MAT_x_xy->me[i][j] = TENSOR_DD_D__P2[2][j][i];
      elt->MAT_y_xy->me[i][j] = TENSOR_DD_D__P2[3][j][i];
      elt->MAT_x_yy->me[i][j] = TENSOR_DD_D__P2[4][j][i];
      elt->MAT_y_yy->me[i][j] = TENSOR_DD_D__P2[5][j][i];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt2D_get_P3 (ELT_2D *elt)
{
   /* Declaration de toutes les matrices pour l' element P3 ... */
   #include "MESCHACH_EF/INCLUDES/ef2D_P3_mtx.h"
   /* Declaration de toutes les matrices pour l' element P3 ... */

   int    i,j,k;
   int    DoF = 10;

   elt-> nb_somm_cell = DoF ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxx_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdyy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );

   elt->f_base[0] = F0_2D_P3 ;
   elt->f_base[1] = F1_2D_P3 ;
   elt->f_base[2] = F2_2D_P3 ;
   elt->f_base[3] = F3_2D_P3 ;
   elt->f_base[4] = F4_2D_P3 ;
   elt->f_base[5] = F5_2D_P3 ;
   elt->f_base[6] = F6_2D_P3 ;
   elt->f_base[7] = F7_2D_P3 ;
   elt->f_base[8] = F8_2D_P3 ;
   elt->f_base[9] = F9_2D_P3 ;

   elt->dfdx_base[0] = DFDX0_2D_P3 ;
   elt->dfdx_base[1] = DFDX1_2D_P3 ;
   elt->dfdx_base[2] = DFDX2_2D_P3 ;
   elt->dfdx_base[3] = DFDX3_2D_P3 ;
   elt->dfdx_base[4] = DFDX4_2D_P3 ;
   elt->dfdx_base[5] = DFDX5_2D_P3 ;
   elt->dfdx_base[6] = DFDX6_2D_P3 ;
   elt->dfdx_base[7] = DFDX7_2D_P3 ;
   elt->dfdx_base[8] = DFDX8_2D_P3 ;
   elt->dfdx_base[9] = DFDX9_2D_P3 ;

   elt->dfdy_base[0] = DFDY0_2D_P3 ;
   elt->dfdy_base[1] = DFDY1_2D_P3 ;
   elt->dfdy_base[2] = DFDY2_2D_P3 ;
   elt->dfdy_base[3] = DFDY3_2D_P3 ;
   elt->dfdy_base[4] = DFDY4_2D_P3 ;
   elt->dfdy_base[5] = DFDY5_2D_P3 ;
   elt->dfdy_base[6] = DFDY6_2D_P3 ;
   elt->dfdy_base[7] = DFDY7_2D_P3 ;
   elt->dfdy_base[8] = DFDY8_2D_P3 ;
   elt->dfdy_base[9] = DFDY9_2D_P3 ;

   elt->d2fdxx_base[0] = D2FDXX0_2D_P3 ;
   elt->d2fdxx_base[1] = D2FDXX1_2D_P3 ;
   elt->d2fdxx_base[2] = D2FDXX2_2D_P3 ;
   elt->d2fdxx_base[3] = D2FDXX3_2D_P3 ;
   elt->d2fdxx_base[4] = D2FDXX4_2D_P3 ;
   elt->d2fdxx_base[5] = D2FDXX5_2D_P3 ;
   elt->d2fdxx_base[6] = D2FDXX6_2D_P3 ;
   elt->d2fdxx_base[7] = D2FDXX7_2D_P3 ;
   elt->d2fdxx_base[8] = D2FDXX8_2D_P3 ;
   elt->d2fdxx_base[9] = D2FDXX9_2D_P3 ;

   elt->d2fdxy_base[0] = D2FDXY0_2D_P3 ;
   elt->d2fdxy_base[1] = D2FDXY1_2D_P3 ;
   elt->d2fdxy_base[2] = D2FDXY2_2D_P3 ;
   elt->d2fdxy_base[3] = D2FDXY3_2D_P3 ;
   elt->d2fdxy_base[4] = D2FDXY4_2D_P3 ;
   elt->d2fdxy_base[5] = D2FDXY5_2D_P3 ;
   elt->d2fdxy_base[6] = D2FDXY6_2D_P3 ;
   elt->d2fdxy_base[7] = D2FDXY7_2D_P3 ;
   elt->d2fdxy_base[8] = D2FDXY8_2D_P3 ;
   elt->d2fdxy_base[9] = D2FDXY9_2D_P3 ;

   elt->d2fdyy_base[0] = D2FDYY0_2D_P3 ;
   elt->d2fdyy_base[1] = D2FDYY1_2D_P3 ;
   elt->d2fdyy_base[2] = D2FDYY2_2D_P3 ;
   elt->d2fdyy_base[3] = D2FDYY3_2D_P3 ;
   elt->d2fdyy_base[4] = D2FDYY4_2D_P3 ;
   elt->d2fdyy_base[5] = D2FDYY5_2D_P3 ;
   elt->d2fdyy_base[6] = D2FDYY6_2D_P3 ;
   elt->d2fdyy_base[7] = D2FDYY7_2D_P3 ;
   elt->d2fdyy_base[8] = D2FDYY8_2D_P3 ;
   elt->d2fdyy_base[9] = D2FDYY9_2D_P3 ;

   elt->VEC_I  = v_get(DoF);
   elt->VEC_x  = v_get(DoF);
   elt->VEC_y  = v_get(DoF);
   elt->VEC_xx = v_get(DoF);
   elt->VEC_xy = v_get(DoF);
   elt->VEC_yy = v_get(DoF);
   for (i=0; i<DoF; i++)
   {
      elt->VEC_I->ve[i] = VEC_Masse_P3[i];

      elt->VEC_x->ve[i]  = VEC_Dx___P3[i];
      elt->VEC_y->ve[i]  = VEC_Dy___P3[i];

      elt->VEC_xx->ve[i] = VEC_DxDx_P3[i];
      elt->VEC_xy->ve[i] = VEC_DxDy_P3[i];
      elt->VEC_yy->ve[i] = VEC_DyDy_P3[i];
   }


   elt->MAT_I_I = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P3[i][j];
   }

   elt->MAT_x_x = m_get(DoF,DoF);
   elt->MAT_x_y = m_get(DoF,DoF);
   elt->MAT_y_x = m_get(DoF,DoF);
   elt->MAT_y_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P3[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P3[1][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P3[2][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P3[3][i][j];
   }

   elt->MAT_I_x = m_get(DoF,DoF);
   elt->MAT_I_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P3[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P3[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(6,10);
   elt->MAT_I_y_PM1dP = m_get(6,10);
   for (i=0; i<6; i++)
   for (j=0; j<10;j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P2dP3[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P2dP3[i][j];
   }

   elt->TENSOR_I_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_x_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_y_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_x_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_y_I_I = ts_get(DoF,DoF,DoF);
   for (i=0; i<DoF ; i++)
   for (j=0; j<DoF ; j++)
   for (k=0; k<DoF ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P3[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P3[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P3[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P3[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P3[j][i][k];
   }



   elt->MAT_xx_xx = m_get(DoF,DoF);
   elt->MAT_xx_xy = m_get(DoF,DoF);
   elt->MAT_xx_yy = m_get(DoF,DoF);
   elt->MAT_xy_xx = m_get(DoF,DoF);
   elt->MAT_xy_xy = m_get(DoF,DoF);
   elt->MAT_xy_yy = m_get(DoF,DoF);
   elt->MAT_yy_xx = m_get(DoF,DoF);
   elt->MAT_yy_xy = m_get(DoF,DoF);
   elt->MAT_yy_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_xx->me[i][j] = TENSOR_DD_DD_P3[0][i][j];
      elt->MAT_xx_xy->me[i][j] = TENSOR_DD_DD_P3[1][i][j];
      elt->MAT_xx_yy->me[i][j] = TENSOR_DD_DD_P3[2][i][j];
      elt->MAT_xy_xx->me[i][j] = TENSOR_DD_DD_P3[3][i][j];
      elt->MAT_xy_xy->me[i][j] = TENSOR_DD_DD_P3[4][i][j];
      elt->MAT_xy_yy->me[i][j] = TENSOR_DD_DD_P3[5][i][j];
      elt->MAT_yy_xx->me[i][j] = TENSOR_DD_DD_P3[6][i][j];
      elt->MAT_yy_xy->me[i][j] = TENSOR_DD_DD_P3[7][i][j];
      elt->MAT_yy_yy->me[i][j] = TENSOR_DD_DD_P3[8][i][j];
   }

   elt->MAT_xx_x = m_get(DoF,DoF);
   elt->MAT_xx_y = m_get(DoF,DoF);
   elt->MAT_xy_x = m_get(DoF,DoF);
   elt->MAT_xy_y = m_get(DoF,DoF);
   elt->MAT_yy_x = m_get(DoF,DoF);
   elt->MAT_yy_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_x->me[i][j] = TENSOR_DD_D__P3[0][i][j];
      elt->MAT_xx_y->me[i][j] = TENSOR_DD_D__P3[1][i][j];
      elt->MAT_xy_x->me[i][j] = TENSOR_DD_D__P3[2][i][j];
      elt->MAT_xy_y->me[i][j] = TENSOR_DD_D__P3[3][i][j];
      elt->MAT_yy_x->me[i][j] = TENSOR_DD_D__P3[4][i][j];
      elt->MAT_yy_y->me[i][j] = TENSOR_DD_D__P3[5][i][j];
   }

   elt->MAT_x_xx = m_get(DoF,DoF);
   elt->MAT_y_xx = m_get(DoF,DoF);
   elt->MAT_x_xy = m_get(DoF,DoF);
   elt->MAT_y_xy = m_get(DoF,DoF);
   elt->MAT_x_yy = m_get(DoF,DoF);
   elt->MAT_y_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_xx->me[i][j] = TENSOR_DD_D__P3[0][j][i];
      elt->MAT_y_xx->me[i][j] = TENSOR_DD_D__P3[1][j][i];
      elt->MAT_x_xy->me[i][j] = TENSOR_DD_D__P3[2][j][i];
      elt->MAT_y_xy->me[i][j] = TENSOR_DD_D__P3[3][j][i];
      elt->MAT_x_yy->me[i][j] = TENSOR_DD_D__P3[4][j][i];
      elt->MAT_y_yy->me[i][j] = TENSOR_DD_D__P3[5][j][i];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt2D_get_P1b(ELT_2D *elt)
{
   /* Declaration de toutes les matrices pour l' element P1b ... */
   #include "MESCHACH_EF/INCLUDES/ef2D_P1b_mtx.h"
   /* Declaration de toutes les matrices pour l' element P1b ... */
   
   int    i,j,k;
   int    DoF = 4;

   elt-> nb_somm_cell = DoF ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxx_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdxy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );
   elt->d2fdyy_base = (Real (**)(Real,Real))calloc(DoF, sizeof(Real (*)(Real,Real)) );

   elt->f_base[0] = F0_2D_P1 ;
   elt->f_base[1] = F1_2D_P1 ;
   elt->f_base[2] = F2_2D_P1 ;
   elt->f_base[3] = F0_2D_P1b;

   elt->dfdx_base[0] = DFDX0_2D_P1 ;
   elt->dfdx_base[1] = DFDX1_2D_P1 ;
   elt->dfdx_base[2] = DFDX2_2D_P1 ;
   elt->dfdx_base[3] = DFDX0_2D_P1b;

   elt->dfdy_base[0] = DFDY0_2D_P1 ;
   elt->dfdy_base[1] = DFDY1_2D_P1 ;
   elt->dfdy_base[2] = DFDY2_2D_P1 ;
   elt->dfdy_base[3] = DFDY0_2D_P1b;

   elt->d2fdxx_base[0] = D2FDXX0_2D_P1 ;
   elt->d2fdxx_base[1] = D2FDXX1_2D_P1 ;
   elt->d2fdxx_base[2] = D2FDXX2_2D_P1 ;
   elt->d2fdxx_base[3] = D2FDXX0_2D_P1b;

   elt->d2fdxy_base[0] = D2FDXY0_2D_P1 ;
   elt->d2fdxy_base[1] = D2FDXY1_2D_P1 ;
   elt->d2fdxy_base[2] = D2FDXY2_2D_P1 ;
   elt->d2fdxy_base[3] = D2FDXY0_2D_P1b;

   elt->d2fdyy_base[0] = D2FDYY0_2D_P1 ;
   elt->d2fdyy_base[1] = D2FDYY1_2D_P1 ;
   elt->d2fdyy_base[2] = D2FDYY2_2D_P1 ;
   elt->d2fdyy_base[3] = D2FDYY0_2D_P1b;


   elt->VEC_I  = v_get(DoF);
   elt->VEC_x  = v_get(DoF);
   elt->VEC_y  = v_get(DoF);
   elt->VEC_xx = v_get(DoF);
   elt->VEC_xy = v_get(DoF);
   elt->VEC_yy = v_get(DoF);
   for (i=0; i<DoF; i++)
   {
      elt->VEC_I->ve[i] = VEC_Masse_P1b[i];

      elt->VEC_x->ve[i]  = VEC_Dx___P1b[i];
      elt->VEC_y->ve[i]  = VEC_Dy___P1b[i];

      elt->VEC_xx->ve[i] = VEC_DxDx_P1b[i];
      elt->VEC_xy->ve[i] = VEC_DxDy_P1b[i];
      elt->VEC_yy->ve[i] = VEC_DyDy_P1b[i];
   }


   elt->MAT_I_I = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P1b[i][j];
   }

   elt->MAT_x_x = m_get(DoF,DoF);
   elt->MAT_x_y = m_get(DoF,DoF);
   elt->MAT_y_x = m_get(DoF,DoF);
   elt->MAT_y_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P1b[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P1b[1][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P1b[2][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P1b[3][i][j];
   }

   elt->MAT_I_x = m_get(DoF,DoF);
   elt->MAT_I_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P1b[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P1b[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(3,4);
   elt->MAT_I_y_PM1dP = m_get(3,4);
   for (i=0; i<3; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P1dP1b[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P1dP1b[i][j];
   }


   elt->TENSOR_I_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_x_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_I_y_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_x_I_I = ts_get(DoF,DoF,DoF);
   elt->TENSOR_y_I_I = ts_get(DoF,DoF,DoF);
   for (i=0; i<DoF ; i++)
   for (j=0; j<DoF ; j++)
   for (k=0; k<DoF ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P1b[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P1b[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P1b[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P1b[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P1b[j][i][k];
   }



   elt->MAT_xx_xx = m_get(DoF,DoF);
   elt->MAT_xx_xy = m_get(DoF,DoF);
   elt->MAT_xx_yy = m_get(DoF,DoF);
   elt->MAT_xy_xx = m_get(DoF,DoF);
   elt->MAT_xy_xy = m_get(DoF,DoF);
   elt->MAT_xy_yy = m_get(DoF,DoF);
   elt->MAT_yy_xx = m_get(DoF,DoF);
   elt->MAT_yy_xy = m_get(DoF,DoF);
   elt->MAT_yy_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_xx->me[i][j] = TENSOR_DD_DD_P1b[0][i][j];
      elt->MAT_xx_xy->me[i][j] = TENSOR_DD_DD_P1b[1][i][j];
      elt->MAT_xx_yy->me[i][j] = TENSOR_DD_DD_P1b[2][i][j];
      elt->MAT_xy_xx->me[i][j] = TENSOR_DD_DD_P1b[3][i][j];
      elt->MAT_xy_xy->me[i][j] = TENSOR_DD_DD_P1b[4][i][j];
      elt->MAT_xy_yy->me[i][j] = TENSOR_DD_DD_P1b[5][i][j];
      elt->MAT_yy_xx->me[i][j] = TENSOR_DD_DD_P1b[6][i][j];
      elt->MAT_yy_xy->me[i][j] = TENSOR_DD_DD_P1b[7][i][j];
      elt->MAT_yy_yy->me[i][j] = TENSOR_DD_DD_P1b[8][i][j];
   }

   elt->MAT_xx_x = m_get(DoF,DoF);
   elt->MAT_xx_y = m_get(DoF,DoF);
   elt->MAT_xy_x = m_get(DoF,DoF);
   elt->MAT_xy_y = m_get(DoF,DoF);
   elt->MAT_yy_x = m_get(DoF,DoF);
   elt->MAT_yy_y = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_xx_x->me[i][j] = TENSOR_DD_D__P1b[0][i][j];
      elt->MAT_xx_y->me[i][j] = TENSOR_DD_D__P1b[1][i][j];
      elt->MAT_xy_x->me[i][j] = TENSOR_DD_D__P1b[2][i][j];
      elt->MAT_xy_y->me[i][j] = TENSOR_DD_D__P1b[3][i][j];
      elt->MAT_yy_x->me[i][j] = TENSOR_DD_D__P1b[4][i][j];
      elt->MAT_yy_y->me[i][j] = TENSOR_DD_D__P1b[5][i][j];
   }

   elt->MAT_x_xx = m_get(DoF,DoF);
   elt->MAT_y_xx = m_get(DoF,DoF);
   elt->MAT_x_xy = m_get(DoF,DoF);
   elt->MAT_y_xy = m_get(DoF,DoF);
   elt->MAT_x_yy = m_get(DoF,DoF);
   elt->MAT_y_yy = m_get(DoF,DoF);
   for (i=0; i<DoF; i++)
   for (j=0; j<DoF; j++)
   {
      elt->MAT_x_xx->me[i][j] = TENSOR_DD_D__P1b[0][j][i];
      elt->MAT_y_xx->me[i][j] = TENSOR_DD_D__P1b[1][j][i];
      elt->MAT_x_xy->me[i][j] = TENSOR_DD_D__P1b[2][j][i];
      elt->MAT_y_xy->me[i][j] = TENSOR_DD_D__P1b[3][j][i];
      elt->MAT_x_yy->me[i][j] = TENSOR_DD_D__P1b[4][j][i];
      elt->MAT_y_yy->me[i][j] = TENSOR_DD_D__P1b[5][j][i];
   }         
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

int elt2D_free(ELT_2D *elt)
{
   if ( elt == (ELT_2D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_ELT_2D, sizeof(ELT_2D), 0, MY_LIST2);
         mem_numvar_list(TYPE_ELT_2D, -1, MY_LIST2);
      }

      free(elt->name_ef);
      
      V_FREE(elt->VEC_I);
      V_FREE(elt->VEC_x);
      V_FREE(elt->VEC_y);
      V_FREE(elt->VEC_xx);
      V_FREE(elt->VEC_xy);
      V_FREE(elt->VEC_yy);
      
      M_FREE(elt->MAT_I_I);
      
      M_FREE(elt->MAT_x_x);
      M_FREE(elt->MAT_x_y);
      M_FREE(elt->MAT_y_x);
      M_FREE(elt->MAT_y_y);
      
      M_FREE(elt->MAT_I_x);
      M_FREE(elt->MAT_I_y);

      M_FREE(elt->MAT_xx_x);
      M_FREE(elt->MAT_xx_y);
      M_FREE(elt->MAT_xy_x);
      M_FREE(elt->MAT_xy_y);
      M_FREE(elt->MAT_yy_x);
      M_FREE(elt->MAT_yy_y);

      M_FREE(elt->MAT_x_xx);
      M_FREE(elt->MAT_y_xx);
      M_FREE(elt->MAT_x_xy);
      M_FREE(elt->MAT_y_xy);
      M_FREE(elt->MAT_x_yy);
      M_FREE(elt->MAT_y_yy);

      M_FREE(elt->MAT_xx_xx);
      M_FREE(elt->MAT_xx_xy);
      M_FREE(elt->MAT_xx_yy);
      M_FREE(elt->MAT_xy_xx);
      M_FREE(elt->MAT_xy_xy);
      M_FREE(elt->MAT_xy_yy);
      M_FREE(elt->MAT_yy_xx);
      M_FREE(elt->MAT_yy_xy);
      M_FREE(elt->MAT_yy_yy);

      
      M_FREE(elt->MAT_I_x_PM1dP);
      M_FREE(elt->MAT_I_y_PM1dP);
      
      TS_FREE(elt->TENSOR_I_x_I);
      TS_FREE(elt->TENSOR_I_y_I);
      TS_FREE(elt->TENSOR_x_I_I);
      TS_FREE(elt->TENSOR_y_I_I);
      TS_FREE(elt->TENSOR_I_I_I);

      M_FREE(elt->MAT_x_y__plus__y_x);

      
      ELT_1D_FREE(elt->eltDM1);

      free(elt->f_base);
      free(elt->dfdx_base);
      free(elt->dfdy_base);
      free(elt->d2fdxx_base);
      free(elt->d2fdxy_base);
      free(elt->d2fdyy_base);
      
      free(elt->f_face);
      
      /* free the space of the element */
      free(elt);
            
      return EXIT_SUCCESS;
   }
   
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

/*  Fonctions de base ** 2D ** ---------------------- */

Real F0_2D_P0(Real x , Real y)   /* -- Element P0 --- */ 
{
   Real val;
   return val = 1.0;
}


Real F0_2D_P1(Real x , Real y)   /* -- Element P1 --- */ 
{
   Real val;
   return val = 1.0-x-y;
}
Real F1_2D_P1(Real x , Real y)
{
   Real val;
   return val = x;
}
Real F2_2D_P1(Real x , Real y)
{
   Real val;
   return val = y;
}


Real F0_2D_P2(Real x , Real y)   /* -- Element P2 --- */ 
{
   Real val;
   return val = (1.0 - x - y )*( 1.0 -2*x - 2*y) ;
}
Real F1_2D_P2(Real x , Real y)
{
   Real val;
   return val = x*(2*x -1.0 ) ;
}
Real F2_2D_P2(Real x , Real y)
{
   Real val;
   return val = y*(2*y -1.0) ;
}
Real F3_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*x*(1.0-x-y) ;
}
Real F4_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*x*y ;
}
Real F5_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*y*(1.0-x-y);
}


Real F0_2D_P3(Real x , Real y)   /* -- Element P3 --- */ 
{
   Real val;
   return val = 1.0 - 5.5*x - 5.5*y +  9.0*x*x + 18.0*x*y +  9.0*y*y -  4.5*x*x*x - 13.5*x*x*y - 13.5*x*y*y -  4.5*y*y*y ;
}
Real F1_2D_P3(Real x , Real y)
{
   Real val;
   return val =      1.0*x          -  4.5*x*x                       +  4.5*x*x*x ;
}
Real F2_2D_P3(Real x , Real y)
{
   Real val;
   return val =              1.0*y                        -  4.5*y*y                                        +  4.5*y*y*y ;
}
Real F3_2D_P3(Real x , Real y)
{
   Real val;
   return val =      9.0*x           - 22.5*x*x - 22.5*x*y            + 13.5*x*x*x + 27.0*x*x*y + 13.5*x*y*y ;
}
Real F4_2D_P3(Real x , Real y)
{
   Real val;
   return val =    - 4.5*x           + 18.0*x*x +  4.5*x*y            - 13.5*x*x*x - 13.5*x*x*y   ;
}
Real F5_2D_P3(Real x , Real y)
{
   Real val;
   return val =                               -  4.5*x*y                         + 13.5*x*x*y;
}
Real F6_2D_P3(Real x , Real y)
{
   Real val;
   return val =                               -  4.5*x*y                                      + 13.5*x*y*y ;
}
Real F7_2D_P3(Real x , Real y)
{
   Real val;
   return val =            + 9.0*y            - 22.5*x*y - 22.5*y*y              + 13.5*x*x*y + 27.0*x*y*y + 13.5*y*y*y ;
}
Real F8_2D_P3(Real x , Real y)
{
   Real val;
   return val =            - 4.5*y            +  4.5*x*y + 18.0*y*y                           - 13.5*x*y*y - 13.5*y*y*y ;
}
Real F9_2D_P3(Real x , Real y)
{
   Real val;
   return val =                               + 27.0*x*y                         - 27.0*x*x*y - 27.0*x*y*y ;
}



Real F0_2D_P1b(Real x , Real y)   /* -- Element P1b --- */ 
{
   Real val;
   return val = 27*F0_2D_P1(x,y)*F1_2D_P1(x,y)*F2_2D_P1(x,y);
}


/*  Derivees des Fonctions de Base P1---------------- */

Real DFDX0_2D_P1(Real x , Real y)
{
   Real val;
   return val = -1.0;
}
Real DFDX1_2D_P1(Real x , Real y)
{
   Real val;
   return val = 1.0;
}
Real DFDX2_2D_P1(Real x , Real y)
{
   Real val;
   return val = 0.0;
}

Real DFDY0_2D_P1(Real x , Real y)
{
   Real val;
   return val = -1.0;
}
Real DFDY1_2D_P1(Real x , Real y)
{
   Real val;
   return val = 0.0;
}
Real DFDY2_2D_P1(Real x , Real y)
{
   Real val;
   return val = 1.0;
}


/*  Derivees des Fonctions de Base P2---------------- */

Real DFDX0_2D_P2(Real x , Real y)
{
   Real val;
   return val = -1*( 1.0 -2*x - 2*y) -2*(1.0 - x - y ) ;
}
Real DFDX1_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*x -1.0  ;
}
Real DFDX2_2D_P2(Real x , Real y)
{
   Real val;
   return val = 0.0 ;
}
Real DFDX3_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*(1.0-x-y) -4*x ;
}
Real DFDX4_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*y ;
}
Real DFDX5_2D_P2(Real x , Real y)
{
   Real val;
   return val = -4*y;
}

Real DFDY0_2D_P2(Real x , Real y)
{
   Real val;
   return val = (-1.0)*( 1.0 -2*x - 2*y) + (1.0 - x - y )*(-2) ;
}
Real DFDY1_2D_P2(Real x , Real y)
{
   Real val;
   return val = 0.0  ;
}
Real DFDY2_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*y - 1.0 ;
}
Real DFDY3_2D_P2(Real x , Real y)
{
   Real val;
   return val = -4*x ;
}
Real DFDY4_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*x ;
}
Real DFDY5_2D_P2(Real x , Real y)
{
   Real val;
   return val = 4*(1.0-x-y) - 4*y ;
}


/*  Derivees des Fonctions de Base P3---------------- */

Real DFDX0_2D_P3(Real x , Real y)   /* -- Element P3 --- */ 
{
   Real val;
   return val = - 5.5 +  18.0*x + 18.0*y  -  13.5*x*x - 27.0*x*y - 13.5*y*y  ;
}
Real DFDX1_2D_P3(Real x , Real y)
{
   Real val;
   return val =   1.0 -  9.0*x +  13.5*x*x ;
}
Real DFDX2_2D_P3(Real x , Real y)
{
   Real val;
   return val =   0.0  ;
}
Real DFDX3_2D_P3(Real x , Real y)
{
   Real val;
   return val =   9.0 - 45.0*x - 22.5*y  + 40.5*x*x + 54.0*x*y + 13.5*y*y ;
}
Real DFDX4_2D_P3(Real x , Real y)
{
   Real val;
   return val = - 4.5 + 36.0*x +  4.5*y  - 40.5*x*x - 27.0*x*y   ;
}
Real DFDX5_2D_P3(Real x , Real y)
{
   Real val;
   return val =       -  4.5*y + 27.0*x*y;
}
Real DFDX6_2D_P3(Real x , Real y)
{
   Real val;
   return val =       -  4.5*y + 13.5*y*y ;
}
Real DFDX7_2D_P3(Real x , Real y)
{
   Real val;
   return val =       - 22.5*y + 27.0*x*y + 27.0*y*y ;
}
Real DFDX8_2D_P3(Real x , Real y)
{
   Real val;
   return val =       +  4.5*y - 13.5*y*y ;
}
Real DFDX9_2D_P3(Real x , Real y)
{
   Real val;
   return val =       + 27.0*y - 54.0*x*y - 27.0*y*y ;
}



Real DFDY0_2D_P3(Real x , Real y)   /* -- Element P3 --- */ 
{
   Real val;
   return val = - 5.5   + 18.0*x +  18.0*y  - 13.5*x*x - 27.0*x*y -  13.5*y*y ;
}
Real DFDY1_2D_P3(Real x , Real y)
{
   Real val;
   return val =   0.0 ;
}
Real DFDY2_2D_P3(Real x , Real y)
{
   Real val;
   return val =   1.0 -  9.0*y  +  13.5*y*y ;
}
Real DFDY3_2D_P3(Real x , Real y)
{
   Real val;
   return val = - 22.5*x + 27.0*x*x + 27.0*x*y ;
}
Real DFDY4_2D_P3(Real x , Real y)
{
   Real val;
   return val =    4.5*x - 13.5*x*x ;
}
Real DFDY5_2D_P3(Real x , Real y)
{
   Real val;
   return val = -  4.5*x + 13.5*x*x ;
}
Real DFDY6_2D_P3(Real x , Real y)
{
   Real val;
   return val = -  4.5*x + 27.0*x*y ;
}
Real DFDY7_2D_P3(Real x , Real y)
{
   Real val;
   return val = + 9.0 - 22.5*x - 45.0*y + 13.5*x*x + 54.0*x*y + 40.5*y*y ;
}
Real DFDY8_2D_P3(Real x , Real y)
{
   Real val;
   return val = - 4.5 +  4.5*x + 36.0*y  - 27.0*x*y - 40.5*y*y ;
}
Real DFDY9_2D_P3(Real x , Real y)
{
   Real val;
   return val = + 27.0*x - 27.0*x*x - 54.0*x*y ;
}

/*  Derivees des Fonctions de Base P1b---------------- */

Real DFDX0_2D_P1b(Real x , Real y)
{
   Real val;
   return val = 27*DFDX0_2D_P1(x,y)*F1_2D_P1(x,y)*F2_2D_P1(x,y) + 
                27*DFDX1_2D_P1(x,y)*F0_2D_P1(x,y)*F2_2D_P1(x,y) +
                27*DFDX2_2D_P1(x,y)*F0_2D_P1(x,y)*F1_2D_P1(x,y) ;
}
Real DFDY0_2D_P1b(Real x , Real y)
{
   Real val;
   return val = 27*DFDY0_2D_P1(x,y)*F1_2D_P1(x,y)*F2_2D_P1(x,y) + 
                27*DFDY1_2D_P1(x,y)*F0_2D_P1(x,y)*F2_2D_P1(x,y) +
                27*DFDY2_2D_P1(x,y)*F0_2D_P1(x,y)*F1_2D_P1(x,y) ;

}

/* ------------------------------------------------------- */

/* on donne les derivees secondes ... pour l'element P1  */

Real D2FDXX0_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX1_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX2_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}

Real D2FDXY0_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXY1_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXY2_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}

Real D2FDYY0_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY1_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY2_2D_P1(Real x , Real y)
{
   Real val = 0.0;
   return val;
}

/*                       ... pour l'element P2  */

Real D2FDXX0_2D_P2(Real x , Real y)
{
   Real val = 4.0;
   return val;
}
Real D2FDXX1_2D_P2(Real x , Real y)
{
   Real val = 4.0;
   return val;
}
Real D2FDXX2_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX3_2D_P2(Real x , Real y)
{
   Real val = -8.0;
   return val;
}
Real D2FDXX4_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX5_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}

Real D2FDXY0_2D_P2(Real x , Real y)
{
   Real val = 4.0;
   return val;
}
Real D2FDXY1_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXY2_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXY3_2D_P2(Real x , Real y)
{
   Real val = -4.0;
   return val;
}
Real D2FDXY4_2D_P2(Real x , Real y)
{
   Real val = 4.0;
   return val;
}
Real D2FDXY5_2D_P2(Real x , Real y)
{
   Real val = -4.0;
   return val;
}

Real D2FDYY0_2D_P2(Real x , Real y)
{
   Real val = 4.0;
   return val;
}
Real D2FDYY1_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY2_2D_P2(Real x , Real y)
{
   Real val = 4.0;
   return val;
}
Real D2FDYY3_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY4_2D_P2(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY5_2D_P2(Real x , Real y)
{
   Real val = -8.0;
   return val;
}

/*                       ... pour l'element P3  */

Real D2FDXX0_2D_P3(Real x , Real y)
{
   Real val = 18.0 -27.0*x -27.0*y; 
   return val;
}
Real D2FDXX1_2D_P3(Real x , Real y)
{
   Real val = -9.0 +27.0*x;
   return val;
}
Real D2FDXX2_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX3_2D_P3(Real x , Real y)
{
   Real val = -45.0 +81.0*x +54.0*y; 
   return val;
}
Real D2FDXX4_2D_P3(Real x , Real y)
{
   Real val = 36.0 -81.0*x -27.0*y; 
   return val;
}
Real D2FDXX5_2D_P3(Real x , Real y)
{
   Real val = 27.0*y; 
   return val;
}
Real D2FDXX6_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX7_2D_P3(Real x , Real y)
{
   Real val = 27.0*y; 
   return val;
}
Real D2FDXX8_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXX9_2D_P3(Real x , Real y)
{
   Real val = -54.0*y; 
   return val;
}

Real D2FDXY0_2D_P3(Real x , Real y)
{
   Real val = 18.000000 -27.000000*x -27.000000*y; 
   return val;
}
Real D2FDXY1_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXY2_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDXY3_2D_P3(Real x , Real y)
{
   Real val = -22.5 +54.0*x +27.0*y; 
   return val;
}
Real D2FDXY4_2D_P3(Real x , Real y)
{
   Real val = 4.5 -27.0*x;
   return val;
}
Real D2FDXY5_2D_P3(Real x , Real y)
{
   Real val = -4.5 + 27.0*x;
   return val;
}
Real D2FDXY6_2D_P3(Real x , Real y)
{
   Real val = -4.500000 +27.000000*y; 
   return val;
}
Real D2FDXY7_2D_P3(Real x , Real y)
{
   Real val = -22.5 +27.0*x +54.0*y; 
   return val;
}
Real D2FDXY8_2D_P3(Real x , Real y)
{
   Real val = 4.5 -27.0*y; 
   return val;
}
Real D2FDXY9_2D_P3(Real x , Real y)
{
   Real val = 27.0 -54.0*x -54.0*y; 
   return val;
}

Real D2FDYY0_2D_P3(Real x , Real y)
{
   Real val = 18.0 -27.0*x -27.0*y; 
   return val;
}
Real D2FDYY1_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY2_2D_P3(Real x , Real y)
{
   Real val = -9.0 +27.0*y; 
   return val;
}
Real D2FDYY3_2D_P3(Real x , Real y)
{
   Real val = 27.0*x;
   return val;
}
Real D2FDYY4_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY5_2D_P3(Real x , Real y)
{
   Real val = 0.0;
   return val;
}
Real D2FDYY6_2D_P3(Real x , Real y)
{
   Real val = 27.0*x;
   return val;
}
Real D2FDYY7_2D_P3(Real x , Real y)
{
   Real val = -45.0 +54.0*x +81.0*y; 
   return val;
}
Real D2FDYY8_2D_P3(Real x , Real y)
{
   Real val = 36.0 -27.0*x -81.0*y; 
   return val;
}
Real D2FDYY9_2D_P3(Real x , Real y)
{
   Real val = -54.0*x;
   return val;
}

/*                       ... pour l'element P1b  */

Real D2FDXX0_2D_P1b(Real x, Real y)
{
   Real val = -54.0*y; 
   return val;
}
Real D2FDXY0_2D_P1b(Real x, Real y)
{
   Real val = 27.0 -54.0*x -54.0*y; 
   return val;
}
Real D2FDYY0_2D_P1b(Real x, Real y)
{
   Real val = -54.0*x;
   return val;
}





