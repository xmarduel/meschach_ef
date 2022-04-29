
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////
 
#ifndef MEF_ALL_FONCTIONS_DEF__H
#define MEF_ALL_FONCTIONS_DEF__H

%constant Real Zero1D( Real x );
%constant Real Zero2D( Real x , Real y );
%constant Real Zero3D( Real x , Real y , Real z );

%constant Real Zero1Dtransient( Real x                   , Real tps );
%constant Real Zero2Dtransient( Real x , Real y          , Real tps );
%constant Real Zero3Dtransient( Real x , Real y , Real z , Real tps );


/*
 *     1-D
 *
 */

%constant Real Phi1D_00( Real x );
%constant Real Phi1D_01( Real x );
%constant Real Phi1D_02( Real x );
%constant Real Phi1D_03( Real x );
%constant Real Phi1D_04( Real x );
%constant Real Phi1D_05( Real x );
%constant Real Phi1D_06( Real x );
%constant Real Phi1D_07( Real x );
%constant Real Phi1D_08( Real x );
%constant Real Phi1D_09( Real x );

%constant Real Src1D_00( Real x );
%constant Real Src1D_01( Real x );
%constant Real Src1D_02( Real x );
%constant Real Src1D_03( Real x );
%constant Real Src1D_04( Real x );
%constant Real Src1D_05( Real x );
%constant Real Src1D_06( Real x );
%constant Real Src1D_07( Real x );
%constant Real Src1D_08( Real x );
%constant Real Src1D_09( Real x );

%constant Real Sol1D_00( Real x );
%constant Real Sol1D_01( Real x );
%constant Real Sol1D_02( Real x );
%constant Real Sol1D_03( Real x );
%constant Real Sol1D_04( Real x );
%constant Real Sol1D_05( Real x );
%constant Real Sol1D_06( Real x );
%constant Real Sol1D_07( Real x );
%constant Real Sol1D_08( Real x );
%constant Real Sol1D_09( Real x );


/*
 *     2-D
 *
 */

%constant Real Phi2D_00( Real x , Real y );
%constant Real Phi2D_01( Real x , Real y );
%constant Real Phi2D_02( Real x , Real y );
%constant Real Phi2D_03( Real x , Real y );
%constant Real Phi2D_04( Real x , Real y );
%constant Real Phi2D_05( Real x , Real y );
%constant Real Phi2D_06( Real x , Real y );
%constant Real Phi2D_07( Real x , Real y );
%constant Real Phi2D_08( Real x , Real y );
%constant Real Phi2D_09( Real x , Real y );

%constant Real Src2D_00( Real x , Real y );
%constant Real Src2D_01( Real x , Real y );
%constant Real Src2D_02( Real x , Real y );
%constant Real Src2D_03( Real x , Real y );
%constant Real Src2D_04( Real x , Real y );
%constant Real Src2D_05( Real x , Real y );
%constant Real Src2D_06( Real x , Real y );
%constant Real Src2D_07( Real x , Real y );
%constant Real Src2D_08( Real x , Real y );
%constant Real Src2D_09( Real x , Real y );

%constant Real Sol2D_00( Real x , Real y );
%constant Real Sol2D_01( Real x , Real y );
%constant Real Sol2D_02( Real x , Real y );
%constant Real Sol2D_03( Real x , Real y );
%constant Real Sol2D_04( Real x , Real y );
%constant Real Sol2D_05( Real x , Real y );
%constant Real Sol2D_06( Real x , Real y );
%constant Real Sol2D_07( Real x , Real y );
%constant Real Sol2D_08( Real x , Real y );
%constant Real Sol2D_09( Real x , Real y );

/*
 *     3-D
 *
 */

%constant Real Phi3D_00( Real x , Real y , Real z );
%constant Real Phi3D_01( Real x , Real y , Real z );
%constant Real Phi3D_02( Real x , Real y , Real z );
%constant Real Phi3D_03( Real x , Real y , Real z );
%constant Real Phi3D_04( Real x , Real y , Real z );
%constant Real Phi3D_05( Real x , Real y , Real z );
%constant Real Phi3D_06( Real x , Real y , Real z );
%constant Real Phi3D_07( Real x , Real y , Real z );
%constant Real Phi3D_08( Real x , Real y , Real z );
%constant Real Phi3D_09( Real x , Real y , Real z );

