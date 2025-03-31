#include <stdio.h>
#include <string.h>
#include <vogle.h>
#include <vopl.h>

#define SPEC 5
#define GROUPS 8

char * names[GROUPS]= {
   "Name1", "Name2", "Name3", "Name4", "Name5", "Name6",
   "Name7", "Name8"
};
char * lnames[SPEC]= {
   "leg1", "leg2", "leg3", "leg4", "leg5"
};
char *title= {"Title"};

float hgt[GROUPS][SPEC]= {
     {	  8.648978 ,
	  8.821889 ,
	  4.645878 ,
	  8.531050 ,
	  9.036387
     } , {	  
	  8.170376 ,
	  8.999420 ,
	 10.740744 ,
	  6.942840 ,
	  6.663000
     } , {	  
	  9.782000 ,
	  8.204000 ,
	  10.16300 ,
	  12.86800 ,
	  10.62200
     } , {	  
	  9.364000 ,
	  12.08700 ,
	 11.972000 ,
	  2.663000 ,
	  4.782000
     } , {	  
	  8.999999 ,
	  4.204000 ,
	  1.86800 ,
	  0.62200 ,
	  3.364000
     } , {	  
	  5.08700 ,
	  7.972000 ,
	  3.833330 ,
     	  8.648978 ,
	  8.821889
     } , {	  
	  4.645878 ,
	  8.531050 ,
	  9.036387 ,
	  8.170376 ,
	  8.999420
     } , {	  
	 10.740744 ,
	  6.942840 ,
	  6.663000 ,
	  9.999999 ,
	  8.204000 
     }
};
float m0[SPEC]= {
      	  450.0000 ,
	  440.0000 ,
	  430.0000 ,
	  420.0000 ,
	  410.0000 ,
	};
float ms[SPEC]= {
      	  250.0000 ,
	  240.0000 ,
	  230.0000 ,
	  220.0000 ,
	  210.0000 ,
	};
float mn[SPEC]= {
      	  45.00000 ,
	  44.00000 ,
	  43.00000 ,
	  42.00000 ,
	  41.00000 ,
	};

void drawlegend( int ns, char *lnames[], char *title);
	
main()
{
  char device[20], strval[20], str[20], scalefact[20];
  float scale, width, heigh;
  float x0, y0, dy;
  int i;

/*
 *	Get VOGLE device
*/
  fprintf(stderr, "Enter VOGLE device: <X11>: [Rtn]");
  gets(device);
  if (strcmp(device,"") <= 0) {
    strcpy(device, "X11");
  }

  /*
   * Make a little bit room for the legend 
   */
  scale = .80;

  prefsize(768, 512);
  vinit(device);

  getfontsize(&width,&heigh);
  color(0);
  clear();
  color(7);

/* Clipping off */
  clipping(0);

  viewport(-scale,scale,-.90*scale,.90*scale);

  polyfill(1);

  /*
   * Draw the histogram
   */
  hist2(hgt, GROUPS, SPEC, names);

  /*
   * Plot the standard axes
   */
  axistitle("Y Axis",'y');     
  drawaxes2();
 
  /*
   * Adjust the scale for secondary y axis manually
   */
/*  adjustscale(hgt, GROUPS*SPEC, 't'); */
  axistitle("Sec. X Axis",'s');
  axistitle("Sec. Y Axis",'t');
  scaling(1,'s');
  scaling(1,'t');
  range((float)1.,(float)100000.,'t');
  range((float)0.00001,(float)1.,'s');
  drawaxis('s'); 
  drawaxis('t'); 

  /*
   * create a legend
   */
  drawlegend(SPEC, lnames, title); 

  getkey();

  vexit();

}

	
void drawlegend( int ns, char *lnames[], char *title)
{
  float	width, heigh, dy, dy2, x0, dx0, y0;
  int i, j;
	
  pushattributes();
  pushmatrix();
  pushviewport();

  getfontsize(&width,&heigh);
  ortho2(-1.0, 1.0, -1.0, 1.0);
  
  clipping(0);

  dy = (YMAX- YMIN)/25.0;
  dy2= dy/2.5;
  polyfill(1);
  x0=XMAX+0.20;
  dx0 = 0.05;
  y0 =YMIN-0.1;
  textsize(0.9*width,0.9*heigh); 
  leftjustify();
  bottomjustify();
  for (i=0 ; i<ns; i++) {
    j = ns-1-i;
    color(j%6+1);
    rect(x0,y0 +i*dy,x0+dx0,y0 +i*dy+dy2);
    move2(x0+1.5*dx0, y0 +i*dy);
    color(7);
    drawstr(lnames[j]);
  }  
  
  textsize(1.0*width,1.0*heigh); 
  move2(x0,y0 +ns*dy);
  drawstr("Legende");

/* draw the title */
  centertext(1);
  textsize(1.20*width,1.20*heigh); 
  move2(0.10, YMAX + 0.1);
  drawstr(title);
  
  popviewport();
  popmatrix();
  popattributes();
}

