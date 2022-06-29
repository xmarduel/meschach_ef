
/* Implements the structure of the finite element in 3 dimensions
 *
 */


#include "MESCHACH/INCLUDES/matrix.h"          /* Librairie Meschach    */
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"     /* for the 3D arrays     */

#include "MESCHACH_EF/INCLUDES/finite_elements.h"


/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt3D_get_P1 (ELT_3D *elt);
static void elt3D_get_P1b(ELT_3D *elt);
static void elt3D_get_P2 (ELT_3D *elt);
static void elt3D_get_P3 (ELT_3D *elt);

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

ELT_3D *elt3D_get(const char *type)
{
   ELT_3D *elt;

   int    i,j;

   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/

   if ( (elt=NEW(ELT_3D)) == (ELT_3D *)NULL )
   {
      error(E_MEM, "ELT_3D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_ELT_3D, 0, sizeof(ELT_3D), MY_LIST2);
      mem_numvar_list(TYPE_ELT_3D, 1, MY_LIST2);
   }

   /* initialize also now the eltDM1 */
   elt->eltDM1 = elt2D_get(type);

   /* and initialize our fe */
   elt->dim     = 3;
   elt->name_ef = strdup(type) ;

   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/

   if (strcmp(elt->name_ef,"P1") == 0)
   {
      elt3D_get_P1(elt);
   }
   else
   if (strcmp(elt->name_ef,"P1b") == 0)
   {
      elt3D_get_P1b(elt);
   }
   else
   if (strcmp(elt->name_ef,"P2") == 0)
   {
      elt3D_get_P2(elt);
   }
   else
   if (strcmp(elt->name_ef,"P3") == 0)
   {
      elt3D_get_P3(elt);
   }
   else
   {
      error2(E_EF_NOTIMPL, "elt3D_get");
   }

   /*---------------------------------------------------------------------------------------------*/
   /*---------------------------------------------------------------------------------------------*/

   /* set frequently used matrices */
   elt->MAT_x_y__plus__y_x = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   elt->MAT_x_z__plus__z_x = m_get(elt->nb_somm_cell,elt->nb_somm_cell);
   elt->MAT_y_z__plus__z_y = m_get(elt->nb_somm_cell,elt->nb_somm_cell);

   for (i=0; i<elt->nb_somm_cell; i++)
   for (j=0; j<elt->nb_somm_cell; j++)
   {
      elt->MAT_x_y__plus__y_x->me[i][j] = elt->MAT_x_y->me[i][j] + elt->MAT_y_x->me[i][j] ;
      elt->MAT_x_z__plus__z_x->me[i][j] = elt->MAT_x_z->me[i][j] + elt->MAT_z_x->me[i][j] ;
      elt->MAT_y_z__plus__z_y->me[i][j] = elt->MAT_y_z->me[i][j] + elt->MAT_z_y->me[i][j] ;
   }


   /* finally the datas on the faces */
   elt->nb_somm_face = elt->eltDM1->nb_somm_cell ;

   /* elt->f_face  pointe vers (elt->nb_somm_face) fonctions de bases */
   elt->f_face       = (Real (**)(Real,Real))calloc(elt->nb_somm_face,sizeof(Real (*)(Real,Real)) );

   for (i=0; i<elt->nb_somm_face; i++)
   {
      elt->f_face[i] = elt->eltDM1->f_base[i];
   }

   return (elt);
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt3D_get_P1 (ELT_3D *elt)
{
   /* Declaration de toutes les matrices pour l' element P1 ... */
   #include "MESCHACH_EF/INCLUDES/ef3D_P1_mtx.h"
   /* Declaration de toutes les matrices pour l' element P1 ... */

   int    i,j,k;

   elt-> nb_somm_cell = 4 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real,Real))calloc(4, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real,Real))calloc(4, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real,Real))calloc(4, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdz_base   = (Real (**)(Real,Real,Real))calloc(4, sizeof(Real (*)(Real,Real,Real)) );

   elt->f_base[0] = F0_3D_P1 ;
   elt->f_base[1] = F1_3D_P1 ;
   elt->f_base[2] = F2_3D_P1 ;
   elt->f_base[3] = F3_3D_P1 ;

   elt->dfdx_base[0] = DFDX0_3D_P1 ;
   elt->dfdx_base[1] = DFDX1_3D_P1 ;
   elt->dfdx_base[2] = DFDX2_3D_P1 ;
   elt->dfdx_base[3] = DFDX3_3D_P1 ;

   elt->dfdy_base[0] = DFDY0_3D_P1 ;
   elt->dfdy_base[1] = DFDY1_3D_P1 ;
   elt->dfdy_base[2] = DFDY2_3D_P1 ;
   elt->dfdy_base[3] = DFDY3_3D_P1 ;

   elt->dfdz_base[0] = DFDZ0_3D_P1 ;
   elt->dfdz_base[1] = DFDZ1_3D_P1 ;
   elt->dfdz_base[2] = DFDZ2_3D_P1 ;
   elt->dfdz_base[3] = DFDZ3_3D_P1 ;


   elt->VEC_Scmbr = v_get(4);
   for (i=0; i<4; i++)
   {
      elt->VEC_Scmbr->ve[i] = VEC_Masse_P1[i];
   }


   elt->MAT_I_I = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P1[i][j];
   }

   elt->MAT_x_x = m_get(4,4);
   elt->MAT_x_y = m_get(4,4);
   elt->MAT_x_z = m_get(4,4);
   elt->MAT_y_x = m_get(4,4);
   elt->MAT_y_y = m_get(4,4);
   elt->MAT_y_z = m_get(4,4);
   elt->MAT_z_x = m_get(4,4);
   elt->MAT_z_y = m_get(4,4);
   elt->MAT_z_z = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P1[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P1[1][i][j];
      elt->MAT_x_z->me[i][j] = MAT_Rigid_P1[2][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P1[3][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P1[4][i][j];
      elt->MAT_y_z->me[i][j] = MAT_Rigid_P1[5][i][j];
      elt->MAT_z_x->me[i][j] = MAT_Rigid_P1[6][i][j];
      elt->MAT_z_y->me[i][j] = MAT_Rigid_P1[7][i][j];
      elt->MAT_z_z->me[i][j] = MAT_Rigid_P1[8][i][j];
   }

   elt->MAT_I_x = m_get(4,4);
   elt->MAT_I_y = m_get(4,4);
   elt->MAT_I_z = m_get(4,4);
   for (i=0; i<4; i++)
   for (j=0; j<4; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P1[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P1[i][j];
      elt->MAT_I_z->me[i][j] = MAT_Conv_z_P1[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(4,4);
   elt->MAT_I_y_PM1dP = m_get(4,4);
   elt->MAT_I_z_PM1dP = m_get(4,4);
   for (i=0; i<4 ; i++)
   for (j=0; j<4 ; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P1dP1[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P1dP1[i][j];
      elt->MAT_I_z_PM1dP->me[i][j] = MAT_Conv_z_P1dP1[i][j];
   }

   elt->TENSOR_I_I_I = ts_get(4,4,4);
   elt->TENSOR_I_x_I = ts_get(4,4,4);
   elt->TENSOR_I_y_I = ts_get(4,4,4);
   elt->TENSOR_I_z_I = ts_get(4,4,4);
   elt->TENSOR_x_I_I = ts_get(4,4,4);
   elt->TENSOR_y_I_I = ts_get(4,4,4);
   elt->TENSOR_z_I_I = ts_get(4,4,4);
   for (i=0; i<4 ; i++)
   for (j=0; j<4 ; j++)
   for (k=0; k<4 ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P1[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P1[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P1[i][j][k];
      elt->TENSOR_I_z_I->te[i][j][k] = TENSOR_ConvZ_P1[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P1[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P1[j][i][k];
      elt->TENSOR_z_I_I->te[i][j][k] = TENSOR_ConvZ_P1[j][i][k];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt3D_get_P1b(ELT_3D *elt)
{
   /* Declaration de toutes les matrices pour l' element P1b ... */
   #include "MESCHACH_EF/INCLUDES/ef3D_P1b_mtx.h"
   /* Declaration de toutes les matrices pour l' element P1b ... */

   int    i,j,k;

   elt-> nb_somm_cell = 5 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real,Real))calloc(5, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real,Real))calloc(5, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real,Real))calloc(5, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdz_base   = (Real (**)(Real,Real,Real))calloc(5, sizeof(Real (*)(Real,Real,Real)) );

   elt->f_base[0] = F0_3D_P1 ;
   elt->f_base[1] = F1_3D_P1 ;
   elt->f_base[2] = F2_3D_P1 ;
   elt->f_base[3] = F3_3D_P1 ;
   elt->f_base[4] = F0_3D_P1b;

   elt->dfdx_base[0] = DFDX0_3D_P1 ;
   elt->dfdx_base[1] = DFDX1_3D_P1 ;
   elt->dfdx_base[2] = DFDX2_3D_P1 ;
   elt->dfdx_base[3] = DFDX3_3D_P1 ;
   elt->dfdx_base[4] = DFDX0_3D_P1b;

   elt->dfdy_base[0] = DFDY0_3D_P1 ;
   elt->dfdy_base[1] = DFDY1_3D_P1 ;
   elt->dfdy_base[2] = DFDY2_3D_P1 ;
   elt->dfdy_base[3] = DFDY3_3D_P1 ;
   elt->dfdy_base[4] = DFDY0_3D_P1b;

   elt->dfdz_base[0] = DFDZ0_3D_P1 ;
   elt->dfdz_base[1] = DFDZ1_3D_P1 ;
   elt->dfdz_base[2] = DFDZ2_3D_P1 ;
   elt->dfdz_base[3] = DFDZ3_3D_P1 ;
   elt->dfdz_base[4] = DFDZ0_3D_P1b;


   elt->VEC_Scmbr = v_get(5);
   for (i=0; i<5; i++)
   {
      elt->VEC_Scmbr->ve[i] = VEC_Masse_P1b[i];
   }

   elt->MAT_I_I = m_get(5,5);
   for (i=0; i<5; i++)
   for (j=0; j<5; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P1b[i][j];
   }

   elt->MAT_x_x = m_get(5,5);
   elt->MAT_x_y = m_get(5,5);
   elt->MAT_x_z = m_get(5,5);
   elt->MAT_y_x = m_get(5,5);
   elt->MAT_y_y = m_get(5,5);
   elt->MAT_y_z = m_get(5,5);
   elt->MAT_z_x = m_get(5,5);
   elt->MAT_z_y = m_get(5,5);
   elt->MAT_z_z = m_get(5,5);
   for (i=0; i<5; i++)
   for (j=0; j<5; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P1b[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P1b[1][i][j];
      elt->MAT_x_z->me[i][j] = MAT_Rigid_P1b[2][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P1b[3][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P1b[4][i][j];
      elt->MAT_y_z->me[i][j] = MAT_Rigid_P1b[5][i][j];
      elt->MAT_z_x->me[i][j] = MAT_Rigid_P1b[6][i][j];
      elt->MAT_z_y->me[i][j] = MAT_Rigid_P1b[7][i][j];
      elt->MAT_z_z->me[i][j] = MAT_Rigid_P1b[8][i][j];
   }

   elt->MAT_I_x = m_get(5,5);
   elt->MAT_I_y = m_get(5,5);
   elt->MAT_I_z = m_get(5,5);
   for (i=0; i<5; i++)
   for (j=0; j<5; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P1b[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P1b[i][j];
      elt->MAT_I_z->me[i][j] = MAT_Conv_z_P1b[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(4,5);
   elt->MAT_I_y_PM1dP = m_get(4,5);
   elt->MAT_I_z_PM1dP = m_get(4,5);
   for (i=0; i<4 ; i++)
   for (j=0; j<5 ; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P1dP1b[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P1dP1b[i][j];
      elt->MAT_I_z_PM1dP->me[i][j] = MAT_Conv_z_P1dP1b[i][j];
   }

   elt->TENSOR_I_I_I = ts_get(5,5,5);
   elt->TENSOR_I_x_I = ts_get(5,5,5);
   elt->TENSOR_I_y_I = ts_get(5,5,5);
   elt->TENSOR_I_z_I = ts_get(5,5,5);
   elt->TENSOR_x_I_I = ts_get(5,5,5);
   elt->TENSOR_y_I_I = ts_get(5,5,5);
   elt->TENSOR_z_I_I = ts_get(5,5,5);
   for (i=0; i<5 ; i++)
   for (j=0; j<5 ; j++)
   for (k=0; k<5 ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P1b[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P1b[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P1b[i][j][k];
      elt->TENSOR_I_z_I->te[i][j][k] = TENSOR_ConvZ_P1b[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P1b[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P1b[j][i][k];
      elt->TENSOR_z_I_I->te[i][j][k] = TENSOR_ConvZ_P1b[j][i][k];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt3D_get_P2 (ELT_3D *elt)
{
   /* Declaration de toutes les matrices pour l' element P2 ... */
   #include "MESCHACH_EF/INCLUDES/ef3D_P2_mtx.h"
   /* Declaration de toutes les matrices pour l' element P2 ... */

   int    i,j,k;

   elt-> nb_somm_cell = 10 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real,Real))calloc(10, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real,Real))calloc(10, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real,Real))calloc(10, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdz_base   = (Real (**)(Real,Real,Real))calloc(10, sizeof(Real (*)(Real,Real,Real)) );

   elt->f_base[0] = F0_3D_P2 ;
   elt->f_base[1] = F1_3D_P2 ;
   elt->f_base[2] = F2_3D_P2 ;
   elt->f_base[3] = F3_3D_P2 ;
   elt->f_base[4] = F4_3D_P2 ;
   elt->f_base[5] = F5_3D_P2 ;
   elt->f_base[6] = F6_3D_P2 ;
   elt->f_base[7] = F7_3D_P2 ;
   elt->f_base[8] = F8_3D_P2 ;
   elt->f_base[9] = F9_3D_P2 ;

   elt->dfdx_base[0] = DFDX0_3D_P2 ;
   elt->dfdx_base[1] = DFDX1_3D_P2 ;
   elt->dfdx_base[2] = DFDX2_3D_P2 ;
   elt->dfdx_base[3] = DFDX3_3D_P2 ;
   elt->dfdx_base[4] = DFDX4_3D_P2 ;
   elt->dfdx_base[5] = DFDX5_3D_P2 ;
   elt->dfdx_base[6] = DFDX6_3D_P2 ;
   elt->dfdx_base[7] = DFDX7_3D_P2 ;
   elt->dfdx_base[8] = DFDX8_3D_P2 ;
   elt->dfdx_base[9] = DFDX9_3D_P2 ;

   elt->dfdy_base[0] = DFDY0_3D_P2 ;
   elt->dfdy_base[1] = DFDY1_3D_P2 ;
   elt->dfdy_base[2] = DFDY2_3D_P2 ;
   elt->dfdy_base[3] = DFDY3_3D_P2 ;
   elt->dfdy_base[4] = DFDY4_3D_P2 ;
   elt->dfdy_base[5] = DFDY5_3D_P2 ;
   elt->dfdy_base[6] = DFDY6_3D_P2 ;
   elt->dfdy_base[7] = DFDY7_3D_P2 ;
   elt->dfdy_base[8] = DFDY8_3D_P2 ;
   elt->dfdy_base[9] = DFDY9_3D_P2 ;

   elt->dfdz_base[0] = DFDZ0_3D_P2 ;
   elt->dfdz_base[1] = DFDZ1_3D_P2 ;
   elt->dfdz_base[2] = DFDZ2_3D_P2 ;
   elt->dfdz_base[3] = DFDZ3_3D_P2 ;
   elt->dfdz_base[4] = DFDZ4_3D_P2 ;
   elt->dfdz_base[5] = DFDZ5_3D_P2 ;
   elt->dfdz_base[6] = DFDZ6_3D_P2 ;
   elt->dfdz_base[7] = DFDZ7_3D_P2 ;
   elt->dfdz_base[8] = DFDZ8_3D_P2 ;
   elt->dfdz_base[9] = DFDZ9_3D_P2 ;


   elt->VEC_Scmbr = v_get(10);
   for (i=0; i<10; i++)
   {
      elt->VEC_Scmbr->ve[i] = VEC_Masse_P2[i];
   }

   elt->MAT_I_I = m_get(10,10);
   for (i=0; i<10; i++)
   for (j=0; j<10; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P2[i][j];
   }

   elt->MAT_x_x = m_get(10,10);
   elt->MAT_x_y = m_get(10,10);
   elt->MAT_x_z = m_get(10,10);
   elt->MAT_y_x = m_get(10,10);
   elt->MAT_y_y = m_get(10,10);
   elt->MAT_y_z = m_get(10,10);
   elt->MAT_z_x = m_get(10,10);
   elt->MAT_z_y = m_get(10,10);
   elt->MAT_z_z = m_get(10,10);
   for (i=0; i<10; i++)
   for (j=0; j<10; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P2[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P2[1][i][j];
      elt->MAT_x_z->me[i][j] = MAT_Rigid_P2[2][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P2[3][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P2[4][i][j];
      elt->MAT_y_z->me[i][j] = MAT_Rigid_P2[5][i][j];
      elt->MAT_z_x->me[i][j] = MAT_Rigid_P2[6][i][j];
      elt->MAT_z_y->me[i][j] = MAT_Rigid_P2[7][i][j];
      elt->MAT_z_z->me[i][j] = MAT_Rigid_P2[8][i][j];
   }

   elt->MAT_I_x = m_get(10,10);
   elt->MAT_I_y = m_get(10,10);
   elt->MAT_I_z = m_get(10,10);
   for (i=0; i<10; i++)
   for (j=0; j<10; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P2[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P2[i][j];
      elt->MAT_I_z->me[i][j] = MAT_Conv_z_P2[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(4,10);
   elt->MAT_I_y_PM1dP = m_get(4,10);
   elt->MAT_I_z_PM1dP = m_get(4,10);
   for (i=0; i<4 ; i++)
   for (j=0; j<10; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P1dP2[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P1dP2[i][j];
      elt->MAT_I_z_PM1dP->me[i][j] = MAT_Conv_z_P1dP2[i][j];
   }


   elt->TENSOR_I_I_I = ts_get(10,10,10);
   elt->TENSOR_I_x_I = ts_get(10,10,10);
   elt->TENSOR_I_y_I = ts_get(10,10,10);
   elt->TENSOR_I_z_I = ts_get(10,10,10);
   elt->TENSOR_x_I_I = ts_get(10,10,10);
   elt->TENSOR_y_I_I = ts_get(10,10,10);
   elt->TENSOR_z_I_I = ts_get(10,10,10);
   for (i=0; i<10 ; i++)
   for (j=0; j<10 ; j++)
   for (k=0; k<10 ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P2[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P2[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P2[i][j][k];
      elt->TENSOR_I_z_I->te[i][j][k] = TENSOR_ConvZ_P2[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P2[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P2[j][i][k];
      elt->TENSOR_z_I_I->te[i][j][k] = TENSOR_ConvZ_P2[j][i][k];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

static void elt3D_get_P3 (ELT_3D *elt)
{
   /* Declaration de toutes les matrices pour l' element P3 ... */
   #include "MESCHACH_EF/INCLUDES/ef3D_P3_mtx.h"
   /* Declaration de toutes les matrices pour l' element P3 ... */

   int    i,j,k;

   elt-> nb_somm_cell = 20 ;

   /* elt->f_base  pointe vers (elt->nb_somm_cell) fonctions de bases */
   elt->f_base      = (Real (**)(Real,Real,Real))calloc(20, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdx_base   = (Real (**)(Real,Real,Real))calloc(20, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdy_base   = (Real (**)(Real,Real,Real))calloc(20, sizeof(Real (*)(Real,Real,Real)) );
   elt->dfdz_base   = (Real (**)(Real,Real,Real))calloc(20, sizeof(Real (*)(Real,Real,Real)) );

   elt->f_base[0] = F0_3D_P3 ;
   elt->f_base[1] = F1_3D_P3 ;
   elt->f_base[2] = F2_3D_P3 ;
   elt->f_base[3] = F3_3D_P3 ;
   elt->f_base[4] = F4_3D_P3 ;
   elt->f_base[5] = F5_3D_P3 ;
   elt->f_base[6] = F6_3D_P3 ;
   elt->f_base[7] = F7_3D_P3 ;
   elt->f_base[8] = F8_3D_P3 ;
   elt->f_base[9] = F9_3D_P3 ;
   elt->f_base[10] = F10_3D_P3 ;
   elt->f_base[11] = F11_3D_P3 ;
   elt->f_base[12] = F12_3D_P3 ;
   elt->f_base[13] = F13_3D_P3 ;
   elt->f_base[14] = F14_3D_P3 ;
   elt->f_base[15] = F15_3D_P3 ;
   elt->f_base[16] = F16_3D_P3 ;
   elt->f_base[17] = F17_3D_P3 ;
   elt->f_base[18] = F18_3D_P3 ;
   elt->f_base[19] = F19_3D_P3 ;

   elt->dfdx_base[0] = DFDX0_3D_P3 ;
   elt->dfdx_base[1] = DFDX1_3D_P3 ;
   elt->dfdx_base[2] = DFDX2_3D_P3 ;
   elt->dfdx_base[3] = DFDX3_3D_P3 ;
   elt->dfdx_base[4] = DFDX4_3D_P3 ;
   elt->dfdx_base[5] = DFDX5_3D_P3 ;
   elt->dfdx_base[6] = DFDX6_3D_P3 ;
   elt->dfdx_base[7] = DFDX7_3D_P3 ;
   elt->dfdx_base[8] = DFDX8_3D_P3 ;
   elt->dfdx_base[9] = DFDX9_3D_P3 ;
   elt->dfdx_base[10] = DFDX10_3D_P3 ;
   elt->dfdx_base[11] = DFDX11_3D_P3 ;
   elt->dfdx_base[12] = DFDX12_3D_P3 ;
   elt->dfdx_base[13] = DFDX13_3D_P3 ;
   elt->dfdx_base[14] = DFDX14_3D_P3 ;
   elt->dfdx_base[15] = DFDX15_3D_P3 ;
   elt->dfdx_base[16] = DFDX16_3D_P3 ;
   elt->dfdx_base[17] = DFDX17_3D_P3 ;
   elt->dfdx_base[18] = DFDX18_3D_P3 ;
   elt->dfdx_base[19] = DFDX19_3D_P3 ;

   elt->dfdy_base[0] = DFDY0_3D_P3 ;
   elt->dfdy_base[1] = DFDY1_3D_P3 ;
   elt->dfdy_base[2] = DFDY2_3D_P3 ;
   elt->dfdy_base[3] = DFDY3_3D_P3 ;
   elt->dfdy_base[4] = DFDY4_3D_P3 ;
   elt->dfdy_base[5] = DFDY5_3D_P3 ;
   elt->dfdy_base[6] = DFDY6_3D_P3 ;
   elt->dfdy_base[7] = DFDY7_3D_P3 ;
   elt->dfdy_base[8] = DFDY8_3D_P3 ;
   elt->dfdy_base[9] = DFDY9_3D_P3 ;
   elt->dfdy_base[10] = DFDY10_3D_P3 ;
   elt->dfdy_base[11] = DFDY11_3D_P3 ;
   elt->dfdy_base[12] = DFDY12_3D_P3 ;
   elt->dfdy_base[13] = DFDY13_3D_P3 ;
   elt->dfdy_base[14] = DFDY14_3D_P3 ;
   elt->dfdy_base[15] = DFDY15_3D_P3 ;
   elt->dfdy_base[16] = DFDY16_3D_P3 ;
   elt->dfdy_base[17] = DFDY17_3D_P3 ;
   elt->dfdy_base[18] = DFDY18_3D_P3 ;
   elt->dfdy_base[19] = DFDY19_3D_P3 ;

   elt->dfdz_base[0] = DFDZ0_3D_P3 ;
   elt->dfdz_base[1] = DFDZ1_3D_P3 ;
   elt->dfdz_base[2] = DFDZ2_3D_P3 ;
   elt->dfdz_base[3] = DFDZ3_3D_P3 ;
   elt->dfdz_base[4] = DFDZ4_3D_P3 ;
   elt->dfdz_base[5] = DFDZ5_3D_P3 ;
   elt->dfdz_base[6] = DFDZ6_3D_P3 ;
   elt->dfdz_base[7] = DFDZ7_3D_P3 ;
   elt->dfdz_base[8] = DFDZ8_3D_P3 ;
   elt->dfdz_base[9] = DFDZ9_3D_P3 ;
   elt->dfdz_base[10] = DFDZ10_3D_P3 ;
   elt->dfdz_base[11] = DFDZ11_3D_P3 ;
   elt->dfdz_base[12] = DFDZ12_3D_P3 ;
   elt->dfdz_base[13] = DFDZ13_3D_P3 ;
   elt->dfdz_base[14] = DFDZ14_3D_P3 ;
   elt->dfdz_base[15] = DFDZ15_3D_P3 ;
   elt->dfdz_base[16] = DFDZ16_3D_P3 ;
   elt->dfdz_base[17] = DFDZ17_3D_P3 ;
   elt->dfdz_base[18] = DFDZ18_3D_P3 ;
   elt->dfdz_base[19] = DFDZ19_3D_P3 ;


   elt->VEC_Scmbr = v_get(20);
   for (i=0; i<20; i++)
   {
      elt->VEC_Scmbr->ve[i] = VEC_Masse_P3[i];
   }

   elt->MAT_I_I = m_get(20,20);
   for (i=0; i<20; i++)
   for (j=0; j<20; j++)
   {
      elt->MAT_I_I->me[i][j] = MAT_Masse_P3[i][j];
   }

   elt->MAT_x_x = m_get(20,20);
   elt->MAT_x_y = m_get(20,20);
   elt->MAT_x_z = m_get(20,20);
   elt->MAT_y_x = m_get(20,20);
   elt->MAT_y_y = m_get(20,20);
   elt->MAT_y_z = m_get(20,20);
   elt->MAT_z_x = m_get(20,20);
   elt->MAT_z_y = m_get(20,20);
   elt->MAT_z_z = m_get(20,20);
   for (i=0; i<20; i++)
   for (j=0; j<20; j++)
   {
      elt->MAT_x_x->me[i][j] = MAT_Rigid_P3[0][i][j];
      elt->MAT_x_y->me[i][j] = MAT_Rigid_P3[1][i][j];
      elt->MAT_x_z->me[i][j] = MAT_Rigid_P3[2][i][j];
      elt->MAT_y_x->me[i][j] = MAT_Rigid_P3[3][i][j];
      elt->MAT_y_y->me[i][j] = MAT_Rigid_P3[4][i][j];
      elt->MAT_y_z->me[i][j] = MAT_Rigid_P3[5][i][j];
      elt->MAT_z_x->me[i][j] = MAT_Rigid_P3[6][i][j];
      elt->MAT_z_y->me[i][j] = MAT_Rigid_P3[7][i][j];
      elt->MAT_z_z->me[i][j] = MAT_Rigid_P3[8][i][j];
   }

   elt->MAT_I_x = m_get(20,20);
   elt->MAT_I_y = m_get(20,20);
   elt->MAT_I_z = m_get(20,20);
   for (i=0; i<20; i++)
   for (j=0; j<20; j++)
   {
      elt->MAT_I_x->me[i][j] = MAT_Conv_x_P3[i][j];
      elt->MAT_I_y->me[i][j] = MAT_Conv_y_P3[i][j];
      elt->MAT_I_z->me[i][j] = MAT_Conv_z_P3[i][j];
   }

   elt->MAT_I_x_PM1dP = m_get(10,20);
   elt->MAT_I_y_PM1dP = m_get(10,20);
   elt->MAT_I_z_PM1dP = m_get(10,20);
   for (i=0; i<10; i++)
   for (j=0; j<20; j++)
   {
      elt->MAT_I_x_PM1dP->me[i][j] = MAT_Conv_x_P2dP3[i][j];
      elt->MAT_I_y_PM1dP->me[i][j] = MAT_Conv_y_P2dP3[i][j];
      elt->MAT_I_z_PM1dP->me[i][j] = MAT_Conv_z_P2dP3[i][j];
   }


   elt->TENSOR_I_I_I = ts_get(20,20,20);
   elt->TENSOR_I_x_I = ts_get(20,20,20);
   elt->TENSOR_I_y_I = ts_get(20,20,20);
   elt->TENSOR_I_z_I = ts_get(20,20,20);
   elt->TENSOR_x_I_I = ts_get(20,20,20);
   elt->TENSOR_y_I_I = ts_get(20,20,20);
   elt->TENSOR_z_I_I = ts_get(20,20,20);
   for (i=0; i<20 ; i++)
   for (j=0; j<20 ; j++)
   for (k=0; k<20 ; k++)
   {
      elt->TENSOR_I_I_I->te[i][j][k] = TENSOR_Mass_P3[i][j][k];

      elt->TENSOR_I_x_I->te[i][j][k] = TENSOR_ConvX_P3[i][j][k];
      elt->TENSOR_I_y_I->te[i][j][k] = TENSOR_ConvY_P3[i][j][k];
      elt->TENSOR_I_z_I->te[i][j][k] = TENSOR_ConvZ_P3[i][j][k];

      elt->TENSOR_x_I_I->te[i][j][k] = TENSOR_ConvX_P3[j][i][k];
      elt->TENSOR_y_I_I->te[i][j][k] = TENSOR_ConvY_P3[j][i][k];
      elt->TENSOR_z_I_I->te[i][j][k] = TENSOR_ConvZ_P3[j][i][k];
   }
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

int elt3D_free(ELT_3D *elt)
{
   if ( elt == (ELT_3D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_ELT_3D, sizeof(ELT_3D), 0, MY_LIST2);
         mem_numvar_list(TYPE_ELT_3D, -1, MY_LIST2);
      }

      free(elt->name_ef);

      V_FREE(elt->VEC_Scmbr);

      M_FREE(elt->MAT_I_I);

      M_FREE(elt->MAT_x_x);
      M_FREE(elt->MAT_x_y);
      M_FREE(elt->MAT_x_z);
      M_FREE(elt->MAT_y_x);
      M_FREE(elt->MAT_y_y);
      M_FREE(elt->MAT_y_z);
      M_FREE(elt->MAT_z_x);
      M_FREE(elt->MAT_z_y);
      M_FREE(elt->MAT_z_z);

      M_FREE(elt->MAT_x_y__plus__y_x);
      M_FREE(elt->MAT_x_z__plus__z_x);
      M_FREE(elt->MAT_y_z__plus__z_y);

      M_FREE(elt->MAT_I_x);
      M_FREE(elt->MAT_I_y);
      M_FREE(elt->MAT_I_z);

      M_FREE(elt->MAT_I_x_PM1dP);
      M_FREE(elt->MAT_I_y_PM1dP);
      M_FREE(elt->MAT_I_z_PM1dP);

      TS_FREE(elt->TENSOR_I_I_I);
      TS_FREE(elt->TENSOR_I_x_I);
      TS_FREE(elt->TENSOR_I_y_I);
      TS_FREE(elt->TENSOR_I_z_I);
      TS_FREE(elt->TENSOR_x_I_I);
      TS_FREE(elt->TENSOR_y_I_I);
      TS_FREE(elt->TENSOR_z_I_I);

      ELT_2D_FREE(elt->eltDM1);

      free(elt->f_base);
      free(elt->dfdx_base);
      free(elt->dfdy_base);
      free(elt->dfdz_base);

      free(elt->f_face);

      /* free the space of the element */
      free(elt);

      /* finito */
      return EXIT_SUCCESS;
   }

}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

/*  Fonctions de base ** 3D ** ---------------------- */


Real F0_3D_P0(Real x , Real y , Real z)   /* -- Element P0 --- */
{
   Real val;
   return val = 1.0;
}


Real F0_3D_P1(Real x , Real y , Real z)   /* -- Element P1 --- */
{
   Real val = 1.0 - x - y - z;
   return val;
}
Real F1_3D_P1(Real x , Real y , Real z)
{
   Real val = x;
   return val;
}
Real F2_3D_P1(Real x , Real y , Real z)
{
   Real val = y;
   return val;
}
Real F3_3D_P1(Real x , Real y , Real z)
{
   Real val = z;
   return val;
}


Real F0_3D_P2(Real x , Real y , Real z)   /* -- Element P2 --- */
{
   Real val;
   return val =  1.0 - 3.0*(x + y + z) + 4.0*(x*y + x*z + y*z) + 2.0*(x*x + y*y + z*z);
}
Real F1_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -x + 2.0*x*x ;
}
Real F2_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  -y + 2.0*y*y ;
}
Real F3_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  -z + 2.0*z*z ;
}
Real F4_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*(x - x*x - x*y - x*z ) ;
}
Real F5_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*(y - y*y - y*x - y*z ) ;
}
Real F6_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*(z - z*z - z*x - z*y ) ;
}
Real F7_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*x*y ;
}
Real F8_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*y*z ;
}
Real F9_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*x*z ;
}


Real F0_3D_P1b(Real x , Real y , Real z)   /* -- Element P1b -bubble function- -- */
{
   Real val;
   return val = F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z);
}



/*  Derivees des Fonctions de Base P1---------------- */

Real DFDX0_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = -1.0;
}
Real DFDX1_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 1.0;
}
Real DFDX2_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0;
}
Real DFDX3_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0;
}

Real DFDY0_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = -1.0;
}
Real DFDY1_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0;
}
Real DFDY2_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 1.0;
}
Real DFDY3_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0;
}

Real DFDZ0_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = -1.0;
}
Real DFDZ1_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0;
}
Real DFDZ2_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0;
}
Real DFDZ3_3D_P1(Real x , Real y , Real z)
{
   Real val;
   return val = 1.0;
}


/*  Derivees des Fonctions de Base P2---------------- */

Real DFDX0_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*(x + y + z) - 3.0 ;
}
Real DFDX1_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*x - 1.0 ;
}
Real DFDX2_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  0.0;
}
Real DFDX3_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  0.0 ;
}
Real DFDX4_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*(1.0 - 2.0*x - y - z ) ;
}
Real DFDX5_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -4.0*y ;
}
Real DFDX6_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -4.0*z ;
}
Real DFDX7_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*y ;
}
Real DFDX8_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0 ;
}
Real DFDX9_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*z ;
}


Real DFDY0_3D_P2(Real x , Real y , Real z)   /* -- Element P2 --- */
{
   Real val;
   return val = 4.0*(x + y + z) - 3.0 ;
}
Real DFDY1_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  0.0;
}
Real DFDY2_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*y - 1.0 ;
}
Real DFDY3_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  0.0 ;
}
Real DFDY4_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -4.0*x ;
}
Real DFDY5_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*(1.0  - 2.0*y - x - z ) ;
}
Real DFDY6_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -4.0*z ;
}
Real DFDY7_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*x ;
}
Real DFDY8_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*z ;
}
Real DFDY9_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0 ;
}



