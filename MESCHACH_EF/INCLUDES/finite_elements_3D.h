#ifndef MEF_FINITE_ELEMENTS_3D_H
#define MEF_FINITE_ELEMENTS_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file finite_elements_3D.h
 *
 * Contains the structure of the finite element in 3 dimensions
 *
 * Usage : INITIALIZE AN ELT_3D STRUCTURE WITH :
 *
 *    ELT_3D *elt = elt3D_get("P1");\n
 *    ELT_3D *elt = elt3D_get("P2");\n
 *    ELT_3D *elt = elt3D_get("P3");\n
 *    ELT_3D *elt = elt3D_get("P1b");\n
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"


typedef struct {

   int     dim;                           /**< dimension; here dim = 3               */
   char*   name_ef;                       /**< nom de l'element : P1 , P2 , P1b      */

   int     nb_somm_cell ;                 /**< nb de fonctions de base de l'element  */
   Real    (**f_base)(Real,Real,Real);    /**< fonctions de base de l'element        */
   Real    (**dfdx_base)(Real,Real,Real); /**< derivees des fonctions de base        */
   Real    (**dfdy_base)(Real,Real,Real); /**< derivees des fonctions de base        */
   Real    (**dfdz_base)(Real,Real,Real); /**< derivees des fonctions de base        */
   int     nb_somm_face ;                 /**< nb de fonctions de base de l'arete    */
   Real    (**f_face)(Real,Real);         /**< fonctions de base sur les aretes      */


   VEC*    VEC_Scmbr;           /**< rhs (Source cste) Ni              */

   MAT*    MAT_I_I;             /**< The mass matrix Ni.Nj             */

   MAT*    MAT_x_x;             /**< The Stiffness matrix dNidx.dNjdx  */
   MAT*    MAT_x_y;             /**< The Stiffness matrix dNidx.dNjdy  */
   MAT*    MAT_x_z;             /**< The Stiffness matrix dNidx.dNjdz  */
   MAT*    MAT_y_x;             /**< The Stiffness matrix dNidy.dNjdx  */
   MAT*    MAT_y_y;             /**< The Stiffness matrix dNidy.dNjdy  */
   MAT*    MAT_y_z;             /**< The Stiffness matrix dNidy.dNjdz  */
   MAT*    MAT_z_x;             /**< The Stiffness matrix dNidz.dNjdx  */
   MAT*    MAT_z_y;             /**< The Stiffness matrix dNidz.dNjdy  */
   MAT*    MAT_z_z;             /**< The Stiffness matrix dNidz.dNjdz  */

   MAT*    MAT_I_x;             /**< The Convection matrix Ni.dNjdx  */
   MAT*    MAT_I_y;             /**< The Convection matrix Ni.dNjdy  */
   MAT*    MAT_I_z;             /**< The Convection matrix Ni.dNjdz  */

   /* for Stokes */
   MAT*    MAT_I_x_PM1dP;       /**< The Convection matrix Ni.dNjdx  for Stokes (PM1,P) = ("P1","P2") or ("P2","P3") */
   MAT*    MAT_I_y_PM1dP;       /**< The Convection matrix Ni.dNjdy  for Stokes (PM1,P) = ("P1","P2") or ("P2","P3") */
   MAT*    MAT_I_z_PM1dP;       /**< The Convection matrix Ni.dNjdz  for Stokes (PM1,P) = ("P1","P2") or ("P2","P3") */

   /* 3D arrays */
   TENSOR*   TENSOR_I_x_I;        /**< The 3D array Ni.dNjdx.Nk  */
   TENSOR*   TENSOR_I_y_I;        /**< The 3D array Ni.dNjdy.Nk  */
   TENSOR*   TENSOR_I_z_I;        /**< The 3D array Ni.dNjdz.Nk  */
   TENSOR*   TENSOR_x_I_I;        /**< The 3D array dNidx.Nj.Nk  */
   TENSOR*   TENSOR_y_I_I;        /**< The 3D array dNidy.Nj.Nk  */
   TENSOR*   TENSOR_z_I_I;        /**< The 3D array dNidz.Nj.Nk  */
   TENSOR*   TENSOR_I_I_I;        /**< The 3D array Ni.Nj.Nk     */

   /* frequently used matrices */
   MAT*  MAT_x_y__plus__y_x;   /**< is equal to MAT_x_y + MAT_y_x  */
   MAT*  MAT_x_z__plus__z_x;   /**< is equal to MAT_x_y + MAT_y_x  */
   MAT*  MAT_y_z__plus__z_y;   /**< is equal to MAT_x_y + MAT_y_x  */


   ELT_2D* eltDM1;             /**< Same element on the dimenson -1 */

} ELT_3D;            /**< ---- a INITIALISER dans "elt3D_get" ------ */


#define ELT_3D_NULL  ((ELT_3D *)NULL)

int elt3D_free(ELT_3D *) ;

#define ELT_3D_FREE(elt)  ( elt3D_free(elt), (elt)=(ELT_3D *)NULL )


ELT_3D  *elt3D_get(const char* type);



Real F0_3D_P0(Real x , Real y , Real z);  /* Element P0 */

