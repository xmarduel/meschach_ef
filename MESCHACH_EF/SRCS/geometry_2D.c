
/** \file geometry_2D.c
 *
 * Contains the structure of the geometry in 2 dimensions
 *
 *
 * Note : Geom2D_get : actually 4 options :
 *
 *      type="gmsh"  <- gmsh mesh
 *      type="emc2"  <- emc2 mesh
 *      type="quad1" <- mesh from scratch
 *      type="quad2" <- mesh from scratch
 *      type="quad3" <- mesh from scratch
 *
 * --------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"

#include "MESCHACH_EF/INCLUDES/all_params_json_utils.h"

typedef struct {

   VEC *H1 ;
   VEC *H2 ;
   
   /* the references on faces are given explicitely */
   IVEC *SOUTH_FACE_REF_A; 
   IVEC *EAST_FACE_REF_A;
   IVEC *NORTH_FACE_REF_A;
   IVEC *WEST_FACE_REF_A;
   
   /* the references on nodes are given explicitely */
   IMAT *POINTS_REF_S;

} QUAD_MESH_DATA;       /* for the lecture of a quad mesh from  the file MESH_QUAD.dat */


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

static QUAD_MESH_DATA* Geom2D_quad_read_file(const char *meshfile, const char* meshname);

static int athirdpoint_built_or_get(int e, int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP3, const GEOM_2D *GeomP1, int *curr_idx);
static int athirdpoint_built_and_fill_geom(int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP3, const GEOM_2D *GeomP1, int *curr_idx);

static int midpoint_built_or_get   (int e, int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP2, const GEOM_2D *GeomP1, int *curr_idx);
static int midpoint_built_and_fill_geom   (int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP2, const GEOM_2D *GeomP1, int *curr_idx);

static int get_midpoint_idx_if_exist(SPMAT *MID_POINTS, int a, int b);

static int check_two_somm_on_a_face(const GEOM_2D *GeomP1, int a, int b);


static int Geom2D_get_nb_ref_on_somm(const GEOM_2D* Geom);
static int Geom2D_get_nb_ref_on_elts(const GEOM_2D *Geom);
static int Geom2D_get_nb_ref_on_face(const GEOM_2D *Geom);


static GEOM_2D* Geom2D_Base_get(const char *meshfile, const char* meshname, const char *meshtype);

static GEOM_2D* Geom2D_Base_quad1_get(const char *meshfile, const char*meshname);
static GEOM_2D* Geom2D_Base_quad2_get(const char *meshfile, const char*meshname);
static GEOM_2D* Geom2D_Base_quad3_get(const char *meshfile, const char*meshname);
static GEOM_2D* Geom2D_Base_emc2_get (const char *meshfile);
static GEOM_2D* Geom2D_Base_gmsh_get (const char *meshfile);

static GEOM_2D* Geom2D_Parent_get_fromBase(const char* name_ef, const GEOM_2D *GeomP1);
static void     Geom2D_Parent_get_fromBase_P1toP3 ( GEOM_2D *GeomP3 , const GEOM_2D *GeomP1);
static void     Geom2D_Parent_get_fromBase_P1toP2 ( GEOM_2D *GeomP2 , const GEOM_2D *GeomP1);
static void     Geom2D_Parent_get_fromBase_P1toP1b( GEOM_2D *GeomP1b, const GEOM_2D *GeomP1);

static void  Geom2D_checkmeshesP1P3 ( const GEOM_2D *GeomP1, const GEOM_2D *GeomP3 );
static void  Geom2D_checkmeshesP1P2 ( const GEOM_2D *GeomP1, const GEOM_2D *GeomP2 );
static void  Geom2D_checkmeshesP1P1b( const GEOM_2D *GeomP1, const GEOM_2D *GeomP1b);

static GEOM_2D *Geom2D_get_fromParent_P2toP1( const ELT_2D *element, const GEOM_2D* geom );
static GEOM_2D *Geom2D_get_fromParent_P3toP1( const ELT_2D *element, const GEOM_2D* geom );

static GEOM_2D *Geom2D_copy_from(const GEOM_2D* geom);

#define GEOM_2D_NB_HOLES_MAX      10 /* change this if error */


static void Geom2D_Base_gmsh_extract_dimensions        (FILE *fp, GEOM_2D *Geom);
static void Geom2D_Base_gmsh_get_fill_geom             (FILE *fp, GEOM_2D *Geom);


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

