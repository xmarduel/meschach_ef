
/** \file graphics_output_3D.c
 *
 * Contains all the output functions
 *
 */

#ifdef HAVE_SILO
#include "silo.h"
#endif

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_3D.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"



#define VTK_LINE_LIN         3
#define VTK_LINE_QUA        21

#define VTK_TRIANGLE_LIN     5
#define VTK_TRIANGLE_QUA    22

#define VTK_TETRAHEDRA_LIN  10
#define VTK_TETRAHEDRA_QUA  24


#define XVTK_NB_CONTOURS_DEFAULT 7

#define XVTK_CELL_TYPE_LIN  1
#define XVTK_CELL_TYPE_QUA  2
#define XVTK_CELL_TYPE_CUB  3

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics3D_script_vtk          (const char* filename);
static char * graphics3D_script_silo         (const char* filename);

static char * graphics3D_stokes_script_vtk   (const char* filename);
static char * graphics3D_stokes_script_silo  (const char* filename, const VEC *u, const VEC *v, const VEC* w, const VEC *p);


static void graphics3D_vtk         (const char *filename, const ELT_3D *element, const GEOM_3D *geom, const VEC *SOL);
static void graphics3D_silo        (const char *filename, const ELT_3D *element, const GEOM_3D *geom, const VEC *SOL);

static void graphics3D_stokes_vtk  (const char *filename, const ELT_3D *element, const GEOM_3D *geom, const VEC *u, const VEC *v, const VEC* w, const VEC *p);
static void graphics3D_stokes_silo (const char *filename, const ELT_3D *element, const GEOM_3D *geom, const VEC *u, const VEC *v, const VEC* w, const VEC *p);


