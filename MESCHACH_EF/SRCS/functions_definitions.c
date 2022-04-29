
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

#define  SQR(x)  (x)*(x)
#define  CUB(x)  (x)*(x)*(x)

#define  PI      acos(-1.0)

/**
* Contains the definitions of standart functions
*
*/


Real Zero1D( Real x )
{
   return 0.0; 
}
Real Zero2D( Real x , Real y )
{
   return 0.0;
}
Real Zero3D( Real x , Real y , Real z )
{
   return 0.0;
}

Real One1D( Real x )
{
   return 1.0; 
}
Real One2D( Real x , Real y )
{
   return 1.0;
}
Real One3D( Real x , Real y , Real z )
{
   return 1.0;
}

Real Zero1Dtransient( Real x , Real tps )
{
   return 0.0;
}
Real Zero2Dtransient( Real x , Real y , Real tps )
{
   return 0.0;
}
Real Zero3Dtransient( Real x , Real y , Real z , Real tps )
{
   return 0.0;
}

Real One1Dtransient( Real x , Real tps )
{
   return 1.0;
}
Real One2Dtransient( Real x , Real y , Real tps )
{
   return 1.0;
}
Real One3Dtransient( Real x , Real y , Real z , Real tps )
{
   return 1.0;
}

/********************************************************************/
/********************************************************************/

Real Phi1D_00(Real x)
{
   Real resu;
   return resu = 0.0; 
}
Real Src1D_00(Real x)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol1D_00(Real x)
{
   Real resu;
   return resu = 0.0; 
}
/********************************************************************/
Real Phi1D_01(Real x)
{
   Real resu;
   return resu = 1.0;
}
Real Src1D_01(Real x)
{
   Real resu;
   return resu = 1.0; 
}
Real Sol1D_01(Real x)
{
   Real resu;
   return resu = 1.0; 
}
/********************************************************************/
Real Phi1D_02(Real x)
{
   Real resu;
   return resu = 4.0*x*(1.0-x) + 0.0; 
}
Real Src1D_02(Real x)
{
   Real resu;
   return resu = 8.0; 
}
Real Sol1D_02(Real x)
{
   Real resu;
   return resu = 4.0*x*(1.0-x) + 0.0; 
}
/********************************************************************/
Real Phi1D_03(Real x)
{
   Real resu;
   return resu =  4.0*x*(1.0-x) + 1.0;
}
Real Src1D_03(Real x)
{
   Real resu;
   return resu = 8.0; 
}
Real Sol1D_03(Real x)
{
   Real resu;
   return resu =  4.0*x*(1.0-x) + 1.0;
}
/********************************************************************/
Real Phi1D_04(Real x)
{
   Real resu;
   return resu =  (x-2.0)*(x+2.0);
}
Real Src1D_04(Real x)
{
   Real resu;
   return resu = -2.0;
}
Real Sol1D_04(Real x)
{
   Real resu;
   return resu =  (x-2.0)*(x+2.0);
}
/********************************************************************/
Real Phi1D_05(Real x)
{
   Real resu;
   return resu = (x-0.5)*(x-0.5)+2.0; 
}
Real Src1D_05(Real x)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol1D_05(Real x)
{
   Real resu;
   return resu = (x-0.5)*(x-0.5)+2.0; 
}
/********************************************************************/
Real Phi1D_06(Real x)
{
   Real resu =  -0.5*x*(x-1);
   return resu ;
}
Real Src1D_06(Real x)
{
   Real resu;
   return resu = 1.0;
}
Real Sol1D_06(Real x) 
{
   Real resu =  -0.5*x*(x-1);
   return resu ;
}
/********************************************************************/
Real Phi1D_07(Real x) /* is (Phi1D_06)'(0) */
{
   Real resu =   1.0;
   return resu;
}
Real Src1D_07(Real x)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol1D_07(Real x) /* is (Phi1D_06)'(0) */
{
   Real resu =   1.0;
   return resu;
}
/********************************************************************/
Real Phi1D_08(Real x)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) ); 
}
Real Src1D_08(Real x)
{
   Real resu;
   /*return resu = 10.0;*/
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) );
}
Real Sol1D_08(Real x)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) ); 
}
/********************************************************************/
Real Phi1D_09(Real x)
{
   Real resu;
   return resu = cos(2*PI*x);  
}
Real Src1D_09(Real x)
{
   Real resu;
   return resu = SQR(2*PI)*cos(2*PI*x); 
}
Real Sol1D_09(Real x)
{
   Real resu;
   return resu = cos(2*PI*x);  
}

