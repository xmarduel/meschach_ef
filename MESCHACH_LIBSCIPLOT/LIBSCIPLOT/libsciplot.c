

#include "libsciplot.h"


const char *progname = "graph";	/* name of this program */

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Multigrapher*  libsciplot_open(char *display_type, void* display, void *pwindow, char *bg_color, char *bitmap_size )
{
   Multigrapher* MULTIGRAPHER;

   /* command-line parameters (constant over multigrapher operation) */
   
   //const char *display_type    = "X";  /* libplot output format */
   //const char *bg_color        = NULL; /* color of background, if non-NULL */
   //const char *bitmap_size     = NULL;
   const char *emulate_color   = NULL;
   const char *max_line_length = NULL;
   const char *meta_portable   = NULL;
   const char *page_size       = NULL;
   const char *rotation_angle  = NULL;
   const bool save_screen      = false;
   
   if ((MULTIGRAPHER = new_xmultigrapher (display_type,
                                         display,
                                         pwindow,
                                         bg_color,
                                         bitmap_size,
                                         emulate_color,
                                         max_line_length,
                                         meta_portable,
                                         page_size,
                                         rotation_angle,
                                         save_screen)) == NULL)
   {
      fprintf (stderr, "%s: error: couldn't open graphing device\n", progname);
      return NULL;
   }
   else
   {
      set_graph_parameters_default(MULTIGRAPHER);
   }
   
   return MULTIGRAPHER;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_end(Multigrapher* MULTIGRAPHER)
{
   /* finish up by deleting our multigrapher (one must have been created, since we always read at least stdin) */
   if (delete_multigrapher ( MULTIGRAPHER ) < 0)
   {
      fprintf (stderr, "%s: error: could not close graphing device\n", progname);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_bgcolor  (Multigrapher* MULTIGRAPHER, int color)
{
   char *bg_color;

 
   switch(color)
   {
      case LIBPLOT_AQUA:     bg_color = "aqua"; break;
      case LIBPLOT_BLACK:    bg_color = "black"; break;
      case LIBPLOT_BLUE:     bg_color = "blue"; break;
      case LIBPLOT_FUSCHIA:  bg_color = "fuchsia"; break;
      case LIBPLOT_GRAY:     bg_color = "gray"; break;
      case LIBPLOT_GREEN:    bg_color = "green"; break; 
      case LIBPLOT_LIME:     bg_color = "lime"; break;
      case LIBPLOT_MAROON:   bg_color = "maroon"; break; 
      case LIBPLOT_NAVY:     bg_color = "navy"; break;   
      case LIBPLOT_OLIVE:    bg_color = "olive"; break;
      case LIBPLOT_PURPLE:   bg_color = "purple"; break;
      case LIBPLOT_RED:      bg_color = "red"; break;
      case LIBPLOT_SILVER:   bg_color = "silver"; break; 
      case LIBPLOT_TEAL:     bg_color = "teal"; break;
      case LIBPLOT_WHITE:    bg_color = "white"; break;
      case LIBPLOT_YELLOW:   bg_color = "yellow"; break; 

      default:               bg_color = "white"; break;
   }
	
	if (MULTIGRAPHER->bg_color == NULL)
	{
       MULTIGRAPHER->bg_color = strdup(bg_color); // for the "boxes"
	}
	else
	if (strcmp(MULTIGRAPHER->bg_color, bg_color) != 0)
	{
		free((void*)MULTIGRAPHER->bg_color);
		MULTIGRAPHER->bg_color = strdup(bg_color);
	}

   pl_bgcolorname_r(MULTIGRAPHER->plotter, bg_color); // for the "frame"
}


/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_swin(Multigrapher* MULTIGRAPHER, double xmin, double xmax, double ymin, double ymax)
{
   set_graph_single_parameter( MULTIGRAPHER , GRAPH_PARAMe_X_AXIS, xmin, xmax, 0.0);
   set_graph_single_parameter( MULTIGRAPHER , GRAPH_PARAMe_Y_AXIS, ymin, ymax, 0.0);
}

/*---------------------------------------------------------------------*/
/*---------------------------------------------------------------------*/

void libsciplot_ewin(Multigrapher* MULTIGRAPHER, double *x1, double *x2, double *y1, double *y2, double *vec_x, double *vec_y, int no_of_points )
{
   /* for storage of data points  */
   Point *p;			/* points array */

   /* >>> SET-UP THE POINTS NOW <<< */
   p = libsciplot_fillpoints_for_lines(vec_x, vec_y, no_of_points, MULTIGRAPHER->linemode);

   /* fill in any of min_? and max_? that user didn't specify (the
      prefix "final_" means these arguments were finalized at the
      time the first file of the plot was processed) */

   array_bounds(p, no_of_points,
                /*final_transpose_axes*/false, /*clip_mode*/0,
                x1, y1,
                x2, y2,
                /*final_spec_min_x*/false, false/*final_spec_min_y*/,
                /*final_spec_max_x*/false, false/*final_spec_max_y*/);

   free(p);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_vstd(Multigrapher* MULTIGRAPHER)
{
   /* dimensions of graphing area, expressed as fractions of the width of
      the libplot graphics display [by convention square] */

   double right  = 0.125;
   double up     = 0.125;

   double width  = 0.75;
   double height = 0.75;

   /* correct for more than 1 sub-panel */
   if ( MULTIGRAPHER->nb_panl_x != MULTIGRAPHER->nb_panl_y )
   {
      switch(MULTIGRAPHER->nb_panl_x)
      {
         case 1:
         {
            switch(MULTIGRAPHER->nb_panl_y)
            {
               case 2: width = 1.5; break;
               case 3: width = 2.7; break;
               default: printf("no more than 3 panels in one direction\n"); exit(EXIT_FAILURE);
            }
         }
         break;

         case 2:
         switch(MULTIGRAPHER->nb_panl_y)
         {
            case 1: height = 1.5; break;
            case 3: height = 1.5; break;
            default: printf("no more than 3 panels in one direction\n"); exit(EXIT_FAILURE);
         }
         break;

         case 3:
         switch(MULTIGRAPHER->nb_panl_y)
         {
            case 1: height = 2.8; break;
            case 2: height = 0.7; break;
            default: printf("no more than 3 panels in one direction\n"); exit(EXIT_FAILURE);
         }
         break;         
      }
   }

   /*printf(" w = %lf    h=%lf\n", width,height );*/
   
   set_graph_single_parameter ( MULTIGRAPHER, GRAPH_PARAMe_VIEWPORT, right, up, width, height);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_svp(Multigrapher* MULTIGRAPHER, double xmin, double xmax, double ymin, double ymax)
{
   double right  = xmin;
   double up     = ymin;

   double width  = xmax - xmin;
   double height = ymax - ymin;

   set_graph_single_parameter ( MULTIGRAPHER, GRAPH_PARAMe_VIEWPORT, right, up, width, height);

   printf("viewport : pos = %lf - %lf , size = %lf - %lf\n", right, up, width, height );
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_subp(Multigrapher* MULTIGRAPHER, int nx, int ny) /* plotting window divided in (nx x ny) sub-windows */
{
   int i,j;
   /* eval the reposition */
   double size[3] = {1.0, 0.5, 0.33};

   double position[3][3] = { {0.0,0.0,0.0}, {0.0,0.5,0.0}, {0.0,0.33,0.66} };

   MULTIGRAPHER->nb_panl_x = nx;
   MULTIGRAPHER->nb_panl_y = ny;

   for (i=0; i<nx; i++)
   for (j=0; j<ny; j++)
   {
      MULTIGRAPHER->position_x[i][j] = position[nx-1][i];
      MULTIGRAPHER->position_y[i][j] = position[ny-1][j];

      MULTIGRAPHER->size_x[i][j]     = size[nx-1];
      MULTIGRAPHER->size_y[i][j]     = size[ny-1];
   }

   printf("\n libsciplot_subp (%d,%d)\n", nx,ny);
		 
   for (i=0; i<nx; i++)
   for (j=0; j<ny; j++)
   {
      printf(" plot (%d,%d) : size = [%lf-%lf], pos = [%lf-%lf] \n", i+1,j+1,
               MULTIGRAPHER->size_x[i][j],
               MULTIGRAPHER->size_y[i][j],
               MULTIGRAPHER->position_x[i][j],
               MULTIGRAPHER->position_y[i][j]);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_panl(Multigrapher* MULTIGRAPHER, int i, int j)
{
   double reposition_trans_x = MULTIGRAPHER->position_x[i-1][j-1];
   double reposition_trans_y = MULTIGRAPHER->position_y[i-1][j-1];
   double reposition_scale   = MULTIGRAPHER->size_y[i-1][j-1] < MULTIGRAPHER->size_x[i-1][j-1] ? MULTIGRAPHER->size_y[i-1][j-1] : MULTIGRAPHER->size_x[i-1][j-1] ;

   /* call this before "begin_graph" */
   libsciplot_reset(MULTIGRAPHER);

   /* now scale for the current panel */
   begin_graph ( MULTIGRAPHER, reposition_scale, reposition_trans_x, reposition_trans_y);  

   /* set color for graph frame */
   if (MULTIGRAPHER->frame_color)
   {
      pl_pencolorname_r (MULTIGRAPHER->plotter, MULTIGRAPHER->frame_color);
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_titlefont(Multigrapher* MULTIGRAPHER, const char * fontname)
{	
	set_graph_single_parameter ( MULTIGRAPHER, GRAPH_PARAMe_TITLE_FONT_NAME, fontname);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_xaxisfont(Multigrapher* MULTIGRAPHER, const char * fontname)
{	
	set_graph_single_parameter ( MULTIGRAPHER, GRAPH_PARAMe_X_AXIS_FONT_NAME, fontname);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_yaxisfont(Multigrapher* MULTIGRAPHER, const char * fontname)
{	
	set_graph_single_parameter ( MULTIGRAPHER, GRAPH_PARAMe_Y_AXIS_FONT_NAME, fontname);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_framecolor(Multigrapher* MULTIGRAPHER, int color)
{
   char *str_color;
   
   switch(color)
   {
      case LIBPLOT_COLOR_BLACK:   str_color = "black"; break;
      case LIBPLOT_COLOR_WHITE:   str_color = "white"; break;
      case LIBPLOT_COLOR_RED:     str_color = "red";   break;
      case LIBPLOT_COLOR_GREEN:   str_color = "green"; break;
      case LIBPLOT_COLOR_BLUE:    str_color = "blue";  break;
      case LIBPLOT_COLOR_MAGENTA: str_color = "magenta"; break;
      case LIBPLOT_COLOR_CYAN:    str_color = "cyan"; break;

      default:                    str_color = "black"; break;
   }

   set_graph_single_parameter(MULTIGRAPHER, GRAPH_PARAMe_FRAME_COLOR, str_color);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_sci(Multigrapher* MULTIGRAPHER, int color)
{
   int curr_linemode   = MULTIGRAPHER->linemode;  /* "global" style : color + line pattern */
   int curr_line_style = curr_linemode / 5 ;

   int new_linemode    = 5*curr_line_style + color;
   
   switch(color) /* don't change the line style but only the color */
   {
      case LIBPLOT_COLOR_WHITE:   MULTIGRAPHER->linemode = new_linemode; break;
      case LIBPLOT_COLOR_RED:     MULTIGRAPHER->linemode = new_linemode; break;
      case LIBPLOT_COLOR_GREEN:   MULTIGRAPHER->linemode = new_linemode; break;
      case LIBPLOT_COLOR_BLUE:    MULTIGRAPHER->linemode = new_linemode; break;
      case LIBPLOT_COLOR_MAGENTA: MULTIGRAPHER->linemode = new_linemode; break;
      case LIBPLOT_COLOR_CYAN:    MULTIGRAPHER->linemode = new_linemode; break;

      default:                    MULTIGRAPHER->linemode = 0; break;
   }

   switch(color) /* don't change the line style but only the color */
   {
      case LIBPLOT_COLOR_WHITE:   pl_pencolorname_r (MULTIGRAPHER->plotter, "white"); break;
      case LIBPLOT_COLOR_RED:     pl_pencolorname_r (MULTIGRAPHER->plotter, "red"); break;
      case LIBPLOT_COLOR_GREEN:   pl_pencolorname_r (MULTIGRAPHER->plotter, "green"); break;
      case LIBPLOT_COLOR_BLUE:    pl_pencolorname_r (MULTIGRAPHER->plotter, "blue"); break;
      case LIBPLOT_COLOR_MAGENTA: pl_pencolorname_r (MULTIGRAPHER->plotter, "magenta"); break;
      case LIBPLOT_COLOR_CYAN:    pl_pencolorname_r (MULTIGRAPHER->plotter, "cyan"); break;

      default:                    pl_pencolorname_r (MULTIGRAPHER->plotter, "black"); break;
   }

}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_sls(Multigrapher* MULTIGRAPHER, int line_style)
{
   int curr_linemode   = MULTIGRAPHER->linemode; /* "global" style : color + line pattern */
   int curr_color      = curr_linemode % 5 ;

   int linemode        = 5*line_style + curr_color;

   switch(line_style) /* don't change the color but only the line style */
   {
      case LIBPLOT_LINEMODE_SOLID:       MULTIGRAPHER->linemode = linemode; break;
      case LIBPLOT_LINEMODE_DOTTED:      MULTIGRAPHER->linemode = linemode; break;
      case LIBPLOT_LINEMODE_DOTDASHED:   MULTIGRAPHER->linemode = linemode; break;
      case LIBPLOT_LINEMODE_SHORTDASHED: MULTIGRAPHER->linemode = linemode; break;
      case LIBPLOT_LINEMODE_LONGDASHED:  MULTIGRAPHER->linemode = linemode; break;

      default:                           MULTIGRAPHER->linemode = curr_linemode; break;
   }
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_sfs(Multigrapher* MULTIGRAPHER, int fill_style)
{
   fprintf (stderr, "%s: error: fill style not used \n", "libsciplot_sfs");   
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_rect(Multigrapher* MULTIGRAPHER, double x_min, double x_max, double y_min, double y_max)
{
   Point *p = libsciplot_fillpoints_for_rectangle(x_min, x_max, y_min, y_max);

   /* plot the laboriously read-in array */
   plot_point_array ( MULTIGRAPHER, p, 5);

   /* free points array */
   free (p);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_text(Multigrapher* multigrapher, double pos_x, double pos_y, char *label)
{
   pl_move_r   (multigrapher->plotter, XV (pos_x), YV (pos_y));
   pl_alabel_r (multigrapher->plotter, 'c', 'b', label); /* centered in x, bottumed in y */
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_page(Multigrapher* MULTIGRAPHER)
{
   pl_erase_r  (MULTIGRAPHER->plotter);
   pl_fspace_r (MULTIGRAPHER->plotter, 0.0, 0.0, (double)PLOT_SIZE, (double)PLOT_SIZE);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_sch (Multigrapher* MULTIGRAPHER, double size_label)
{
   set_graph_single_parameter ( MULTIGRAPHER, GRAPH_PARAMe_TITLE_FONT_SIZE, size_label);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_lab(Multigrapher* MULTIGRAPHER, const char *xlabel, const char *ylabel, const char *title)
{
   if (MULTIGRAPHER->title == NULL)
	{
		MULTIGRAPHER->title = strdup(title);
	}
	else
	if ( strcmp(MULTIGRAPHER->title, title) != 0 )
	{
		free((void*)MULTIGRAPHER->title);
		MULTIGRAPHER->title = strdup(title);
	}

	if (MULTIGRAPHER->x_axis.label == NULL)
	{
		MULTIGRAPHER->x_axis.label = strdup(xlabel);
	}
	else
	if ( strcmp(MULTIGRAPHER->x_axis.label, xlabel) != 0 )
	{
		free((void*)MULTIGRAPHER->x_axis.label);
		MULTIGRAPHER->x_axis.label = strdup(xlabel);
	}
	
	if (MULTIGRAPHER->y_axis.label == NULL)
	{
		MULTIGRAPHER->y_axis.label = strdup(ylabel);
	}
	else
	if ( strcmp(MULTIGRAPHER->y_axis.label, ylabel) != 0 )
	{
		free((void*)MULTIGRAPHER->y_axis.label);
		MULTIGRAPHER->y_axis.label = strdup(ylabel);
	}

   draw_frame_title(MULTIGRAPHER);

   draw_frame_xlabel(MULTIGRAPHER);
   draw_frame_ylabel(MULTIGRAPHER);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_erase(Multigrapher* MULTIGRAPHER)
{
   pl_erase_r  (MULTIGRAPHER->plotter);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_flush(Multigrapher* MULTIGRAPHER)
{
	end_polyline_and_flush(MULTIGRAPHER);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_reset(Multigrapher* MULTIGRAPHER)
{
   pl_fspace_r (MULTIGRAPHER->plotter, 0.0, 0.0, (double)PLOT_SIZE, (double)PLOT_SIZE);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_env      (Multigrapher* MULTIGRAPHER, double xmin, double xmax, double ymin, double ymax, int justification, int axis_style)
{
   libsciplot_svp(MULTIGRAPHER, xmin, xmax, ymin, ymax);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_box(Multigrapher* multigrapher, const char* xstyle, double xtick, int nxsub, const char* ystyle, double ytick, int nysub)
{
   /* default box */
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_FRAME_LINE_WIDTH       , -0.05);
   /*set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_FRAME_COLOR            , "black");*/
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_GRID_SPEC              , AXES_AND_BOX);
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_ROUND_TO_NEXT_TICK     , false); /* round axis limits to nearest tick? */
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_TICKS_SIZE             , 0.02); 
     
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_LOG_AXIS        , false);
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_SWITCH_END      , false); /* ticks+legend of frame for x-dir at top/bottom */
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_OMIT_TICKS      , false); /* no ticks+legend at the frame in x-dir */
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_DRAW_AXE_ORIGIN , false);

   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_LOG_AXIS        , false);
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_SWITCH_END      , false); /* ticks+legend of frame for y-dir at right/left */
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_OMIT_TICKS      , false); /* no ticks+legend at the frame in y-dir */
   set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_DRAW_AXE_ORIGIN , false);

   /* log. axis */
   if ( strstr(xstyle, "L") != NULL )
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_LOG_AXIS  , true);

   if ( strstr(ystyle, "L") != NULL )
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_LOG_AXIS  , true);

   /* where are the legend & ticks */
   if ( strstr(xstyle, "M") != NULL )
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_SWITCH_END, true);

   if ( strstr(ystyle, "M") != NULL )
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_SWITCH_END, true);


   /* type of box */
   if ( (NULL!=strstr(xstyle, "G")) || (NULL!=strstr(ystyle, "G")) )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_GRID_SPEC        , AXES_AND_BOX_AND_GRID);
   }
   else
   {
      if ( (NULL!=strstr(xstyle, "BC")) && (NULL!=strstr(ystyle, "BC")) )  
      {
          set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_GRID_SPEC    , AXES_AND_BOX);
      }
      else 
      {
         set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_GRID_SPEC     , AXES);
      }
   }

   /* ticks */
   if ( strstr(xstyle, "T") == NULL )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_OMIT_TICKS, true);
   }

   if ( strstr(ystyle, "T") == NULL )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_OMIT_TICKS, true);
   }

   /* sub-ticks */
   if ( strstr(xstyle, "S") != NULL )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_SUBTICKS_SIZE , true);
   }

   if ( strstr(ystyle, "S") != NULL )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_SUBTICKS_SIZE , true);
   }


   /* draw axes at origin  */
   if ( strstr(xstyle, "A") != NULL )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_X_AXIS_DRAW_AXE_ORIGIN , true);
   }

   if ( strstr(ystyle, "A") != NULL )
   {
      set_graph_single_parameter ( multigrapher, GRAPH_PARAMe_Y_AXIS_DRAW_AXE_ORIGIN , true);
   }


   /* unimplemented ones */
   if ( (NULL!=strstr(xstyle, "I")) || (NULL!=strstr(ystyle, "I")) )
   {
      printf("format \"I\" for libsciplot_box is not implemented \n");
   }
   if ( (NULL!=strstr(xstyle, "V")) || (NULL!=strstr(ystyle, "V")) )
   {
      printf("format \"V\" for libsciplot_box is not implemented \n");
   }
   
   /* draw the frame */
   draw_frame_of_graph ( multigrapher , false);  /* true -> draw bg ; false -> do not draw bg */

