
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////
 
#ifndef GEOMETRY_H
#define GEOMETRY_H

#define EOL(fp) while( getc(fp) != '\n')

#define MY_LIST3   3

#define TYPE_GEOM_1D    0
#define TYPE_GEOM_2D    1
#define TYPE_GEOM_3D    2

#define NB_STRUCTS3     3


#include "geometry_1D.h"
#include "geometry_2D.h"
#include "geometry_3D.h"

#define GMSH_POINT        15

#define GMSH_P1_LINE       1
#define GMSH_P1_TRIANGLE   2
#define GMSH_P1_TETRAEDRE  4

#define GMSH_P2_LINE       8
#define GMSH_P2_TRIANGLE   9
#define GMSH_P2_TETRAEDRE 10 



#define  NB_ERRS3                    26
#define  NB_WARNS3                   6

//static char *my_err_mesg3[NB_ERRS3] =
//{
//   "unknown geometry error",               /* 0  */
//"geometry type not implemeted" ,        /* 1  */
//"geometry has no geomBase",             /* 2  */
//   "geometry gmsh : wrong indices",        /* 3  */
//   "geometry P2 or P3 : NBSOMM too small (increase COEFF_P1_P2_NBSOMM)",  /* 4 */
//   "geometry P2 : NBSOMM too small ",      /* 5  */
//   "geometry P3 : NBSOMM too small ",      /* 6  */
//   "geometry incompatible with bc",        /* 7  */
//   "geometry cube not [0:1]x[0:1]x[0:1]",  /* 8  */
//	"gmsh :read wrong number of elements",  /* 9  */
//	"gmsh :read wrong number of elements",  /* 10 */
//   "json mesh file validation error",                 /* 11 */
//	"json mesh name object not found",                 /* 12 */
//	"SEGMENTS wrong size",                             /* 13 */
//	"json mesh wrong sum of segments length (diff xmax-xmin)", /* 14 */
//	"X_AXIS_SEGMENTS wrong size",                  /* 15 */
//	"Y_AXIS_SEGMENTS wrong size",                  /* 16 */
//	"SIDES_INFOS - SIDES_IDX - SOUTH wrong size",  /* 17 */
// 	"SIDES_INFOS - SIDES_IDX - NORTH wrong size",  /* 18 */
//	"SIDES_INFOS - SIDES_IDX - EAST wrong size",   /* 19 */
//	"SIDES_INFOS - SIDES_IDX - WEST wrong size",   /* 20 */
//	"VERTICES_INFOS - VERTICES_IDX wrong size",    /* 21 */
//	"VERTICES_INFOS - VERTICES_IDX wrong size",    /* 22 */
//	"X_AXIS_SEGMENTS wrong size",                  /* 23 */
//	"Y_AXIS_SEGMENTS wrong size",                  /* 24 */
//	"Z_AXIS_SEGMENTS wrong size",                  /* 25 */
//};


//static char *my_warn_mesg3[NB_WARNS3] = 
//{
//   "unknown warning",	                                                  /* 0 */
//   "geometry already P1 -> no need for subdivision",                      /* 1 */
//   "geometry has less references than the bc have functions",             /* 2 */
//   "geometry has more ref. than a bc can handle ( increase NBMAX_BC_nD_FUNCTIONS )",  /* 3 */
//   "geometry has more ref. than a bc can handle ( increase NBMAX_BC_nD_FUNCTIONS )",  /* 4 */ 
//   "geometry has more ref. than a bc can handle ( increase NBMAX_BC_nD_FUNCTIONS )"   /* 5 */ 
//};

#define  E_GEOM_UNKNOWN	                0
#define  E_GEOM_TYPENOTIMPL             1
#define  E_GEOM_NOCHILDGEOM             2
#define  E_GEOM_GMSH_WRONG_IDX          3
#define  E_GEOM_3D_NBSOMM_P2_TOO_SMALL  4
#define  E_GEOM_2D_NBSOMM_P2_TOO_SMALL  5
#define  E_GEOM_2D_NBSOMM_P3_TOO_SMALL  6
#define  E_GEOM_INCOMPATIBLE_WITH_BC    7
#define  E_GEOM_WRONG_CUBE              8

#define	E_GEOM_2D_GMSH_WRONG_ELT_NUMBER  9
#define	E_GEOM_3D_GMSH_WRONG_ELT_NUMBER 10

#define  E_GEOM_MESHFILE_JSON_VALIDATION                11
#define  E_GEOM_MESHFILE_JSON_MESH_NOT_FOUND            12

#define	E_GEOM_MESH1D_JSON_SEGMENTS_BAD_ARRAY_SIZE     13
#define	E_GEOM_MESH1D_JSON_SEGMENTS_BAD_TOTAL_SIZE     14