%constant Real Src3D_00( Real x , Real y , Real z );
%constant Real Src3D_01( Real x , Real y , Real z );
%constant Real Src3D_02( Real x , Real y , Real z );
%constant Real Src3D_03( Real x , Real y , Real z );
%constant Real Src3D_04( Real x , Real y , Real z );
%constant Real Src3D_05( Real x , Real y , Real z );
%constant Real Src3D_06( Real x , Real y , Real z );
%constant Real Src3D_07( Real x , Real y , Real z );
%constant Real Src3D_08( Real x , Real y , Real z );
%constant Real Src3D_09( Real x , Real y , Real z );

%constant Real Sol3D_00( Real x , Real y , Real z );
%constant Real Sol3D_01( Real x , Real y , Real z );
%constant Real Sol3D_02( Real x , Real y , Real z );
%constant Real Sol3D_03( Real x , Real y , Real z );
%constant Real Sol3D_04( Real x , Real y , Real z );
%constant Real Sol3D_05( Real x , Real y , Real z );
%constant Real Sol3D_06( Real x , Real y , Real z );
%constant Real Sol3D_07( Real x , Real y , Real z );
%constant Real Sol3D_08( Real x , Real y , Real z );
%constant Real Sol3D_09( Real x , Real y , Real z );


/*
 *     All in an Tab !
 *
 */

/*
static Real (*ExSol1D[10])(Real) =
{
  Phi1D_00 ,
  Phi1D_01 ,
  Phi1D_02 ,
  Phi1D_03 ,
  Phi1D_04 ,
  Phi1D_05 ,
  Phi1D_06 ,
  Phi1D_07 ,
  Phi1D_08 ,
  Phi1D_09 
};
 
static Real (*ExSol2D[10])(Real,Real) = 
{
  Phi2D_00 ,
  Phi2D_01 ,
  Phi2D_02 ,
  Phi2D_03 ,
  Phi2D_04 ,
  Phi2D_05 ,
  Phi2D_06 ,
  Phi2D_07 ,
  Phi2D_08 ,
  Phi2D_09 
};
  
static Real (*ExSol3D[10])(Real,Real,Real) =
{
  Phi3D_01 ,
  Phi3D_02 ,
  Phi3D_03 ,
  Phi3D_04 ,
  Phi3D_05 ,
  Phi3D_06 ,
  Phi3D_07 ,
  Phi3D_08 ,
  Phi3D_09 
};

static Real (*BCphi1D[10])(Real) =
{
  Phi1D_00 ,
  Phi1D_01 ,
  Phi1D_02 ,
  Phi1D_03 ,
  Phi1D_04 ,
  Phi1D_05 ,
  Phi1D_06 ,
  Phi1D_07 ,
  Phi1D_08 ,
  Phi1D_09
};
  
static Real (*BCphi2D[10])(Real,Real) =
{
  Phi2D_00 ,
  Phi2D_01 ,
  Phi2D_02 ,
  Phi2D_03 ,
  Phi2D_04 ,
  Phi2D_05 ,
  Phi2D_06 ,
  Phi2D_07 ,
  Phi2D_08 ,
  Phi2D_09 
};

static Real (*BCphi3D[10])(Real,Real,Real) =
{ 
   Phi3D_00 ,
   Phi3D_01 ,
   Phi3D_02 ,
   Phi3D_03 ,
   Phi3D_04 ,
   Phi3D_05 ,
   Phi3D_06 ,
   Phi3D_07 ,
   Phi3D_08 ,
   Phi3D_09 
};

static Real (*sources1D[10])(Real) =
{
   Src1D_00 ,
   Src1D_01 ,
   Src1D_02 ,
   Src1D_03 ,
   Src1D_04 ,
   Src1D_05 ,
   Src1D_06 ,
   Src1D_07 ,
   Src1D_08 ,
   Src1D_09
};

static Real (*sources2D[10])(Real,Real) =
{
   Src2D_00 ,
   Src2D_01 ,
   Src2D_02 ,
   Src2D_03 ,
   Src2D_04 ,
   Src2D_05 ,
   Src2D_06 ,
   Src2D_07 ,
   Src2D_08 ,
   Src2D_09
};

static Real (*sources3D[10])(Real,Real,Real) =
{
   Src3D_00 ,
   Src3D_01 ,
   Src3D_02 ,
   Src3D_03 ,
   Src3D_04 ,
   Src3D_05 ,
   Src3D_06 ,
   Src3D_07 ,
   Src3D_08 ,
   Src3D_09 
};
*/



/*
 *     1-D
 *
 */

