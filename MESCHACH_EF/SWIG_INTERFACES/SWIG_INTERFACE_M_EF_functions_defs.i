
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////
 
#ifndef MEF_ALL_FONCTIONS_DEF__H
#define MEF_ALL_FONCTIONS_DEF__H

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



#endif
