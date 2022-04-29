
/** \file geometry_1D.c
 *
 * Contains the structure of the geometry in 1 dimension
 *
 */

#include <stdlib.h>

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/splines_1D.h"

#include "MESCHACH_EF/INCLUDES/all_params_json_utils.h"

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_segments(VEC* H1, Real xmin, Real xmax, const char* distribution_type);
static GEOM_1D* Geom1D_get_from_segments(const ELT_1D *elt, const VEC* H1, Real X_LEFT, Real X_RIGHT, MESH_PERIODICITY periodicity);

static void     Geom1D_get_P1geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P2geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P3geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_H3geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S2geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S3geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S5geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);

static int      Geom1D_get_nb_ref_on_somm(const GEOM_1D* Geom);
static int      Geom1D_get_nb_ref_on_elts(const GEOM_1D *Geom);
static int      Geom1D_get_nb_ref_on_face(const GEOM_1D *Geom);

static GEOM_1D *Geom1D_copy_from(const GEOM_1D* GeomP1);

static GEOM_1D *Geom1D_get_fromParent_P2toP1( const ELT_1D *element, const GEOM_1D* geom );
static GEOM_1D *Geom1D_get_fromParent_P3toP1( const ELT_1D *element, const GEOM_1D* geom );
static GEOM_1D *Geom1D_get_fromParent_H3toP1( const ELT_1D *element, const GEOM_1D* geom );
static GEOM_1D *Geom1D_get_fromParent_S2toP1( const ELT_1D *element, const GEOM_1D* geom );
static GEOM_1D *Geom1D_get_fromParent_S3toP1( const ELT_1D *element, const GEOM_1D* geom );
static GEOM_1D *Geom1D_get_fromParent_S5toP1( const ELT_1D *element, const GEOM_1D* geom );


static void     Geom1D_get_P1geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P2geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P3geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_H3geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S2geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S3geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S5geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);

static void     Geom1D_get_P1geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P2geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P3geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_H3geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S2geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S3geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S5geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);

static void     Geom1D_get_P1geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P2geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P3geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_H3geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S2geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S3geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S5geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);

static void     Geom1D_get_P1geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P2geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_P3geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_H3geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S2geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S3geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);
static void     Geom1D_get_S5geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1);

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

