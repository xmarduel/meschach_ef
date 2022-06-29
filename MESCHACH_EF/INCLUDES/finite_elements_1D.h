#ifndef MEF_FINITE_ELEMENTS_1D_H
#define MEF_FINITE_ELEMENTS_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file finite_elements_1D.h
 *
 * Contains the structure of the finite element in 1 dimension
 *
 * Usage : INITIALIZE AN ELT_1D STRUCTURE WITH :
 *
 *    ELT_1D *elt = elt1D_get("P1"); \n
 *    ELT_1D *elt = elt1D_get("P2"); \n
 *    ELT_1D *elt = elt1D_get("P3"); \n
 *    ELT_1D *elt = elt1D_get("H3"); \n
 *    ELT_1D *elt = elt1D_get("S2"); \n
 *    ELT_1D *elt = elt1D_get("S3"); \n
 *    ELT_1D *elt = elt1D_get("S5"); \n
 *
 * ------------------------------------------------------------------------ */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

/*#include "splines_1D.h"*/


typedef struct {

   int      dim;                   /**< dimension; here dim = 1            */
   char*    name_ef;               /**< element's name : P1,P2,P3,H3,S3,S5 */

   int      nb_somm_cell ;         /**< nb base functions of the element   */
   Real     (**f_base)(Real);      /**< fonctions de base de l'element     */
   Real     (**dfdx_base)(Real);   /**< derivees des fonctions de bases    */
   Real     (**ddfdxx_base)(Real); /**< derivees secondes                  */

   MAT*     MAT_I_I;          /**< Mass Matrix Ni.Nj                   */

   MAT*     MAT_x_x;          /**< Stiffness  matrix dNi.dNj   */
   MAT*     MAT_xx_xx;        /**< Stiffness  matrix d2Ni.d2Nj */
   MAT*     MAT_I_x;          /**< Convection matrix Ni.dNj    */

   MAT*     MAT_x_xx ;        /**< for the stabilization terms dNi.ddNj   */
   MAT*     MAT_I_xx ;        /**< for the stabilization terms Ni.ddNj    */

   TENSOR*   TENSOR_I_x_I;      /**< 3D array Ni.dNj.Nk  */
   TENSOR*   TENSOR_x_I_I;      /**< 3D array dNi.Nj.Nk  */
   TENSOR*   TENSOR_I_I_x;      /**< 3D array Ni.Nj.dNk  */
   TENSOR*   TENSOR_I_I_I;      /**< 3D array Ni.Nj.Nk   */

   VEC*     VEC_I;            /**< Right Hand Side (Source cste)  Ni     */
   VEC*     VEC_x;            /**< for the stabilization terms    dNi    */
   VEC*     VEC_xx;           /**< for the stabilization terms    ddNi   */

   MAT*     MAT_FuncBasis_CL_L;   /**< basis permutation to fit BC  with splines */
   MAT*     MAT_FuncBasis_CL_R;   /**< basis permutation to fit BC  with splines */

   /* only for "S2", "S3" or "S5" with non-uniform mesh */
   /*SPLINES_1D*/void *splines_def; /* definition of all functions on the mesh : will depend on the geometry */


} ELT_1D;


#define ELT_1D_NULL  ((ELT_1D *)NULL)

int elt1D_free(ELT_1D *) ;

#define ELT_1D_FREE(elt)  ( elt1D_free(elt ), (elt )=(ELT_1D  *)NULL )

ELT_1D  *elt1D_get(const char* type);

/*-------------------------------------------------------------------*/
/* some methods                                                      */
/*-------------------------------------------------------------------*/
void elt1D_foutput(FILE* fp, ELT_1D* elt);
void elt1D_fdump(FILE* fp, ELT_1D* elt);

#define elt1D_output(elt) elt1D_foutput(stdout, elt);
#define elt1D_dump(elt)   elt1D_fdump(stdout, elt);

/*-------------------------------------------------------------------*/
/* all basis functions                                               */
/*-------------------------------------------------------------------*/
Real F0_1D_P0(Real x);    /* Element P0 */

Real F0_1D_P1(Real x);    /* Element P1 */
Real F1_1D_P1(Real x);

Real F0_1D_P2(Real x);    /* Element P2 */
Real F1_1D_P2(Real x);
Real F2_1D_P2(Real x);

Real F0_1D_P3(Real x);    /* Element P3 */
Real F1_1D_P3(Real x);
Real F2_1D_P3(Real x);
Real F3_1D_P3(Real x);

Real F0_1D_H3(Real x);    /* Element Hermite Cubique */
Real F1_1D_H3(Real x);
Real F2_1D_H3(Real x);
Real F3_1D_H3(Real x);

Real F0_1D_S2(Real x);    /* Element Spline  2nd order */
Real F1_1D_S2(Real x);
Real F2_1D_S2(Real x);

Real F0_1D_S3(Real x);    /* Element Spline  Cubique   */
Real F1_1D_S3(Real x);
Real F2_1D_S3(Real x);
Real F3_1D_S3(Real x);

Real F0_1D_S4(Real x);    /* Element Spline  4 deg */
Real F1_1D_S4(Real x);
Real F2_1D_S4(Real x);
Real F3_1D_S4(Real x);
Real F4_1D_S4(Real x);

Real F0_1D_S5(Real x);    /* Element Spline  Quintique */
Real F1_1D_S5(Real x);
Real F2_1D_S5(Real x);
Real F3_1D_S5(Real x);
Real F4_1D_S5(Real x);
Real F5_1D_S5(Real x);