Real F0_3D_P1(Real x , Real y , Real z);  /* Element P1 */
Real F1_3D_P1(Real x , Real y , Real z);
Real F2_3D_P1(Real x , Real y , Real z);
Real F3_3D_P1(Real x , Real y , Real z);

Real F0_3D_P2(Real x , Real y , Real z);  /* Element P2 */
Real F1_3D_P2(Real x , Real y , Real z);
Real F2_3D_P2(Real x , Real y , Real z);
Real F3_3D_P2(Real x , Real y , Real z);
Real F4_3D_P2(Real x , Real y , Real z);
Real F5_3D_P2(Real x , Real y , Real z);
Real F6_3D_P2(Real x , Real y , Real z);
Real F7_3D_P2(Real x , Real y , Real z);
Real F8_3D_P2(Real x , Real y , Real z);
Real F9_3D_P2(Real x , Real y , Real z);

Real F0_3D_P1b(Real x , Real y , Real z);  /* Element P1b -bubble function- */

/************************************************/
/* on donne les derivees ... pour l'element P1  */

Real DFDX0_3D_P1(Real x , Real y , Real z);
Real DFDX1_3D_P1(Real x , Real y , Real z);
Real DFDX2_3D_P1(Real x , Real y , Real z);
Real DFDX3_3D_P1(Real x , Real y , Real z);

Real DFDY0_3D_P1(Real x , Real y , Real z);
Real DFDY1_3D_P1(Real x , Real y , Real z);
Real DFDY2_3D_P1(Real x , Real y , Real z);
Real DFDY3_3D_P1(Real x , Real y , Real z);

Real DFDZ0_3D_P1(Real x , Real y , Real z);
Real DFDZ1_3D_P1(Real x , Real y , Real z);
Real DFDZ2_3D_P1(Real x , Real y , Real z);
Real DFDZ3_3D_P1(Real x , Real y , Real z);

/*                       ... pour l'element P2  */

Real DFDX0_3D_P2(Real x , Real y , Real z);
Real DFDX1_3D_P2(Real x , Real y , Real z);
Real DFDX2_3D_P2(Real x , Real y , Real z);
Real DFDX3_3D_P2(Real x , Real y , Real z);
Real DFDX4_3D_P2(Real x , Real y , Real z);
Real DFDX5_3D_P2(Real x , Real y , Real z);
Real DFDX6_3D_P2(Real x , Real y , Real z);
Real DFDX7_3D_P2(Real x , Real y , Real z);
Real DFDX8_3D_P2(Real x , Real y , Real z);
Real DFDX9_3D_P2(Real x , Real y , Real z);

Real DFDY0_3D_P2(Real x , Real y , Real z);
Real DFDY1_3D_P2(Real x , Real y , Real z);
Real DFDY2_3D_P2(Real x , Real y , Real z);
Real DFDY3_3D_P2(Real x , Real y , Real z);
Real DFDY4_3D_P2(Real x , Real y , Real z);
Real DFDY5_3D_P2(Real x , Real y , Real z);
Real DFDY6_3D_P2(Real x , Real y , Real z);
Real DFDY7_3D_P2(Real x , Real y , Real z);
Real DFDY8_3D_P2(Real x , Real y , Real z);
Real DFDY9_3D_P2(Real x , Real y , Real z);

Real DFDZ0_3D_P2(Real x , Real y , Real z);
Real DFDZ1_3D_P2(Real x , Real y , Real z);
Real DFDZ2_3D_P2(Real x , Real y , Real z);
Real DFDZ3_3D_P2(Real x , Real y , Real z);
Real DFDZ4_3D_P2(Real x , Real y , Real z);
Real DFDZ5_3D_P2(Real x , Real y , Real z);
Real DFDZ6_3D_P2(Real x , Real y , Real z);
Real DFDZ7_3D_P2(Real x , Real y , Real z);
Real DFDZ8_3D_P2(Real x , Real y , Real z);
Real DFDZ9_3D_P2(Real x , Real y , Real z);

/*                       ... pour l'element P1b  */

Real DFDX0_3D_P1b(Real x , Real y , Real z);
Real DFDY0_3D_P1b(Real x , Real y , Real z);
Real DFDZ0_3D_P1b(Real x , Real y , Real z);

/*                       ... pour l'element P3  */

Real F0_3D_P3(Real x, Real y, Real z);
Real F1_3D_P3(Real x, Real y, Real z);
Real F2_3D_P3(Real x, Real y, Real z);
Real F3_3D_P3(Real x, Real y, Real z);
Real F4_3D_P3(Real x, Real y, Real z);
Real F5_3D_P3(Real x, Real y, Real z);
Real F6_3D_P3(Real x, Real y, Real z);
Real F7_3D_P3(Real x, Real y, Real z);
Real F8_3D_P3(Real x, Real y, Real z);
Real F9_3D_P3(Real x, Real y, Real z);
Real F10_3D_P3(Real x, Real y, Real z);
Real F11_3D_P3(Real x, Real y, Real z);
Real F12_3D_P3(Real x, Real y, Real z);
Real F13_3D_P3(Real x, Real y, Real z);
Real F14_3D_P3(Real x, Real y, Real z);
Real F15_3D_P3(Real x, Real y, Real z);
Real F16_3D_P3(Real x, Real y, Real z);
Real F17_3D_P3(Real x, Real y, Real z);
Real F18_3D_P3(Real x, Real y, Real z);
Real F19_3D_P3(Real x, Real y, Real z);