Real DFDZ0_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  4.0*(x + y + z) - 3.0;
}
Real DFDZ1_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  0.0 ;
}
Real DFDZ2_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0 ;
}
Real DFDZ3_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*z - 1.0 ;
}
Real DFDZ4_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -4.0*x ;
}
Real DFDZ5_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = -4.0*y ;
}
Real DFDZ6_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val =  4.0*(1.0 - x - y - 2.0*z);
}
Real DFDZ7_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 0.0 ;
}
Real DFDZ8_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*y ;
}
Real DFDZ9_3D_P2(Real x , Real y , Real z)
{
   Real val;
   return val = 4.0*x ;
}


Real DFDX0_3D_P1b(Real x , Real y , Real z)   /* -- Element P1b -bubble function- -- */
{
   Real val;
   return val = DFDX0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDX1_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDX2_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDX3_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z) ;
}
Real DFDY0_3D_P1b(Real x , Real y , Real z)   /* -- Element P1b -bubble function- -- */
{
   Real val;
   return val = DFDY0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDY1_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDY2_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDY3_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z) ;
}
Real DFDZ0_3D_P1b(Real x , Real y , Real z)   /* -- Element P1b -bubble function- -- */
{
   Real val;
   return val = DFDZ0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDZ1_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F2_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDZ2_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F3_3D_P1(x,y,z) +
                DFDZ3_3D_P1(x,y,z)*F0_3D_P1(x,y,z)*F1_3D_P1(x,y,z)*F2_3D_P1(x,y,z) ;
}