/*-------------------------------------------------------------------*/
/* on donne les derivees ... pour ts les elements */

Real DFDX0_1D_P0(Real x);    /* Element P0 */

Real DFDX0_1D_P1(Real x);    /* Element P1 */
Real DFDX1_1D_P1(Real x);

Real DFDX0_1D_P2(Real x);    /* Element P2 */
Real DFDX1_1D_P2(Real x);
Real DFDX2_1D_P2(Real x);

Real DFDX0_1D_P3(Real x);    /* Element P3 */
Real DFDX1_1D_P3(Real x);
Real DFDX2_1D_P3(Real x);
Real DFDX3_1D_P3(Real x);

Real DFDX0_1D_H3(Real x);    /* Element Hermite Cubique */
Real DFDX1_1D_H3(Real x);
Real DFDX2_1D_H3(Real x);
Real DFDX3_1D_H3(Real x);

Real DFDX0_1D_S2(Real x);    /* Element Spline  2nd order */
Real DFDX1_1D_S2(Real x);
Real DFDX2_1D_S2(Real x);

Real DFDX0_1D_S3(Real x);    /* Element Spline  Cubique   */
Real DFDX1_1D_S3(Real x);
Real DFDX2_1D_S3(Real x);
Real DFDX3_1D_S3(Real x);

Real DFDX0_1D_S4(Real x);    /* Element Spline  4 order */
Real DFDX1_1D_S4(Real x);
Real DFDX2_1D_S4(Real x);
Real DFDX3_1D_S4(Real x);
Real DFDX4_1D_S4(Real x);

Real DFDX0_1D_S5(Real x);    /* Element Spline  Quintique */
Real DFDX1_1D_S5(Real x);
Real DFDX2_1D_S5(Real x);
Real DFDX3_1D_S5(Real x);
Real DFDX4_1D_S5(Real x);
Real DFDX5_1D_S5(Real x);

/*-------------------------------------------------------------------*/
/* on donne les derivees secondes ... pour ts les elements */

Real DDFDXX0_1D_P0(Real x);    /* Element P0 */

Real DDFDXX0_1D_P1(Real x);    /* Element P1 */
Real DDFDXX1_1D_P1(Real x);

Real DDFDXX0_1D_P2(Real x);    /* Element P2 */
Real DDFDXX1_1D_P2(Real x);
Real DDFDXX2_1D_P2(Real x);

Real DDFDXX0_1D_P3(Real x);    /* Element P3 */
Real DDFDXX1_1D_P3(Real x);
Real DDFDXX2_1D_P3(Real x);
Real DDFDXX3_1D_P3(Real x);


Real DDFDXX0_1D_H3(Real x);    /* Element Hermite Cubique */
Real DDFDXX1_1D_H3(Real x);
Real DDFDXX2_1D_H3(Real x);
Real DDFDXX3_1D_H3(Real x);

Real DDFDXX0_1D_S2(Real x);    /* Element Spline  2nd order */
Real DDFDXX1_1D_S2(Real x);
Real DDFDXX2_1D_S2(Real x);

Real DDFDXX0_1D_S3(Real x);    /* Element Spline  Cubique   */
Real DDFDXX1_1D_S3(Real x);
Real DDFDXX2_1D_S3(Real x);
Real DDFDXX3_1D_S3(Real x);

Real DDFDXX0_1D_S4(Real x);    /* Element Spline  4 order */
Real DDFDXX1_1D_S4(Real x);
Real DDFDXX2_1D_S4(Real x);
Real DDFDXX3_1D_S4(Real x);
Real DDFDXX4_1D_S4(Real x);

Real DDFDXX0_1D_S5(Real x);    /* Element Spline  Quintique */
Real DDFDXX1_1D_S5(Real x);
Real DDFDXX2_1D_S5(Real x);
Real DDFDXX3_1D_S5(Real x);
Real DDFDXX4_1D_S5(Real x);
Real DDFDXX5_1D_S5(Real x);


#ifdef __cplusplus
}
#endif

#endif


/*! \struct ELT_1D
* \brief the structure for the finite element in 1-D
*
* Contains all basis functions for the elements, as well as all matrices : Mass, Stiffness etc.
*/

/*! \fn elt1D_get(const char *type)
*  \brief initialize an ELT_1D structure
*  \param type : the type of the finite element ("P1", "P2", P3", H3", "S2", "S3", "S4", "S5")
*/

/*! \fn elt1D_free(ELT_1D *elt)
*  \brief release an ELT_1D structure
*  \param elt : the structure to be released
*/

/*! \fn elt1D_foutput(FILE *fp, ELT_1D *elt)
*  \brief print to the stream fp the members of elt
*  \param fp  : the output stream
*  \param elt : the structure to be released
*/

/*! \fn elt1D_fdump(FILE *fp, ELT_1D *elt)
*  \brief print to the stream fp the members of elt
*  \param fp  : the output stream
*  \param elt : the structure to be released
*/


/*! \fn F0_1D_P0( Real x )
*  \brief the base function for element "P0"
*/

/*! \fn F0_1D_P1( Real x )
*  \brief the 1rst base function for element "P1"
*/

/*! \fn F1_1D_P1( Real x )
*  \brief the 2nd base function for element "P1"
*/

/*! \fn F0_1D_P2( Real x )
*  \brief the 1rst base function for element "P2"
*/

/*! \fn F1_1D_P2( Real x )
*  \brief the 2nd base function for element "P2"
*/

/*! \fn F2_1D_P2( Real x )
*  \brief the 3th base function for element "P2"
*/