Real DFDX0_3D_P3(Real x, Real y, Real z);
Real DFDX1_3D_P3(Real x, Real y, Real z);
Real DFDX2_3D_P3(Real x, Real y, Real z);
Real DFDX3_3D_P3(Real x, Real y, Real z);
Real DFDX4_3D_P3(Real x, Real y, Real z);
Real DFDX5_3D_P3(Real x, Real y, Real z);
Real DFDX6_3D_P3(Real x, Real y, Real z);
Real DFDX7_3D_P3(Real x, Real y, Real z);
Real DFDX8_3D_P3(Real x, Real y, Real z);
Real DFDX9_3D_P3(Real x, Real y, Real z);
Real DFDX10_3D_P3(Real x, Real y, Real z);
Real DFDX11_3D_P3(Real x, Real y, Real z);
Real DFDX12_3D_P3(Real x, Real y, Real z);
Real DFDX13_3D_P3(Real x, Real y, Real z);
Real DFDX14_3D_P3(Real x, Real y, Real z);
Real DFDX15_3D_P3(Real x, Real y, Real z);
Real DFDX16_3D_P3(Real x, Real y, Real z);
Real DFDX17_3D_P3(Real x, Real y, Real z);
Real DFDX18_3D_P3(Real x, Real y, Real z);
Real DFDX19_3D_P3(Real x, Real y, Real z);

Real DFDY0_3D_P3(Real x, Real y, Real z);
Real DFDY1_3D_P3(Real x, Real y, Real z);
Real DFDY2_3D_P3(Real x, Real y, Real z);
Real DFDY3_3D_P3(Real x, Real y, Real z);
Real DFDY4_3D_P3(Real x, Real y, Real z);
Real DFDY5_3D_P3(Real x, Real y, Real z);
Real DFDY6_3D_P3(Real x, Real y, Real z);
Real DFDY7_3D_P3(Real x, Real y, Real z);
Real DFDY8_3D_P3(Real x, Real y, Real z);
Real DFDY9_3D_P3(Real x, Real y, Real z);
Real DFDY10_3D_P3(Real x, Real y, Real z);
Real DFDY11_3D_P3(Real x, Real y, Real z);
Real DFDY12_3D_P3(Real x, Real y, Real z);
Real DFDY13_3D_P3(Real x, Real y, Real z);
Real DFDY14_3D_P3(Real x, Real y, Real z);
Real DFDY15_3D_P3(Real x, Real y, Real z);
Real DFDY16_3D_P3(Real x, Real y, Real z);
Real DFDY17_3D_P3(Real x, Real y, Real z);
Real DFDY18_3D_P3(Real x, Real y, Real z);
Real DFDY19_3D_P3(Real x, Real y, Real z);

Real DFDZ0_3D_P3(Real x, Real y, Real z);
Real DFDZ1_3D_P3(Real x, Real y, Real z);
Real DFDZ2_3D_P3(Real x, Real y, Real z);
Real DFDZ3_3D_P3(Real x, Real y, Real z);
Real DFDZ4_3D_P3(Real x, Real y, Real z);
Real DFDZ5_3D_P3(Real x, Real y, Real z);
Real DFDZ6_3D_P3(Real x, Real y, Real z);
Real DFDZ7_3D_P3(Real x, Real y, Real z);
Real DFDZ8_3D_P3(Real x, Real y, Real z);
Real DFDZ9_3D_P3(Real x, Real y, Real z);
Real DFDZ10_3D_P3(Real x, Real y, Real z);
Real DFDZ11_3D_P3(Real x, Real y, Real z);
Real DFDZ12_3D_P3(Real x, Real y, Real z);
Real DFDZ13_3D_P3(Real x, Real y, Real z);
Real DFDZ14_3D_P3(Real x, Real y, Real z);
Real DFDZ15_3D_P3(Real x, Real y, Real z);
Real DFDZ16_3D_P3(Real x, Real y, Real z);
Real DFDZ17_3D_P3(Real x, Real y, Real z);
Real DFDZ18_3D_P3(Real x, Real y, Real z);
Real DFDZ19_3D_P3(Real x, Real y, Real z);


#ifdef __cplusplus
}
#endif

#endif

/*! \struct ELT_3D
* \brief the structure for the finite element in 3-D
*
* Contains all basis functions for the elements, as well as all matrices : Mass, Stiffness etc.
*/

/*! \fn elt3D_get(const char *type)
*  \brief initialize an ELT_3D structure
*  \param type : the type of the finite element ("P1", "P2", "P3", P1b")
*/

/*! \fn elt3D_free(ELT_3D *elt)
*  \brief release an ELT_3D structure
*  \param elt : the structure to be released
*/