/* ------------------------------------------------------- */

Real F0_3D_P3(Real x, Real y, Real z)
{
    return 1 + 9*pow(x,2) + 9*pow(y,2) + 9*pow(z,2) - 11/2.0*x - 11/2.0*y - 11/2.0*z - 9/2.0*pow(x,3) - 9/2.0*pow(y,3) - 9/2.0*pow(z,3) + 18*x*y + 18*x*z + 18*y*z - 27/2.0*x*pow(y,2) - 27/2.0*x*pow(z,2) - 27/2.0*y*pow(x,2) - 27/2.0*y*pow(z,2) - 27/2.0*z*pow(x,2) - 27/2.0*z*pow(y,2) - 27*x*y*z;
}

Real F1_3D_P3(Real x, Real y, Real z)
{
    return x - 9/2.0*pow(x,2) + (9/2.0)*pow(x,3);
}

Real F2_3D_P3(Real x, Real y, Real z)
{
    return y - 9/2.0*pow(y,2) + (9/2.0)*pow(y,3);
}

Real F3_3D_P3(Real x, Real y, Real z)
{
    return z - 9/2.0*pow(z,2) + (9/2.0)*pow(z,3);
}

Real F4_3D_P3(Real x, Real y, Real z)
{
    return 9*x - 45/2.0*pow(x,2) + (27/2.0)*pow(x,3) + 27*y*pow(x,2) + 27*z*pow(x,2) - 45/2.0*x*y - 45/2.0*x*z + (27/2.0)*x*pow(y,2) + (27/2.0)*x*pow(z,2) + 27*x*y*z;
}

