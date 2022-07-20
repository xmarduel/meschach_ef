#ifndef MEF_GEOMETRY_3D_H
#define MEF_GEOMETRY_3D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file geometry_3D.h
 *
 * Contains the structure of the geometry in 3 dimensions
 *
 */

#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_3D.h"

typedef struct GEOM_3D {

   char type[16];    /**< "gmsh" , "cube" */

   int NBSOMM ;      /**< nb sommets  in the computational domain */
   int NBELMT ;      /**< nb elements in the computational domain */
   int NBFACE ;      /**< nb b.faces  in the computational domain */

   IVEC *REF_S ;     /**< references on the nodes               */
   IVEC *REF_T ;     /**< references on the elements(triangles) */
   IVEC *REF_A ;     /**< references on the boundary faces      */

   MAT  *XYSOMM  ;   /**< nodes's coordinates array */
   IMAT *NSELMT  ;   /**< elements's nodes arrays   */
   IMAT *NSFACE  ;   /**< b.aretes's nodes array    */


  struct GEOM_3D *geomBase ;  /**< pointer to a mesh with "P1" ef type if the "parent" Geom is "P2" */

} GEOM_3D ;


#define GEOM_3D_NULL  ((GEOM_3D *)NULL)

int Geom3D_free(GEOM_3D *Geom);
#define GEOM_3D_FREE(Geom)  ( Geom3D_free(Geom), (Geom)=(GEOM_3D *)NULL )

void Geom3D_foutput(FILE *fp, GEOM_3D *Geom);
#define Geom3D_output(Geom) Geom3D_foutput(stdout,Geom);

GEOM_3D *Geom3D_get(const ELT_3D *element, const char *meshfile, const char *meshname, const char *meshtype);
GEOM_3D *Geom3D_get_base_tetrahedra(int NX);

GEOM_3D *Geom3D_getP1geom_from(const ELT_3D *element, const GEOM_3D* geom);

VEC     *Geom3D_extract_P1geom_VEC_from(const ELT_3D *element, const GEOM_3D* geom, const VEC* v);


int      Geom3D_check_with_boundaryconditions   ( const GEOM_3D *MyGeom, const BC_3D *MyBC, const int axe );

#ifdef __cplusplus
}
#endif

#endif

/*! \struct GEOM_3D
* \brief the structure for the Geometry in 3-D
*
* Store all the arrays describing the geometry and needed for the finite element method
*/

