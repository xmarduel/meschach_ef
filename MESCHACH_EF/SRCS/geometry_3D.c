
/** \file geometry_3D.c
 *
 * Contains the structure of the geometry in 3 dimensions
 *
 *
 * Note : geom3D_get : actually 2 methods  :
 *
 *      type="cube"  <- cube mesh ( from scratch ) ( see P2 )
 *      type="gmsh"  <- gmsh mesh
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/matrix2.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h" 
#include "MESCHACH_EF/INCLUDES/all_params.h"

#include "MESCHACH_EF/INCLUDES/all_params_json_utils.h"

typedef struct {

   VEC *H1;
   VEC *H2;
   VEC *H3;
   
   IMAT *POINTS_FACE_TOP_REFS;
   IMAT *POINTS_FACE_BOTTOM_REFS;
   IMAT *POINTS_FACE_FRONT_REFS;
   IMAT *POINTS_FACE_BACK_REFS;
   IMAT *POINTS_FACE_RIGHT_REFS;
   IMAT *POINTS_FACE_LEFT_REFS;
   
   IMAT *FACES_FACE_TOP_REFS;    /* not yet in file */
   IMAT *FACES_FACE_BOTTOM_REFS; /* not yet in file */
   IMAT *FACES_FACE_FRONT_REFS;  /* not yet in file */
   IMAT *FACES_FACE_BACK_REFS;   /* not yet in file */
   IMAT *FACES_FACE_RIGHT_REFS;  /* not yet in file */
   IMAT *FACES_FACE_LEFT_REFS;   /* not yet in file */
   
} CUBE_MESH_DATA;       /* for the lecture of a "quad" mesh from the file <meshfile> */


/* fixed dimensions for a cube : [0:1]x[0:1]x[0:1] */
#define CUBEd_XMIN   0.0
#define CUBEd_XMAX   1.0
#define CUBEd_YMIN   0.0
#define CUBEd_YMAX   1.0
#define CUBEd_ZMIN   0.0
#define CUBEd_ZMAX   1.0

/* fixed references for a cube */
#define CUBEd_REF_SIDE_LEFT    6  /* x = 0 */
#define CUBEd_REF_SIDE_RIGHT   5  /* x = 1 */
#define CUBEd_REF_SIDE_FRONT   3  /* y = 0 */
#define CUBEd_REF_SIDE_BACK    4  /* y = 1 */
#define CUBEd_REF_SIDE_BOTTOM  2  /* z = 0 */
#define CUBEd_REF_SIDE_TOP     1  /* z = 1 */


#define ELT_FACE_1  0
#define ELT_FACE_2  1
#define ELT_FACE_3  2
#define ELT_FACE_4  3


typedef struct
{
   Real x,y,z;
   
} point3D;

#define REACH_EOL(fp) while( getc(fp) != '\n')

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

#define GMSH_POINT         15

#define GMSH_P1_LINE        1
#define GMSH_P1_TRIANGLE    2
#define GMSH_P1_QUADRANGLE  3
#define GMSH_P1_TETRAEDRE   4

#define GMSH_P2_LINE        8
#define GMSH_P2_TRIANGLE    9
#define GMSH_P2_QUADRANGLE 10
#define GMSH_P2_TETRAEDRE  11

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static CUBE_MESH_DATA* Geom3D_cube_read_file(const char *meshfile, const char*meshname);

static GEOM_3D* Geom3D_Base_get(const char *meshfile, const char *meshname, const char *meshtype);
static GEOM_3D* Geom3D_Base_gmsh_get(const char *meshfile);
static GEOM_3D* Geom3D_Base_cube_get(const char *meshfile, const char *meshname);

static GEOM_3D* Geom3D_Parent_get_fromBase        ( const char*   type_ef , const GEOM_3D *GeomP1);
static void     Geom3D_Parent_get_fromBase_P1toP2 ( GEOM_3D *GeomP2 , const GEOM_3D *GeomP1);
static void     Geom3D_Parent_get_fromBase_P1toP1b( GEOM_3D *GeomP1b, const GEOM_3D *GeomP1);

/* mesh check */
static void Geom3D_Base_check(const GEOM_3D *Geom);

static void Geom3D_checkmeshesP1P2 ( const GEOM_3D *GeomP1, const GEOM_3D *GeomP2 );
static void Geom3D_checkmeshesP1P1b( const GEOM_3D *GeomP1, const GEOM_3D *GeomP1b);

static int midpoint_built_or_get(int e, int a, int b, IMAT *MID_POINTS, GEOM_3D *GeomP2, const GEOM_3D *GeomP1, int *curr_idx);
static int midpoint_built_and_fill_geom(int a, int b, IMAT *MID_POINTS, GEOM_3D *GeomP2, const GEOM_3D *GeomP1, int *curr_idx);
static int check_two_somm_on_a_face(const GEOM_3D *GeomP1, int a, int b);

static int Geom3D_get_nb_ref_on_somm(const GEOM_3D* Geom);
static int Geom3D_get_nb_ref_on_elts(const GEOM_3D *Geom);
static int Geom3D_get_nb_ref_on_face(const GEOM_3D *Geom);

#define COEFF_P1_P2_NBSOMM  2 /* change this if error */

static GEOM_3D *Geom3D_getP1geom_from_P2geom(const ELT_3D *element, const GEOM_3D* geom);
static GEOM_3D *Geom3D_getP1geom_from_P3geom(const ELT_3D *element, const GEOM_3D* geom);

static VEC     *Geom3D_extract_P1geom_VEC_from_P2geom_VEC(const ELT_3D *element, const GEOM_3D* geom, const VEC* v);
static VEC     *Geom3D_extract_P1geom_VEC_from_P3geom_VEC(const ELT_3D *element, const GEOM_3D* geom, const VEC* v);


static void Geom3D_Base_gmsh_extract_dimensions        (FILE *fp, GEOM_3D* MyGeom);
static void Geom3D_Base_gmsh_get_fill_geom             (FILE *fp, GEOM_3D *MyGeom);


/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

