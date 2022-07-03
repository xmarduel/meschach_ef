
/** \file graphics_matrix_patterns.c
 *
 * Contains all the output functions
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"

#include <sys/types.h>
#include <sys/stat.h>

const char *suffix = "mtx_prof";

/*---------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_outputfile( const SPMAT* A, const char *file_name );

static void graphics_matrix_pattern_X11       ( const SPMAT* A, const char* file_name );
static void graphics_matrix_pattern_graph     ( const SPMAT* A, const char *file_name );
static void graphics_matrix_pattern_gnuplot   ( const SPMAT* A, const char *file_name );

static void graphics_matrix_pattern_script_graph   ( const SPMAT* A, const char *file_name );
static void graphics_matrix_pattern_script_gnuplot ( const SPMAT* A, const char *file_name );

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_outputfile( const SPMAT* A, const char *file_name )
{
   FILE *fp;
   char file_data[64];
   int i,idx;
   SPROW *r;

   snprintf(file_data, 64, "%s_%s.dat" , file_name, suffix);
   file_data[63] = '\0';

   fp = fopen(file_data, "w");

   for (i=0; i<A->m; i++)
   {
      r = A->row + i;

      for (idx=0; idx<r->len; idx++ )
      {
         //fprintf(fp, "#m=1,S=0\n");
         fprintf(fp, "%d  -%d\n", r->elt[idx].col, i);
      }
      fprintf(fp, "\n");
   }

   fclose(fp);
}

/*----------------------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_X11( const SPMAT* A, const char* file_name)
{
   char command[128];

   /* a string to be executed by the shell */
   snprintf(command, 128, "graph -T X --top-label \"%s\" "
            "-x 0 %d "
            "-y %d 0 "
            " -C -m -1 -S 2 0.02 "
            " < graphxx_%s.dat",
            file_name,
            A->n,
            -A->m,
            suffix);

   command[127] = '\0';

   /* write the gnuplot script to read and plot the file */
   graphics_matrix_pattern_outputfile( A, "graphxx" );

   /* execute the command in the shell */
   system(command);

   /* remove the file of the solution */
   //system("rm -f graphxx_mtxprof.dat");
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_script_graph     ( const SPMAT *A, const char *file_name )
{
   FILE *fp;
   char file_graph[64];

   snprintf(file_graph, 64, "%s_%s.graph" , file_name, suffix);
   file_graph[63] = '\0';

   fp = fopen(file_graph, "w");

   fprintf(fp, "#!/bin/sh \n");
   fprintf(fp, "# \n");
   fprintf(fp, "graph -T X --top-label \"%s\" --x-label \"(%dx%d)\" -x 0 %d -y %d 0 -C -m -1 -S 2 0.02 < %s_%s.dat",
           "MATRIX PROFILE", A->m, A->n, A->n, -A->m, file_name, suffix);

   /* and close it */
   fclose(fp);

   /* make the file executable */
#ifdef HAVE_POSIX
   chmod(file_graph, S_IRWXU | S_IRGRP | S_IROTH );
#endif
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_graph( const SPMAT* A, const char *file_name )
{
   /* write the file of the solution */
   graphics_matrix_pattern_outputfile( A, file_name );

   /* write the gnuplot script to read and plot the file */
   graphics_matrix_pattern_script_graph( A, file_name );

   /* finito */
   return;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_script_gnuplot   ( const SPMAT* A, const char *file_name )
{
   FILE *fp;
   char file_gnuplot[64];

   snprintf(file_gnuplot, 64, "%s_%s.gnu" , file_name, suffix);
   file_gnuplot[63] = '\0';

   fp = fopen(file_gnuplot, "w");

   fprintf(fp, "#! /usr/bin/env gnuplot\n");
   fprintf(fp, "# \n");
   fprintf(fp, "set title \"MATRIX PROFILE (%dx%d)\"\n",A->m,A->n);
   fprintf(fp, "set nokey \n");
   fprintf(fp, "set size square \n");
   fprintf(fp, "# \n");
   fprintf(fp, "plot [0:%d][%d:0] '%s_%s.dat'\n", A->n, -A->m, file_name, suffix);
   fprintf(fp, "pause -1\n");

   fclose(fp);

   /* make the file executable */
#ifdef HAVE_POSIX
   chmod(file_gnuplot, S_IRWXU | S_IRGRP | S_IROTH );
#endif
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

static void graphics_matrix_pattern_gnuplot( const SPMAT* A, const char *file_name )
{
   /* write the file of the solution */
   graphics_matrix_pattern_outputfile( A, file_name );

   /* write the gnuplot script to read and plot the file */
   graphics_matrix_pattern_script_gnuplot( A, file_name );

   /* finito */
   return;
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

void graphics_matrix_pattern (const char* format, const SPMAT* A, const char *name_file)
{
   if ( format    == (char*)NULL     ) error(E_NULL, "graphics_matrix_pattern");
   if ( A         == (SPMAT*)NULL    ) error(E_NULL, "graphics_matrix_pattern");
   if ( name_file == (char*)NULL     ) error(E_NULL, "graphics_matrix_pattern");


   if ( strcmp("X11",format) == 0 )
   {
      graphics_matrix_pattern_X11(A, name_file);
   }
   else
   if ( strcmp("gnuplot",format) == 0 )
   {
      graphics_matrix_pattern_gnuplot(A, name_file);
   }
   else
   if ( strcmp("graph",format) == 0 )
   {
      graphics_matrix_pattern_graph(A, name_file);
   }
   else
   {
      error(E_UNKNOWN, "graphics_matrix_pattern");
   }

   return;
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

