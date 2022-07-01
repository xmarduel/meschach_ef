
/** \file graphics_output_2D.c
 *
 * Contains all the output functions
 *
 */

#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

//#include "meschach_config.h"

#ifdef HAVE_SILO
#include "silo.h"
#endif

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_2D.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"


#define VTK_LINE_LIN      3
#define VTK_LINE_QUA     21

#define VTK_TRIANGLE_LIN  5
#define VTK_TRIANGLE_QUA 22


#define XVTK_NB_CONTOURS_DEFAULT 7

#define XVTK_PLOT_PLANE   0
#define XVTK_PLOT_SURFACE 1

#define XVTK_CELL_TYPE_LIN  1
#define XVTK_CELL_TYPE_QUA  2
#define XVTK_CELL_TYPE_CUB  3

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_gnuplot    (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC  *SOL);
static void graphics2D_matlab     (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC  *SOL);
static void graphics2D_vtk        (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC  *SOL);
static void graphics2D_silo       (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC  *SOL);

static char * graphics2D_script_matlab    (const char *filename);
static char * graphics2D_script_gnuplot   (const char *filename);
static char * graphics2D_script_vtk       (const char *filename);
static char * graphics2D_script_silo      (const char *filename);



static void graphics2D_stokes_silo(const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC *u, const VEC *v, const VEC *p);
static void graphics2D_stokes_vtk (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC *u, const VEC *v, const VEC *p);

static char * graphics2D_stokes_script_vtk   (const char* filename);
static char * graphics2D_stokes_script_silo  (const char* filename, const VEC *u, const VEC *v, const VEC *p);



static void graphics2D_gnuplot_set (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const int nb, const MAT *SOL);
static void graphics2D_silo_set    (const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const int nb, const MAT *SOL);

static void graphics2D_script_gnuplot_set (const char* filename, int nb_plots);


static void graphics2D_vtk_1var(const char *filename, const GEOM_2D *geom, const VEC* var,                   int vtk_cell_type, int vtk_plot_type);
static void graphics2D_vtk_2var(const char *filename, const GEOM_2D *geom, const VEC* var1, const VEC* var2, int vtk_cell_type, int vtk_plot_type);