GEOM_3D *Geom3D_get(const ELT_3D *element, const char *meshfile, const char *meshname, const char *meshtype)
{
   GEOM_3D *GeomP1;
   GEOM_3D *GeomP2;
   GEOM_3D *GeomP1b;
   GEOM_3D *Geom;

   if ( element   == NULL ) error(E_NULL, "Geom3D_get");
   if ( meshfile  == NULL ) error(E_NULL, "Geom3D_get");
   
   GeomP1 = Geom3D_Base_get(meshfile, meshname, meshtype);

   if ( (strcmp(element->name_ef, "P1")==0 ) )
   {               
      Geom = GeomP1;    
   }
   else
   if ( (strcmp(element->name_ef, "P1b")==0 ) )
   {               
      GeomP1b = Geom3D_Parent_get_fromBase("P1b", GeomP1);
      Geom    = GeomP1b;  
   }
   else
   if ( (strcmp(element->name_ef, "P2")==0 ) )
   {        
      GeomP2  = Geom3D_Parent_get_fromBase("P2", GeomP1);
      Geom    = GeomP2;  
   }
   else
   {
      error2(E_EF_NOTIMPL, "Geom3D_get");
   }

   return Geom;
   
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

int Geom3D_free(GEOM_3D *Geom)
{  
   if ( Geom == (GEOM_3D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_GEOM_3D, sizeof(GEOM_3D), 0, MY_LIST3);
         mem_numvar_list(TYPE_GEOM_3D, -1, MY_LIST3);
      }
      
      /* free the structure GEOM_3D */
      
      IV_FREE(Geom->REF_S);
      IV_FREE(Geom->REF_T);
      IV_FREE(Geom->REF_A);
      
      M_FREE(Geom->XYSOMM);
      IM_FREE(Geom->NSELMT);
      IM_FREE(Geom->NSFACE);
      
      /* may be also Geom->Geom_P1 to free */
      if ( Geom->geomBase != (GEOM_3D *)NULL ) GEOM_3D_FREE(Geom->geomBase);
      
      free(Geom);
      
      return EXIT_SUCCESS;
   }
   
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void Geom3D_foutput(FILE *fp, GEOM_3D *Geom)
{
   if ( Geom == NULL ) error(E_NULL, "Geom3D_foutput");
   
   
   if ( (strcmp(Geom->type, "gmsh") == 0 )||
        (strcmp(Geom->type, "cube") == 0 )||
        (strcmp(Geom->type, "tetra") == 0 ) )
   {
      fprintf(fp, "TYPE OF MESH : %s \n" ,Geom->type);
      
      fprintf(fp, "#NBSOMM = %d \n", Geom->NBSOMM);
      fprintf(fp, "#NBELMT = %d \n", Geom->NBELMT);
      fprintf(fp, "#NBFACE = %d \n", Geom->NBFACE);
      
      fprintf(fp, "#REF_S : "); iv_foutput(fp, Geom->REF_S);
      fprintf(fp, "#REF_T : "); iv_foutput(fp, Geom->REF_T);
      fprintf(fp, "#REF_A : "); iv_foutput(fp, Geom->REF_A);
      
      fprintf(fp, "#XYSOMM : ");  m_foutput(fp, Geom->XYSOMM);
      fprintf(fp, "#NSELMT : "); im_foutput(fp, Geom->NSELMT);
      fprintf(fp, "#NSFACE : "); im_foutput(fp, Geom->NSFACE);

      fprintf(fp, "#nb_ref_on_somm = %d \n", Geom3D_get_nb_ref_on_somm(Geom) );
      fprintf(fp, "#nb_ref_on_elts = %d \n", Geom3D_get_nb_ref_on_elts(Geom) );
      fprintf(fp, "#nb_ref_on_face = %d \n", Geom3D_get_nb_ref_on_face(Geom) );
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom3D_foutput");
   }
   
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static GEOM_3D *Geom3D_Base_get(const char *meshfile, const char*meshname, const char *meshtype)
{
   GEOM_3D *Geom;
   
   if ( meshtype  == NULL ) error(E_NULL, "Geom3D_Parent_get");
   if ( meshfile  == NULL ) error(E_NULL, "Geom3D_Parent_get");
   

   if ( strcmp(meshtype, "cube") == 0 )
   {
      Geom = Geom3D_Base_cube_get(meshfile, meshname);

      Geom3D_Base_check(Geom);
   }
   else
   if ( strcmp(meshtype, "gmsh") == 0 )
   {
      Geom = Geom3D_Base_gmsh_get(meshfile);

      Geom3D_Base_check(Geom);
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom3D_Base_get");
   }
   
   return Geom;
   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static CUBE_MESH_DATA* Geom3D_cube_read_file(const char *meshfile, const char *meshname)
{
   FILE *fp;

   CUBE_MESH_DATA* cube_mesh_data = (CUBE_MESH_DATA*)malloc(sizeof(CUBE_MESH_DATA));

   int i,j;

   int NX;
   int NY;
   int NZ;

   if ( meshfile == NULL )  error(E_NULL, "Geom3D_cube_read_file");
   if ( meshname == NULL )  error(E_NULL, "Geom3D_cube_read_file");

   // NEW : read JSON file ...
   char validation_output[8192];
   char *meshfile_schema = "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/JSON_SCHEMAS/SCHEMA_MESH_3D.json";
   int status = json_check_data(meshfile, meshfile_schema, validation_output);
   
   printf("%s", validation_output);
   
   if ( status != 0 )
   {
      error3(E_GEOM_MESHFILE_JSON_VALIDATION, "Geom3D_cube_read_file");
      fprintf(stderr, "%s", validation_output);
   }
   
   json_t *meshes = json_load_xfile(meshfile);
   json_t *mesh   = json_object_get(meshes, meshname);
   
   if ( mesh == NULL )
   {
      error3(E_GEOM_MESHFILE_JSON_MESH_NOT_FOUND, "Geom3D_cube_read_file");
   }
   
   json_unpack(mesh, "{s:i, s:i, s:i}", "NX", &NX, "NY", &NY, "NZ", &NZ);

   cube_mesh_data->H1 = v_get(NX);
   cube_mesh_data->H2 = v_get(NY);
   cube_mesh_data->H3 = v_get(NZ);
   
   json_t *x_segments = json_object_get(mesh, "X_AXIS_SEGMENTS");
   json_t *y_segments = json_object_get(mesh, "Y_AXIS_SEGMENTS");
   json_t *z_segments = json_object_get(mesh, "Z_AXIS_SEGMENTS");
   
   if ( NX != json_array_size(x_segments) )
   {
      error3(E_GEOM_MESH3D_JSON_X_AXIS_SEGMENTS_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   if ( NY != json_array_size(y_segments) )
   {
      error3(E_GEOM_MESH3D_JSON_Y_AXIS_SEGMENTS_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   if ( NZ != json_array_size(z_segments) )
   {
      error3(E_GEOM_MESH3D_JSON_Z_AXIS_SEGMENTS_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   
   for (i=0 ;i<NX ;i++) cube_mesh_data->H1->ve[i] = json_real_value(json_array_get(x_segments, i)); 
   for (i=0 ;i<NY ;i++) cube_mesh_data->H2->ve[i] = json_real_value(json_array_get(y_segments, i)); 
   for (i=0 ;i<NZ ;i++) cube_mesh_data->H3->ve[i] = json_real_value(json_array_get(z_segments, i)); 

   
   cube_mesh_data->POINTS_FACE_TOP_REFS    = im_get(NX+1, NY+1);
   cube_mesh_data->POINTS_FACE_BOTTOM_REFS = im_get(NX+1, NY+1);
   
   cube_mesh_data->POINTS_FACE_FRONT_REFS  = im_get(NX+1, NZ+1);
   cube_mesh_data->POINTS_FACE_BACK_REFS   = im_get(NX+1, NZ+1);
   
   cube_mesh_data->POINTS_FACE_RIGHT_REFS  = im_get(NY+1, NZ+1);
   cube_mesh_data->POINTS_FACE_LEFT_REFS   = im_get(NY+1, NZ+1);
   
   json_t *vertices_info = json_object_get(mesh, "VERTICES_INFOS");
   json_t *vertices_idx  = json_object_get(vertices_info, "VERTICES_IDX");
   
   json_t *top_face    = json_object_get(vertices_idx, "TOP_FACE");
   json_t *bottom_face = json_object_get(vertices_idx, "BOTTOM_FACE");
   json_t *front_face  = json_object_get(vertices_idx, "FRONT_FACE");
   json_t *back_face   = json_object_get(vertices_idx, "BACK_FACE");
   json_t *right_face  = json_object_get(vertices_idx, "RIGHT_FACE");
   json_t *left_face   = json_object_get(vertices_idx, "LEFT_FACE");
   
   if ( (NY+1) != json_array_size(top_face) )
   {
      error3(E_GEOM_MESH3D_JSON_TOP_FACE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   if ( (NY+1) != json_array_size(bottom_face) )
   {
      error3(E_GEOM_MESH3D_JSON_BOTTOM_FACE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   
   if ( (NZ+1) != json_array_size(front_face) )
   {
      error3(E_GEOM_MESH3D_JSON_FRONT_FACE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   if ( (NZ+1) != json_array_size(back_face) )
   {
      error3(E_GEOM_MESH3D_JSON_BACK_FACE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   
   if ( (NZ+1) != json_array_size(right_face) )
   {
      error3(E_GEOM_MESH3D_JSON_RIGHT_FACE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   if ( (NZ+1) != json_array_size(left_face) )
   {
      error3(E_GEOM_MESH3D_JSON_LEFT_FACE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
   }
   
   for (j=0 ;j<NY+1 ;j++) 
   {
      json_t *top_face_line    = json_array_get(top_face, j); 
      
      if ( (NX+1) != json_array_size(top_face_line) )
      {
         error3(E_GEOM_MESH3D_JSON_TOP_FACE_LINE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
      }
      
      for (i=0 ;i<NX+1 ;i++)
      {
         cube_mesh_data->POINTS_FACE_TOP_REFS->im[i][j] = json_integer_value(json_array_get(top_face_line, i));
      }
   }
   
   for (j=0 ;j<NY+1 ;j++) 
   {
      json_t *bottom_face_line    = json_array_get(bottom_face, j); 
      
      if ( (NX+1) != json_array_size(bottom_face_line) )
      {
         error3(E_GEOM_MESH3D_JSON_BOTTOM_FACE_LINE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
      }
      
      for (i=0 ;i<NX+1 ;i++)
      {
         cube_mesh_data->POINTS_FACE_BOTTOM_REFS->im[i][j] = json_integer_value(json_array_get(bottom_face_line, i));
      }
   }
   
   for (j=0 ;j<NZ+1 ;j++) 
   {
      json_t *front_face_line    = json_array_get(front_face, j); 
      
      if ( (NX+1) != json_array_size(front_face_line) )
      {
         error3(E_GEOM_MESH3D_JSON_FRONT_FACE_LINE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
      }
      
      for (i=0 ;i<NX+1 ;i++)
      {
         cube_mesh_data->POINTS_FACE_FRONT_REFS->im[i][j] = json_integer_value(json_array_get(front_face_line, i));
      }
   }
   
   for (j=0 ;j<NZ+1 ;j++) 
   {
      json_t *back_face_line    = json_array_get(back_face, j); 
      
      if ( (NX+1) != json_array_size(back_face_line) )
      {
         error3(E_GEOM_MESH3D_JSON_BACK_FACE_LINE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
      }
      
      for (i=0 ;i<NX+1 ;i++)
      {
         cube_mesh_data->POINTS_FACE_BACK_REFS->im[i][j] = json_integer_value(json_array_get(back_face_line, i));
      }
   }
   
   for (j=0 ;j<NZ+1 ;j++) 
   {
      json_t *right_face_line    = json_array_get(right_face, j); 
      
      if ( (NY+1) != json_array_size(right_face_line) )
      {
         error3(E_GEOM_MESH3D_JSON_RIGHT_FACE_LINE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
      }
      
      for (i=0 ;i<NY+1 ;i++)
      {
         cube_mesh_data->POINTS_FACE_RIGHT_REFS->im[i][j] = json_integer_value(json_array_get(right_face_line, i));
      }
   }
   
   for (j=0 ;j<NZ+1 ;j++) 
   {
      json_t *left_face_line    = json_array_get(left_face, j); 
      
      if ( (NY+1) != json_array_size(left_face_line) )
      {
         error3(E_GEOM_MESH3D_JSON_LEFT_FACE_LINE_BAD_ARRAY_SIZE, "Geom3D_cube_read_file");
      }
      
      for (i=0 ;i<NY+1 ;i++)
      {
         cube_mesh_data->POINTS_FACE_LEFT_REFS->im[i][j] = json_integer_value(json_array_get(left_face_line, i));
      }
   }
   
   /* finito */
   return cube_mesh_data;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_cube_set_xyzsomm(GEOM_3D* Geom, const CUBE_MESH_DATA *cube_mesh_data)
{
   int i,j,k,s;

   int NX = cube_mesh_data->H1->dim;
   int NY = cube_mesh_data->H2->dim;
   int NZ = cube_mesh_data->H3->dim;
   
   /* set coords in each directions */
   VEC *COORD1 = v_get(NX+1);
   VEC *COORD2 = v_get(NY+1);
   VEC *COORD3 = v_get(NZ+1);

   COORD1->ve[0] = CUBEd_XMIN;
   for (i=0; i<NX; i++)
   {
      COORD1->ve[i+1] = COORD1->ve[i] + cube_mesh_data->H1->ve[i];
   }

   COORD2->ve[0] = CUBEd_YMIN;
   for (i=0; i<NY; i++)
   {
      COORD2->ve[i+1] = COORD2->ve[i] + cube_mesh_data->H2->ve[i];
   }

   COORD3->ve[0] = CUBEd_ZMIN;
   for (i=0; i<NZ; i++)
   {
      COORD3->ve[i+1] = COORD3->ve[i] + cube_mesh_data->H3->ve[i];
   }

   /**/
   for (k=0; k<=NZ; k++)
   for (j=0; j<=NY; j++)
   for (i=0; i<=NX; i++)
   {
      s = i + j*(NX+1) + k*((NX+1)*(NY+1));

      Geom->XYSOMM->me[s][0] = COORD1->ve[i];
      Geom->XYSOMM->me[s][1] = COORD2->ve[j];
      Geom->XYSOMM->me[s][2] = COORD3->ve[k];
   }

   /* check */
   if ( fabs(COORD1->ve[NX] - CUBEd_XMAX) > 1e5*MACHEPS )
   {
      error3(E_GEOM_WRONG_CUBE, "Geom3D_Base_cube_set_xyzsomm");
   }
   if ( fabs(COORD2->ve[NY] - CUBEd_YMAX) > 1e5*MACHEPS )
   {
      error3(E_GEOM_WRONG_CUBE, "Geom3D_Base_cube_set_xyzsomm");
   }
   if ( fabs(COORD3->ve[NZ] - CUBEd_ZMAX) > 1e5*MACHEPS )
   {
      error3(E_GEOM_WRONG_CUBE, "Geom3D_Base_cube_set_xyzsomm");
   }
   
   /* clean */
   V_FREE(COORD1);
   V_FREE(COORD2);
   V_FREE(COORD3);
}

/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_cube_set_nselmt(GEOM_3D* Geom, const CUBE_MESH_DATA *cube_mesh_data)
{
   int i,j,k,e;
   int NBSOMM = Geom->NBSOMM;
   int s1,s2,s3,s4,s5,s6,s7,s8;

   int NX = cube_mesh_data->H1->dim;
   int NY = cube_mesh_data->H2->dim;
   int NZ = cube_mesh_data->H3->dim;

   for (k=0; k<NZ; k++)
   for (j=0; j<NY; j++)
   for (i=0; i<NX; i++)
   {
      e = 5*( i + j*(NX) + k*((NX)*(NY)) );

      /* the 8 somm on this cube */
      s1 = i   + (j+0)*(NX+1) + (k+0)*((NX+1)*(NY+1));
      s2 = i+1 + (j+0)*(NX+1) + (k+0)*((NX+1)*(NY+1));
      s3 = i+1 + (j+1)*(NX+1) + (k+0)*((NX+1)*(NY+1));
      s4 = i   + (j+1)*(NX+1) + (k+0)*((NX+1)*(NY+1));
      s5 = i   + (j+0)*(NX+1) + (k+1)*((NX+1)*(NY+1));
      s6 = i+1 + (j+0)*(NX+1) + (k+1)*((NX+1)*(NY+1));
      s7 = i+1 + (j+1)*(NX+1) + (k+1)*((NX+1)*(NY+1));
      s8 = i   + (j+1)*(NX+1) + (k+1)*((NX+1)*(NY+1));

      assert( s1 < NBSOMM );
      assert( s2 < NBSOMM );
      assert( s3 < NBSOMM );
      assert( s4 < NBSOMM );
      assert( s5 < NBSOMM );
      assert( s6 < NBSOMM );
      assert( s7 < NBSOMM );
      assert( s8 < NBSOMM );

      /* 2 types of "basic cube" (with 5 tetraedras) */
      if ( (i+j+k)%2 == 0 )
      {
         Geom->NSELMT->im[e+0][0] = s2;
         Geom->NSELMT->im[e+0][1] = s3;
         Geom->NSELMT->im[e+0][2] = s1;
         Geom->NSELMT->im[e+0][3] = s6;

         Geom->NSELMT->im[e+1][0] = s7;
         Geom->NSELMT->im[e+1][1] = s6;
         Geom->NSELMT->im[e+1][2] = s8;
         Geom->NSELMT->im[e+1][3] = s3;

         Geom->NSELMT->im[e+2][0] = s4;
         Geom->NSELMT->im[e+2][1] = s1;
         Geom->NSELMT->im[e+2][2] = s3;
         Geom->NSELMT->im[e+2][3] = s8;

         Geom->NSELMT->im[e+3][0] = s5;
         Geom->NSELMT->im[e+3][1] = s8;
         Geom->NSELMT->im[e+3][2] = s6;
         Geom->NSELMT->im[e+3][3] = s1;

         Geom->NSELMT->im[e+4][0] = s8;
         Geom->NSELMT->im[e+4][1] = s6;
         Geom->NSELMT->im[e+4][2] = s1;
         Geom->NSELMT->im[e+4][3] = s3;
      }
      else
      {
         Geom->NSELMT->im[e+0][0] = s1;
         Geom->NSELMT->im[e+0][1] = s2;
         Geom->NSELMT->im[e+0][2] = s4;
         Geom->NSELMT->im[e+0][3] = s5;

         Geom->NSELMT->im[e+1][0] = s8;
         Geom->NSELMT->im[e+1][1] = s7;
         Geom->NSELMT->im[e+1][2] = s5;
         Geom->NSELMT->im[e+1][3] = s4;

         Geom->NSELMT->im[e+2][0] = s6;
         Geom->NSELMT->im[e+2][1] = s5;
         Geom->NSELMT->im[e+2][2] = s7;
         Geom->NSELMT->im[e+2][3] = s2;

         Geom->NSELMT->im[e+3][0] = s3;
         Geom->NSELMT->im[e+3][1] = s4;
         Geom->NSELMT->im[e+3][2] = s2;
         Geom->NSELMT->im[e+3][3] = s7;

         Geom->NSELMT->im[e+4][0] = s7;
         Geom->NSELMT->im[e+4][1] = s5;
         Geom->NSELMT->im[e+4][2] = s4;
         Geom->NSELMT->im[e+4][3] = s2;
      }
   }
}

/*----------------------------------------------------------------------------------------------------*/

static int  Geom3D_Base_cube_set_nsface__facecenter_is_on_boundary(const point3D *center)
{
   return ( ( (fabs(center->x - CUBEd_XMIN)<1.0e-5) || (fabs(center->x - CUBEd_XMAX)<1.0e-5) ) ||
            ( (fabs(center->y - CUBEd_YMIN)<1.0e-5) || (fabs(center->y - CUBEd_YMAX)<1.0e-5) ) ||
            ( (fabs(center->z - CUBEd_ZMIN)<1.0e-5) || (fabs(center->z - CUBEd_ZMAX)<1.0e-5) ) );
}

/*----------------------------------------------------------------------------------------------------*/

static int  Geom3D_Base_cube_set_nsface__face_is_boundaryface(const GEOM_3D* Geom, int a1, int a2, int a3)
{
   point3D center;
   
   point3D pt_a1;
   point3D pt_a2;
   point3D pt_a3;
   
   /* the coordinates of the cell's node */
   pt_a1.x = Geom->XYSOMM->me[a1][0];
   pt_a1.y = Geom->XYSOMM->me[a1][1];
   pt_a1.z = Geom->XYSOMM->me[a1][2];
   
   pt_a2.x = Geom->XYSOMM->me[a2][0];
   pt_a2.y = Geom->XYSOMM->me[a2][1];
   pt_a2.z = Geom->XYSOMM->me[a2][2];
   
   pt_a3.x = Geom->XYSOMM->me[a3][0];
   pt_a3.y = Geom->XYSOMM->me[a3][1];
   pt_a3.z = Geom->XYSOMM->me[a3][2];

   
   /* first check if all the somm are on the boundary */
   if ( ( Geom->REF_S->ive[a1] > 0 ) &&  ( Geom->REF_S->ive[a2] > 0 ) && ( Geom->REF_S->ive[a3] > 0 ) )
   {
      /* now check if the center is also on the boundary */
      center.x = ( pt_a1.x + pt_a2.x + pt_a3.x ) / 3.0;
      center.y = ( pt_a1.y + pt_a2.y + pt_a3.y ) / 3.0;
      center.z = ( pt_a1.z + pt_a2.z + pt_a3.z ) / 3.0;

      if ( Geom3D_Base_cube_set_nsface__facecenter_is_on_boundary(&center))
      {
         return 1; /* is boundaryface */
      }
      else
      {
         /*
          printf("center = (%lf - %lf - %lf) \n\ta0(%lf-%lf-%lf)\n\ta1(%lf-%lf-%lf)\n\ta2(%lf-%lf-%lf)\n",
                 center.x, center.y, center.z,
                 pt_a1.x , pt_a2.x , pt_a3.x ,
                 pt_a1.y , pt_a2.y , pt_a3.y ,
                 pt_a1.z , pt_a2.z , pt_a3.z );
          */
      }
   }

   return 0; /* not a boundaryface */
}

/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_cube_set_nsface(GEOM_3D* Geom)
{
   int a = -1;
   int elt_node0;
   int elt_node1;
   int elt_node2;
   int elt_node3;
   int i,e;
   
   int NBELMT = Geom->NBELMT;
   int NBFACE = Geom->NBFACE;
   
   int idx_loc[4][3] =
   {
      {0,1,2}, /* local idx for the 1rst face of an elt */
      {0,1,3}, /* local idx for the 2nd  face of an elt */
      {0,2,3}, /* local idx for the 3th  face of an elt */
      {1,2,3}  /* local idx for the 4th  face of an elt */
   };
   
   for (e=0; e<NBELMT; e++)
   {
      /* the 4 nodes indices on this zone */
      elt_node0 = Geom->NSELMT->im[e][0];
      elt_node1 = Geom->NSELMT->im[e][1];
      elt_node2 = Geom->NSELMT->im[e][2];
      elt_node3 = Geom->NSELMT->im[e][3];

      /* 4 faces on this cell : check the 4 faces */

      if ( Geom3D_Base_cube_set_nsface__face_is_boundaryface(Geom, elt_node0, elt_node1, elt_node2) )
      {
         a++; assert( a < Geom->NBFACE );

         Geom->NSFACE->im[a][0] = elt_node0;
         Geom->NSFACE->im[a][1] = elt_node1;
         Geom->NSFACE->im[a][2] = elt_node2;
      }

      if ( Geom3D_Base_cube_set_nsface__face_is_boundaryface(Geom, elt_node0, elt_node1, elt_node3) )
      {
         a++; assert( a < Geom->NBFACE );

         Geom->NSFACE->im[a][0] = elt_node0;
         Geom->NSFACE->im[a][1] = elt_node1;
         Geom->NSFACE->im[a][2] = elt_node3;
      }

      if ( Geom3D_Base_cube_set_nsface__face_is_boundaryface(Geom, elt_node0, elt_node2, elt_node3) )
      {
         a++; assert( a < Geom->NBFACE );

         Geom->NSFACE->im[a][0] = elt_node0;
         Geom->NSFACE->im[a][1] = elt_node2;
         Geom->NSFACE->im[a][2] = elt_node3;
      }

      if ( Geom3D_Base_cube_set_nsface__face_is_boundaryface(Geom, elt_node1, elt_node2, elt_node3) )
      {
         a++; assert( a < Geom->NBFACE );

         Geom->NSFACE->im[a][0] = elt_node1;
         Geom->NSFACE->im[a][1] = elt_node2;
         Geom->NSFACE->im[a][2] = elt_node3;
      }
   }

   /* final check */
   assert( a == (NBFACE-1) );
}

/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_cube_set_refa(GEOM_3D* Geom, const CUBE_MESH_DATA *cube_mesh_data) // FIXME should use cube_mesh_data !!!
{
   int i,a;
   point3D face_center;
   point3D face_nodes[3];

   int NBFACE = Geom->NBFACE;

   for (a=0; a<NBFACE; a++)
   {
      /* the coordinates of the face's node */
      for (i=0; i<3; i++)
      {
         face_nodes[i].x = Geom->XYSOMM->me[Geom->NSFACE->im[a][i]][0];
         face_nodes[i].y = Geom->XYSOMM->me[Geom->NSFACE->im[a][i]][1];
         face_nodes[i].z = Geom->XYSOMM->me[Geom->NSFACE->im[a][i]][2];
      }

      face_center.x = (face_nodes[0].x + face_nodes[1].x + face_nodes[2].x) / 3.0;
      face_center.y = (face_nodes[0].y + face_nodes[1].y + face_nodes[2].y) / 3.0;
      face_center.z = (face_nodes[0].z + face_nodes[1].z + face_nodes[2].z) / 3.0;

      if ( fabs(face_center.x - CUBEd_XMIN) < 1.0e-5 ) Geom->REF_A->ive[a] = CUBEd_REF_SIDE_LEFT;
      if ( fabs(face_center.x - CUBEd_XMAX) < 1.0e-5 ) Geom->REF_A->ive[a] = CUBEd_REF_SIDE_RIGHT;
      if ( fabs(face_center.y - CUBEd_YMIN) < 1.0e-5 ) Geom->REF_A->ive[a] = CUBEd_REF_SIDE_FRONT;
      if ( fabs(face_center.y - CUBEd_YMAX) < 1.0e-5 ) Geom->REF_A->ive[a] = CUBEd_REF_SIDE_BACK;
      if ( fabs(face_center.z - CUBEd_ZMIN) < 1.0e-5 ) Geom->REF_A->ive[a] = CUBEd_REF_SIDE_BOTTOM;
      if ( fabs(face_center.z - CUBEd_ZMAX) < 1.0e-5 ) Geom->REF_A->ive[a] = CUBEd_REF_SIDE_TOP;
   }
}

/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_cube_set_refs(GEOM_3D* Geom, const CUBE_MESH_DATA *cube_mesh_data) // FIXME should use cube_mesh_data !!!
{
   int s;
   int NBSOMM = Geom->NBSOMM;

   for (s=0; s<NBSOMM; s++)
   {
      if ( fabs(Geom->XYSOMM->me[s][0]-CUBEd_XMIN) < 1.0e-5 ) Geom->REF_S->ive[s] = CUBEd_REF_SIDE_LEFT;   /* on X=0 */
      if ( fabs(Geom->XYSOMM->me[s][0]-CUBEd_XMAX) < 1.0e-5 ) Geom->REF_S->ive[s] = CUBEd_REF_SIDE_RIGHT;  /* on X=1 */
      if ( fabs(Geom->XYSOMM->me[s][1]-CUBEd_YMIN) < 1.0e-5 ) Geom->REF_S->ive[s] = CUBEd_REF_SIDE_FRONT;  /* on Y=0 */
      if ( fabs(Geom->XYSOMM->me[s][1]-CUBEd_YMAX) < 1.0e-5 ) Geom->REF_S->ive[s] = CUBEd_REF_SIDE_BACK;   /* on Y=1 */
      if ( fabs(Geom->XYSOMM->me[s][2]-CUBEd_ZMIN) < 1.0e-5 ) Geom->REF_S->ive[s] = CUBEd_REF_SIDE_BOTTOM; /* on Z=0 */
      if ( fabs(Geom->XYSOMM->me[s][2]-CUBEd_ZMAX) < 1.0e-5 ) Geom->REF_S->ive[s] = CUBEd_REF_SIDE_TOP;    /* on Z=1 */
   }
}

/*----------------------------------------------------------------------------------------------------*/

static GEOM_3D* Geom3D_Base_cube_get(const char *meshfile, const char *meshname)
{
   int NBSOMM, NBELMT, NBFACE;
   int NX,NY,NZ;
   
   GEOM_3D *Geom;

   CUBE_MESH_DATA * cube_mesh_data;
      
   /* check */
   if ( meshfile  == NULL ) error(E_NULL, "Geom3D_Base_cube_get");
   if ( meshname  == NULL ) error(E_NULL, "Geom3D_Base_cube_get");

   /* --------- create the structure GEOM_3D ----------- */
   if ( (Geom=NEW(GEOM_3D)) == (GEOM_3D *)NULL )
   {
      error(E_MEM, "Geom3D_Base_cube_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_3D, 0, sizeof(GEOM_3D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_3D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   /* IT IS A CUBE FORMAT */
   strncpy(Geom->type, "cube", 16);
   
   /* we first read NX, NY, NZ the number of segments in each direction */
   cube_mesh_data = Geom3D_cube_read_file(meshfile, meshname);


   /* we have */
   NX = cube_mesh_data->H1->dim;
   NY = cube_mesh_data->H2->dim;
   NZ = cube_mesh_data->H3->dim;
   
   /* we have */
   NBSOMM = (NX+1)*(NY+1)*(NZ+1);  
   /* we have */
   NBELMT = 5*NX*NY*NZ;
   /* we have */
   NBFACE = 2*( 2*(NX*NY) + 2*(NX*NZ) + 2*(NY*NZ) );

   Geom->NBSOMM = NBSOMM;
   Geom->NBELMT = NBELMT;
   Geom->NBFACE = NBFACE;

   /* allocate memory */
   Geom->XYSOMM = m_get(NBSOMM,3);
   Geom->NSELMT = im_get(NBELMT,4);
   Geom->NSFACE = im_get(Geom->NBFACE,3) ;

   Geom->REF_S = iv_get(Geom->NBSOMM);  iv_zero(Geom->REF_S);
   Geom->REF_T = iv_get(Geom->NBELMT);  iv_zero(Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE);  iv_zero(Geom->REF_A);

   /* fill XYSOMM */
   Geom3D_Base_cube_set_xyzsomm(Geom, cube_mesh_data);

   /* fill NSELMT */
   Geom3D_Base_cube_set_nselmt(Geom, cube_mesh_data);
   
   /* fill REF_S */
   Geom3D_Base_cube_set_refs(Geom, cube_mesh_data);

   /* fill NSFACE */
   Geom3D_Base_cube_set_nsface(Geom);   

   /* fill REF_A */
   Geom3D_Base_cube_set_refa(Geom, cube_mesh_data);

   
   /* clean */
   V_FREE(cube_mesh_data->H1);
   V_FREE(cube_mesh_data->H2);
   V_FREE(cube_mesh_data->H3);
   IM_FREE(cube_mesh_data->POINTS_FACE_TOP_REFS);
   IM_FREE(cube_mesh_data->POINTS_FACE_BOTTOM_REFS);
   IM_FREE(cube_mesh_data->POINTS_FACE_FRONT_REFS);
   IM_FREE(cube_mesh_data->POINTS_FACE_BACK_REFS);
   IM_FREE(cube_mesh_data->POINTS_FACE_RIGHT_REFS);
   IM_FREE(cube_mesh_data->POINTS_FACE_LEFT_REFS);
   free(cube_mesh_data);

   /* end */
   return Geom;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_gmsh_extract_dimensions(FILE *fp, GEOM_3D *MyGeom)
{
   int ALL_ELMT, TYPE_ELMT;
   int NBXXXX = 0;
   int s,e,dummy;
   char A_LINE[128];
   char *res = NULL;

   int NBSOMM=0; /* init */
   int NBELMT=0; /* init */
   int NBFACE=0; /* init */
   int NBLINE=0; /* init */
   int NBPNKT=0; /* init */

   fgets(A_LINE, 128, fp);
   while ( strncmp("$MeshFormat", A_LINE, 11) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_3D_GMSH_CANNOT_FIND_MESHFORMAT_LINE, "Geom3D_Base_gmsh_extract_dimensions");
      }
   }
   
   /* find "$Nodes" line */
   while ( strncmp("$Nodes", A_LINE, 6) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_3D_GMSH_CANNOT_FIND_NODE_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   fscanf(fp, "%d \n", &NBSOMM);
   
   rewind(fp);
   
   fgets(A_LINE, 128, fp);
   /* find "$Elements" line */
   while ( strncmp("$Elements", A_LINE, 9) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_3D_GMSH_CANNOT_FIND_NODE_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   /* The number of ALL the structures : points, lines, triangles, tetraedras */
   fscanf(fp, "%d \n", &ALL_ELMT);
   /* Now extract the number of triangles */
   for (e=0; e<ALL_ELMT; e++)
   {
      fscanf(fp, "%d %d", &dummy, &TYPE_ELMT); REACH_EOL(fp);

      switch(TYPE_ELMT)
      {
         case GMSH_POINT:        NBPNKT++; break;
         case GMSH_P1_LINE:      NBLINE++; break;
         case GMSH_P1_TRIANGLE:  NBFACE++; break;
         case GMSH_P1_TETRAEDRE: NBELMT++; break;
         default: /*assert(0);*/ NBXXXX++; break;
      }
   }

   assert( (NBPNKT+NBLINE+NBFACE+NBELMT+NBXXXX)==(ALL_ELMT) );

   /* fill dimensions */
   MyGeom->NBSOMM = NBSOMM;
   MyGeom->NBELMT = NBELMT;
   MyGeom->NBFACE = NBFACE;

   /* finito */
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_gmsh_get_fill_geom(FILE *fp, GEOM_3D *Geom)
{
   int NBSOMM;
   int s, e, num, num_max = 0;
   
   int ALL_ELMT, TYPE_ELMT;
   int intern;
   int gmsh_physical_idx;
   int gmsh_entity_idx;
   int n1,n2,n3,n4;
   int na=0;
   int ne=0;
   int nl=0;
   int np=0;
   char A_LINE[128];
   char *res = NULL;

   double x1,x2,x3;
   double y1,y2,y3;

   double jacobien;

   IVEC *MAP_IDX = iv_get(Geom->NBSOMM);
   IVEC *XMAP_IDX;

   fgets(A_LINE, 128, fp);
   /* find "$Nodes" line */
   while ( strncmp("$Nodes", A_LINE, 6) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_3D_GMSH_CANNOT_FIND_NODE_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   fscanf(fp, "%d \n", &NBSOMM);

   /* get somm coordinates */
   for (s=0; s<NBSOMM; s++)
   {
      fscanf(fp, "%d %lf %lf %lf\n", 
             &num,
             &(Geom->XYSOMM->me[s][0]),
             &(Geom->XYSOMM->me[s][1]),
             &(Geom->XYSOMM->me[s][2]));

      /* fill MAP_IDX for later use : the indices that are read are not contiguous !!! */
      MAP_IDX->ive[s] = num;

      if (num > num_max) num_max = num;
   }

   /* fill XMAP_IDX the inv of MAP_IDX */
   XMAP_IDX = iv_get(num_max+1);
   
   for (s=0; s<NBSOMM; s++)
   {
      num = MAP_IDX->ive[s];
      XMAP_IDX->ive[num] = s;
   }
   /* fill XMAP_IDX the inv of MAP_IDX */

   rewind(fp);
   
   fgets(A_LINE, 128, fp);
   /* find "$Elements" line */
   while ( strncmp("$Elements", A_LINE, 9) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_3D_GMSH_CANNOT_FIND_ELEMENT_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   fscanf(fp, "%d \n", &ALL_ELMT);

   /* get elts somms */
   for (e=0; e<ALL_ELMT; e++)
   {
      fscanf(fp, "%d %d", &num, &TYPE_ELMT );

      switch(TYPE_ELMT)
      {
         case GMSH_POINT:
            REACH_EOL(fp);  np++;

            break;

         case GMSH_P1_LINE:
            REACH_EOL(fp);  nl++;

            break;

         case GMSH_P1_TRIANGLE:
            fscanf(fp, "%d %d %d %d %d %d\n", &intern, &gmsh_physical_idx, &gmsh_entity_idx, &n1, &n2, &n3);

            Geom->REF_S->ive[XMAP_IDX->ive[n1]] = gmsh_physical_idx;
            Geom->REF_S->ive[XMAP_IDX->ive[n2]] = gmsh_physical_idx;
            Geom->REF_S->ive[XMAP_IDX->ive[n3]] = gmsh_physical_idx;


            x1 = Geom->XYSOMM->me[XMAP_IDX->ive[n1]][0];
            x2 = Geom->XYSOMM->me[XMAP_IDX->ive[n2]][0];
            x3 = Geom->XYSOMM->me[XMAP_IDX->ive[n3]][0];

            y1 = Geom->XYSOMM->me[XMAP_IDX->ive[n1]][1];
            y2 = Geom->XYSOMM->me[XMAP_IDX->ive[n2]][1];
            y3 = Geom->XYSOMM->me[XMAP_IDX->ive[n3]][1];

            jacobien = (y3-y1)*(x2-x1) - (x3-x1)*(y2-y1) ;

            if  ( jacobien > 0 )
            {
               Geom->NSFACE->im[na][0] = XMAP_IDX->ive[n1];
               Geom->NSFACE->im[na][1] = XMAP_IDX->ive[n2];
               Geom->NSFACE->im[na][2] = XMAP_IDX->ive[n3];
            }
            else
            {
               Geom->NSFACE->im[na][0] = XMAP_IDX->ive[n1];
               Geom->NSFACE->im[na][1] = XMAP_IDX->ive[n3];
               Geom->NSFACE->im[na][2] = XMAP_IDX->ive[n2];
            }

            Geom->REF_A->ive[na]       = gmsh_physical_idx;

            na++;

            break;

         case GMSH_P1_TETRAEDRE:
            fscanf(fp, "%d %d %d %d %d %d %d\n", &intern, &gmsh_physical_idx, &gmsh_entity_idx, &n1, &n2, &n3, &n4);

            Geom->REF_T->ive[ne] = gmsh_physical_idx;

            Geom->NSELMT->im[ne][0] = XMAP_IDX->ive[n1];
            Geom->NSELMT->im[ne][1] = XMAP_IDX->ive[n2];
            Geom->NSELMT->im[ne][2] = XMAP_IDX->ive[n3];
            Geom->NSELMT->im[ne][3] = XMAP_IDX->ive[n4];

            ne++;

            break;
      }
   }

   if ( (np+nl+ne+na) != (ALL_ELMT) )
   {
      error3(E_GEOM_3D_GMSH_WRONG_ELT_NUMBER, "Geom3D_Base_gmsh_get_fill_geom");
   }
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static GEOM_3D* Geom3D_Base_gmsh_get(const char *meshfile)
{
   int NBSOMM=0;
   int NBELMT=0;
   int NBFACE=0;
   int NBLINE=0;
   int NBPNKT=0;
   
   GEOM_3D * Geom;
   FILE *    fp;

   /* check */
   if ( meshfile  == NULL ) error(E_NULL, "Geom3DBase_gmsh_get");

   fp = fopen(meshfile, "r");
   /* open file */
   if ( fp == NULL )
   {
      error(E_FNF, "Geom3DBase_gmsh_get");
   }
   
   /* --------- create the structure GEOM_3D ----------- */
   if ( (Geom=NEW(GEOM_3D)) == (GEOM_3D *)NULL )
   {
      error(E_MEM, "Geom3DBase_gmsh_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_3D, 0, sizeof(GEOM_3D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_3D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
   
   
   /* IT IS A GMSH FORMAT */
   strncpy(Geom->type,"gmsh", 16);

   
   /* we first read NBSOMM, NBELMT and NBFACE */
   Geom3D_Base_gmsh_extract_dimensions(fp, Geom);
   
   /* allocate memory */
   Geom->XYSOMM = m_get(Geom->NBSOMM,3); 
   Geom->NSELMT = im_get(Geom->NBELMT,4) ;
   Geom->NSFACE = im_get(Geom->NBFACE,3) ;
   
   Geom->REF_S = iv_get(Geom->NBSOMM);  iv_zero(Geom->REF_S);
   Geom->REF_T = iv_get(Geom->NBELMT);  iv_zero(Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE);  iv_zero(Geom->REF_A);
   
   /* rewind the file and re-read and fill the arrays */
   rewind(fp);
   
   /**/
   Geom3D_Base_gmsh_get_fill_geom(fp, Geom);

   /**/
   fclose(fp);

   /* end */
   return Geom;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static GEOM_3D *Geom3D_Parent_get_fromBase(const char* name_ef, const GEOM_3D *GeomP1)
{
   GEOM_3D *Geom = NULL;

   if ( GeomP1 == NULL )  error(E_NULL, "Geom3DParent_get_fromBase");
   if ( name_ef == NULL )  error(E_NULL, "Geom3DParent_get_fromBase");

   /* --------- create the structure GEOM_3D ----------- */
   if ( (Geom=NEW(GEOM_3D)) == (GEOM_3D *)NULL )
   {
      error(E_MEM, "Geom3DParent_get_fromBase");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_3D, 0, sizeof(GEOM_3D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_3D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
   
   Geom->geomBase = (GEOM_3D*)GeomP1;  /* cast because of the "const" */
      
   if ( strcmp("P2",name_ef) == 0 )
   {
      Geom3D_Parent_get_fromBase_P1toP2(Geom, GeomP1);
   }
   else
   if ( strcmp("P1b",name_ef) == 0 )
   {
      Geom3D_Parent_get_fromBase_P1toP1b(Geom, GeomP1);
   }
   else
   {
      error2(E_EF_NOTIMPL, "Geom3DParent_get_fromBase");
   }
   
   /* return */
   return Geom;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_checkmeshesP1P1b(const GEOM_3D *GeomP1, const GEOM_3D *GeomP1b)
{
   if ( GeomP1  == NULL ) error(E_NULL, "Geom3D_checkmeshesP1P1b");
   if ( GeomP1b == NULL ) error(E_NULL, "Geom3D_checkmeshesP1P1b");
   
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static double point3D_norm(const point3D *pt)
{
   return ( (pt->x * pt->x) + (pt->y * pt->y) + (pt->y * pt->y) );
}

/*----------------------------------------------------------------------------------------------------*/

static int check_pt_equal_pt( const point3D *pt1, const point3D *pt2)
{
   int ok = 1; /* init */

   if ( (fabs(pt1->x - pt2->x) > MACHEPS) || (fabs(pt1->y - pt2->y) > MACHEPS) || (fabs(pt1->z - pt2->z) > MACHEPS) )
   {
      ok = 0;
   }
   
   return ok;
}

/*----------------------------------------------------------------------------------------------------*/

static point3D * set_pt1_pt2_center( const point3D *pt1, const point3D *pt2, point3D *center)
{
   center->x = ( pt1->x + pt2->x ) / 2.0 ;
   center->y = ( pt1->y + pt2->y ) / 2.0 ;
   center->z = ( pt1->z + pt2->z ) / 2.0 ;

   return center;
}

/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_checkmeshesP1P2 ( const GEOM_3D *GeomP1, const GEOM_3D *GeomP2)
{
   int e,i;

   point3D onP1[4];
   point3D onP2[10];
   point3D center;
   
   if ( GeomP1 == NULL ) error(E_NULL, "Geom3D_checkmeshesP1P2");
   if ( GeomP2 == NULL ) error(E_NULL, "Geom3D_checkmeshesP1P2");   
   

   for (e=0; e<GeomP2->NBELMT; e++)
   {      
      for (i=0; i<4; i++)
      {
         onP1[i].x = GeomP2->XYSOMM->me[GeomP1->NSELMT->im[e][i]][0];
         onP1[i].y = GeomP2->XYSOMM->me[GeomP1->NSELMT->im[e][i]][1];
         onP1[i].z = GeomP2->XYSOMM->me[GeomP1->NSELMT->im[e][i]][2];
      }
      
      for (i=0; i<10; i++)
      {
         onP2[i].x = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][i]][0];
         onP2[i].y = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][i]][1];
         onP2[i].z = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][i]][2];
      }

      
      if ( !check_pt_equal_pt( &onP1[0], &onP2[0] ) ) printf("elt %d : onP1[0] != onP2[0] \n",e);
      if ( !check_pt_equal_pt( &onP1[1], &onP2[1] ) ) printf("elt %d : onP1[1] != onP2[1] \n",e);
      if ( !check_pt_equal_pt( &onP1[2], &onP2[2] ) ) printf("elt %d : onP1[0] != onP2[0] \n",e);
      if ( !check_pt_equal_pt( &onP1[3], &onP2[3] ) ) printf("elt %d : onP1[1] != onP2[1] \n",e);
      
      if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[0],&onP2[1],&center), &onP2[4] ) ) printf("elt %d  onP2[4] \n", e);
      if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[0],&onP2[2],&center), &onP2[5] ) ) printf("elt %d  onP2[5] \n", e);
      if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[0],&onP2[3],&center), &onP2[6] ) ) printf("elt %d  onP2[6] \n", e);
      if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[2],&onP2[1],&center), &onP2[7] ) ) printf("elt %d  onP2[7] \n", e);
      if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[2],&onP2[3],&center), &onP2[8] ) ) printf("elt %d  onP2[8] \n", e);
      if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[1],&onP2[3],&center), &onP2[9] ) ) printf("elt %d  onP2[9] \n", e);

      /*
      printf("\t P2:node1=(%lf;%lf;%lf) \n", onP2[0].x , onP2[0].y ,onP2[0].z ); 
      printf("\t P1:node1=(%lf;%lf;%lf) \n", onP1[0].x , onP1[0].y ,onP1[0].z ); 

      printf("\t P2:node1=(%lf;%lf;%lf) \n", onP2[1].x , onP2[1].y ,onP2[1].z );
      printf("\t P1:node1=(%lf;%lf;%lf) \n", onP1[1].x , onP1[1].y ,onP1[1].z );

      printf("\t P2:node1=(%lf;%lf;%lf) \n", onP2[2].x , onP2[2].y ,onP2[2].z );
      printf("\t P1:node1=(%lf;%lf;%lf) \n", onP1[2].x , onP1[2].y ,onP1[2].z );

      printf("\t P2:node1=(%lf;%lf;%lf) \n", onP2[3].x , onP2[3].y ,onP2[3].z );
      printf("\t P1:node1=(%lf;%lf;%lf) \n", onP1[3].x , onP1[3].y ,onP1[3].z );
      */
   }
   
   printf(" GeomP1 and P2 checked \n");

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Parent_get_fromBase_P1toP2( GEOM_3D *GeomP2, const GEOM_3D *GeomP1)
{
   int e,a,i,j;
   int curr_idx;
   int NB_TOTAL_SIDE;
   
   IMAT *MID_POINTS;

   int n0,n1,n2,n3,n4,n5,n6,n7,n8,n9;

   if ( GeomP1 == NULL )  error(E_NULL, "Geom2DParent_get_fromBase_P1toP2");
   if ( GeomP2 == NULL )  error(E_NULL, "Geom2DParent_get_fromBase_P1toP2");

   /* copy ONLY SOME members of geomBase into Geom */
   strncpy(GeomP2->type,GeomP1->type, 16);
   GeomP2->type[15] = '\0';


   GeomP2->NBELMT = GeomP1->NBELMT;
   GeomP2->NSELMT = im_get(GeomP2->NBELMT, 10); /* To Fill */

   GeomP2->NBFACE = GeomP1->NBFACE;
   GeomP2->NSFACE = im_get(GeomP2->NBFACE,  6); /* To Fill */

   NB_TOTAL_SIDE = COEFF_P1_P2_NBSOMM*(GeomP1->NBELMT +10); /* estimation */

   GeomP2->NBSOMM = /* P1 somm */GeomP1->NBSOMM + /* new somm */ NB_TOTAL_SIDE;
   GeomP2->XYSOMM = m_get(GeomP2->NBSOMM,3);   /* To Fill */

   GeomP2->REF_A  = iv_get(GeomP2->NBFACE);  iv_copy(GeomP1->REF_A, GeomP2->REF_A);
   GeomP2->REF_S  = iv_get(GeomP2->NBSOMM);  /* To Fill : new sommets , new refs */
   GeomP2->REF_T  = iv_get(GeomP2->NBELMT);  iv_copy(GeomP1->REF_T, GeomP2->REF_T);

   iv_move(GeomP1->REF_S, 0,  GeomP1->NBSOMM, GeomP2->REF_S, 0 );
   
   /* init */
   curr_idx = GeomP1->NBSOMM;
   MID_POINTS = im_get(GeomP1->NBSOMM, GeomP1->NBSOMM);
   im_ones(MID_POINTS);
   im_smlt(MID_POINTS, -1, MID_POINTS); /* init to -1 */
   
   /* fill  the new NSELMT , XYSOMM, NSFACE, REF_T, */
   for (e=0; e<GeomP1->NBELMT; e++)
   {
      n0 = GeomP1->NSELMT->im[e][0];
      n1 = GeomP1->NSELMT->im[e][1];
      n2 = GeomP1->NSELMT->im[e][2];
      n3 = GeomP1->NSELMT->im[e][3];

      GeomP2->XYSOMM->me[n0][0] = GeomP1->XYSOMM->me[n0][0];
      GeomP2->XYSOMM->me[n0][1] = GeomP1->XYSOMM->me[n0][1];
      GeomP2->XYSOMM->me[n0][2] = GeomP1->XYSOMM->me[n0][2];
      
      GeomP2->XYSOMM->me[n1][0] = GeomP1->XYSOMM->me[n1][0];
      GeomP2->XYSOMM->me[n1][1] = GeomP1->XYSOMM->me[n1][1];
      GeomP2->XYSOMM->me[n1][2] = GeomP1->XYSOMM->me[n1][2];
      
      GeomP2->XYSOMM->me[n2][0] = GeomP1->XYSOMM->me[n2][0];
      GeomP2->XYSOMM->me[n2][1] = GeomP1->XYSOMM->me[n2][1];
      GeomP2->XYSOMM->me[n2][2] = GeomP1->XYSOMM->me[n2][2];
      
      GeomP2->XYSOMM->me[n3][0] = GeomP1->XYSOMM->me[n3][0];
      GeomP2->XYSOMM->me[n3][1] = GeomP1->XYSOMM->me[n3][1];
      GeomP2->XYSOMM->me[n3][2] = GeomP1->XYSOMM->me[n3][2];

      n4 = midpoint_built_or_get(e, n0, n1, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n5 = midpoint_built_or_get(e, n0, n2, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n6 = midpoint_built_or_get(e, n0, n3, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n7 = midpoint_built_or_get(e, n1, n2, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n8 = midpoint_built_or_get(e, n2, n3, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n9 = midpoint_built_or_get(e, n1, n3, MID_POINTS, GeomP2, GeomP1, &curr_idx);

      GeomP2->NSELMT->im[e][0] = n0;
      GeomP2->NSELMT->im[e][1] = n1;
      GeomP2->NSELMT->im[e][2] = n2;
      GeomP2->NSELMT->im[e][3] = n3;

      GeomP2->NSELMT->im[e][4] = n4;
      GeomP2->NSELMT->im[e][5] = n5;
      GeomP2->NSELMT->im[e][6] = n6;
      GeomP2->NSELMT->im[e][7] = n7;
      GeomP2->NSELMT->im[e][8] = n8;
      GeomP2->NSELMT->im[e][9] = n9;
   }

   for (a=0; a<GeomP1->NBFACE; a++)
   {
      n0 = GeomP1->NSFACE->im[a][0];
      n1 = GeomP1->NSFACE->im[a][1];
      n2 = GeomP1->NSFACE->im[a][2];

      /* check n0, n1, n2 */
      if ( (n0 == -1)||((n1 == -1))||((n2 == -1)) )
      {
         printf("n0 = %d\n",n0);
         printf("n1 = %d\n",n1);
         printf("n2 = %d\n",n2);

         error(E_UNKNOWN, "Geom3D_Parent_get_fromBase_P1toP2");
      }

      /* */
      n3 = MID_POINTS->im[n0][n1];
      n4 = MID_POINTS->im[n1][n2];
      n5 = MID_POINTS->im[n0][n2];

      /* check n3, n4, n5 */
      if ( (n3 == -1)||((n4 == -1))||((n5 == -1)) )
      {
         printf("n3 = %d\n",n3);
         printf("n4 = %d\n",n4);
         printf("n5 = %d\n",n5);

         error(E_UNKNOWN, "Geom3D_Parent_get_fromBase_P1toP2");
      }

      GeomP2->NSFACE->im[a][0] = n0;
      GeomP2->NSFACE->im[a][1] = n1;
      GeomP2->NSFACE->im[a][2] = n2;
      GeomP2->NSFACE->im[a][3] = n3;
      GeomP2->NSFACE->im[a][4] = n4;
      GeomP2->NSFACE->im[a][5] = n5;
   }

   GeomP2->NBSOMM = curr_idx;
   {
      MAT * XYSOMM = m_get(GeomP2->NBSOMM,3);   /* To Fill */
      
      for (i=0; i<GeomP2->NBSOMM; i++)
      for (j=0; j<3; j++)
      {
         XYSOMM->me[i][j] = GeomP2->XYSOMM->me[i][j];
      }

      M_FREE(GeomP2->XYSOMM);
      GeomP2->XYSOMM = XYSOMM; 
   }

   GeomP2->REF_S  = iv_resize(GeomP2->REF_S, GeomP2->NBSOMM);  /* xxx */

   /* free stuff */
   IM_FREE(MID_POINTS);

   printf ("P2 generation: NBELMT = %d   NBSOMM = %d    NBFACE = %d \n", GeomP2->NBELMT, GeomP2->NBSOMM, GeomP2->NBFACE);
   /* -------------------- */
   /* check the 2 meshes   */
   /* -------------------- */
   Geom3D_checkmeshesP1P2 ( GeomP1 , GeomP2);

   /* end */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Parent_get_fromBase_P1toP1b( GEOM_3D *GeomP1b, const GEOM_3D *GeomP1)
{
   int e;
   
   if ( GeomP1  == NULL )  error(E_NULL, "Geom3DParent_get_fromBase_P1toP2");
   if ( GeomP1b == NULL )  error(E_NULL, "Geom3DParent_get_fromBase_P1toP2");
    

   strncpy(GeomP1b->type, GeomP1->type, 16);
   GeomP1b->type[15] = '\0';

   GeomP1b->NBSOMM  = GeomP1->NBSOMM + GeomP1->NBELMT;
   GeomP1b->NBELMT  = GeomP1->NBELMT;
   GeomP1b->NBFACE  = GeomP1->NBFACE;

   
   GeomP1b->REF_S = iv_get(GeomP1b->NBSOMM);  /* references on the nodes : new dimension */
   iv_zero(GeomP1b->REF_S);                   /* new somms are interiors pts             */
   iv_move(GeomP1->REF_S, 0,GeomP1->REF_S->dim, GeomP1b->REF_S, 0);

   GeomP1b->REF_T = iv_get(GeomP1->REF_T->dim); iv_copy(GeomP1->REF_T, GeomP1b->REF_T);
   GeomP1b->REF_A = iv_get(GeomP1->REF_A->dim); iv_copy(GeomP1->REF_A, GeomP1b->REF_A);
   
   GeomP1b->XYSOMM  = m_get(GeomP1b->NBSOMM,3) ;   /* nodes's coordinates array : 4 somm per elt */
   GeomP1b->NSELMT  = im_get(GeomP1b->NBELMT,5);   /* elements's nodes arrays   */

   /* init-fill GeomP1b->XYSOMM */
   m_move(GeomP1->XYSOMM, 0,0,GeomP1->NBSOMM,3, GeomP1b->XYSOMM, 0,0);
   /* fill GeomP1b->XYSOMM & GeomP1b->NSELMT */
   for (e=0; e<GeomP1->NBELMT; e++)
   {
      double val = 1.0/3.0;
      
      double x1 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][0] ][0] ;
      double x2 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][1] ][0] ;
      double x3 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][2] ][0] ;
      double x4 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][3] ][0] ;

      double y1 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][0] ][1] ;
      double y2 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][1] ][1] ;
      double y3 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][2] ][1] ;
      double y4 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][3] ][1] ;

      double z1 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][0] ][2] ;
      double z2 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][1] ][2] ;
      double z3 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][2] ][2] ;
      double z4 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][3] ][2] ;
      
      GeomP1b->NSELMT->im[e][0] = GeomP1->NSELMT->im[e][0];
      GeomP1b->NSELMT->im[e][1] = GeomP1->NSELMT->im[e][1];
      GeomP1b->NSELMT->im[e][2] = GeomP1->NSELMT->im[e][2];
      GeomP1b->NSELMT->im[e][3] = GeomP1->NSELMT->im[e][3];

      GeomP1b->NSELMT->im[e][4] = e + GeomP1->NBSOMM;

      GeomP1b->XYSOMM->me[e + GeomP1->NBSOMM][0] = x1 + (x2-x1)*val + (x3-x1)*val + (x4-x1)*val;
      GeomP1b->XYSOMM->me[e + GeomP1->NBSOMM][1] = y1 + (y2-y1)*val + (y3-y1)*val + (y4-y1)*val;
      GeomP1b->XYSOMM->me[e + GeomP1->NBSOMM][2] = z1 + (z2-z1)*val + (z3-z1)*val + (z4-z1)*val;
   }

   GeomP1b->NSFACE  = im_get(GeomP1->NSFACE->m, GeomP1->NSFACE->n); im_copy(GeomP1->NSFACE, GeomP1b->NSFACE);
   
   /* -------------------------- */
   /* check the 2 meshes         */
   /* -------------------------- */
   Geom3D_checkmeshesP1P1b(GeomP1b, GeomP1);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom3D_Base_check(const GEOM_3D *Geom)
{
   /* a gmsh mesh not "corrected"  ? */
   int NBSOMM = Geom->NBSOMM;
   int NBELMT = Geom->NBELMT;
   int NBSOMM_ELT = 4;
   
   int i,e,idx;

   for (i=0; i<NBELMT; i++)
   for (e=0; e<NBSOMM_ELT; e++)
   {
      idx = Geom->NSELMT->im[e][i];

      if ( idx <  0 )      error3(E_GEOM_GMSH_WRONG_IDX, "Geom3D_Base_check");
      if ( idx >= NBSOMM ) error3(E_GEOM_GMSH_WRONG_IDX, "Geom3D_Base_check");
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int midpoint_built_or_get(int e, int a, int b, IMAT *MID_POINTS, GEOM_3D *GeomP2, const GEOM_3D *GeomP1, int *curr_idx)
{
   int midpoint = MID_POINTS->im[a][b];

   if ( -1 != midpoint )
   {
      return midpoint;
   }
   else
   {
      return midpoint_built_and_fill_geom(a,b, MID_POINTS, GeomP2, GeomP1, curr_idx);
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int midpoint_built_and_fill_geom(int a, int b, IMAT *MID_POINTS, GEOM_3D *GeomP2, const GEOM_3D *GeomP1, int *curr_idx)
{
   double xa,ya,za;
   double xb,yb,zb;
   double xm,ym,zm;
   int new_midpoint = (*curr_idx);
   int ref_on_face;
   
   if ( new_midpoint >= GeomP2->XYSOMM->m )
   {
      error3(E_GEOM_3D_NBSOMM_P2_TOO_SMALL, "midpoint_built_and_fill_geom");
   }

   xa = GeomP1->XYSOMM->me[a][0];
   ya = GeomP1->XYSOMM->me[a][1];
   za = GeomP1->XYSOMM->me[a][2];

   xb = GeomP1->XYSOMM->me[b][0];
   yb = GeomP1->XYSOMM->me[b][1];
   zb = GeomP1->XYSOMM->me[b][2];

   xm = (xa+xb)/2.0;
   ym = (ya+yb)/2.0;
   zm = (za+zb)/2.0;

   GeomP2->XYSOMM->me[new_midpoint][0] = xm;
   GeomP2->XYSOMM->me[new_midpoint][1] = ym;
   GeomP2->XYSOMM->me[new_midpoint][2] = zm;

   /* Now the reference on the border somm : NSFACE + REF_S */
   if ( (GeomP1->REF_S->ive[a] == 0) || (GeomP1->REF_S->ive[b] == 0) )
   {
      GeomP2->REF_S->ive[new_midpoint] = 0;
   }
   else
   if ( (GeomP1->REF_S->ive[a] > 0) && (GeomP1->REF_S->ive[b] > 0) && (GeomP1->REF_S->ive[a] == GeomP1->REF_S->ive[b]) )
   {
      GeomP2->REF_S->ive[new_midpoint] = GeomP1->REF_S->ive[a] ;
   }
   /*
   else
   if ( (GeomP1->REF_S->ive[a] > 0) && (GeomP1->REF_S->ive[b] > 0) )
   {
      GeomP2->REF_S->ive[new_midpoint] = GeomP1->REF_S->ive[a] ;
   }*/
   else
   {
      ref_on_face = check_two_somm_on_a_face(GeomP1, a,b);

      if ( ref_on_face == 0 )
      {
         GeomP2->REF_S->ive[new_midpoint] = 0;
      }
      else
      {
         GeomP2->REF_S->ive[new_midpoint] = ref_on_face;
      }
   }

   /* fill MID_POINTS */
   MID_POINTS->im[a][b] = new_midpoint;
   MID_POINTS->im[b][a] = new_midpoint;

   /* increment by 1 */;
   (++(*curr_idx));

   if ( *curr_idx > GeomP2->NBSOMM )
   {
      error3(E_GEOM_3D_NBSOMM_P2_TOO_SMALL, "midpoint_built_and_fill_geom");
   }

   /* finito */
   return new_midpoint;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int check_two_somm_on_a_face(const GEOM_3D *GeomP1, int a, int b)
{
   int aa;
   int s0,s1,s2;
   int count;
   
   for (aa=0; aa<GeomP1->NBFACE; aa++)
   {
      s0 = GeomP1->NSFACE->im[aa][0];
      s1 = GeomP1->NSFACE->im[aa][1];
      s2 = GeomP1->NSFACE->im[aa][2];

      count = 0;

      if ( (a==s0)||(a==s1)||(a==s2) )
      {
         ++count;
      }
      if ( (b==s0)||(b==s1)||(b==s2) )
      {
         ++count;
      }

      if ( count==2 )
      {
         return GeomP1->REF_A->ive[aa] ; /* the 2 somm a and b belong to a same face */
      }
   }

   /*
   printf("check_two_somm_on_a_face : somm %d (%lf ; %lf ; %lf) ; ref %d\n", a,
          GeomP1->XYSOMM->me[a][0], GeomP1->XYSOMM->me[a][1], GeomP1->XYSOMM->me[a][2], GeomP1->REF_S->ive[a]);
   printf("check_two_somm_on_a_face : somm %d (%lf ; %lf ; %lf) ; ref %d\n", b,
          GeomP1->XYSOMM->me[b][0], GeomP1->XYSOMM->me[b][1], GeomP1->XYSOMM->me[b][2], GeomP1->REF_S->ive[b]);

   printf("check_two_somm_on_a_face : but interior point somm %d (%lf ; %lf ; %lf) ; ref 00\n", new_midpoint,
          GeomP2->XYSOMM->me[new_midpoint][0], GeomP2->XYSOMM->me[new_midpoint][1], GeomP2->XYSOMM->me[new_midpoint][2]);
   */


   return 0; /* the 2 somm a and b do not belong to a same face */
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

GEOM_3D *Geom3D_getP1geom_from( const ELT_3D *element, const GEOM_3D* geom )
{
   GEOM_3D *GeomP1     = NULL;

   if ( element  == NULL )  error(E_NULL, "Geom3D_getP1geom_from");
   if ( geom     == NULL )  error(E_NULL, "Geom3D_getP1geom_from");

   if ( (strcmp(element->name_ef,"P2")==0 ) )
   {        
      GeomP1 = Geom3D_getP1geom_from_P2geom( element, geom);    
   }
   else
   if ( (strcmp(element->name_ef,"P3")==0 ) )
   {        
      GeomP1 = Geom3D_getP1geom_from_P3geom( element, geom); 
   }
   else
   {
      warning3(W_GEOM_ALREADY_P1, "Geom3D_getP1geom_from");
      return NULL;
   }

   return GeomP1;
   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

VEC *Geom3D_extract_P1geom_VEC_from(const ELT_3D *element, const GEOM_3D* geom, const VEC *v)
{
   VEC *w  = NULL;

   if ( element  == NULL )  error(E_NULL, "Geom3D_extract_P1geom_VEC_from");
   if ( geom     == NULL )  error(E_NULL, "Geom3D_extract_P1geom_VEC_from");
   if ( v        == NULL )  error(E_NULL, "Geom3D_extract_P1geom_VEC_from");

   if ( (strcmp(element->name_ef,"P2")==0 ) )
   {
      w = Geom3D_extract_P1geom_VEC_from_P2geom_VEC(element, geom, v);
   }
   else
   if ( (strcmp(element->name_ef,"P3")==0 ) )
   {
      w = Geom3D_extract_P1geom_VEC_from_P3geom_VEC(element, geom, v);
   }
   else
   {
      warning3(W_GEOM_ALREADY_P1, "Geom3D_getP1geom_from");
      return NULL;
   }

   return w;

}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_3D *Geom3D_getP1geom_from_P2geom( const ELT_3D *element, const GEOM_3D* GeomP2 )
{
   int  e,a,s;
   int  n0,n1,n2;
   
   double xm,x0,x1,x2,x3;
   double ym,y0,y1,y2,y3;
   double zm,z0,z1,z2,z3;

   GEOM_3D* GeomP1;

   if ( element == NULL ) error(E_NULL, "Geom3D_getP1geom_from_P2geom");  
   if ( GeomP2  == NULL ) error(E_NULL, "Geom3D_getP1geom_from_P2geom");

   /* --------- create the structure GEOM_3D ----------- */
   if ( (GeomP1=NEW(GEOM_3D))== (GEOM_3D *)NULL )
   {
      error(E_MEM,"Geom3D_getP1geom_from_P2geom");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_3D, 0, sizeof(GEOM_3D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_3D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(GeomP1->type, GeomP2->type, 16);
   GeomP1->type[15] = '\0';

   /* fill GeomP1 */
   GeomP1->NBSOMM =    GeomP2->NBSOMM + GeomP2->NBELMT ; /* same number of somms + a somm in the middle of each cell */
   GeomP1->NBELMT = 12*GeomP2->NBELMT; /* 1 P2-tetrahedra -> 12 P1-tetrahedras from the subdivision       */
   GeomP1->NBFACE =  4*GeomP2->NBFACE; /* 1 P2-face       ->  4 P1-faces(triangles) from the subdivision  */


   GeomP1->REF_S = iv_get(GeomP1->NBSOMM);
   GeomP1->REF_T = iv_get(GeomP1->NBELMT); 
   GeomP1->REF_A = iv_get(GeomP1->NBFACE); 

   GeomP1->XYSOMM = m_get(GeomP1->NBSOMM, 3);
   GeomP1->NSELMT = im_get(GeomP1->NBELMT, 4);
   GeomP1->NSFACE = im_get(GeomP1->NBFACE, 3);

   for (e=0; e<GeomP2->NBELMT; e++)
   {
      GeomP1->NSELMT->im[12*e+0][0] = GeomP2->NSELMT->im[e][6] ; /* "top-corner" tetrahedra */
      GeomP1->NSELMT->im[12*e+0][1] = GeomP2->NSELMT->im[e][9] ;
      GeomP1->NSELMT->im[12*e+0][2] = GeomP2->NSELMT->im[e][8] ;
      GeomP1->NSELMT->im[12*e+0][3] = GeomP2->NSELMT->im[e][3] ;

      GeomP1->NSELMT->im[12*e+1][0] = GeomP2->NSELMT->im[e][6] ; /* "top-corner" sym tetrahedra */
      GeomP1->NSELMT->im[12*e+1][1] = GeomP2->NSELMT->im[e][8] ;
      GeomP1->NSELMT->im[12*e+1][2] = GeomP2->NSELMT->im[e][9] ;
      GeomP1->NSELMT->im[12*e+1][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+2][0] = GeomP2->NSELMT->im[e][9] ; /* "right-corner" tetrahedra */
      GeomP1->NSELMT->im[12*e+2][1] = GeomP2->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[12*e+2][2] = GeomP2->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[12*e+2][3] = GeomP2->NSELMT->im[e][1] ;

      GeomP1->NSELMT->im[12*e+3][0] = GeomP2->NSELMT->im[e][9] ; /* "right-corner" sym tetrahedra */
      GeomP1->NSELMT->im[12*e+3][1] = GeomP2->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[12*e+3][2] = GeomP2->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[12*e+3][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+4][0] = GeomP2->NSELMT->im[e][8] ; /* "far-corner" tetrahedra */
      GeomP1->NSELMT->im[12*e+4][1] = GeomP2->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[12*e+4][2] = GeomP2->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[12*e+4][3] = GeomP2->NSELMT->im[e][2] ;

      GeomP1->NSELMT->im[12*e+5][0] = GeomP2->NSELMT->im[e][8] ; /* "far-corner" sym tetrahedra */
      GeomP1->NSELMT->im[12*e+5][1] = GeomP2->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[12*e+5][2] = GeomP2->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[12*e+5][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+6][0] = GeomP2->NSELMT->im[e][4] ; /* "front-corner" tetrahedra */
      GeomP1->NSELMT->im[12*e+6][1] = GeomP2->NSELMT->im[e][6] ;
      GeomP1->NSELMT->im[12*e+6][2] = GeomP2->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[12*e+6][3] = GeomP2->NSELMT->im[e][0] ;

      GeomP1->NSELMT->im[12*e+7][0] = GeomP2->NSELMT->im[e][4] ; /* "front-corner" sym tetrahedra */
      GeomP1->NSELMT->im[12*e+7][1] = GeomP2->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[12*e+7][2] = GeomP2->NSELMT->im[e][6] ;
      GeomP1->NSELMT->im[12*e+7][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+8][0] = GeomP2->NSELMT->im[e][4] ; /* "bottom" middle tetrahedra */
      GeomP1->NSELMT->im[12*e+8][1] = GeomP2->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[12*e+8][2] = GeomP2->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[12*e+8][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+9][0] = GeomP2->NSELMT->im[e][6] ; /* "front" middle tetrahedra */
      GeomP1->NSELMT->im[12*e+9][1] = GeomP2->NSELMT->im[e][9] ;
      GeomP1->NSELMT->im[12*e+9][2] = GeomP2->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[12*e+9][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+10][0] = GeomP2->NSELMT->im[e][8] ; /* "left" middle tetrahedra */
      GeomP1->NSELMT->im[12*e+10][1] = GeomP2->NSELMT->im[e][6] ;
      GeomP1->NSELMT->im[12*e+10][2] = GeomP2->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[12*e+10][3] = GeomP2->NBSOMM + e;

      GeomP1->NSELMT->im[12*e+11][0] = GeomP2->NSELMT->im[e][9] ; /* "right" middle tetrahedra */
      GeomP1->NSELMT->im[12*e+11][1] = GeomP2->NSELMT->im[e][8] ;
      GeomP1->NSELMT->im[12*e+11][2] = GeomP2->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[12*e+11][3] = GeomP2->NBSOMM + e;

      /* the references */
      GeomP1->REF_T->ive[12*e+ 0]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 1]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 2]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 3]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 4]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 5]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 6]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 7]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 8]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+ 9]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+10]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[12*e+11]    = GeomP2->REF_T->ive[e];
   }

   /* the P2 face has 6 somms -> make of then 4 P1-faces with the 6 somms */
   for (a=0; a<GeomP2->NBFACE; a++)
   {
      /* 1rst sub-face */
      n0 = GeomP2->NSFACE->im[a][0] ;
      n1 = GeomP2->NSFACE->im[a][3] ;
      n2 = GeomP2->NSFACE->im[a][5] ;

      GeomP1->NSFACE->im[4*a+0][0] = n0 ;
      GeomP1->NSFACE->im[4*a+0][1] = n1 ;
      GeomP1->NSFACE->im[4*a+0][2] = n2 ;
      
      /* 2nd sub-face */
      n0 = GeomP2->NSFACE->im[a][1] ;
      n1 = GeomP2->NSFACE->im[a][4] ;
      n2 = GeomP2->NSFACE->im[a][3] ;

      GeomP1->NSFACE->im[4*a+1][0] = n0 ;
      GeomP1->NSFACE->im[4*a+1][1] = n1 ;
      GeomP1->NSFACE->im[4*a+1][2] = n2 ;

      /* 3th sub-face */
      n0 = GeomP2->NSFACE->im[a][5] ;
      n1 = GeomP2->NSFACE->im[a][4] ;
      n2 = GeomP2->NSFACE->im[a][2] ;

      GeomP1->NSFACE->im[4*a+2][0] = n0 ;
      GeomP1->NSFACE->im[4*a+2][1] = n1 ;
      GeomP1->NSFACE->im[4*a+2][2] = n2 ;

      /* 4th sub-face */
      n0 = GeomP2->NSFACE->im[a][3] ;
      n1 = GeomP2->NSFACE->im[a][4] ;
      n2 = GeomP2->NSFACE->im[a][5] ;

      GeomP1->NSFACE->im[4*a+3][0] = n0 ;
      GeomP1->NSFACE->im[4*a+3][1] = n1 ;
      GeomP1->NSFACE->im[4*a+3][2] = n2 ;

      
      /* the references */
      GeomP1->REF_A->ive[4*a+0]    = GeomP2->REF_A->ive[a];
      GeomP1->REF_A->ive[4*a+1]    = GeomP2->REF_A->ive[a];
      GeomP1->REF_A->ive[4*a+2]    = GeomP2->REF_A->ive[a];
      GeomP1->REF_A->ive[4*a+3]    = GeomP2->REF_A->ive[a];
   }

   /* the existing nodes */
   for (s=0; s<GeomP2->NBSOMM; s++)
   {
      GeomP1->XYSOMM->me[s][0] = GeomP2->XYSOMM->me[s][0] ;
      GeomP1->XYSOMM->me[s][1] = GeomP2->XYSOMM->me[s][1] ;
      GeomP1->XYSOMM->me[s][2] = GeomP2->XYSOMM->me[s][2] ;

      /* the references */
      GeomP1->REF_S->ive[s] = GeomP2->REF_S->ive[s];
   }
   /* the new nodes */
   for (e=0; e<GeomP2->NBELMT; e++)
   {
       x0 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][0]][0];
       y0 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][0]][1];
       z0 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][0]][2];

       x1 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][1]][0];
       y1 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][1]][1];
       z1 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][1]][2];

       x2 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][2]][0];
       y2 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][2]][1];
       z2 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][2]][2];

       x3 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][3]][0];
       y3 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][3]][1];
       z3 = GeomP1->XYSOMM->me[GeomP2->NSELMT->im[e][3]][2];


       xm = (x0+x1+x2+x3) / 4.0 ;
       ym = (y0+y1+y2+y3) / 4.0 ;
       zm = (z0+z1+z2+z3) / 4.0 ;

       GeomP1->XYSOMM->me[GeomP2->NBSOMM+e][0] = xm; /* the middle of the cell */
       GeomP1->XYSOMM->me[GeomP2->NBSOMM+e][1] = ym; /* the middle of the cell */
       GeomP1->XYSOMM->me[GeomP2->NBSOMM+e][2] = zm; /* the middle of the cell */
   }

   printf("Geom3D_getP1geom_from_P2geom... END\n");

   return GeomP1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_3D *Geom3D_getP1geom_from_P3geom( const ELT_3D *element, const GEOM_3D* GeomP3 )
{
   GEOM_3D* GeomP1; /* Hou-la ! nearly impossible ... */

   error3(E_GEOM_TYPENOTIMPL, "Geom3D_get_fromParent_P3toP1");

   if ( element == NULL ) error(E_NULL, "Geom3D_get_fromParent_P3toP1");  
   if ( GeomP3  == NULL ) error(E_NULL, "Geom3D_get_fromParent_P3toP1");
   
   return NULL;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC *Geom3D_extract_P1geom_VEC_from_P2geom_VEC(const ELT_3D *element, const GEOM_3D* GeomP2, const VEC* v)
{
   int n,e,i;

   double xm,x0,x1,x2,x3;
   double ym,y0,y1,y2,y3;
   double zm,z0,z1,z2,z3;

   double val;

   int NBSOMM = GeomP2->NBSOMM + GeomP2->NBELMT ; /* same number of somms + a somm in the middle of each cell */

   VEC *interp = v_get(NBSOMM);

   PERM *px = px_get(3);
   MAT  *A  = m_get(3,3);

   VEC *r   = v_get(3);
   VEC *s   = v_get(3);

   /* init */
   v_zero(interp);

   /* the existing nodes */
   for (n=0; n<GeomP2->NBSOMM; n++)
   {
      interp->ve[n] = v->ve[n];
   }

   /* the new nodes */
   for (e=0; e<GeomP2->NBELMT; e++)
   {
      x0 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][0]][0];
      y0 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][0]][1];
      z0 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][0]][2];

      x1 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][1]][0];
      y1 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][1]][1];
      z1 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][1]][2];

      x2 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][2]][0];
      y2 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][2]][1];
      z2 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][2]][2];

      x3 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][3]][0];
      y3 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][3]][1];
      z3 = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][3]][2];

      xm = (x0+x1+x2+x3) / 4.0 ;
      ym = (y0+y1+y2+y3) / 4.0 ;
      zm = (z0+z1+z2+z3) / 4.0 ;

      /*
      
      x = x1 + (x2-x1)*ksi + (x3-x1)*eta + (x4-x1)*phi
      y = y1 + (y2-y1)*ksi + (y3-y1)*eta + (y4-y1)*phi
      z = z1 + (z2-z1)*ksi + (z3-z1)*eta + (z4-z1)*phi

      => As = r

      =>  s = inv(A)*r ; 
     
      */

      r->ve[0] = xm - x0 ;
      r->ve[1] = ym - y0 ;
      r->ve[2] = zm - z0 ;

      A->me[0][0] = x1-x0; 
      A->me[0][1] = x2-x0; 
      A->me[0][2] = x3-x0; 

      A->me[1][0] = y1-y0;
      A->me[1][1] = y2-y0;
      A->me[1][2] = y3-y0;

      A->me[2][0] = z1-z0;
      A->me[2][1] = z2-z0;
      A->me[2][2] = z3-z0;

      A = LUfactor(A, px) ; 
      s = LUsolve(A, px, r, s);

      /* the value on the centroid interpolated from the values on the nodes */
      val = 0.0;
      /* the value on the centroid interpolated from the values on the nodes */

      for (i=0; i<element->nb_somm_cell; i++)
      {
         val += v->ve[GeomP2->NSELMT->im[e][i]] * element->f_base[i](s->ve[0], s->ve[1], s->ve[2]);
      }

      interp->ve[GeomP2->NBSOMM + e] = val;
   }

   V_FREE(r);
   V_FREE(s);
   M_FREE(A);
   PX_FREE(px);

   return interp;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static VEC *Geom3D_extract_P1geom_VEC_from_P3geom_VEC(const ELT_3D *element, const GEOM_3D* geom, const VEC* v)
{
    /* Hou-la ! nearly impossible ... */
    return NULL;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/


static int Geom3D_get_nb_ref_on_somm(const GEOM_3D* Geom)
{
   int s,k;
   int nb_refs = 0;

   /* count the nb of reference on somm */
   int ref_s[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

   for ( s=0; s<Geom->NBSOMM; s++)
   {
      if ( Geom->REF_S->ive[s] > 0 )
      {
         /* is the ref already considered ? */
         for (k=0; k<nb_refs; k++)
         {
            if ( Geom->REF_S->ive[s] == ref_s[k] )
            {
               break;
            }
         }

         if ( k == nb_refs )
         {
            ref_s[k] = Geom->REF_S->ive[s];
            nb_refs += 1;
         }
      }
   }

   return nb_refs;
}

static int Geom3D_get_nb_ref_on_elts(const GEOM_3D *Geom)
{
   int e,k;
   int nb_refs = 0;

   /* count the nb of reference on somm */
   int ref_e[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


   for ( e=0; e<Geom->NBELMT; e++)
   {
      if ( Geom->REF_T->ive[e] > 0 )
      {
         /* is the ref already considered ? */
         for (k=0; k<nb_refs; k++)
         {
            if ( Geom->REF_T->ive[e] == ref_e[k] )
            {
               break;
            }
         }

         if ( k == nb_refs )
         {
            ref_e[k] = Geom->REF_T->ive[e];
            nb_refs += 1;
         }
      }
   }

   return nb_refs;
}

static int Geom3D_get_nb_ref_on_face(const GEOM_3D *Geom)
{
   int a,k;
   int nb_refs = 0;

   /* count the nb of reference on face */
   int ref_a[20] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };


   for ( a=0; a<Geom->NBFACE; a++)
   {
      if ( Geom->REF_A->ive[a] > 0 )
      {
         /* is the ref already considered ? */
         for (k=0; k<nb_refs; k++)
         {
            if ( Geom->REF_A->ive[a] == ref_a[k] )
            {
               break;
            }
         }

         if ( k == nb_refs )
         {
            ref_a[k] = Geom->REF_A->ive[a];
            nb_refs += 1;
         }
      }
   }

   return nb_refs;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int  Geom3D_check_with_boundaryconditions( const GEOM_3D *MyGeom , const BC_3D *MyBC , const int axe )
{
   char axe_name[8];
   int i;
   
   int nb_refs_on_somm ;
   int nb_refs_on_face ;

   if ( MyBC   == ( BC_3D*)NULL   ) error(E_NULL, "Geom3D_check_with_boundaryconditions");
   if ( MyGeom == ( GEOM_3D*)NULL ) error(E_NULL, "Geom3D_check_with_boundaryconditions");

   switch(axe)
   {
      case AXEe_X: strcpy(axe_name, "AXEe_X"); break;
      case AXEe_Y: strcpy(axe_name, "AXEe_Y"); break;
      case AXEe_Z: strcpy(axe_name, "AXEe_Z"); break;

      default: error(E_UNKNOWN, "Geom3D_check_with_boundaryconditions");
   }

   /*
    * Check BC compatibility
    */

   Bc3D_checkBC(MyBC, axe);

   /*
    * Check compatibility between the number of references
    * given in the geometry and the number of functions
    * to handle theses references in MyParams->bc_conditions
    */

   nb_refs_on_somm = Geom3D_get_nb_ref_on_somm(MyGeom);
   nb_refs_on_face = Geom3D_get_nb_ref_on_face(MyGeom);

   printf("\t Geometry->nb_ref_on_somm = %d \n", nb_refs_on_somm);
   printf("\t Geometry->nb_ref_on_face = %d \n", nb_refs_on_face);

   for (i=1; i<=nb_refs_on_somm; i++)
   {
      if ( BC_3De_UNKNOWN == Bc3D_getBcType(MyBC, axe, i) )
      {
         error3(E_GEOM_INCOMPATIBLE_WITH_BC, "Geom3D_check_with_boundaryconditions");
         return EXIT_FAILURE;
      }
   }

   for (i=1; i<=nb_refs_on_face; i++)
   {
      if ( BC_3De_UNKNOWN == Bc3D_getBcType(MyBC, axe, i) )
      {
         error3(E_GEOM_INCOMPATIBLE_WITH_BC, "Geom3D_check_with_boundaryconditions");
         return EXIT_FAILURE;
      }
   }

   return EXIT_SUCCESS;
}   

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

GEOM_3D *Geom3D_get_base_tetrahedra(int NX)
{
   GEOM_3D *Geom;
   GEOM_3D *Geom_P2;

   ELT_3D  *elt_P2 = elt3D_get("P2");

   int i;

   /* --------- create the structure GEOM_3D ----------- */
   if ( (Geom=NEW(GEOM_3D)) == (GEOM_3D *)NULL )
   {
      error(E_MEM, "GEOM_3D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_3D, 0, sizeof(GEOM_3D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_3D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(Geom->type,"tetra", 16);

   /* I- the unit tetrahedra */

   /* we have */
   Geom->NBSOMM = 4;  
   /* we have */
   Geom->NBELMT = 1;
   /* we have */
   Geom->NBFACE = 4;

   /* allocate memory */
   Geom->XYSOMM =  m_get(Geom->NBSOMM,3);
   Geom->NSELMT = im_get(Geom->NBELMT,4);
   Geom->NSFACE = im_get(Geom->NBFACE,3) ;

   Geom->REF_S = iv_get(Geom->NBSOMM);  iv_zero(Geom->REF_S);
   Geom->REF_T = iv_get(Geom->NBELMT);  iv_zero(Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE);  iv_zero(Geom->REF_A);

   /* fill XYSOMM */
   Geom->XYSOMM->me[0][0] = 0.0;
   Geom->XYSOMM->me[0][1] = 0.0;
   Geom->XYSOMM->me[0][2] = 0.0;

   Geom->XYSOMM->me[1][0] = 1.0;
   Geom->XYSOMM->me[1][1] = 0.0;
   Geom->XYSOMM->me[1][2] = 0.0;

   Geom->XYSOMM->me[2][0] = 0.5;
   Geom->XYSOMM->me[2][1] = sqrt(3.0)/2.0;
   Geom->XYSOMM->me[2][2] = 0.0;

   Geom->XYSOMM->me[3][0] = 0.0;
   Geom->XYSOMM->me[3][1] = 0.0;
   Geom->XYSOMM->me[3][2] = 1.0;

   /* fill NSELMT */
   Geom->NSELMT->im[0][0] = 0;
   Geom->NSELMT->im[0][1] = 1;
   Geom->NSELMT->im[0][2] = 2;
   Geom->NSELMT->im[0][3] = 3;

   /* fill NSFACE */
   Geom->NSFACE->im[0][0] = 0;
   Geom->NSFACE->im[0][1] = 1;
   Geom->NSFACE->im[0][2] = 3;

   Geom->NSFACE->im[1][0] = 0;
   Geom->NSFACE->im[1][1] = 3;
   Geom->NSFACE->im[1][2] = 2;

   Geom->NSFACE->im[2][0] = 0;
   Geom->NSFACE->im[2][1] = 2;
   Geom->NSFACE->im[2][2] = 1;

   Geom->NSFACE->im[3][0] = 1;
   Geom->NSFACE->im[3][1] = 2;
   Geom->NSFACE->im[3][2] = 3;


   /* fill REF_T */
   Geom->REF_T->ive[0] = 0;

   /* fill REF_S */
   Geom->REF_S->ive[0] = 1;
   Geom->REF_S->ive[1] = 1;
   Geom->REF_S->ive[2] = 1;
   Geom->REF_S->ive[3] = 1;

   /* fill REF_A */
   Geom->REF_A->ive[0] = 0;
   Geom->REF_A->ive[1] = 0;
   Geom->REF_A->ive[2] = 0;
   Geom->REF_A->ive[3] = 0;


   /* II - now refine by subdivising */
   for ( i=0; i<NX; i++)
   {
      Geom_P2 = Geom3D_Parent_get_fromBase("P2", Geom);
      Geom_P2->geomBase = NULL;
      GEOM_3D_FREE(Geom);

      Geom = Geom3D_getP1geom_from(elt_P2, Geom_P2);
      GEOM_3D_FREE(Geom_P2);
   }

   /* free mem */
   ELT_3D_FREE(elt_P2);

   /* end */
   return Geom;
}

