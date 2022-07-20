#ifndef MEF_GEOMETRY_H
#define MEF_GEOMETRY_H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file geometry.h
 *
 * Contains the geometry memory and errors routines handling
 *
 * -------------------------------------------------------------------- */

#define MY_LIST3   3

#define TYPE_GEOM_1D    0
#define TYPE_GEOM_2D    1
#define TYPE_GEOM_3D    2

#define NB_STRUCTS3     3


#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"


/* --------------------------------------------------------------------------- */

static char *my_names3[NB_STRUCTS3] = { "GEOM_1D" ,
                                        "GEOM_2D" ,
                                        "GEOM_3D" };

static int (*my_frees3[NB_STRUCTS3])(void*) = { (int (*)(void *))Geom1D_free ,
                                                (int (*)(void *))Geom2D_free ,
                                                (int (*)(void *))Geom3D_free };

static MEM_ARRAY my_tnums3[NB_STRUCTS3]; /* initialised to zeros */

/* --------------------------------------------------------------------------- */

#define  NB_ERRS3                    44
#define  NB_WARNS3                   6

static char *my_err_mesg3[NB_ERRS3] =
{
   "unknown geometry error",                       /* 0  */
   "geometry type not implemeted" ,                /* 1  */
   "geometry has no geomBase",                     /* 2  */
   "geometry gmsh : wrong indices",                /* 3  */
   "geometry P2 or P3 : NBSOMM too small (increase COEFF_P1_P2_NBSOMM)",  /* 4 */
   "geometry P2 : NBSOMM too small ",              /* 5  */
   "geometry P3 : NBSOMM too small ",              /* 6  */
   "geometry incompatible with bc",                /* 7  */
   "geometry cube not [0:1]x[0:1]x[0:1]",          /* 8  */
	"gmsh :read wrong number of elements",          /* 9  */
	"gmsh :read wrong number of elements",          /* 10 */
   "json mesh file validation error",              /* 11 */
	"json mesh name object not found",              /* 12 */
	"SEGMENTS wrong size",                          /* 13 */
	"json mesh wrong sum of segments length (diff xmax-xmin)", /* 14 */
	"X_AXIS_SEGMENTS wrong size",                   /* 15 */
	"Y_AXIS_SEGMENTS wrong size",                   /* 16 */
	"SIDES_INFOS - SIDES_IDX - SOUTH wrong size",   /* 17 */
 	"SIDES_INFOS - SIDES_IDX - NORTH wrong size",   /* 18 */
	"SIDES_INFOS - SIDES_IDX - EAST wrong size",    /* 19 */
	"SIDES_INFOS - SIDES_IDX - WEST wrong size",    /* 20 */
	"VERTICES_INFOS - VERTICES_IDX wrong size",     /* 21 */
	"VERTICES_INFOS - VERTICES_IDX wrong size",     /* 22 */
	"X_AXIS_SEGMENTS wrong size",                   /* 23 */
	"Y_AXIS_SEGMENTS wrong size",                   /* 24 */
	"Z_AXIS_SEGMENTS wrong size",                   /* 25 */
	"TOP_FACE_BAD_ARRAY_SIZE wrong size",           /* 26 */
	"BOTTOM_FACE_BAD_ARRAY_SIZE wrong size",        /* 27 */
	"FRONT_FACE_BAD_ARRAY_SIZE wrong size",         /* 28 */
	"BACK_FACE_BAD_ARRAY_SIZE wrong size",          /* 29 */
	"RIGHT_FACE_BAD_ARRAY_SIZE wrong size",         /* 30 */
	"LEFT_FACE_BAD_ARRAY_SIZE wrong size",          /* 31 */
	"TOP_FACE_LINE_BAD_ARRAY_SIZE wrong size",      /* 32 */
	"BOTTOM_FACE_LINE_BAD_ARRAY_SIZE wrong size",   /* 33 */
	"FRONT_FACE_LINE_BAD_ARRAY_SIZE wrong size",    /* 34 */
	"BACK_FACE_LINE_BAD_ARRAY_SIZE wrong size",     /* 35 */
	"RIGHT_FACE_LINE_BAD_ARRAY_SIZE wrong size",    /* 36 */
	"LEFT_FACE_LINE_BAD_ARRAY_SIZE wrong size",     /* 37 */
	"FAILED TO FIND $MeshFormat in GMSH 2D MESH",   /* 38 */
	"FAILED TO FIND $Nodes in GMSH 2D MESH",        /* 39 */
	"FAILED TO FIND $Elements in GMSH 2D MESH",     /* 40 */
	"FAILED TO FIND $MeshFormat in GMSH 3D MESH",   /* 41 */
	"FAILED TO FIND $Nodes in GMSH 3D MESH",        /* 42 */
	"FAILED TO FIND $Elements in GMSH 3D MESH",     /* 43 */
};

static char *my_warn_mesg3[NB_WARNS3] =
{
   "unknown warning",	                                                  /* 0 */
   "geometry already P1 -> no need for subdivision",                      /* 1 */
   "more bc functions set than geometry has references (unused bc functions)",        /* 2 */
   "geometry has more ref. than a bc can handle ( increase NBMAX_BC_nD_FUNCTIONS )",  /* 3 */
   "geometry has more ref. than a bc can handle ( increase NBMAX_BC_nD_FUNCTIONS )",  /* 4 */
   "geometry has more ref. than a bc can handle ( increase NBMAX_BC_nD_FUNCTIONS )"   /* 5 */
};

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

