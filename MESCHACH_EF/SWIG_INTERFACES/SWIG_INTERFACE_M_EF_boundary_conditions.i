
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  
 

#ifndef MEF_BOUNDARY_CONDITIONS_H
#define MEF_BOUNDARY_CONDITIONS_H

#define MY_LIST4   4

#define TYPE_BC_1D    0
#define TYPE_BC_2D    1
#define TYPE_BC_3D    2

#define NB_STRUCTS4   3


#include "matrix.h"

#include "boundary_conditions_1D.h"
#include "boundary_conditions_2D.h"
#include "boundary_conditions_3D.h"




#define  NB_ERRS4                   10
#define  NB_WARNS4                   7

//static  char *my_err_mesg4[NB_ERRS4] =
//{
//   "unknown bc error",                                /* 0 */
//   "boundary condition incompatible with geometry",   /* 1 */
//   "too many boundary conditions in lecture",         /* 2 */
//   "index in boundary conditions too big",            /* 3 */
//   "axe in boundary conditions wrong",                /* 4 */
//   "boundary condition not set on reference index",   /* 5 */
//   "dirichlet function not set on a ref'ed somm",     /* 6 */
//   "neumann function not set on a ref'ed somm",       /* 7 */
//   "cauchy function not set on a ref'ed somm",        /* 8 */
//   "robin function not set on a ref'ed somm"          /* 9 */
//};

//static char *my_warn_mesg4[NB_WARNS4] = 
//{
//   "unknown bc warning"	, 			/* 0 */
//   "more boundary functions than refs on somms",      /* 1 */
//   "mask of bc set not initialized on a reference",   /* 2 */
//   "mask of bc set on a reference but no function set on this mask",  /* 3 */
//   "second function for dirichlet BC is never used",  /* 4 */
//   "second function for neumann BC is never used",    /* 5 */
//   "second function for cauchy BC is never used"      /* 6 */
//};

#define  E_BC_UNKNOWN	            0
#define  E_BC_INCOMPATIBLE          1
#define  E_BC_NBBOUNDARYCONDITIONS  2
#define  E_BC_WRONGIDXNUMBER        3
#define  E_BC_WRONGAXENUMBER        4
#define  E_BC_WRONGBCTYPE           5
#define  E_BC_DIRICHLETFUNNOTSET    6
#define  E_BC_NEUMANNFUNNOTSET      7
#define  E_BC_CAUCHYFUNNOTSET       8
#define  E_BC_ROBINFUNNOTSET        9

#define  W_BC_UNKNOWN               0
#define  W_BC_TOOMANYFUNCTIONS      1
#define  W_BC_TYPENOTSET            2
#define  W_BC_MASKSETBUTNOFUNCTION  3
#define  W_BC_SECONDFUNCDIRICHLET   4
#define  W_BC_SECONDFUNCNEUMANN     5
#define  W_BC_SECONDFUNCCAUCHY      6

/******************************************************************************/

int mem_attach_list4(void);
int err_attach_list4(void);
int warn_attach_list4(void);
int err_warn_attach_lists4(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error4(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,4)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning4(err_num,fn_name) ev_warn(__FILE__,err_num,__LINE__,fn_name,4)

#endif



#ifndef MEF_BOUNDARY_CONDITIONS_1D_H
#define MEF_BOUNDARY_CONDITIONS_1D_H


#include "matrix.h"

#include "functions_structs.h"
#include "geometry_1D.h"

/*------------------------------------------------------------------------- */

typedef enum {

   BC_1De_UNKNOWN   = -1,

   BC_1De_DIRICHLET =  0,
   BC_1De_NEUMANN   =  1,
   BC_1De_CAUCHY    =  2,
   BC_1De_ROBIN     =  3

} BC_1D_TYPE;

//typedef struct {
//
//   BC_1D_TYPE  bc_type;                    /**< type of bc : dirichlet, neumann, cauchy or robin  */
//  
//   FUN_1D Fun1[1][NBMAX_BC_1D_FUNCTIONS];  /* nb_bc_dirichlet+1 functions on the border    */
//   FUN_1D Fun2[1][NBMAX_BC_1D_FUNCTIONS];  /* nb_bc_dirichlet+1 functions on the border    */
// 
//   Real tps;
//
//   int current_working_axe;
//   
//} BC_1D_XTYPE ;            /* to INITIALIZE in "BCdirichlet_get"           */


typedef struct {

// BC_1D_XTYPE* BC_dirichlet;
// BC_1D_XTYPE* BC_neumann;
// BC_1D_XTYPE* BC_cauchy;
// BC_1D_XTYPE* BC_robin;

// BC_1D_TYPE   BC_MASK[1][NBMAX_BC_1D_FUNCTIONS];
   
} BC_1D ;


// BC_1D* BoundaryConditions1D_check_with_geom( BC_1D *BC, GEOM_1D *Geom);

#define BC1D_FREE(MyBC) ( Bc1D_free(MyBC), (MyBC)=(BC_1D *)NULL )


BC_1D      *Bc1D_get(void);

int         Bc1D_free(BC_1D  *BC);

BC_1D      *Bc1D_setBcType             ( BC_1D* BC, BC_1D_TYPE bctype, int ref, int axe );
BC_1D_TYPE  Bc1D_getBcType             ( BC_1D* BC, int axe, int ref);

BC_1D      *Bc1D_setFunction1           ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , FUN_TYPE type, void* phi, void* clientdata);
BC_1D      *Bc1D_setFunction2           ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , FUN_TYPE type, void* phi, void* clientdata);

