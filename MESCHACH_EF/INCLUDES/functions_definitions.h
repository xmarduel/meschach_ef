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

/*
 *     1-D
 *
 */

Real Phi1D_00( Real x );
Real Phi1D_01( Real x );
Real Phi1D_02( Real x );
Real Phi1D_03( Real x );
Real Phi1D_04( Real x );
Real Phi1D_05( Real x );
Real Phi1D_06( Real x );
Real Phi1D_07( Real x );
Real Phi1D_08( Real x );
Real Phi1D_09( Real x );

Real Src1D_00( Real x );
Real Src1D_01( Real x );
Real Src1D_02( Real x );
Real Src1D_03( Real x );
Real Src1D_04( Real x );
Real Src1D_05( Real x );
Real Src1D_06( Real x );
Real Src1D_07( Real x );
Real Src1D_08( Real x );
Real Src1D_09( Real x );
	
Real Sol1D_00( Real x );
Real Sol1D_01( Real x );
Real Sol1D_02( Real x );
Real Sol1D_03( Real x );
Real Sol1D_04( Real x );
Real Sol1D_05( Real x );
Real Sol1D_06( Real x );
Real Sol1D_07( Real x );
Real Sol1D_08( Real x );
Real Sol1D_09( Real x );

/*
 *     2-D
 *
 */

Real Phi2D_00( Real x , Real y );
Real Phi2D_01( Real x , Real y );
Real Phi2D_02( Real x , Real y );
Real Phi2D_03( Real x , Real y );
Real Phi2D_04( Real x , Real y );
Real Phi2D_05( Real x , Real y );
Real Phi2D_06( Real x , Real y );
Real Phi2D_07( Real x , Real y );
Real Phi2D_08( Real x , Real y );
Real Phi2D_09( Real x , Real y );

Real Src2D_00( Real x , Real y );
Real Src2D_01( Real x , Real y );
Real Src2D_02( Real x , Real y );
Real Src2D_03( Real x , Real y );
Real Src2D_04( Real x , Real y );
Real Src2D_05( Real x , Real y );
Real Src2D_06( Real x , Real y );
Real Src2D_07( Real x , Real y );
Real Src2D_08( Real x , Real y );
Real Src2D_09( Real x , Real y );
	
Real Sol2D_00( Real x , Real y );
Real Sol2D_01( Real x , Real y );
Real Sol2D_02( Real x , Real y );
Real Sol2D_03( Real x , Real y );
Real Sol2D_04( Real x , Real y );
Real Sol2D_05( Real x , Real y );
Real Sol2D_06( Real x , Real y );
Real Sol2D_07( Real x , Real y );
Real Sol2D_08( Real x , Real y );
Real Sol2D_09( Real x , Real y );

/*
 *     3-D
 *
 */

Real Phi3D_00( Real x , Real y , Real z );
Real Phi3D_01( Real x , Real y , Real z );
Real Phi3D_02( Real x , Real y , Real z );
Real Phi3D_03( Real x , Real y , Real z );
Real Phi3D_04( Real x , Real y , Real z );
Real Phi3D_05( Real x , Real y , Real z );
Real Phi3D_06( Real x , Real y , Real z );
Real Phi3D_07( Real x , Real y , Real z );
Real Phi3D_08( Real x , Real y , Real z );
Real Phi3D_09( Real x , Real y , Real z );

Real Src3D_00( Real x , Real y , Real z );
Real Src3D_01( Real x , Real y , Real z );
Real Src3D_02( Real x , Real y , Real z );
Real Src3D_03( Real x , Real y , Real z );
Real Src3D_04( Real x , Real y , Real z );
Real Src3D_05( Real x , Real y , Real z );
Real Src3D_06( Real x , Real y , Real z );
Real Src3D_07( Real x , Real y , Real z );
Real Src3D_08( Real x , Real y , Real z );
Real Src3D_09( Real x , Real y , Real z );
	
Real Sol3D_00( Real x , Real y , Real z );
Real Sol3D_01( Real x , Real y , Real z );
Real Sol3D_02( Real x , Real y , Real z );
Real Sol3D_03( Real x , Real y , Real z );
Real Sol3D_04( Real x , Real y , Real z );
Real Sol3D_05( Real x , Real y , Real z );
Real Sol3D_06( Real x , Real y , Real z );
Real Sol3D_07( Real x , Real y , Real z );
Real Sol3D_08( Real x , Real y , Real z );
Real Sol3D_09( Real x , Real y , Real z );