#define  E_GEOM_MESH2D_JSON_X_AXIS_SEGMENTS_BAD_ARRAY_SIZE            15
#define  E_GEOM_MESH2D_JSON_Y_AXIS_SEGMENTS_BAD_ARRAY_SIZE            16
#define  E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_SOUTH_BAD_ARRAY_SIZE  17
#define  E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_NORTH_BAD_ARRAY_SIZE  18
#define  E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_EAST_BAD_ARRAY_SIZE   19
#define  E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_WEST_BAD_ARRAY_SIZE   20
#define  E_GEOM_MESH2D_JSON_VERTICES_INFOS_VERTICES_IDX_BAD_ARRAY_SIZE      21
#define  E_GEOM_MESH2D_JSON_VERTICES_INFOS_VERTICES_ROW_IDX_BAD_ARRAY_SIZE  22

#define  E_GEOM_MESH3D_JSON_X_AXIS_SEGMENTS_BAD_ARRAY_SIZE            23
#define  E_GEOM_MESH3D_JSON_Y_AXIS_SEGMENTS_BAD_ARRAY_SIZE            24
#define  E_GEOM_MESH3D_JSON_Z_AXIS_SEGMENTS_BAD_ARRAY_SIZE            25

#define  W_GEOM_UNKNOWN                 0
#define  W_GEOM_ALREADY_P1              1
#define  W_GEOM_TOOMANYFUNCS_FOR_BC     2
#define  W_GEOM_TOOMANYREFSFORSOMM      3
#define  W_GEOM_TOOMANYREFSFORELTM      4
#define  W_GEOM_TOOMANYREFSFORFACE      5


/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error3(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,3)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning3(err_num,fn_name) ev_warn(__FILE__,err_num,__LINE__,fn_name,3)


int mem_attach_list3(void);
int err_attach_list3(void);
int warn_attach_list3(void);
int err_warn_attach_lists3(void);

#endif


#ifndef GEOMETRY_1D_H
#define GEOMETRY_1D_H

/* Contains the structure of the geometry in 1 dimension
 *
 */

#include "matrix.h"
#include "sparse.h"

#include "geometry_1D.h"
#include "finite_elements_1D.h"
#include "boundary_conditions_1D.h"

typedef enum {
	
   NON_PERIODIC_MESHe = 0,
   PERIODIC_MESHe = 1,
	
} MESH_PERIODICITY;

typedef struct GEOM_1D {

    %immutable;
	 char type[16];     /* "line" */
	
	 int periodicity;  /* 1->"periodic" or 0->"non-periodic" */
	
    int NB_DOF ;      /* nb sommets  in the computational domain */
    int NBSOMM ;      /* nb sommets  in the computational domain */
    int NBELMT ;      /* nb elements in the computational domain */
    int NBFACE ;      /* nb b.faces  in the computational domain */
      
    VEC  *XSOMM   ;   /* contains the arrays of coords sommets */
//  IMAT *NSELMT  ;   /* contains the arrays of coords sommets */
//  IMAT *NSFACE  ;   /* b.aretes's nodes array    */
  
//  IVEC *REF_S ;     /* references on the nodes               */
//  IVEC *REF_T ;     /* references on the elements(segments)  */
//  IVEC *REF_A ;     /* references on the boundary faces      */

//  Real X_LEFT;
//  Real X_RIGHT;

    SPMAT* EF_to_WORLD; /* map EF coords to PHYSIC coords */
    
    SPMAT* BASEFUNC_BASICS_to_CL; /* matrix transformation Base Functions : basic bf to cl'ed bf */
  %mutable;
  
} GEOM_1D ;

 
void Geom1D_foutput(FILE *fp, GEOM_1D *Geom);
#define Geom1D_output(Geom) Geom1D_foutput(stdout,Geom);

int     Geom1D_free(GEOM_1D *Geom);
#define GEOM_1D_FREE(Geom)  ( Geom1D_free(Geom), (Geom)=(GEOM_1D *)NULL )

GEOM_1D* Geom1D_get_fromfile   (ELT_1D *elt , const char *meshfile, const char *meshname, const char*meshtype);
GEOM_1D* Geom1D_get_fromscratch(ELT_1D *elt , Real xmin, Real xman, int nx, MESH_PERIODICITY periodicity, const char* distribution_type);

GEOM_1D *Geom1D_getP1geom_from ( ELT_1D *element, GEOM_1D* geom );
VEC     *Geom1D_getBaseMesh    ( ELT_1D *element, GEOM_1D* geom );

int      Geom1D_check_with_boundaryconditions   ( GEOM_1D *MyGeom, BC_1D *MyBC, int axe);

#endif




#ifndef GEOMETRY_2D_H
#define GEOMETRY_2D_H

/* Contains the structure of the geometry in 2 dimensions
 *
 */