GEOM_1D *Geom1D_setup_from_params(const ELT_1D *MyElt, const PARAMS* MyParams)
{
   GEOM_1D *MyGeom;
   
   if ( strcmp(MyParams->geom_params.meshfile, "NULL") != 0  )
   {
      MyGeom = Geom1D_get_fromfile(MyElt, MyParams->geom_params.meshfile,
                                MyParams->geom_params.meshname,
                                MyParams->geom_params.meshtype);
   }
   else
   {
      MyGeom = Geom1D_get_fromscratch(MyElt, MyParams->geom_params.xmin ,
                                   MyParams->geom_params.xmax ,
                                   MyParams->geom_params.nx   ,
                                   MyParams->geom_params.periodicity,
                                   MyParams->geom_params.distribution);
   }
   
   return MyGeom;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

GEOM_1D *Geom1D_get_fromfile(const ELT_1D *elt, const char *meshfile, const char *meshname, const char *meshtype)
{
   GEOM_1D *Geom = NULL;
   FILE    *fp   = NULL;
   
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_fromfile");
   if ( meshfile == NULL ) error(E_NULL, "Geom1D_get_fromfile");
   if ( meshtype == NULL ) error(E_NULL, "Geom1D_get_fromfile");
   if ( meshname == NULL ) error(E_NULL, "Geom1D_get_fromfile");
   
   int i;
   /* ----- fill the fields of Geom --- */
   int NX;  /* the number of segments */
   real XMIN, XMAX;
   VEC* H1; /* the vector of segments */
   /* --------------------------------- */
   char validation_output[8192];
   char *meshfile_schema = "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/JSON_SCHEMAS/SCHEMA_MESH_1D.json";
   int status = json_check_data(meshfile, meshfile_schema, validation_output);
   
   printf("%s", validation_output);
   
   if ( status != 0 )
   {
      error3(E_GEOM_MESHFILE_JSON_VALIDATION, "Geom1D_get_fromfile");
      fprintf(stderr, "%s", validation_output);
   }
   
   // NEW : read JSON file ...
   json_t *meshes = json_load_xfile(meshfile);
   json_t *mesh   = json_object_get(meshes, meshname);
   
   if ( mesh == NULL )
   {
      error3(E_GEOM_MESHFILE_JSON_MESH_NOT_FOUND, "Geom1D_get_fromfile");
   }
   
   json_unpack(mesh, "{s:i}", "NX", &NX);
   json_unpack(mesh, "{s:f, s:f}", "XMIN", &XMIN, "XMAX", &XMAX);
      
   /* allocate H1 */
   H1    = v_get(NX);
      
   json_t *segments = json_object_get(mesh, "SEGMENTS");
   
   // check NX with array real length
   int size = json_array_size(segments);
   
   if ( size != NX )
   {
      error3(E_GEOM_MESH1D_JSON_SEGMENTS_BAD_ARRAY_SIZE, "Geom1D_get_fromfile");
   }
   
   
   for (i=0; i<NX; i++) H1->ve[i] = json_real_value(json_array_get(segments, i));

   /* check consistance */
   real dist = 0.0;
   for (i=0; i<NX; i++) dist += H1->ve[i];
   
   if ( fabs(dist - (XMAX-XMIN)) > 1.0e-6)
   {
      error3(E_GEOM_MESH1D_JSON_SEGMENTS_BAD_TOTAL_SIZE, "Geom1D_get_fromfile");
   }
   
   json_t* json_periodicity = json_object_get(mesh, "PERIODICITY");
   int periodicity = json_integer_value(json_periodicity);
   
   /* final construct */
   Geom = Geom1D_get_from_segments(elt, H1, XMIN, XMAX, periodicity);
   strncpy(Geom->type, meshtype, 16);
   
   /* got "default" ref_s - read them now from file ...*/
   json_t *vertices_infos   = json_object_get(mesh, "VERTICES_INFOS");
   json_t *vertices_idxs    = json_object_get(vertices_infos, "VERTICES_IDX");
   
   int LEFT_IDX, RIGHT_IDX;
   json_unpack(vertices_idxs, "{s:i, s:i}", "LEFT", &LEFT_IDX, "RIGHT", &RIGHT_IDX); 
   
   Geom->REF_S->ive[0] = LEFT_IDX;
   Geom->REF_S->ive[Geom->NBSOMM-1] = RIGHT_IDX;
   
   /* got "default" ref_a - read them now from file ...*/
   json_t *sides_infos   = json_object_get(mesh, "SIDES_INFOS");
   json_t *sides_idxs    = json_object_get(vertices_infos, "SIDES_IDX");
   
   int SIDE_LEFT_IDX, SIDE_RIGHT_IDX;
   json_unpack(sides_idxs, "{s:i, s:i}", "LEFT", &SIDE_LEFT_IDX, "RIGHT", &SIDE_RIGHT_IDX); 
   
   Geom->REF_A->ive[0] = SIDE_LEFT_IDX;
   Geom->REF_A->ive[1] = SIDE_RIGHT_IDX;
   
   /* free memory */
   V_FREE(H1);

   return Geom;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

GEOM_1D *Geom1D_get_fromscratch( const ELT_1D *elt , Real xmin , Real xmax, int nx, MESH_PERIODICITY periodicity, const char* distribution_type )
{
   GEOM_1D *Geom;
   VEC     *H1;

   /* check */
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_fromscratch");
   
   /* construct segments */
   H1 = v_get(nx);
   Geom1D_get_segments(H1, xmin, xmax, distribution_type);
   
   /* final construct */
   Geom = Geom1D_get_from_segments(elt, H1, xmin, xmax, periodicity);
   strncpy(Geom->type, "scratch", 16);
   
   /* free memory */
   V_FREE(H1);

   return Geom;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

int Geom1D_free(GEOM_1D *Geom)
{
   if ( Geom == GEOM_1D_NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_GEOM_1D, sizeof(GEOM_1D), 0, MY_LIST3);
         mem_numvar_list(TYPE_GEOM_1D, -1, MY_LIST3);
      }
      
      /* free the structure GEOM_1D */
      
      IV_FREE(Geom->REF_S);
      IV_FREE(Geom->REF_T);
      IV_FREE(Geom->REF_A);
      
      V_FREE(Geom->XSOMM);
      IM_FREE(Geom->NSELMT);
      IM_FREE(Geom->NSFACE);
      
      SP_FREE(Geom->EF_to_WORLD);
      SP_FREE(Geom->BASEFUNC_BASICS_to_CL);
   
      free(Geom);
      
      return EXIT_SUCCESS;
   }
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

void Geom1D_foutput(FILE *fp, GEOM_1D *Geom)
{
   if ( fp   == (FILE*)NULL ) error(E_NULL, "Geom1D_foutput");
   if ( Geom ==        NULL ) error(E_NULL, "Geom1D_foutput");

   fprintf(fp, "Geometry Output \n");

   fprintf(fp, "\tType: %s\n", Geom->type);
   fprintf(fp, "\tPeriodicity: %d\n", Geom->periodicity);
   fprintf(fp, "\tLimits: X_LEFT = %lf\n", Geom->X_LEFT);
   fprintf(fp, "\tLimits: XRIGHT = %lf\n", Geom->X_RIGHT);

   fprintf(fp, "\tNB_DOF = %d\n", Geom->NB_DOF);

   fprintf(fp, "\tNBSOMM = %d\n", Geom->NBSOMM);
   fprintf(fp, "\tNBELMT = %d\n", Geom->NBELMT);
   fprintf(fp, "\tNBFACE = %d\n", Geom->NBFACE);

   fprintf(fp, "\tArray of Somm\n");
   v_foutput(fp, Geom->XSOMM);
   fprintf(fp, "\tArray of Elements\n");
   im_foutput(fp, Geom->NSELMT);
   fprintf(fp, "\tArray of Faces\n");
   im_foutput(fp, Geom->NSFACE);
   
   fprintf(fp, "\tArray of Ref. on Somm\n");
   iv_foutput(fp, Geom->REF_S);
   fprintf(fp, "\tArray of Ref. on Elements\n");
   iv_foutput(fp, Geom->REF_T);     /* references on the elements(segments)  */
   fprintf(fp, "\tArray of Ref. on Sides\n");
   iv_foutput(fp, Geom->REF_A);     /* references on the faces  */
   
   fprintf(fp, "\t Nb Ref on SOMM = %d\n", Geom1D_get_nb_ref_on_somm(Geom) );
   fprintf(fp, "\t Nb Ref on ELTS = %d\n", Geom1D_get_nb_ref_on_elts(Geom) );
   fprintf(fp, "\t Nb Ref on FACE = %d\n", Geom1D_get_nb_ref_on_face(Geom) );

   fprintf(fp, "\tThe matrix EF_to_WORLD\n");
   sp_foutput(fp, Geom->EF_to_WORLD); /* map EF coords to PHYSIC coords */

   fprintf(fp, "\tThe matrix BASEFUNC_BASICS_to_CL\n");
   sp_foutput(fp, Geom->BASEFUNC_BASICS_to_CL); /* */

   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_segments(VEC* H1, Real xmin, Real xmax, const char* distribution_type)
{
   int  i;
   Real len = xmax - xmin;
   Real tmp_len = 0.0;
   
   if ( H1 == NULL ) error(E_NULL, "Geom1D_get_segments");
   if ( len <= 0.0 ) error(E_NEG,  "Geom1D_get_segments"); 
   
   double q  = 0.0;
   double Uo = 0.0;
   
   if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-RIGHT") == 0 )
   {
      q = 0.9;
      Uo = 1.0 / ( (pow(q,H1->dim) -1)/(q-1));
   }
   else
   if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-LEFT") == 0 )
   {
      q = 1.0 / 0.9;
      Uo = 1.0 / ( (pow(q,H1->dim) -1)/(q-1));
   }
   else
   if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-LEFTRIGHT") == 0 )
   {
      q = 1.0 / 0.85;
      Uo = 1.0 / ( (pow(q,H1->dim) -1)/(q-1));
   }
   else
   if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-MIDDLE") == 0 )
   {
      q = 0.85;
      Uo = 1.0 / ( (pow(q,H1->dim) -1)/(q-1));
   }
   
   for (i=0; i<H1->dim; i++ )
   {
      //H1->ve[i] = fun->eval(fun, (double)i );
      if ( strcmp(distribution_type, "UNIFORM") == 0 )
      {
         H1->ve[i] = 1.0; // uniform 
      }
      else
      if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-RIGHT") == 0 )
      {
         if ( i == 0 )
            H1->ve[i] = Uo;
         else
            H1->ve[i] = q * H1->ve[i-1];
      }
      else
      if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-LEFT") == 0 )
      {
         if ( i == 0 )
            H1->ve[i] = Uo;
         else
            H1->ve[i] = q * H1->ve[i-1]; 
      }
      else
      if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-LEFTRIGHT") == 0 )
      {
         if ( i == 0 )
            H1->ve[i] = Uo;
         else
         {
            if ( i < H1->dim / 2 )
               H1->ve[i] = q * H1->ve[i-1];
            else
               H1->ve[i] = 1.0 / q * H1->ve[i-1];
         }
      }
      else
      if ( strcmp(distribution_type, "NON-UNIFORM-HIGHDENSITY-MIDDLE") == 0 )
      {
         if ( i == 0 )
            H1->ve[i] = Uo;
         else
         {
            if ( i < H1->dim / 2 )
               H1->ve[i] = q * H1->ve[i-1];
            else
               H1->ve[i] = 1.0 / q * H1->ve[i-1];
         }
      }
      else 
      {
         H1->ve[i] = 1.0; // uniform
      }

      
      tmp_len += H1->ve[i];
   }

   /* and normalize */
   for (i=0; i<H1->dim; i++ )
   {
      H1->ve[i] *= (len / tmp_len );
   }

   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static GEOM_1D* Geom1D_get_from_segments(const ELT_1D *element, const VEC* H1, Real X_LEFT, Real X_RIGHT, MESH_PERIODICITY periodicity)
{
   GEOM_1D* Geom;

   /* check */
   if ( element  == NULL ) error(E_NULL, "Geom1D_get_from_segments");

   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_from_segments");

   
   /* --------- create the structure GEOM_1D ------------------------ */
   if ( (Geom=NEW(GEOM_1D))== (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_from_segments");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* --------------------------------------------------------------- */

   /* init basics fields */
   Geom->periodicity = periodicity;

   Geom->X_LEFT  = X_LEFT;
   Geom->X_RIGHT = X_RIGHT;

   /* --------------------------------------------------------------- */
   /* --------------------------------------------------------------- */
   
   if ( strcmp(element->name_ef,"P1")==0 )
   {
      Geom1D_get_P1geom_from_segments(Geom, element, H1);
   }
   else
   if ( strcmp(element->name_ef,"P2")==0 )
   {
      Geom1D_get_P2geom_from_segments(Geom, element, H1);
   }
   else
   if ( strcmp(element->name_ef,"P3")==0 )
   {
      Geom1D_get_P3geom_from_segments(Geom, element, H1);
   }
   else
   if ( strcmp(element->name_ef,"H3")==0 )
   {
      Geom1D_get_H3geom_from_segments(Geom, element, H1);
   }
   else
   if ( strcmp(element->name_ef,"S2")==0 )
   {
      Geom1D_get_S2geom_from_segments(Geom, element, H1);
   }
   else
   if ( strcmp(element->name_ef,"S3")==0 )
   {
      Geom1D_get_S3geom_from_segments(Geom, element, H1);
   }
   else
   if ( strcmp(element->name_ef,"S5")==0 )
   {
      Geom1D_get_S5geom_from_segments(Geom, element, H1);
   }
   else
   {
      error2(E_EF_NOTIMPL, "Geom1D_get_from_segments");
   }

   return Geom;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_P1geom_from_segments(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_P1geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_P1geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_P1geom_from_segments");

   /* initialisation tab ef --------------- */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = NX+1      ;

      NBSOMM = NX+1      ;
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = NX        ;
      
      NBSOMM = NX        ;
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_P1geom_from_segments");
   }
   
   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);
   
   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;

   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_P1geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_P1geom_init_matrix_nsface_with_refs(geom, elt, H1);
      
   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_P1geom_init_matrix_coords_with_refs(geom, elt, H1);

   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_P1geom_init_matrix_ef2world(geom, elt, H1);

   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_P2geom_from_segments(GEOM_1D *geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_P2geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_P2geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_P2geom_from_segments");

   /* initialisation tab ef --------------- */
   
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = 2*NX+1    ;
      
      NBSOMM = 2*NX+1    ;
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = 2*NX      ;
      
      NBSOMM = 2*NX      ;
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_P2geom_from_segments");
   }
   
   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);
   
   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;
   
   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_P2geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_P2geom_init_matrix_nsface_with_refs(geom, elt, H1);

   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_P2geom_init_matrix_coords_with_refs(geom, elt, H1);
   
   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_P2geom_init_matrix_ef2world(geom, elt, H1);

   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_P3geom_from_segments(GEOM_1D *geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_P1geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_P3geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_P3geom_from_segments");

   /* initialisation tab ef --------------- */
   
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = 3*NX+1    ;
      
      NBSOMM = 3*NX+1    ;
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = 3*NX      ;
      
      NBSOMM = 3*NX      ;
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_P3geom_from_segments");
   }

   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);
   
   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;

   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_P3geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_P3geom_init_matrix_nsface_with_refs(geom, elt, H1);
   
   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_P3geom_init_matrix_coords_with_refs(geom, elt, H1);

   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_P3geom_init_matrix_ef2world(geom, elt, H1);

   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_H3geom_from_segments(GEOM_1D *geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_H3geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_H3geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_H3geom_from_segments");
   
   /* initialisation tab ef --------------- */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = 2*(NX+1)  ;
      
      NBSOMM = 2*(NX+1)  ; /* NX+1 ?? */
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = 2*NX      ;
      
      NBSOMM = 2*NX      ; /* NX ?? */
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_H3geom_from_segments");
   }
  
   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);
   
   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;

   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_H3geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_H3geom_init_matrix_nsface_with_refs(geom, elt, H1);

   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_H3geom_init_matrix_coords_with_refs(geom, elt, H1);
   
   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_H3geom_init_matrix_ef2world(geom, elt, H1);
   
   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_S2geom_from_segments(GEOM_1D *geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_S2geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_S2geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_S2geom_from_segments");

   /* initialisation tab ef --------------- */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = NX+2      ;
      
      NBSOMM = NX+1      ; /* NEW */
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = NX        ;
      
      NBSOMM = NX        ;
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S3geom_from_segments");
   }

   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);

   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;

   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_S2geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_S2geom_init_matrix_nsface_with_refs(geom, elt, H1);

   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_S2geom_init_matrix_coords_with_refs(geom, elt, H1);

   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_S2geom_init_matrix_ef2world(geom, elt, H1);

   /* if mesh non-uniform */
   /*
    ((ELT_1D*)elt)->splines_def = Splines1D_get (elt, geom, H1);
    */

   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_S3geom_from_segments(GEOM_1D *geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_S3geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_S3geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_S3geom_from_segments");

   /* initialisation tab ef --------------- */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = NX+3      ;
      
      NBSOMM = NX+3      ; /* NX+1 */
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = NX        ;
      
      NBSOMM = NX        ;
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S3geom_from_segments");
   }
  
   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);
   
   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;

   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_S3geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_S3geom_init_matrix_nsface_with_refs(geom, elt, H1);

   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_S3geom_init_matrix_coords_with_refs(geom, elt, H1);
   
   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_S3geom_init_matrix_ef2world(geom, elt, H1);

   /* if mesh non-uniform */
   /*
    ((ELT_1D*)elt)->splines_def = Splines1D_get (elt, geom, H1);
   */
   
   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static void Geom1D_get_S5geom_from_segments(GEOM_1D *geom, const ELT_1D *elt, const VEC* H1)
{
   int  NX = H1->dim;
   int  NBELMT, NBSOMM, NBFACE, NB_DOF;

   /* check */
   if ( geom     == NULL ) error(E_NULL, "Geom1D_get_S5geom_from_segments");
   if ( elt      == NULL ) error(E_NULL, "Geom1D_get_S5geom_from_segments");
   if ( H1       == NULL ) error(E_NULL, "Geom1D_get_S5geom_from_segments");

   /* initialisation tab ef --------------- */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      NB_DOF = NX+5      ;
      
      NBSOMM = NX+5      ; /* NX+1 */
      NBELMT = NX        ;
      NBFACE = 2         ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      NB_DOF = NX        ;
      
      NBSOMM = NX        ;
      NBELMT = NX        ;
      NBFACE = 1         ;     /* infact 0 */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S5geom_from_segments");
   }

   /* initialisation tab ef --------------- */
   geom->XSOMM  = v_get(NBSOMM);

   geom->REF_T  = iv_get(NBELMT);
   geom->REF_S  = iv_get(NBSOMM);
   geom->REF_A  = iv_get(NBFACE);
   
   geom->NSELMT = im_get(NBELMT, elt->nb_somm_cell);
   geom->NSFACE = im_get(NBFACE, 1);

   /* assign dimensions to geom */
   geom->NB_DOF  = NB_DOF;
   geom->NBELMT  = NBELMT;
   geom->NBSOMM  = NBSOMM;
   geom->NBFACE  = NBFACE;

   /*----------------------------------------------------*/
   /* numerotation des sommets des elements et des faces */
   /*----------------------------------------------------*/
   Geom1D_get_S5geom_init_matrix_nselmt_with_refs(geom, elt, H1);

   Geom1D_get_S5geom_init_matrix_nsface_with_refs(geom, elt, H1);

   /*--------------------------*/
   /* coordonnees  des sommets */
   /*--------------------------*/
   Geom1D_get_S5geom_init_matrix_coords_with_refs(geom, elt, H1);

   /*----------------------------------------------------*/
   /* initialisation de la matrice EF_to_WORLD           */
   /*----------------------------------------------------*/
   Geom1D_get_S5geom_init_matrix_ef2world(geom, elt, H1);

   /* if mesh non-uniform */
   /*
    ((ELT_1D*)elt)->splines_def = Splines1D_get (elt, geom, H1);
   */
   
   /* end */
   return;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static int Geom1D_get_nb_ref_on_somm(const GEOM_1D* Geom)
{
   if ( Geom->periodicity == PERIODIC_MESHe )
   {
      return 0;
   }
   else
   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      return 2;
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_from_segments");
   }

   return 0;
}