BC_1D      *Bc1D_setCFunction           ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_1D phi);
BC_1D      *Bc1D_setCFunction2          ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_1D phi);
BC_1D      *Bc1D_setCFunctionTransient  ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_2D phi);
BC_1D      *Bc1D_setCFunctionTransient2 ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , FUNC_2D phi);

Real        Bc1D_evalFunction1          ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , Real x);
Real        Bc1D_evalFunction2          ( BC_1D* BC, BC_1D_TYPE bctype, int ref_s, int axe , Real x);

BC_1D      *Bc1D_setTps                ( BC_1D *BC, Real tps       );

int         Bc1D_checkBC               ( const BC_1D* BC );

int         Bc1D_hasDirichletBC        ( const BC_1D* BC , const int axe);
int         Bc1D_hasNeumannBC          ( const BC_1D* BC , const int axe);
int         Bc1D_hasCauchyBC           ( const BC_1D* BC , const int axe);

int         Bc1D_getNumberOfBC         ( const BC_1D* BC , const int axe);
int         Bc1D_getNumberOfBcOnSomms  ( const BC_1D* BC , const int axe);
int         Bc1D_getNumberOfBcOnFaces  ( const BC_1D* BC , const int axe);

#endif



#ifndef MEF_BOUNDARY_CONDITIONS_2D_H
#define MEF_BOUNDARY_CONDITIONS_2D_H


#include "matrix.h"

#include "functions_structs.h"
#include "geometry_2D.h"

/*---------------------------------------------------------------------- */

typedef enum {

   BC_2De_UNKNOWN   = -1,

   BC_2De_DIRICHLET =  0,
   BC_2De_NEUMANN   =  1,
   BC_2De_CAUCHY    =  2,
   BC_2De_ROBIN     =  3

} BC_2D_TYPE;


//typedef struct {
//
//   BC_2D_TYPE  bc_type;                    /**< type of bc : dirichlet, neumann, cauchy or robin  */
  
//   FUN_2D Fun1[2][NBMAX_BC_2D_FUNCTIONS];         /* nb_bc_dirichlet+1 functions on the border    */
//   FUN_2D Fun2[2][NBMAX_BC_2D_FUNCTIONS];         /* nb_bc_dirichlet+1 functions on the border    */

//   Real tps;

//   int  current_selected_axe;
   
//} BC_2D_XTYPE ;            /* to INITIALIZE in "BCdirichlet_get"           */


typedef struct {

// BC_2D_XTYPE* BC_dirichlet;
// BC_2D_XTYPE* BC_neumann;
// BC_2D_XTYPE* BC_cauchy;
// BC_2D_XTYPE* BC_robin;

// BC_2D_TYPE   BC_MASK[2][NBMAX_BC_2D_FUNCTIONS];

} BC_2D ;



#define BC2D_FREE(MyBC) ( Bc2D_free(MyBC), (MyBC)=(BC_2D *)NULL )


BC_2D      *Bc2D_get(void);

int         Bc2D_free(BC_2D  *BC);

BC_2D      *Bc2D_setBcType             ( BC_2D* BC, BC_2D_TYPE bctype, int ref, int axe );
BC_2D_TYPE  Bc2D_getBcType             ( BC_2D* BC, int axe, int ref);

