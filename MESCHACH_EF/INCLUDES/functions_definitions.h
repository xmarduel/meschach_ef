#ifndef MEF_ALL_FONCTIONS_DEF__H
#define MEF_ALL_FONCTIONS_DEF__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file functions_definitions.h
 *
 * Contains many "C" functions, useful for our problems
 *
 * --------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/machine.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"

/*---------------------------------------------------------------------- */
/*---------------------------------------------------------------------- */

Real Zero1D(Real x);
Real Zero2D(Real x, Real y);
Real Zero3D(Real x, Real y, Real z);
   
Real Zero1D_Transient(Real x, Real t);
Real Zero2D_Transient(Real x, Real y, Real t);
Real Zero3D_Transient(Real x, Real y, Real z, Real t);
   
/*
 *     1-D
 */

Real Rhs1D_01( Real x );
Real Rhs1D_02( Real x );
Real Rhs1D_03( Real x );
Real Rhs1D_04( Real x );
Real Rhs1D_05( Real x );

Real Sol1D_01( Real x );
Real Sol1D_02( Real x );
Real Sol1D_03( Real x );
Real Sol1D_04( Real x );
Real Sol1D_05( Real x );

/*
 *     2-D
 */

Real Rhs2D_01( Real x , Real y );
Real Rhs2D_02( Real x , Real y );
Real Rhs2D_03( Real x , Real y );
Real Rhs2D_04( Real x , Real y );
Real Rhs2D_05( Real x , Real y );

Real Sol2D_01( Real x , Real y );
Real Sol2D_02( Real x , Real y );
Real Sol2D_03( Real x , Real y );
Real Sol2D_04( Real x , Real y );
Real Sol2D_05( Real x , Real y );

/*
 *     3-D
 */

Real Rhs3D_01( Real x , Real y , Real z );
Real Rhs3D_02( Real x , Real y , Real z );
Real Rhs3D_03( Real x , Real y , Real z );
Real Rhs3D_04( Real x , Real y , Real z );
Real Rhs3D_05( Real x , Real y , Real z );

Real Sol3D_01( Real x , Real y , Real z );
Real Sol3D_02( Real x , Real y , Real z );
Real Sol3D_03( Real x , Real y , Real z );
Real Sol3D_04( Real x , Real y , Real z );
Real Sol3D_05( Real x , Real y , Real z );

/*
 *     1-D Transient
 */

Real Rhs1D_01_Transient( Real x , Real t );
Real Rhs1D_02_Transient( Real x , Real t );
Real Rhs1D_03_Transient( Real x , Real t );
Real Rhs1D_04_Transient( Real x , Real t );
Real Rhs1D_05_Transient( Real x , Real t );

Real Sol1D_01_Transient( Real x , Real t );
Real Sol1D_02_Transient( Real x , Real t );
Real Sol1D_03_Transient( Real x , Real t );
Real Sol1D_04_Transient( Real x , Real t );
Real Sol1D_05_Transient( Real x , Real t );

/*
 *     2-D Transient
 */

Real Rhs2D_01_Transient( Real x , Real y , Real t );
Real Rhs2D_02_Transient( Real x , Real y , Real t );
Real Rhs2D_03_Transient( Real x , Real y , Real t );
Real Rhs2D_04_Transient( Real x , Real y , Real t );
Real Rhs2D_05_Transient( Real x , Real y , Real t );

Real Sol2D_01_Transient( Real x , Real y , Real t );
Real Sol2D_02_Transient( Real x , Real y , Real t );
Real Sol2D_03_Transient( Real x , Real y , Real t );
Real Sol2D_04_Transient( Real x , Real y , Real t );
Real Sol2D_05_Transient( Real x , Real y , Real t );

/*
 *     3-D Transient
 */

Real Rhs3D_01_Transient( Real x , Real y , Real z , Real t );
Real Rhs3D_02_Transient( Real x , Real y , Real z , Real t );
Real Rhs3D_03_Transient( Real x , Real y , Real z , Real t );
Real Rhs3D_04_Transient( Real x , Real y , Real z , Real t );
Real Rhs3D_05_Transient( Real x , Real y , Real z , Real t );

Real Sol3D_01_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_02_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_03_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_04_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_05_Transient( Real x , Real y , Real z , Real t );

/*---------------------------------------------------------------------- */
/*---------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
