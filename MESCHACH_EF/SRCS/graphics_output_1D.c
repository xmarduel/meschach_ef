
/** \file graphics_output_1D.c
 *
 * Contains all the output functions
 *
 */

//#include "meschach_config.h"

#ifdef HAVE_SILO
#include "silo.h"
#endif

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_1D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_1D.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot   (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL);
static void graphics1D_graph     (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL);
static void graphics1D_matlab    (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL);
static void graphics1D_matplotlib(const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL);
static void graphics1D_vtk       (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL);
static void graphics1D_silo      (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL);

static char * graphics1D_silo_script   (const char* filename, const VEC *SOL);
static char * graphics1D_vtk_script    (const char* filename, const VEC *SOL);
static char * graphics1D_gnuplot_script(const char* filename, const VEC *SOL);

static void graphics1D_gnuplot_write_script             (const char *filename, const char* solApprox, const char* solExacte, const char* solExDiscrete, int num_iter, const char* residus_file);
static void graphics1D_gnuplot_write_script_residus     (FILE *fp, int num_iter, const char* residus_file);
static void graphics1D_gnuplot_write_script_BeginSolPlot(FILE *fp, Real xmin, Real xmax);
static void graphics1D_gnuplot_write_script_EndSolPlot  (FILE *fp);
static void graphics1D_gnuplot_write_script_SolPlot     (FILE *fp, const char* SolFile, const char* SolLegend, const char* style);
static void graphics1D_gnuplot_write_script_ContPlot    (FILE *fp);

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics1D_gnuplot_script(const char*filename, const VEC *SOL)
{
   /* Script GNUPLOT */
   char *script_gnuplot = (char*)malloc(64);
   FILE *fp;

   snprintf(script_gnuplot, 64, "Script4gnuplot_%s.gpp", filename); script_gnuplot[63] = '\0';

   graphics1D_gnuplot_write_script(script_gnuplot, "SolApproch1D.dat", NULL, NULL, 100, "the_residuals.dat"); 

   return script_gnuplot;
}