/* ------------------------------------------------------------------------------------------*/

static int Geom1D_get_nb_ref_on_elts(const GEOM_1D *Geom)
{
   return 0;  
}

/* ------------------------------------------------------------------------------------------*/

static int Geom1D_get_nb_ref_on_face(const GEOM_1D *Geom)
{
   if ( Geom->periodicity == PERIODIC_MESHe )
   {
      return 0;
   }
   else
   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      return 2;
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_nb_ref_on_elts");
   }

   return 0;   
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

int  Geom1D_check_with_boundaryconditions( const GEOM_1D *MyGeom , const BC_1D *MyBC, const int axe )
{
   char axe_name[8];
   int i;
   
   int nb_refs_on_somm ;
   int nb_refs_on_face ;

   if ( MyBC   == ( BC_1D*)NULL   ) error(E_NULL, "Geom1D_check_with_boundaryconditions");
   if ( MyGeom == ( GEOM_1D*)NULL ) error(E_NULL, "Geom1D_check_with_boundaryconditions");

   switch(axe)
   {
      case AXEe_X: strcpy(axe_name, "AXEe_X"); break;

      default: error(E_UNKNOWN, "Geom1D_check_with_boundaryconditions");
   }

   /*
    * Check BC compatibility
    */

   Bc1D_checkBC(MyBC);

   /*
    * Check compatibility between the number of references
    * given in the geometry and the number of functions
    * to handle theses references in MyParams->bc_conditions
    */

   nb_refs_on_somm = Geom1D_get_nb_ref_on_somm(MyGeom);
   nb_refs_on_face = Geom1D_get_nb_ref_on_face(MyGeom);

   printf("\t Geometry->nb_ref_on_somm = %d \n", nb_refs_on_somm);
   printf("\t Geometry->nb_ref_on_face = %d \n", nb_refs_on_face);

   for (i=1; i<=nb_refs_on_somm; i++)
   {
      if ( BC_1De_UNKNOWN == Bc1D_getBcType(MyBC, axe, i) )
      {
         error3(E_GEOM_INCOMPATIBLE_WITH_BC, "Geom1D_check_with_boundaryconditions");
         return EXIT_FAILURE;
      }
   }

   for (i=1; i<=nb_refs_on_face; i++)
   {
      if ( BC_1De_UNKNOWN == Bc1D_getBcType(MyBC, axe, i) )
      {
         error3(E_GEOM_INCOMPATIBLE_WITH_BC, "Geom1D_check_with_boundaryconditions");
         return EXIT_FAILURE;
      }
   }
   
   return EXIT_SUCCESS;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

VEC     *Geom1D_getBaseMesh    ( const ELT_1D *elt , const GEOM_1D* geom )
{
   GEOM_1D * GeomP1;
   VEC     * BASE_MESH;

   /* check */
   if ( elt     == ELT_1D_NULL   ) error(E_NULL, "Geom1D_getBaseMesh");
   if ( geom    == GEOM_1D_NULL  ) error(E_NULL, "Geom1D_getBaseMesh");

   /**/
   GeomP1    = Geom1D_getP1geom_from ( elt, geom );

   BASE_MESH = v_copy(GeomP1->XSOMM, NULL);

   /* clean */
   GEOM_1D_FREE(GeomP1);

   /* finito */
   return BASE_MESH;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

GEOM_1D *Geom1D_getP1geom_from ( const ELT_1D *element, const GEOM_1D* geom )
{
   GEOM_1D *GeomP1     = NULL;

   if ( element  == NULL )  error(E_NULL, "Geom1D_getP1geom_from");
   if ( geom     == NULL )  error(E_NULL, "Geom1D_getP1geom_from");


   if ( (strcmp(element->name_ef,"P2")==0 ) )
   {
      GeomP1 = Geom1D_get_fromParent_P2toP1( element , geom );
   }
   else
   if ( (strcmp(element->name_ef,"P3")==0 ) )
   {
      GeomP1 = Geom1D_get_fromParent_P3toP1( element , geom );
   }
   else
   if ( (strcmp(element->name_ef,"H3")==0 ) )
   {
      GeomP1 = Geom1D_get_fromParent_H3toP1( element , geom );
   }
   else
   if ( (strcmp(element->name_ef,"S2")==0 ) )
   {
      GeomP1 = Geom1D_get_fromParent_S2toP1( element , geom );
   }
   else
   if ( (strcmp(element->name_ef,"S3")==0 ) )
   {
      GeomP1 = Geom1D_get_fromParent_S3toP1( element , geom );
   }
   else
      if ( (strcmp(element->name_ef,"S5")==0 ) )
   {
      GeomP1 = Geom1D_get_fromParent_S5toP1( element , geom );
   }
   else
   {
      GeomP1 = Geom1D_copy_from(geom);
      warning3(W_GEOM_ALREADY_P1, "Geom1D_getP1geom_from");
   }

   return GeomP1;
}

/* ------------------------------------------------------------------------------------------*/
/* ------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_copy_from(const GEOM_1D* GeomP1)
{
   GEOM_1D* Geom;

   if ( GeomP1  == NULL ) error(E_NULL, "Geom1D_copy_from");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_copy_from");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomP1->periodicity;

   strncpy(Geom->type, GeomP1->type, 16);
   Geom->type[15] = '\0';
   
   /* fill Geom */
   Geom->NB_DOF = GeomP1->NB_DOF;

   Geom->NBSOMM = GeomP1->NBSOMM;
   Geom->NBELMT = GeomP1->NBELMT;
   Geom->NBFACE = GeomP1->NBFACE;

   Geom->REF_S = iv_copy(GeomP1->REF_S, NULL);
   Geom->REF_T = iv_copy(GeomP1->REF_T, NULL);
   Geom->REF_A = iv_copy(GeomP1->REF_A, NULL);

   Geom->XSOMM  =  v_copy(GeomP1->XSOMM , NULL);
   Geom->NSELMT = im_copy(GeomP1->NSELMT, NULL);
   Geom->NSFACE = im_copy(GeomP1->NSFACE, NULL);

   Geom->EF_to_WORLD           = sp_copy(GeomP1->EF_to_WORLD);
   Geom->BASEFUNC_BASICS_to_CL = sp_copy(GeomP1->BASEFUNC_BASICS_to_CL);

   Geom->X_LEFT  = GeomP1->X_LEFT;
   Geom->X_RIGHT = GeomP1->X_RIGHT;
   
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_get_fromParent_P2toP1( const ELT_1D *element, const GEOM_1D* GeomX )
{
   GEOM_1D* Geom;
   int e;
   
   if ( GeomX  == NULL ) error(E_NULL, "Geom1D_get_fromParent_P2toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_fromParent_P2toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomX->periodicity;

   strncpy(Geom->type, GeomX->type, 16);
   Geom->type[15] = '\0';
   
   /* fill Geom */
   Geom->X_LEFT  = GeomX->X_LEFT;
   Geom->X_RIGHT = GeomX->X_RIGHT;
   
   Geom->NBSOMM = GeomX->NBSOMM;
   Geom->NBELMT = GeomX->NBELMT * 2;
   Geom->NBFACE = GeomX->NBFACE;

   Geom->REF_S = iv_get(Geom->NBSOMM); iv_copy(GeomX->REF_S, Geom->REF_S);
   Geom->REF_T = iv_get(Geom->NBELMT); 
   Geom->REF_A = iv_get(Geom->NBFACE); iv_copy(GeomX->REF_A, Geom->REF_A);

   Geom->XSOMM  =  v_get(Geom->NBSOMM   );  v_copy(GeomX->XSOMM, Geom->XSOMM);
   Geom->NSELMT = im_get(Geom->NBELMT, 2);
   Geom->NSFACE = im_get(Geom->NBFACE, 1); im_copy(GeomX->NSFACE, Geom->NSFACE);

   Geom->EF_to_WORLD = sp_get(Geom->NBSOMM, Geom->NBSOMM, 2);
   sp_eye(Geom->EF_to_WORLD);
   
   /* new */
   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->REF_T->ive[2*e  ] = GeomX->REF_T->ive[e];
      Geom->REF_T->ive[2*e+1] = GeomX->REF_T->ive[e];
   }

   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->NSELMT->im[2*e  ][0] = GeomX->NSELMT->im[e][0];
      Geom->NSELMT->im[2*e  ][1] = GeomX->NSELMT->im[e][1];

      Geom->NSELMT->im[2*e+1][0] = GeomX->NSELMT->im[e][1];
      Geom->NSELMT->im[2*e+1][1] = GeomX->NSELMT->im[e][2];
   }

   /* finished */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_get_fromParent_P3toP1( const ELT_1D *element, const GEOM_1D* GeomX )
{
   GEOM_1D* Geom;
   int e;
   
   if ( GeomX  == NULL ) error(E_NULL, "Geom1D_get_fromParent_P3toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_fromParent_P3toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomX->periodicity;

   strncpy(Geom->type, GeomX->type, 16);
   Geom->type[15] = '\0';
   
   /* fill Geom */
   Geom->X_LEFT  = GeomX->X_LEFT;
   Geom->X_RIGHT = GeomX->X_RIGHT;

   Geom->NBSOMM = GeomX->NBSOMM;
   Geom->NBELMT = GeomX->NBELMT * 3;
   Geom->NBFACE = GeomX->NBFACE;

   Geom->REF_S = iv_get(Geom->NBSOMM); iv_copy(GeomX->REF_S, Geom->REF_S);
   Geom->REF_T = iv_get(Geom->NBELMT);
   Geom->REF_A = iv_get(Geom->NBFACE); iv_copy(GeomX->REF_A, Geom->REF_A);

   Geom->XSOMM  =  v_get(Geom->NBSOMM   );  v_copy(GeomX->XSOMM, Geom->XSOMM);
   Geom->NSELMT = im_get(Geom->NBELMT, 2);
   Geom->NSFACE = im_get(Geom->NBFACE, 1); im_copy(GeomX->NSFACE, Geom->NSFACE);

   Geom->EF_to_WORLD = sp_get(Geom->NBSOMM, Geom->NBSOMM, 2);
   sp_eye(Geom->EF_to_WORLD);

   /* new */
   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->REF_T->ive[3*e  ] = GeomX->REF_T->ive[e];
      Geom->REF_T->ive[3*e+1] = GeomX->REF_T->ive[e];
   }

   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->NSELMT->im[3*e  ][0] = GeomX->NSELMT->im[e][0];
      Geom->NSELMT->im[3*e  ][1] = GeomX->NSELMT->im[e][1];

      Geom->NSELMT->im[3*e+1][0] = GeomX->NSELMT->im[e][1];
      Geom->NSELMT->im[3*e+1][1] = GeomX->NSELMT->im[e][2];

      Geom->NSELMT->im[3*e+2][0] = GeomX->NSELMT->im[e][2];
      Geom->NSELMT->im[3*e+2][1] = GeomX->NSELMT->im[e][3];
   }

   /* finished */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_get_fromParent_H3toP1( const ELT_1D *element, const GEOM_1D* GeomX )
{
   GEOM_1D* Geom;
   int e,s;
   
   if ( GeomX  == NULL ) error(E_NULL, "Geom1D_get_fromParent_H3toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_fromParent_H3toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomX->periodicity;

   strncpy(Geom->type, GeomX->type, 16);
   Geom->type[15] = '\0';
   
   /* fill Geom */
   Geom->X_LEFT  = GeomX->X_LEFT;
   Geom->X_RIGHT = GeomX->X_RIGHT;

   Geom->NBSOMM = GeomX->NBSOMM / 2;
   Geom->NBELMT = GeomX->NBELMT;
   Geom->NBFACE = GeomX->NBFACE;

   Geom->REF_S = iv_get(Geom->NBSOMM);
   Geom->REF_T = iv_get(Geom->NBELMT); iv_copy(Geom->REF_T, GeomX->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE);

   Geom->XSOMM  =  v_get(Geom->NBSOMM   );
   Geom->NSELMT = im_get(Geom->NBELMT, 2);
   Geom->NSFACE = im_get(Geom->NBFACE, 1); im_copy(GeomX->NSFACE, Geom->NSFACE);

   Geom->EF_to_WORLD = sp_get(Geom->NBSOMM, Geom->NBSOMM, 2);
   sp_eye(Geom->EF_to_WORLD);

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->REF_S->ive[s] = GeomX->REF_S->ive[2*s];
   }

   
   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->XSOMM->ve[s] = GeomX->XSOMM->ve[2*s];
   }
   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->NSELMT->im[e][0] = GeomX->NSELMT->im[e][0];
      Geom->NSELMT->im[e][1] = GeomX->NSELMT->im[e][2];
   }

   /* finished */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_get_fromParent_S2toP1( const ELT_1D *element, const GEOM_1D* GeomX ) /* TO CHECK */
{
   GEOM_1D* Geom;
   int s,e,a;

   int delta_nbsomm;

   if ( GeomX  == NULL ) error(E_NULL, "Geom1D_get_fromParent_S2toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_fromParent_S2toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomX->periodicity;

   strncpy(Geom->type, GeomX->type, 16);
   Geom->type[15] = '\0';
   
   if ( Geom->periodicity == PERIODIC_MESHe )
   {
      delta_nbsomm = 0;
   }
   else
   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      delta_nbsomm = 1; /* ? */
   }
   else
   {
      error(E_UNKNOWN, "Geom1D_get_fromParent_S3toP1");
   }


   /* fill Geom */
   Geom->X_LEFT  = GeomX->X_LEFT;
   Geom->X_RIGHT = GeomX->X_RIGHT;

   Geom->NBSOMM = GeomX->NBSOMM - 2*delta_nbsomm ;
   Geom->NBELMT = GeomX->NBELMT;
   Geom->NBFACE = GeomX->NBFACE;

   Geom->REF_S = iv_get(Geom->NBSOMM);
   Geom->REF_T = iv_get(Geom->NBELMT); iv_copy(GeomX->REF_T, Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE); iv_copy(GeomX->REF_A, Geom->REF_A);

   Geom->XSOMM  =  v_get(Geom->NBSOMM   );
   Geom->NSELMT = im_get(Geom->NBELMT, 2);
   Geom->NSFACE = im_get(Geom->NBFACE, 1);

   Geom->EF_to_WORLD = sp_get(Geom->NBSOMM, Geom->NBSOMM, 2);
   sp_eye(Geom->EF_to_WORLD);

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->REF_S->ive[s] = GeomX->REF_S->ive[s+delta_nbsomm];
   }

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->XSOMM->ve[s] = GeomX->XSOMM->ve[s+delta_nbsomm];
   }

   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->NSELMT->im[e][0] = GeomX->NSELMT->im[e][0];
      Geom->NSELMT->im[e][1] = GeomX->NSELMT->im[e][1];
   }

   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      Geom->NSFACE->im[0][0] = GeomX->NSFACE->im[0][0] + delta_nbsomm;
      Geom->NSFACE->im[1][0] = GeomX->NSFACE->im[1][0] - delta_nbsomm;
   }

   /* finished */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_get_fromParent_S3toP1( const ELT_1D *element, const GEOM_1D* GeomX )
{
   GEOM_1D* Geom;
   int s,e,a;

   int delta_nbsomm;

   if ( GeomX  == NULL ) error(E_NULL, "Geom1D_get_fromParent_S3toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_fromParent_S3toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomX->periodicity;

   strncpy(Geom->type, GeomX->type, 16);
   Geom->type[15] = '\0';
   
   if ( Geom->periodicity == PERIODIC_MESHe )
   {
      delta_nbsomm = 0;
   }
   else
   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      delta_nbsomm = 1;
   }
   else
   {
      error(E_UNKNOWN, "Geom1D_get_fromParent_S3toP1");
   }


   /* fill Geom */
   Geom->X_LEFT  = GeomX->X_LEFT;
   Geom->X_RIGHT = GeomX->X_RIGHT;

   Geom->NBSOMM = GeomX->NBSOMM - 2*delta_nbsomm ;
   Geom->NBELMT = GeomX->NBELMT;
   Geom->NBFACE = GeomX->NBFACE;

   Geom->REF_S = iv_get(Geom->NBSOMM);
   Geom->REF_T = iv_get(Geom->NBELMT); iv_copy(GeomX->REF_T, Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE); iv_copy(GeomX->REF_A, Geom->REF_A);

   Geom->XSOMM  =  v_get(Geom->NBSOMM   );
   Geom->NSELMT = im_get(Geom->NBELMT, 2);
   Geom->NSFACE = im_get(Geom->NBFACE, 1);

   Geom->EF_to_WORLD = sp_get(Geom->NBSOMM, Geom->NBSOMM, 2);
   sp_eye(Geom->EF_to_WORLD);

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->REF_S->ive[s] = GeomX->REF_S->ive[s+delta_nbsomm];
   }

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->XSOMM->ve[s] = GeomX->XSOMM->ve[s+delta_nbsomm];
   }
   
   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->NSELMT->im[e][0] = GeomX->NSELMT->im[e][0];
      Geom->NSELMT->im[e][1] = GeomX->NSELMT->im[e][1];
   }
   
   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      Geom->NSFACE->im[0][0] = GeomX->NSFACE->im[0][0] + delta_nbsomm;
      Geom->NSFACE->im[1][0] = GeomX->NSFACE->im[1][0] - delta_nbsomm;
   }
   
   /* finished */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_1D *Geom1D_get_fromParent_S5toP1( const ELT_1D *element, const GEOM_1D* GeomX )
{
   GEOM_1D* Geom;
   int s,e,a;
   
   int delta_nbsomm;

   if ( GeomX  == NULL ) error(E_NULL, "Geom1D_get_fromParent_S5toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_1D)) == (GEOM_1D *)NULL )
   {
      error(E_MEM, "Geom1D_get_fromParent_S5toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_1D, 0, sizeof(GEOM_1D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_1D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   Geom->periodicity = GeomX->periodicity;

   strncpy(Geom->type, GeomX->type, 16);
   Geom->type[15] = '\0';
   
   if ( Geom->periodicity == PERIODIC_MESHe )
   {
      delta_nbsomm = 0;
   }
   else
   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      delta_nbsomm = 2;
   }
   else
   {
      error(E_UNKNOWN, "Geom1D_get_fromParent_S5toP1");
   }

   /* fill Geom */
   Geom->X_LEFT  = GeomX->X_LEFT;
   Geom->X_RIGHT = GeomX->X_RIGHT;

   Geom->NBSOMM = GeomX->NBSOMM - 2*delta_nbsomm ;
   Geom->NBELMT = GeomX->NBELMT;
   Geom->NBFACE = GeomX->NBFACE;

   Geom->REF_S = iv_get(Geom->NBSOMM);
   Geom->REF_T = iv_get(Geom->NBELMT); iv_copy(GeomX->REF_T, Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE); iv_copy(GeomX->REF_A, Geom->REF_A);

   Geom->XSOMM  =  v_get(Geom->NBSOMM   );
   Geom->NSELMT = im_get(Geom->NBELMT, 2);
   Geom->NSFACE = im_get(Geom->NBFACE, 1);

   Geom->EF_to_WORLD = sp_get(Geom->NBSOMM, Geom->NBSOMM, 2);
   sp_eye(Geom->EF_to_WORLD);

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->REF_S->ive[s] = GeomX->REF_S->ive[s+delta_nbsomm];
   }

   for (s=0; s<Geom->NBSOMM; s++)
   {
      Geom->XSOMM->ve[s] = GeomX->XSOMM->ve[s+delta_nbsomm];
   }
   for (e=0; e<GeomX->NBELMT; e++)
   {
      Geom->NSELMT->im[e][0] = GeomX->NSELMT->im[e][0];
      Geom->NSELMT->im[e][1] = GeomX->NSELMT->im[e][1];
   }

   if ( Geom->periodicity == NON_PERIODIC_MESHe )
   {
      Geom->NSFACE->im[0][0] = GeomX->NSFACE->im[0][0] + delta_nbsomm;
      Geom->NSFACE->im[1][0] = GeomX->NSFACE->im[1][0] - delta_nbsomm;
   }

   /* finished */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P1geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NB_DOF = geom->NB_DOF;
   int NBSOMM = geom->NBSOMM;
   int i;
   
   geom->EF_to_WORLD           = sp_get(NBSOMM,NB_DOF,2);
   geom->BASEFUNC_BASICS_to_CL = sp_get(NB_DOF,NB_DOF,2);
   
   for (i=0; i<NB_DOF; i++) /*  "P1" -> NB_DOF = NBSOMM  */
   {
      sp_set_val(geom->EF_to_WORLD,          i,i, 1.0); /* Identity */
      sp_set_val(geom->BASEFUNC_BASICS_to_CL,i,i, 1.0); /* Identity */
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P2geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NB_DOF = geom->NB_DOF;
   int NBSOMM = geom->NBSOMM;
   int i;

   geom->EF_to_WORLD           = sp_get(NBSOMM,NB_DOF,2);
   geom->BASEFUNC_BASICS_to_CL = sp_get(NB_DOF,NB_DOF,2);
   
   for (i=0; i<NB_DOF; i++) /*  "P2" -> NB_DOF = NBSOMM  */
   {
      sp_set_val(geom->EF_to_WORLD,          i,i, 1.0); /* Identity */
      sp_set_val(geom->BASEFUNC_BASICS_to_CL,i,i, 1.0); /* Identity */
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P3geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NB_DOF = geom->NB_DOF;
   int NBSOMM = geom->NBSOMM;
   int i;

   geom->EF_to_WORLD           = sp_get(NBSOMM,NB_DOF,2);
   geom->BASEFUNC_BASICS_to_CL = sp_get(NB_DOF,NB_DOF,2);
   
   for (i=0; i<NB_DOF; i++) /*  "P3" -> NB_DOF = NBSOMM  */
   {
      sp_set_val(geom->EF_to_WORLD,          i,i, 1.0); /* Identity */
      sp_set_val(geom->BASEFUNC_BASICS_to_CL,i,i, 1.0); /* Identity */
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_H3geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NB_DOF = geom->NB_DOF;
   int NBSOMM = geom->NBSOMM;
   int i;

   geom->EF_to_WORLD           = sp_get(NBSOMM/2,NB_DOF,2); /* ATTENTION */
   geom->BASEFUNC_BASICS_to_CL = sp_get(NB_DOF,  NB_DOF,2);
   
   for (i=0; i<NBSOMM/2; i++)
   {
      sp_set_val(geom->EF_to_WORLD,          i,2*i, 1.0); /* Switch values of f' */
   }

   for (i=0; i<NB_DOF; i++)
   {
      sp_set_val(geom->BASEFUNC_BASICS_to_CL,i,i, 1.0);
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S2geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NB_DOF = geom->NB_DOF;
   int NBSOMM = geom->NBSOMM; 
   int i,j;

   int P = elt->MAT_FuncBasis_CL_L->m;

   geom->EF_to_WORLD           = sp_get(NBSOMM,NB_DOF,2);
   geom->BASEFUNC_BASICS_to_CL = sp_get(NB_DOF,NB_DOF,2);

   /* cf Prenter "Splines and Variational Methods" pp.80-81 */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      /* Matrix for Periodic "nd-order Splines as if there were no CL transformations */
      SPMAT* EF_util1 = sp_get(NB_DOF,NB_DOF,2);

      /* set first the matrix for the CL of the basics base functions */
      sp_ident(geom->BASEFUNC_BASICS_to_CL);

      for (i=0; i<P; i++)
      for (j=0; j<P; j++)
      {
         sp_set_val(geom->BASEFUNC_BASICS_to_CL,          i,         j, elt->MAT_FuncBasis_CL_L->me[i][j]);
         sp_set_val(geom->BASEFUNC_BASICS_to_CL, NB_DOF-P+i,NB_DOF-P+j, elt->MAT_FuncBasis_CL_R->me[i][j]);
      }
      /* voila */


      /* Set the matrix EF_to_WORLD for Quad Splines as if there were no CL transformations */
      for (i=0; i<geom->NBSOMM; i++)
      {
         sp_set_val(EF_util1,i,i   , 1.0);
         sp_set_val(EF_util1,i,i+1 , 1.0);
      }

      /* with the true basis functions ... */

      /* Matrix for Quad Splines when there has been as CL of "basics" base functions */
      geom->EF_to_WORLD = sp_m_mlt(EF_util1, geom->BASEFUNC_BASICS_to_CL, geom->EF_to_WORLD);
      geom->EF_to_WORLD = sp_compact(geom->EF_to_WORLD, 0.0);

      /* clean */
      SP_FREE(EF_util1);
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      /* no CL of base functions */
      sp_ident(geom->BASEFUNC_BASICS_to_CL);

      /* Matrix for Periodic Quad Splines */
      
      /* first line (i=0) */
      sp_set_val(geom->EF_to_WORLD,0,NB_DOF-1, 1.0);
      sp_set_val(geom->EF_to_WORLD,0,       0, 1.0);
      /* interior lines  (1<= i <= NB_DOF-2) */
      for (i=1; i<NBSOMM; i++)
      {
         sp_set_val(geom->EF_to_WORLD,i,i-1  , 1.0);
         sp_set_val(geom->EF_to_WORLD,i,i    , 1.0);
      }
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S2geom_from_segments");
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S3geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int i,j;

   int P = elt->MAT_FuncBasis_CL_L->m;
      
   geom->EF_to_WORLD           = sp_get(NBSOMM,NBSOMM,3);
   geom->BASEFUNC_BASICS_to_CL = sp_get(NBSOMM,NBSOMM,3);
   
   /* cf Prenter "Splines and Variational Methods" pp.80-81 */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      /* Matrix for Periodic Cubic Splines as if there were no CL transformations */
      SPMAT* EF_util1 = sp_get(NBSOMM,NBSOMM,3);

      /* set first the matrix for the CL of the basics base functions */
      sp_ident(geom->BASEFUNC_BASICS_to_CL);

      for (i=0; i<P; i++)
      for (j=0; j<P; j++)
      {
         sp_set_val(geom->BASEFUNC_BASICS_to_CL,          i,         j, elt->MAT_FuncBasis_CL_L->me[i][j]);
         sp_set_val(geom->BASEFUNC_BASICS_to_CL, NBSOMM-P+i,NBSOMM-P+j, elt->MAT_FuncBasis_CL_R->me[i][j]);
      }
      /* voila */


      /* Set the matrix EF_to_WORLD for Cubic Splines as if there were no CL transformations */
      sp_set_val(EF_util1,0,0 , 4.0);
      sp_set_val(EF_util1,0,1 , 1.0);
      for (i=1; i<geom->NBSOMM-1; i++)
      {
         sp_set_val(EF_util1,i,i-1 , 1.0);
         sp_set_val(EF_util1,i,i   , 4.0);
         sp_set_val(EF_util1,i,i+1 , 1.0);
      }
      sp_set_val(EF_util1,NBSOMM-1,NBSOMM-2 , 1.0);
      sp_set_val(EF_util1,NBSOMM-1,NBSOMM-1 , 4.0);

      /* with the true basis functions ... */

      /*
       *  u =  V_{0}.Q_{0} + V_{1}.Q_{1} + V_{2}.Q_{2} + ... +  V_{N+1}.Q_{N+1} +  V_{N+2}.Q_{N+2}
       *  with Q_{0}, Q_{1}, Q_{2}, ...  our "modified" basis functions that we utilize
       *
       *  X =  EF_to_WORLD * V    ( with  V = [V1, V2, V3, ...]' )
       *
       *  we have u = U_{0}.P_{0} + U_{1}.P_{1} + U_{2}.P_{2} + ...+  U_{N+1}.Q_{N+1} +  U_{N+2}.P_{N+2}
       *  with P0, P1, P2, ...  the "basic" splines functions
       *
       *  The transformation from { U_i } to the phyical. coords is known , because easy:
       *  it is given with EF_util1
       *
       *              X = UTIL1 * U
       *
       *
       *  we look for
       *
       *              U = UTIL2 * V  ( with  V = [V0, V1, V2, V3, ...]' ) 
       *
       *  so that the desired matrix EF_to_WORLD can be calculated with 
       *
       *              X = EF_to_WORLD * V  = UTIL1 * U
       *                                   = UTIL1 * UTIL2 * V
       *
       *
       *  let's calculate UTILS2 :
       *
       *  we have Qo = k_00.Po + k_10.P1 + k_20.P2
       *          Q1 = k_01.Po + k_11.P1 + k_21.P2
       *          Q2 = k_02.Po + k_12.P1 + k_22.P2
       *          Q3 = P3
       *          Q4 = P4
       *
       *     u = Vo.Qo + V1.Q1 + V2.Q2 + ...
       *  -> u = Vo.[k_00.Po + k_10.P1 + k_20.P2] +
       *         V1.[k_01.Po + k_11.P1 + k_21.P2] +
       *         V2.[k_02.Po + k_12.P1 + k_22.P2] +  ...
       *  -> u = [ Vo.k_00 + V1.k_01  + V2.k_02 ].Po + [ Vo.k_10 + V1.k_11  + V2.k_12 ].P1 + ...
       *
       *  -> Uo =  Vo.k_00 + V1.k_01 +  V2.k_02
       *  -> U1 =  Vo.k_10 + V1.k_11 +  V2.k_12
       *  -> U2 =  Vo.k_20 + V1.k_21 +  V2.k_22
       *  -> U3 =  V3
       *
       *      [ k_00  k_01  k_02   0   0  ]  [ V0 ]
       *      [ k_10  k_11  k_12   0   0  ]  [ V1 ]
       *  U = [ k_20  k_21  k_22   0   0  ]  [ V2 ]  = UTILS2 * V 
       *      [  0     0     0     1   0  ]  [ V3 ]
       *      [  0     0     0     0   1  ]  [ V4 ]
       *
       *                    
       *   UTIL2 is infact  BASEFUNC_BASICS_to_CL
       *
       */

      /* Matrix for Cubic Splines when there has been as CL of "basics" base functions */
      geom->EF_to_WORLD = sp_m_mlt(EF_util1, geom->BASEFUNC_BASICS_to_CL, geom->EF_to_WORLD);
      geom->EF_to_WORLD = sp_compact(geom->EF_to_WORLD, 0.0);
        
      /* clean */
      SP_FREE(EF_util1);
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      /* no CL of base functions */
      sp_ident(geom->BASEFUNC_BASICS_to_CL);

      /* Matrix for Periodic Cubic Splines */

      /* first line (i=0) */
      sp_set_val(geom->EF_to_WORLD,0,NBSOMM-1, 1.0);
      sp_set_val(geom->EF_to_WORLD,0,       0, 4.0);
      sp_set_val(geom->EF_to_WORLD,0,       1, 1.0);
      /* interior lines  (1<= i <= NBSOMM-2) */
      for (i=1; i<NBSOMM-1; i++)
      {
         sp_set_val(geom->EF_to_WORLD,i,i-1, 1.0);
         sp_set_val(geom->EF_to_WORLD,i,i  , 4.0);
         sp_set_val(geom->EF_to_WORLD,i,i+1, 1.0);
      }
      /*  last  line (i=NBSOMM-1) */
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,NBSOMM-2, 1.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,NBSOMM-1, 4.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,       0, 1.0);
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S3geom_from_segments");
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S5geom_init_matrix_ef2world(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int i,j;

   int P = elt->MAT_FuncBasis_CL_L->m;

   geom->EF_to_WORLD           = sp_get(NBSOMM,NBSOMM,5);
   geom->BASEFUNC_BASICS_to_CL = sp_get(NBSOMM,NBSOMM,5);

   /* cf Prenter "Splines and Variational Methods" pp.80-81 */

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      /* Matrix for Periodic Cubic Splines as if there were no CL transformations */
      SPMAT* EF_util1 = sp_get(NBSOMM,NBSOMM,5);

      /* set first the matrix for the CL of the basics base functions */
      sp_ident(geom->BASEFUNC_BASICS_to_CL);

      for (i=0; i<P; i++)
      for (j=0; j<P; j++)
      {
         sp_set_val(geom->BASEFUNC_BASICS_to_CL,          i,         j, elt->MAT_FuncBasis_CL_L->me[i][j]);
         sp_set_val(geom->BASEFUNC_BASICS_to_CL, NBSOMM-P+i,NBSOMM-P+j, elt->MAT_FuncBasis_CL_R->me[i][j]);
      }
      /* voila */

            
      /* Set the matrix EF_to_WORLD for Quintic Splines as if there were no CL transformations */
      sp_set_val(EF_util1,0,0, 66.0);
      sp_set_val(EF_util1,0,1, 26.0);
      sp_set_val(EF_util1,0,2,  1.0);

      sp_set_val(EF_util1,1,1-1, 26.0);
      sp_set_val(EF_util1,1,1  , 66.0);
      sp_set_val(EF_util1,1,1+1, 26.0);
      sp_set_val(EF_util1,1,1+2,  1.0);

      for (i=2; i<NBSOMM-2; i++)
      {
         sp_set_val(EF_util1,i,i-2,  1.0);
         sp_set_val(EF_util1,i,i-1, 26.0);
         sp_set_val(EF_util1,i,i  , 66.0);
         sp_set_val(EF_util1,i,i+1, 26.0);
         sp_set_val(EF_util1,i,i+2,  1.0);
      }

      sp_set_val(EF_util1,NBSOMM-2,NBSOMM-4,  1.0);
      sp_set_val(EF_util1,NBSOMM-2,NBSOMM-3, 26.0);
      sp_set_val(EF_util1,NBSOMM-2,NBSOMM-2, 66.0);
      sp_set_val(EF_util1,NBSOMM-2,NBSOMM-1, 26.0);

      sp_set_val(EF_util1,NBSOMM-1,NBSOMM-3,   1.0);
      sp_set_val(EF_util1,NBSOMM-1,NBSOMM-2 , 26.0);
      sp_set_val(EF_util1,NBSOMM-1,NBSOMM-1 , 66.0);


      /* Matrix for Quintic Splines when there has been as CL of "basics" base functions */
      geom->EF_to_WORLD = sp_m_mlt(EF_util1, geom->BASEFUNC_BASICS_to_CL, geom->EF_to_WORLD);
      geom->EF_to_WORLD = sp_compact(geom->EF_to_WORLD, 0.0);

      /* clean */
      SP_FREE(EF_util1);
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->BASEFUNC_BASICS_to_CL = sp_ident(geom->BASEFUNC_BASICS_to_CL);

      /* Matrix for Periodic Quintic Splines */
      geom->EF_to_WORLD = sp_get(NBSOMM,NBSOMM,5);

      /* first line (i=0) */
      sp_set_val(geom->EF_to_WORLD,0,NBSOMM-2, 1.0);
      sp_set_val(geom->EF_to_WORLD,0,NBSOMM-1,26.0);
      sp_set_val(geom->EF_to_WORLD,0,       0,66.0);
      sp_set_val(geom->EF_to_WORLD,0,       1,26.0);
      sp_set_val(geom->EF_to_WORLD,0,       2, 1.0);
      /* second line (i=1) */
      sp_set_val(geom->EF_to_WORLD,1,NBSOMM-1, 1.0);
      sp_set_val(geom->EF_to_WORLD,1,       0,26.0);
      sp_set_val(geom->EF_to_WORLD,1,       1,66.0);
      sp_set_val(geom->EF_to_WORLD,1,       2,26.0);
      sp_set_val(geom->EF_to_WORLD,1,       3, 1.0);
      /* interior lines  (2<= i <= NBSOMM-3) */
      for (i=2; i<NBSOMM-2; i++)
      {
         sp_set_val(geom->EF_to_WORLD,i,i-2, 1.0);
         sp_set_val(geom->EF_to_WORLD,i,i-1,26.0);
         sp_set_val(geom->EF_to_WORLD,i,i  ,66.0);
         sp_set_val(geom->EF_to_WORLD,i,i+1,26.0);
         sp_set_val(geom->EF_to_WORLD,i,i+2, 1.0);
      }
      /*  for-last  line (i=NBSOMM-2) */
      sp_set_val(geom->EF_to_WORLD,NBSOMM-2,NBSOMM-4, 1.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-2,NBSOMM-3,26.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-2,NBSOMM-2,66.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-2,NBSOMM-1,26.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-2,       0, 1.0);
      /*  last  line (i=NBSOMM-1) */
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,NBSOMM-3, 1.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,NBSOMM-2,26.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,NBSOMM-1,66.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,       0,26.0);
      sp_set_val(geom->EF_to_WORLD,NBSOMM-1,       1, 1.0);
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S5geom_from_segments");
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P1geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int e;

   for (e=0; e<NBELMT; e++)
   {
      geom->NSELMT->im[e][0] = e  ;
      geom->NSELMT->im[e][1] = e+1;
   }

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->NSELMT->im[NBELMT-1][1] = 0; /* periodicite ici */
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T); 
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P2geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int e;

   for (e=0; e<NBELMT; e++)
   {
      geom->NSELMT->im[e][0] = 2*e  ;
      geom->NSELMT->im[e][1] = 2*e+1;
      geom->NSELMT->im[e][2] = 2*e+2;
   }

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->NSELMT->im[NBELMT-1][2] = 0; /* periodicite ici */
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T); 
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P3geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int e;

   for (e=0; e<NBELMT; e++)
   {
      geom->NSELMT->im[e][0] = 3*e  ;
      geom->NSELMT->im[e][1] = 3*e+1;
      geom->NSELMT->im[e][2] = 3*e+2;
      geom->NSELMT->im[e][3] = 3*e+3;
   }

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->NSELMT->im[NBELMT-1][3] = 0; /* periodicite ici */
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T); 
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_H3geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int e;

   for (e=0; e<NBELMT; e++)
   {
      geom->NSELMT->im[e][0] = 2*e  ;
      geom->NSELMT->im[e][1] = 2*e+1;
      geom->NSELMT->im[e][2] = 2*e+2;
      geom->NSELMT->im[e][3] = 2*e+3;
   }

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->NSELMT->im[NBELMT-1][2] = 0; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][3] = 1; /* periodicite ici */
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T); 
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S2geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int NBSOMM = geom->NBSOMM;
   int e;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      for (e=0; e<NBELMT; e++)
      {
         geom->NSELMT->im[e][0] = e  ;
         geom->NSELMT->im[e][1] = e+1;
         geom->NSELMT->im[e][2] = e+2; /* NBELMT + 2 base functions */
      }
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      for (e=0; e<NBELMT; e++)
      {
         geom->NSELMT->im[e][0] = e-1;
         geom->NSELMT->im[e][1] = e  ;
         geom->NSELMT->im[e][2] = e+1;
      }

      geom->NSELMT->im[       0][0] = NBSOMM-1 ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][2] = 0        ; /* periodicite ici */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S2geom_from_segments");
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T);
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S3geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int NBSOMM = geom->NBSOMM;
   int e;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      for (e=0; e<NBELMT; e++)
      {
         geom->NSELMT->im[e][0] = e  ;
         geom->NSELMT->im[e][1] = e+1;
         geom->NSELMT->im[e][2] = e+2;
         geom->NSELMT->im[e][3] = e+3; /* NBELMT+3 base funtions */
      }
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      for (e=0; e<NBELMT; e++)
      {
         geom->NSELMT->im[e][0] = e-1;
         geom->NSELMT->im[e][1] = e  ;
         geom->NSELMT->im[e][2] = e+1;
         geom->NSELMT->im[e][3] = e+2;
      }

      geom->NSELMT->im[       0][0] = NBSOMM-1 ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-2][3] = 0        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][2] = 0        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][3] = 1        ; /* periodicite ici */
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S3geom_from_segments");
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T); 
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S5geom_init_matrix_nselmt_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBELMT = geom->NBELMT;
   int NBSOMM = geom->NBSOMM;
   int e;

   for (e=0; e<NBELMT; e++)
   {
      geom->NSELMT->im[e][0] = e  ;
      geom->NSELMT->im[e][1] = e+1;
      geom->NSELMT->im[e][2] = e+2;
      geom->NSELMT->im[e][3] = e+3;
      geom->NSELMT->im[e][4] = e+4;
      geom->NSELMT->im[e][5] = e+5;
   }

   if  ( geom->periodicity == PERIODIC_MESHe )
   {
      for (e=0; e<NBELMT; e++)
      {
         geom->NSELMT->im[e][0] = e-2;
         geom->NSELMT->im[e][1] = e-1;
         geom->NSELMT->im[e][2] = e  ;
         geom->NSELMT->im[e][3] = e+1;
         geom->NSELMT->im[e][4] = e+2;
         geom->NSELMT->im[e][5] = e+3;
      }

      geom->NSELMT->im[       0][0] = NBSOMM-2 ; /* periodicite ici */
      geom->NSELMT->im[       0][1] = NBSOMM-1 ; /* periodicite ici */
      geom->NSELMT->im[       1][0] = NBSOMM-1 ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-3][5] = 0        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-2][4] = 0        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-2][5] = 1        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][3] = 0        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][4] = 1        ; /* periodicite ici */
      geom->NSELMT->im[NBELMT-1][5] = 2        ; /* periodicite ici */
   }

   /* initialisation a zero */
   iv_zero(geom->REF_T); 
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P1geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 0;
      geom->NSFACE->im[1][0] = NBSOMM-1;
   }


   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]    = 1;
      geom->REF_A->ive[1]    = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P2geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 0;
      geom->NSFACE->im[1][0] = NBSOMM-1;
   }


   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]    = 1;
      geom->REF_A->ive[1]    = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P3geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 0;
      geom->NSFACE->im[1][0] = NBSOMM-1;
   }


   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]    = 1;
      geom->REF_A->ive[1]    = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_H3geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 0;
      geom->NSFACE->im[1][0] = NBSOMM-2;
   }


   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]        = 1;
      geom->REF_A->ive[1]        = 2;
   } 
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S2geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 1;
      geom->NSFACE->im[1][0] = NBSOMM-1;
   }

   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]        = 1;
      geom->REF_A->ive[1]        = 2;
   }

}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S3geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 1;
      geom->NSFACE->im[1][0] = NBSOMM-2;
   }

   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]        = 1;
      geom->REF_A->ive[1]        = 2;
   }
   
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S5geom_init_matrix_nsface_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->NSFACE->im[0][0] = 2;
      geom->NSFACE->im[1][0] = NBSOMM-3;
   }

   
   /* initialisation a zero */
   iv_zero(geom->REF_A);         /* OK if periodic geometry */

   /* references on faces */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_A->ive[0]        = 1;
      geom->REF_A->ive[1]        = 2;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P1geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;
   
   geom->XSOMM->ve[0] = geom->X_LEFT;
   for (s=1; s<NX; s++)
   {
      geom->XSOMM->ve[s] = geom->XSOMM->ve[s-1] + H1->ve[s-1];
   }

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT;
   }


   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[0]        = 1;
      geom->REF_S->ive[NBSOMM-1] = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P2geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;

   geom->XSOMM->ve[0] = geom->X_LEFT;
   for (s=1; s<NX; s++)
   {
      geom->XSOMM->ve[2*s-1] = geom->XSOMM->ve[2*(s-1)] + H1->ve[s-1]/2.0;
      geom->XSOMM->ve[2*s  ] = geom->XSOMM->ve[2*(s-1)] + H1->ve[s-1]    ;
   }

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->XSOMM->ve[NBSOMM-1] = geom->XSOMM->ve[NBSOMM-2] + H1->ve[NX-1]/2.0 ;
   }
   else
   {
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT;
      geom->XSOMM->ve[NBSOMM-2] = geom->XSOMM->ve[NBSOMM-1] - H1->ve[NX-1]/2.0 ;
   }

   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[0]        = 1;
      geom->REF_S->ive[NBSOMM-1] = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_P3geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;

   geom->XSOMM->ve[0] = geom->X_LEFT;
   for (s=1; s<NX; s++)
   {
      geom->XSOMM->ve[3*s-2] = geom->XSOMM->ve[3*(s-1)] + H1->ve[s-1]/3.0*1.0 ;
      geom->XSOMM->ve[3*s-1] = geom->XSOMM->ve[3*(s-1)] + H1->ve[s-1]/3.0*2.0 ;
      geom->XSOMM->ve[3*s  ] = geom->XSOMM->ve[3*(s-1)] + H1->ve[s-1]/3.0*3.0 ;
   }

   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->XSOMM->ve[NBSOMM-1] = geom->XSOMM->ve[NBSOMM-3] + H1->ve[NX-1]/3.0*2.0 ;
      geom->XSOMM->ve[NBSOMM-2] = geom->XSOMM->ve[NBSOMM-3] + H1->ve[NX-1]/3.0*1.0 ;
   }
   else
   {
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT;
      geom->XSOMM->ve[NBSOMM-2] = geom->XSOMM->ve[NBSOMM-1] - H1->ve[NX-1]/3.0*1.0 ;
      geom->XSOMM->ve[NBSOMM-3] = geom->XSOMM->ve[NBSOMM-1] - H1->ve[NX-1]/3.0*2.0 ;
   }


   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[0]        = 1;
      geom->REF_S->ive[NBSOMM-1] = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_H3geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;

   geom->XSOMM->ve[0] = geom->X_LEFT;
   geom->XSOMM->ve[1] = geom->X_LEFT;
   for (s=1; s<NX; s++)
   {
      geom->XSOMM->ve[2*s  ] = geom->XSOMM->ve[2*(s-1)] + H1->ve[s-1] ;
      geom->XSOMM->ve[2*s+1] = geom->XSOMM->ve[2*(s-1)] + H1->ve[s-1] ;
   }

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->XSOMM->ve[NBSOMM-2] = geom->X_RIGHT;
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT;
   }

   
   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[0]        = 1;
      geom->REF_S->ive[NBSOMM-2] = 2; /*le sommet NBSOMM-1 est celui de la derivee */
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S2geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->XSOMM->ve[0] = geom->X_LEFT -  H1->ve[0] ; /* uniform mesh */
      geom->XSOMM->ve[1] = geom->X_LEFT              ;
      for (s=0; s<NX; s++)
      {
         geom->XSOMM->ve[s+2] = geom->XSOMM->ve[s+1] + H1->ve[s] ;
      }
      geom->XSOMM->ve[NBSOMM-2] = geom->X_RIGHT                 ;
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT +  H1->ve[NX-1] ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->XSOMM->ve[0] = geom->X_LEFT;
      for (s=1; s<NX; s++)
      {
         geom->XSOMM->ve[s] = geom->XSOMM->ve[s-1] + H1->ve[s-1];
      }
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S2geom_init_matrix_coords_with_refs");
   }


   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[1]        = 1;
      geom->REF_S->ive[NBSOMM-2] = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S3geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->XSOMM->ve[0] = geom->X_LEFT -  H1->ve[0] ; /* uniform mesh */
      geom->XSOMM->ve[1] = geom->X_LEFT              ;
      for (s=0; s<NX; s++)
      {
         geom->XSOMM->ve[s+2] = geom->XSOMM->ve[s+1] + H1->ve[s] ;
      }
      geom->XSOMM->ve[NBSOMM-2] = geom->X_RIGHT                 ;
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT +  H1->ve[NX-1] ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->XSOMM->ve[0] = geom->X_LEFT;
      for (s=1; s<NX; s++)
      {
         geom->XSOMM->ve[s] = geom->XSOMM->ve[s-1] + H1->ve[s-1];
      }
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S3geom_init_matrix_coords_with_refs");
   }

   
   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[1]        = 1;
      geom->REF_S->ive[NBSOMM-2] = 2;
   }
}