/*
 *     1-D
 *
 */

Real Phi1D_00_Transient( Real x , Real t );
Real Phi1D_01_Transient( Real x , Real t );
Real Phi1D_02_Transient( Real x , Real t );
Real Phi1D_03_Transient( Real x , Real t );
Real Phi1D_04_Transient( Real x , Real t );
Real Phi1D_05_Transient( Real x , Real t );
Real Phi1D_06_Transient( Real x , Real t );
Real Phi1D_07_Transient( Real x , Real t );
Real Phi1D_08_Transient( Real x , Real t );
Real Phi1D_09_Transient( Real x , Real t );

Real Src1D_00_Transient( Real x , Real t );
Real Src1D_01_Transient( Real x , Real t );
Real Src1D_02_Transient( Real x , Real t );
Real Src1D_03_Transient( Real x , Real t );
Real Src1D_04_Transient( Real x , Real t );
Real Src1D_05_Transient( Real x , Real t );
Real Src1D_06_Transient( Real x , Real t );
Real Src1D_07_Transient( Real x , Real t );
Real Src1D_08_Transient( Real x , Real t );
Real Src1D_09_Transient( Real x , Real t );
	
Real Sol1D_00_Transient( Real x , Real t );
Real Sol1D_01_Transient( Real x , Real t );
Real Sol1D_02_Transient( Real x , Real t );
Real Sol1D_03_Transient( Real x , Real t );
Real Sol1D_04_Transient( Real x , Real t );
Real Sol1D_05_Transient( Real x , Real t );
Real Sol1D_06_Transient( Real x , Real t );
Real Sol1D_07_Transient( Real x , Real t );
Real Sol1D_08_Transient( Real x , Real t );
Real Sol1D_09_Transient( Real x , Real t );

/*
 *     2-D
 *
 */

Real Phi2D_00_Transient( Real x , Real y , Real t );
Real Phi2D_01_Transient( Real x , Real y , Real t );
Real Phi2D_02_Transient( Real x , Real y , Real t );
Real Phi2D_03_Transient( Real x , Real y , Real t );
Real Phi2D_04_Transient( Real x , Real y , Real t );
Real Phi2D_05_Transient( Real x , Real y , Real t );
Real Phi2D_06_Transient( Real x , Real y , Real t );
Real Phi2D_07_Transient( Real x , Real y , Real t );
Real Phi2D_08_Transient( Real x , Real y , Real t );
Real Phi2D_09_Transient( Real x , Real y , Real t );

Real Src2D_00_Transient( Real x , Real y , Real t );
Real Src2D_01_Transient( Real x , Real y , Real t );
Real Src2D_02_Transient( Real x , Real y , Real t );
Real Src2D_03_Transient( Real x , Real y , Real t );
Real Src2D_04_Transient( Real x , Real y , Real t );
Real Src2D_05_Transient( Real x , Real y , Real t );
Real Src2D_06_Transient( Real x , Real y , Real t );
Real Src2D_07_Transient( Real x , Real y , Real t );
Real Src2D_08_Transient( Real x , Real y , Real t );
Real Src2D_09_Transient( Real x , Real y , Real t );
	
Real Sol2D_00_Transient( Real x , Real y , Real t );
Real Sol2D_01_Transient( Real x , Real y , Real t );
Real Sol2D_02_Transient( Real x , Real y , Real t );
Real Sol2D_03_Transient( Real x , Real y , Real t );
Real Sol2D_04_Transient( Real x , Real y , Real t );
Real Sol2D_05_Transient( Real x , Real y , Real t );
Real Sol2D_06_Transient( Real x , Real y , Real t );
Real Sol2D_07_Transient( Real x , Real y , Real t );
Real Sol2D_08_Transient( Real x , Real y , Real t );
Real Sol2D_09_Transient( Real x , Real y , Real t );

/*
 *     3-D
 *
 */

Real Phi3D_00_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_01_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_02_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_03_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_04_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_05_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_06_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_07_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_08_Transient( Real x , Real y , Real z , Real t );
Real Phi3D_09_Transient( Real x , Real y , Real z , Real t );

