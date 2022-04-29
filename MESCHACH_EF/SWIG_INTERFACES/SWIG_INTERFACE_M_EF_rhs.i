
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////

#ifndef MEF_RHS_H
#define MEF_RHS_H


#define MY_LIST6   6

#define TYPE_RHS_1D    0
#define TYPE_RHS_2D    1
#define TYPE_RHS_3D    2

#define NB_STRUCTS6    3


#include "matrix.h"

#include "rhs_1D.h"
#include "rhs_2D.h"
#include "rhs_3D.h"



#define  NB_ERRS6                    5
#define  NB_WARNS6                   1

//static  char *my_err_mesg6[NB_ERRS6] =
//{
//   "unknown rhs error",                /* 0 */
//   "rhs incompatible with geometry" ,  /* 1 */
//   "too many rhs  in lecture",         /* 2 */
//   "rhs index too big",                /* 3 */
//   "rhs axe wrong"                     /* 4 */
//};

//static char *my_warn_mesg6[NB_WARNS6] = 
//{
//   "unknown rhs warning"               /* 0 */
//};

#define	 E_RHS_UNKNOWN	            0
#define	 E_RHS_INCOMPATIBLE         1
#define  E_RHS_TOOMANYRHS           2
#define  E_RHS_WRONGIDXNUMBER       3
#define  E_RHS_WRONGAXENUMBER       4

#define  W_RHS_UNKNOWN              0

/******************************************************************************/

