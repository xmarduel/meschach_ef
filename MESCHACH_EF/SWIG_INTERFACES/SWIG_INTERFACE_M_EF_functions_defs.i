
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////
 
#ifndef MEF_ALL_FONCTIONS_DEF__H
#define MEF_ALL_FONCTIONS_DEF__H


%constant Real Zero1D          ( Real x );
%constant Real Zero1D_Transient( Real x, Real t );
%constant Real Zero2D          ( Real x, Real y );
%constant Real Zero2D_Transient( Real x, Real y, Real t );
%constant Real Zero3D          ( Real x, Real y, Real z );
%constant Real Zero3D_Transient( Real x, Real y, Real z, Real t );

/*
 *     1-D
 */

%constant Real Rhs1D_01( Real x );
%constant Real Rhs1D_02( Real x );
%constant Real Rhs1D_03( Real x );
%constant Real Rhs1D_04( Real x );
%constant Real Rhs1D_05( Real x );

%constant Real Sol1D_01( Real x );
%constant Real Sol1D_02( Real x );
%constant Real Sol1D_03( Real x );
%constant Real Sol1D_04( Real x );
%constant Real Sol1D_05( Real x );

/*
 *     2-D
 */

%constant Real Rhs2D_01( Real x , Real y );
%constant Real Rhs2D_02( Real x , Real y );
%constant Real Rhs2D_03( Real x , Real y );
%constant Real Rhs2D_04( Real x , Real y );
%constant Real Rhs2D_05( Real x , Real y );

%constant Real Sol2D_01( Real x , Real y );
%constant Real Sol2D_02( Real x , Real y );
%constant Real Sol2D_03( Real x , Real y );
%constant Real Sol2D_04( Real x , Real y );
%constant Real Sol2D_05( Real x , Real y );

/*
 *     3-D
 */

%constant Real Rhs3D_01( Real x , Real y , Real z );
%constant Real Rhs3D_02( Real x , Real y , Real z );
%constant Real Rhs3D_03( Real x , Real y , Real z );
%constant Real Rhs3D_04( Real x , Real y , Real z );
%constant Real Rhs3D_05( Real x , Real y , Real z );

%constant Real Sol3D_01( Real x , Real y , Real z );
%constant Real Sol3D_02( Real x , Real y , Real z );
%constant Real Sol3D_03( Real x , Real y , Real z );
%constant Real Sol3D_04( Real x , Real y , Real z );
%constant Real Sol3D_05( Real x , Real y , Real z );

/*
 *     1-D
 */

%constant Real Rhs1D_01_Transient( Real x , Real t );
%constant Real Rhs1D_02_Transient( Real x , Real t );
%constant Real Rhs1D_03_Transient( Real x , Real t );
%constant Real Rhs1D_04_Transient( Real x , Real t );
%constant Real Rhs1D_05_Transient( Real x , Real t );

%constant Real Sol1D_01_Transient( Real x , Real t );
%constant Real Sol1D_02_Transient( Real x , Real t );
%constant Real Sol1D_03_Transient( Real x , Real t );
%constant Real Sol1D_04_Transient( Real x , Real t );
%constant Real Sol1D_05_Transient( Real x , Real t );

/*
 *     2-D
 */

%constant Real Rhs2D_01_Transient( Real x , Real y , Real t );
%constant Real Rhs2D_02_Transient( Real x , Real y , Real t );
%constant Real Rhs2D_03_Transient( Real x , Real y , Real t );
%constant Real Rhs2D_04_Transient( Real x , Real y , Real t );
%constant Real Rhs2D_05_Transient( Real x , Real y , Real t );

%constant Real Sol2D_01_Transient( Real x , Real y , Real t );
%constant Real Sol2D_02_Transient( Real x , Real y , Real t );
%constant Real Sol2D_03_Transient( Real x , Real y , Real t );
%constant Real Sol2D_04_Transient( Real x , Real y , Real t );
%constant Real Sol2D_05_Transient( Real x , Real y , Real t );

/*
 *     3-D
 */

%constant Real Rhs3D_01_Transient( Real x , Real y , Real z , Real t );
%constant Real Rhs3D_02_Transient( Real x , Real y , Real z , Real t );
%constant Real Rhs3D_03_Transient( Real x , Real y , Real z , Real t );
%constant Real Rhs3D_04_Transient( Real x , Real y , Real z , Real t );
%constant Real Rhs3D_05_Transient( Real x , Real y , Real z , Real t );

%constant Real Sol3D_01_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_02_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_03_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_04_Transient( Real x , Real y , Real z , Real t );
%constant Real Sol3D_05_Transient( Real x , Real y , Real z , Real t );


#endif