GEOM_2D *Geom2D_get( const ELT_2D *element, const char *meshfile, const char* meshname, const char *meshtype)
{
   GEOM_2D *GeomP1   = NULL;
   GEOM_2D *GeomP2   = NULL;
   GEOM_2D *GeomP3   = NULL;
   GEOM_2D *GeomP1b  = NULL;
   GEOM_2D *Geom     = NULL;
   
   if ( element  == NULL )  error(E_NULL, "Geom2D_get");
   if ( meshfile == NULL )  error(E_NULL, "Geom2D_get");

   
   
   GeomP1 = Geom2D_Base_get(meshfile, meshname, meshtype); /* for element "P1" */
      
   if ( (strcmp(element->name_ef,"P1")==0 ) )
   {        
      Geom = GeomP1;    
   }
   else
   if ( (strcmp(element->name_ef,"P1b")==0 ) )
   {        
      GeomP1b = Geom2D_Parent_get_fromBase("P1b", GeomP1);
      Geom    = GeomP1b; 
   }
   else
   if ( (strcmp(element->name_ef,"P2")==0 ) )
   {        
      GeomP2  = Geom2D_Parent_get_fromBase("P2", GeomP1);
      Geom    = GeomP2; 
   }
   else
   if ( (strcmp(element->name_ef,"P3")==0 ) )
   {        
      GeomP3  = Geom2D_Parent_get_fromBase("P3", GeomP1);
      GeomP2  = Geom2D_Parent_get_fromBase("P2", GeomP1);
      Geom    = GeomP3;
      Geom->geomBase = GeomP2;
   }
   else      
   {
      error2(E_EF_NOTIMPL, "Geom2D_get");
   }

   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

int Geom2D_free(GEOM_2D *Geom)
{
   
   if ( Geom == (GEOM_2D *)NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_GEOM_2D, sizeof(GEOM_2D), 0, MY_LIST3);
         mem_numvar_list(TYPE_GEOM_2D, -1, MY_LIST3);
      }
      
      /* free the structure GEOM_2D */
      
      IV_FREE(Geom->REF_S);
      IV_FREE(Geom->REF_T);
      IV_FREE(Geom->REF_A);
      
      M_FREE(Geom->XYSOMM);
      IM_FREE(Geom->NSELMT);
      IM_FREE(Geom->NSFACE);
      
      /* may be also Geom->Geom_P1 to free */
      if ( Geom->geomBase != (GEOM_2D *)NULL ) GEOM_2D_FREE(Geom->geomBase);
      
      free(Geom);
      
      return EXIT_SUCCESS;
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void Geom2D_foutput(FILE *fp, GEOM_2D *Geom)
{
   if ( Geom == NULL ) error(E_NULL, "Geom2D_foutput");
 
   
   fprintf(fp, "TYPE OF MESH : %s \n" ,Geom->type);
      
   fprintf(fp, "#NBSOMM = %d \n",Geom->NBSOMM);
   fprintf(fp, "#NBELMT = %d \n",Geom->NBELMT);
   fprintf(fp, "#NBFACE = %d \n",Geom->NBFACE);
      
   fprintf(fp, "#REF_S : "); iv_foutput(fp,Geom->REF_S);
   fprintf(fp, "#REF_T : "); iv_foutput(fp,Geom->REF_T);
   fprintf(fp, "#REF_A : "); iv_foutput(fp,Geom->REF_A);
      
   fprintf(fp, "#XYSOMM : ");  m_foutput(fp,Geom->XYSOMM);
   fprintf(fp, "#NSELMT : "); im_foutput(fp,Geom->NSELMT);
   fprintf(fp, "#NSFACE : "); im_foutput(fp,Geom->NSFACE);
   
   fprintf(fp, "#nb_ref_on_somm = %d \n",Geom2D_get_nb_ref_on_somm(Geom) );
   fprintf(fp, "#nb_ref_on_elts = %d \n",Geom2D_get_nb_ref_on_elts(Geom) );
   fprintf(fp, "#nb_ref_on_face = %d \n",Geom2D_get_nb_ref_on_face(Geom) );

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D *Geom2D_Base_get( const char *meshfile, const char *meshname, const char *meshtype )
{
   GEOM_2D *Geom;

   if ( meshtype == NULL ) error(E_NULL, "Geom2D_Base_get");
   if ( meshfile == NULL ) error(E_NULL, "Geom2D_Base_get");   
   
   if ( strcmp(meshtype,"gmsh") == 0 )
   {
      Geom = Geom2D_Base_gmsh_get(meshfile);
   }
   
   else if ( strcmp(meshtype,"emc2") == 0 )
   {
      Geom = Geom2D_Base_emc2_get(meshfile);
   }
   
   else if ( strcmp(meshtype,"quad1") == 0 )
   {
      Geom = Geom2D_Base_quad1_get(meshfile, meshname);
   }
   
   else if ( strcmp(meshtype,"quad2") == 0 )
   {
      Geom = Geom2D_Base_quad2_get(meshfile, meshname);
   }
   
   else if ( strcmp(meshtype,"quad3") == 0 )
   {
      Geom = Geom2D_Base_quad3_get(meshfile, meshname);
   }

   else
   {
      error3(E_GEOM_TYPENOTIMPL, "Geom2D_Base_get");
   }
   
   /* no child geometry if element already "P1" */
   Geom->geomBase = (GEOM_2D *)NULL ; 
   
   /* and return */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D* Geom2D_Base_emc2_get(const char *meshfile)
{
   int s,e,a;
   
   GEOM_2D* Geom;
   FILE*    fp;

   if ( meshfile == NULL ) error(E_NULL, "Geom2DBase_emc2_get");  

   fp = fopen(meshfile, "r");

   if ( fp == NULL )
   {
      error(E_FNF, "Geom2DBase_emc2_get");  
   }

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2DBase_emc2_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
   
   
   /* IT IS A "MSH" FORMAT */
   strncpy(Geom->type,"emc2", 16);
   Geom->type[15] = '\0';
   
   
   fscanf(fp, "%d %d %d",&(Geom->NBSOMM),&(Geom->NBELMT),&(Geom->NBFACE)); /*REACH_EOL(fp);*/
   
   Geom->XYSOMM = m_get(Geom->NBSOMM,2);
   Geom->NSELMT = im_get(Geom->NBELMT,3);
   Geom->NSFACE = im_get(Geom->NBFACE,2);
   
   Geom->REF_S = iv_get(Geom->NBSOMM);
   Geom->REF_T = iv_get(Geom->NBELMT) ;
   Geom->REF_A = iv_get(Geom->NBFACE);   
   
   for (s=0; s<Geom->NBSOMM; s++)
   {
      fscanf(fp, "%lf %lf %d\n",
         &(Geom->XYSOMM->me[s][0]),
         &(Geom->XYSOMM->me[s][1]),
         &(Geom->REF_S->ive[s])  );
   }
   
   for (e=0; e<Geom->NBELMT; e++)
   {
      fscanf(fp, "%d %d %d %d\n",
         &(Geom->NSELMT->im[e][0]),
         &(Geom->NSELMT->im[e][1]),
         &(Geom->NSELMT->im[e][2]),
         &(Geom->REF_T->ive[e])  );
   }
   
   for (a=0; a<Geom->NBFACE; a++)
   {
      fscanf(fp, "%d %d %d \n",
         &(Geom->NSFACE->im[a][0]),
         &(Geom->NSFACE->im[a][1]),
         &(Geom->REF_A->ive[a])  );
   }


   /* and renumber Geom->NSELMT for "C" notations */
   for (e=0; e<Geom->NBELMT; e++)
   {
      Geom->NSELMT->im[e][0] = Geom->NSELMT->im[e][0] -1 ;
      Geom->NSELMT->im[e][1] = Geom->NSELMT->im[e][1] -1 ;
      Geom->NSELMT->im[e][2] = Geom->NSELMT->im[e][2] -1 ;
   }
   /* and renumber Geom->NSFACE for "C" notations */
   for (a=0; a<Geom->NBFACE; a++)
   {
      Geom->NSFACE->im[a][0] = Geom->NSFACE->im[a][0] -1 ;
      Geom->NSFACE->im[a][1] = Geom->NSFACE->im[a][1] -1 ;
   }
   
   fclose(fp);
   
   return Geom;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom2D_Base_gmsh_extract_dimensions(FILE *fp, GEOM_2D *MyGeom)
{
   int ALL_ELMT, TYPE_ELMT;
   int s,e;
   int gmsh_idx;
   char A_LINE[128];
   char *res = NULL;
   
   int NBXXXX=0;
   int NBSOMM=0; /* init */
   int NBELMT=0; /* init */
   int NBFACE=0; /* init */
   int NBLINE=0; /* init */
   int NBPNKT=0; /* init */
   
   fgets(A_LINE, 128, fp);
   /* find "$MeshFormat" line */
   while ( strncmp("$MeshFormat", A_LINE, 11) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_2D_GMSH_CANNOT_FIND_MESHFORMAT_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   
   rewind(fp);
   
   /* find "$Nodes" line */
   while ( strncmp("$Nodes", A_LINE, 6) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_2D_GMSH_CANNOT_FIND_NODE_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   
   /* the number of SOMMS */
   fscanf(fp, "%d \n", &NBSOMM);

   rewind(fp);
   
   fgets(A_LINE, 128, fp);
   /* find "$Elements" line */
   while ( strncmp("$Elements", A_LINE, 9) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_2D_GMSH_CANNOT_FIND_ELEMENT_LINE, "Geom2D_Base_gmsh_extract_dimensions");
      }
   }
   /* The number of ALL the structures : points, lines, triangles */
   fscanf(fp, "%d \n", &ALL_ELMT);
   /* Now extract the number of triangles */
   for (e=0; e<ALL_ELMT; e++)
   {
      fscanf(fp, "%d %d", &gmsh_idx, &TYPE_ELMT); REACH_EOL(fp);

      switch(TYPE_ELMT)
      {
         case GMSH_POINT:        NBPNKT++; break;
         case GMSH_P1_LINE:      NBFACE++; break;
         case GMSH_P1_TRIANGLE:  NBELMT++; break;
         default:                NBXXXX++; break;
      }
   }

   if ( (NBPNKT+NBLINE+NBFACE+NBELMT+NBXXXX) != (ALL_ELMT) )
   {
      error3(E_GEOM_2D_GMSH_WRONG_ELT_NUMBER, "Geom3D_Base_gmsh_get_fill_geom");
   }

   /* fill dimensions */
   MyGeom->NBSOMM = NBSOMM;
   MyGeom->NBELMT = NBELMT;
   MyGeom->NBFACE = NBFACE;

   /* finito */
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void Geom2D_Base_gmsh_get_fill_geom(FILE *fp, GEOM_2D *Geom)
{
   int NBSOMM;
   int s, e, num, num_max = 0;
   double z;

   int ALL_ELMT, TYPE_ELMT;
   int intern;
   int gmsh_physical_idx;
   int gmsh_entity_idx;
   int n1,n2,n3;
   int na=0;
   int ne=0;
   int np=0;
   char A_LINE[128];
   char *res = NULL;

   IVEC *MAP_IDX = iv_get(Geom->NBSOMM);
   IVEC *XMAP_IDX;

   fgets(A_LINE, 128, fp);
   /* find "$Nodes" line */
   while ( strncmp("$Nodes", A_LINE, 6) != 0 )
   {
      res = fgets(A_LINE, 128, fp);
      
      if ( res == NULL )
      {
         error3(E_GEOM_2D_GMSH_CANNOT_FIND_NODE_LINE, "Geom2D_Base_gmsh_extract_dimensions");
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
             &z );

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
         error3(E_GEOM_2D_GMSH_CANNOT_FIND_NODE_LINE, "Geom2D_Base_gmsh_extract_dimensions");
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
            REACH_EOL(fp); 
            
            np++;
            
            break;

         case GMSH_P1_LINE:
            fscanf(fp, "%d %d %d %d %d\n", &intern, &gmsh_physical_idx, &gmsh_entity_idx, &n1, &n2 );

            Geom->REF_A->ive[na] = gmsh_physical_idx;

            Geom->REF_S->ive[XMAP_IDX->ive[n1]] = gmsh_physical_idx;
            Geom->REF_S->ive[XMAP_IDX->ive[n2]] = gmsh_physical_idx;

            Geom->NSFACE->im[na][0] = XMAP_IDX->ive[n1];
            Geom->NSFACE->im[na][1] = XMAP_IDX->ive[n2];

            na++;

            break;

         case GMSH_P1_TRIANGLE:
            fscanf(fp, "%d %d %d %d %d %d\n", &intern, &gmsh_physical_idx, &gmsh_entity_idx, &n1, &n2, &n3);

            Geom->REF_T->ive[ne] = gmsh_physical_idx;

            Geom->NSELMT->im[ne][0] = XMAP_IDX->ive[n1];
            Geom->NSELMT->im[ne][1] = XMAP_IDX->ive[n2];
            Geom->NSELMT->im[ne][2] = XMAP_IDX->ive[n3];

            ne++;

            break;
            
         default:
            printf("unkwown entity!!! : %d\n", TYPE_ELMT);
      }
   }

   if ( (ne+na+np) != (ALL_ELMT) )
   {
      error3(E_GEOM_2D_GMSH_WRONG_ELT_NUMBER, "Geom2D_Base_gmsh_get_fill_geom");
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D* Geom2D_Base_gmsh_get(const char *meshfile)
{
   int NBSOMM=0;
   int NBELMT=0;
   int NBFACE=0;
   int NBPNKT=0;
   int NBLINE=0;
   
   GEOM_2D * Geom;
   FILE *    fp;


   if ( meshfile == NULL ) error(E_NULL, "Geom2DBase_gmsh_get"); 
   
   fp = fopen(meshfile, "r");
   
   if ( fp == NULL )
   {
      error(E_FNF, "Geom2DBase_gmsh_get");  
   }
   
   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D))== (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2DBase_gmsh_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
    
   /* IT IS A GMSH FORMAT */
   strncpy(Geom->type,"gmsh", 16);
   Geom->type[15] = '\0';
   
   /* get mesh dimensions */
   Geom2D_Base_gmsh_extract_dimensions(fp, Geom);
   
   /* allocate memory */
   Geom->XYSOMM = m_get(Geom->NBSOMM,2); 
   Geom->NSELMT = im_get(Geom->NBELMT,3) ;
   Geom->NSFACE = im_get(Geom->NBFACE,2) ;
   
   Geom->REF_S = iv_get(Geom->NBSOMM);
   Geom->REF_T = iv_get(Geom->NBELMT);
   Geom->REF_A = iv_get(Geom->NBFACE);
   
   /* rewind the file and re-read and fill the arrays */
   rewind(fp);
   
   /**/
   Geom2D_Base_gmsh_get_fill_geom(fp, Geom);

   /**/   
   fclose(fp);

   /* finito */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_Base_quad_set_all_refs(GEOM_2D *Geom, const QUAD_MESH_DATA* quad_mesh_data)
{
   int i,j;
   int NX,NY;
   
   if ( Geom           == NULL )  error(E_NULL, "Geom2D_Base_quad_set_all_refs");
   if ( quad_mesh_data == NULL )  error(E_NULL, "Geom2D_Base_quad_set_all_refs");  
   
   /* we dont reference any triangle yet == uniform media */
   Geom->REF_T = iv_get(Geom->NBELMT);
   iv_zero(Geom->REF_T);
   
   /* allocate memory for aretes references */
   Geom->REF_A = iv_get(Geom->NBFACE);
   iv_zero(Geom->REF_A);
   
   /* allocate memory for aretes references */
   Geom->REF_S = iv_get(Geom->NBSOMM);
   iv_zero(Geom->REF_S);
   
   NX = quad_mesh_data->H1->dim;
   NY = quad_mesh_data->H2->dim;
   
   /* the references on faces are given explicitely */
   for ( i=0 ;i<NX ;i++) Geom->REF_A->ive[i]          = quad_mesh_data->SOUTH_FACE_REF_A->ive[i]; 
   for ( i=0 ;i<NY ;i++) Geom->REF_A->ive[i+NX]       = quad_mesh_data->EAST_FACE_REF_A->ive[i];
   for ( i=0 ;i<NX ;i++) Geom->REF_A->ive[i+NX+NY]    = quad_mesh_data->NORTH_FACE_REF_A->ive[i];
   for ( i=0 ;i<NY ;i++) Geom->REF_A->ive[i+NX+NY+NX] = quad_mesh_data->WEST_FACE_REF_A->ive[i];
   
   /* the references on nodes are given explicitely */
   for (j=0; j<=NY; j++)
   {
      for (i=0; i<=NX; i++)
      {
         int s = i + j*(NX+1) ; 
         Geom->REF_S->ive[s] = quad_mesh_data->POINTS_REF_S->im[i][j];
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_Base_quad_set_nselmt(GEOM_2D *Geom, const QUAD_MESH_DATA* quad_mesh_data)
{
   int i,j,e;
   int NBSOMM = Geom->NBSOMM;
   int s1,s2,s3,s4;

   int N1 = quad_mesh_data->H1->dim;
   int N2 = quad_mesh_data->H2->dim;
   
   /* allocate */
   Geom->NSELMT = im_get(Geom->NBELMT,3) ;

   /*---------------------------------------*/
   /* numerotation des sommets des elements */
   /*---------------------------------------*/

   for ( j=0 ; j<N2 ; j++ )
   for ( i=0 ; i<N1 ; i++ )
   {
      /* pro "quadra" 2 cells */
      e = 2*(i + j*N1);

      /* the 4 somm on this quadra */  /*  s4 --------- s3    */   /* "quad1" */
      s1 = i   + (j+0)*(N1+1) ;        /*     |     * |       */
      s2 = i+1 + (j+0)*(N1+1) ;        /*     |   *   |       */
      s3 = i+1 + (j+1)*(N1+1) ;        /*     | *     |       */
      s4 = i   + (j+1)*(N1+1) ;        /*  s1 --------- s2    */

                                       /*  s4 --------- s3    s4 --------- s3   */  /* "quad2" */ /* "quad3" */
                                       /*     |     * |          | *     |      */
                                       /*     |   *   |          |   *   |      */
                                       /*     | *     |          |     * |      */
                                       /*  s1 --------- s2    s1 --------- s2   */

      assert( s1 < NBSOMM );
      assert( s2 < NBSOMM );
      assert( s3 < NBSOMM );
      assert( s4 < NBSOMM );
      
      assert( e  < Geom->NBELMT );
      
      if ( strcmp("quad1", Geom->type) == 0 ) /* 1 types of "basic quadra" (with 2 triangles) */
      {
         Geom->NSELMT->im[e+0][0] = s2 ;
         Geom->NSELMT->im[e+0][1] = s3 ;
         Geom->NSELMT->im[e+0][2] = s1 ;
         
         Geom->NSELMT->im[e+1][0] = s4 ;
         Geom->NSELMT->im[e+1][1] = s1 ;
         Geom->NSELMT->im[e+1][2] = s3 ;
      }
      else
      if ( strcmp("quad2", Geom->type) == 0 ) /* 2 types of "basic quadra" (with 2 triangles) */
      {
         if ( ((i<N1/2)&&(j<N2/2)) || ((i>=N1/2)&&(j>=N2/2)) )
         {
            Geom->NSELMT->im[e+0][0] = s2 ;
            Geom->NSELMT->im[e+0][1] = s3 ;
            Geom->NSELMT->im[e+0][2] = s1 ;

            Geom->NSELMT->im[e+1][0] = s4 ;
            Geom->NSELMT->im[e+1][1] = s1 ;
            Geom->NSELMT->im[e+1][2] = s3 ;
         }
         else
         {
            Geom->NSELMT->im[e+0][0] = s1 ;
            Geom->NSELMT->im[e+0][1] = s2 ;
            Geom->NSELMT->im[e+0][2] = s4 ;

            Geom->NSELMT->im[e+1][0] = s3 ;
            Geom->NSELMT->im[e+1][1] = s4 ;
            Geom->NSELMT->im[e+1][2] = s2 ;
         }
      }
      else
      if ( strcmp("quad3", Geom->type) == 0 ) /* 2 types of "basic quadras" (with 2 triangles) */
      {
         if ( (i+j)%2 == 0 )
         {
            Geom->NSELMT->im[e+0][0] = s2 ;
            Geom->NSELMT->im[e+0][1] = s3 ;
            Geom->NSELMT->im[e+0][2] = s1 ;

            Geom->NSELMT->im[e+1][0] = s4 ;
            Geom->NSELMT->im[e+1][1] = s1 ;
            Geom->NSELMT->im[e+1][2] = s3 ;
         }
         else
         {
            Geom->NSELMT->im[e+0][0] = s1 ;
            Geom->NSELMT->im[e+0][1] = s2 ;
            Geom->NSELMT->im[e+0][2] = s4 ;

            Geom->NSELMT->im[e+1][0] = s3 ;
            Geom->NSELMT->im[e+1][1] = s4 ;
            Geom->NSELMT->im[e+1][2] = s2 ;
         }
      }
      else
      {
         error(E_UNKNOWN, "Geom2D_Base_quad_set_nselmt");
      }
   }
}

/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_Base_quad_set_nsface(GEOM_2D *Geom, const QUAD_MESH_DATA* quad_mesh_data)
{
   int i;

   int N1 = quad_mesh_data->H1->dim;
   int N2 = quad_mesh_data->H2->dim;
   
   /* allocate */
   Geom->NSFACE = im_get(Geom->NBFACE,2) ;

   /*initialisation a ZERO */
   for (i=0; i<Geom->NBFACE; i++ )
   {
      Geom->NSFACE->im[i][0] = 0 ;
      Geom->NSFACE->im[i][1] = 0 ;
   }

   /* set values */
   for ( i=0 ; i<N1 ; i++ )
   {
      Geom->NSFACE->im[i][0] = i;
      Geom->NSFACE->im[i][1] = Geom->NSFACE->im[i][0] + 1;
   }

   for ( i=0 ; i<N2 ; i++ )
   {
      Geom->NSFACE->im[i+N1][0] = Geom->NSFACE->im[i+N1 -1][1]       ;
      Geom->NSFACE->im[i+N1][1] = Geom->NSFACE->im[i+N1   ][0] + N1+1;
   }

   for ( i=0 ; i<N1 ; i++ )
   {
      Geom->NSFACE->im[i+N1+N2][0] = Geom->NSFACE->im[i+N1+N2 -1][1]    ;
      Geom->NSFACE->im[i+N1+N2][1] = Geom->NSFACE->im[i+N1+N2   ][0] - 1;
   }

   for ( i=0 ; i<N2 ; i++ )
   {
      Geom->NSFACE->im[i+2*N1+N2][0] = Geom->NSFACE->im[i+2*N1+N2 -1][1]       ;
      Geom->NSFACE->im[i+2*N1+N2][1] = Geom->NSFACE->im[i+2*N1+N2   ][0] -N1-1 ;
   }   
}

/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_Base_quad_set_xysomm(GEOM_2D *Geom, const QUAD_MESH_DATA *quad_mesh_data)
{
   int i,j,s;

   int N1 = quad_mesh_data->H1->dim;
   int N2 = quad_mesh_data->H2->dim;

   /* allocate */
   Geom->XYSOMM = m_get(Geom->NBSOMM,2);

   /* and fill */
   for ( j=0 ; j<N2+1 ; j++ )
   for ( i=0 ; i<N1+1 ; i++ )
   {
      s = i + j*(N1+1)  ;

      if ( j==0 )
         m_set_val(Geom->XYSOMM,s,1, 0.0);
      else
         m_set_val(Geom->XYSOMM,s,1, Geom->XYSOMM->me[s-N1-1][1]     + quad_mesh_data->H2->ve[j-1]);

      if ( i==0 )
         m_set_val(Geom->XYSOMM,s,0, 0.0);
      else
         m_set_val(Geom->XYSOMM,s,0, Geom->XYSOMM->me[s-1][0]        + quad_mesh_data->H1->ve[i-1]);
   }
}

/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D* Geom2D_Base_quad1_get(const char *meshfile, const char *meshname)
{
   GEOM_2D * Geom;
   QUAD_MESH_DATA *quad_mesh_data;
   
   int NX,NY;
   
   if ( meshfile == NULL ) error(E_NULL, "Geom2DBase_quad1_get");
   if ( meshname == NULL ) error(E_NULL, "Geom2DBase_quad1_get");
   
   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2DBase_quad1_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
   
   strncpy(Geom->type, "quad1", 16);

   /* read from file mesh dimensions and others stuff */
   quad_mesh_data = Geom2D_quad_read_file(meshfile, meshname);
   
   NX = quad_mesh_data->H1->dim;
   NY = quad_mesh_data->H2->dim;
   
   /* --- FOR A P1 MESH --- */
   Geom->NBSOMM = (NX+1)*(NY+1)     ;
   Geom->NBELMT = 2*NX*NY           ;
   Geom->NBFACE = 2*NX + 2*NY       ;
   
   /* remplissage des REFERENCES */
   Geom2D_Base_quad_set_all_refs(Geom, quad_mesh_data);
   
   /* remplissage de NSELMT */
   Geom2D_Base_quad_set_nselmt(Geom, quad_mesh_data);
         
   /* remplissage de NSFACE */
   Geom2D_Base_quad_set_nsface(Geom, quad_mesh_data);
   
   /* remplissage de XYSOMM */
   Geom2D_Base_quad_set_xysomm(Geom, quad_mesh_data);
   
   /* clean */
   V_FREE(quad_mesh_data->H1);
   V_FREE(quad_mesh_data->H2);
   IV_FREE(quad_mesh_data->SOUTH_FACE_REF_A);
   IV_FREE(quad_mesh_data->EAST_FACE_REF_A);
   IV_FREE(quad_mesh_data->NORTH_FACE_REF_A);
   IV_FREE(quad_mesh_data->WEST_FACE_REF_A);
   IM_FREE(quad_mesh_data->POINTS_REF_S);
   
   free(quad_mesh_data);
   
   /* finito */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D* Geom2D_Base_quad2_get(const char *meshfile, const char *meshname)
{
   GEOM_2D * Geom;
   QUAD_MESH_DATA *quad_mesh_data;
   
   int NX,NY;
   
   if ( meshfile == NULL ) error(E_NULL, "Geom2DBase_quad2_get");
   if ( meshname == NULL ) error(E_NULL, "Geom2DBase_quad2_get");
   
   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D))== (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2DBase_quad2_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
   
   strncpy(Geom->type, "quad2", 16);
   
   /* read from file mesh dimensions and others stuff */
   quad_mesh_data = Geom2D_quad_read_file(meshfile, meshname);

   NX = quad_mesh_data->H1->dim;
   NY = quad_mesh_data->H2->dim;

   /* --- FOR A P1 MESH --- */
   Geom->NBSOMM = (NX+1)*(NY+1)     ;
   Geom->NBELMT = 2*NX*NY           ;
   Geom->NBFACE = 2*NX + 2*NY       ;
   
   /* remplissage des REFERENCES */
   Geom2D_Base_quad_set_all_refs(Geom, quad_mesh_data);
   
   /* remplissage de NSELMT */
   Geom2D_Base_quad_set_nselmt(Geom, quad_mesh_data);
   
   /* remplissage de NSFACE */
   Geom2D_Base_quad_set_nsface(Geom, quad_mesh_data);

   /* remplissage de XYSOMM */
   Geom2D_Base_quad_set_xysomm(Geom, quad_mesh_data);
   
   /* clean */
   V_FREE(quad_mesh_data->H1);
   V_FREE(quad_mesh_data->H2);
   IV_FREE(quad_mesh_data->SOUTH_FACE_REF_A);
   IV_FREE(quad_mesh_data->EAST_FACE_REF_A);
   IV_FREE(quad_mesh_data->NORTH_FACE_REF_A);
   IV_FREE(quad_mesh_data->WEST_FACE_REF_A);
   IM_FREE(quad_mesh_data->POINTS_REF_S);
   free(quad_mesh_data);

   /* finito */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D* Geom2D_Base_quad3_get(const char *meshfile, const char *meshname)
{
   GEOM_2D * Geom;
   QUAD_MESH_DATA *quad_mesh_data;
   
   int NX,NY;
   
   if ( meshfile == NULL ) error(E_NULL, "Geom2DBase_quad3_get");
   if ( meshname == NULL ) error(E_NULL, "Geom2DBase_quad3_get");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2DBase_quad3_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(Geom->type, "quad3", 16);

   /* read from file mesh dimensions and others stuff */
   quad_mesh_data = Geom2D_quad_read_file(meshfile, meshname);
   
   NX = quad_mesh_data->H1->dim;
   NY = quad_mesh_data->H2->dim;

   /* --- FOR A P1 MESH --- */
   Geom->NBSOMM = (NX+1)*(NY+1)     ;
   Geom->NBELMT = 2*NX*NY           ;
   Geom->NBFACE = 2*NX + 2*NY       ;
   
   /* remplissage des REFERENCES */
   Geom2D_Base_quad_set_all_refs(Geom, quad_mesh_data);

   /* remplissage de NSELMT */
   Geom2D_Base_quad_set_nselmt(Geom, quad_mesh_data);

   /* remplissage de NSFACE */
   Geom2D_Base_quad_set_nsface(Geom, quad_mesh_data);

   /* remplissage de XYSOMM */
   Geom2D_Base_quad_set_xysomm(Geom, quad_mesh_data);

   /* clean */
   V_FREE(quad_mesh_data->H1);
   V_FREE(quad_mesh_data->H2);
   IV_FREE(quad_mesh_data->SOUTH_FACE_REF_A);
   IV_FREE(quad_mesh_data->EAST_FACE_REF_A);
   IV_FREE(quad_mesh_data->NORTH_FACE_REF_A);
   IV_FREE(quad_mesh_data->WEST_FACE_REF_A);
   IM_FREE(quad_mesh_data->POINTS_REF_S);
   free(quad_mesh_data);

   /* finito */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static QUAD_MESH_DATA* Geom2D_quad_read_file(const char *meshfile , const char* meshname)
{
   QUAD_MESH_DATA* quad_mesh_data = (QUAD_MESH_DATA*)malloc(sizeof(QUAD_MESH_DATA));
   
   int i,j;
   
   int NX;
   int NY;
   
   int nb_ref_on_face;
   int nb_ref_on_somm;
   
   if ( meshfile == NULL )  error(E_NULL, "Geom2D_quad_read_file");
   if ( meshname == NULL )  error(E_NULL, "Geom2D_quad_read_file");  
   
   // NEW : read JSON file ...
   char validation_output[8192];
   char *meshfile_schema = "/Users/xavier/DEVELOPMENT/MESCHACH_WORK/JSON_SCHEMAS/SCHEMA_MESH_2D.json";
   int status = json_check_data(meshfile, meshfile_schema, validation_output);
   
   printf("mesh %s : %s\n", meshfile, validation_output);
   
   if ( status != 0 )
   {
      error3(E_GEOM_MESHFILE_JSON_VALIDATION, "Geom2D_quad_read_file");
      fprintf(stderr, "%s", validation_output);
   }
   
   json_t *meshes = json_load_xfile(meshfile);
   json_t *mesh   = json_object_get(meshes, meshname);
   
   if ( mesh == NULL )
   {
      error3(E_GEOM_MESHFILE_JSON_MESH_NOT_FOUND, "Geom2D_quad_read_file");
   }
   
   json_unpack(mesh, "{s:i, s:i}", "NX", &NX, "NY", &NY);

   json_t *x_segments = json_object_get(mesh, "X_AXIS_SEGMENTS");
   json_t *y_segments = json_object_get(mesh, "Y_AXIS_SEGMENTS");
   
   if ( NX != json_array_size(x_segments) )
   {
      error3(E_GEOM_MESH2D_JSON_X_AXIS_SEGMENTS_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   if ( NY != json_array_size(y_segments) )
   {
      error3(E_GEOM_MESH2D_JSON_Y_AXIS_SEGMENTS_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   
   quad_mesh_data->H1 = v_get(NX);
   quad_mesh_data->H2 = v_get(NY);
   
   quad_mesh_data->SOUTH_FACE_REF_A = iv_get(NX);
   quad_mesh_data->EAST_FACE_REF_A  = iv_get(NY);
   quad_mesh_data->NORTH_FACE_REF_A = iv_get(NX);
   quad_mesh_data->WEST_FACE_REF_A  = iv_get(NY);
   
   quad_mesh_data->POINTS_REF_S     = im_get(NX+1,NY+1);
   
   
   for (i=0; i<NX; i++) quad_mesh_data->H1->ve[i] = json_real_value(json_array_get(x_segments, i)); 
   for (j=0; j<NY; j++) quad_mesh_data->H2->ve[j] = json_real_value(json_array_get(y_segments, j)); 

   /* we read the references on the faces */
   json_t *side_infos = json_object_get(mesh, "SIDES_INFOS");
   json_unpack(side_infos, "{s:i}", "NB_REFS", &nb_ref_on_face); /* the number of references */ 
   
   json_t *side_idxs = json_object_get(side_infos, "SIDES_IDX");
   
   json_t *south_face = json_object_get(side_idxs, "SOUTH");
   json_t *east_face  = json_object_get(side_idxs, "EAST");
   json_t *north_face = json_object_get(side_idxs, "NORTH");
   json_t *west_face  = json_object_get(side_idxs, "WEST");
   
   
   if ( NX != json_array_size(south_face) )
   {
      error3(E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_SOUTH_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   if ( NX != json_array_size(north_face) )
   {
      error3(E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_NORTH_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   if ( NY != json_array_size(east_face) )
   {
      error3(E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_EAST_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   if ( NY != json_array_size(west_face) )
   {
      error3(E_GEOM_MESH2D_JSON_SIDE_INFOS_SIDE_IDX_WEST_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   
   /* the references on faces are given explicitely */
   for ( i=0 ;i<NX ;i++) quad_mesh_data->SOUTH_FACE_REF_A->ive[i] = json_integer_value(json_array_get(south_face, i)); 
   for ( i=0 ;i<NY ;i++) quad_mesh_data->EAST_FACE_REF_A->ive[i]  = json_integer_value(json_array_get(east_face, i));
   for ( i=0 ;i<NX ;i++) quad_mesh_data->NORTH_FACE_REF_A->ive[i] = json_integer_value(json_array_get(north_face, i));
   for ( i=0 ;i<NY ;i++) quad_mesh_data->WEST_FACE_REF_A->ive[i]  = json_integer_value(json_array_get(west_face, i));
   
   /* we read the references on the nodes */
   json_t *vertice_infos = json_object_get(mesh, "VERTICES_INFOS");
   json_unpack(vertice_infos, "{s:i}", "NB_REFS", &nb_ref_on_somm); /* the number of references */ 
   
   json_t *vertice_idxs = json_object_get(vertice_infos, "VERTICES_IDX");
   
   if ( (NY +1)!= json_array_size(vertice_idxs) )
   {
      error3(E_GEOM_MESH2D_JSON_VERTICES_INFOS_VERTICES_IDX_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
   }
   
   /* the references on  nodes are given explicitely */
   for (j=0; j<=NY; j++)
   {
      json_t *vertice_idxs_row = json_array_get(vertice_idxs, NY-j); // reversed here!
      
      if ( (NX +1)!= json_array_size(vertice_idxs_row) )
      {
         error3(E_GEOM_MESH2D_JSON_VERTICES_INFOS_VERTICES_ROW_IDX_BAD_ARRAY_SIZE, "Geom2D_quad_read_file");
      }
      
      for (i=0; i<=NX; i++)
      {
         quad_mesh_data->POINTS_REF_S->im[i][j] = json_integer_value(json_array_get(vertice_idxs_row, i));
      }
   }

   return quad_mesh_data;
}


/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/

typedef struct
{
   Real x,y;

} point2D;

/*----------------------------------------------------------------------------------------------------*/

static int check_pt_equal_pt( const point2D *pt1, const point2D *pt2)
{
   int ok = 1; /* init */

   if ( (fabs(pt1->x - pt2->x) > MACHEPS) || (fabs(pt1->y - pt2->y) > 1e5*MACHEPS) )
   {
      ok = 0;
   }

   return ok;
}

/*----------------------------------------------------------------------------------------------------*/

static point2D * set_pt1_pt2_center( const point2D *pt1, const point2D *pt2, point2D *center)
{
   center->x = ( pt1->x + pt2->x ) / 2.0 ;
   center->y = ( pt1->y + pt2->y ) / 2.0 ;

   return center;
}

/*----------------------------------------------------------------------------------------------------*/

static point2D * set_pt1_pt2_weightedcenter( const point2D *pt1, const point2D *pt2, double w1, double w2, point2D *pt)
{
   pt->x = ( w1*pt1->x + w2*pt2->x ) / (w1+w2) ;
   pt->y = ( w1*pt1->y + w2*pt2->y ) / (w1+w2) ;

   return pt;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

void Geom2D_checkmeshesP1P1b( const GEOM_2D *GeomP1, const GEOM_2D *GeomP1b)
{
   int e,k;
   point2D onP1[3];
   point2D onP1b[4];

   if ( GeomP1  == NULL )  error(E_NULL, "Geom2D_checkmeshesP1P1b");
   if ( GeomP1b == NULL )  error(E_NULL, "Geom2D_checkmeshesP1P1b");

   for (e=0; e<GeomP1b->NBELMT; e++)
   {      
      for (k=0; k<3; k++)
      {
         onP1[k].x = GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][k]][0];
         onP1[k].y = GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][k]][1];
      }

      for (k=0; k<4; k++)
      {
         onP1b[k].x = GeomP1b->XYSOMM->me[GeomP1b->NSELMT->im[e][k]][0];
         onP1b[k].y = GeomP1b->XYSOMM->me[GeomP1b->NSELMT->im[e][k]][1];
      }

      /* check */
      for (k=0; k<3; k++)
      {
         if ( !check_pt_equal_pt( &onP1[k], &onP1b[k]) )
         {
            printf("Elt %d - Somm No %d (local): zut ! onP1 = [%f - %f] -- onP1b = [%f - %f]\n",
                   e,k, onP1[k].x, onP1[k].y, onP1b[k].y, onP1b[k].y);
         }
      }
   }

   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_checkmeshesP1P2( const GEOM_2D *GeomP1, const GEOM_2D *GeomP2)
{
   int e,k,k1,k2;
   point2D onP1[3];
   point2D onP2[6];
   point2D center;
   
   if ( GeomP1 == NULL )  error(E_NULL, "Geom2D_checkmeshesP1P2");
   if ( GeomP2 == NULL )  error(E_NULL, "Geom2D_checkmeshesP1P2");
   
   for (e=0; e<GeomP2->NBELMT; e++)
   {      
      for (k=0; k<3; k++)
      {
         onP1[k].x = GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][k]][0];
         onP1[k].y = GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][k]][1];
      }

      for (k=0; k<6; k++)
      {
         onP2[k].x = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][k]][0];
         onP2[k].y = GeomP2->XYSOMM->me[GeomP2->NSELMT->im[e][k]][1];
      }

      
      for (k=0; k<3; k++)
      {
         if ( !check_pt_equal_pt( &onP1[k], &onP2[k]) )
         {
            printf("Elt %d - Somm No %d (local): zut ! onP1 = [%f - %f] -- onP2 = [%f - %f]\n",
                   e,k, onP1[k].x, onP1[k].y, onP2[k].y, onP2[k].y);
         }
      }

      for (k=0; k<3; k++)
      {
         switch(k)
         {
            case 0: k1 = 0; k2 = 1; break;
            case 1: k1 = 2; k2 = 1; break;
            case 2: k1 = 2; k2 = 0; break;
         }
         
         /* check the nodes in the middle of the triangles */
         if ( !check_pt_equal_pt( set_pt1_pt2_center(&onP2[k1],&onP2[k2],&center), &onP2[k+3]) )
         {
            printf("Elt %d - Somm No %d (local): zut ! \n", e,k+3);
            /*exit(0);*/
         }
      }
      
   }
   
   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_checkmeshesP1P3( const GEOM_2D *GeomP1, const GEOM_2D *GeomP3)
{
   int e,k,k1,k2;
   point2D onP1[3];
   point2D onP3[10];
   point2D center;
   
   if ( GeomP1 == NULL )  error(E_NULL, "Geom2D_checkmeshesP1P3");
   if ( GeomP3 == NULL )  error(E_NULL, "Geom2D_checkmeshesP1P3");

   for (e=0; e<GeomP3->NBELMT; e++)
   {      
      for (k=0; k<3; k++)
      {
         onP1[k].x = GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][k]][0];
         onP1[k].y = GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][k]][1];
      }

      for (k=0; k<10; k++)
      {
         onP3[k].x = GeomP3->XYSOMM->me[GeomP3->NSELMT->im[e][k]][0];
         onP3[k].y = GeomP3->XYSOMM->me[GeomP3->NSELMT->im[e][k]][1];
      }


      for (k=0; k<3; k++)
      {
         if ( !check_pt_equal_pt( &onP1[k], &onP3[k]) )
         {
            printf("Elt %d - Somm No %d (local): zut ! onP1 = [%f - %f] -- onP3 = [%f - %f]\n",
                   e,k, onP1[k].x, onP1[k].y, onP3[k].y, onP3[k].y);
         }
      }

      /* check the nodes at a third and two-third of a triangle's edge */
      for (k=0; k<3; k++)
      {
         switch(k)
         {
            case 0: k1 = 0; k2 = 1; break;
            case 1: k1 = 1; k2 = 2; break;
            case 2: k1 = 0; k2 = 2; break;
         }

         /* check the nodes at a third of a triangle's edge */
         if ( !check_pt_equal_pt( set_pt1_pt2_weightedcenter(&onP3[k1],&onP3[k2],2.0,1.0,&center), &onP3[3+2*k]) )
         {
            printf("Elt %d - Somm No %d (local): zut ! \n", e,3+2*k);
            /*exit(0);*/
         }

         /* check the nodes at two-third of a triangle's edge */
         if ( !check_pt_equal_pt( set_pt1_pt2_weightedcenter(&onP3[k1],&onP3[k2],1.0,2.0,&center), &onP3[3+2*k+1]) )
         {
            printf("Elt %d - Somm No %d (local): zut ! \n", e,3+2*k+1);
            /*exit(0);*/
         }
      }

      /* check the nodes in the middle of the triangles (as in P1b) -ToDo- */
      
   }

   return;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

static GEOM_2D *Geom2D_Parent_get_fromBase(const char* name_ef, const GEOM_2D *GeomP1)
{
   GEOM_2D *Geom;

   if ( GeomP1  == NULL )  error(E_NULL, "Geom2DParent_get_fromBase");
   if ( name_ef == NULL )  error(E_NULL, "Geom2DParent_get_fromBase");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2DParent_get_fromBase");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */
   
   Geom->geomBase = (GEOM_2D*)GeomP1; /* cast because of the "const" */
      
   if ( strcmp("P3",name_ef) == 0 )
   {
      Geom2D_Parent_get_fromBase_P1toP3(Geom, GeomP1);
   }
   else
   if ( strcmp("P2",name_ef) == 0 )
   {
      Geom2D_Parent_get_fromBase_P1toP2(Geom, GeomP1);
   }
   else
   if ( strcmp("P1b",name_ef) == 0 )
   {
      Geom2D_Parent_get_fromBase_P1toP1b(Geom, GeomP1);
   }
   else
   {
      error(E_UNKNOWN, "Geom2DParent_get_fromBase");
   }
         
   /* return */
   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int get_midpoint_idx_if_exist(SPMAT *MID_POINTS, int a, int b)
{
   SPROW *r = MID_POINTS->row + a;
   int idx;
   
   /* check if we have an element in the row at column b */
   idx = sprow_idx(r,b);

   if ( idx < 0 )
   {
      return -1;
   }
   else
   {
      return (int)(r->elt[idx].val);
   }

   return -1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int midpoint_built_or_get(int e, int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP2, const GEOM_2D *GeomP1, int *curr_idx)
{
   int midpoint = get_midpoint_idx_if_exist(MID_POINTS,a,b);

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

static int athirdpoint_built_or_get(int e, int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP3, const GEOM_2D *GeomP1, int *curr_idx)
{
   int midpoint = get_midpoint_idx_if_exist(MID_POINTS,a,b);

   if ( -1 != midpoint )
   {
      return midpoint;
   }
   else
   {
      return athirdpoint_built_and_fill_geom(a,b, MID_POINTS, GeomP3, GeomP1, curr_idx);
   }
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int midpoint_built_and_fill_geom(int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP2, const GEOM_2D *GeomP1, int *curr_idx)
{
   double xa,ya;
   double xb,yb;
   double xm,ym;
   int new_midpoint = (*curr_idx);
   int ref_on_face;
   
   xa = GeomP1->XYSOMM->me[a][0];
   ya = GeomP1->XYSOMM->me[a][1];
   
   xb = GeomP1->XYSOMM->me[b][0];
   yb = GeomP1->XYSOMM->me[b][1];
   
   xm = (xa+xb)/2.0;
   ym = (ya+yb)/2.0;

   if ( new_midpoint >= GeomP2->XYSOMM->m )
   {
      error3(E_GEOM_2D_NBSOMM_P2_TOO_SMALL, "midpoint_built_and_fill_geom");
   }

   GeomP2->XYSOMM->me[new_midpoint][0] = xm;
   GeomP2->XYSOMM->me[new_midpoint][1] = ym;

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
   else
   {
      ref_on_face = check_two_somm_on_a_face(GeomP1, a, b);
      
     if ( ref_on_face == 0 )
     {
        printf("check_two_somm_on_a_face : somm %d (%lf ; %lf) with ref %d\n", a,
               GeomP1->XYSOMM->me[a][0], GeomP1->XYSOMM->me[a][1], GeomP1->REF_S->ive[a]);
        printf("check_two_somm_on_a_face : somm %d (%lf ; %lf) with ref %d\n", b,
               GeomP1->XYSOMM->me[b][0], GeomP1->XYSOMM->me[b][1], GeomP1->REF_S->ive[b]);
        printf("check_two_somm_on_a_face : but these 2 points do not constitute a face ");
        printf(" -> middle point somm %d (%lf ; %lf) -> ref 0 \n", new_midpoint,
               GeomP2->XYSOMM->me[new_midpoint][0], GeomP2->XYSOMM->me[new_midpoint][1]);

        GeomP2->REF_S->ive[new_midpoint] = 0;
     }
     else
     {
         GeomP2->REF_S->ive[new_midpoint] = ref_on_face;
     }
   }

   /* fill MID_POINTS */
   sp_set_val(MID_POINTS,a,b, new_midpoint);
   sp_set_val(MID_POINTS,b,a, new_midpoint);

   /* increment by 1 */;
   (++(*curr_idx));
   
   if ( *curr_idx > GeomP2->NBSOMM )
   {
      error3(E_GEOM_2D_NBSOMM_P2_TOO_SMALL, "midpoint_built_and_fill_geom");
   }

   /* finito */
   return new_midpoint;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int athirdpoint_built_and_fill_geom(int a, int b, SPMAT *MID_POINTS, GEOM_2D *GeomP3, const GEOM_2D *GeomP1, int *curr_idx)
{
   double xa,ya;
   double xb,yb;
   double xm,ym;
   int new_midpoint = (*curr_idx);
   int ref_on_face;
   
   xa = GeomP1->XYSOMM->me[a][0];
   ya = GeomP1->XYSOMM->me[a][1];

   xb = GeomP1->XYSOMM->me[b][0];
   yb = GeomP1->XYSOMM->me[b][1];

   xm = (2*xa+xb)/3.0;
   ym = (2*ya+yb)/3.0;

   if ( new_midpoint >= GeomP3->XYSOMM->m )
   {
      error3(E_GEOM_2D_NBSOMM_P3_TOO_SMALL, "athirdpoint_built_and_fill_geom");
   }

   GeomP3->XYSOMM->me[new_midpoint][0] = xm;
   GeomP3->XYSOMM->me[new_midpoint][1] = ym;

   xm = (xa+2*xb)/3.0;
   ym = (ya+2*yb)/3.0;

   if ( new_midpoint >= GeomP3->XYSOMM->m )
   {
      error3(E_GEOM_2D_NBSOMM_P3_TOO_SMALL, "athirdpoint_built_and_fill_geom");
   }

   GeomP3->XYSOMM->me[new_midpoint+1][0] = xm;
   GeomP3->XYSOMM->me[new_midpoint+1][1] = ym;

   /* Now the reference on the border somm : NSFACE + REF_S */
   if ( (GeomP3->REF_S->ive[a] == 0) || (GeomP3->REF_S->ive[b] == 0) )
   {
      GeomP3->REF_S->ive[new_midpoint+0] = 0;
      GeomP3->REF_S->ive[new_midpoint+1] = 0;
   }
   else
   {
      ref_on_face = check_two_somm_on_a_face(GeomP1, a, b);

      if ( ref_on_face == 0 )
      {
         printf("check_two_somm_on_a_face : somm %d (%lf ; %lf) with ref %d\n", a,
             GeomP1->XYSOMM->me[a][0], GeomP1->XYSOMM->me[a][1], GeomP1->REF_S->ive[a]);
         printf("check_two_somm_on_a_face : somm %d (%lf ; %lf) with ref %d\n", b,
             GeomP1->XYSOMM->me[b][0], GeomP1->XYSOMM->me[b][1], GeomP1->REF_S->ive[b]);
         printf("check_two_somm_on_a_face : but these 2 points do not constitute a face ");
         printf(" -> middle point somm %d (%lf ; %lf) -> ref 0\n", new_midpoint,
             GeomP3->XYSOMM->me[new_midpoint][0], GeomP3->XYSOMM->me[new_midpoint][1]);
         printf("check_two_somm_on_a_face : but these 2 points do not constitute a face ");
         printf(" -> middle point somm %d (%lf ; %lf) -> ref 0\n", new_midpoint+1,
             GeomP3->XYSOMM->me[new_midpoint+1][0], GeomP3->XYSOMM->me[new_midpoint+1][1]); 

         GeomP3->REF_S->ive[new_midpoint+0] = 0;
         GeomP3->REF_S->ive[new_midpoint+1] = 0;
      }
      else 
      {
         GeomP3->REF_S->ive[new_midpoint+0] = ref_on_face;
         GeomP3->REF_S->ive[new_midpoint+1] = ref_on_face;
      }
   }

   /* fill MID_POINTS */
   sp_set_val(MID_POINTS, a,b, new_midpoint+0);
   sp_set_val(MID_POINTS, b,a, new_midpoint+1);
   
   /* increment by 1 */;
   *curr_idx += 2;

   if ( *curr_idx > GeomP3->NBSOMM )
   {
      error3(E_GEOM_2D_NBSOMM_P3_TOO_SMALL, "athirdpoint_built_and_fill_geom");
   }

   /* finito */
   return new_midpoint;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int check_two_somm_on_a_face(const GEOM_2D *GeomP1, int a, int b)
{
   int aa;
   int s0,s1;
   int count;

   for (aa=0; aa<GeomP1->NBFACE; aa++)
   {
      s0 = GeomP1->NSFACE->im[aa][0];
      s1 = GeomP1->NSFACE->im[aa][1];

      count = 0;

      if ( (a==s0)||(a==s1) )
      {
         ++count;
      }
      if ( (b==s0)||(b==s1) )
      {
         ++count;
      }

      if ( count==2 )
      {
         return GeomP1->REF_A->ive[aa]; /* the 2 somm a and b belong to a face of reference : given as returned value */
      }
   }

   return 0; /* the 2 somm a and b do not belong to a face */
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

static void Geom2D_Parent_get_fromBase_P1toP3( GEOM_2D *GeomP3, const GEOM_2D *GeomP1)
{
   int e,a,i,j;
   int curr_idx, old_idx;
   double val = 1.0/3.0;
   int NB_TOTAL_SIDE;
   
   SPMAT *MID_POINTS;

   int n0,n1,n2,n3,n4,n5,n6,n7,n8;

   double x0,x1,x2 ;
   double y0,y1,y2 ;
   
   if ( GeomP1 == NULL )  error(E_NULL, "Geom2D_Parent_get_fromBase_P1toP3");
   if ( GeomP3 == NULL )  error(E_NULL, "Geom2D_Parent_get_fromBase_P1toP3");

   /* copy ONLY SOME members of geomBase into Geom */
   strncpy(GeomP3->type,GeomP1->type, 16);
   GeomP3->type[15] = '\0';


   GeomP3->NBELMT = GeomP1->NBELMT;
   GeomP3->NSELMT = im_get(GeomP3->NBELMT, 10); /* To Fill */

   GeomP3->NBFACE = GeomP1->NBFACE;
   GeomP3->NSFACE = im_get(GeomP3->NBFACE, 4); /* To Fill */

   NB_TOTAL_SIDE = GeomP1->NBELMT + GeomP1->NBSOMM -1 ;
   /* mesh without hole   : NB:_TOTAL_SIDE = NBELMT + NBSOMM -1 */
   NB_TOTAL_SIDE += GEOM_2D_NB_HOLES_MAX;
   /* mesh with "n" holes : NB:_TOTAL_SIDE = NBELMT + NBSOMM -1 + n*/

   GeomP3->NBSOMM = /* P1 somm */ GeomP1->NBSOMM + /* new somm */ 2*NB_TOTAL_SIDE + GeomP1->NBELMT;
   GeomP3->XYSOMM = m_get(GeomP3->NBSOMM,2);   /* To Fill */

   GeomP3->REF_A  = iv_get(GeomP3->NBFACE);  iv_copy(GeomP1->REF_A, GeomP3->REF_A);
   GeomP3->REF_S  = iv_get(GeomP3->NBSOMM);  /* To Fill : new sommets , new refs */
   GeomP3->REF_T  = iv_get(GeomP3->NBELMT);  iv_copy(GeomP1->REF_T, GeomP3->REF_T);

   /* ALGO */
   iv_move(GeomP1->REF_S, 0,  GeomP1->NBSOMM, GeomP3->REF_S, 0 );

   MID_POINTS = sp_get(GeomP1->NBSOMM, GeomP1->NBSOMM, 20);
   sp_zero(MID_POINTS);

   /* init */
   curr_idx = GeomP1->NBSOMM;

   /* fill  the new NSELMT , XYSOMM, NSFACE, REF_T, */
   for (e=0; e<GeomP1->NBELMT; e++)
   {
      old_idx = curr_idx ;

      n0 = GeomP1->NSELMT->im[e][0];
      n1 = GeomP1->NSELMT->im[e][1];
      n2 = GeomP1->NSELMT->im[e][2];

      x0 = GeomP1->XYSOMM->me[n0][0] ;
      x1 = GeomP1->XYSOMM->me[n1][0] ;
      x2 = GeomP1->XYSOMM->me[n2][0] ;

      y0 = GeomP1->XYSOMM->me[n0][1] ;
      y1 = GeomP1->XYSOMM->me[n1][1] ;
      y2 = GeomP1->XYSOMM->me[n2][1] ;

      GeomP3->XYSOMM->me[n0][0] = x0;
      GeomP3->XYSOMM->me[n0][1] = y0;
      GeomP3->XYSOMM->me[n1][0] = x1;
      GeomP3->XYSOMM->me[n1][1] = y1;
      GeomP3->XYSOMM->me[n2][0] = x2;
      GeomP3->XYSOMM->me[n2][1] = y2;

      n3 = athirdpoint_built_or_get(e, n0, n1, MID_POINTS, GeomP3, GeomP1, &curr_idx);
      n5 = athirdpoint_built_or_get(e, n1, n2, MID_POINTS, GeomP3, GeomP1, &curr_idx);
      n7 = athirdpoint_built_or_get(e, n0, n2, MID_POINTS, GeomP3, GeomP1, &curr_idx);

      GeomP3->NSELMT->im[e][0] = n0;
      GeomP3->NSELMT->im[e][1] = n1;
      GeomP3->NSELMT->im[e][2] = n2;

      
      GeomP3->NSELMT->im[e][3] = n3;
      n4 = (int)sp_get_val(MID_POINTS,n1,n0);
      GeomP3->NSELMT->im[e][4] = n4;
      

      GeomP3->NSELMT->im[e][5] = n5;
      n6 = (int)sp_get_val(MID_POINTS,n2,n1);
      GeomP3->NSELMT->im[e][6] = n6;

      
      GeomP3->NSELMT->im[e][7] = n7;
      n8 = (int)sp_get_val(MID_POINTS,n2,n0);
      GeomP3->NSELMT->im[e][8] = n8;

      
      /* the last somm in the middle of the triangle */
      GeomP3->NSELMT->im[e][9] = curr_idx;

      GeomP3->XYSOMM->me[curr_idx][0] = x0 + (x1-x0)*val + (x2-x0)*val;
      GeomP3->XYSOMM->me[curr_idx][1] = y0 + (y1-y0)*val + (y2-y0)*val;

      /* finally */
      curr_idx += 1;
   }

   for (a=0; a<GeomP1->NBFACE; a++)
   {
      int n0 = GeomP1->NSFACE->im[a][0];
      int n1 = GeomP1->NSFACE->im[a][1];

      GeomP3->NSFACE->im[a][0] = n0;
      GeomP3->NSFACE->im[a][1] = (int)sp_get_val(MID_POINTS,n0,n1);
      GeomP3->NSFACE->im[a][2] = (int)sp_get_val(MID_POINTS,n1,n0);
      GeomP3->NSFACE->im[a][3] = n1;
   }

   /* free stuff */
   SP_FREE(MID_POINTS);

   /* we had only an estimation of NBSOMM -> resize XYSOMM and REF_S */
   GeomP3->NBSOMM = curr_idx;
   {
      MAT * XYSOMM = m_get(GeomP3->NBSOMM,2);   /* To Fill */

      for (i=0; i<GeomP3->NBSOMM; i++)
      for (j=0; j<2; j++)
      {
         XYSOMM->me[i][j] = GeomP3->XYSOMM->me[i][j];
      }

      M_FREE(GeomP3->XYSOMM);
      GeomP3->XYSOMM = XYSOMM; 
   }

   GeomP3->REF_S  = iv_resize(GeomP3->REF_S, GeomP3->NBSOMM);  /* xxx */

   /* -------------------- */
   /* check the 2 meshes   */
   /* -------------------- */
   Geom2D_checkmeshesP1P3 ( GeomP1 , GeomP3);

   /* end */
   return;
}

/*--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------*/

static void Geom2D_Parent_get_fromBase_P1toP2(GEOM_2D *GeomP2, const GEOM_2D *GeomP1)
{
   int e,a,i,j;
   int curr_idx;
   int NB_TOTAL_SIDE;
   
   SPMAT *MID_POINTS;

   int n0,n1,n2,n3,n4,n5;
   
   
   if ( GeomP1 == NULL )  error(E_NULL, "Geom2DParent_get_fromBase_P1toP2");
   if ( GeomP2 == NULL )  error(E_NULL, "Geom2DParent_get_fromBase_P1toP2");
   
   /* copy ONLY SOME members of geomBase into Geom */
   strncpy(GeomP2->type,GeomP1->type, 16);
   GeomP2->type[15] = '\0';


   GeomP2->NBELMT = GeomP1->NBELMT;
   GeomP2->NSELMT = im_get(GeomP2->NBELMT, 6); /* To Fill */

   GeomP2->NBFACE = GeomP1->NBFACE;
   GeomP2->NSFACE = im_get(GeomP2->NBFACE, 3); /* To Fill */

   NB_TOTAL_SIDE = GeomP1->NBELMT + GeomP1->NBSOMM -1 ;
   /* mesh without hole   : NB:_TOTAL_SIDE = NBELMT + NBSOMM -1 */
   NB_TOTAL_SIDE += GEOM_2D_NB_HOLES_MAX;
   /* mesh with "n" holes : NB:_TOTAL_SIDE = NBELMT + NBSOMM -1 + n */   
   GeomP2->NBSOMM = /* P1 somm */ GeomP1->NBSOMM + /* new somm */ NB_TOTAL_SIDE ;
   GeomP2->XYSOMM = m_get(GeomP2->NBSOMM,2);   /* To Fill */

   GeomP2->REF_A  = iv_get(GeomP2->NBFACE);  iv_copy(GeomP1->REF_A, GeomP2->REF_A);
   GeomP2->REF_S  = iv_get(GeomP2->NBSOMM);  /* To Fill : new sommets , new refs */
   GeomP2->REF_T  = iv_get(GeomP2->NBELMT);  iv_copy(GeomP1->REF_T, GeomP2->REF_T);

   /* ALGO */
   iv_move(GeomP1->REF_S, 0,  GeomP1->NBSOMM, GeomP2->REF_S, 0 );

   MID_POINTS = sp_get(GeomP1->NBSOMM, GeomP1->NBSOMM, 10);
   sp_zero( MID_POINTS );

   /* init */
   curr_idx = GeomP1->NBSOMM;

   /* fill  the new NSELMT , XYSOMM, NSFACE, REF_T, */
   for (e=0; e<GeomP1->NBELMT; e++)
   {
      n0 = GeomP1->NSELMT->im[e][0];
      n1 = GeomP1->NSELMT->im[e][1];
      n2 = GeomP1->NSELMT->im[e][2];

      GeomP2->XYSOMM->me[n0][0] = GeomP1->XYSOMM->me[n0][0];
      GeomP2->XYSOMM->me[n0][1] = GeomP1->XYSOMM->me[n0][1];
      GeomP2->XYSOMM->me[n1][0] = GeomP1->XYSOMM->me[n1][0];
      GeomP2->XYSOMM->me[n1][1] = GeomP1->XYSOMM->me[n1][1];
      GeomP2->XYSOMM->me[n2][0] = GeomP1->XYSOMM->me[n2][0];
      GeomP2->XYSOMM->me[n2][1] = GeomP1->XYSOMM->me[n2][1];
      
      n3 = midpoint_built_or_get(e, n0, n1, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n4 = midpoint_built_or_get(e, n1, n2, MID_POINTS, GeomP2, GeomP1, &curr_idx);
      n5 = midpoint_built_or_get(e, n0, n2, MID_POINTS, GeomP2, GeomP1, &curr_idx);

      GeomP2->NSELMT->im[e][0] = n0;
      GeomP2->NSELMT->im[e][1] = n1;
      GeomP2->NSELMT->im[e][2] = n2;
      
      GeomP2->NSELMT->im[e][3] = n3;
      GeomP2->NSELMT->im[e][4] = n4;
      GeomP2->NSELMT->im[e][5] = n5;
   }
   
   for (a=0; a<GeomP1->NBFACE; a++)
   {
      int n0 = GeomP1->NSFACE->im[a][0];
      int n1 = GeomP1->NSFACE->im[a][1];

      GeomP2->NSFACE->im[a][0] = n0;
      GeomP2->NSFACE->im[a][1] = (int)sp_get_val(MID_POINTS,n0,n1);
      GeomP2->NSFACE->im[a][2] = n1;
   }

   /* free stuff */
   SP_FREE(MID_POINTS);

   /* we had only an estimation of NBSOMM -> resite XYSOMM and REF_S */
   GeomP2->NBSOMM = curr_idx;
   {
      MAT * XYSOMM = m_get(GeomP2->NBSOMM,2);   /* To Fill */

      for (i=0; i<GeomP2->NBSOMM; i++)
      for (j=0; j<2; j++)
      {
         XYSOMM->me[i][j] = GeomP2->XYSOMM->me[i][j];
      }

      M_FREE(GeomP2->XYSOMM);
      GeomP2->XYSOMM = XYSOMM; 
   }

   GeomP2->REF_S  = iv_resize(GeomP2->REF_S, GeomP2->NBSOMM);  /* xxx */


   /* -------------------- */
   /* check the 2 meshes   */
   /* -------------------- */
   Geom2D_checkmeshesP1P2 ( GeomP1 , GeomP2);

   /* end */
   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void Geom2D_Parent_get_fromBase_P1toP1b(GEOM_2D *GeomP1b, const GEOM_2D *GeomP1)
{
   int e;

   if ( GeomP1  == NULL )  error(E_NULL, "Geom2DParent_get_fromBase_P1toP2");
   if ( GeomP1b == NULL )  error(E_NULL, "Geom2DParent_get_fromBase_P1toP2");
    

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
   
   GeomP1b->XYSOMM  = m_get(GeomP1b->NBSOMM,2) ;   /* nodes's coordinates array : 4 somm per elt */
   GeomP1b->NSELMT  = im_get(GeomP1b->NBELMT,4);   /* elements's nodes arrays   */

   /* init-fill GeomP1b->XYSOMM */
   m_move(GeomP1->XYSOMM, 0,0,GeomP1->NBSOMM,2, GeomP1b->XYSOMM, 0,0);
   /* fill GeomP1b->XYSOMM & GeomP1b->NSELMT */
   for (e=0; e<GeomP1->NBELMT; e++)
   {
      double val = 1.0/3.0;
      
      double x1 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][0] ][0] ;
      double x2 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][1] ][0] ;
      double x3 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][2] ][0] ;
   
      double y1 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][0] ][1] ;
      double y2 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][1] ][1] ;
      double y3 = GeomP1->XYSOMM->me[ GeomP1->NSELMT->im[e][2] ][1] ;
   
      GeomP1b->NSELMT->im[e][0] = GeomP1->NSELMT->im[e][0];
      GeomP1b->NSELMT->im[e][1] = GeomP1->NSELMT->im[e][1];
      GeomP1b->NSELMT->im[e][2] = GeomP1->NSELMT->im[e][2];

      GeomP1b->NSELMT->im[e][3] = e + GeomP1->NBSOMM;

      
      GeomP1b->XYSOMM->me[e + GeomP1->NBSOMM][0] = x1 + (x2-x1)*val + (x3-x1)*val;
      GeomP1b->XYSOMM->me[e + GeomP1->NBSOMM][1] = y1 + (y2-y1)*val + (y3-y1)*val;
   }
   
   
   GeomP1b->NSFACE  = im_get(GeomP1->NSFACE->m, GeomP1->NSFACE->n); im_copy(GeomP1->NSFACE, GeomP1b->NSFACE);

   /* -------------------- */
   /* check the 2 meshes   */
   /* -------------------- */
   Geom2D_checkmeshesP1P1b( GeomP1, GeomP1b);

   /* end */
   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

GEOM_2D *Geom2D_get_base_triangle(const char* type, int nb_subdivisions)
{
   GEOM_2D *Geom     = NULL;
   ELT_2D  *elt_P2   = elt2D_get("P2");
   
   int i;

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2D_get_base_triangle");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(Geom->type,"base_triangle", 16);
   Geom->type[15] = '\0';

   if ( strcmp(type, "P2isoP1") == 0 )
   {
      /* construct first a "P2-isoP1" base triangle */

      /*    s2 -              */
      /*       | *            */
      /*    s5 |   * s4       */
      /*       |     *        */
      /*    s0 ----.---- s1   */
      /*          s3          */

      Geom->NBSOMM = 6;
      Geom->NBELMT = 4;
      Geom->NBFACE = 6;

      Geom->XYSOMM = m_get(Geom->NBSOMM,2);
      Geom->NSELMT = im_get(Geom->NBELMT,3);
      Geom->NSFACE = im_get(Geom->NBFACE,2);
   
      Geom->REF_S = iv_get(Geom->NBSOMM);
      Geom->REF_T = iv_get(Geom->NBELMT); iv_zero(Geom->REF_T);
      Geom->REF_A = iv_get(Geom->NBFACE);


      Geom->XYSOMM->me[0][0] = 0.0;
      Geom->XYSOMM->me[0][1] = 0.0;

      Geom->XYSOMM->me[1][0] = 1.0;
      Geom->XYSOMM->me[1][1] = 0.0;

      Geom->XYSOMM->me[2][0] = 0.0;
      Geom->XYSOMM->me[2][1] = 1.0;

      Geom->XYSOMM->me[3][0] = 0.5;
      Geom->XYSOMM->me[3][1] = 0.0;

      Geom->XYSOMM->me[4][0] = 0.5;
      Geom->XYSOMM->me[4][1] = 0.5;

      Geom->XYSOMM->me[5][0] = 0.0;
      Geom->XYSOMM->me[5][1] = 0.5;


      Geom->NSELMT->im[0][0] = 0;
      Geom->NSELMT->im[0][1] = 3;
      Geom->NSELMT->im[0][2] = 5;

      Geom->NSELMT->im[1][0] = 4;
      Geom->NSELMT->im[1][1] = 5;
      Geom->NSELMT->im[1][2] = 3;

      Geom->NSELMT->im[2][0] = 3;
      Geom->NSELMT->im[2][1] = 1;
      Geom->NSELMT->im[2][2] = 4;

      Geom->NSELMT->im[3][0] = 5;
      Geom->NSELMT->im[3][1] = 4;
      Geom->NSELMT->im[3][2] = 2;

      Geom->REF_S->ive[0] = 1;
      Geom->REF_S->ive[1] = 1;
      Geom->REF_S->ive[2] = 2;
      Geom->REF_S->ive[3] = 1;
      Geom->REF_S->ive[4] = 2;
      Geom->REF_S->ive[5] = 3;

   
      Geom->NSFACE->im[0][0] = 0;
      Geom->NSFACE->im[0][1] = 3;

      Geom->NSFACE->im[1][0] = 3;
      Geom->NSFACE->im[1][1] = 1;

      Geom->NSFACE->im[2][0] = 1;
      Geom->NSFACE->im[2][1] = 4;

      Geom->NSFACE->im[3][0] = 4;
      Geom->NSFACE->im[3][1] = 2;

      Geom->NSFACE->im[4][0] = 2;
      Geom->NSFACE->im[4][1] = 5;

      Geom->NSFACE->im[5][0] = 5;
      Geom->NSFACE->im[5][1] = 0;


      Geom->REF_A->ive[0] = 1;
      Geom->REF_A->ive[1] = 1;
      Geom->REF_A->ive[2] = 2;
      Geom->REF_A->ive[3] = 2;
      Geom->REF_A->ive[4] = 3;
      Geom->REF_A->ive[5] = 3;
   }
   else
   if ( strcmp(type, "P3isoP1") == 0 )
   {
      /* construct first a "P2-isoP1" base triangle */

      /*    s2 -                 */
      /*       | *   s6          */
      /*    s8 *   *             */
      /*       |     *           */
      /*    s7 *   *   *s5       */
      /*       |         *       */
      /*       *---*---*---* s1  */
      /*    s0     s3  s4        */

      Geom->NBSOMM = 10;
      Geom->NBELMT = 9;
      Geom->NBFACE = 9;

      Geom->XYSOMM = m_get(Geom->NBSOMM,2);
      Geom->NSELMT = im_get(Geom->NBELMT,3);
      Geom->NSFACE = im_get(Geom->NBFACE,2);

      Geom->REF_S = iv_get(Geom->NBSOMM);
      Geom->REF_T = iv_get(Geom->NBELMT); iv_zero(Geom->REF_T);
      Geom->REF_A = iv_get(Geom->NBFACE);


      Geom->XYSOMM->me[0][0] = 0.0;
      Geom->XYSOMM->me[0][1] = 0.0;

      Geom->XYSOMM->me[1][0] = 1.0;
      Geom->XYSOMM->me[1][1] = 0.0;

      Geom->XYSOMM->me[2][0] = 0.0;
      Geom->XYSOMM->me[2][1] = 1.0;

      Geom->XYSOMM->me[3][0] = 1.0/3;
      Geom->XYSOMM->me[3][1] = 0.0;

      Geom->XYSOMM->me[4][0] = 2.0/3;
      Geom->XYSOMM->me[4][1] = 0.0;

      Geom->XYSOMM->me[5][0] = 2.0/3;
      Geom->XYSOMM->me[5][1] = 1.0/3;

      Geom->XYSOMM->me[6][0] = 1.0/3;
      Geom->XYSOMM->me[6][1] = 2.0/3;

      Geom->XYSOMM->me[7][0] = 0.0;
      Geom->XYSOMM->me[7][1] = 1.0/3;

      Geom->XYSOMM->me[8][0] = 0.0;
      Geom->XYSOMM->me[8][1] = 2.0/3;

      Geom->XYSOMM->me[9][0] = 1.0/3;
      Geom->XYSOMM->me[9][1] = 1.0/3;


      /*    s2 -                 */
      /*       | *   s6          */
      /*    s8 *   *             */
      /*       |     *           */
      /*    s7 *   *   *s5       */
      /*       |         *       */
      /*       *---*---*---* s1  */
      /*    s0     s3  s4        */

      Geom->NSELMT->im[0][0] = 0;
      Geom->NSELMT->im[0][1] = 3;
      Geom->NSELMT->im[0][2] = 7;

      Geom->NSELMT->im[1][0] = 9;
      Geom->NSELMT->im[1][1] = 7;
      Geom->NSELMT->im[1][2] = 3;

      Geom->NSELMT->im[2][0] = 3;
      Geom->NSELMT->im[2][1] = 4;
      Geom->NSELMT->im[2][2] = 9;

      Geom->NSELMT->im[3][0] = 5;
      Geom->NSELMT->im[3][1] = 9;
      Geom->NSELMT->im[3][2] = 4;

      Geom->NSELMT->im[4][0] = 7;
      Geom->NSELMT->im[4][1] = 9;
      Geom->NSELMT->im[4][2] = 8;

      Geom->NSELMT->im[5][0] = 6;
      Geom->NSELMT->im[5][1] = 8;
      Geom->NSELMT->im[5][2] = 9;

      Geom->NSELMT->im[6][0] = 4;
      Geom->NSELMT->im[6][1] = 1;
      Geom->NSELMT->im[6][2] = 5;

      Geom->NSELMT->im[7][0] = 9;
      Geom->NSELMT->im[7][1] = 5;
      Geom->NSELMT->im[7][2] = 6;

      Geom->NSELMT->im[8][0] = 8;
      Geom->NSELMT->im[8][1] = 6;
      Geom->NSELMT->im[8][2] = 2;

      
      Geom->REF_S->ive[0] = 1;
      Geom->REF_S->ive[1] = 1;
      Geom->REF_S->ive[2] = 3;
      Geom->REF_S->ive[3] = 1;
      Geom->REF_S->ive[4] = 1;
      Geom->REF_S->ive[5] = 2;
      Geom->REF_S->ive[6] = 2;
      Geom->REF_S->ive[7] = 3;
      Geom->REF_S->ive[8] = 3;
      Geom->REF_S->ive[9] = 0;

      /*    s2 -                 */
      /*       | *   s6          */
      /*    s8 *   *             */
      /*       |     *           */
      /*    s7 *   *   *s5       */
      /*       |         *       */
      /*       *---*---*---* s1  */
      /*    s0     s3  s4        */

      Geom->NSFACE->im[0][0] = 0;
      Geom->NSFACE->im[0][1] = 3;

      Geom->NSFACE->im[1][0] = 3;
      Geom->NSFACE->im[1][1] = 4;

      Geom->NSFACE->im[2][0] = 4;
      Geom->NSFACE->im[2][1] = 1;

      Geom->NSFACE->im[3][0] = 1;
      Geom->NSFACE->im[3][1] = 5;

      Geom->NSFACE->im[4][0] = 5;
      Geom->NSFACE->im[4][1] = 6;

      Geom->NSFACE->im[5][0] = 6;
      Geom->NSFACE->im[5][1] = 2;

      Geom->NSFACE->im[6][0] = 2;
      Geom->NSFACE->im[6][1] = 8;

      Geom->NSFACE->im[7][0] = 8;
      Geom->NSFACE->im[7][1] = 7;

      Geom->NSFACE->im[8][0] = 7;
      Geom->NSFACE->im[8][1] = 0;


      
      Geom->REF_A->ive[0] = 1;
      Geom->REF_A->ive[1] = 1;
      Geom->REF_A->ive[2] = 1;
      Geom->REF_A->ive[3] = 2;
      Geom->REF_A->ive[4] = 2;
      Geom->REF_A->ive[5] = 2;
      Geom->REF_A->ive[6] = 3;
      Geom->REF_A->ive[7] = 3;
      Geom->REF_A->ive[8] = 3;
   }
   else
   {
      error(E_UNKNOWN, "Geom2D_get_base_triangle");
   }
   
   /* now refine */
   for ( i=0; i<nb_subdivisions; i++)
   {
      GEOM_2D *Geom_P2 = Geom2D_Parent_get_fromBase("P2", Geom);
      Geom_P2->geomBase = NULL;
      GEOM_2D_FREE(Geom);

      Geom = Geom2D_getP1geom_from( elt_P2, Geom_P2);
      GEOM_2D_FREE(Geom_P2);
   }

   /* free mem */
   ELT_2D_FREE(elt_P2);
   

   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

GEOM_2D *Geom2D_getP1geom_from( const ELT_2D *element, const GEOM_2D* geom )
{
   GEOM_2D *GeomP1     = NULL;

   if ( element  == NULL )  error(E_NULL, "Geom2D_getP1geom_from");
   if ( geom     == NULL )  error(E_NULL, "Geom2D_getP1geom_from");

   if ( (strcmp(element->name_ef,"P2")==0 ) )
   {        
      GeomP1 = Geom2D_get_fromParent_P2toP1( element, geom);    
   }
   else
   if ( (strcmp(element->name_ef,"P3")==0 ) )
   {        
      GeomP1 = Geom2D_get_fromParent_P3toP1( element, geom); 
   }
   else
   {
      GeomP1 = Geom2D_copy_from(geom);
      /*warning3(W_GEOM_ALREADY_P1, "Geom2D_getP1geom_from");*/
   }

   return GeomP1;
   
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D *Geom2D_copy_from(const GEOM_2D* GeomP1)
{
   GEOM_2D* Geom;

   if ( GeomP1  == NULL ) error(E_NULL, "Geom2D_copy_from");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (Geom=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2D_copy_from");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(Geom->type, GeomP1->type, 16);
   Geom->type[15] = '\0';

   /* fill Geom */
   Geom->NBSOMM = GeomP1->NBSOMM;
   Geom->NBELMT = GeomP1->NBELMT;
   Geom->NBFACE = GeomP1->NBFACE;


   Geom->REF_S = iv_get(Geom->NBSOMM); iv_copy(GeomP1->REF_S,Geom->REF_S);
   Geom->REF_T = iv_get(Geom->NBELMT); iv_copy(GeomP1->REF_T,Geom->REF_T);
   Geom->REF_A = iv_get(Geom->NBFACE); iv_copy(GeomP1->REF_A,Geom->REF_A);

   Geom->XYSOMM =  m_get(Geom->NBSOMM, 2);  m_copy(GeomP1->XYSOMM, Geom->XYSOMM);
   Geom->NSELMT = im_get(Geom->NBELMT, 3); im_copy(GeomP1->NSELMT, Geom->NSELMT);
   Geom->NSFACE = im_get(Geom->NBFACE, 2); im_copy(GeomP1->NSFACE, Geom->NSFACE);

   Geom->geomBase = NULL;

   return Geom;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D *Geom2D_get_fromParent_P2toP1( const ELT_2D *element, const GEOM_2D* GeomP2 )
{
   int e,a;

   GEOM_2D* GeomP1;

   if ( element == NULL ) error(E_NULL, "Geom2D_get_fromParent_P2toP1");  
   if ( GeomP2  == NULL ) error(E_NULL, "Geom2D_get_fromParent_P2toP1");

   /* --------- create the structure GEOM_2D ----------- */
   if ( (GeomP1=NEW(GEOM_2D)) == (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2D_get_fromParent_P2toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(GeomP1->type, GeomP2->type, 16);
   GeomP1->type[15] = '\0';

   /* fill GeomP1 */
   GeomP1->NBSOMM =   GeomP2->NBSOMM;
   GeomP1->NBELMT = 4*GeomP2->NBELMT;
   GeomP1->NBFACE = 2*GeomP2->NBFACE;


   GeomP1->REF_S = iv_get(GeomP1->NBSOMM); iv_copy(GeomP2->REF_S,GeomP1->REF_S);
   GeomP1->REF_T = iv_get(GeomP1->NBELMT); iv_copy(GeomP2->REF_T,GeomP1->REF_T);
   GeomP1->REF_A = iv_get(GeomP1->NBFACE); iv_copy(GeomP2->REF_A,GeomP1->REF_A);

   GeomP1->XYSOMM = m_get(GeomP1->NBSOMM,2); m_copy(GeomP2->XYSOMM,GeomP1->XYSOMM);
   GeomP1->NSELMT = im_get(GeomP1->NBELMT, 3);
   GeomP1->NSFACE = im_get(GeomP1->NBFACE, 2);

   for (e=0; e<GeomP2->NBELMT; e++)
   {
      GeomP1->NSELMT->im[4*e+0][0] = GeomP2->NSELMT->im[e][0] ;
      GeomP1->NSELMT->im[4*e+0][1] = GeomP2->NSELMT->im[e][3] ;
      GeomP1->NSELMT->im[4*e+0][2] = GeomP2->NSELMT->im[e][5] ;

      GeomP1->NSELMT->im[4*e+1][0] = GeomP2->NSELMT->im[e][3] ;
      GeomP1->NSELMT->im[4*e+1][1] = GeomP2->NSELMT->im[e][1] ;
      GeomP1->NSELMT->im[4*e+1][2] = GeomP2->NSELMT->im[e][4] ;

      GeomP1->NSELMT->im[4*e+2][0] = GeomP2->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[4*e+2][1] = GeomP2->NSELMT->im[e][2] ;
      GeomP1->NSELMT->im[4*e+2][2] = GeomP2->NSELMT->im[e][5] ;

      GeomP1->NSELMT->im[4*e+3][0] = GeomP2->NSELMT->im[e][3] ;
      GeomP1->NSELMT->im[4*e+3][1] = GeomP2->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[4*e+3][2] = GeomP2->NSELMT->im[e][5] ;

      /* the references */
      GeomP1->REF_T->ive[4*e+0]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[4*e+1]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[4*e+2]    = GeomP2->REF_T->ive[e];
      GeomP1->REF_T->ive[4*e+3]    = GeomP2->REF_T->ive[e];
      
   }

   for (a=0; a<GeomP2->NBFACE; a++)
   {
      GeomP1->NSFACE->im[2*a+0][0] = GeomP2->NSFACE->im[a][0] ;
      GeomP1->NSFACE->im[2*a+0][1] = GeomP2->NSFACE->im[a][1] ;

      GeomP1->NSFACE->im[2*a+1][0] = GeomP2->NSFACE->im[a][1] ;
      GeomP1->NSFACE->im[2*a+1][1] = GeomP2->NSFACE->im[a][2] ;

      /* the references */
      GeomP1->REF_A->ive[2*a+0]    = GeomP2->REF_A->ive[a];
      GeomP1->REF_A->ive[2*a+1]    = GeomP2->REF_A->ive[a];
   }

   return GeomP1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static GEOM_2D *Geom2D_get_fromParent_P3toP1( const ELT_2D *element, const GEOM_2D* GeomP3 )
{
   int a,e;

   GEOM_2D* GeomP1;

   if ( element == NULL ) error(E_NULL, "Geom2D_get_fromParent_P3toP1");  
   if ( GeomP3  == NULL ) error(E_NULL, "Geom2D_get_fromParent_P3toP1");
   
   /* --------- create the structure GEOM_2D ----------- */
   if ( (GeomP1=NEW(GEOM_2D))== (GEOM_2D *)NULL )
   {
      error(E_MEM, "Geom2D_get_fromParent_P3toP1");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_GEOM_2D, 0, sizeof(GEOM_2D), MY_LIST3);
      mem_numvar_list(TYPE_GEOM_2D, 1, MY_LIST3);
   }
   /* ---------------------------------------------------- */

   strncpy(GeomP1->type, GeomP3->type, 16);
   GeomP1->type[15] = '\0';

   /* fill GeomP1 */
   GeomP1->NBSOMM =   GeomP3->NBSOMM;
   GeomP1->NBELMT = 9*GeomP3->NBELMT;
   GeomP1->NBFACE = 3*GeomP3->NBFACE;


   GeomP1->REF_S = iv_get(GeomP1->NBSOMM); iv_copy(GeomP3->REF_S,GeomP1->REF_S);
   GeomP1->REF_T = iv_get(GeomP1->NBELMT); iv_copy(GeomP3->REF_T,GeomP1->REF_T); 
   GeomP1->REF_A = iv_get(GeomP1->NBFACE); iv_copy(GeomP3->REF_A,GeomP1->REF_A);

   GeomP1->XYSOMM = m_get(GeomP1->NBSOMM,2); m_copy(GeomP3->XYSOMM,GeomP1->XYSOMM);
   GeomP1->NSELMT = im_get(GeomP1->NBELMT, 3);
   GeomP1->NSFACE = im_get(GeomP1->NBFACE, 2);

   for (e=0; e<GeomP3->NBELMT; e++)
   {
      GeomP1->NSELMT->im[9*e+0][0] = GeomP3->NSELMT->im[e][0] ;
      GeomP1->NSELMT->im[9*e+0][1] = GeomP3->NSELMT->im[e][3] ;
      GeomP1->NSELMT->im[9*e+0][2] = GeomP3->NSELMT->im[e][7] ;

      GeomP1->NSELMT->im[9*e+1][0] = GeomP3->NSELMT->im[e][9] ;
      GeomP1->NSELMT->im[9*e+1][1] = GeomP3->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[9*e+1][2] = GeomP3->NSELMT->im[e][3] ;

      GeomP1->NSELMT->im[9*e+2][0] = GeomP3->NSELMT->im[e][3] ;
      GeomP1->NSELMT->im[9*e+2][1] = GeomP3->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[9*e+2][2] = GeomP3->NSELMT->im[e][9] ;

      GeomP1->NSELMT->im[9*e+3][0] = GeomP3->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[9*e+3][1] = GeomP3->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[9*e+3][2] = GeomP3->NSELMT->im[e][9] ;

      GeomP1->NSELMT->im[9*e+4][0] = GeomP3->NSELMT->im[e][7] ;
      GeomP1->NSELMT->im[9*e+4][1] = GeomP3->NSELMT->im[e][9] ;
      GeomP1->NSELMT->im[9*e+4][2] = GeomP3->NSELMT->im[e][8] ;

      GeomP1->NSELMT->im[9*e+5][0] = GeomP3->NSELMT->im[e][6] ;
      GeomP1->NSELMT->im[9*e+5][1] = GeomP3->NSELMT->im[e][8] ;
      GeomP1->NSELMT->im[9*e+5][2] = GeomP3->NSELMT->im[e][9] ;

      GeomP1->NSELMT->im[9*e+6][0] = GeomP3->NSELMT->im[e][4] ;
      GeomP1->NSELMT->im[9*e+6][1] = GeomP3->NSELMT->im[e][1] ;
      GeomP1->NSELMT->im[9*e+6][2] = GeomP3->NSELMT->im[e][5] ;

      GeomP1->NSELMT->im[9*e+7][0] = GeomP3->NSELMT->im[e][9] ;
      GeomP1->NSELMT->im[9*e+7][1] = GeomP3->NSELMT->im[e][5] ;
      GeomP1->NSELMT->im[9*e+7][2] = GeomP3->NSELMT->im[e][6] ;

      GeomP1->NSELMT->im[9*e+8][0] = GeomP3->NSELMT->im[e][8] ;
      GeomP1->NSELMT->im[9*e+8][1] = GeomP3->NSELMT->im[e][6] ;
      GeomP1->NSELMT->im[9*e+8][2] = GeomP3->NSELMT->im[e][2] ;

      /* the references */
      GeomP1->REF_T->ive[9*e+0]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+1]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+2]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+3]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+4]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+5]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+6]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+7]    = GeomP3->REF_T->ive[e];
      GeomP1->REF_T->ive[9*e+8]    = GeomP3->REF_T->ive[e];
   }

   for (a=0; a<GeomP3->NBFACE; a++)
   {
      GeomP1->NSFACE->im[3*a+0][0] = GeomP3->NSFACE->im[a][0] ;
      GeomP1->NSFACE->im[3*a+0][1] = GeomP3->NSFACE->im[a][1] ;

      GeomP1->NSFACE->im[3*a+1][0] = GeomP3->NSFACE->im[a][1] ;
      GeomP1->NSFACE->im[3*a+1][1] = GeomP3->NSFACE->im[a][2] ;

      GeomP1->NSFACE->im[3*a+2][0] = GeomP3->NSFACE->im[a][2] ;
      GeomP1->NSFACE->im[3*a+2][1] = GeomP3->NSFACE->im[a][3] ;


      /* the references */
      GeomP1->REF_A->ive[2*a+0]    = GeomP3->REF_A->ive[a];
      GeomP1->REF_A->ive[2*a+1]    = GeomP3->REF_A->ive[a];
      GeomP1->REF_A->ive[3*a+2]    = GeomP3->REF_A->ive[a];
   }


   return GeomP1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static int Geom2D_get_nb_ref_on_somm(const GEOM_2D* Geom)
{
   int s,k;
   int nb_refs = 0;

   /* count the nb of reference on somm */
   int ref_s[NBMAX_BC_2D_FUNCTIONS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
   
   for ( s=0; s<Geom->NBSOMM; s++)
   {
      if ( Geom->REF_S->ive[s] > 0 )
      {
         if ( Geom->REF_S->ive[s] > NBMAX_BC_2D_FUNCTIONS )
         {
            warning3(W_GEOM_TOOMANYREFSFORSOMM, "Geom2D_get_nb_ref_on_somm");
         }

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

static int Geom2D_get_nb_ref_on_elts(const GEOM_2D *Geom)
{
   int e,k;
   int nb_refs = 0;
   
   /* count the nb of reference on somm */
   int ref_e[NBMAX_BC_2D_FUNCTIONS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

 
   for ( e=0; e<Geom->NBELMT; e++)
   {
      if ( Geom->REF_T->ive[e] > 0 )
      {
         if ( Geom->REF_T->ive[e] > NBMAX_BC_2D_FUNCTIONS )
         {
            warning3(W_GEOM_TOOMANYREFSFORELTM, "Geom2D_get_nb_ref_on_elts");
         }

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

static int Geom2D_get_nb_ref_on_face(const GEOM_2D *Geom)
{
   int a,k;
   int nb_refs = 0;
   
   /* count the nb of reference on face */
   int ref_a[NBMAX_BC_2D_FUNCTIONS] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

   for ( a=0; a<Geom->NBFACE; a++)
   {
      if ( Geom->REF_A->ive[a] > 0 )
      {
         if ( Geom->REF_A->ive[a] > NBMAX_BC_2D_FUNCTIONS )
         {
            warning3(W_GEOM_TOOMANYREFSFORFACE, "Geom2D_get_nb_ref_on_face");
         }

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

int  Geom2D_check_with_boundaryconditions( const GEOM_2D *MyGeom , const BC_2D *MyBC , const int axe )
{
   char axe_name[8];
   int i;
   
   int nb_refs_on_somm ;
   int nb_refs_on_face ;

   if ( MyBC   == ( BC_2D*)NULL   ) error(E_NULL, "Geom2D_check_with_boundaryconditions");
   if ( MyGeom == ( GEOM_2D*)NULL ) error(E_NULL, "Geom2D_check_with_boundaryconditions");

   switch(axe)
   {
      case AXEe_X: strcpy(axe_name, "AXEe_X"); break;
      case AXEe_Y: strcpy(axe_name, "AXEe_Y"); break;

      default: error(E_UNKNOWN, "Geom3D_check_with_boundaryconditions");
   }

   /*
    * Check BC compatibility
    */

   Bc2D_checkBC(MyBC, axe);

   /*
    * Check compatibility between the number of references
    * given in the geometry and the number of functions
    * to handle theses references in MyParams->bc_conditions
    */

   nb_refs_on_somm = Geom2D_get_nb_ref_on_somm(MyGeom);
   nb_refs_on_face = Geom2D_get_nb_ref_on_face(MyGeom);
   
   printf("\t Geometry->nb_ref_on_somm = %d \n", nb_refs_on_somm);
   printf("\t Geometry->nb_ref_on_face = %d \n", nb_refs_on_face);

   for (i=1; i<=nb_refs_on_somm; i++)
   {
      if ( BC_2De_UNKNOWN == Bc2D_getBcType(MyBC, axe, i) )
      {
         error3(E_GEOM_INCOMPATIBLE_WITH_BC, "Geom2D_check_with_boundaryconditions");
         return EXIT_FAILURE;
      }
   }

   for (i=1; i<=nb_refs_on_face; i++)
   {
      if ( BC_2De_UNKNOWN == Bc2D_getBcType(MyBC, axe, i) )
      {
         error3(E_GEOM_INCOMPATIBLE_WITH_BC, "Geom2D_check_with_boundaryconditions");
         return EXIT_FAILURE;
      }
   }


   if ( nb_refs_on_somm < Bc2D_getNumberOfBcOnSomms(MyBC, axe) )
   {
      warning3(W_GEOM_TOOMANYFUNCS_FOR_BC, "Geom2D_check_with_boundaryconditions");
   }

   if ( nb_refs_on_face < Bc2D_getNumberOfBcOnFaces(MyBC, axe) )
   {
      warning3(W_GEOM_TOOMANYFUNCS_FOR_BC, "Geom2D_check_with_boundaryconditions");
   }

   return EXIT_SUCCESS;
}




