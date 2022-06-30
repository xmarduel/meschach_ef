
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

#define  SQR(x)  (x)*(x)
#define  CUB(x)  (x)*(x)*(x)

#define  PI      acos(-1.0)

/**
* Contains the definitions of standart functions
*/

/********************************************************************/
/********************************************************************/

Real Zero1D(Real x)
{
   return 0.0;
}
Real Zero2D(Real x, Real y)
{
   return 0.0;
}
Real Zero3D(Real x, Real y, Real z)
{
   return 0.0;
}


Real Zero1D_Transient(Real x, Real t)
{
   return 0.0;
}
Real Zero2D_Transient(Real x, Real y, Real t)
{
   return 0.0;
}
Real Zero3D_Transient(Real x, Real y, Real z, Real t)
{
   return 0.0;
}

/********************************************************************/
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
   //return resu = (x-0.5)*(x-0.5)+2.0;
   return resu = x*(x-1) + 2.0;
}
Real Src1D_05(Real x)
{
   Real resu;
   return resu = -2.0;
}
Real Sol1D_05(Real x)
{
   Real resu;
   //return resu = (x-0.5)*(x-0.5)+2.0;
   return resu = x*(x-1) +2;
}

/****************************************************************************/
/****************************************************************************/

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

/***************************************************************************/
/***************************************************************************/

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
   //return resu = 0.0;
   return resu = -2.0;
}
Real Sol1D_05_Transient(Real x, Real t)
{
   Real resu;
   return resu = (x-0.5)*(x-0.5)+2.0;
}

/****************************************************************************/
/****************************************************************************/

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

/***************************************************************************/
/***************************************************************************/

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