/****************************************************************************/
/****************************************************************************/

Real Phi2D_00(Real x,Real y)
{
   Real resu;
   return resu = 0.0; 
}
Real Src2D_00(Real x,Real y)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol2D_00(Real x,Real y)
{
   Real resu;
   return resu = 0.0; 
}
/********************************************************************/
Real Phi2D_01(Real x,Real y)
{
   Real resu;
   return resu = 1.0;
}
Real Src2D_01(Real x,Real y)
{
   Real resu;
   return resu = 1.0; 
}
Real Sol2D_01(Real x,Real y)
{
   Real resu;
   return resu = 1.0;
}
/********************************************************************/
Real Phi2D_02(Real x,Real y)
{
   Real resu;
   return resu = 2.0; 
}
Real Src2D_02(Real x,Real y)
{
   Real resu;
   return resu = -1.0; 
}
Real Sol2D_02(Real x,Real y)
{
   Real resu;
   return resu = 2.0; 
}
/********************************************************************/
Real Phi2D_03(Real x,Real y)
{
   Real resu;
   return resu =  4.0*y*(1.0-y);
}
Real Src2D_03(Real x,Real y)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol2D_03(Real x,Real y)
{
   Real resu;
   return resu =  4.0*y*(1.0-y);
}
/********************************************************************/
Real Phi2D_04(Real x,Real y)
{
   Real resu;
   return resu = x + 2*y;
}
Real Src2D_04(Real x,Real y)
{
   Real resu;
   /*return resu = 0.0; */
   return resu = x + 2*y; 
}
Real Sol2D_04(Real x,Real y)
{
   Real resu;
   return resu = x + 2*y;
}
/********************************************************************/
Real Phi2D_05(Real x,Real y)
{
   Real resu;
   return resu = (y-2.0)*(y+2.0)/8.0; 
}
Real Src2D_05(Real x,Real y)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol2D_05(Real x,Real y)
{
   Real resu;
   return resu = (y-2.0)*(y+2.0)/8.0; 
}
/********************************************************************/
Real Phi2D_06(Real x,Real y)
{
   Real resu;
   return resu = (y-1.0)*(y+1.0);
}
Real Src2D_06(Real x,Real y)
{
   Real resu;
   return resu = 1.0; 
}
Real Sol2D_06(Real x,Real y)
{
   Real resu;
   return resu = (y-1.0)*(y+1.0);
}
/********************************************************************/
Real Phi2D_07(Real x,Real y)
{
   Real resu;
   return resu = 4.0*x*(1.0-x);
}
Real Src2D_07(Real x,Real y)
{
   Real resu;
   return resu = -1.0; 
}
Real Sol2D_07(Real x,Real y)
{
   Real resu;
   return resu = 4.0*x*(1.0-x);
}
/********************************************************************/
Real Phi2D_08(Real x,Real y)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) ); 
}
Real Src2D_08(Real x,Real y)
{
   Real resu;
   return resu = 10.0;
   /*return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) );*/
}
Real Sol2D_08(Real x,Real y)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) ); 
}
/********************************************************************/
Real Phi2D_09(Real x,Real y)
{
   Real resu;
   return resu = cos(PI*x)*sin(PI*y);  
}
Real Src2D_09(Real x,Real y)
{
   Real resu;
   return resu = 2*SQR(PI)*cos(PI*x)*sin(PI*y); 
}
Real Sol2D_09(Real x,Real y)
{
   Real resu;
   return resu = cos(PI*x)*sin(PI*y);  
}
/***************************************************************************/ 
/***************************************************************************/

