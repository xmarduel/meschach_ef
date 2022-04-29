#ifndef MEF_FINITE_ELEMENTS_2D_H
#define MEF_FINITE_ELEMENTS_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file finite_elements_2D.h
 *
 * Contains the structure of the finite element in 2 dimensions
 *
 * Usage : INITIALIZE AN ELT_1D STRUCTURE WITH :
 *
 *    ELT_2D *elt = elt2D_get("P1"); \n
 *    ELT_2D *elt = elt2D_get("P2"); \n
 *    ELT_2D *elt = elt2D_get("P3"); \n
 *    ELT_2D *elt = elt2D_get("P1b"); \n
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"


typedef struct {

   int     dim;                        /**< dimension; here dim = 2               */
   char*   name_ef;                    /**< nom de l'element : P1 , P2 , P3 , P1b */
   
   int     nb_somm_cell ;              /**< nb de fonctions de base de l'element  */
   Real    (**f_base)(Real,Real);      /**< fonctions de base de l'element        */
   Real    (**dfdx_base)(Real,Real);   /**< derivees des fonctions de base        */
   Real    (**dfdy_base)(Real,Real);   /**< derivees des fonctions de base        */
   Real    (**d2fdxx_base)(Real,Real); /**< derivees des fonctions de base        */
   Real    (**d2fdxy_base)(Real,Real); /**< derivees des fonctions de base        */
   Real    (**d2fdyy_base)(Real,Real); /**< derivees des fonctions de base        */
   int     nb_somm_face ;              /**< nb de fonctions de base de l'arete    */
   Real    (**f_face)(Real);           /**< fonctions de base sur les aretes      */
   
   VEC*    VEC_I;                /**< rhs (Source cste)   Ni            */
   VEC*    VEC_x;                /**< rhs (Source cste)   dNidx  for the "stabilized" rhs    */
   VEC*    VEC_y;                /**< rhs (Source cste)   dNidy  for the "stabilized" rhs    */
   VEC*    VEC_xx;               /**< rhs (Source cste)   d2Ni   for the "stabilized" rhs    */
   VEC*    VEC_xy;               /**< rhs (Source cste)   d2Ni   for the "stabilized" rhs    */
   VEC*    VEC_yy;               /**< rhs (Source cste)   d2Ni   for the "stabilized" rhs    */
   
   MAT*    MAT_I_I;              /**< The mass matrix Ni.Nj             */

   MAT*    MAT_x_x;              /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_x_y;              /**< The Stiffness matrix dNidx.dNjdy  */
   MAT*    MAT_y_x;              /**< The Stiffness matrix dNidy.dNjdx  */
   MAT*    MAT_y_y;              /**< The Stiffness matrix dNidy.dNjdy  */
   
   MAT*    MAT_I_x;              /**< The Convection matrix Ni.dNjdx    */
   MAT*    MAT_I_y;              /**< The Convection matrix Ni.dNjdx    */
   
   MAT*    MAT_xx_xx;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_xx_xy;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_xx_yy;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_xy_xx;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_xy_xy;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_xy_yy;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_yy_xx;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_yy_xy;            /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_yy_yy;            /**< The Stiffness matrix dNidx.dNjdx  */

   MAT*    MAT_xx_x;             /**< for the "stabilization" matrices  */
   MAT*    MAT_xx_y;             /**< for the "stabilization" matrices  */
   MAT*    MAT_xy_x;             /**< for the "stabilization" matrices  */
   MAT*    MAT_xy_y;             /**< for the "stabilization" matrices  */
   MAT*    MAT_yy_x;             /**< for the "stabilization" matrices  */
   MAT*    MAT_yy_y;             /**< for the "stabilization" matrices  */

   MAT*    MAT_x_xx;             /**< for the "stabilization" matrices  */
   MAT*    MAT_y_xx;             /**< for the "stabilization" matrices  */
   MAT*    MAT_x_xy;             /**< for the "stabilization" matrices  */
   MAT*    MAT_y_xy;             /**< for the "stabilization" matrices  */
   MAT*    MAT_x_yy;             /**< for the "stabilization" matrices  */
   MAT*    MAT_y_yy;             /**< for the "stabilization" matrices  */

   /*MAT*    MAT_I_xx;*/         /* for the "stabilization" matrices  */
   /*MAT*    MAT_I_xy;*/         /* for the "stabilization" matrices  */
   /*MAT*    MAT_I_yy;*/         /* for the "stabilization" matrices  */
   
   /* for Stokes */
   MAT*    MAT_I_x_PM1dP;        /**< The Convection matrix Ni.dNjdx  for Stokes (PM1,P) = ("P1","P2") or ("P2","P3") */
   MAT*    MAT_I_y_PM1dP;        /**< The Convection matrix Ni.dNjdy  for Stokes (PM1,P) = ("P1","P2") or ("P2","P3") */
   
   /* 3D arrays */
   TENSOR*   TENSOR_I_x_I;         /**< 3D array Ni.dNj.Nk  */
   TENSOR*   TENSOR_I_y_I;         /**< 3D array Ni.Nj.dNk  */ 
   TENSOR*   TENSOR_x_I_I;         /**< 3D array Ni.Nj.Nk   */
   TENSOR*   TENSOR_y_I_I;         /**< 3D array dNi.Nj.Nk  */
   TENSOR*   TENSOR_I_I_I;         /**< 3D array Ni.Nj.Nk   */

   /* frequently used matrices */
   MAT*  MAT_x_y__plus__y_x;     /**< is equal to MAT_x_y + MAT_y_x  */

   
   ELT_1D* eltDM1;               /**< Same element on the dimenson -1 */          
   
} ELT_2D;
 