/*
 
Options (for parameters XOPT and YOPT):

* = done  ,
o = possible, todo ,
- = impossible to implement

*   A : draw Axis (X axis is horizontal line Y=0, Y axis is vertical line X=0).
*   B : draw bottom (X) or left (Y) edge of frame.
*   C : draw top (X) or right (Y) edge of frame.
*   G : draw Grid of vertical (X) or horizontal (Y) lines.
-   I : Invert the tick marks; ie draw them outside the viewport instead of inside.
*   L : label axis Logarithmically (see below).
*   N : write Numeric labels in the conventional location below the viewport (X) or to the left of the viewport (Y).
-   P : extend ("Project") major tick marks outside the box (ignored if option I is specified).
*   M : write numeric labels in the unconventional location above the viewport (X) or to the right of the viewport (Y).
*   T : draw major Tick marks at the major coordinate interval.
o   S : draw minor tick marks (Subticks).
-   V : orient numeric labels Vertically. This is only applicable to Y. The default is to write Y-labels parallel to the axis.
-   1 : force decimal labelling, instead of automatic choice (see PGNUMB).
-   2 : force exponential labelling, instead of automatic.

*/
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_curs(Multigrapher* MULTIGRAPHER, double *x, double  *y, char *ch)
{
   fprintf (stdout, "---> function libsciplot_curs not yet implemented !! \n");
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_checkcursor(Multigrapher* MULTIGRAPHER)
{
   fprintf (stdout, "---> function libsciplot_checkcursor not yet implemented !! \n");
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_pap (Multigrapher* MULTIGRAPHER, int xpixel, int ypixel)
{
   fprintf (stdout, "---> function libsciplot_pap not yet implemented !! \n");
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_ask (Multigrapher* MULTIGRAPHER, int ask)
{
   fprintf (stdout, "---> function libsciplot_ask not yet implemented !! \n");
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_pt1(Multigrapher* MULTIGRAPHER, double x, double y, int marker)
{
   Point *p = libsciplot_fillpoints_for_points(&x, &y, 1, marker, MULTIGRAPHER->linemode);
   
   /* plot the laboriously read-in array */
   plot_point(MULTIGRAPHER, p);

   /* free points array */
   free (p);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_line(Multigrapher* MULTIGRAPHER, int len , double *x  , double *y )
{
   Point *p = libsciplot_fillpoints_for_lines(x, y, len, MULTIGRAPHER->linemode);

   /* plot the laboriously read-in array */
   plot_point_array (MULTIGRAPHER, p, len);

   /* free points array */
   free (p);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

void libsciplot_pt  (Multigrapher* MULTIGRAPHER, int len , double *x  , double *y , int marker )
{
   Point *p = libsciplot_fillpoints_for_points(x, y, len, marker, MULTIGRAPHER->linemode);

   /* plot the laboriously read-in array */
   plot_point_array ( MULTIGRAPHER, p, len);

   /* free points array */
   free (p);
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Point* libsciplot_fillpoints_for_array_bounds_eval(double *x, double *y, int len)
{
   int i;
   Point *p = (Point *)malloc ( len  * sizeof (Point));

   for (i=0; i<len; i++)
   {
      p[i].x = x[i];
      p[i].y = y[i];

      /* we need only these 3 more fields in array-bounds functions */
      p[i].pendown = (i == 0 ? false : true );
      p[i].linemode   = 1;
      p[i].fill_fraction = -1;
   }

   return p;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Point* libsciplot_fillpoints_for_lines(double *x, double *y, int len, int linemode)
{
   int i;
   Point *p = (Point *)malloc ( len  * sizeof (Point));

   p[0].x = x[0];
	p[0].y = y[0];
	
	p[0].have_x_errorbar = false;
	p[0].have_y_errorbar = false;
	
	p[0].xmin = false;
	p[0].xmax = false;
	
	p[0].ymin = false;
	p[0].ymax = false;
	
	p[0].pendown = false;
	
	p[0].symbol           = false;
	p[0].symbol_size      = 0.03;
	//p[i].symbol_font_name = strdup("ZapfdDingbats");
	
	p[0].linemode   = linemode;
	p[0].line_width = -0.001;
	
	p[0].fill_fraction = -1;
	p[0].use_color     = true;
	
	for (i=1; i<len; i++)
   {
		memcpy(p+i, p, sizeof(Point));

      p[i].x = x[i];
      p[i].y = y[i];

		p[i].pendown = true;
		/*
		p[i].have_x_errorbar = false;
      p[i].have_y_errorbar = false;

      p[i].xmin = false;
      p[i].xmax = false;

      p[i].ymin = false;
      p[i].ymax = false;

      p[i].pendown = (i == 0 ? false : true );

      p[i].symbol           = false;
      p[i].symbol_size      = 0.03;
			//p[i].symbol_font_name = strdup("ZapfdDingbats");

      p[i].linemode   = linemode;
      p[i].line_width = -0.001;

      p[i].fill_fraction = -1;
      p[i].use_color     = true;
		*/
   }

   return p;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Point* libsciplot_fillpoints_for_points(double *x, double *y, int len, int symbol, int linemode)
{
   int i;
   Point *p = (Point *)malloc ( len  * sizeof (Point));

   p[0].x = x[0];
	p[0].y = y[0];
	
	p[0].have_x_errorbar = false;
	p[0].have_y_errorbar = false;
	
	p[0].xmin = false;
	p[0].xmax = false;
	
	p[0].ymin = false;
	p[0].ymax = false;
	
	p[0].pendown = false ;
	
	p[0].symbol           = symbol;
	p[0].symbol_size      = 0.03;
	//p[i].symbol_font_name = strdup("ZapfdDingbats");
	
	p[0].linemode   = linemode;
	p[0].line_width = -0.001;
	
	p[0].fill_fraction = -1;
	p[0].use_color     = true;
	
	for (i=1; i<len; i++)
   {
      memcpy(p+i, p, sizeof(Point));
		
		p[i].x = x[i];
      p[i].y = y[i];

		/*
      p[i].have_x_errorbar = false;
      p[i].have_y_errorbar = false;

      p[i].xmin = false;
      p[i].xmax = false;

      p[i].ymin = false;
      p[i].ymax = false;

      p[i].pendown = false ;

      p[i].symbol           = symbol;
      p[i].symbol_size      = 0.03;
		//p[i].symbol_font_name = strdup("ZapfdDingbats");

      p[i].linemode   = linemode;
      p[i].line_width = -0.001;

      p[i].fill_fraction = -1;
      p[i].use_color     = true;
		*/
   }

   return p;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/

Point* libsciplot_fillpoints_for_rectangle(double x1, double x2, double y1, double y2)
{
   int i;
   Point *p = (Point *)malloc ( 5  * sizeof (Point));

   double x[5] = { x1, x2, x2, x1, x1 };
   double y[5] = { y1, y1, y2, y2, y1 };
   
   for (i=0; i<5; i++)
   {
      p[i].x = x[i];
      p[i].y = y[i];

      p[i].have_x_errorbar = false;
      p[i].have_y_errorbar = false;

      p[i].xmin = false;
      p[i].xmax = false;

      p[i].ymin = false;
      p[i].ymax = false;

      p[i].pendown = (i == 0 ? false : true );

      p[i].symbol           = false;
      p[i].symbol_size      = 0.03;
      p[i].symbol_font_name = strdup("ZapfdDingbats");

      p[i].linemode   = 1;
      p[i].line_width = -0.001;

      p[i].fill_fraction = -1;
      p[i].use_color     = true;
   }

   return p;
}

/*--------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------*/