BC_2D      *Bc2D_setFunction1          ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , FUN_TYPE type, void* phi, void* clientdata);
BC_2D      *Bc2D_setFunction2          ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , FUN_TYPE type, void* phi, void* clientdata);

BC_2D      *Bc2D_setCFunction          ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_2D phi);
BC_2D      *Bc2D_setCFunction2         ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_2D phi);

BC_2D      *Bc2D_setCFunctionTransient ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_3D phi);
BC_2D      *Bc2D_setCFunctionTransient2( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , FUNC_3D phi);

Real        Bc2D_evalFunction1         ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , Real x, Real y);
Real        Bc2D_evalFunction2         ( BC_2D* BC, BC_2D_TYPE bctype, int ref_s, int axe , Real x, Real y);

BC_2D      *Bc2D_setTps               ( BC_2D *MyBC, Real tps       );
BC_2D      *Bc2D_setCurrentSelectedAxe( BC_2D *MyBC, int  axe       );

int         Bc2D_checkBC              ( const BC_2D* BC , const int axe);

int         Bc2D_hasDirichletBC       ( const BC_2D* BC , const int axe);
int         Bc2D_hasNeumannBC         ( const BC_2D* BC , const int axe);
int         Bc2D_hasCauchyBC          ( const BC_2D* BC , const int axe);
int         Bc2D_hasRobinBC           ( const BC_2D* BC , const int axe);

int         Bc2D_getNumberOfBC        ( const BC_2D* BC , const int axe);
int         Bc2D_getNumberOfBcOnSomms ( const BC_2D* BC , const int axe);
int         Bc2D_getNumberOfBcOnFaces ( const BC_2D* BC , const int axe);

#endif



#ifndef MEF_BOUNDARY_CONDITIONS_3D_H
#define MEF_BOUNDARY_CONDITIONS_3D_H


#include "matrix.h"

#include "functions_structs.h"
#include "geometry_3D.h"

/*---------------------------------------------------------------------- */

typedef enum {

   BC_3De_UNKNOWN   = -1,

   BC_3De_DIRICHLET =  0,
   BC_3De_NEUMANN   =  1,
   BC_3De_CAUCHY    =  2,
   BC_3De_ROBIN     =  3

} BC_3D_TYPE;

//typedef struct {

//   BC_3D_TYPE  bc_type;                    /**< type of bc : dirichlet, neumann, cauchy or robin  */
  
//   FUN_3D Fun1[3][NBMAX_BC_3D_FUNCTIONS];  /* nb_bc_dirichlet+1 functions on the border    */
//   FUN_3D Fun2[3][NBMAX_BC_3D_FUNCTIONS];  /* nb_bc_dirichlet+1 functions on the border    */

//   Real tps;
   
//   int current_selected_axe;
 
//} BC_3D_XTYPE ;            /* to INITIALIZE in "BCdirichlet_get"           */


typedef struct {

// BC_3D_XTYPE* BC_dirichlet;
// BC_3D_XTYPE* BC_neumann;
// BC_3D_XTYPE* BC_cauchy;
// BC_3D_XTYPE* BC_robin;

// BC_3D_TYPE   BC_MASK[3][NBMAX_BC_3D_FUNCTIONS];
   
} BC_3D ;


#define BC3D_FREE(MyBC) ( Bc3D_free(MyBC), (MyBC)=(BC_3D *)NULL )


BC_3D      *Bc3D_get(void);

int         Bc3D_free(BC_3D  *BC);

BC_3D      *Bc3D_setBcType             ( BC_3D* BC, BC_3D_TYPE bctype, int ref, int axe );
BC_3D_TYPE  Bc3D_getBcType             ( BC_3D* BC, int axe, int ref);

BC_3D      *Bc3D_setFunction1          ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , FUN_TYPE type, void* phi, void* clientdata);
BC_3D      *Bc3D_setFunction2          ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , FUN_TYPE type, void* phi, void* clientdata);

BC_3D      *Bc3D_setCFunction          ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , FUNC_3D phi);
BC_3D      *Bc3D_setCFunction2         ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , FUNC_3D phi);

BC_3D      *Bc3D_setCFunctionTransient ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , FUNC_4D phi);
BC_3D      *Bc3D_setCFunctionTransient2( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , FUNC_4D phi);

Real        Bc3D_evalFunction1         ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , Real x, Real y, Real z);
Real        Bc3D_evalFunction2         ( BC_3D* BC, BC_3D_TYPE bctype, int ref_s, int axe , Real x, Real y, Real z);