static void graphics3D_vtk_1var (const char *filename, const GEOM_3D *geom, const VEC* var,                                    int vtk_cell_type);
static void graphics3D_vtk_3var (const char *filename, const GEOM_3D *geom, const VEC* var1, const VEC* var2, const VEC* var3, int vtk_cell_type);

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics3D_script_vtk(const char* filename)
{
   FILE *fp;
   char *script_vtk = (char*)malloc(64);

   snprintf(script_vtk, 64, "Script4vtk_%s.py", filename); script_vtk[63] = '\0';

   fp = fopen(script_vtk, "w");
      
   /* Script python */
   fprintf(fp, "#!/usr/bin/env python\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated - vtk version 6.1\n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "#\n");
   fprintf(fp, "import vtk\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#\n");
   fprintf(fp, "reader = vtk.vtkUnstructuredGridReader() \n");
   fprintf(fp, "reader.SetFileName( \"./%s.vtk\" )\n", filename);
   fprintf(fp, "reader.Update()\n");
   fprintf(fp, "#\n");   
   fprintf(fp, "lut1 = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut1.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut1.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut1.SetRampToSQRT() \n");
   fprintf(fp, "lut1.Build() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "lut2 = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut2.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut2.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut2.SetRampToSQRT() \n");
   fprintf(fp, "lut2.Build() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "valrange = reader.GetOutput().GetPointData().GetScalars().GetRange() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "surfaceMapper = vtk.vtkDataSetMapper() \n");
   fprintf(fp, "surfaceMapper.SetInputData( reader.GetOutput() )\n");
   fprintf(fp, "surfaceMapper.SetLookupTable( lut1 )\n");
   fprintf(fp, "surfaceMapper.SetScalarRange( valrange[0], valrange[1] )\n");
   fprintf(fp, "surfaceMapper.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "surfaceActor = vtk.vtkActor() \n");
   fprintf(fp, "surfaceActor.SetMapper( surfaceMapper )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "contour = vtk.vtkContourFilter() \n");
   fprintf(fp, "contour.SetInputData( reader.GetOutput() ) \n");
   fprintf(fp, "contour.GenerateValues( %d , valrange[0], valrange[1] )\n", XVTK_NB_CONTOURS_DEFAULT);
   fprintf(fp, "contour.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "contourMapper = vtk.vtkPolyDataMapper() \n");
   fprintf(fp, "contourMapper.SetInputData( contour.GetOutput() ) \n");
   fprintf(fp, "contourMapper.SetLookupTable( lut2 ) \n");
   fprintf(fp, "contourMapper.SetScalarRange( valrange[0], valrange[1] )\n");
   fprintf(fp, "contourMapper.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "contourActor = vtk.vtkActor() \n");
   fprintf(fp, "contourActor.SetMapper( contourMapper )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#\n");
   fprintf(fp, "ren1 = vtk.vtkRenderer()\n");
   fprintf(fp, "ren1.SetViewport(0.0, 0.0,  0.5, 1.0)\n");
   fprintf(fp, "ren1.SetBackground( 1, 2, 1 ) \n");
   fprintf(fp, "ren1.AddActor( surfaceActor )\n");
   fprintf(fp, "ren2 = vtk.vtkRenderer()\n");
   fprintf(fp, "ren2.SetViewport(0.5, 0.0,  1.0, 1.0)\n");
   fprintf(fp, "ren2.AddActor( contourActor )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "renWin = vtk.vtkRenderWindow() \n");
   fprintf(fp, "renWin.SetSize( 800, 400 ) \n");
   fprintf(fp, "renWin.AddRenderer( ren1 )\n");
   fprintf(fp, "renWin.AddRenderer( ren2 )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "iren = vtk.vtkRenderWindowInteractor()\n");
   fprintf(fp, "iren.SetRenderWindow(renWin)\n");
   fprintf(fp, "iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())\n");
   fprintf(fp, "iren.Initialize()\n");
   fprintf(fp, "iren.Start()\n");

   fclose(fp);

   return script_vtk;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics3D_vtk(const char *filename, const ELT_3D *element, const GEOM_3D *geom, const VEC *SOL)
{
   char vtk_file[64];
   
   const GEOM_3D *GeomX;

   int vtk_cell_type;

   if ( element   == NULL ) error(E_NULL, "graphics3D_vtk");
   if ( geom      == NULL ) error(E_NULL, "graphics3D_vtk");
   if ( SOL       == NULL ) error(E_NULL, "graphics3D_vtk");
   if ( filename == NULL ) error(E_NULL, "graphics3D_vtk");

   snprintf(vtk_file, 64, "%s.vtk", filename); vtk_file[63]   = '\0';
   
      
   if ( strcmp(element->name_ef, "P1") == 0 )
   {
      GeomX = geom;

      vtk_cell_type = XVTK_CELL_TYPE_LIN;
   }
   else
   if ( strcmp(element->name_ef, "P2") == 0 )
   {
      GeomX = geom;

      vtk_cell_type = XVTK_CELL_TYPE_QUA;
   }
   else
   if ( strcmp(element->name_ef, "P1b") == 0 )
   {
      GeomX = geom->geomBase;

      vtk_cell_type = XVTK_CELL_TYPE_LIN;
   }
   else
   {
      error(E_UNKNOWN, "graphics3D_vtk");
   }

   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/

   graphics3D_vtk_1var(vtk_file, GeomX, SOL, vtk_cell_type);

   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics3D_vtk_1var(const char *filename, const GEOM_3D *geom, const VEC* SOL, int vtk_cell_type)
{
   FILE *fp1 = fopen(filename, "w");
   
   int e,s,k,f;

   int MAP_CELL_TO_CELL[10] = /* VTK_ORDER[i] = MY_ORDER[ MAP_CELL_TO_CELL[i] ] */
   {
      0, 1, 2, 3, 4, 7, 5, 6, 9, 8,
   };

   int MAP_CELL_TO_FACE[4][6] =  /* VTK_ORDER[i] = MY_ORDER[ MAP_CELL_TO_CELL[i] ] */
   {
      {0, 1, 3, 4, 9, 6},
      {0, 3, 2, 6, 8, 5},
      {1, 2, 3, 7, 8, 9},
      {0, 2, 1, 5, 7, 4},
   };


   int NB_SOMM_CELL;
   int NB_SOMM_FACE;

   int tetrahedra_type;
   int triangle_type;
   
   switch (vtk_cell_type)
   {
      case XVTK_CELL_TYPE_LIN:
         NB_SOMM_CELL = 4;
         NB_SOMM_FACE = 3;
         tetrahedra_type = VTK_TETRAHEDRA_LIN;
         triangle_type   = VTK_TRIANGLE_LIN;
         break;
      case XVTK_CELL_TYPE_QUA:
         NB_SOMM_CELL = 10;
         NB_SOMM_FACE =  6;
         tetrahedra_type = VTK_TETRAHEDRA_QUA;
         triangle_type   = VTK_TRIANGLE_QUA;
         break;
      default:
         NB_SOMM_CELL = 4;
         NB_SOMM_FACE = 3;
         tetrahedra_type = VTK_TETRAHEDRA_LIN;
         triangle_type   = VTK_TRIANGLE_LIN;
   }

   /* File VTK */
   fprintf(fp1, "# vtk DataFile Version 4.2 \n");
   fprintf(fp1, "Solution PDE problem \n");
   fprintf(fp1, "\n");
   fprintf(fp1, "ASCII \n");
   fprintf(fp1, "\n");
   fprintf(fp1, "DATASET UNSTRUCTURED_GRID \n");
   fprintf(fp1, "POINTS %d float \n", geom->NBSOMM);
   for (s=0; s<geom->NBSOMM; s++)
   {
      fprintf(fp1, "%lf %lf %lf\n",
              geom->XYSOMM->me[s][0],
              geom->XYSOMM->me[s][1],
              geom->XYSOMM->me[s][2]);
   }

   fprintf(fp1, "\n");
   fprintf(fp1, "CELLS %d %d \n", (1+4)*geom->NBELMT, ((1+NB_SOMM_CELL) + 4*(1+NB_SOMM_FACE))*(geom->NBELMT) );
   for (e=0; e<geom->NBELMT ; e++)
   {
      /* the tetraedra */
      fprintf(fp1, "%d ", NB_SOMM_CELL);

      for (k=0; k<NB_SOMM_CELL; k++)
      {
         fprintf(fp1, "%d ", geom->NSELMT->im[e][ MAP_CELL_TO_CELL[k] ]);
      }
      fprintf(fp1, "\n");

      /* the 4 faces */
      for (f=0; f<4; f++)
      {
         fprintf(fp1, "%d ", NB_SOMM_FACE);

         for (k=0; k<NB_SOMM_FACE; k++)
         {
            fprintf(fp1, "%d ", geom->NSELMT->im[e][ MAP_CELL_TO_FACE[f][k] ]);
         }
         fprintf(fp1, "\n");
      }
   }

   fprintf(fp1, "\n");
   fprintf(fp1, "CELL_TYPES %d \n", (1+4)*(geom->NBELMT)); /* 1 tetrahedra + 4 faces */
   for (e=0; e<geom->NBELMT ; e++)
   {
      fprintf(fp1, "%d\n", tetrahedra_type );
      fprintf(fp1, "%d\n", triangle_type   );
      fprintf(fp1, "%d\n", triangle_type   );
      fprintf(fp1, "%d\n", triangle_type   );
      fprintf(fp1, "%d\n", triangle_type   );
   }

   fprintf(fp1, "\n");
   fprintf(fp1, "POINT_DATA %d \n" ,geom->NBSOMM);
   fprintf(fp1, "SCALARS scalars float\n");
   fprintf(fp1, "LOOKUP_TABLE default \n");

   for (s=0; s<geom->NBSOMM; s++)
   {
      fprintf(fp1, "%lf\n", SOL->ve[s] );
   }

   fclose(fp1);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics3D_vtk_3var(const char *filename, const GEOM_3D *geom, const VEC* var1, const VEC* var2, const VEC* var3, int vtk_cell_type)
{
   FILE *fp = fopen(filename, "w");

   int s,e,k,f;

   int MAP_CELL_TO_CELL[10] = /* VTK_ORDER[i] = MY_ORDER[ MAP_CELL_TO_CELL[i] ] */
   {
      0, 1, 2, 3, 4, 7, 5, 6, 9, 8,
   };

   int MAP_CELL_TO_FACE[4][6] =  /* VTK_ORDER[i] = MY_ORDER[ MAP_CELL_TO_CELL[i] ] */
   {
      {0, 1, 3, 4, 9, 6},
      {2, 0, 3, 5, 6, 8},
      {1, 2, 3, 7, 8, 9},
      {0, 2, 1, 5, 7, 4},
   };

   int NB_SOMM_CELL;
   int NB_SOMM_FACE;

   int tetrahedra_type;
   int triangle_type;

   int show_faces = 0; /* or 4 */

   switch (vtk_cell_type)
   {
      case XVTK_CELL_TYPE_LIN:
         NB_SOMM_CELL = 4;
         NB_SOMM_FACE = 3;
         tetrahedra_type = VTK_TETRAHEDRA_LIN;
         triangle_type   = VTK_TRIANGLE_LIN;
         break;
      case XVTK_CELL_TYPE_QUA:
         NB_SOMM_CELL = 10;
         NB_SOMM_FACE =  6;
         tetrahedra_type = VTK_TETRAHEDRA_QUA;
         triangle_type   = VTK_TRIANGLE_QUA;
         break;
      default:
         NB_SOMM_CELL = 4;
         NB_SOMM_FACE = 3;
         tetrahedra_type = VTK_TETRAHEDRA_LIN;
         triangle_type   = VTK_TRIANGLE_LIN;
   }

   /* File VTK for uvw */
   fprintf(fp, "# vtk DataFile Version 4.2 \n");
   fprintf(fp, "Solution PDE problem \n");
   fprintf(fp, "\n");
   fprintf(fp, "ASCII \n");
   fprintf(fp, "\n");
   fprintf(fp, "DATASET UNSTRUCTURED_GRID \n");
   fprintf(fp, "POINTS   %d float \n", geom->NBSOMM);

   for (s=0; s<geom->NBSOMM; s++)
   {
      fprintf(fp, "%lf %lf %lf\n",
              geom->XYSOMM->me[s][0],
              geom->XYSOMM->me[s][1],
              geom->XYSOMM->me[s][2]) ;
   }

   fprintf(fp, "\n");

   fprintf(fp, "CELLS %d %d \n", (1+show_faces)*(geom->NBELMT), ((1+NB_SOMM_CELL) + show_faces*(1+NB_SOMM_FACE))*(geom->NBELMT) );
   for (e=0; e<geom->NBELMT; e++)
   {
      /* the tetraedra */
      fprintf(fp, "%d ", NB_SOMM_CELL);
      for (k=0; k<NB_SOMM_CELL; k++)
      {
         fprintf(fp, "%d ", geom->NSELMT->im[e][MAP_CELL_TO_CELL[k]]);
      }
      fprintf(fp, "\n");

      /* the 4 faces */
      for (f=0; f<show_faces; f++)
      {
         fprintf(fp, "%d ", NB_SOMM_FACE);
         for (k=0; k<NB_SOMM_FACE; k++)
         {
            fprintf(fp, "%d ", geom->NSELMT->im[e][MAP_CELL_TO_FACE[f][k]]);
         }
         fprintf(fp, "\n");
      }
   }

   fprintf(fp, "\n");
   
   fprintf(fp, "CELL_TYPES %d \n", (1+show_faces)*(geom->NBELMT));
   for (e=0; e<geom->NBELMT; e++)
   {
      fprintf(fp, "%d\n",tetrahedra_type);
      if (show_faces) fprintf(fp, "%d\n",triangle_type);
      if (show_faces) fprintf(fp, "%d\n",triangle_type);
      if (show_faces) fprintf(fp, "%d\n",triangle_type);
      if (show_faces) fprintf(fp, "%d\n",triangle_type);
   }

   fprintf(fp, "\n");
   fprintf(fp, "POINT_DATA %d \n", geom->NBSOMM);
   fprintf(fp, "VECTORS velocity float\n");
   
   for (s=0; s<geom->NBSOMM; s++)
   {
      fprintf(fp, "%lf %lf %lf \n",
              var1->ve[s],
              var2->ve[s],
              var3->ve[s]);
   }

   fclose(fp);
}


/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics3D_script_silo(const char* filename)
{
   /* Script MESHTVX */
   char *script_meshtv = (char*)malloc(64);
   FILE *fp;

   snprintf(script_meshtv, 64, "Script4meshtv_%s.sil", filename); script_meshtv[63] = '\0';

   fp = fopen(script_meshtv, "w");

   fprintf(fp, "#! /usr/bin/env meshtvx\n");
   fprintf(fp, "# \n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "#\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#  host[#] meshtvx -s name_of_the_file \n");
   fprintf(fp, "open \"./%s.silo\";\n", filename);

   fprintf(fp, "iso colors={10 11 9 10 8 9 7 8 6 7 5 6 4 5 3 4 2 3 1 2 }\n");
   fprintf(fp, "iso lc=off,ls=solid,lt=1,nlevels=10,min=off,max=off,legend=on,var=\"vitesse\",pop=off\n");;

   fprintf(fp, "plot iso\n");

   fclose(fp);

   return script_meshtv;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics3D_silo(const char* filename, const ELT_3D *element, const GEOM_3D *geom, const VEC  *SOL)
{
   
#ifdef HAVE_SILO
   DBfile   *file_silo = NULL;
   int      DB_REAL    = sizeof(Real)==sizeof(float) ? DB_FLOAT : DB_DOUBLE ;
#endif   

   char     *coordnames[3];
   Real     *coords[3];
   int      nzones,nfaces,nnodes;
   int      *znodelist,*fnodelist;
   int      lznodelist,lfnodelist;
   int      znodeorigin,fnodeorigin;
   int      zoneshapecnt[1] ,faceshapecnt[1];
   int      zoneshapesize[1],faceshapesize[1];
   int      nzoneshapes,nfaceshapes;
   
   int NB_SOMMS_PER_CELL;
   int NB_FACES_PER_CELL;
   
   char silo_file[64];
   
   int i,e,a,s;
   
   const GEOM_3D *GeomP1;

   /* check */
   if ( element   == NULL ) error(E_NULL, "graphics3D_silo");
   if ( geom      == NULL ) error(E_NULL, "graphics3D_silo");
   if ( SOL       == NULL ) error(E_NULL, "graphics3D_silo");
   if ( filename  == NULL ) error(E_NULL, "graphics3D_silo");
   

   NB_SOMMS_PER_CELL = 4; /* print mesh and solution as a "P1" solution */
   NB_FACES_PER_CELL = 3; /* print mesh and solution as a "P1" solution */


   /*--- ECRITURE DU FICHIER "<filename>.silo" ---*/
   
   snprintf(silo_file, 64, "%s.silo", filename); silo_file[63] = '\0';
   
   /* Write out the mesh to the file ---------- DBPutZonelist */

   /* nb zones = nb cells of the mesh */
   nzones = geom->NBELMT;
   /* length of vector zonelist : sum of all nodes on each zone */
   lznodelist = NB_SOMMS_PER_CELL*nzones;
   /* Give the NB indices of the nodes for each zone of the mesh */
   znodelist = (int*)calloc(lznodelist,sizeof(int));

   for (i=0; i<nzones; i++)
   for (e=0; e<NB_SOMMS_PER_CELL; e++)
   {
      znodelist[NB_SOMMS_PER_CELL*i+e] = geom->NSELMT->im[i][e];
   }

   /* "C" notation */
   znodeorigin= 0;
   /* only one type of zone : tetrahedra */
   nzoneshapes = 1;
   /* array of lenght "nzoneshapes" containing the number of nodes used by each zone shape (here only 1 zone shape) */
   zoneshapesize[0] = NB_SOMMS_PER_CELL;
   /* array of lenght "nzoneshapes"  containing the number of zones used by each zone shape */
   zoneshapecnt[0]  = nzones;
   
   
   /* Write out the mesh to the file --------- DBPutFacelist */

   /* nb faces = nb faces of the mesh */
   nfaces = geom->NBFACE;
   /* length of vector facelist which for each zone */
   lfnodelist = NB_FACES_PER_CELL*nfaces;
   /* Give the NA indices of the nodes for each zone of the mesh */
   fnodelist = (int*)calloc(lfnodelist, sizeof(int));

   for (i=0; i<nfaces; i++)
   for (a=0; a<NB_FACES_PER_CELL; a++)
   {
      fnodelist[NB_FACES_PER_CELL*i+a] = geom->NSFACE->im[i][a];
   }

   /* "C" notation */
   fnodeorigin = 0;
   /* only one type of face : triangle */   
   nfaceshapes=1;
   /* array of lenght "nfaceshapes" containing the number of nodes used by each face shape (here only 1 face shape) */   
   faceshapesize[0] = NB_FACES_PER_CELL;
   /* array of lenght "nfaceshapes"  containing the number of zones used by each face shape */
   faceshapecnt[0]  = nfaces; 
   
   
   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   GeomP1 = geom;

   while ( GeomP1->geomBase != NULL )
   {
      GeomP1 = GeomP1->geomBase;
   }

   /* nb of nodes */  /* GeomP1 is is P1 mesh */
   nnodes = GeomP1->NBSOMM;

   coordnames[0] = strdup("X");
   coordnames[1] = strdup("Y");
   coordnames[2] = strdup("Z");

   coords[0] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[1] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[2] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));

   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      coords[0][s] = GeomP1->XYSOMM->me[s][0];
      coords[1][s] = GeomP1->XYSOMM->me[s][1];
      coords[2][s] = GeomP1->XYSOMM->me[s][2];
   }
   
   
#ifdef HAVE_SILO

   /* Create the SILO file */

   file_silo = DBCreate(silo_file, DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);

   /* Write out the mesh to the file ---------- DBPutFacelist */

   DBPutFacelist(file_silo, "face", nfaces, 3, fnodelist, lfnodelist, fnodeorigin, NULL, faceshapesize, faceshapecnt, nfaceshapes,  NULL, NULL, 0);

   /* Write out the mesh to the file ---------- DBPutZonelist */

   DBPutZonelist(file_silo, "zone", nzones, 3, znodelist , lznodelist, znodeorigin, zoneshapesize, zoneshapecnt, nzoneshapes);
   
   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   DBPutUcdmesh(file_silo, (char*)geom->type, 3,  coordnames, coords, nnodes, nzones, "zone", "face", DB_REAL, NULL); 
   
   /* Write out the variables to the file */
   
   DBPutUcdvar1(file_silo, "vitesse", (char*)geom->type, SOL->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL); 
   
   /* Close the silo file */
   
   DBClose(file_silo);

#endif
   
   /* free memory */
   free(coords[0]);
   free(coords[1]);
   free(coords[2]);

   free(znodelist);
   free(fnodelist);

   free(coordnames[0]);
   free(coordnames[1]);
   free(coordnames[2]);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics3D_stokes_script_vtk(const char* filename)
{
   char *script_vtk = (char*)malloc(64);
   FILE *fp;

   snprintf(script_vtk, 64, "Script4vtk_%s.py", filename); script_vtk[63]  = '\0';
   fp = fopen(script_vtk, "w");

   /* Script python */
   fprintf(fp, "#! /usr/bin/env python\n");
   fprintf(fp, "# \n");
   fprintf(fp, "# File automatically generated - vtk version : 6.1\n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "#\n");
   fprintf(fp, "import vtk\n");
   fprintf(fp, "#\n");
   fprintf(fp, "ren_u = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_u.SetViewport( 0.000, 0.5,  0.333, 1.0) \n");
   fprintf(fp, "ren_v = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_v.SetViewport( 0.333, 0.5,  0.666, 1.0) \n");
   fprintf(fp, "ren_w = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_w.SetViewport( 0.666, 0.5,  1.000, 1.0) \n");
   fprintf(fp, "ren_a = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_a.SetViewport( 0.000, 0.0,  0.500, 0.5) \n");
   fprintf(fp, "ren_s = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_s.SetViewport( 0.500, 0.0,  1.000, 0.5) \n");
   fprintf(fp, "#\n");
   fprintf(fp, "renWin = vtk.vtkRenderWindow()\n");
   fprintf(fp, "renWin.AddRenderer( ren_u )\n");
   fprintf(fp, "renWin.AddRenderer( ren_v )\n");
   fprintf(fp, "renWin.AddRenderer( ren_w )\n");
   fprintf(fp, "renWin.AddRenderer( ren_a )\n");
   fprintf(fp, "renWin.AddRenderer( ren_s )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "reader_u = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_v = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_w = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_s = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_u.SetFileName( \"./%s3D_u.vtk\" )\n", filename);
   fprintf(fp, "reader_u.Update()\n");
   fprintf(fp, "reader_v.SetFileName( \"./%s3D_v.vtk\" )\n", filename);
   fprintf(fp, "reader_v.Update()\n");
   fprintf(fp, "reader_w.SetFileName( \"./%s3D_w.vtk\" )\n", filename);
   fprintf(fp, "reader_w.Update()\n");
   fprintf(fp, "reader_s.SetFileName( \"./%s3D_uvw.vtk\" )\n", filename);
   fprintf(fp, "reader_s.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "lut_u = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut_u.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut_u.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut_u.SetRampToSQRT() \n");
   fprintf(fp, "lut_u.Build() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "lut_v = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut_v.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut_v.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut_v.SetRampToSQRT() \n");
   fprintf(fp, "lut_v.Build() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "lut_w = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut_w.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut_w.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut_w.SetRampToSQRT() \n");
   fprintf(fp, "lut_w.Build() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "u_range = reader_u.GetOutput().GetPointData().GetScalars().GetRange() \n");
   fprintf(fp, "v_range = reader_v.GetOutput().GetPointData().GetScalars().GetRange() \n");
   fprintf(fp, "w_range = reader_w.GetOutput().GetPointData().GetScalars().GetRange() \n");
   fprintf(fp, "#\n");
   fprintf(fp, "surfaceMapper_u = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper_v = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper_w = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper_u.SetInputData( reader_u.GetOutput() ) \n");
   fprintf(fp, "surfaceMapper_u.SetLookupTable( lut_u ) \n");
   fprintf(fp, "surfaceMapper_u.SetScalarRange( u_range[0], u_range[1] )\n");
   fprintf(fp, "surfaceMapper_v.SetInputData( reader_v.GetOutput() ) \n");
   fprintf(fp, "surfaceMapper_v.SetLookupTable( lut_v ) \n");
   fprintf(fp, "surfaceMapper_v.SetScalarRange( v_range[0], v_range[1] )\n");
   fprintf(fp, "surfaceMapper_w.SetInputData( reader_w.GetOutput() ) \n");
   fprintf(fp, "surfaceMapper_w.SetLookupTable( lut_w ) \n");
   fprintf(fp, "surfaceMapper_w.SetScalarRange( w_range[0], w_range[1] )\n");
   fprintf(fp, "surfaceMapper_u.Update()\n");
   fprintf(fp, "surfaceMapper_v.Update()\n");
   fprintf(fp, "surfaceMapper_w.Update()\n");
   fprintf(fp, "surfaceMapper_p.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "surfaceActor_u = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor_v = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor_w = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor_u.SetMapper( surfaceMapper_u )\n");
   fprintf(fp, "surfaceActor_v.SetMapper( surfaceMapper_v )\n");
   fprintf(fp, "surfaceActor_w.SetMapper( surfaceMapper_w )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "maxVelocity = reader_s.GetOutput().GetPointData().GetVectors().GetMaxNorm()\n");
   fprintf(fp, "print \"maxVelocity = \", maxVelocity\n");
   fprintf(fp, "#\n");
   fprintf(fp, "lut = vtk.vtkLookupTable()\n");
   fprintf(fp, "lut.SetHueRange(0.667, 0.0)\n");
   fprintf(fp, "lut.SetNumberOfTableValues(256)\n");
   fprintf(fp, "lut.SetRampToSQRT()\n");
   fprintf(fp, "lut.Build()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# vectors\n");
   fprintf(fp, "glyph2d_src = vtk.vtkGlyphSource2D()\n");
   fprintf(fp, "cone        = vtk.vtkConeSource()\n");
   fprintf(fp, "arrow       = vtk.vtkArrowSource()\n");
   fprintf(fp, "glyph_src   = cone\n");
   fprintf(fp, "glyph3d     = vtk.vtkGlyph3D()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "glph_trfm   = vtk.vtkTransformFilter()\n");
   fprintf(fp, "glph_trfm.SetTransform (vtk.vtkTransform())\n");
   fprintf(fp, "#\n");
   fprintf(fp, "scale      = 0.2\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# initialize\n");
   fprintf(fp, "glyph2d_src.SetGlyphTypeToArrow()\n");
   fprintf(fp, "glyph2d_src.SetFilled(0)\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#cone.SetResolution(5)\n");
   fprintf(fp, "#cone.SetHeight(1)\n");
   fprintf(fp, "#cone.SetRadius(0.2)\n");
   fprintf(fp, "#glph_trfm.SetInput(glyph_src.GetOutput())\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#tr = glph_trfm.GetTransform()\n");
   fprintf(fp, "#tr.Identity()\n");
   fprintf(fp, "#tr.Translate(-1, 0.0, 0.0)\n");
   fprintf(fp, "#glyph2d_src.SetCenter(0.5, 0.0, 0.0)\n");
   fprintf(fp, "#\n");
   fprintf(fp, "glyph3d.SetInputData(reader_s.GetOutput())\n");
   fprintf(fp, "glyph3d.SetSource(glyph2d_src.GetOutput())\n");
   fprintf(fp, "glyph3d.SetScaleFactor(scale)\n");
   fprintf(fp, "glyph3d.SetScaleModeToScaleByVector()\n");
   fprintf(fp, "glyph3d.SetColorModeToColorByVector()\n");
   fprintf(fp, "glyph3d.SetClamping (1)\n");
   fprintf(fp, "glyph3d.SetRange(0.0, maxVelocity)\n");
   fprintf(fp, "glyph3d.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "vectorsMapper = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "vectorsMapper.SetInput(glyph3d.GetOutput())\n");
   fprintf(fp, "vectorsMapper.SetLookupTable(lut)\n");
   fprintf(fp, "vectorsMapper.SetScalarRange(0.000000, maxVelocity)\n");
   fprintf(fp, "vectorsMapper.ScalarVisibilityOn()\n");
   fprintf(fp, "vectorsMapper.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "vectorsActor = vtk.vtkActor()\n");
   fprintf(fp, "vectorsActor.SetMapper(vectorsMapper)\n");
   fprintf(fp, "vectorsActor.GetProperty().SetLineWidth(1.0)\n");
   fprintf(fp, "vectorsActor.GetProperty().BackfaceCullingOff()\n");
   fprintf(fp, "vectorsActor.GetProperty().FrontfaceCullingOff()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# streamlines:\n");
   fprintf(fp, "seeds = vtk.vtkPlaneSource()\n");
   fprintf(fp, "seeds.SetXResolution(50)\n");
   fprintf(fp, "seeds.SetYResolution(50)\n");
   fprintf(fp, "seeds.SetOrigin(2,-2,26))\n");
   fprintf(fp, "seeds.SetPoint2(2,2,26))\n");
   fprintf(fp, "seeds.SetPoint2(2,-2,32))\n");
   fprintf(fp, "#\n");
   fprintf(fp, "integ = vtk.vtkRungeKutta4()\n");
   fprintf(fp, "streamer = vtk.vtkStreamLine()\n");
   fprintf(fp, "streamer.SetInputConnection(reader_s.GetOutputPort())\n");
   fprintf(fp, "streamer.SetSource(seeds.GetOutput())\n");
   fprintf(fp, "streamer.SetMaximumPropagationTime(5)\n");
   fprintf(fp, "streamer.SetStepLength(0.01)\n");
   fprintf(fp, "streamer.SetIntegrationStepLength(0.05)\n");
   fprintf(fp, "streamer.SetIntegrationDirectionToIntegrateBothDirections()\n");
   fprintf(fp, "streamer.SetIntegrator(integ)\n");
   fprintf(fp, "streamer.SpeedScalarsOn()\n");
   fprintf(fp, "streamer.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "streamMapper = vtk.vtkPolyDataMapper()\n");
   fprintf(fp, "streamMapper.SetInputData(streamer.GetOutput())\n");
   fprintf(fp, "streamMapper.SetScalarRange(0, maxVelocity)\n");
   fprintf(fp, "streamMapper.ScalarVisibilityOn()\n");
   fprintf(fp, "streamMapper.SetLookupTable(lut)\n");
   fprintf(fp, "streamMapper.Update()\n");
   fprintf(fp, "#\n");
   fprintf(fp, "streamsActor = vtk.vtkActor()\n");
   fprintf(fp, "streamsActor.SetMapper(streamMapper)\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# rendering\n");
   fprintf(fp, "renWin.SetSize( 900, 600) \n");
   fprintf(fp, "ren_u.SetBackground( 1.0, 1.0, 1.0 )\n");
   fprintf(fp, "ren_v.SetBackground( 1.0, 1.0, 1.0 )\n");
   fprintf(fp, "ren_w.SetBackground( 1.0, 1.0, 1.0 )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "ren_u.AddActor( surfaceActor_u )\n");
   fprintf(fp, "ren_v.AddActor( surfaceActor_v )\n");
   fprintf(fp, "ren_w.AddActor( surfaceActor_w )\n");
   fprintf(fp, "ren_a.AddActor( vectorsActor   )\n");
   fprintf(fp, "ren_s.AddActor( streamsActor   )\n");
   fprintf(fp, "#\n");
   fprintf(fp, "iren = vtk.vtkRenderWindowInteractor()\n");
   fprintf(fp, "iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())\n");
   fprintf(fp, "iren.SetRenderWindow(renWin)\n");
   fprintf(fp, "#\n");
   fprintf(fp, "iren.Initialize()\n");
   fprintf(fp, "iren.Start()\n");

   fclose(fp);

   return script_vtk;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics3D_stokes_vtk(const char *filename, const ELT_3D *element, const GEOM_3D *geom, const VEC *u, const VEC *v, const VEC* w, const VEC *p)
{
   char u_vtk_file[64];
   char v_vtk_file[64];
   char w_vtk_file[64];
   char p_vtk_file[64];
   char V_vtk_file[64]; /* vector */
     
   const GEOM_3D *GeomV;
   const GEOM_3D *GeomP;
   const GEOM_3D *GeomX;

   int vtk_cell_type_V;
   int vtk_cell_type_P;

   if ( element   == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   if ( geom      == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   if ( u         == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   if ( v         == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   if ( w         == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   if ( p         == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   if ( filename  == NULL ) error(E_NULL, "graphics3D_stokes_vtk");
   

   snprintf(u_vtk_file, 64, "%s_u.vtk", filename); u_vtk_file[63] = '\0';
   snprintf(v_vtk_file, 64, "%s_v.vtk", filename); v_vtk_file[63] = '\0';
   snprintf(w_vtk_file, 64, "%s_w.vtk", filename); w_vtk_file[63] = '\0';
   snprintf(p_vtk_file, 64, "%s_p.vtk", filename); p_vtk_file[63] = '\0';

   snprintf(V_vtk_file, 64, "%s_V.vtk", filename); V_vtk_file[63] = '\0';


   if ( strcmp(element->name_ef,"P1") == 0 )
   {
      GeomV = geom;
      GeomP = geom;

      vtk_cell_type_V = XVTK_CELL_TYPE_LIN;
      vtk_cell_type_P = XVTK_CELL_TYPE_LIN;
   }
   else
   if ( strcmp(element->name_ef,"P2") == 0 )
   {
      GeomV = geom;
      GeomX = Geom3D_getP1geom_from(element, geom);
      GeomP = geom->geomBase;

      vtk_cell_type_V = XVTK_CELL_TYPE_QUA;
      vtk_cell_type_P = XVTK_CELL_TYPE_LIN;
   }
   else
   if ( strcmp(element->name_ef,"P1b") == 0 )
   {
      GeomV = geom->geomBase;
      GeomP = geom->geomBase;
      GeomX = geom->geomBase; 
 
      vtk_cell_type_V = XVTK_CELL_TYPE_LIN;
      vtk_cell_type_P = XVTK_CELL_TYPE_LIN;
   }
   else
   {
      error(E_UNKNOWN, "graphics3D_stokes_vtk");
   }

   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/

   graphics3D_vtk_1var(u_vtk_file, GeomV, u, vtk_cell_type_V);
   graphics3D_vtk_1var(v_vtk_file, GeomV, v, vtk_cell_type_V);
   graphics3D_vtk_1var(w_vtk_file, GeomV, w, vtk_cell_type_V);

   graphics3D_vtk_1var(p_vtk_file, GeomP, p, vtk_cell_type_P);

   graphics3D_vtk_3var(V_vtk_file,
                       GeomX, 
                       Geom3D_extract_P1geom_VEC_from(element, geom, u),
                       Geom3D_extract_P1geom_VEC_from(element, geom, v),
                       Geom3D_extract_P1geom_VEC_from(element, geom, w),
                       XVTK_CELL_TYPE_LIN);

   /* graphics3D_vtk_3var(V_vtk_file, GeomV, u, v,  w, XVTK_CELL_TYPE_QUA); */ /* ?? vtk bug ?? */

   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics3D_stokes_script_silo(const char *filename, const VEC *u, const VEC *v, const VEC* w, const VEC *p)
{
   /* Script MESHTVX */
   char *script_meshtv = (char*)malloc(64);
   FILE *fp;

   snprintf(script_meshtv, 64, "Script4meshtv_%s.sil", filename); script_meshtv[63] = '\0';

   fp = fopen(script_meshtv, "w");

   fprintf(fp, "#! /usr/bin/env meshtvx\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "#\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#  host[] meshtvx -s name_of_the_file \n");
   fprintf(fp, "open \"./%s.silo\";\n", filename);
   fprintf(fp, "curve legend=off label=on;");
   fprintf(fp, "dist var=\"velocity\" outwin=-1 legend=off repline=off repplot=on logical=off ");
   fprintf(fp, "lc=off ls=solid lt=1 \n");
   fprintf(fp, "winset 1;pick tc=1,var=\"velocity\" \n");
   fprintf(fp, "rect var=\"velocity\"\n");
   fprintf(fp, "vec lc=2,size=1.00,headsize=0.25,numvec=%d,legend=on,var=\"velocity\",pop=off;\n", u->dim);
   fprintf(fp, "plot vec \n");
   fprintf(fp, "# \n");
   fprintf(fp, "pause \n");
   fprintf(fp, "# \n");
   fprintf(fp, "clear \n");
   fprintf(fp, "stream lc=2,method=\"rk2a\", maxsteps=35 \n");
   fprintf(fp, "plot stream \n");

   fclose(fp);

   return script_meshtv;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics3D_stokes_silo(const char* filename, const ELT_3D *element, const GEOM_3D *geom, const VEC  *u, const VEC *v, const VEC *w, const VEC *p)
{
#ifdef HAVE_SILO
   DBfile   *file_silo = NULL;
   int      DB_REAL    = sizeof(Real)==sizeof(float) ? DB_FLOAT : DB_DOUBLE ;
#endif
   
   char     *coordnames[3];
   Real     *coords[3];
   int      nzones,nfaces,nnodes;
   int      *znodelist,*fnodelist;
   int      lznodelist,lfnodelist;
   int      znodeorigin,fnodeorigin;
   int      zoneshapecnt[1] ,faceshapecnt[1];
   int      zoneshapesize[1],faceshapesize[1];
   int      nzoneshapes,nfaceshapes;

   int NB_SOMMS_PER_CELL  = 4; /* bacause we wil consider "P1" mesh */
   int NB_FACES_PER_CELL  = 3; /* bacause we wil consider "P1" mesh */

   Real  *vars[3];
   char  *varnames[3];

   /*------------------------------------------------------*/

   int i,e,a,s;

   char silo_file[64];
   
   const GEOM_3D *GeomP1;

   if ( element   == NULL ) error(E_NULL, "graphics3D_stokes_silo");
   if ( geom      == NULL ) error(E_NULL, "graphics3D_stokes_silo");
   if ( u         == NULL ) error(E_NULL, "graphics3D_stokes_silo");
   if ( v         == NULL ) error(E_NULL, "graphics3D_stokes_silo");
   if ( w         == NULL ) error(E_NULL, "graphics3D_stokes_silo");
   if ( p         == NULL ) error(E_NULL, "graphics3D_stokes_silo");
   if ( filename  == NULL ) error(E_NULL, "graphics3D_stokes_silo");


   /*--- ECRITURE DU FICHIER "<filename>.silo"---(MESH-TV)---- */

   snprintf(silo_file, 64, "%s.silo", filename); silo_file[63] = '\0';

   /* Write out the mesh to the file ------------------------------------ DBPutUcdmesh */

   GeomP1 = geom;

   while ( GeomP1->geomBase != NULL )
   {
      GeomP1 = GeomP1->geomBase;
   }

   /* nb of nodes */  /* GeomP1 is is P1 mesh */
   nnodes = GeomP1->NBSOMM;

   /* Name the coordinate axes 'X' and 'Y' and 'Z' */
   coordnames[0] = strdup("X");
   coordnames[1] = strdup("Y");
   coordnames[2] = strdup("Z");

   /* Assign coordinates to coordinates array */
   coords[0] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[1] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[2] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));

   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      coords[0][s] = GeomP1->XYSOMM->me[s][0];
      coords[1][s] = GeomP1->XYSOMM->me[s][1];
      coords[2][s] = GeomP1->XYSOMM->me[s][2];
   }
   

   /* Write out the mesh to the file ------------------------------ DBPutZonelist */

   /* nb zones = nb cells of the mesh */
   nzones = geom->NBELMT;
   /* length of vector zonelist which for each zone */
   lznodelist = NB_SOMMS_PER_CELL*nzones;
   /* Give the indices of the nodes for each zone of the mesh */
   znodelist = (int*)calloc(lznodelist,sizeof(int));

   for (i=0; i<nzones; i++)
   for (e=0; e<NB_SOMMS_PER_CELL; e++)
   {
      znodelist[NB_SOMMS_PER_CELL*i+e] = geom->NSELMT->im[i][e];
   }

   /* "C" notation */
   znodeorigin = 0;
   /* only one type of zone : tetrahedra */
   nzoneshapes = 1;
   /* array of lenght "nzoneshapes" containing the number of nodes used by each zone shape (here only 1 zone shape) */
   zoneshapesize[0] = NB_SOMMS_PER_CELL;
   /* array of lenght "nzoneshapes"  containing the number of zones used by each zone shape */
   zoneshapecnt[0]  = nzones;


   /* Write out the mesh to the file --------------------------------- DBPutFacelist */

   /* nb faces = nb faces of the mesh */
   nfaces = geom->NBFACE;
   /* length of vector facelist which for each zone */
   lfnodelist = NB_FACES_PER_CELL*nfaces;
   /* Give the NA indices of the nodes for each zone of the mesh */
   fnodelist = (int*)calloc(lfnodelist,sizeof(int));

   for (i=0; i<nfaces; i++)
   for (a=0; a<NB_FACES_PER_CELL; a++)
   {
      fnodelist[NB_FACES_PER_CELL*i+a] = geom->NSFACE->im[i][a];
   }

   /* "C" notation */
   fnodeorigin = 0;
   /* only one type of face : triangle */
   nfaceshapes=1;
   /* array of lenght "nfaceshapes" containing the number of nodes used by each face shape (here only 1 face shape) */
   faceshapesize[0] = NB_FACES_PER_CELL;
   /* array of lenght "nfaceshapes"  containing the number of zones used by each face shape */
   faceshapecnt[0]  = nfaces;



#ifdef HAVE_SILO

   /* Create the SILO file */

   file_silo = DBCreate(silo_file, DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);

   /* Write out the mesh to the file ---------- DBPutFacelist */

   DBPutFacelist(file_silo, "face", nfaces, 3, fnodelist, lfnodelist, fnodeorigin, NULL, faceshapesize, faceshapecnt, nfaceshapes, NULL, NULL, 0);

   /* Write out the mesh to the file ---------- DBPutZonelist */

   DBPutZonelist(file_silo, "zone", nzones, 3, znodelist, lznodelist, znodeorigin, zoneshapesize, zoneshapecnt, nzoneshapes);

   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   DBPutUcdmesh(file_silo, (char*)geom->type, 3, coordnames, coords, nnodes, nzones, "zone", "face", DB_REAL, NULL);


   /* Write out the variables to the file ---------- DBPutUcdvar */

   vars[0] = (u->ve);
   varnames[0] = strdup("u_comp");
   vars[1] = (v->ve);
   varnames[1] = strdup("v_comp");
   vars[2] = (w->ve);
   varnames[2] = strdup("w_comp");
   
   DBPutUcdvar(file_silo, "velocity", (char*)geom->type,  3, varnames, vars, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL); 

   DBPutUcdvar1(file_silo, "u_1", (char*)geom->type, u->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL); 

   DBPutUcdvar1(file_silo, "u_2", (char*)geom->type, v->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL); 

   DBPutUcdvar1(file_silo, "u_3", (char*)geom->type, w->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);

   /* Close the silo file */

   DBClose(file_silo);

#endif
   
   /* free mem */
   free(coords[0]);
   free(coords[1]);
   free(coords[2]);

   free(znodelist);
   free(fnodelist);

   free(coordnames[0]);
   free(coordnames[1]);
   free(coordnames[2]);

   free(varnames[0]);
   free(varnames[1]);
   free(varnames[2]);

   
   /* end */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics3D       (const char* format, const ELT_3D *element, const GEOM_3D *geom, const VEC *SOL, const char *filename)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics3D");
   if ( element   == ELT_3D_NULL   ) error(E_NULL, "graphics3D");
   if ( geom      == GEOM_3D_NULL  ) error(E_NULL, "graphics3D");
   if ( SOL       == VNULL         ) error(E_NULL, "graphics3D");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics3D");
   
   
   if ( strcmp("vtk", format) == 0 )
   {
      graphics3D_vtk(filename, element, geom, SOL);
      graphics3D_script_vtk (filename);
   }
   else
   if ( strcmp("silo", format) == 0 )
   {
      graphics3D_silo(filename, element, geom, SOL);
      graphics3D_script_silo(filename);
   }
   else
   {
      error(E_UNKNOWN, "graphics3D");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics3D_fun   (const char* format, const ELT_3D *element, const GEOM_3D *geom, const FUN_3D *fun3D, const char *filename)
{
   int s;

   VEC *SOL;

   /* check */
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics3D_fun");
   if ( element   == ELT_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( geom      == GEOM_3D_NULL  ) error(E_NULL, "graphics3D_fun");
   if ( fun3D     == FUN_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( filename == (char*)NULL   ) error(E_NULL, "graphics3D_fun");
   
   /* init */
   SOL = v_get(geom->NBSOMM);

   for ( s=0; s<geom->NBSOMM; s++)
   {
      SOL->ve[s] = fun3D->eval(fun3D, geom->XYSOMM->me[s][0],
                                      geom->XYSOMM->me[s][1],
                                      geom->XYSOMM->me[s][2]); 
   }

   graphics3D(format, element, geom, SOL, filename);

   V_FREE(SOL);
   
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics3D_stokes(const char* format, const ELT_3D *element, const GEOM_3D *geom, const VEC *u, const VEC *v, const VEC *w, const VEC *p, const char *filename)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics3D_stokes");
   if ( element   == ELT_3D_NULL   ) error(E_NULL, "graphics3D_stokes");
   if ( geom      == GEOM_3D_NULL  ) error(E_NULL, "graphics3D_stokes");
   if ( u         == VNULL         ) error(E_NULL, "graphics3D_stokes");
   if ( u         == VNULL         ) error(E_NULL, "graphics3D_stokes");
   if ( w         == VNULL         ) error(E_NULL, "graphics3D_stokes");
   if ( p         == VNULL         ) error(E_NULL, "graphics3D_stokes");
   if ( filename  == (char*)NULL   ) error(E_NULL, "graphics3D_stokes");
   
   
   if ( strcmp("vtk", format) == 0 )
   {
      graphics3D_stokes_vtk(filename, element, geom, u, v, w, p);
      graphics3D_stokes_script_vtk (filename);
   }
   else
   if ( strcmp("silo", format) == 0 )
   {
      graphics3D_stokes_silo(filename, element, geom, u, v, w, p);
      graphics3D_stokes_script_silo(filename, u , v , w , p);
   }
   else
   {
      error(E_UNKNOWN, "graphics3D_stokes");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics3D_stokes_fun   (const char* format, const ELT_3D *element, const GEOM_3D *geom, const FUN_3D *Fu, const FUN_3D *Fv, const FUN_3D *Fw, const FUN_3D *Fp, const char *filename)
{
   int s;

   VEC *U ;
   VEC *V ;
   VEC *W ;
   VEC *P ;

   /* check */
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics3D_fun");
   if ( element   == ELT_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( geom      == GEOM_3D_NULL  ) error(E_NULL, "graphics3D_fun");
   if ( Fu        == FUN_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( Fv        == FUN_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( Fw        == FUN_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( Fp        == FUN_3D_NULL   ) error(E_NULL, "graphics3D_fun");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics3D_fun");

   /* init */
   U = v_get(geom->NBSOMM);
   V = v_get(geom->NBSOMM);
   W = v_get(geom->NBSOMM);
   P = v_get(geom->geomBase->NBSOMM);
   
   for (s=0; s<geom->NBSOMM; s++)
   {
      U->ve[s] = Fu->eval(Fu, geom->XYSOMM->me[s][0],
                              geom->XYSOMM->me[s][1],
                              geom->XYSOMM->me[s][2]); 

      V->ve[s] = Fv->eval(Fv, geom->XYSOMM->me[s][0],
                              geom->XYSOMM->me[s][1],
                              geom->XYSOMM->me[s][2]);

      W->ve[s] = Fw->eval(Fw, geom->XYSOMM->me[s][0],
                              geom->XYSOMM->me[s][1],
                              geom->XYSOMM->me[s][2]);
   }

   for (s=0; s<geom->geomBase->NBSOMM; s++)
   {
      P->ve[s] = Fp->eval(Fp, geom->XYSOMM->me[s][0],
                              geom->XYSOMM->me[s][1],
                              geom->XYSOMM->me[s][2]);

   }

   graphics3D_stokes(format, element, geom, U, V, W, P, filename);

   V_FREE(U);
   V_FREE(V);
   V_FREE(W);
   V_FREE(P);
   
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/