#define ELT_2D_NULL  ((ELT_2D *)NULL)
 
int elt2D_free(ELT_2D *) ; 

#define ELT_2D_FREE(elt)  ( elt2D_free(elt ), (elt )=(ELT_2D *)NULL )

ELT_2D  *elt2D_get(const char* type);



Real F0_2D_P0(Real x , Real y);  /* Element P0 */

Real F0_2D_P1(Real x , Real y);  /* Element P1 */
Real F1_2D_P1(Real x , Real y);
Real F2_2D_P1(Real x , Real y);

Real F0_2D_P2(Real x , Real y);  /* Element P2 */
Real F1_2D_P2(Real x , Real y);
Real F2_2D_P2(Real x , Real y);
Real F3_2D_P2(Real x , Real y);
Real F4_2D_P2(Real x , Real y);
Real F5_2D_P2(Real x , Real y);

Real F0_2D_P3(Real x , Real y);  /* Element P3 */
Real F1_2D_P3(Real x , Real y);
Real F2_2D_P3(Real x , Real y);
Real F3_2D_P3(Real x , Real y);
Real F4_2D_P3(Real x , Real y);
Real F5_2D_P3(Real x , Real y);
Real F6_2D_P3(Real x , Real y);
Real F7_2D_P3(Real x , Real y);
Real F8_2D_P3(Real x , Real y);
Real F9_2D_P3(Real x , Real y);

Real F0_2D_P1b(Real x, Real y);  /* Element P1b - bubble function - */

/************************************************/
/* on donne les derivees ... pour l'element P1  */

Real DFDX0_2D_P1(Real x , Real y);
Real DFDX1_2D_P1(Real x , Real y);
Real DFDX2_2D_P1(Real x , Real y);

Real DFDY0_2D_P1(Real x , Real y);
Real DFDY1_2D_P1(Real x , Real y);
Real DFDY2_2D_P1(Real x , Real y);

/*                       ... pour l'element P2  */

Real DFDX0_2D_P2(Real x , Real y);
Real DFDX1_2D_P2(Real x , Real y);
Real DFDX2_2D_P2(Real x , Real y);
Real DFDX3_2D_P2(Real x , Real y);
Real DFDX4_2D_P2(Real x , Real y);
Real DFDX5_2D_P2(Real x , Real y);

Real DFDY0_2D_P2(Real x , Real y);
Real DFDY1_2D_P2(Real x , Real y);
Real DFDY2_2D_P2(Real x , Real y);
Real DFDY3_2D_P2(Real x , Real y);
Real DFDY4_2D_P2(Real x , Real y);
Real DFDY5_2D_P2(Real x , Real y);

/*                       ... pour l'element P3  */

Real DFDX0_2D_P3(Real x , Real y);
Real DFDX1_2D_P3(Real x , Real y);
Real DFDX2_2D_P3(Real x , Real y);
Real DFDX3_2D_P3(Real x , Real y);
Real DFDX4_2D_P3(Real x , Real y);
Real DFDX5_2D_P3(Real x , Real y);
Real DFDX6_2D_P3(Real x , Real y);
Real DFDX7_2D_P3(Real x , Real y);
Real DFDX8_2D_P3(Real x , Real y);
Real DFDX9_2D_P3(Real x , Real y);

Real DFDY0_2D_P3(Real x , Real y);
Real DFDY1_2D_P3(Real x , Real y);
Real DFDY2_2D_P3(Real x , Real y);
Real DFDY3_2D_P3(Real x , Real y);
Real DFDY4_2D_P3(Real x , Real y);
Real DFDY5_2D_P3(Real x , Real y);
Real DFDY6_2D_P3(Real x , Real y);
Real DFDY7_2D_P3(Real x , Real y);
Real DFDY8_2D_P3(Real x , Real y);
Real DFDY9_2D_P3(Real x , Real y);