Real Phi3D_00( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_00( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_00( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_01( Real x , Real y , Real z )
{
   Real resu =  x + 2*y + 3*z;
   return resu;
}
Real Src3D_01( Real x , Real y , Real z )
{
   Real resu = 0.0 ;
   return resu;
}
Real Sol3D_01( Real x , Real y , Real z )
{
   Real resu =  x + 2*y + 3*z;
   return resu;
}

/********************************************************************/
Real Phi3D_02( Real x , Real y , Real z )
{
	Real resu = 1.0; // dPhi03/dx
	//Real resu = 2.0; // dPhi03/dy
	//Real resu = 3.0; // dPhi03/dz
   return resu;
}
Real Src3D_02( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_02( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_03( Real x , Real y , Real z )
{
   Real resu = x + 2*y + 3*z;
   return resu;
}
Real Src3D_03( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_03( Real x , Real y , Real z )
{
   Real resu = x + 2*y + 3*z;
   return resu;
}
/********************************************************************/
Real Phi3D_04( Real x , Real y , Real z )
{
   Real resu = 10.0/6.0*(1.0/2.0-(x-0.5)*(x-0.5)-(y-0.5)*(y-0.5)-(z-0.5)*(z-0.5) ) ;
   return resu;
}
Real Src3D_04( Real x , Real y , Real z )
{
   Real resu = 10.0;
   return resu;
}
Real Sol3D_04( Real x , Real y , Real z )
{
   Real resu = 10.0/6.0*(1.0/2.0-(x-0.5)*(x-0.5)-(y-0.5)*(y-0.5)-(z-0.5)*(z-0.5) ) ;
   return resu;
}
/********************************************************************/
Real Phi3D_05( Real x , Real y , Real z )
{
   Real resu = 3.0;
   return resu;
}
Real Src3D_05( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_05( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_06( Real x , Real y , Real z )
{
   Real resu = x + 2*y + 3*z ;
   return resu;
}
Real Src3D_06( Real x , Real y , Real z )
{
   Real resu = 0.0 ;
   return resu;
}
Real Sol3D_06( Real x , Real y , Real z )
{
   Real resu = x + 2*y + 3*z ;
   return resu;
}
/********************************************************************/
Real Phi3D_07( Real x , Real y , Real z )
{
   Real resu = 4.0*x*(1.0-x)*y*(1.0-y);
   return resu;
}
Real Src3D_07( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_07( Real x , Real y , Real z )
{
   Real resu = 4.0*x*(1.0-x)*y*(1.0-y);
   return resu;
}
/********************************************************************/
Real Phi3D_08( Real x , Real y , Real z )
{
   Real resu = 10.0/6.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5)-SQR(z-0.5) ); 
   return resu;
}
Real Src3D_08( Real x , Real y , Real z )
{
   Real resu = 10.0;
   return resu;
}
Real Sol3D_08( Real x , Real y , Real z )
{
   Real resu = 10.0/6.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5)-SQR(z-0.5) ); 
   return resu;
}
/********************************************************************/
Real Phi3D_09( Real x , Real y , Real z )
{
   Real resu = cos(PI*x)*sin(PI*y)*cos(PI*z);
   return resu;
}
Real Src3D_09( Real x , Real y , Real z )
{
   Real resu =  2*SQR(PI)*cos(PI*x)*sin(PI*y)*cos(PI*z);
   return resu;
}
Real Sol3D_09( Real x , Real y , Real z )
{
   Real resu = cos(PI*x)*sin(PI*y)*cos(PI*z);
   return resu;
}

/********************************************************************/
/********************************************************************/

Real Phi1D_00_Transient(Real x, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Src1D_00_Transient(Real x, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol1D_00_Transient(Real x, Real t)
{
   Real resu;
   return resu = 0.0; 
}
/********************************************************************/
Real Phi1D_01_Transient(Real x, Real t)
{
   Real resu;
   return resu = 1.0;
}
Real Src1D_01_Transient(Real x, Real t)
{
   Real resu;
   return resu = 1.0; 
}
Real Sol1D_01_Transient(Real x, Real t)
{
   Real resu;
   return resu = 1.0;
}
/********************************************************************/
Real Phi1D_02_Transient(Real x, Real t)
{
   Real resu;
   return resu = 4.0*x*(1.0-x) + 0.0; 
}
Real Src1D_02_Transient(Real x, Real t)
{
   Real resu;
   return resu = 8.0; 
}
Real Sol1D_02_Transient(Real x, Real t)
{
   Real resu;
   return resu = 4.0*x*(1.0-x) + 0.0; 
}
/********************************************************************/
Real Phi1D_03_Transient(Real x, Real t)
{
   Real resu;
   return resu =  4.0*x*(1.0-x) + 1.0;
}
Real Src1D_03_Transient(Real x, Real t)
{
   Real resu;
   return resu = 8.0; 
}
Real Sol1D_03_Transient(Real x, Real t)
{
   Real resu;
   return resu =  4.0*x*(1.0-x) + 1.0;
}
/********************************************************************/
Real Phi1D_04_Transient(Real x, Real t)
{
   Real resu;
   return resu =  (x-2.0)*(x+2.0);
}
Real Src1D_04_Transient(Real x, Real t)
{
   Real resu;
   return resu = -2.0;
}
Real Sol1D_04_Transient(Real x, Real t)
{
   Real resu;
   return resu =  (x-2.0)*(x+2.0);
}
/********************************************************************/
Real Phi1D_05_Transient(Real x, Real t)
{
   Real resu;
   return resu = (x-0.5)*(x-0.5)+2.0; 
}
Real Src1D_05_Transient(Real x, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol1D_05_Transient(Real x, Real t)
{
   Real resu;
   return resu = (x-0.5)*(x-0.5)+2.0; 
}
/********************************************************************/
Real Phi1D_06_Transient(Real x, Real t) /* is (Phi1D_05)'(0) */
{
   Real resu =  sin(x-t); 
   return resu ;
}
Real Src1D_06_Transient(Real x, Real t)
{
   Real resu;
   return resu = sin(x-t); 
}
Real Sol1D_06_Transient(Real x, Real t) /* is (Phi1D_05)'(0) */
{
   Real resu =  sin(x-t); 
   return resu ;
}
/********************************************************************/
Real Phi1D_07_Transient(Real x, Real t) /* is (Phi1D_06)'(0) */
{
   Real resu =   cos(x-t);
   return resu;
}
Real Src1D_07_Transient(Real x, Real t)
{
   Real resu;
   return resu = cos(x-t); 
}
Real Sol1D_07_Transient(Real x, Real t) /* is (Phi1D_06)'(0) */
{
   Real resu =   cos(x-t);
   return resu;
}
/********************************************************************/
Real Phi1D_08_Transient(Real x, Real t)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) ); 
}
Real Src1D_08_Transient(Real x, Real t)
{
   Real resu;
   /*return resu = 10.0;*/
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) );
}
Real Sol1D_08_Transient(Real x, Real t)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) ); 
}
/********************************************************************/
Real Phi1D_09_Transient(Real x, Real t)
{
   Real resu;
   return resu = -sin(x-t);  
}
Real Src1D_09_Transient(Real x, Real t)
{
   Real resu;
   return resu = -sin(x-t); 
}
Real Sol1D_09_Transient(Real x, Real t)
{
   Real resu;
   return resu = -sin(x-t);  
}
/****************************************************************************/
/****************************************************************************/