%constant Real Phi1D_00_Transient( Real x , Real t );
%constant Real Phi1D_01_Transient( Real x , Real t );
%constant Real Phi1D_02_Transient( Real x , Real t );
%constant Real Phi1D_03_Transient( Real x , Real t );
%constant Real Phi1D_04_Transient( Real x , Real t );
%constant Real Phi1D_05_Transient( Real x , Real t );
%constant Real Phi1D_06_Transient( Real x , Real t );
%constant Real Phi1D_07_Transient( Real x , Real t );
%constant Real Phi1D_08_Transient( Real x , Real t );
%constant Real Phi1D_09_Transient( Real x , Real t );

%constant Real Src1D_00_Transient( Real x , Real t );
%constant Real Src1D_01_Transient( Real x , Real t );
%constant Real Src1D_02_Transient( Real x , Real t );
%constant Real Src1D_03_Transient( Real x , Real t );
%constant Real Src1D_04_Transient( Real x , Real t );
%constant Real Src1D_05_Transient( Real x , Real t );
%constant Real Src1D_06_Transient( Real x , Real t );
%constant Real Src1D_07_Transient( Real x , Real t );
%constant Real Src1D_08_Transient( Real x , Real t );
%constant Real Src1D_09_Transient( Real x , Real t );

%constant Real Sol1D_00_Transient( Real x , Real t );
%constant Real Sol1D_01_Transient( Real x , Real t );
%constant Real Sol1D_02_Transient( Real x , Real t );
%constant Real Sol1D_03_Transient( Real x , Real t );
%constant Real Sol1D_04_Transient( Real x , Real t );
%constant Real Sol1D_05_Transient( Real x , Real t );
%constant Real Sol1D_06_Transient( Real x , Real t );
%constant Real Sol1D_07_Transient( Real x , Real t );
%constant Real Sol1D_08_Transient( Real x , Real t );
%constant Real Sol1D_09_Transient( Real x , Real t );


/*
 *     2-D
 *
 */

%constant Real Phi2D_00_Transient( Real x , Real y , Real t );
%constant Real Phi2D_01_Transient( Real x , Real y , Real t );
%constant Real Phi2D_02_Transient( Real x , Real y , Real t );
%constant Real Phi2D_03_Transient( Real x , Real y , Real t );
%constant Real Phi2D_04_Transient( Real x , Real y , Real t );
%constant Real Phi2D_05_Transient( Real x , Real y , Real t );
%constant Real Phi2D_06_Transient( Real x , Real y , Real t );
%constant Real Phi2D_07_Transient( Real x , Real y , Real t );
%constant Real Phi2D_08_Transient( Real x , Real y , Real t );
%constant Real Phi2D_09_Transient( Real x , Real y , Real t );

%constant Real Src2D_00_Transient( Real x , Real y , Real t );
%constant Real Src2D_01_Transient( Real x , Real y , Real t );
%constant Real Src2D_02_Transient( Real x , Real y , Real t );
%constant Real Src2D_03_Transient( Real x , Real y , Real t );
%constant Real Src2D_04_Transient( Real x , Real y , Real t );
%constant Real Src2D_05_Transient( Real x , Real y , Real t );
%constant Real Src2D_06_Transient( Real x , Real y , Real t );
%constant Real Src2D_07_Transient( Real x , Real y , Real t );
%constant Real Src2D_08_Transient( Real x , Real y , Real t );
%constant Real Src2D_09_Transient( Real x , Real y , Real t );

%constant Real Sol2D_00_Transient( Real x , Real y , Real t );
%constant Real Sol2D_01_Transient( Real x , Real y , Real t );
%constant Real Sol2D_02_Transient( Real x , Real y , Real t );
%constant Real Sol2D_03_Transient( Real x , Real y , Real t );
%constant Real Sol2D_04_Transient( Real x , Real y , Real t );
%constant Real Sol2D_05_Transient( Real x , Real y , Real t );
%constant Real Sol2D_06_Transient( Real x , Real y , Real t );
%constant Real Sol2D_07_Transient( Real x , Real y , Real t );
%constant Real Sol2D_08_Transient( Real x , Real y , Real t );
%constant Real Sol2D_09_Transient( Real x , Real y , Real t );

/*
 *     3-D
 *
 */

%constant Real Phi3D_00_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_01_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_02_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_03_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_04_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_05_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_06_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_07_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_08_Transient( Real x , Real y , Real z , Real t );
%constant Real Phi3D_09_Transient( Real x , Real y , Real z , Real t );