Real F5_3D_P3(Real x, Real y, Real z)
{
    return 9*y - 45/2.0*pow(y,2) + (27/2.0)*pow(y,3) + 27*x*pow(y,2) + 27*z*pow(y,2) - 45/2.0*x*y - 45/2.0*y*z + (27/2.0)*y*pow(x,2) + (27/2.0)*y*pow(z,2) + 27*x*y*z;
}

Real F6_3D_P3(Real x, Real y, Real z)
{
    return 9*z - 45/2.0*pow(z,2) + (27/2.0)*pow(z,3) + 27*x*pow(z,2) + 27*y*pow(z,2) - 45/2.0*x*z - 45/2.0*y*z + (27/2.0)*z*pow(x,2) + (27/2.0)*z*pow(y,2) + 27*x*y*z;
}

Real F7_3D_P3(Real x, Real y, Real z)
{
    return 18*pow(x,2) - 27/2.0*pow(x,3) - 9/2.0*x - 27/2.0*y*pow(x,2) - 27/2.0*z*pow(x,2) + (9/2.0)*x*y + (9/2.0)*x*z;
}

Real F8_3D_P3(Real x, Real y, Real z)
{
    return 18*pow(y,2) - 27/2.0*pow(y,3) - 9/2.0*y - 27/2.0*x*pow(y,2) - 27/2.0*z*pow(y,2) + (9/2.0)*x*y + (9/2.0)*y*z;
}