/* ------------------------------------------------------------------------------------------*/

static void     Geom1D_get_S5geom_init_matrix_coords_with_refs(GEOM_1D* geom, const ELT_1D *elt, const VEC* H1)
{
   int NBSOMM = geom->NBSOMM;
   int s;
   int NX = H1->dim;

   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->XSOMM->ve[0] = geom->X_LEFT -  2*H1->ve[0] ; /* uniform mesh */
      geom->XSOMM->ve[1] = geom->X_LEFT -  1*H1->ve[0] ;
      for (s=0; s<NX; s++)
      {
         geom->XSOMM->ve[s+2] = geom->XSOMM->ve[s+1] + H1->ve[s] ;
      }
      geom->XSOMM->ve[NBSOMM-3] = geom->X_RIGHT;
      geom->XSOMM->ve[NBSOMM-2] = geom->X_RIGHT + 1*H1->ve[NX-1] ;
      geom->XSOMM->ve[NBSOMM-1] = geom->X_RIGHT + 2*H1->ve[NX-1] ;
   }
   else
   if ( geom->periodicity == PERIODIC_MESHe )
   {
      geom->XSOMM->ve[0] = geom->X_LEFT;
      for (s=1; s<NX; s++)
      {
         geom->XSOMM->ve[s] = geom->XSOMM->ve[s-1] + H1->ve[s-1];
      }
   }
   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom1D_get_S5geom_init_matrix_coords_with_refs");
   }

   
   /* initialisation a zero */
   iv_zero(geom->REF_S);         /* OK if periodic geometry */

   /* references on somms */
   if ( geom->periodicity == NON_PERIODIC_MESHe )
   {
      geom->REF_S->ive[2]        = 1;
      geom->REF_S->ive[NBSOMM-3] = 2;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