int mem_attach_list6(void);
int err_attach_list6(void);
int warn_attach_list6(void);
int err_warn_attach_lists6(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error6(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,6)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning6(err_num,fn_name) ev_warn(__FILE__,err_num,__LINE__,fn_name,6)

#endif


#ifndef MEF_RHS_1D_H
#define MEF_RHS_1D_H



#include "matrix.h"

#include "functions_structs.h"

/*------------------------------------------------------------------------- */

typedef struct {
     
// FUN_1D Fun[1][NBMAX_RHS_1D_FUNCTIONS];   /* nb_RHS_rhs_fun+1 functions      */

   Real tps;
   
} RHS_1D ;



#define RHS1D_FREE(MyRHS)       ( Rhs1D_free(MyRHS),       (MyRHS)=(RHS_1D *)NULL )

RHS_1D *Rhs1D_get(void);

int     Rhs1D_free(RHS_1D  *RHS);

RHS_1D *Rhs1D_setFunction          ( RHS_1D* RHS, int ref_e, int axe , FUN_TYPE type, void* phi, void* clientdata);

RHS_1D *Rhs1D_setCFunction         ( RHS_1D* RHS, int ref_e, int axe , FUNC_1D phi);
RHS_1D *Rhs1D_setCFunctionTransient( RHS_1D* RHS, int ref_e, int axe , FUNC_2D phi);

RHS_1D *Rhs1D_setTps               ( RHS_1D* RHS, Real tps );

Real    Rhs1D_evalFunction         ( RHS_1D* RHS, int ref_e, int axe , Real x);

#endif



#ifndef MEF_RHS_2D_H
#define MEF_RHS_2D_H


#include "matrix.h"

#include "functions_structs.h"

/*---------------------------------------------------------------------- */


typedef struct {
   
// FUN_2D Fun[2][NBMAX_RHS_2D_FUNCTIONS];      /* nb_RHS_fun+1 functions on the domain */

   Real tps;

   int  current_selected_axe;
      
} RHS_2D ;            /* to INITIALIZE in "RHS2D_get"           */



#define RHS2D_FREE(MyRHS) ( Rhs2D_free(MyRHS), (MyRHS)=(RHS_2D *)NULL )


RHS_2D *Rhs2D_get(void);

int     Rhs2D_free(RHS_2D  *RHS);

RHS_2D* Rhs2D_setFunction           ( RHS_2D* RHS, int ref_e, int axe , FUN_TYPE type, void* phi, void* clientdata);

RHS_2D *Rhs2D_setCFunction          ( RHS_2D* RHS, int ref_e, int axe , FUNC_2D phi);
RHS_2D *Rhs2D_setCFunctionTransient ( RHS_2D* RHS, int ref_e, int axe , FUNC_3D phi);

RHS_2D *Rhs2D_setTps                ( RHS_2D* RHS, Real tps );
RHS_2D *Rhs2D_setCurrentSelectedAxe ( RHS_2D* RHS, int  axe );

Real    Rhs2D_evalFunction          ( RHS_2D* RHS, int ref_e, int axe , Real x, Real y);

#endif



#ifndef MEF_RHS_3D_H
#define MEF_RHS_3D_H

#include "matrix.h"

#include "functions_structs.h"

/*---------------------------------------------------------------------- */

typedef struct {
   
// FUN_3D Fun[3][NBMAX_RHS_3D_FUNCTIONS];  /* nb_RHS_fun+1 functions on the domain  */

   Real tps;
   
   int  current_selected_axe;
   
} RHS_3D ;            /* to INITIALIZE in "RHS_get"           */




#define RHS3D_FREE(MyRHS) ( Rhs3D_free(MyRHS), (MyRHS)=(RHS_3D *)NULL )


RHS_3D *Rhs3D_get(void);

int     Rhs3D_free(RHS_3D  *RHS);

RHS_3D* Rhs3D_setFunction          ( RHS_3D* RHS, int ref_e, int axe , FUN_TYPE type, void* phi, void* clientdata);

RHS_3D *Rhs3D_setCFunction         ( RHS_3D* RHS, int ref_e, int axe , FUNC_3D phi);
RHS_3D *Rhs3D_setCFunctionTransient( RHS_3D* RHS, int ref_e, int axe , FUNC_4D phi);

RHS_3D *Rhs3D_setTps               ( RHS_3D* RHS, Real tps );
RHS_3D *Rhs3D_setCurrentSelectedAxe( RHS_3D* RHS, int  axe );

Real    Rhs3D_evalFunction         ( RHS_3D* RHS, int ref_e, int axe , Real x, Real y, Real z);

#endif




//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{


#undef  RHS_1D_FREE
#undef  RHS_2D_FREE
#undef  RHS_3D_FREE




void RHS_1D_FREE(RHS_1D* rhs)
{ 
   Rhs1D_free(rhs);
   (rhs)=(RHS_1D *)NULL;
}
void RHS_2D_FREE(RHS_2D* rhs)
{ 
   Rhs2D_free(rhs);
   (rhs)=(RHS_2D *)NULL;
}
void RHS_3D_FREE(RHS_3D* rhs)
{ 
   Rhs3D_free(rhs);
   (rhs)=(RHS_3D *)NULL;
}


RHS_1D* Rhs1D_setFunctionPython(RHS_1D* MyRhs, int ref_e, int axe, PyObject *pyfunc)
{
   //printf("in <Rhs1D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Rhs1D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction1D);
   Rhs1D_setFunction(MyRhs, ref_e, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject1D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Rhs1D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyRhs;
}

RHS_2D* Rhs2D_setFunctionPython(RHS_2D* MyRhs, int ref_e, int axe, PyObject *pyfunc)
{
   //printf("in <Rhs2D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Rhs2D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction2D);
   Rhs2D_setFunction(MyRhs, ref_e, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Rhs2D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyRhs;
}

RHS_3D* Rhs3D_setFunctionPython(RHS_3D* MyRhs, int ref_e, int axe, PyObject *pyfunc)
{
   //printf("in <Rhs3D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Rhs3D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction3D);
   Rhs3D_setFunction(MyRhs, ref_e, axe, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Rhs3D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyRhs;
}


RHS_1D* Rhs1D_setFunctionTransientPython(RHS_1D* MyRhs, int ref_e, int axe, PyObject *pyfunc)
{
   //printf("in <Rhs1D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Rhs1D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction2D);
   Rhs1D_setFunction(MyRhs, ref_e, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Rhs1D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return 0;
}

RHS_2D* Rhs2D_setFunctionTransientPython(RHS_2D* MyRhs, int ref_e, int axe, PyObject *pyfunc)
{
   //printf("in <Rhs2D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Rhs2D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction3D);
   Rhs2D_setFunction(MyRhs, ref_e, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Rhs2D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return 0;
}

RHS_3D* Rhs3D_setFunctionTransientPython(RHS_3D* MyRhs, int ref_e, int axe, PyObject *pyfunc)
{
   //printf("in <Rhs3D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Rhs3D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction4D);
   Rhs3D_setFunction(MyRhs, ref_e, axe, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject4D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Rhs3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyRhs;
}

%}