Real F9_3D_P3(Real x, Real y, Real z)
{
    return 18*pow(z,2) - 27/2.0*pow(z,3) - 9/2.0*z - 27/2.0*x*pow(z,2) - 27/2.0*y*pow(z,2) + (9/2.0)*x*z + (9/2.0)*y*z;
}

Real F10_3D_P3(Real x, Real y, Real z)
{
    return -27*y*pow(z,2) - 27*z*pow(y,2) + 27*y*z - 27*x*y*z;
}

Real F11_3D_P3(Real x, Real y, Real z)
{
    return -27*x*pow(z,2) - 27*z*pow(x,2) + 27*x*z - 27*x*y*z;
}

Real F12_3D_P3(Real x, Real y, Real z)
{
    return -27*x*pow(y,2) - 27*y*pow(x,2) + 27*x*y - 27*x*y*z;
}

Real F13_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*y*z + (27/2.0)*y*pow(z,2);
}

Real F14_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*y*z + (27/2.0)*z*pow(y,2);
}

Real F15_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x*z + (27/2.0)*x*pow(z,2);
}

Real F16_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x*z + (27/2.0)*z*pow(x,2);
}

Real F17_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x*y + (27/2.0)*x*pow(y,2);
}

Real F18_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x*y + (27/2.0)*y*pow(x,2);
}