Real Phi2D_00_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Src2D_00_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol2D_00_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 0.0; 
}
/********************************************************************/
Real Phi2D_01_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 1.0;
}
Real Src2D_01_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 1.0; 
}
Real Sol2D_01_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 1.0;
}
/********************************************************************/
Real Phi2D_02_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 2.0; 
}
Real Src2D_02_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = -1.0; 
}
Real Sol2D_02_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 2.0; 
}
/********************************************************************/
Real Phi2D_03_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu =  4.0*y*(1.0-y);
}
Real Src2D_03_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol2D_03_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu =  4.0*y*(1.0-y);
}
/********************************************************************/
Real Phi2D_04_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = x + 2*y;
}
Real Src2D_04_Transient(Real x,Real y, Real t)
{
   Real resu;
   /*return resu = 0.0;*/ 
   return resu = x + 2*y; 
}
Real Sol2D_04_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = x + 2*y;
}
/********************************************************************/
Real Phi2D_05_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = (y-2.0)*(y+2.0)/8.0; 
}
Real Src2D_05_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 0.0; 
}
Real Sol2D_05_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = (y-2.0)*(y+2.0)/8.0; 
}
/********************************************************************/
Real Phi2D_06_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = (y-1.0)*(y+1.0);
}
Real Src2D_06_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 1.0; 
}
Real Sol2D_06_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = (y-1.0)*(y+1.0);
}
/********************************************************************/
Real Phi2D_07_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 4.0*x*(1.0-x);
}
Real Src2D_07_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = -1.0; 
}
Real Sol2D_07_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 4.0*x*(1.0-x);
}
/********************************************************************/
Real Phi2D_08_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) ); 
}
Real Src2D_08_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 10.0;
   /*return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) );*/
}
Real Sol2D_08_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) ); 
}
/********************************************************************/
Real Phi2D_09_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = cos(PI*x)*sin(PI*y);  
}
Real Src2D_09_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = 2*SQR(PI)*cos(PI*x)*sin(PI*y); 
}
Real Sol2D_09_Transient(Real x,Real y, Real t)
{
   Real resu;
   return resu = cos(PI*x)*sin(PI*y);  
}