BC_3D      *Bc3D_setTps               ( BC_3D *MyBC, Real tps       );
BC_3D      *Bc3D_setCurrentSelectedAxe( BC_3D *MyBC, int  axe       );

int         Bc3D_checkBC              ( const BC_3D* BC , const int axe);

int         Bc3D_hasDirichletBC       ( const BC_3D* BC , const int axe);
int         Bc3D_hasNeumannBC         ( const BC_3D* BC , const int axe);
int         Bc3D_hasCauchyBC          ( const BC_3D* BC , const int axe);
int         Bc3D_hasRobinBC           ( const BC_3D* BC , const int axe);

int         Bc3D_getNumberOfBC        ( const BC_3D* BC , const int axe);
int         Bc3D_getNumberOfBcOnSomms ( const BC_3D* BC , const int axe);
int         Bc3D_getNumberOfBcOnFaces ( const BC_3D* BC , const int axe);

#endif




//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{

#undef  BC_1D_FREE
#undef  BC_2D_FREE
#undef  BC_3D_FREE



void BC_1D_FREE(BC_1D* bc)
{ 
   Bc1D_free(bc);
   (bc)=(BC_1D *)NULL;
}
void BC_2D_FREE(BC_2D* bc)
{ 
   Bc2D_free(bc);
   (bc)=(BC_2D *)NULL;
}
void BC_3D_FREE(BC_3D* bc)
{ 
   Bc3D_free(bc);
   (bc)=(BC_3D *)NULL;
}



BC_1D *Bc1D_setFunctionPython (BC_1D* MyBc, BC_1D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc1D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc1D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction1D);
   Bc1D_setFunction1(MyBc, bctype, ref_s, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject1D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc1D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyBc;
}
BC_1D *Bc1D_setFunctionPython2(BC_1D* MyBc, BC_1D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc1D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc1D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction1D);
   Bc1D_setFunction2(MyBc, bctype, ref_s, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject1D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc1D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyBc;
}

BC_2D *Bc2D_setFunctionPython (BC_2D* MyBc, BC_2D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc2D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc2D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction2D);
   Bc2D_setFunction1(MyBc, bctype, ref_s, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc2D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyBc;
}
BC_2D *Bc2D_setFunctionPython2(BC_2D* MyBc, BC_2D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc2D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc2D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction2D);
   Bc2D_setFunction2(MyBc, bctype, ref_s, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc2D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyBc;
}

BC_3D *Bc3D_setFunctionPython (BC_3D* MyBc, BC_3D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc )
{
   //printf("in <Bc3D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc3D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction3D);
   Bc3D_setFunction1(MyBc, bctype, ref_s, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}
BC_3D *Bc3D_setFunctionPython2(BC_3D* MyBc, BC_3D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc )
{
   //printf("in <Bc3D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc3D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction3D);
   Bc3D_setFunction2(MyBc, bctype, ref_s, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}



BC_1D *Bc1D_setFunctionTransientPython (BC_1D* MyBc, BC_1D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc1D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc1D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction2D);
   Bc1D_setFunction1(MyBc, bctype, ref_s, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc1D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}
BC_1D *Bc1D_setFunctionTransientPython2(BC_1D* MyBc, BC_1D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc )
{
   //printf("in <Bc1D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc1D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction2D);
   Bc1D_setFunction2(MyBc, bctype, ref_s, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc1D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}

BC_2D *Bc2D_setFunctionTransientPython (BC_2D* MyBc, BC_2D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc2D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc2D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction3D);
   Bc2D_setFunction1(MyBc, bctype, ref_s, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc2D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}
BC_2D *Bc2D_setFunctionTransientPython2(BC_2D* MyBc, BC_2D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc2D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc2D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction3D);
   Bc2D_setFunction2(MyBc, bctype, ref_s, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc2D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}

BC_3D *Bc3D_setFunctionTransientPython (BC_3D* MyBc, BC_3D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc )
{
   //printf("in <Bc3D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc3D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction4D);
   Bc3D_setFunction1(MyBc, bctype, ref_s, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject4D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}
BC_3D *Bc3D_setFunctionTransientPython2(BC_3D* MyBc, BC_3D_TYPE bctype, int ref_s, int axe, PyObject *pyfunc)
{
   //printf("in <Bc3D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Bc3D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction4D);
   Bc3D_setFunction2(MyBc, bctype, ref_s, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject4D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Bc3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyBc;
}

%}