/*                       ... pour l'element P1b  */

Real DFDX0_2D_P1b(Real x, Real y);
Real DFDY0_2D_P1b(Real x, Real y);


/*********************************************************/
/* on donne les derivees secondes ... pour l'element P1  */

Real D2FDXX0_2D_P1(Real x , Real y);
Real D2FDXX1_2D_P1(Real x , Real y);
Real D2FDXX2_2D_P1(Real x , Real y);

Real D2FDXY0_2D_P1(Real x , Real y);
Real D2FDXY1_2D_P1(Real x , Real y);
Real D2FDXY2_2D_P1(Real x , Real y);

Real D2FDYY0_2D_P1(Real x , Real y);
Real D2FDYY1_2D_P1(Real x , Real y);
Real D2FDYY2_2D_P1(Real x , Real y);

/*                       ... pour l'element P2  */

Real D2FDXX0_2D_P2(Real x , Real y);
Real D2FDXX1_2D_P2(Real x , Real y);
Real D2FDXX2_2D_P2(Real x , Real y);
Real D2FDXX3_2D_P2(Real x , Real y);
Real D2FDXX4_2D_P2(Real x , Real y);
Real D2FDXX5_2D_P2(Real x , Real y);

Real D2FDXY0_2D_P2(Real x , Real y);
Real D2FDXY1_2D_P2(Real x , Real y);
Real D2FDXY2_2D_P2(Real x , Real y);
Real D2FDXY3_2D_P2(Real x , Real y);
Real D2FDXY4_2D_P2(Real x , Real y);
Real D2FDXY5_2D_P2(Real x , Real y);

Real D2FDYY0_2D_P2(Real x , Real y);
Real D2FDYY1_2D_P2(Real x , Real y);
Real D2FDYY2_2D_P2(Real x , Real y);
Real D2FDYY3_2D_P2(Real x , Real y);
Real D2FDYY4_2D_P2(Real x , Real y);
Real D2FDYY5_2D_P2(Real x , Real y);

/*                       ... pour l'element P3  */

Real D2FDXX0_2D_P3(Real x , Real y);
Real D2FDXX1_2D_P3(Real x , Real y);
Real D2FDXX2_2D_P3(Real x , Real y);
Real D2FDXX3_2D_P3(Real x , Real y);
Real D2FDXX4_2D_P3(Real x , Real y);
Real D2FDXX5_2D_P3(Real x , Real y);
Real D2FDXX6_2D_P3(Real x , Real y);
Real D2FDXX7_2D_P3(Real x , Real y);
Real D2FDXX8_2D_P3(Real x , Real y);
Real D2FDXX9_2D_P3(Real x , Real y);

Real D2FDXY0_2D_P3(Real x , Real y);
Real D2FDXY1_2D_P3(Real x , Real y);
Real D2FDXY2_2D_P3(Real x , Real y);
Real D2FDXY3_2D_P3(Real x , Real y);
Real D2FDXY4_2D_P3(Real x , Real y);
Real D2FDXY5_2D_P3(Real x , Real y);
Real D2FDXY6_2D_P3(Real x , Real y);
Real D2FDXY7_2D_P3(Real x , Real y);
Real D2FDXY8_2D_P3(Real x , Real y);
Real D2FDXY9_2D_P3(Real x , Real y);

Real D2FDYY0_2D_P3(Real x , Real y);
Real D2FDYY1_2D_P3(Real x , Real y);
Real D2FDYY2_2D_P3(Real x , Real y);
Real D2FDYY3_2D_P3(Real x , Real y);
Real D2FDYY4_2D_P3(Real x , Real y);
Real D2FDYY5_2D_P3(Real x , Real y);
Real D2FDYY6_2D_P3(Real x , Real y);
Real D2FDYY7_2D_P3(Real x , Real y);
Real D2FDYY8_2D_P3(Real x , Real y);
Real D2FDYY9_2D_P3(Real x , Real y);

/*                       ... pour l'element P1b  */

Real D2FDXX0_2D_P1b(Real x, Real y);
Real D2FDXY0_2D_P1b(Real x, Real y);
Real D2FDYY0_2D_P1b(Real x, Real y);

#ifdef __cplusplus
}
#endif

#endif

/*! \struct ELT_2D
* \brief the structure for the finite element in 2-D
*
* Contains all basis functions for the elements, as well as all matrices : Mass, Stiffness etc.
*/

/*! \fn elt2D_get(const char *type)
*  \brief initialize an ELT_2D structure
*  \param type : the type of the finite element ("P1", "P2", P3", "P1b")
*/

/*! \fn elt2D_free(ELT_2D *elt)
*  \brief release an ELT_2D structure
*  \param elt : the structure to be released
*/



