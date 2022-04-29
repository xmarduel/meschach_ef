
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/problems_functions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"


#define  SQR(x)  (x)*(x)
#define  CUB(x)  (x)*(x)*(x)

#define  PI      acos(-1.0)



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

Real Phi1D_06(Real x) /* is (Phi1D_05)'(0) */
{
   Real resu =  -1.0;
   return resu ;
}
Real Src1D_06(Real x)
{
   Real resu;
   return resu = 0.0; 
}

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

Real Phi1D_08(Real x)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) ); 
}
Real Src1D_08(Real x)
{
   Real resu;
   //return resu = 10.0;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5) );
}

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

Real Phi2D_04(Real x,Real y)
{
   Real resu;
   return resu = x + 2*y;
}
Real Src2D_04(Real x,Real y)
{
   Real resu;
   //return resu = 0.0; 
   return resu = x + 2*y; 
}

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

Real Phi2D_08(Real x,Real y)
{
   Real resu;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) ); 
}
Real Src2D_08(Real x,Real y)
{
   Real resu;
   //return resu = 10.0;
   return resu = 10/4.0*(1.0/2.0-SQR(x-0.5)-SQR(y-0.5) );
}

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

Real Phi3D_01( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_01( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}

Real Phi3D_02( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_02( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}

Real Phi3D_03( Real x , Real y , Real z )
{
   Real resu = 10.0;
   return resu;
}
Real Src3D_03( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}

Real Phi3D_04( Real x , Real y , Real z )
{
   Real resu = x + 2*y + 3*z;
   return resu;
}
Real Src3D_04( Real x , Real y , Real z )
{
   //Real resu = x + 2*y + 3*z; // for tests
   Real resu = 0.0;
   return resu;
}

Real Phi3D_05( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_05( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}

Real Phi3D_06( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_06( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}

Real Phi3D_07( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}
Real Src3D_07( Real x , Real y , Real z )
{
   Real resu = 0.0;
   return resu;
}

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