Real Src3D_00_Transient( Real x , Real y , Real z , Real t );
Real Src3D_01_Transient( Real x , Real y , Real z , Real t );
Real Src3D_02_Transient( Real x , Real y , Real z , Real t );
Real Src3D_03_Transient( Real x , Real y , Real z , Real t );
Real Src3D_04_Transient( Real x , Real y , Real z , Real t );
Real Src3D_05_Transient( Real x , Real y , Real z , Real t );
Real Src3D_06_Transient( Real x , Real y , Real z , Real t );
Real Src3D_07_Transient( Real x , Real y , Real z , Real t );
Real Src3D_08_Transient( Real x , Real y , Real z , Real t );
Real Src3D_09_Transient( Real x , Real y , Real z , Real t );

Real Sol3D_00_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_01_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_02_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_03_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_04_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_05_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_06_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_07_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_08_Transient( Real x , Real y , Real z , Real t );
Real Sol3D_09_Transient( Real x , Real y , Real z , Real t );
	


static FUNC_1D_NAMEMAP BC_FUNC_1D_NAMEMAP[] = 
{
	{ "Phi1D_00" , Phi1D_00 },
   { "Phi1D_01" , Phi1D_01 },
	{ "Phi1D_02" , Phi1D_02 },
	{ "Phi1D_03" , Phi1D_03 },
	{ "Phi1D_04" , Phi1D_04 },
	{ "Phi1D_05" , Phi1D_05 },
	{ "Phi1D_06" , Phi1D_06 },
	{ "Phi1D_07" , Phi1D_07 },
	{ "Phi1D_08" , Phi1D_08 },
	{ "Phi1D_09" , Phi1D_09 },
	{ "NULL"     , NULL     },
};
   