Real F19_3D_P3(Real x, Real y, Real z)
{
    return 27*x*y*z;
}

/*--------------------------------------------------------*/
/*--------------------------------------------------------*/

Real DFDX0_3D_P3(Real x, Real y, Real z)
{
    return (-11/2.0) + 18*x + 18*y + 18*z - 27/2.0*pow(x,2) - 27/2.0*pow(y,2) - 27/2.0*pow(z,2) - 27*x*y - 27*x*z - 27*y*z;
}

Real DFDX1_3D_P3(Real x, Real y, Real z)
{
    return 1 - 9*x + (27/2.0)*pow(x,2);
}

Real DFDX2_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDX3_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDX4_3D_P3(Real x, Real y, Real z)
{
    return 9 - 45*x - 45/2.0*y - 45/2.0*z + (27/2.0)*pow(y,2) + (27/2.0)*pow(z,2) + (81/2.0)*pow(x,2) + 27*y*z + 54*x*y + 54*x*z;
}

Real DFDX5_3D_P3(Real x, Real y, Real z)
{
    return 27*pow(y,2) - 45/2.0*y + 27*x*y + 27*y*z;
}

Real DFDX6_3D_P3(Real x, Real y, Real z)
{
    return 27*pow(z,2) - 45/2.0*z + 27*x*z + 27*y*z;
}

