
/*
 * Contains output functions for the geometry
 */


#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"


/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom2D_write_mesh    ( const GEOM_2D* Geom, const char *file_name );
static void graphics_geom2D_write_edge    ( const GEOM_2D* Geom, const char *file_name );
static void graphics_geom2D_view_X11      ( const GEOM_2D* Geom );
static void graphics_geom2D_view_gnuplot  ( const GEOM_2D* Geom, const char *file_name );
static void graphics_geom2D_view_graph    ( const GEOM_2D* Geom, const char *file_name );

static void graphics_geom3D_write_mesh    ( const GEOM_3D* Geom, const char *file_name );
static void graphics_geom3D_write_surf    ( const GEOM_3D* Geom, const char *file_name );
static void graphics_geom3D_view_X11      ( const GEOM_3D* Geom );
static void graphics_geom3D_view_gnuplot  ( const GEOM_3D* Geom, const char *file_name );



/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom2D_write_mesh( const GEOM_2D* Geom, const char *file_name )
{
   FILE *fp;
   int e;
   char data_file[64];

   double x0, y0;
   double x1, y1;
   double x2, y2;

   snprintf(data_file, 64, "%s_mesh.dat" , file_name);
   data_file[63] = '\0';

   fp = fopen(data_file, "w");

   for (e=0; e<Geom->NBELMT; e++)
   {
      x0 = Geom->XYSOMM->me[Geom->NSELMT->im[e][0]][0];
      y0 = Geom->XYSOMM->me[Geom->NSELMT->im[e][0]][1];

      x1 = Geom->XYSOMM->me[Geom->NSELMT->im[e][1]][0];
      y1 = Geom->XYSOMM->me[Geom->NSELMT->im[e][1]][1];

      x2 = Geom->XYSOMM->me[Geom->NSELMT->im[e][2]][0];
      y2 = Geom->XYSOMM->me[Geom->NSELMT->im[e][2]][1];

      fprintf(fp, "## triangle %d ( nodes %d %d %d )\n", e,
              Geom->NSELMT->im[e][0],
              Geom->NSELMT->im[e][1],
              Geom->NSELMT->im[e][2]);
      fprintf(fp, "#m=1,S=0\n");

      fprintf(fp, "%lf %lf \n",x0,y0);
      fprintf(fp, "%lf %lf \n",x1,y1);
      fprintf(fp, "%lf %lf \n",x2,y2);
      fprintf(fp, "%lf %lf \n",x0,y0);
      fprintf(fp, "\n\n"); /* for the "next" data set */
   }

   fclose(fp);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom2D_write_edge   ( const GEOM_2D* Geom, const char *file_name )
{
   FILE *fp;
   int a;
   char data_file[64];

   double x0, y0;
   double x1, y1;

   snprintf(data_file, 64, "%s_edge.dat" , file_name);
   data_file[63] = '\0';

   fp = fopen(data_file, "w");

   for (a=0; a<Geom->NBFACE; a++)
   {
      x0 = Geom->XYSOMM->me[Geom->NSFACE->im[a][0]][0];
      y0 = Geom->XYSOMM->me[Geom->NSFACE->im[a][0]][1];

      x1 = Geom->XYSOMM->me[Geom->NSFACE->im[a][1]][0];
      y1 = Geom->XYSOMM->me[Geom->NSFACE->im[a][1]][1];

      fprintf(fp, "## edge %d ( nodes %d %d )\n", a,
              Geom->NSFACE->im[a][0],
              Geom->NSFACE->im[a][1]);
      fprintf(fp, "#m=1,S=0\n");

      fprintf(fp, "%lf %lf \n",x0,y0);
      fprintf(fp, "%lf %lf \n",x1,y1);
      fprintf(fp, "\n\n"); /* for the "next" data set */
   }

   fclose(fp);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom3D_write_mesh( const GEOM_3D* Geom, const char *file_name )
{
   FILE *fp;
   int e;
   char data_file[64];

   double x0, y0, z0;
   double x1, y1, z1;
   double x2, y2, z2;
   double x3, y3, z3;

   snprintf(data_file, 64, "%s_mesh.dat" , file_name);
   data_file[63] = '\0';

   fp = fopen(data_file, "w");

   for (e=0; e<Geom->NBELMT; e++)
   {
      x0 = Geom->XYSOMM->me[Geom->NSELMT->im[e][0]][0];
      y0 = Geom->XYSOMM->me[Geom->NSELMT->im[e][0]][1];
      z0 = Geom->XYSOMM->me[Geom->NSELMT->im[e][0]][2];

      x1 = Geom->XYSOMM->me[Geom->NSELMT->im[e][1]][0];
      y1 = Geom->XYSOMM->me[Geom->NSELMT->im[e][1]][1];
      z1 = Geom->XYSOMM->me[Geom->NSELMT->im[e][1]][2];

      x2 = Geom->XYSOMM->me[Geom->NSELMT->im[e][2]][0];
      y2 = Geom->XYSOMM->me[Geom->NSELMT->im[e][2]][1];
      z2 = Geom->XYSOMM->me[Geom->NSELMT->im[e][2]][2];

      x3 = Geom->XYSOMM->me[Geom->NSELMT->im[e][3]][0];
      y3 = Geom->XYSOMM->me[Geom->NSELMT->im[e][3]][1];
      z3 = Geom->XYSOMM->me[Geom->NSELMT->im[e][3]][2];

      fprintf(fp, "## tetrahedra %d (nodes %d %d %d %d)\n", e,
              Geom->NSELMT->im[e][0],
              Geom->NSELMT->im[e][1],
              Geom->NSELMT->im[e][2],
              Geom->NSELMT->im[e][3]);

      fprintf(fp, "%lf %lf %lf \n", x0,y0,z0);
      fprintf(fp, "%lf %lf %lf \n", x1,y1,z1);
      fprintf(fp, "%lf %lf %lf \n", x2,y2,z2);
      fprintf(fp, "%lf %lf %lf \n", x3,y3,z3);
      fprintf(fp, "%lf %lf %lf \n", x0,y0,z0);
      fprintf(fp, "%lf %lf %lf \n", x2,y2,z2);
      fprintf(fp, "%lf %lf %lf \n", x1,y1,z1);
      fprintf(fp, "%lf %lf %lf \n", x3,y3,z3);
      fprintf(fp, "\n\n"); /* for the "next" data set */
   }

   fclose(fp);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom3D_write_surf( const GEOM_3D *Geom, const char *file_name )
{
   FILE *fp;
   int a;
   char data_file[64];

   double x0, y0, z0;
   double x1, y1, z1;
   double x2, y2, z2;

   snprintf(data_file, 64, "%s_surf.dat" , file_name);
   data_file[63] = '\0';

   fp = fopen(data_file, "w");

   for (a=0; a<Geom->NBFACE; a++)
   {
      x0 = Geom->XYSOMM->me[Geom->NSFACE->im[a][0]][0];
      y0 = Geom->XYSOMM->me[Geom->NSFACE->im[a][0]][1];
      z0 = Geom->XYSOMM->me[Geom->NSFACE->im[a][0]][2];

      x1 = Geom->XYSOMM->me[Geom->NSFACE->im[a][1]][0];
      y1 = Geom->XYSOMM->me[Geom->NSFACE->im[a][1]][1];
      z1 = Geom->XYSOMM->me[Geom->NSFACE->im[a][1]][2];

      x2 = Geom->XYSOMM->me[Geom->NSFACE->im[a][2]][0];
      y2 = Geom->XYSOMM->me[Geom->NSFACE->im[a][2]][1];
      z2 = Geom->XYSOMM->me[Geom->NSFACE->im[a][2]][2];

      fprintf(fp, "## triangle on face %d (nodes %d %d %d)\n", a,
              Geom->NSFACE->im[a][0],
              Geom->NSFACE->im[a][1],
              Geom->NSFACE->im[a][2]);

      fprintf(fp, "%lf %lf %lf \n",x0,y0,z0);
      fprintf(fp, "%lf %lf %lf \n",x1,y1,z1);
      fprintf(fp, "%lf %lf %lf \n",x2,y2,z2);
      fprintf(fp, "%lf %lf %lf \n",x0,y0,z0);
      fprintf(fp, "\n\n"); /* for the "next" data set */
   }

   fclose(fp);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom2D_view_X11( const GEOM_2D* Geom ) /* with "graph" */
{
   graphics_geom2D_write_mesh( Geom, "geom2D_dummy" );
   graphics_geom2D_write_edge( Geom, "geom2D_dummy" );

   /*system("graph -T X -C   < geom2D_dummy_edge.dat");*/
   system("graph -T X -C  < geom2D_dummy_mesh.dat");

   //system("rm -f geom2D_dummy_edge.dat");
   //system("rm -f geom2D_dummy_mesh.dat");
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom2D_view_graph( const GEOM_2D* Geom, const char *file_name )
{
   /* write only the files of the solutions */
   graphics_geom2D_write_mesh( Geom, file_name );
   graphics_geom2D_write_edge( Geom, file_name );
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom2D_view_gnuplot( const GEOM_2D* Geom, const char *file_name )
{
   FILE *fp;
   char  file_gnuplot[64];

   /*------ ECRITURE D' UN FICHIER "file_name.gnu" --------*/
   snprintf(file_gnuplot, 64, "%s.gnu" , file_name);
   file_gnuplot[63] = '\0';

   fp = fopen(file_gnuplot, "w");

   fprintf(fp, "# ----------------------------------------------------------- \n");
   fprintf(fp, "# Ceci est un programme GNUPLOT nomme <file_name>.gnu'        \n");
   fprintf(fp, "# ----------------------------------------------------------- \n");
   fprintf(fp, "# \n");

   fprintf(fp, "set title \"MESH (%s)\" \n", file_gnuplot);
   fprintf(fp, "set nokey \n");
   fprintf(fp, "#set size square \n");
   fprintf(fp, "set size ratio -1 \n");

   fprintf(fp, "#  \n");
   fprintf(fp, "plot '%s_mesh.dat' with lines\n", file_name);
   fprintf(fp, "#  \n");
   fprintf(fp, "pause 0 \"...done\" \n");
   fprintf(fp, "pause -1 \n");


   fprintf(fp, "set title \"MESH - EDGES (%s)\" \n", file_gnuplot);
   fprintf(fp, "set nokey \n");
   fprintf(fp, "#set size square \n");
   fprintf(fp, "set size ratio -1 \n");

   fprintf(fp, "#  \n");
   fprintf(fp, "plot '%s_edge.dat' with lines\n", file_name);
   fprintf(fp, "#  \n");
   fprintf(fp, "pause 0 \"...done\" \n");
   fprintf(fp, "pause -1 \n");

   fclose(fp);


   /* and write the files of the solutions */
   graphics_geom2D_write_mesh( Geom, file_name );
   graphics_geom2D_write_edge( Geom, file_name );

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom3D_view_X11( const GEOM_3D* Geom ) /* with "gnuplot" */
{
   graphics_geom3D_view_gnuplot( Geom, "geom3D_dummy" );

   system("gnuplot geom3D_dummy.gnu");

   system("rm -f geom3D_dummy_mesh.dat");
   system("rm -f geom3D_dummy_surf.dat");
   system("rm -f geom3D_dummy.gnu");
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_geom3D_view_gnuplot( const GEOM_3D* Geom, const char *file_name )
{
   FILE *fp;
   char  file_gnuplot[64];

   /*------ ECRITURE D' UN FICHIER "file_name.gnu" --------*/
   snprintf(file_gnuplot, 64, "%s.gnu" , file_name);
   file_gnuplot[63] = '\0';

   fp = fopen(file_gnuplot, "w");

   fprintf(fp, "# ----------------------------------------------------------- \n");
   fprintf(fp, "# Ceci est un programme GNUPLOT nomme <file_name>.gnu'        \n");
   fprintf(fp, "# ----------------------------------------------------------- \n");
   fprintf(fp, "# \n");

   fprintf(fp, "set title \"MESH (%s)\" \n", file_gnuplot);
   fprintf(fp, "set nokey \n");
   fprintf(fp, "#set size square \n");
   fprintf(fp, "set size ratio -1 \n");

   fprintf(fp, "#  \n");
   fprintf(fp, "splot '%s_surf.dat' with lines\n", file_name);
   fprintf(fp, "#  \n");
   fprintf(fp, "pause 0 \"...done\" \n");
   fprintf(fp, "pause -1 \n");

   fprintf(fp, "set title \"MESH (%s)\" \n", file_gnuplot);
   fprintf(fp, "set nokey \n");
   fprintf(fp, "#set size square \n");
   fprintf(fp, "set size ratio -1 \n");

   fprintf(fp, "#  \n");
   fprintf(fp, "splot '%s_mesh.dat' with lines\n", file_name);
   fprintf(fp, "#  \n");
   fprintf(fp, "pause 0 \"...done\" \n");
   fprintf(fp, "pause -1 \n");

   fclose(fp);

   /* and write the files of the solutions */
   graphics_geom3D_write_mesh( Geom, file_name );
   graphics_geom3D_write_surf( Geom, file_name );

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics_geom1D_view( const char *format, const GEOM_1D * Geom, const char *name_file)
{
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics_geom2D_view( const char *format, const GEOM_2D * Geom, const char *name_file)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics_geom2D_view");
   if ( name_file == (char*)NULL   ) error(E_NULL, "graphics_geom2D_view");

   if ( Geom      == GEOM_2D_NULL  )
   {
      warning(WARN_UNKNOWN, "graphics_geom2D_view");
      return ;
   }

   if ( strcmp("X11", format) == 0 )
   {
      graphics_geom2D_view_X11(Geom);
   }
   else
   if ( strcmp("gnuplot", format) == 0 )
   {
      graphics_geom2D_view_gnuplot(Geom, name_file);
   }
   else
   if ( strcmp("graph", format) == 0 )
   {
      graphics_geom2D_view_graph(Geom, name_file);
   }
   else
   {
      error(E_UNKNOWN, "graphics_geom2D_view");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics_geom3D_view( const char *format, const GEOM_3D * Geom, const char *name_file)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics_geom3D_view");
   if ( name_file == (char*)NULL   ) error(E_NULL, "graphics_geom3D_view");

   if ( Geom      == GEOM_3D_NULL  )
   {
      warning(WARN_UNKNOWN, "graphics_geom3D_view");
      return ;
   }


   if ( strcmp("X11",format) == 0 )
   {
      graphics_geom3D_view_X11(Geom);
   }
   else
   if ( strcmp("gnuplot",format) == 0 )
   {
      graphics_geom3D_view_gnuplot(Geom, name_file);
   }
   /*
   else
   if ( strcmp("graph",format) == 0 )
   {
      graphics_geom3D_view_graph(Geom, name_file);
   }
   */
   else
   {
      error(E_UNKNOWN, "graphics_geom3D_view");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