/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics2D_script_gnuplot (const char* filename)
{
   FILE *fp;
   char *script_gnuplot = (char*)malloc(64);

   /* write the script to dislay the solution */
   snprintf(script_gnuplot, 64, "Script4gnuplot_%s.gnu" , filename); script_gnuplot[63] = '\0';

   fp = fopen(script_gnuplot, "w");

   fprintf(fp, "#! /usr/bin/env gnuplot\n");
   fprintf(fp, "# \n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "# \n");
   /*
      if ( (MyParams->misc_params.iter_info==ITER_INFOe_FILE)||
           (MyParams->misc_params.iter_info==ITER_INFOe_ALL ) )
    {
         fprintf(fp, "set logscale y\n");
         fprintf(fp, "plot [00:%d] [%le:%le] %c\n",MyParams->resol_params.max_steps,
                 MyParams->resol_params.eps_steps*0.01,
                 residu_ini*10+1, '\\');
         fprintf(fp, " '%s' title 'Residu (iter)' with lines \n",
                 MyParams->misc_params.iter_file);
         fprintf(fp, "pause -1 \n\n");
         fprintf(fp, "set nologscale y\n");
    }
    */

   fprintf(fp, "#  \n");
   fprintf(fp, "set parametric \n");
   fprintf(fp, "#set size square \n");
   fprintf(fp, "set size ratio -1 \n");
   fprintf(fp, "#set noxtics   \n");
   fprintf(fp, "#set noytics   \n");
   fprintf(fp, "#set view 60,340,1  \n");
   fprintf(fp, "set xlabel %c X axis %c \n",'"','"');
   fprintf(fp, "set ylabel %c Y axis %c \n",'"','"');
   fprintf(fp, "set xtics 0 ,%lf, 1  \n", 0.2);
   fprintf(fp, "set ytics 0 ,%lf, 1  \n", 0.2);
   fprintf(fp, "splot '%s.dat' title 'Solution App' with lines\n", filename);
   fprintf(fp, "pause -1 \n");
   fprintf(fp, "#  \n");

   fclose(fp);

   return script_gnuplot;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_gnuplot(const char *filename, const ELT_2D *elt, const GEOM_2D *geom, const VEC  *SOL)
{
   FILE *fp;
   char  gnuplot_file[64];
   int  e;

   GEOM_2D *GeomP1 = Geom2D_getP1geom_from(elt, geom);

   if ( elt      == NULL ) error(E_NULL, "graphics2D_gnuplot");
   if ( geom     == NULL ) error(E_NULL, "graphics2D_gnuplot");
   if ( SOL      == NULL ) error(E_NULL, "graphics2D_gnuplot");
   if ( filename == NULL ) error(E_NULL, "graphics2D_gnuplot");

   /* write the files of the solution */
   snprintf(gnuplot_file, 64, "%s.dat", filename);  gnuplot_file[63] = '\0';

   fp = fopen(gnuplot_file, "w");

   for (e=0; e<GeomP1->NBELMT; e++)
   {
      fprintf(fp, "## elt %d ( somms %d - %d - %d )\n", e,
              GeomP1->NSELMT->im[e][0],
              GeomP1->NSELMT->im[e][1],
              GeomP1->NSELMT->im[e][2]);
      fprintf(fp, "%lf %lf %lf \n",
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][0]][0],
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][0]][1],
              SOL->ve[GeomP1->NSELMT->im[e][0]]);

      fprintf(fp, "%lf %lf %lf \n",
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][1]][0],
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][1]][1],
              SOL->ve[GeomP1->NSELMT->im[e][1]]);

      fprintf(fp, "%lf %lf %lf \n",
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][2]][0],
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][2]][1],
              SOL->ve[GeomP1->NSELMT->im[e][2]]);

      fprintf(fp, "%lf %lf %lf \n",
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][0]][0],
              GeomP1->XYSOMM->me[GeomP1->NSELMT->im[e][0]][1],
              SOL->ve[GeomP1->NSELMT->im[e][0]]);

      fprintf(fp, "\n\n"); /* for a new "data file" in the gnuplot data-file */
   }

   fclose(fp);

   GEOM_2D_FREE(GeomP1);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_script_gnuplot_set (const char* filename, int nb_plots)
{
   FILE *fp;
   char script_gnuplot[64];
   int i;

   /* write the script to dislay the solution */
   snprintf(script_gnuplot, 64, "Script4gnuplot_%s.gnu", filename);
   script_gnuplot[63] = '\0';

   fp = fopen(script_gnuplot, "w");

   fprintf(fp, "#! /usr/bin/env gnuplot\n");
   fprintf(fp, "# \n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "# \n");

   fprintf(fp, "# \n");
   fprintf(fp, "set parametric \n");
   fprintf(fp, "#set size square \n");
   fprintf(fp, "set size ratio -1 \n");
   fprintf(fp, "#set noxtics   \n");
   fprintf(fp, "#set noytics   \n");
   fprintf(fp, "#set view 60,340,1  \n");
   fprintf(fp, "set xlabel %c X axis %c \n",'"','"');
   fprintf(fp, "set ylabel %c Y axis %c \n",'"','"');
   fprintf(fp, "set xtics 0 ,%lf, 1  \n", 0.2);
   fprintf(fp, "set ytics 0 ,%lf, 1  \n", 0.2);

   for (i=0; i<nb_plots; i++)
   {
      fprintf(fp, "splot '%s_%05d.dat' title 'Solution App %d' with lines\n", filename,i,i);
      fprintf(fp, "pause -1 \n");
      fprintf(fp, "#  \n");
   }

   fclose(fp);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_gnuplot_set (const char *filename, const ELT_2D *element, const GEOM_2D *geom, const int nb_plots, const MAT *A)
{
   int i, nb_plots_ok;
   VEC *vec_sol = v_get(A->m);
   char tmp_filename[64];

   if ( nb_plots > A->n )
   {
      nb_plots_ok = A->n;  warning(WARN_UNKNOWN, "graphics2D_gnuplot_set");
   }
   else
   {
      nb_plots_ok = nb_plots;
   }

   for (i=0; i<nb_plots_ok; i++)
   {
      snprintf(tmp_filename, 63, "%s_%05d_", filename,i);

      vec_sol = get_col(A, i, vec_sol);

      graphics2D_gnuplot(tmp_filename, element, geom, vec_sol);
   }

   /* a single script to plot all the solution ... */
   graphics2D_script_gnuplot_set (filename, nb_plots_ok);

   /* clean*/
   V_FREE(vec_sol);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics2D_script_matlab   (const char* filename)
{
   FILE *fp;
   char *script_matlab = (char*)malloc(64);

   /* write the script to dislay the solution */
   snprintf(script_matlab, 64, "Script4matlab_%s.m", filename); script_matlab[63] = '\0';

   fp = fopen(script_matlab, "w");

   fprintf(fp, "%c \n",'%');
   fprintf(fp, "%c File automatically generated  - changes will not be kept - \n", '%');
   fprintf(fp, "%c date : %s", '%', get_date() );
   fprintf(fp, "%c \n",'%');

   fprintf(fp, "load matlab_%s_coordinates.dat; matlab_%s_coordinates(:,1)=[]; \n", filename, filename);
   fprintf(fp, "load matlab_%s_solution.dat; matlab_%s_solution(:,1)=[]; \n", filename, filename);
   fprintf(fp, "load matlab_%s_elements.dat; matlab_%s_elements3(:,1)=[]; \n", filename, filename);
   fprintf(fp, "%c \n",'%');
   fprintf(fp, "figure(1) \n");
   fprintf(fp, "set(1,'Position',[750 450 500 500]) \n");
   fprintf(fp, "trisurf(matlab_%s_elements3,matlab_%s_coordinates(:,1),matlab_%s_coordinates(:,2),matlab_%s_solution,... \n",
           filename, filename, filename, filename);
   fprintf(fp, "        'facecolor','interp','LineStyle','none'); \n");
   fprintf(fp, "hold on;           \n");
   fprintf(fp, "view(10,40);       \n");
   fprintf(fp, "rotate3d;          \n");
   fprintf(fp, "%c \n",'%');

   fclose(fp);

   return script_matlab;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_matlab(const char *filename, const ELT_2D *element, const GEOM_2D *geom, const VEC  *SOL)
{
   int s,e;
   char matlab_coordinates[64];
   char matlab_solution[64];
   char matlab_elements3[64];

   GEOM_2D *GeomP1 = Geom2D_getP1geom_from(element, geom);

   FILE *fp;

   /* write the script to dislay the solution */
   snprintf(matlab_coordinates, 64, "matlab_%s_coordinates.dat" , filename);
   matlab_coordinates[63] = '\0';

   snprintf(matlab_solution, 64, "matlab_%s_solution.dat" , filename);
   matlab_solution[63] = '\0';

   snprintf(matlab_elements3, 64, "matlab_%s_elements3.dat" , filename);
   matlab_elements3[63] = '\0';


   /* and write the files that this script reads */
   fp = fopen(matlab_coordinates, "w");
   fprintf(fp, "%c Input-file of vertices for the mesh \n",'%');
   fprintf(fp, "%c node-number  /  x-coordinate  / y-coordinate \n",'%');
   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      fprintf(fp, "%d \t %lf %lf \n",s+1,
              GeomP1->XYSOMM->me[s][0],
              GeomP1->XYSOMM->me[s][1]);
   }
   fclose(fp);

   fp = fopen(matlab_elements3, "w");
   fprintf(fp, "%c Input-file of triangles for the mesh \n",'%');
   fprintf(fp, "%c element-number  /  1-node  /  2-node  /  3-node \n",'%');
   for (e=0; e<GeomP1->NBELMT; e++)
   {
      fprintf(fp, "%d \t %d %d %d \n",e+1,
              GeomP1->NSELMT->im[e][0]+1,
              GeomP1->NSELMT->im[e][1]+1,
              GeomP1->NSELMT->im[e][2]+1);
   }
   fclose(fp);

   fp = fopen(matlab_solution, "w");
   fprintf(fp, "%c vector solution on the mesh (element3.dat + coordinates) \n",'%');
   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      fprintf(fp, "%d \t %lf \n",s+1,SOL->ve[s]);
   }
   fclose(fp);

   GEOM_2D_FREE(GeomP1);

   /* end */
   return;
}


/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics2D_script_silo(const char* filename)
{
   /* with MESHTVX */
   char *script_meshtv = (char*)malloc(64);
   FILE *fp;

   snprintf(script_meshtv, 64, "Script4meshtv_%s.sil", filename); script_meshtv[63] = '\0';

   fp = fopen(script_meshtv, "w");

   fprintf(fp, "#! /usr/bin/env meshtvx\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
#ifdef HAVE_SILO
   fprintf(fp, "# silo version : %s\n", DBVersion() );
#else
   fprintf(fp, "# date : %s", get_date() );
#endif
   fprintf(fp, "#\n");
   fprintf(fp, "#  host[] meshtvx -s name_of_the_file \n");
   fprintf(fp, "# \n");
   fprintf(fp, "open \"./%s.silo\";\n", filename);
   fprintf(fp, "surf surfcol=z,legend=on,var=\"vitesse\";\n");
   fprintf(fp, "plot surf");

   fclose(fp);

   return script_meshtv;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_silo(const char* filename, const ELT_2D *element, const GEOM_2D *geom, const VEC  *SOL)
{
#ifdef HAVE_SILO
   DBfile   *db = NULL;
   int      DB_REAL    = sizeof(Real)==sizeof(float) ? DB_FLOAT : DB_DOUBLE ; /* our Real is float or double */
#endif

   char     silo_file[64];

   char     *coordnames[2];
   Real     *coords[2];
   int      nzones,nfaces,nnodes;
   int      *znodelist,*fnodelist;
   int      lznodelist,lfnodelist;
   int      znodeorigin,fnodeorigin;
   int      zoneshapecnt[1] ,faceshapecnt[1];
   int      zoneshapesize[1],faceshapesize[1];
   int      nzoneshapes,nfaceshapes;

   int i,e,a,s;

   int NB_SOMM_CELL  = 3;
   int NB_SOMM_FACE  = 2;

   GEOM_2D *GeomP1 = Geom2D_getP1geom_from(element, geom);

   snprintf(silo_file, 64, "%s.silo", filename); silo_file[63] = '\0';

   /*--- ECRITURE DU FICHIER "Solution_Approc2.silo"---(MESH-TV)---- */

   /* Write out the mesh to the file ----------------------------------------- DBPutZonelist */
   nzones = GeomP1->NBELMT;      /* nb cells = nb zones */
   lznodelist = NB_SOMM_CELL*(GeomP1->NBELMT); /* nb nodes pro triangle */
   /* Give the 3 indices of the nodes for each  zone of the mesh  */
   znodelist = (int*)calloc(lznodelist, sizeof(int));
   for (i=0; i<GeomP1->NBELMT; i++)
   for (e=0; e<NB_SOMM_CELL; e++)
   {
      znodelist[NB_SOMM_CELL*i+e] = GeomP1->NSELMT->im[i][e];
   }

   znodeorigin= 0;

   nzoneshapes = 1;                        /* only one type of zone : triangle         */
   zoneshapesize[0] = NB_SOMM_CELL;        /* array of lenght "nzoneshapes"            */
   /* containing the number of nodes used by  each zone shape (here only 1 zone shape) */
   zoneshapecnt[0]  = GeomP1->NBELMT; /* array of lenght "nzoneshapes"                 */
   /* containing the number of zones used by each zone shape                           */


   /* Write out the mesh to the file ------------------------------------------ DBPutFacelist */
   nfaces = GeomP1->NBFACE;
   lfnodelist = NB_SOMM_FACE*GeomP1->NBFACE; /* nb nodes pro face */
   /* Give the 2 indices of the nodes for each  face of the mesh  */
   fnodelist  = (int*)calloc(lfnodelist, sizeof(int));
   for (i=0; i<GeomP1->NBFACE; i++)
   for (a=0; a<NB_SOMM_FACE; a++)
   {
      fnodelist[NB_SOMM_FACE*i+a] = GeomP1->NSFACE->im[i][a];
   }

   fnodeorigin = 0;

   nfaceshapes=1;                             /* only one type of zone : triangle      */
   faceshapesize[0] = NB_SOMM_FACE;           /* array of lenght "nfaceshapes"         */
   /* containing the number of nodes used by  each face shape (here only 1 face shape) */
   faceshapecnt[0]  = GeomP1->NBFACE; /* array of lenght "nfaceshapes"                 */
   /* containing the number of faces used by each face shape                           */


   /* Write out the mesh to the file ----------------------------------------- DBPutUcdmesh */

   nnodes = GeomP1->NBSOMM;      /* nb of nodes */  /* GeomP1 is is P1 mesh */

   coordnames[0] = strdup("X");
   coordnames[1] = strdup("Y");

   /* Assign coordinates to coordinates array */
   coords[0] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[1] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      coords[0][s] = GeomP1->XYSOMM->me[s][0];
      coords[1][s] = GeomP1->XYSOMM->me[s][1];
   }

   /* ---------------------------------------------------------------------------------- */


#ifdef HAVE_SILO

   /* Create the SILO file */

   db = DBCreate(silo_file, DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);

   /* Write out the mesh to the file ---------- DBPutFacelist */

   DBPutFacelist(db, "face", nfaces, 2, fnodelist, lfnodelist, fnodeorigin, NULL, faceshapesize, faceshapecnt, nfaceshapes, NULL, NULL, 0);

   /* Write out the mesh to the file ---------- DBPutZonelist */

   DBPutZonelist(db, "zone", nzones, 2, znodelist, lznodelist, znodeorigin, zoneshapesize, zoneshapecnt, nzoneshapes);

   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   DBPutUcdmesh(db, "mesh", 2, coordnames, coords, nnodes, nzones, "zone", "face", DB_REAL, NULL);

   /* Write out the variables to the file ---------- DBPutUcdvar1 */

   DBPutUcdvar1(db, "vitesse", "mesh", SOL->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);

   /* Close the silo file */

   DBClose(db);

#endif

   /* free mem */
   free(fnodelist);
   free(znodelist);
   free(coords[0]);
   free(coords[1]);
   free(coordnames[0]);
   free(coordnames[1]);

   GEOM_2D_FREE(GeomP1);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_silo_set(const char* filename, const ELT_2D *element, const GEOM_2D *geom, const int nb_plot, const MAT *A)
{
#ifdef HAVE_SILO
   DBfile   *db = NULL;
   int      DB_REAL    = sizeof(Real)==sizeof(float) ? DB_FLOAT : DB_DOUBLE ; /* our Real is float or double */
#endif

   char     silo_file[64];

   char     *coordnames[2];
   Real     *coords[2];
   int      nzones,nfaces,nnodes;
   int      *znodelist,*fnodelist;
   int      lznodelist,lfnodelist;
   int      znodeorigin,fnodeorigin;
   int      zoneshapecnt[1] ,faceshapecnt[1];
   int      zoneshapesize[1],faceshapesize[1];
   int      nzoneshapes,nfaceshapes;

   int i,e,a,s;
   int nb_plot_ok;

   int NB_SOMM_CELL  = 3;
   int NB_SOMM_FACE  = 2;

   GEOM_2D *GeomP1 = Geom2D_getP1geom_from(element, geom);

   /* check */
   if ( nb_plot > A->n )
   {
      nb_plot_ok = A->n;  warning(WARN_UNKNOWN, "graphics2D_silo_set");
   }
   else
   {
      nb_plot_ok = nb_plot;
   }

   /* start */
   snprintf(silo_file, 64, "%s.silo" , filename); silo_file[63] = '\0';

   /*--- ECRITURE DU FICHIER "<filename>.silo"---(MESH-TV)---- */

   /* Write out the mesh to the file ----------------------------------------- DBPutZonelist */
   nzones = GeomP1->NBELMT;      /* nb cells = nb zones */
   lznodelist = NB_SOMM_CELL*(GeomP1->NBELMT); /* nb nodes pro triangle */
   /* Give the 3 indices of the nodes for each  zone of the mesh  */
   znodelist = (int*)calloc(lznodelist, sizeof(int));
   for (i=0; i<GeomP1->NBELMT; i++)
   for (e=0; e<NB_SOMM_CELL; e++)
   {
      znodelist[NB_SOMM_CELL*i+e] = GeomP1->NSELMT->im[i][e];
   }

   znodeorigin= 0;

   nzoneshapes = 1;                        /* only one type of zone : triangle         */
   zoneshapesize[0] = NB_SOMM_CELL;        /* array of lenght "nzoneshapes"            */
   /* containing the number of nodes used by  each zone shape (here only 1 zone shape) */
   zoneshapecnt[0]  = GeomP1->NBELMT; /* array of lenght "nzoneshapes"                 */
   /* containing the number of zones used by each zone shape                           */


   /* Write out the mesh to the file ------------------------------------------ DBPutFacelist */
   nfaces = GeomP1->NBFACE;
   lfnodelist = NB_SOMM_FACE*GeomP1->NBFACE; /* nb nodes pro face */
   /* Give the 2 indices of the nodes for each  face of the mesh  */
   fnodelist  = (int*)calloc(lfnodelist, sizeof(int));
   for (i=0; i<GeomP1->NBFACE; i++)
   for (a=0; a<NB_SOMM_FACE; a++)
   {
      fnodelist[NB_SOMM_FACE*i+a] = GeomP1->NSFACE->im[i][a];
   }

   fnodeorigin = 0;

   nfaceshapes=1;                             /* only one type of zone : triangle      */
   faceshapesize[0] = NB_SOMM_FACE;           /* array of lenght "nfaceshapes"         */
   /* containing the number of nodes used by  each face shape (here only 1 face shape) */
   faceshapecnt[0]  = GeomP1->NBFACE; /* array of lenght "nfaceshapes"                 */
   /* containing the number of faces used by each face shape                           */


   /* Write out the mesh to the file ----------------------------------------- DBPutUcdmesh */

   nnodes = GeomP1->NBSOMM;      /* nb of nodes */  /* GeomP1 is is P1 mesh */

   coordnames[0] = strdup("X");
   coordnames[1] = strdup("Y");

   /* Assign coordinates to coordinates array */
   coords[0] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[1] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      coords[0][s] = GeomP1->XYSOMM->me[s][0];
      coords[1][s] = GeomP1->XYSOMM->me[s][1];
   }

   /* ---------------------------------------------------------------------------------- */


#ifdef HAVE_SILO

   /* Create the SILO file */

   db = DBCreate(silo_file, DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);

   /* Write out the mesh to the file ---------- DBPutFacelist */

   DBPutFacelist(db, "face", nfaces, 2, fnodelist, lfnodelist, fnodeorigin, NULL, faceshapesize, faceshapecnt, nfaceshapes, NULL, NULL, 0);

   /* Write out the mesh to the file ---------- DBPutZonelist */

   DBPutZonelist(db, "zone", nzones, 2, znodelist, lznodelist, znodeorigin, zoneshapesize, zoneshapecnt, nzoneshapes);

   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   DBPutUcdmesh(db, "mesh", 2, coordnames, coords, nnodes, nzones, "zone", "face", DB_REAL, NULL);

   /* Write out the variables to the file ---------- DBPutUcdvar1 */

   {
      int i;
      VEC *SOL = v_get(A->m);
      char tmp_filename[64];

      for (i=0; i<nb_plot_ok; i++)
      {
         snprintf(tmp_filename, 63, "%s%05d", filename, i);

         SOL = get_col(A, i, SOL);

         DBPutUcdvar1(db, tmp_filename , "mesh", SOL->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);
      }

      V_FREE(SOL);
   }

   /* Close the silo file */

   DBClose(db);

#endif

   /* free mem */
   free(fnodelist);
   free(znodelist);
   free(coords[0]);
   free(coords[1]);
   free(coordnames[0]);
   free(coordnames[1]);

   GEOM_2D_FREE(GeomP1);

   /* finito */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics2D_script_vtk(const char* filename)
{
   FILE *fp;
   char *script_vtk = (char*)malloc(64);

   snprintf(script_vtk, 64, "Script4vtk_%s.py", filename); script_vtk[63] = '\0';
   fp = fopen(script_vtk, "w");

   /* Script python */
   fprintf(fp, "#!/usr/bin/env python\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated - vtk version : 4.2\n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "#\n");
   fprintf(fp, "import vtk\n");
   fprintf(fp, "\n");
   fprintf(fp, "\n");
   fprintf(fp, "reader = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader.SetFileName(\"./%s.vtk\")\n", filename);
   fprintf(fp, "reader.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "lut = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut.SetRampToSQRT() \n");
   fprintf(fp, "lut.Build() \n");
   fprintf(fp, "\n");
   fprintf(fp, "valrange      = reader.GetOutput().GetPointData().GetScalars().GetRange()\n");
   fprintf(fp, "\n");
   fprintf(fp, "geofilter = vtk.vtkGeometryFilter()\n");
   fprintf(fp, "geofilter.SetInputData(reader.GetOutput())\n");
   fprintf(fp, "geofilter.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "polydatanormals = vtk.vtkPolyDataNormals()\n");
   fprintf(fp, "polydatanormals.SetInputData(geofilter.GetOutput())\n");
   fprintf(fp, "polydatanormals.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "surfaceMapper = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper.SetInputData(polydatanormals.GetOutput())\n");
   fprintf(fp, "#surfaceMapper.SetInputData(reader.GetOutput()) # -> jagged effect \n");
   fprintf(fp, "surfaceMapper.SetLookupTable( lut ) \n");
   fprintf(fp, "surfaceMapper.SetScalarRange( valrange[0], valrange[1]) \n");
   fprintf(fp, "surfaceMapper.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "surfaceActor = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor.SetMapper(surfaceMapper)\n");
   fprintf(fp, "\n");
   fprintf(fp, "\n");
   fprintf(fp, "ren = vtk.vtkRenderer()\n");
   fprintf(fp, "ren.SetBackground(131, 22, 21)\n");
   fprintf(fp, "ren.AddActor(surfaceActor)\n");
   fprintf(fp, "\n");
   fprintf(fp, "renWin = vtk.vtkRenderWindow()\n");
   fprintf(fp, "renWin.AddRenderer(ren)\n");
   fprintf(fp, "renWin.SetSize( 500, 500)\n");
   fprintf(fp, "\n");
   fprintf(fp, "iren = vtk.vtkRenderWindowInteractor()\n");
   fprintf(fp, "iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())\n");
   fprintf(fp, "iren.SetRenderWindow(renWin)\n");
   fprintf(fp, "iren.Initialize()\n");
   fprintf(fp, "iren.Start()\n");

   fclose(fp);

   return script_vtk;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_vtk(const char *filename, const ELT_2D *element, const GEOM_2D *geom, const VEC  *SOL)
{
   char vtk_file[64];

   int vtk_cell_type;
   int vtk_plot_type;

   const GEOM_2D *GeomX;

   snprintf(vtk_file, 64, "%s.vtk" , filename); vtk_file[63] = '\0';


   if (SOL     == NULL )  error(E_NULL, "graphics2D_vtk");
   if (element == NULL )  error(E_NULL, "graphics2D_vtk");
   if (geom    == NULL )  error(E_NULL, "graphics2D_vtk");

   if ( SOL->dim != geom->NBSOMM ) error(E_SIZES, "graphics2D_vtk");


   if ( strcmp(element->name_ef, "P1") == 0 )
   {
      GeomX = geom;

      vtk_cell_type = XVTK_CELL_TYPE_LIN;
      vtk_plot_type = XVTK_PLOT_SURFACE;
      //vtk_plot_type = XVTK_PLOT_PLANE;
   }
   else
   if ( strcmp(element->name_ef, "P2") == 0 )
   {
      GeomX = geom;

      vtk_cell_type = XVTK_CELL_TYPE_QUA;
      vtk_plot_type = XVTK_PLOT_SURFACE;
   }
   else
   if ( strcmp(element->name_ef, "P3") == 0 )
   {
      GeomX = geom->geomBase->geomBase; /* like "P1" */

      vtk_cell_type = XVTK_CELL_TYPE_LIN;
      vtk_plot_type = XVTK_PLOT_SURFACE;
   }
   else
   if ( strcmp(element->name_ef, "P1b") == 0 )
   {
      GeomX = geom->geomBase;  /* like "P1" */

      vtk_cell_type = XVTK_CELL_TYPE_LIN;
      vtk_plot_type = XVTK_PLOT_SURFACE;
   }
   else
   {
      error(E_UNKNOWN, "graphics2D_vtk");
   }


   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/

   graphics2D_vtk_1var(vtk_file, GeomX, SOL, vtk_cell_type, vtk_plot_type);

   /* end */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics2D_stokes_script_silo(const char* filename, const VEC *u, const VEC *v, const VEC *p)
{
   FILE *fp;
   char *script_meshtv = (char*)malloc(64);

   snprintf(script_meshtv, 64, "Script4meshtv_%s.sil", filename); script_meshtv[63] = '\0';

   fp = fopen(script_meshtv, "w");
   if (fp == NULL) error(E_NULL, "graphics2D_stokes_script_silo");

   /* Script MESHTVX */
   fprintf(fp, "#! /usr/bin/env meshtvx\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
#ifdef HAVE_SILO
   fprintf(fp, "# silo version : %s\n", DBVersion() );
#else
   fprintf(fp, "# date : %s", get_date() );
#endif
   fprintf(fp, "# usage : host[] meshtvx -s name_of_the_file.sil \n");
   fprintf(fp, "# \n");
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

/*-----------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------*/

static void graphics2D_stokes_silo(const char* filename, const ELT_2D *element, const GEOM_2D *geom, const VEC  *u, const VEC *v, const VEC *p)
{
#ifdef HAVE_SILO
   DBfile   *db = NULL;
   int      DB_REAL    = sizeof(Real)==sizeof(float) ? DB_FLOAT : DB_DOUBLE ; /* our Real is float or double */
#endif

   /*--- ECRITURE DU FICHIER "Solution_Stokes.silo"---(MESH-TV)---- */

   char     *coordnames[2];
   Real     *coords[2];
   int      nzones,nfaces,nnodes;
   int      *znodelist,*fnodelist;
   int      lznodelist,lfnodelist;
   int      znodeorigin,fnodeorigin;
   int      zoneshapecnt[1] ,faceshapecnt[1];
   int      zoneshapesize[1],faceshapesize[1];
   int      nzoneshapes,nfaceshapes;

   Real  *vars[2];
   char  *varnames[2];

   int i,e,a,s;

   int NB = 3; /* "P1" */
   int NA = 2; /* "P1" */

   GEOM_2D *GeomP1 = Geom2D_getP1geom_from(element, geom);

   char silo_file[64];
   snprintf(silo_file, 64, "%s.silo", filename); silo_file[63] = '\0';


   /* Write out the mesh to the file ------------------------------ DBPutZonelist */

   nzones = GeomP1->NBELMT;      /* nb cells = nb zones */
   lznodelist = NB*(GeomP1->NBELMT); /* lg of vector zonelist which for each zone */
   /* Give the 3 indices of the nodes for each   zone of the mesh             */
   znodelist = (int*)calloc(lznodelist,sizeof(int));

   for (i=0; i<GeomP1->NBELMT; i++)
   for (e=0; e<NB; e++)
   {
      znodelist[NB*i+e] = GeomP1->NSELMT->im[i][e];
   }

   znodeorigin = 0;

   nzoneshapes = 1;            /* only one type of zone : triangle */
   zoneshapesize[0] = NB;      /* array of lenght "nzoneshapes"            */
   /* containing the number of nodes used by each zone shape (here only 1 zone shape) */
   zoneshapecnt[0]  = GeomP1->NBELMT; /* array of lenght "nzoneshapes"       */
   /* containing the number of zones used  by each zone shape              */


   /* Write out the mesh to the file --------------------------------- DBPutFacelist */
   nfaces = GeomP1->NBFACE;
   lfnodelist = NA*GeomP1->NBFACE; /* 2 nodes pro face               */
   /* Give the 2 indices of the nodes for each  face of the mesh  */
   fnodelist  = (int*)calloc(lfnodelist, sizeof(int));
   for (i=0; i<GeomP1->NBFACE; i++)
   for (a=0; a<NA; a++)
   {
      fnodelist[NA*i+a] = GeomP1->NSFACE->im[i][a];
   }

   fnodeorigin = 0;

   nfaceshapes=1;         /* only one type of zone : triangle      */
   faceshapesize[0] = NA; /* array of lenght "nfaceshapes"         */
   /* containing the number of nodes used by  each face shape (here only 1 face shape) */
   faceshapecnt[0]  = GeomP1->NBFACE; /* array of lenght "nfaceshapes"                   */
   /* containing the number of faces used by each face shape                           */


   /* Write out the mesh to the file ------------------------------------ DBPutUcdmesh */
   nnodes = GeomP1->NBSOMM;      /* nb of nodes */  /* GeomP1 is is P1 mesh */

   /* Name the coordinate axes 'X' and 'Y'  */
   coordnames[0] = strdup("X");
   coordnames[1] = strdup("Y");

   /* Assign coordinates to coordinates array */
   coords[0] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   coords[1] = (Real*)calloc(GeomP1->NBSOMM, sizeof(Real));
   for (s=0; s<GeomP1->NBSOMM; s++)
   {
      coords[0][s] = GeomP1->XYSOMM->me[s][0];
      coords[1][s] = GeomP1->XYSOMM->me[s][1];
   }


#ifdef HAVE_SILO

   /* Create the SILO file */

   db = DBCreate(silo_file, DB_CLOBBER, DB_LOCAL, NULL, DB_PDB);

   /* Write out the mesh to the file ---------- DBPutFacelist */

   DBPutFacelist(db, "faceV", nfaces, 2, fnodelist, lfnodelist, fnodeorigin, NULL, faceshapesize, faceshapecnt, nfaceshapes, NULL, NULL, 0);

   /* Write out the mesh to the file ---------- DBPutZonelist */

   DBPutZonelist(db, "zoneV", nzones, 2, znodelist, lznodelist, znodeorigin, zoneshapesize, zoneshapecnt, nzoneshapes);

   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   DBPutUcdmesh(db, "meshV", 2, coordnames, coords, nnodes, nzones, "zoneV", "faceV", DB_REAL, NULL);

   /* Write out the variables to the file ---------- DBPutUcdvar */

   vars[0] = (u->ve);
   varnames[0] = "u_comp";
   vars[1] = (v->ve);
   varnames[1] = "v_comp";

   DBPutUcdvar(db, "velocity", "meshV",  2, varnames, vars, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);

   DBPutUcdvar1(db, "u_1", "meshV", u->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);

   DBPutUcdvar1(db, "u_2", "meshV", v->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);

#endif

   /* free mem */
   free(fnodelist);
   free(znodelist);
   free(coords[0]);
   free(coords[1]);
   free(coordnames[0]);
   free(coordnames[1]);



   /* Write out the mesh to the file ------------------------------ DBPutZonelist */

   nzones = geom->geomBase->NBELMT;      /* nb cells = nb zones */
   lznodelist = NB*(geom->geomBase->NBELMT); /* lg of vector zonelist which for each zone */
   /* Give the 3 indices of the nodes for each   zone of the mesh             */
   znodelist = (int*)calloc(lznodelist,sizeof(int));

   for (i=0; i<geom->geomBase->NBELMT; i++)
   for (e=0; e<NB; e++)
   {
      znodelist[NB*i+e] = geom->geomBase->NSELMT->im[i][e];
   }

   znodeorigin = 0;

   nzoneshapes = 1;            /* only one type of zone : triangle */
   zoneshapesize[0] = NB;      /* array of lenght "nzoneshapes"            */
   /* containing the number of nodes used by each zone shape (here only 1 zone shape) */
   zoneshapecnt[0]  = geom->geomBase->NBELMT; /* array of lenght "nzoneshapes"       */
   /* containing the number of zones used  by each zone shape              */


   /* Write out the mesh to the file --------------------------------- DBPutFacelist */
   nfaces = geom->geomBase->NBFACE;
   lfnodelist = NA*geom->geomBase->NBFACE; /* 2 nodes pro face               */
   /* Give the 2 indices of the nodes for each  face of the mesh  */
   fnodelist  = (int*)calloc(lfnodelist, sizeof(int));
   for (i=0; i<geom->geomBase->NBFACE; i++)
   for (a=0; a<NA; a++)
   {
      fnodelist[NA*i+a] = geom->geomBase->NSFACE->im[i][a];
   }

   fnodeorigin = 0;

   nfaceshapes=1;         /* only one type of zone : triangle      */
   faceshapesize[0] = NA; /* array of lenght "nfaceshapes"         */
   /* containing the number of nodes used by  each face shape (here only 1 face shape) */
   faceshapecnt[0]  = geom->geomBase->NBFACE; /* array of lenght "nfaceshapes"                   */
   /* containing the number of faces used by each face shape                           */


   /* Write out the mesh to the file ------------------------------------ DBPutUcdmesh */
   nnodes = geom->geomBase->NBSOMM;      /* nb of nodes */  /* geom->geomBase is is P1 mesh */

   /* Name the coordinate axes 'X' and 'Y'  */
   coordnames[0] = strdup("X");
   coordnames[1] = strdup("Y");

   /* Assign coordinates to coordinates array */
   coords[0] = (Real*)calloc(geom->geomBase->NBSOMM, sizeof(Real));
   coords[1] = (Real*)calloc(geom->geomBase->NBSOMM, sizeof(Real));
   for (s=0; s<geom->geomBase->NBSOMM; s++)
   {
      coords[0][s] = geom->geomBase->XYSOMM->me[s][0];
      coords[1][s] = geom->geomBase->XYSOMM->me[s][1];
   }


#ifdef HAVE_SILO

   /* Write out the mesh to the file ---------- DBPutFacelist */

   DBPutFacelist(db, "faceP", nfaces, 2, fnodelist, lfnodelist, fnodeorigin, NULL, faceshapesize, faceshapecnt, nfaceshapes, NULL, NULL, 0);

   /* Write out the mesh to the file ---------- DBPutZonelist */

   DBPutZonelist(db, "zoneP", nzones, 2, znodelist, lznodelist, znodeorigin, zoneshapesize, zoneshapecnt, nzoneshapes);

   /* Write out the mesh to the file ---------- DBPutUcdmesh */

   DBPutUcdmesh(db, "meshP", 2, coordnames, coords, nnodes, nzones, "zoneP", "faceP", DB_REAL, NULL);

   /* Write out the variables to the file ---------- DBPutUcdvar */

   DBPutUcdvar1(db, "p", "meshP", p->ve, nnodes, NULL, 0, DB_REAL, DB_NODECENT, NULL);

   /* Close the silo file */

   DBClose(db);

#endif


   /* free mem */
   free(fnodelist);
   free(znodelist);
   free(coords[0]);
   free(coords[1]);
   free(coordnames[0]);
   free(coordnames[1]);

   GEOM_2D_FREE(GeomP1);

   /* finito */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics2D_stokes_script_vtk(const char* filename)
{
   char *script_vtk = (char*)malloc(64);
   FILE *fp;

   snprintf(script_vtk, 64, "Script4vtk_%s.py", filename); script_vtk[63]  = '\0';
   fp = fopen(script_vtk, "w");

   /* Script python */
   fprintf(fp, "#! /usr/bin/env python\n");
   fprintf(fp, "# \n");
   fprintf(fp, "# File automatically generated - vtk version : 5.2\n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "\n");
   fprintf(fp, "import vtk\n");
   fprintf(fp, "\n");
   fprintf(fp, "ren_u = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_u.SetViewport( 0.000, 0.5,  0.333, 1.0) \n");
   fprintf(fp, "ren_p = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_p.SetViewport( 0.333, 0.5,  0.666, 1.0) \n");
   fprintf(fp, "ren_v = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_v.SetViewport( 0.666, 0.5,  1.000, 1.0) \n");
   fprintf(fp, "ren_a = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_a.SetViewport( 0.000, 0.0,  0.500, 0.5) \n");
   fprintf(fp, "ren_s = vtk.vtkRenderer()\n");
   fprintf(fp, "ren_s.SetViewport( 0.500, 0.0,  1.000, 0.5) \n");
   fprintf(fp, "\n");
   fprintf(fp, "renWin = vtk.vtkRenderWindow()\n");
   fprintf(fp, "renWin.AddRenderer( ren_u )\n");
   fprintf(fp, "renWin.AddRenderer( ren_p )\n");
   fprintf(fp, "renWin.AddRenderer( ren_v )\n");
   fprintf(fp, "renWin.AddRenderer( ren_a )\n");
   fprintf(fp, "renWin.AddRenderer( ren_s )\n");
   fprintf(fp, "\n");
   fprintf(fp, "reader_u = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_p = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_v = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_s = vtk.vtkUnstructuredGridReader()\n");
   fprintf(fp, "reader_u.SetFileName( \"./%s_u.vtk\" )\n", filename);
   fprintf(fp, "reader_u.Update()\n");
   fprintf(fp, "reader_p.SetFileName( \"./%s_p.vtk\" )\n", filename);
   fprintf(fp, "reader_p.Update()\n");
   fprintf(fp, "reader_v.SetFileName( \"./%s_v.vtk\" )\n", filename);
   fprintf(fp, "reader_v.Update()\n");
   fprintf(fp, "reader_s.SetFileName( \"./%s_S.vtk\" )\n", filename);
   fprintf(fp, "reader_s.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "lut_u = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut_u.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut_u.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut_u.SetRampToSQRT() \n");
   fprintf(fp, "lut_u.Build() \n");
   fprintf(fp, "\n");
   fprintf(fp, "lut_v = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut_v.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut_v.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut_v.SetRampToSQRT() \n");
   fprintf(fp, "lut_v.Build() \n");
   fprintf(fp, "\n");
   fprintf(fp, "lut_p = vtk.vtkLookupTable() \n");
   fprintf(fp, "lut_p.SetHueRange(0.667, 0.0) \n");
   fprintf(fp, "lut_p.SetNumberOfTableValues(256) \n");
   fprintf(fp, "lut_p.SetRampToSQRT() \n");
   fprintf(fp, "lut_p.Build() \n");
   fprintf(fp, "\n");
   fprintf(fp, "u_range = reader_u.GetOutput().GetPointData().GetScalars().GetRange()\n");
   fprintf(fp, "p_range = reader_p.GetOutput().GetPointData().GetScalars().GetRange()\n");
   fprintf(fp, "v_range = reader_v.GetOutput().GetPointData().GetScalars().GetRange()\n");
   fprintf(fp, "\n");
   fprintf(fp, "geofilter_u = vtk.vtkGeometryFilter()\n");
   fprintf(fp, "geofilter_u.SetInputData(reader_u.GetOutput())\n");
   fprintf(fp, "geofilter_u.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "polydatanormals_u = vtk.vtkPolyDataNormals()\n");
   fprintf(fp, "polydatanormals_u.SetInputData(geofilter_u.GetOutput())\n");
   fprintf(fp, "polydatanormals_u.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "geofilter_v = vtk.vtkGeometryFilter()\n");
   fprintf(fp, "geofilter_v.SetInputData(reader_v.GetOutput())\n");
   fprintf(fp, "geofilter_v.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "polydatanormals_v = vtk.vtkPolyDataNormals()\n");
   fprintf(fp, "polydatanormals_v.SetInputData(geofilter_v.GetOutput())\n");
   fprintf(fp, "polydatanormals_v.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "geofilter_p = vtk.vtkGeometryFilter()\n");
   fprintf(fp, "geofilter_p.SetInputData(reader_p.GetOutput())\n");
   fprintf(fp, "geofilter_p.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "polydatanormals_p = vtk.vtkPolyDataNormals()\n");
   fprintf(fp, "polydatanormals_p.SetInputData(geofilter_p.GetOutput())\n");
   fprintf(fp, "polydatanormals_p.Update()\n");
   fprintf(fp, "\n");

   fprintf(fp, "surfaceMapper_u = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper_p = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper_v = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "surfaceMapper_u.SetInputData( polydatanormals_u.GetOutput() ) \n");
   fprintf(fp, "surfaceMapper_u.SetLookupTable( lut_u ) \n");
   fprintf(fp, "surfaceMapper_u.SetScalarRange( u_range[0], u_range[1] )\n");
   fprintf(fp, "surfaceMapper_p.SetInputData( polydatanormals_p.GetOutput() ) \n");
   fprintf(fp, "surfaceMapper_p.SetLookupTable( lut_p ) \n");
   fprintf(fp, "surfaceMapper_p.SetScalarRange( p_range[0], p_range[1] )\n");
   fprintf(fp, "surfaceMapper_v.SetInputData( polydatanormals_v.GetOutput() ) \n");
   fprintf(fp, "surfaceMapper_v.SetLookupTable( lut_v ) \n");
   fprintf(fp, "surfaceMapper_v.SetScalarRange( v_range[0], v_range[1] )\n");
   fprintf(fp, "surfaceMapper_u.Update()\n");
   fprintf(fp, "surfaceMapper_p.Update()\n");
   fprintf(fp, "surfaceMapper_v.Update()\n");
   fprintf(fp, "\n");
   fprintf(fp, "surfaceActor_u = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor_p = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor_v = vtk.vtkActor()\n");
   fprintf(fp, "surfaceActor_u.SetMapper( surfaceMapper_u )\n");
   fprintf(fp, "surfaceActor_p.SetMapper( surfaceMapper_p )\n");
   fprintf(fp, "surfaceActor_v.SetMapper( surfaceMapper_v )\n");
   fprintf(fp, "\n");
   fprintf(fp, "maxVelocity = reader_s.GetOutput().GetPointData().GetVectors().GetMaxNorm()\n");
   fprintf(fp, "print(\"maxVelocity = \", maxVelocity)\n");
   fprintf(fp, "\n");
   fprintf(fp, "lut = vtk.vtkLookupTable()\n");
   fprintf(fp, "lut.SetHueRange(0.667, 0.0)\n");
   fprintf(fp, "lut.SetNumberOfTableValues(256)\n");
   fprintf(fp, "lut.SetRampToSQRT()\n");
   fprintf(fp, "lut.Build()\n");
   fprintf(fp, "\n");
   fprintf(fp, "WITH_VECTORS = True\n");
   fprintf(fp, "WITH_STREAMLINES = True\n");
   fprintf(fp, "if WITH_VECTORS:\n");
   fprintf(fp, "    # vectors\n");
   fprintf(fp, "    glyph2d_src = vtk.vtkGlyphSource2D()\n");
   fprintf(fp, "    cone        = vtk.vtkConeSource()\n");
   fprintf(fp, "    arrow       = vtk.vtkArrowSource()\n");
   fprintf(fp, "    glyph_src   = cone\n");
   fprintf(fp, "    glyph3d     = vtk.vtkGlyph3D()\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    glph_trfm   = vtk.vtkTransformFilter()\n");
   fprintf(fp, "    glph_trfm.SetTransform (vtk.vtkTransform())\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    scale      = 0.2\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    # initialize\n");
   fprintf(fp, "    glyph2d_src.SetGlyphTypeToArrow()\n");
   fprintf(fp, "    glyph2d_src.SetFilled(0)\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    #cone.SetResolution(5)\n");
   fprintf(fp, "    #cone.SetHeight(1)\n");
   fprintf(fp, "    #cone.SetRadius(0.2)\n");
   fprintf(fp, "    #glph_trfm.SetInput(glyph_src.GetOutput())\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    #tr = glph_trfm.GetTransform()\n");
   fprintf(fp, "    #tr.Identity()\n");
   fprintf(fp, "    #tr.Translate(-1, 0.0, 0.0)\n");
   fprintf(fp, "    #glyph2d_src.SetCenter(0.5, 0.0, 0.0)\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    glyph3d.SetInputData(reader_s.GetOutput())\n");
   fprintf(fp, "    glyph3d.SetSourceConnection(glyph2d_src.GetOutputPort())\n");
   fprintf(fp, "    glyph3d.SetScaleFactor(scale)\n");
   fprintf(fp, "    glyph3d.SetScaleModeToScaleByVector()\n");
   fprintf(fp, "    glyph3d.SetColorModeToColorByVector()\n");
   fprintf(fp, "    glyph3d.SetClamping (1)\n");
   fprintf(fp, "    glyph3d.SetRange(0.0, maxVelocity)\n");
   fprintf(fp, "    glyph3d.Update()\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    vectorsMapper = vtk.vtkDataSetMapper()\n");
   fprintf(fp, "    vectorsMapper.SetInputData(glyph3d.GetOutput())\n");
   fprintf(fp, "    vectorsMapper.SetLookupTable(lut)\n");
   fprintf(fp, "    vectorsMapper.SetScalarRange(0.000000, maxVelocity)\n");
   fprintf(fp, "    vectorsMapper.ScalarVisibilityOn()\n");
   fprintf(fp, "    vectorsMapper.Update()\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    vectorsActor = vtk.vtkActor()\n");
   fprintf(fp, "    vectorsActor.SetMapper(vectorsMapper)\n");
   fprintf(fp, "    vectorsActor.GetProperty().SetLineWidth(1.0)\n");
   fprintf(fp, "    vectorsActor.GetProperty().BackfaceCullingOff()\n");
   fprintf(fp, "    vectorsActor.GetProperty().FrontfaceCullingOff()\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    \n");
   fprintf(fp, "if WITH_STREAMLINES:\n");
   fprintf(fp, "    # streamlines:\n");
   fprintf(fp, "    seeds = vtk.vtkLineSource()\n");
   fprintf(fp, "    # canal\n");
   fprintf(fp, "    seeds.SetPoint1(-2.0, -0.95, 0.0)\n");
   fprintf(fp, "    seeds.SetPoint2(-2.0,  0.95, 0.0)\n");
   fprintf(fp, "    # cavity\n");
   fprintf(fp, "    seeds.SetPoint1(0.025, 0.025, 0.0)\n");
   fprintf(fp, "    seeds.SetPoint2(0.975, 0.975, 0.0)\n");
   fprintf(fp, "    seeds.SetResolution(15)\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    # kovaznay canal geom\n");
   fprintf(fp, "    seeds.SetPoint1(-0.5, -0.5, 0.0)\n");
   fprintf(fp, "    seeds.SetPoint2(-0.5,  0.5, 0.0)\n");
   fprintf(fp, "    seeds.SetResolution(25)\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    integ = vtk.vtkRungeKutta4()\n");
   fprintf(fp, "    streamer = vtk.vtkStreamTracer()\n");
   fprintf(fp, "    streamer.SetInputData(reader_s.GetOutput())\n");
   fprintf(fp, "    streamer.SetSourceConnection(seeds.GetOutputPort())\n");
   fprintf(fp, "    # canal\n");
   fprintf(fp, "    streamer.SetMaximumPropagation(5000)\n");
   fprintf(fp, "    streamer.SetInitialIntegrationStep(0.01)\n");
   fprintf(fp, "    # cavity\n");
   fprintf(fp, "    streamer.SetMaximumPropagation(5000)\n");
   fprintf(fp, "    streamer.SetInitialIntegrationStep(0.1)\n");
   fprintf(fp, "    #streamer.SetIntegrationDirectionToIntegrateBothDirections()\n");
   fprintf(fp, "    streamer.SetIntegrator(integ)\n");
   fprintf(fp, "    #streamer.SpeedScalarsOn()\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    streamMapper = vtk.vtkPolyDataMapper()\n");
   fprintf(fp, "    streamMapper.SetInputConnection(streamer.GetOutputPort())\n");
   fprintf(fp, "    streamMapper.SetScalarRange(0, maxVelocity)\n");
   fprintf(fp, "    streamMapper.ScalarVisibilityOn()\n");
   fprintf(fp, "    streamMapper.SetLookupTable(lut)\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    streamsActor = vtk.vtkActor()\n");
   fprintf(fp, "    streamsActor.SetMapper(streamMapper)\n");
   fprintf(fp, "    \n");
   fprintf(fp, "    \n");
   fprintf(fp, "# rendering\n");
   fprintf(fp, "renWin.SetSize(900, 600) \n");
   fprintf(fp, "ren_u.SetBackground( 1.0, 1.0, 1.0 )\n");
   fprintf(fp, "ren_p.SetBackground( 1.0, 1.0, 1.0 )\n");
   fprintf(fp, "ren_v.SetBackground( 1.0, 1.0, 1.0 )\n");
   fprintf(fp, "\n");
   fprintf(fp, "ren_u.AddActor( surfaceActor_u )\n");
   fprintf(fp, "ren_p.AddActor( surfaceActor_p )\n");
   fprintf(fp, "ren_v.AddActor( surfaceActor_v )\n");
   fprintf(fp, "if WITH_VECTORS: ren_a.AddActor( vectorsActor )\n");
   fprintf(fp, "if WITH_STREAMLINES: ren_s.AddActor( streamsActor )\n");
   fprintf(fp, "\n");
   fprintf(fp, "iren = vtk.vtkRenderWindowInteractor()\n");
   fprintf(fp, "iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())\n");
   fprintf(fp, "iren.SetRenderWindow(renWin)\n");
   fprintf(fp, "\n");
   fprintf(fp, "iren.Initialize()\n");
   fprintf(fp, "iren.Start()\n");

   fclose(fp);

   return script_vtk;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_vtk_1var(const char *filename, const GEOM_2D *geom, const VEC* var, int vtk_cell_type, int vtk_plot_type)
{
   int s,e,k;

   int NB_SOMM_CELL;
   int NB_SOMM_FACE;

   int XVTK_TRIANGLE_TYPE;

   switch(vtk_cell_type)
   {
      case XVTK_CELL_TYPE_LIN: NB_SOMM_CELL = 3; break;
      case XVTK_CELL_TYPE_QUA: NB_SOMM_CELL = 6; break;
      default:                 NB_SOMM_CELL = 3;
   }

   switch(vtk_cell_type)
   {
      case XVTK_CELL_TYPE_LIN: XVTK_TRIANGLE_TYPE = VTK_TRIANGLE_LIN; break;
      case XVTK_CELL_TYPE_QUA: XVTK_TRIANGLE_TYPE = VTK_TRIANGLE_QUA; break;
      default:                 XVTK_TRIANGLE_TYPE = VTK_TRIANGLE_LIN;
   }

   /*------ ECRITURE DU FICHIER ---(vtk)----*/
   /*                                       */
   /* for componant u or v or p             */
   /*                                       */
   /*------ ECRITURE DU FICHIER ---(vtk)----*/

   FILE *fp = fopen(filename, "w");
   /* File VTK for u or v */
   fprintf(fp, "# vtk DataFile Version 4.2 \n");
   fprintf(fp, "Solution PDE problem \n");
   fprintf(fp, "\n");
   fprintf(fp, "ASCII \n");
   fprintf(fp, "\n");
   fprintf(fp, "DATASET UNSTRUCTURED_GRID \n");
   fprintf(fp, "POINTS %d float \n", geom->NBSOMM);
   for (s=0; s<geom->NBSOMM ; s++)
   {
      fprintf(fp, "%lf %lf %lf \n",
              geom->XYSOMM->me[s][0],
              geom->XYSOMM->me[s][1],
              vtk_plot_type == XVTK_PLOT_SURFACE ? var->ve[s] : 0.0);
   }

   fprintf(fp, "\n");
   fprintf(fp, "CELLS %d %d \n", geom->NBELMT, (1+NB_SOMM_CELL)*(geom->NBELMT) );
   for (e=0; e<geom->NBELMT ; e++)
   {
      fprintf(fp, "%d ", NB_SOMM_CELL); /* start the line */
      for (k=0; k<NB_SOMM_CELL; k++)
      {
         fprintf(fp, "%d ", geom->NSELMT->im[e][k]);
      }
      fprintf(fp, "\n"); /* finish the line */
   }

   fprintf(fp, "\n");
   fprintf(fp, "CELL_TYPES %d \n", geom->NBELMT);
   for (e=0; e<geom->NBELMT; e++)
   {
      fprintf(fp, "%d\n", XVTK_TRIANGLE_TYPE);
   }

   fprintf(fp, "\n");
   fprintf(fp, "POINT_DATA %d \n",geom->NBSOMM);
   fprintf(fp, "SCALARS scalars float\n");
   fprintf(fp, "LOOKUP_TABLE default\n");

   for (s=0; s<geom->NBSOMM; s++)
   {
      fprintf(fp, "%lf \n", var->ve[s] );
   }

   fclose(fp);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_vtk_2var(const char *filename, const GEOM_2D *geom, const VEC* var1, const VEC* var2, int vtk_cell_type, int vtk_plot_type)
{
   int s,e,k,f;

   int NB_SOMM_CELL;
   int NB_SOMM_FACE;

   int faces_output=0; /* 0-> do not write faces cells ; 3->write the faces cells */

   int XVTK_TRIANGLE_TYPE;
   int XVTK_FACE_TYPE;

   int MAP_TO_VTK_NUM[3][3] =  /* VTK_ORDER[i] = MY_ORDER[ MAP_CELL_TO_CELL[i] ] */
   {
      {0, 1, 3},
      {1, 2, 4},
      {0, 2, 5},
   };

   switch(vtk_cell_type)
   {
      case XVTK_CELL_TYPE_LIN: XVTK_TRIANGLE_TYPE = VTK_TRIANGLE_LIN; NB_SOMM_CELL = 3;
                               XVTK_FACE_TYPE     = VTK_LINE_LIN    ; NB_SOMM_FACE = 2;
                               break;
      case XVTK_CELL_TYPE_QUA: XVTK_TRIANGLE_TYPE = VTK_TRIANGLE_QUA; NB_SOMM_CELL = 6;
                               XVTK_FACE_TYPE     = VTK_LINE_QUA    ; NB_SOMM_FACE = 3;
                               break;
      default:                 XVTK_TRIANGLE_TYPE = VTK_TRIANGLE_LIN; NB_SOMM_CELL = 3;
                               XVTK_FACE_TYPE     = VTK_LINE_LIN    ; NB_SOMM_FACE = 2;
                               break;
   }

   /*------ ECRITURE DU FICHIER ---(vtk)----*/
   /*                                       */
   /* for componant u and v                 */
   /*                                       */
   /*------ ECRITURE DU FICHIER ---(vtk)----*/

   FILE *fp = fopen(filename, "w");
   /* File VTK for uv */
   fprintf(fp, "# vtk DataFile Version 4.2 \n");
   fprintf(fp, "Solution PDE problem \n");
   fprintf(fp, "\n");
   fprintf(fp, "ASCII \n");
   fprintf(fp, "\n");
   fprintf(fp, "DATASET UNSTRUCTURED_GRID \n");
   fprintf(fp, "POINTS   %d float \n", geom->NBSOMM);
   for (s=0; s<geom->NBSOMM; s++)
   {
      fprintf(fp, "%lf %lf %lf  \n",
              geom->XYSOMM->me[s][0],
              geom->XYSOMM->me[s][1],
              0.0) ;
   }

   fprintf(fp, "\n");
   fprintf(fp, "CELLS %d %d \n", (1/*triangle*/+faces_output/*faces*/)*geom->NBELMT, (/*triangle*/1*(1+NB_SOMM_CELL) + /*faces*/faces_output*(1+NB_SOMM_FACE))*(geom->NBELMT));
   for (e=0; e<geom->NBELMT ; e++)
   {
      fprintf(fp, "%d ", NB_SOMM_CELL); /* start the line */

      for (k=0; k<NB_SOMM_CELL; k++)
      {
         fprintf(fp, "%d ", geom->NSELMT->im[e][k]);
      }
      fprintf(fp, "\n"); /* finish the line */

      /* the 3 faces */
      for (f=0; f<faces_output; f++)
      {
         fprintf(fp, "%d ", NB_SOMM_FACE);

         for (k=0; k<NB_SOMM_FACE; k++)
         {
            fprintf(fp, "%d ", geom->NSELMT->im[e][MAP_TO_VTK_NUM[f][k]]);
         }
         fprintf(fp, "\n");
      }
   }

   fprintf(fp, "\n");
   fprintf(fp, "CELL_TYPES %d \n", (1/*triangle*/+faces_output/*faces*/)*geom->NBELMT);
   for (e=0; e<geom->NBELMT; e++)
   {
      fprintf(fp, "%d\n"        , XVTK_TRIANGLE_TYPE);

      /* the 3 faces */
      for (f=0; f<faces_output; f++)
      {
         fprintf(fp, "%d\n", XVTK_FACE_TYPE);
      }
   }

   fprintf(fp, "\n");
   fprintf(fp, "POINT_DATA %d \n",geom->NBSOMM);
   fprintf(fp, "VECTORS vectors float\n");
   for (s=0; s<geom->NBSOMM ; s++)
   {
      fprintf(fp, "%lf %lf %lf \n", var1->ve[s], var2->ve[s], 0.0);
   }

   fclose(fp);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics2D_stokes_vtk(const char *filename, const ELT_2D *element, const GEOM_2D *geom, const VEC  *u, const VEC *v, const VEC *p)
{
   char u_vtk_file[64];
   char v_vtk_file[64];
   char p_vtk_file[64];
   char V_vtk_file[64]; /* vector */

   const GEOM_2D *GeomV; /* geometry for the velocity */
   const GEOM_2D *GeomP; /* geometry for the pressure */
   const GEOM_2D *GeomX;

   int vtk_cell_type_V;
   int vtk_cell_type_P;

   int vtk_plot_type_V;
   int vtk_plot_type_P;

   snprintf(u_vtk_file, 64, "%s_u.vtk", filename); u_vtk_file[63] = '\0';
   snprintf(v_vtk_file, 64, "%s_v.vtk", filename); v_vtk_file[63] = '\0';
   snprintf(p_vtk_file, 64, "%s_p.vtk", filename); p_vtk_file[63] = '\0';

   snprintf(V_vtk_file, 64, "%s_S.vtk", filename); V_vtk_file[63] = '\0';


   if ( u->dim != geom->NBSOMM ) error(E_SIZES, "graphics2D_stokes_vtk");
   if ( v->dim != geom->NBSOMM ) error(E_SIZES, "graphics2D_stokes_vtk");
   if ( p->dim != geom->geomBase->NBSOMM ) error(E_SIZES, "graphics2D_stokes_vtk");

   if ( strcmp(element->name_ef, "P1") == 0 )
   {
      GeomV = geom;
      GeomP = geom;

      GeomX = geom;

      vtk_cell_type_V = XVTK_CELL_TYPE_LIN;
      vtk_cell_type_P = XVTK_CELL_TYPE_LIN;
   }
   else
   if ( strcmp(element->name_ef, "P2") == 0 )
   {
      GeomV = geom;             /* vtk can draw quadratic triangle */
      GeomP = geom->geomBase;   /* the pressure is P1 if the velocity is P2 */

      GeomX = Geom2D_getP1geom_from(element, geom);

      vtk_cell_type_V = XVTK_CELL_TYPE_QUA;
      vtk_cell_type_P = XVTK_CELL_TYPE_LIN;
   }
   else
   if ( strcmp(element->name_ef, "P3") == 0 )
   {
      GeomV = Geom2D_getP1geom_from(element, geom); /* consider as a refined P1 mesh from the P3 somms */
      GeomP = geom->geomBase; /* consider a P2 mesh */

      GeomX = GeomV;

      vtk_cell_type_V = XVTK_CELL_TYPE_LIN;
      vtk_cell_type_P = XVTK_CELL_TYPE_QUA;
   }
   else
   if ( strcmp(element->name_ef, "P1b") == 0 )
   {
      GeomV = geom->geomBase; /* consider only values at triangle's corners( like "P1") */
      GeomP = geom->geomBase; /* consider only values at triangle's corners( like "P1") */

      GeomX = geom;

      vtk_cell_type_V = XVTK_CELL_TYPE_LIN;
      vtk_cell_type_P = XVTK_CELL_TYPE_LIN;
   }
   else
   {
      error(E_UNKNOWN, "graphics2D_stokes_vtk");
   }

   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/

   graphics2D_vtk_1var(u_vtk_file, GeomV, u, vtk_cell_type_V, XVTK_PLOT_SURFACE);
   graphics2D_vtk_1var(v_vtk_file, GeomV, v, vtk_cell_type_V, XVTK_PLOT_SURFACE);

   graphics2D_vtk_1var(p_vtk_file, GeomP, p, vtk_cell_type_P, XVTK_PLOT_PLANE  );


   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/

/*   graphics2D_vtk_2var(V_vtk_file, GeomV, u , v , vtk_cell_type_V, XVTK_PLOT_SURFACE);*/ /* ?? does not work ?? */
/*   graphics2D_vtk_2var(V_vtk_file, GeomP, u , v , vtk_cell_type_P, XVTK_PLOT_SURFACE);*/
     graphics2D_vtk_2var(V_vtk_file, GeomX, u , v , XVTK_CELL_TYPE_LIN, XVTK_PLOT_SURFACE);

   /* end */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics2D       (const char* format, const ELT_2D *element , const GEOM_2D *geom , const VEC *SOL , const char *filename)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics2D");
   if ( element   == ELT_2D_NULL   ) error(E_NULL, "graphics2D");
   if ( geom      == GEOM_2D_NULL  ) error(E_NULL, "graphics2D");
   if ( SOL       == VNULL         ) error(E_NULL, "graphics2D");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics2D");

   if ( strcmp("matlab", format) == 0 )
   {
      graphics2D_matlab(filename, element, geom, SOL);
      graphics2D_script_matlab(filename);
   }
   else
   if ( strcmp("gnuplot", format) == 0 )
   {
      graphics2D_gnuplot(filename, element, geom, SOL);
      graphics2D_script_gnuplot(filename);
   }
   else
   if ( strcmp("vtk", format) == 0 )
   {
      graphics2D_vtk(filename, element, geom, SOL);
      graphics2D_script_vtk(filename);
   }
   else
   if ( strcmp("silo", format) == 0 )
   {
      graphics2D_silo(filename, element, geom, SOL);
      graphics2D_script_silo(filename);
   }
   else
   {
      error(E_UNKNOWN, "graphics2D");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics2D_fun   (const char* format, const ELT_2D *element , const GEOM_2D *geom , const FUN_2D *Fun2D , const char *filename)
{
   int s;

   VEC *SOL ;

   /* check */
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics2D_fun");
   if ( element   == ELT_2D_NULL   ) error(E_NULL, "graphics2D_fun");
   if ( geom      == GEOM_2D_NULL  ) error(E_NULL, "graphics2D_fun");
   if ( Fun2D     == NULL          ) error(E_NULL, "graphics2D_fun");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics2D_fun");

   /* init */
   SOL = v_get(geom->NBSOMM);

   for ( s=0; s<geom->NBSOMM; s++)
   {
      SOL->ve[s] = Fun2D->eval(Fun2D, geom->XYSOMM->me[s][0],
                                      geom->XYSOMM->me[s][1]);
   }

   graphics2D(format, element,  geom, SOL, filename);

   V_FREE(SOL);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics2D_stokes(const char* format, const ELT_2D *element , const GEOM_2D *geom , const VEC *u, const VEC *v, const VEC *p, const char *filename)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics2D_stokes");
   if ( element   == ELT_2D_NULL   ) error(E_NULL, "graphics2D_stokes");
   if ( geom      == GEOM_2D_NULL  ) error(E_NULL, "graphics2D_stokes");
   if ( u         == VNULL         ) error(E_NULL, "graphics2D_stokes");
   if ( v         == VNULL         ) error(E_NULL, "graphics2D_stokes");
   if ( p         == VNULL         ) error(E_NULL, "graphics2D_stokes");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics2D_stokes");

   if ( strcmp("vtk", format) == 0 )
   {
      printf("graphics2D_stokes...\n");
      graphics2D_stokes_vtk(filename, element, geom, u, v, p);
      graphics2D_stokes_script_vtk(filename);
   }
   else
   if ( strcmp("silo", format) == 0 )
   {
      graphics2D_stokes_silo(filename, element, geom, u, v, p);
      graphics2D_stokes_script_silo(filename, u,v,p);
   }
   else
   {
      error(E_UNKNOWN, "graphics2D_stokes");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics2D_stokes_fun(const char* format, const ELT_2D *element, const GEOM_2D *geom, const FUN_2D *Fu, const FUN_2D *Fv, const FUN_2D *Fp, const char *filename)
{
   int s;

   VEC *U ;
   VEC *V ;
   VEC *P ;

   /* check */
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics2D_stokes_fun");
   if ( element   == ELT_2D_NULL   ) error(E_NULL, "graphics2D_stokes_fun");
   if ( geom      == GEOM_2D_NULL  ) error(E_NULL, "graphics2D_stokes_fun");
   if ( Fu        == FUN_2D_NULL   ) error(E_NULL, "graphics2D_stokes_fun");
   if ( Fv        == FUN_2D_NULL   ) error(E_NULL, "graphics2D_stokes_fun");
   if ( Fp        == FUN_2D_NULL   ) error(E_NULL, "graphics2D_stokes_fun");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics2D_stokes_fun");

   /* init */
   U = v_get(geom->NBSOMM);
   V = v_get(geom->NBSOMM);
   P = v_get(geom->geomBase->NBSOMM);

   for ( s=0; s<geom->NBSOMM; s++)
   {
      U->ve[s] = Fu->eval(Fu, geom->XYSOMM->me[s][0], geom->XYSOMM->me[s][1]);

      V->ve[s] = Fv->eval(Fv, geom->XYSOMM->me[s][0], geom->XYSOMM->me[s][1]);
   }

   for ( s=0; s<geom->NBSOMM; s++)
   {
      P->ve[s] = Fp->eval(Fp, geom->geomBase->XYSOMM->me[s][0], geom->geomBase->XYSOMM->me[s][1]);
   }

   graphics2D_stokes(format, element, geom, U, V, P, filename);

   V_FREE(U);
   V_FREE(V);
   V_FREE(P);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics2D_set   (const char *format, const ELT_2D *element, const GEOM_2D *geom, int nb_plot, const MAT *SOL, const char *filename)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics2D_set");
   if ( element   == ELT_2D_NULL   ) error(E_NULL, "graphics2D_set");
   if ( geom      == GEOM_2D_NULL  ) error(E_NULL, "graphics2D_set");
   if ( SOL       == MNULL         ) error(E_NULL, "graphics2D_set");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics2D_set");


   if ( strcmp("gnuplot", format) == 0 )
   {
      graphics2D_gnuplot_set(filename, element, geom, nb_plot, SOL);
   }
   else
   if ( strcmp("silo", format) == 0 )
   {
      graphics2D_silo_set(filename, element, geom, nb_plot, SOL);
   }
   else
   {
      error(E_UNKNOWN, "graphics2D_set");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

//static void graphics2D_vtk_1var(const GEOM_2D *geom, const VEC* var, int triangle_type, const char *file_name , int plot_type)
//{
//   int s,e,k;
//   int NB_SOMM_CELL;
//
//   switch(triangle_type)
//   {
//      case XVTK_TRIANGLE_LIN: NB_SOMM_CELL = 3; break;
//      case XVTK_TRIANGLE_QUA: NB_SOMM_CELL = 6; break;
//      default:                NB_SOMM_CELL = 3;
//   }
//
//   /*------ ECRITURE DU FICHIER ---(vtk)----*/
//   /*                                       */
//   /* for componant u or v or p             */
//   /*                                       */
//   /*------ ECRITURE DU FICHIER ---(vtk)----*/
//
//   FILE *fp = fopen(file_name, "w");
//   /* File VTK for u or v */
//   fprintf(fp, "<?xml version=\"1.0\"?>\n");
//   fprintf(fp, "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\">\n");
//   fprintf(fp, "  <UnstructuredGrid>\n");
//   fprintf(fp, "    <Piece NumberOfPoints=\"%d\" NumberOfCells=\"%d\">\n", geom->NBSOMM, geom->NBELMT);
//
//   fprintf(fp, "      <PointData Scalars=\"scalars\">\n");
//   fprintf(fp, "        <DataArray type=\"Float32\" Name=\"scalars\" format=\"ascii\">\n");
//   for (s=0; s<geom->NBSOMM ; s++)
//   {
//      fprintf(fp, "        %lf \n", var->ve[s] );
//   }
//   fprintf(fp, "        </DataArray>\n");
//   fprintf(fp, "      </PointData>\n");
//
//   fprintf(fp, "      <Points>\n");
//   fprintf(fp, "        <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n");
//   for (s=0; s<geom->NBSOMM; s++)
//   {
//      fprintf(fp, "          %lf %lf %lf\n", geom->XYSOMM->me[s][0],
//                                            geom->XYSOMM->me[s][1],
//                                            plot_type == XVTK_PLOT_SURFACE ? var->ve[s] : 0.0);
//   }
//   fprintf(fp, "        </DataArray>\n");
//   fprintf(fp, "      </Points>\n");
//
//
//   fprintf(fp, "      <Cells>\n");
//   fprintf(fp, "        <DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">\n");
//   for (e=0; e<geom->NBELMT; e++)
//   {
//      fprintf(fp, "        "); /* nice format */
//      for (k=0; k<NB_SOMM_CELL; k++)
//      {
//         fprintf(fp, "%d ", geom->NSELMT->im[e][k]);
//      }
//      fprintf(fp, "\n"); /* finish the line */
//   }
//   fprintf(fp, "        </DataArray>\n");
//
//   fprintf(fp, "        <DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">\n");
//   for (k=0; k<geom->NBELMT; k++)
//   {
//      fprintf(fp, "          %d\n", (k+1)*NB_SOMM_CELL);
//   }
//   fprintf(fp, "        </DataArray>\n");
//
//   fprintf(fp, "        <DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">\n");
//   for (e=0; e<geom->NBELMT; e++)
//   {
//      fprintf(fp, "          %d\n", triangle_type); /* triangle type */
//   }
//   fprintf(fp, "        </DataArray>\n");
//   fprintf(fp, "      </Cells>\n");
//
//   fprintf(fp, "     </Piece>\n");
//   fprintf(fp, "  </UnstructuredGrid>\n");
//   fprintf(fp, "</VTKFile>\n");
//
//   fclose(fp);
//}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/


