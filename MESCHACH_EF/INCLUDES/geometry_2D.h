#ifndef MEF_GEOMETRY_2D_H
#define MEF_GEOMETRY_2D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file geometry_2D.h
 *
 * Contains the structure of the geometry in 2 dimensions
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_2D.h"

typedef struct GEOM_2D {

   char type[16];    /**< "quad1" , "quad2" , "quad3" , "emc2" , "gmsh" */

   int NBSOMM ;      /**< nb sommets  in the computational domain */
   int NBELMT ;      /**< nb elements in the computational domain */
   int NBFACE ;      /**< nb b.faces  in the computational domain */

   IVEC *REF_S ;     /**< references on the nodes               */
   IVEC *REF_T ;     /**< references on the elements(triangles) */
   IVEC *REF_A ;     /**< references on the boundary faces      */

   MAT  *XYSOMM  ;   /**< nodes's coordinates array */
   IMAT *NSELMT  ;   /**< elements's nodes arrays   */
   IMAT *NSFACE  ;   /**< b.aretes's nodes array    */


   struct GEOM_2D *geomBase ;  /**< pointer to a mesh with "P1" ef type if the "parent" Geom is "P2" */

} GEOM_2D ;


#define GEOM_2D_NULL  ((GEOM_2D *)NULL)

int Geom2D_free(GEOM_2D *Geom);
#define GEOM_2D_FREE(Geom)  ( Geom2D_free(Geom), (Geom)=(GEOM_2D *)NULL )

void Geom2D_foutput(FILE *fp, GEOM_2D *Geom);
#define Geom2D_output(Geom) Geom2D_foutput(stdout,Geom);

GEOM_2D *Geom2D_get(const ELT_2D *element, const char *meshfile, const char* meshname, const char *meshtype);
GEOM_2D *Geom2D_get_base_triangle(const char* type, int nb_subdivisions);

GEOM_2D *Geom2D_getP1geom_from(const ELT_2D *element, const GEOM_2D* geom);


int      Geom2D_check_with_boundaryconditions  (const GEOM_2D *MyGeom, const BC_2D *MyBC, const int axe);

#ifdef __cplusplus
}
#endif

#endif

/*! \struct GEOM_2D
* \brief the structure for the Geometry in 2-D
*
* Store all the arrays describing the geometry and needed for the finite element method
*/