/***************************************************************************/ 
/***************************************************************************/

Real Phi3D_00_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_00_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_00_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_01_Transient( Real x , Real y , Real z , Real t)
{
   Real resu =  x + 2*y + 3*z;
   return resu;
}
Real Src3D_01_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0 ;
   return resu;
}
Real Sol3D_01_Transient( Real x , Real y , Real z , Real t)
{
   Real resu =  x + 2*y + 3*z;
   return resu;
}
/********************************************************************/
Real Phi3D_02_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_02_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_02_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_03_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 10.0;
   return resu;
}
Real Src3D_03_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_03_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 10.0;
   return resu;
}
/********************************************************************/
Real Phi3D_04_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = x*(1-x)*y*(1-y);
   printf("in Phi3D_04 : z= %lf \n", z);
   return resu;
}
Real Src3D_04_Transient( Real x , Real y , Real z , Real t)
{
   /*Real resu = x + 2*y + 3*z; */
   Real resu = 0.0;
   return resu;
}
Real Sol3D_04_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = x*(1-x)*y*(1-y);
   printf("in Phi3D_04 : z= %lf \n", z);
   return resu;
}
/********************************************************************/
Real Phi3D_05_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_05_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_05_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_06_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_06_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_06_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
/********************************************************************/
Real Phi3D_07_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 4.0*x*(1.0-x)*y*(1.0-y);
   return resu;
}
Real Src3D_07_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 0.0;
   return resu;
}
Real Sol3D_07_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 4.0*x*(1.0-x)*y*(1.0-y);
   return resu;
}
/********************************************************************/
Real Phi3D_08_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 10.0/6.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5)-SQR(z-0.5) ); 
   return resu;
}
Real Src3D_08_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 10.0;
   return resu;
}
Real Sol3D_08_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = 10.0/6.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5)-SQR(z-0.5) ); 
   return resu;
}
/********************************************************************/
Real Phi3D_09_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = cos(PI*x)*sin(PI*y)*cos(PI*z);
   return resu;
}
Real Src3D_09_Transient( Real x , Real y , Real z , Real t)
{
   Real resu =  2*SQR(PI)*cos(PI*x)*sin(PI*y)*cos(PI*z);
   return resu;
}
Real Sol3D_09_Transient( Real x , Real y , Real z , Real t)
{
   Real resu = cos(PI*x)*sin(PI*y)*cos(PI*z);
   return resu;
}

