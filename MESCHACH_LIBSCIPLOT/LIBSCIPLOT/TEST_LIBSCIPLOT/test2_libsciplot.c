
#include "libsciplot.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 2000

int main(int argc, char **argv)
{
   Multigrapher* plotter;

   double x[SIZE];
   double y[SIZE];
   int i;

   for (i=0; i<SIZE; i++)
   {
      x[i] = (double)(i)/50.0;
      y[i] = sin(x[i])*exp(-i/600.0);
   }

   plotter = libsciplot_open("X", NULL, NULL, "green", "600x600");

	//libsciplot_bgcolor(plotter, LIBPLOT_GREEN);
   libsciplot_vstd(plotter);
   libsciplot_swin(plotter,0, SIZE/50.0, -1.1, 1.1);
   
   libsciplot_box(plotter, "BCNST", 10.0, 5, "BCNST", 10.0, 5);
   libsciplot_lab(plotter, "(x)", "(y)", "y = sin(x) * exp(-i/50)");

   libsciplot_line(plotter, SIZE, x, y);

   libsciplot_end(plotter);
   
   return 0;
}