Real DFDX7_3D_P3(Real x, Real y, Real z)
{
    return (-9/2.0) + 36*x - 81/2.0*pow(x,2) + (9/2.0)*y + (9/2.0)*z - 27*x*y - 27*x*z;
}

Real DFDX8_3D_P3(Real x, Real y, Real z)
{
    return -27/2.0*pow(y,2) + (9/2.0)*y;
}

Real DFDX9_3D_P3(Real x, Real y, Real z)
{
    return -27/2.0*pow(z,2) + (9/2.0)*z;
}

Real DFDX10_3D_P3(Real x, Real y, Real z)
{
    return -27*y*z;
}

Real DFDX11_3D_P3(Real x, Real y, Real z)
{
    return -27*pow(z,2) + 27*z - 54*x*z - 27*y*z;
}

Real DFDX12_3D_P3(Real x, Real y, Real z)
{
    return -27*pow(y,2) + 27*y - 54*x*y - 27*y*z;
}

Real DFDX13_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDX14_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDX15_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*z + (27/2.0)*pow(z,2);
}

Real DFDX16_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*z + 27*x*z;
}

Real DFDX17_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*y + (27/2.0)*pow(y,2);
}

Real DFDX18_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*y + 27*x*y;
}

Real DFDX19_3D_P3(Real x, Real y, Real z)
{
    return 27*y*z;
}