#define	E_GEOM_MESH3D_JSON_TOP_FACE_BAD_ARRAY_SIZE         26
#define	E_GEOM_MESH3D_JSON_BOTTOM_FACE_BAD_ARRAY_SIZE      27
#define	E_GEOM_MESH3D_JSON_FRONT_FACE_BAD_ARRAY_SIZE       28
#define	E_GEOM_MESH3D_JSON_BACK_FACE_BAD_ARRAY_SIZE        29
#define	E_GEOM_MESH3D_JSON_RIGHT_FACE_BAD_ARRAY_SIZE       30
#define	E_GEOM_MESH3D_JSON_LEFT_FACE_BAD_ARRAY_SIZE        31

#define	E_GEOM_MESH3D_JSON_TOP_FACE_LINE_BAD_ARRAY_SIZE         32
#define	E_GEOM_MESH3D_JSON_BOTTOM_FACE_LINE_BAD_ARRAY_SIZE      33
#define	E_GEOM_MESH3D_JSON_FRONT_FACE_LINE_BAD_ARRAY_SIZE       34
#define	E_GEOM_MESH3D_JSON_BACK_FACE_LINE_BAD_ARRAY_SIZE        35
#define	E_GEOM_MESH3D_JSON_RIGHT_FACE_LINE_BAD_ARRAY_SIZE       36
#define	E_GEOM_MESH3D_JSON_LEFT_FACE_LINE_BAD_ARRAY_SIZE        37

#define	E_GEOM_2D_GMSH_CANNOT_FIND_MESHFORMAT_LINE  38
#define	E_GEOM_2D_GMSH_CANNOT_FIND_NODE_LINE        39
#define	E_GEOM_2D_GMSH_CANNOT_FIND_ELEMENT_LINE     40

#define	E_GEOM_3D_GMSH_CANNOT_FIND_MESHFORMAT_LINE  41
#define	E_GEOM_3D_GMSH_CANNOT_FIND_NODE_LINE        42
#define	E_GEOM_3D_GMSH_CANNOT_FIND_ELEMENT_LINE     43




#define  W_GEOM_UNKNOWN                 0
#define  W_GEOM_ALREADY_P1              1
#define  W_GEOM_TOOMANYFUNCS_FOR_BC     2
#define  W_GEOM_TOOMANYREFSFORSOMM      3
#define  W_GEOM_TOOMANYREFSFORELTM      4
#define  W_GEOM_TOOMANYREFSFORFACE      5

/************************************************************************/

/* static memory routines handling */
int mem_stat_reg3 (void **var, int type);
int mem_stat_free3(int mark);
#define MEM_STAT_REG3(var,type) mem_stat_reg_list((void **)&(var),type,MY_LIST3) /**< similar to mem_stat_reg3 */

/* dynamic memory routine handling */
int mem_attach_list3(void);

/* error and warnings routine handling */
int err_attach_list3(void);
int warn_attach_list3(void);
int err_warn_attach_lists3(void);

/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	 error3(err_num, fn_name)    ev_err(__FILE__, err_num, __LINE__, fn_name, MY_LIST3)
/* error(E_TYPE, "myfunc") raises error type E_TYPE for function my_func() */
#define	 warning3(warn_num, fn_name) ev_warn(__FILE__, warn_num, __LINE__, fn_name, MY_LIST3)

#ifdef __cplusplus
}
#endif

#endif

/*! \fn mem_attach_list3          ( void )
*
* Set "on" the statistics about the memory used for all the GEOM_xD structures
*/

/*! \fn err_attach_list3          ( void )
*
* Set "on" the setjmp-longjmp error handling
*/

/*! \fn warn_attach_list3          ( void )
*
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn err_warn_attach_lists3     ( void )
*
* Set "on" the setjmp-longjmp error handling and \n
* Set "on" the setjmp-longjmp warning handling
*/

/*! \fn mem_stat_reg3(void **var, int type)
*
* \param var  : adress of a pointer to a ELT_nD structure
* \param type : TYPE_GEOM_1D, TYPE_GEOM_2D or TYPE_GEOM_3D
*
* Register the static variables in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"
*
* static GEOM_2D *MyStaticGeom = Geom2D_get(eltP1, "name_of_file");\n
* mem_stat_reg3(&MyStaticGeom, TYPE_GEOM_2D); // or MEM_STAT_REG3(MyStaticGeom, TYPE_GEOM_2D);\n
*
* mem_stat_free3(1);
*
*/

/*! \fn mem_stat_free3(int mark)
*
* \param mark  : the index of the "workspace" where the static variables are registered
*
* Free the static variables created in a workspace
*
* usage :\n
*
* mem_stat_mark(1); // init the workspace of index "1"\n
*
* static GEOM_2D *MyStaticGeom = Geom2D_get(eltP1, "name_of_file");\n
* mem_stat_reg3(&MyStaticGeom, TYPE_GEOM_2D); // or MEM_STAT_REG3(MyStaticGeom, TYPE_GEOM_2D);\n
*
* mem_stat_free3(1);
*
*/