static void graphics1D_gnuplot_write_script(const char* filename, const char* solApprox, const char* solExacte, const char* solExDiscrete, int num_iter, const char* residus_file)
{
   FILE *fp = fopen(filename, "w");

   fprintf(fp, "#! /usr/bin/env gnuplot\n");
   fprintf(fp, "#\n");
   fprintf(fp, "#\n");

   if (residus_file != NULL)
   {
      graphics1D_gnuplot_write_script_residus(fp, num_iter, residus_file) ;
   }
   
   graphics1D_gnuplot_write_script_BeginSolPlot(fp, 0.0, 1.0);

   if (solApprox != NULL)
   {
      graphics1D_gnuplot_write_script_SolPlot(fp, solApprox, "SolApprochee", "lines");
   }
   if (solExacte != NULL)
   {
      graphics1D_gnuplot_write_script_ContPlot(fp);     
      graphics1D_gnuplot_write_script_SolPlot(fp, solExacte, "SolExacte" , "lines");
   }
   if (solExDiscrete != NULL)
   {
      graphics1D_gnuplot_write_script_ContPlot(fp);
      graphics1D_gnuplot_write_script_SolPlot(fp, solExDiscrete, "SolExacteDiscrete" , "lines");
   }
   graphics1D_gnuplot_write_script_EndSolPlot(fp);
   
   fclose(fp);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot_write_script_residus(FILE *fp, int num_iter, const char* residus_file)
{
   if (num_iter>0)
   {
      fprintf(fp, "set logscale y\n");
      fprintf(fp, "plot [00:%d] [1.0e-15:10.0] %c\n",(int)(num_iter*10.0/8), '\\');
      fprintf(fp, " '%s' title 'Residu (iter)' with lines\n", residus_file);
      fprintf(fp, "pause -1 \n\n");
   }
   else
   {
      fprintf(fp, "set logscale y\n");
      fprintf(fp, "plot [1.0e-15:10.0]  %c\n", '\\');
      fprintf(fp, " '%s' title 'Residu (iter)' with lines\n", residus_file);
      fprintf(fp, "pause -1 \n\n");
   }
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot_write_script_BeginSolPlot(FILE *fp, double xmin, double xmax)
{
   fprintf(fp, "set nologscale y\n");
   fprintf(fp, "plot [%f:%f] %c", xmin, xmax, '\\');
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot_write_script_ContPlot(FILE *fp)
{
   fprintf(fp, "  , %c", '\\');  /* separation with the preceeding plot */
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot_write_script_EndSolPlot(FILE *fp)
{
   fprintf(fp, "\npause -1\n\n"); 
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot_write_script_SolPlot(FILE *fp, const char* SolFile, const char* SolLegend, const char* style)
{
   fprintf(fp, "\n '%s' title '%s' with %s", SolFile, SolLegend, style);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics1D_output_fun(const char *filename, const ELT_1D *element, const GEOM_1D *geom, const FUN_1D *f)
{
   int  i;
   FILE *fp;

   if ( element == NULL ) error(E_NULL, "graphics1D_output_fun");
   if ( geom    == NULL ) error(E_NULL, "graphics1D_output_fun");
   if ( f       == NULL ) error(E_NULL, "graphics1D_output_fun");

   if ( filename == NULL )
   {
      fp = stdout;
   }
   else
   {
      fp = fopen(filename,"w");
   }


   if ( strcmp(element->name_ef,"P1") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], f->eval(f, geom->XSOMM->ve[i]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"P2") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], f->eval(f, geom->XSOMM->ve[i]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"P3") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i],  f->eval(f, geom->XSOMM->ve[i]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim/2; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[2*i],  f->eval(f, geom->XSOMM->ve[2*i]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"S2") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim-2; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i+1],  f->eval(f, geom->XSOMM->ve[i+1]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"S3") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim-2; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i+1],  f->eval(f, geom->XSOMM->ve[i+1]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"S4") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim-2; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i+1],  f->eval(f, geom->XSOMM->ve[i+1]) );
      }
   }
   else
   if ( strcmp(element->name_ef,"S5") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim-4; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i+2], f->eval(f, geom->XSOMM->ve[i+2]) );
      }
   }
   else
   {
      error(E_EF_NOTIMPL, "graphics1D_output_fun");
   }

   /* clean */
   if ( filename != NULL )
   {
      fclose(fp);
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics1D_output_vec(const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL)
{
   int  i;
   FILE *fp;
   VEC  *TMP;
   
   if ( element == NULL ) error(E_NULL, "graphics1D_output_vec");
   if ( geom    == NULL ) error(E_NULL, "graphics1D_output_vec");
   if ( SOL     == NULL ) error(E_NULL, "graphics1D_output_vec");
   
   if ( geom->EF_to_WORLD->n != SOL->dim ) error(E_SIZES, "graphics1D_output_vec");

   TMP = sp_mv_mlt(geom->EF_to_WORLD, (VEC*)SOL, NULL);
   
   if ( filename == NULL )
   {
      fp = stdout;
   }
   else
   {
      fp = fopen(filename, "w");
   }
   
   
   if ( strcmp(element->name_ef,"P1") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"P2") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"P3") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"H3") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim/2; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[2*i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"S2") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"S3") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"S4") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   if ( strcmp(element->name_ef,"S5") == 0 )
   {
      for (i=0; i<geom->XSOMM->dim; i++)
      {
         fprintf(fp, "%f  %f \n", geom->XSOMM->ve[i], TMP->ve[i] );
      }
   }
   else
   {
      error(E_EF_NOTIMPL, "graphics1D_output_vec");
   }

   /* clean */
   V_FREE(TMP);

   if ( filename != NULL )
   {
      fclose(fp);
   }
   
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_gnuplot(const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC  *SOL)
{
   char     gnuplot_file[64];

   snprintf(gnuplot_file, 64, "%s.dat", filename);
   gnuplot_file[63] = '\0';

   graphics1D_output_vec(gnuplot_file, element, geom, SOL);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_graph  (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC  *SOL)
{
   graphics1D_output_vec("graphics1D_graph_dummy.dat", element, geom, SOL);

   system("graph -T X -C -m 1  < graphics1D_graph_dummy.dat");

   system("rm graphics1D_graph_dummy.dat");
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_matplotlib(const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC  *SOL)
{
   warning(WARN_UNKNOWN, "graphics1D_matplotlib not implemented");
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics1D_silo_script (const char* filename, const VEC  *SOL)
{
   /* Script MESHTVX */
   char *script_meshtv = (char*)malloc(64);
   FILE *fp;

   snprintf(script_meshtv, 64, "Script4meshtv_%s.sil" , filename);
   script_meshtv[63] = '\0';

   fp = fopen(script_meshtv,"w");

   fprintf(fp, "#! /usr/bin/env meshtvx\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated  - changes will not be kept - \n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "#\n");
   fprintf(fp, "#  host[] meshtvx -s <filename> \n");
   fprintf(fp, "# \n");
   fprintf(fp, "open \"./%s.silo\";\n", filename);
   fprintf(fp, "surf surfcol=z,legend=on,var=\"vitesse\";\n");
   fprintf(fp, "plot surf");

/*
import visit
#
visit.OpenDatabase("SolApproch1D.silo")
visit.AddPlot("Curve", "curve")
visit.DrawPlots()
visit.quit()
*/

   fclose(fp);

   return script_meshtv;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_silo   (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC  *SOL)
{
#ifdef HAVE_SILO
   DBfile   *db = NULL;
   int      DB_REAL    = sizeof(Real)==sizeof(float) ? DB_FLOAT : DB_DOUBLE ;
#endif

   char     silo_file[64];

   
   GEOM_1D *GeomP1 = Geom1D_getP1geom_from(element, geom);
   VEC *SOL_P1     = build_vec_world_from_vec_ef_1D( element, geom, SOL );
   
   snprintf(silo_file, 64, "%s.silo", filename); silo_file[63] = '\0';

   /*--- ECRITURE DU FICHIER "Solution_Approc2.silo"---(MESH-TV)---- */

#ifdef HAVE_SILO

   /* Create the SILO file */

   db = DBCreate(silo_file, DB_CLOBBER, DB_LOCAL, "UCD mesh test", DB_PDB);
   
   /* Write out the mesh to the file ---------- DBPutCurve */

   DBPutCurve(db, "curve", GeomP1->XSOMM->ve, SOL_P1->ve, DB_REAL, SOL_P1->dim, NULL);

   /* Close the silo file */

   DBClose(db);

#endif

   /* free mem */
   GEOM_1D_FREE(GeomP1);
   V_FREE(SOL_P1);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_matlab (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL)
{
   error(E_UNKNOWN, "graphics1D_matlab");
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics1D_vtk    (const char *filename, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL)
{
   char vtk_file[64];
   FILE *fp;

   int  s;

   GEOM_1D *GeomP1 ;
   VEC *SOL_P1     ;

   /* check */
   if (SOL     == NULL )  error(E_NULL, "graphics1D_vtk");
   if (element == NULL )  error(E_NULL, "graphics1D_vtk");
   if (geom    == NULL )  error(E_NULL, "graphics1D_vtk");

   if ( SOL->dim != geom->NBSOMM ) error(E_SIZES, "graphics1D_vtk");

   /* start ... */
   snprintf(vtk_file, 64, "%s.vtk" , filename); vtk_file[63] = '\0';

   GeomP1 = Geom1D_getP1geom_from(element, geom);
   SOL_P1 = build_vec_world_from_vec_ef_1D( element, geom, SOL );

   /*------ ECRITURE DU FICHIER "Solution_Approc.vtk"---(vtk)----*/

   fp = fopen(vtk_file, "w");
   if (fp==NULL) error(E_NULL, "graphics1D_vtk");
   
   /* File VTK */
   fprintf(fp, "# vtk DataFile Version 5.0 \n");
   fprintf(fp, "Solution PDE problem \n");
   fprintf(fp, "\n");
   fprintf(fp, "ASCII \n");
   fprintf(fp, "\n");
   fprintf(fp, "DATASET POLYDATA \n");
   fprintf(fp, "POINTS %d float \n", GeomP1->NBSOMM);
   for (s=0; s<GeomP1->NBSOMM ; s++)
   {
      fprintf(fp, "%lf %lf %lf \n",  GeomP1->XSOMM->ve[s], 0.0, 0.0);
   }
   
   fprintf(fp, "LINES %d %d \n", GeomP1->NBSOMM-1 , 3*(GeomP1->NBSOMM-1));
   for (s=0; s<GeomP1->NBSOMM-1 ; s++)
   {
      fprintf(fp, "%d %d %d\n", 2, s, s+1);
   }

   fprintf(fp, "POINT_DATA %d \n", GeomP1->NBSOMM);
   fprintf(fp, "SCALARS scalars float\n");
   fprintf(fp, "LOOKUP_TABLE default\n");
   for (s=0; s<GeomP1->NBSOMM ; s++)
   {
      fprintf(fp, "%lf \n",  SOL_P1->ve[s]);
   }

   fclose(fp);

   /* end */
   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static char * graphics1D_vtk_script(const char* filename, const VEC  *SOL)
{
   FILE *fp;
   char *script_vtk = (char*)malloc(64);

   snprintf(script_vtk, 64, "Script4vtk_%s.py", filename); script_vtk[63] = '\0';
   fp = fopen(script_vtk, "w");

   /* Script python */
   fprintf(fp, "#!/usr/bin/env pythonw\n");
   fprintf(fp, "#\n");
   fprintf(fp, "# File automatically generated - vtk version : 4.2\n");
   fprintf(fp, "# date : %s", get_date() );
   fprintf(fp, "\n");
   fprintf(fp, "import vtk \n");
   fprintf(fp, "\n");
   fprintf(fp, "\n");
   fprintf(fp, "redColor   = (1.0, 0.0, 0.0) \n");
   fprintf(fp, "greenColor = (0.0, 1.0, 0.0) \n");
   fprintf(fp, "blueColor  = (0.0, 0.0, 1.0) \n");
   fprintf(fp, "blackColor = (0.0, 0.0, 0.0) \n");
   fprintf(fp, "\n");
   fprintf(fp, "file_vtk = vtk.vtkPolyDataReader() \n");
   fprintf(fp, "file_vtk.SetFileName(\"./%s.vtk\") \n", filename);
   fprintf(fp, "file_vtk.Update() \n");
   fprintf(fp, "\n");
   fprintf(fp, "xyplot = vtk.vtkXYPlotActor() \n");
   fprintf(fp, "xyplot.AddDataSetInput( file_vtk.GetOutput() ) \n");
   fprintf(fp, "xyplot.GetPositionCoordinate().SetValue(0.1, 0.1, 0.0)  \n"); /* the viewport of the graph */
   fprintf(fp, "xyplot.GetPosition2Coordinate().SetValue(0.8, 0.7, 0.0)  \n"); /* the viewport of the graph */
   fprintf(fp, "xyplot.SetXValuesToValue() \n");
   fprintf(fp, "#xyplot.LogxOn() \n");
   fprintf(fp, "#xyplot.LogyOn() \n");
   fprintf(fp, "#xyplot.SetXRange( 0.0, 1.3) \n");
   fprintf(fp, "#xyplot.SetYRange(-3.0, 2.0) \n");
   fprintf(fp, "xyplot.SetNumberOfXLabels(6) \n");
   fprintf(fp, "xyplot.SetNumberOfYLabels(6) \n");
   fprintf(fp, "#xyplot.SetTitle(\"Pressure\") \n");
   fprintf(fp, "xyplot.SetXTitle(\"X\") \n");
   fprintf(fp, "xyplot.SetYTitle(\"Y\") \n");
   fprintf(fp, "\n");
   fprintf(fp, "xyplot.GetProperty().SetColor( blackColor ) \n");
   fprintf(fp, "xyplot.GetProperty().SetLineWidth( 1 ) \n");
   fprintf(fp, "\n");
   fprintf(fp, "xyplot.SetPlotColor( 0 , redColor) \n");
   fprintf(fp, "#xyplot.SetPlotColor( 1 , greenColor) \n");
   fprintf(fp, " \n");
   fprintf(fp, "#XAxis = xyplot.GetXAxisActor2D() \n");
   fprintf(fp, "#YAxis = xyplot.GetYAxisActor2D() \n");
   fprintf(fp, "\n");
   fprintf(fp, "# Create graphics stuff \n");
   fprintf(fp, "\n");
   fprintf(fp, "ren1 = vtk.vtkRenderer() \n");
   fprintf(fp, "ren1.SetBackground(1, 1, 1) \n");
   fprintf(fp, "ren1.SetViewport(0.0, 0.0, 1.0, 1.0) \n");
   fprintf(fp, "ren1.AddActor2D( xyplot ) \n");
   fprintf(fp, "\n");
   fprintf(fp, "renWin = vtk.vtkRenderWindow() \n");
   fprintf(fp, "renWin.SetSize( 500, 500) \n");
   fprintf(fp, "renWin.AddRenderer( ren1 ) \n");
   fprintf(fp, "\n");
   fprintf(fp, "iren = vtk.vtkRenderWindowInteractor()\n");
   fprintf(fp, "iren.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera())\n");
   fprintf(fp, "iren.SetRenderWindow(renWin)\n");
   fprintf(fp, "iren.Initialize()\n");
   fprintf(fp, "iren.Start()\n");
   fprintf(fp, "\n");

   fclose(fp);

   return script_vtk; 
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics1D       (const char* format, const ELT_1D *element, const GEOM_1D *geom, const VEC *SOL, const char *filename)
{
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics1D");
   if ( element   == ELT_1D_NULL   ) error(E_NULL, "graphics1D");
   if ( geom      == GEOM_1D_NULL  ) error(E_NULL, "graphics1D");
   if ( SOL       == VNULL         ) error(E_NULL, "graphics1D");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics1D");


   if ( strcmp("matlab", format) == 0 )
   {
      graphics1D_matlab(filename, element, geom, SOL);
   }
   else
   if ( strcmp("gnuplot", format) == 0 )
   {
      graphics1D_gnuplot(filename, element, geom, SOL);
      graphics1D_gnuplot_script(filename, SOL);
   }
   else
   if ( strcmp("matplotlib", format) == 0 )
   {
      graphics1D_matplotlib(filename, element, geom, SOL);
   }
   else
   if ( strcmp("graph", format) == 0 )
   {
      graphics1D_graph(filename, element, geom, SOL);
   }
   else
   if ( strcmp("vtk", format) == 0 )
   {
      graphics1D_vtk(filename, element, geom, SOL);
      graphics1D_vtk_script(filename, SOL);
   }
   else
   if ( strcmp("silo", format) == 0 )
   {
      graphics1D_silo(filename, element, geom, SOL);
      graphics1D_silo_script(filename, SOL);
   }
   else
   {
      error(E_UNKNOWN, "graphics1D");
   }

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

void graphics1D_fun   (const char* format, const ELT_1D *element , const GEOM_1D *Geom , const FUN_1D *Fun1D , const char *filename)
{
   int s;

   VEC *SOL ;

   /* check */
   if ( format    == (char*)NULL   ) error(E_NULL, "graphics1D_fun");
   if ( element   == ELT_1D_NULL   ) error(E_NULL, "graphics1D_fun");
   if ( Geom      == GEOM_1D_NULL  ) error(E_NULL, "graphics1D_fun");
   if ( Fun1D     == NULL          ) error(E_NULL, "graphics1D_fun");
   if ( filename == (char*)NULL    ) error(E_NULL, "graphics1D_fun");

   /* init */
   SOL = v_get(Geom->NBSOMM);

   for ( s=0; s<Geom->NBSOMM; s++)
   {
      SOL->ve[s] = Fun1D->eval(Fun1D, Geom->XSOMM->ve[s]);
   }

   graphics1D(format, element,  Geom, SOL, filename);

   V_FREE(SOL);

   return;
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