/*--------------------------------------------------------*/
/*--------------------------------------------------------*/

Real DFDY0_3D_P3(Real x, Real y, Real z)
{
    return (-11/2.0) + 18*x + 18*y + 18*z - 27/2.0*pow(x,2) - 27/2.0*pow(y,2) - 27/2.0*pow(z,2) - 27*x*y - 27*x*z - 27*y*z;
}

Real DFDY1_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDY2_3D_P3(Real x, Real y, Real z)
{
    return 1 - 9*y + (27/2.0)*pow(y,2);
}

Real DFDY3_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDY4_3D_P3(Real x, Real y, Real z)
{
    return 27*pow(x,2) - 45/2.0*x + 27*x*y + 27*x*z;
}

Real DFDY5_3D_P3(Real x, Real y, Real z)
{
    return 9 - 45*y - 45/2.0*x - 45/2.0*z + (27/2.0)*pow(x,2) + (27/2.0)*pow(z,2) + (81/2.0)*pow(y,2) + 27*x*z + 54*x*y + 54*y*z;
}

Real DFDY6_3D_P3(Real x, Real y, Real z)
{
    return 27*pow(z,2) - 45/2.0*z + 27*x*z + 27*y*z;
}

Real DFDY7_3D_P3(Real x, Real y, Real z)
{
    return -27/2.0*pow(x,2) + (9/2.0)*x;
}

Real DFDY8_3D_P3(Real x, Real y, Real z)
{
    return (-9/2.0) + 36*y - 81/2.0*pow(y,2) + (9/2.0)*x + (9/2.0)*z - 27*x*y - 27*y*z;
}

Real DFDY9_3D_P3(Real x, Real y, Real z)
{
    return -27/2.0*pow(z,2) + (9/2.0)*z;
}

Real DFDY10_3D_P3(Real x, Real y, Real z)
{
    return -27*pow(z,2) + 27*z - 54*y*z - 27*x*z;
}

Real DFDY11_3D_P3(Real x, Real y, Real z)
{
    return -27*x*z;
}

Real DFDY12_3D_P3(Real x, Real y, Real z)
{
    return -27*pow(x,2) + 27*x - 54*x*y - 27*x*z;
}

Real DFDY13_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*z + (27/2.0)*pow(z,2);
}

Real DFDY14_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*z + 27*y*z;
}

Real DFDY15_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDY16_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDY17_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x + 27*x*y;
}

Real DFDY18_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x + (27/2.0)*pow(x,2);
}

Real DFDY19_3D_P3(Real x, Real y, Real z)
{
    return 27*x*z;
}

/*--------------------------------------------------------*/
/*--------------------------------------------------------*/

Real DFDZ0_3D_P3(Real x, Real y, Real z)
{
    return (-11/2.0) + 18*x + 18*y + 18*z - 27/2.0*pow(x,2) - 27/2.0*pow(y,2) - 27/2.0*pow(z,2) - 27*x*y - 27*x*z - 27*y*z;
}

Real DFDZ1_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDZ2_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDZ3_3D_P3(Real x, Real y, Real z)
{
    return 1 - 9*z + (27/2.0)*pow(z,2);
}

Real DFDZ4_3D_P3(Real x, Real y, Real z)
{
    return 27*pow(x,2) - 45/2.0*x + 27*x*y + 27*x*z;
}

Real DFDZ5_3D_P3(Real x, Real y, Real z)
{
    return 27*pow(y,2) - 45/2.0*y + 27*x*y + 27*y*z;
}

Real DFDZ6_3D_P3(Real x, Real y, Real z)
{
    return 9 - 45*z - 45/2.0*x - 45/2.0*y + (27/2.0)*pow(x,2) + (27/2.0)*pow(y,2) + (81/2.0)*pow(z,2) + 27*x*y + 54*x*z + 54*y*z;
}

Real DFDZ7_3D_P3(Real x, Real y, Real z)
{
    return -27/2.0*pow(x,2) + (9/2.0)*x;
}

Real DFDZ8_3D_P3(Real x, Real y, Real z)
{
    return -27/2.0*pow(y,2) + (9/2.0)*y;
}

Real DFDZ9_3D_P3(Real x, Real y, Real z)
{
    return (-9/2.0) + 36*z - 81/2.0*pow(z,2) + (9/2.0)*x + (9/2.0)*y - 27*x*z - 27*y*z;
}

Real DFDZ10_3D_P3(Real x, Real y, Real z)
{
    return -27*pow(y,2) + 27*y - 54*y*z - 27*x*y;
}

Real DFDZ11_3D_P3(Real x, Real y, Real z)
{
    return -27*pow(x,2) + 27*x - 54*x*z - 27*x*y;
}

Real DFDZ12_3D_P3(Real x, Real y, Real z)
{
    return -27*x*y;
}

Real DFDZ13_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*y + 27*y*z;
}

Real DFDZ14_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*y + (27/2.0)*pow(y,2);
}

Real DFDZ15_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x + 27*x*z;
}

Real DFDZ16_3D_P3(Real x, Real y, Real z)
{
    return -9/2.0*x + (27/2.0)*pow(x,2);
}

Real DFDZ17_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDZ18_3D_P3(Real x, Real y, Real z)
{
    return 0;
}

Real DFDZ19_3D_P3(Real x, Real y, Real z)
{
    return 27*x*y;
}