%constant Real Src3D_00_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_01_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_02_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_03_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_04_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_05_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_06_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_07_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_08_Transient( Real x , Real y , Real z , Real t );
%constant Real Src3D_09_Transient( Real x , Real y , Real z , Real t );

%constant Real Sol3D_00_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_01_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_02_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_03_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_04_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_05_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_06_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_07_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_08_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_09_Transient( Real x , Real y , Real z , Real t );


/*
 *     All in an Tab !
 *
 */

/*
static Real (*ExSol1D_Transient[10])(Real, Real) =
{
  Phi1D_00_Transient ,
  Phi1D_01_Transient ,
  Phi1D_02_Transient ,
  Phi1D_03_Transient ,
  Phi1D_04_Transient ,
  Phi1D_05_Transient ,
  Phi1D_06_Transient ,
  Phi1D_07_Transient ,
  Phi1D_08_Transient ,
  Phi1D_09_Transient 
};
 
static Real (*ExSol2D_Transient[10])(Real,Real,Real) = 
{
  Phi2D_00_Transient ,
  Phi2D_01_Transient ,
  Phi2D_02_Transient ,
  Phi2D_03_Transient ,
  Phi2D_04_Transient ,
  Phi2D_05_Transient ,
  Phi2D_06_Transient ,
  Phi2D_07_Transient ,
  Phi2D_08_Transient ,
  Phi2D_09_Transient 
};
  
static Real (*ExSol3D_Transient[10])(Real,Real,Real,Real) =
{
  Phi3D_01_Transient ,
  Phi3D_02_Transient ,
  Phi3D_03_Transient ,
  Phi3D_04_Transient ,
  Phi3D_05_Transient ,
  Phi3D_06_Transient ,
  Phi3D_07_Transient ,
  Phi3D_08_Transient ,
  Phi3D_09_Transient 
};

static Real (*BCphi1D_Transient[10])(Real,Real) =
{
  Phi1D_00_Transient ,
  Phi1D_01_Transient ,
  Phi1D_02_Transient ,
  Phi1D_03_Transient ,
  Phi1D_04_Transient ,
  Phi1D_05_Transient ,
  Phi1D_06_Transient ,
  Phi1D_07_Transient ,
  Phi1D_08_Transient ,
  Phi1D_09_Transient
};
  
static Real (*BCphi2D_Transient[10])(Real,Real,Real) =
{
  Phi2D_00_Transient ,
  Phi2D_01_Transient ,
  Phi2D_02_Transient ,
  Phi2D_03_Transient ,
  Phi2D_04_Transient ,
  Phi2D_05_Transient ,
  Phi2D_06_Transient ,
  Phi2D_07_Transient ,
  Phi2D_08_Transient ,
  Phi2D_09_Transient 
};

static Real (*BCphi3D_Transient[10])(Real,Real,Real,Real) =
{ 
   Phi3D_00_Transient ,
   Phi3D_01_Transient ,
   Phi3D_02_Transient ,
   Phi3D_03_Transient ,
   Phi3D_04_Transient ,
   Phi3D_05_Transient ,
   Phi3D_06_Transient ,
   Phi3D_07_Transient ,
   Phi3D_08_Transient ,
   Phi3D_09_Transient 
};

static Real (*sources1D_Transient[10])(Real,Real) =
{
   Src1D_00_Transient ,
   Src1D_01_Transient ,
   Src1D_02_Transient ,
   Src1D_03_Transient ,
   Src1D_04_Transient ,
   Src1D_05_Transient ,
   Src1D_06_Transient ,
   Src1D_07_Transient ,
   Src1D_08_Transient ,
   Src1D_09_Transient
};

static Real (*sources2D_Transient[10])(Real,Real,Real) =
{
   Src2D_00_Transient ,
   Src2D_01_Transient ,
   Src2D_02_Transient ,
   Src2D_03_Transient ,
   Src2D_04_Transient ,
   Src2D_05_Transient ,
   Src2D_06_Transient ,
   Src2D_07_Transient ,
   Src2D_08_Transient ,
   Src2D_09_Transient
};

static Real (*sources3D_Transient[10])(Real,Real,Real,Real) =
{
   Src3D_00_Transient ,
   Src3D_01_Transient ,
   Src3D_02_Transient ,
   Src3D_03_Transient ,
   Src3D_04_Transient ,
   Src3D_05_Transient ,
   Src3D_06_Transient ,
   Src3D_07_Transient ,
   Src3D_08_Transient ,
   Src3D_09_Transient 
};
*/

#endif
