#ifndef MEF_GEOMETRY_1D_H
#define MEF_GEOMETRY_1D_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file geometry_1D.h
 *
 * Contains the structure of the geometry in 1 dimension
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_1D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

typedef enum {
  
   NON_PERIODIC_MESHe = 0,
   PERIODIC_MESHe = 1,
	
} MESH_PERIODICITY;


typedef struct GEOM_1D {

  char type[16];     /* "line" */
	
  int periodicity;    /**< 1->"periodic" or 0->"non-periodic" */
	
  int NB_DOF ;      /**< nb dof      in the computationnal domain */

  int NBSOMM ;      /**< nb sommets  in the computationnal domain */
  int NBELMT ;      /**< nb elements in the computationnal domain */
  int NBFACE ;      /**< nb faces    in the computationnal domain */
  
  VEC  *XSOMM   ;   /**< contains the array of coords sommets */
  IMAT *NSELMT  ;   /**< contains the array of elements somm's number */
  IMAT *NSFACE  ;   /**< contains the array of aretes somm's number   */
  
  IVEC *REF_S ;     /**< references on the nodes               */
  IVEC *REF_T ;     /**< references on the elements(segments)  */
  IVEC *REF_A ;     /**< references on the boundary faces      */

  Real X_LEFT;
  Real X_RIGHT;
  
  SPMAT* EF_to_WORLD; /**< map EF coords to PHYSIC coords */

  SPMAT* BASEFUNC_BASICS_to_CL; /**< matrix transformation Base Functions : basic bf to cl'ed bf */
   
  IVEC* S2_S4_REF_S_TO_DOF; /**< mapping between DOF and NODES */
  IVEC* S2_S4_DOF_TO_REF_S; /**< mapping between NODES and DOF */

} GEOM_1D ;

 
#define GEOM_1D_NULL  ((GEOM_1D *)NULL)

void Geom1D_foutput(FILE *fp, GEOM_1D *Geom);
#define Geom1D_output(Geom) Geom1D_foutput(stdout,Geom);

int Geom1D_free(GEOM_1D *Geom);
#define GEOM_1D_FREE(Geom)  ( Geom1D_free(Geom), (Geom)=(GEOM_1D *)NULL )

GEOM_1D* Geom1D_setup_from_params(const ELT_1D *MyElt, const PARAMS* params);
	
GEOM_1D* Geom1D_get_fromfile   (const ELT_1D *elt, const char *meshfile, const char *meshname, const char *meshtype);
GEOM_1D* Geom1D_get_fromscratch(const ELT_1D *elt, Real xmin, Real xman, int nx, MESH_PERIODICITY periodicity, const char* distribution_type);

GEOM_1D *Geom1D_getP1geom_from (const ELT_1D *elt, const GEOM_1D* geom);

VEC     *Geom1D_getBaseMesh    (const ELT_1D *elt, const GEOM_1D* geom);

int      Geom1D_check_with_boundaryconditions   (const GEOM_1D *MyGeom, const BC_1D *MyBC, const int axe );

#ifdef __cplusplus
}
#endif

#endif

/*! \struct GEOM_1D
* \brief the structure for the Geometry in 1-D
*
* Store all the arrays describing the geometry and needed for the finite element method
*/

