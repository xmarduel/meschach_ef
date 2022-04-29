
#include "libsciplot.h"

#define SIZE 2000
#define NBITER 100

int main(int argc, char **argv)
{
   Multigrapher* plotter;

   double x[SIZE];
   double y[SIZE];
   int i;
   int k;
   
   for (i=0; i<SIZE; i++)
   {
      x[i] = (double)(i)/50.0;
      y[i] = sin(x[i])*exp(-i/600.0);
   }

   plotter = libsciplot_open("X", NULL, NULL, NULL, "900x900");
	
   if (plotter == NULL)
   {
	   printf("Failed to create plotter");
	   return 1;
   }

	libsciplot_bgcolor(plotter, LIBPLOT_GREEN);
   libsciplot_vstd(plotter);
   libsciplot_swin(plotter,0, SIZE/50.0, -1.1, 1.1);
   
   libsciplot_box(plotter, "BCNST", 10.0, 5, "BCNST", 10.0, 5);
   libsciplot_lab(plotter, "(x)", "(y)", "y = sin(x) * exp(-i/50)");

   for (k=0; k<NBITER; k++)
   {
       for (i=0; i<SIZE; i++)
       {
          y[i] = sin(x[i]-k*0.25)*exp(-i/600.0);
       }
       
        
       libsciplot_page (plotter);
       
       libsciplot_line(plotter, SIZE, x, y);
       /*libsciplot_swin(plotter,0, SIZE/50.0, -1.1, 1.1);*/
   
       libsciplot_box(plotter, "BCNST", 10.0, 5, "BCNST", 10.0, 5);
       libsciplot_lab(plotter, "(x)", "(y)", "y = sin(x - k*dt) * exp(-i/50)");
   }
   
   libsciplot_end(plotter);
   
   return 0;
}