static FUNC_1D_NAMEMAP ADV_FUNC_1D_NAMEMAP[] = 
{
	{ "Phi1D_00" , Phi1D_00 },
	{ "Phi1D_01" , Phi1D_01 },
	{ "Phi1D_02" , Phi1D_02 },
	{ "Phi1D_03" , Phi1D_03 },
	{ "Phi1D_04" , Phi1D_04 },
	{ "Phi1D_05" , Phi1D_05 },
	{ "Phi1D_06" , Phi1D_06 },
	{ "Phi1D_07" , Phi1D_07 },
	{ "Phi1D_08" , Phi1D_08 },
	{ "Phi1D_09" , Phi1D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_1D_NAMEMAP RHS_FUNC_1D_NAMEMAP[] = 
{
	{ "Src1D_00" , Src1D_00 },
	{ "Src1D_01" , Src1D_01 },
	{ "Src1D_02" , Src1D_02 },
	{ "Src1D_03" , Src1D_03 },
	{ "Src1D_04" , Src1D_04 },
	{ "Src1D_05" , Src1D_05 },
	{ "Src1D_06" , Src1D_06 },
	{ "Src1D_07" , Src1D_07 },
	{ "Src1D_08" , Src1D_08 },
	{ "Src1D_09" , Src1D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_1D_NAMEMAP SOL_FUNC_1D_NAMEMAP[] = 
{
	{ "Sol1D_00" , Sol1D_00 },
	{ "Sol1D_01" , Sol1D_01 },
	{ "Sol1D_02" , Sol1D_02 },
	{ "Sol1D_03" , Sol1D_03 },
	{ "Sol1D_04" , Sol1D_04 },
	{ "Sol1D_05" , Sol1D_05 },
	{ "Sol1D_06" , Sol1D_06 },
	{ "Sol1D_07" , Sol1D_07 },
	{ "Sol1D_08" , Sol1D_08 },
	{ "Sol1D_09" , Sol1D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_2D_NAMEMAP BC_FUNC_2D_NAMEMAP[] = 
{
	{ "Phi2D_00" , Phi2D_00 },
	{ "Phi2D_01" , Phi2D_01 },
	{ "Phi2D_02" , Phi2D_02 },
	{ "Phi2D_03" , Phi2D_03 },
	{ "Phi2D_04" , Phi2D_04 },
	{ "Phi2D_05" , Phi2D_05 },
	{ "Phi2D_06" , Phi2D_06 },
	{ "Phi2D_07" , Phi2D_07 },
	{ "Phi2D_08" , Phi2D_08 },
	{ "Phi2D_09" , Phi2D_09 },
	{ "NULL"     , NULL     },
};
   
static	FUNC_2D_NAMEMAP ADV_FUNC_2D_NAMEMAP[] = 
{
	{ "Phi2D_00" , Phi2D_00 },
	{ "Phi2D_01" , Phi2D_01 },
	{ "Phi2D_02" , Phi2D_02 },
	{ "Phi2D_03" , Phi2D_03 },
	{ "Phi2D_04" , Phi2D_04 },
	{ "Phi2D_05" , Phi2D_05 },
	{ "Phi2D_06" , Phi2D_06 },
	{ "Phi2D_07" , Phi2D_07 },
	{ "Phi2D_08" , Phi2D_08 },
	{ "Phi2D_09" , Phi2D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_2D_NAMEMAP RHS_FUNC_2D_NAMEMAP[] = 
{
	{ "Src2D_00" , Src2D_00 },
	{ "Src2D_01" , Src2D_01 },
	{ "Src2D_02" , Src2D_02 },
	{ "Src2D_03" , Src2D_03 },
	{ "Src2D_04" , Src2D_04 },
	{ "Src2D_05" , Src2D_05 },
	{ "Src2D_06" , Src2D_06 },
	{ "Src2D_07" , Src2D_07 },
	{ "Src2D_08" , Src2D_08 },
	{ "Src2D_09" , Src2D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_2D_NAMEMAP SOL_FUNC_2D_NAMEMAP[] = 
{
	{ "Sol2D_00" , Sol2D_00 },
	{ "Sol2D_01" , Sol2D_01 },
	{ "Sol2D_02" , Sol2D_02 },
	{ "Sol2D_03" , Sol2D_03 },
	{ "Sol2D_04" , Sol2D_04 },
	{ "Sol2D_05" , Sol2D_05 },
	{ "Sol2D_06" , Sol2D_06 },
	{ "Sol2D_07" , Sol2D_07 },
	{ "Sol2D_08" , Sol2D_08 },
	{ "Sol2D_09" , Sol2D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_3D_NAMEMAP BC_FUNC_3D_NAMEMAP[] = 
{
	{ "Phi3D_00" , Phi3D_00 },
	{ "Phi3D_01" , Phi3D_01 },
	{ "Phi3D_02" , Phi3D_02 },
	{ "Phi3D_03" , Phi3D_03 },
	{ "Phi3D_04" , Phi3D_04 },
	{ "Phi3D_05" , Phi3D_05 },
	{ "Phi3D_06" , Phi3D_06 },
	{ "Phi3D_07" , Phi3D_07 },
	{ "Phi3D_08" , Phi3D_08 },
	{ "Phi3D_09" , Phi3D_09 },
	{ "NULL"     , NULL     },
};
   
static	FUNC_3D_NAMEMAP ADV_FUNC_3D_NAMEMAP[] = 
{
	{ "Phi3D_00" , Phi3D_00 },
	{ "Phi3D_01" , Phi3D_01 },
	{ "Phi3D_02" , Phi3D_02 },
	{ "Phi3D_03" , Phi3D_03 },
	{ "Phi3D_04" , Phi3D_04 },
	{ "Phi3D_05" , Phi3D_05 },
	{ "Phi3D_06" , Phi3D_06 },
	{ "Phi3D_07" , Phi3D_07 },
	{ "Phi3D_08" , Phi3D_08 },
	{ "Phi3D_09" , Phi3D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_3D_NAMEMAP RHS_FUNC_3D_NAMEMAP[] = 
{
	{ "Src3D_00" , Src3D_00 },
	{ "Src3D_01" , Src3D_01 },
	{ "Src3D_02" , Src3D_02 },
	{ "Src3D_03" , Src3D_03 },
	{ "Src3D_04" , Src3D_04 },
	{ "Src3D_05" , Src3D_05 },
	{ "Src3D_06" , Src3D_06 },
	{ "Src3D_07" , Src3D_07 },
	{ "Src3D_08" , Src3D_08 },
	{ "Src3D_09" , Src3D_09 },
	{ "NULL"     , NULL     },
};
	
static	FUNC_3D_NAMEMAP SOL_FUNC_3D_NAMEMAP[] = 
{
	{ "Sol3D_00" , Sol3D_00 },
	{ "Sol3D_01" , Sol3D_01 },
	{ "Sol3D_02" , Sol3D_02 },
	{ "Sol3D_03" , Sol3D_03 },
	{ "Sol3D_04" , Sol3D_04 },
	{ "Sol3D_05" , Sol3D_05 },
	{ "Sol3D_06" , Sol3D_06 },
	{ "Sol3D_07" , Sol3D_07 },
	{ "Sol3D_08" , Sol3D_08 },
	{ "Sol3D_09" , Sol3D_09 },
	{ "NULL"     , NULL     },
};

/*---------------------------------------------------------------------- */
/*---------------------------------------------------------------------- */

#ifdef __cplusplus
}
#endif

#endif