#include "geometry_2D.h"
#include "finite_elements_2D.h"
#include "boundary_conditions_2D.h"

typedef struct GEOM_2D {

    %immutable;
    char type[16];     /* "quad1" , "quad2" , "quad3" , "emc2" , "gmsh" */

    int NBSOMM ;      /* nb sommets  in the computational domain */
    int NBELMT ;      /* nb elements in the computational domain */
    int NBFACE ;      /* nb b.aretes in the computational domain */
    
//  IVEC *REF_S ;     /* references on the nodes               */
//  IVEC *REF_T ;     /* references on the elements(triangles) */
//  IVEC *REF_A ;     /* references on the boundary faces      */

    MAT  *XYSOMM  ;   /* nodes's coordinates array */
    IMAT *NSELMT  ;   /* elements's nodes arrays   */
//  IMAT *NSFACE  ;   /* b.faces's nodes array     */

    %immutable;
    struct GEOM_2D *geomBase ;  /* pointer to a mesh with "P1" ef type */
                                /* if the "parent" Geom is "P2" ef     */
    %mutable;
    
} GEOM_2D ;



int     Geom2D_free(GEOM_2D *Geom);
#define GEOM_2D_FREE(Geom)  ( Geom2D_free(Geom), (Geom)=(GEOM_2D *)NULL )

void    Geom2D_foutput(FILE *fp, GEOM_2D *Geom);  
#define Geom2D_output(Geom) Geom2D_foutput(stdout,Geom);

GEOM_2D *Geom2D_get(ELT_2D *element, const char *meshfile, const char *meshname, const char* meshtype);
GEOM_2D *Geom2D_get_base_triangle(const char* type, int nb_subdivisions);

GEOM_2D *Geom2D_getP1geom_from( ELT_2D *element, GEOM_2D* geom );

int      Geom2D_check_with_boundaryconditions   ( GEOM_2D *MyGeom, BC_2D *MyBC, int axe);

#endif




#ifndef GEOMETRY_3D_H
#define GEOMETRY_3D_H

/* Contains the structure of the geometry in 3 dimensions
 *
 */

#include "geometry_3D.h"
#include "finite_elements_3D.h"
#include "boundary_conditions_3D.h"

typedef struct GEOM_3D {

  %immutable;
  char type[16];     /* "gmsh" , "cube" */

  int NBSOMM ;      /* nb sommets  in the computational domain */
  int NBELMT ;      /* nb elements in the computational domain */
  int NBFACE ;      /* nb b.faces  in the computational domain */

  %mutable;  
//  IVEC *REF_S ;     /* references on the nodes               */
//  IVEC *REF_T ;     /* references on the elements(triangles) */
//  IVEC *REF_A ;     /* references on the boundary faces      */

//  MAT  *XYSOMM  ;   /* nodes's coordinates array */
//  IMAT *NSELMT  ;   /* elements's nodes arrays   */
//  IMAT *NSFACE  ;   /* b.faces's nodes array     */

   %immutable;
   struct GEOM_3D *geomBase ;  /* pointer to a mesh with "P1" ef type */
                               /* if the "parent" Geom is "P2" ef     */
   %mutable; 
    
} GEOM_3D ;



int     Geom3D_free(GEOM_3D *Geom);
#define GEOM_3D_FREE(Geom)  ( Geom3D_free(Geom), (Geom)=(GEOM_3D *)NULL )

void    Geom3D_foutput(FILE *fp, GEOM_3D *Geom);
#define Geom3D_output(Geom) Geom3D_foutput(stdout,Geom);

GEOM_3D *Geom3D_get(const ELT_3D *element, const char *meshfile, const char *meshname, const char* meshtype);
GEOM_3D *Geom3D_get_base_tetrahedra(int NX);

GEOM_3D *Geom3D_getP1geom_from(const ELT_3D *element, const GEOM_3D* geom);

VEC     *Geom3D_extract_P1geom_VEC_from(const ELT_3D *element, const GEOM_3D* geom, const VEC* v);


int      Geom3D_check_with_boundaryconditions   (const GEOM_3D *MyGeom, const BC_3D *MyBC, int axe);

#endif





//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{


#undef	GEOM_1D_FREE
#undef	GEOM_2D_FREE
#undef  GEOM_3D_FREE



void GEOM_1D_FREE(GEOM_1D* geom)
{ 
   Geom1D_free(geom);
   (geom)=(GEOM_1D *)NULL;
}
void GEOM_2D_FREE(GEOM_2D* geom)
{ 
   Geom2D_free(geom);
   (geom)=(GEOM_2D *)NULL;
}
void GEOM_3D_FREE(GEOM_3D* geom)
{ 
   Geom3D_free(geom);
   (geom)=(GEOM_3D *)NULL;
}



%}
