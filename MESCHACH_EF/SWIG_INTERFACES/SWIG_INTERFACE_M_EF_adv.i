
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  

#ifndef MEF_ADV_H
#define MEF_ADV_H


#define MY_LIST7   7

#define TYPE_ADV_1D    0
#define TYPE_ADV_2D    1
#define TYPE_ADV_3D    2

#define NB_STRUCTS7    3


#include "matrix.h"

#include "ADV_1D.h"
#include "ADV_2D.h"
#include "ADV_3D.h"


#define  NB_ERRS7                    5
#define  NB_WARNS7                   1

//static  char *my_err_mesg7[NB_ERRS7] =
//{
//   "unknown adv error",                /* 0 */
//   "adv incompatible with geometry" ,  /* 1 */
//   "too many adv  in lecture",         /* 2 */
//   "adv index too big",                /* 3 */
//   "adv axe too big"                   /* 4 */
//};

//static char *my_warn_mesg7[NB_WARNS7] = 
//{
//   "unknown rhs warning"               /* 0 */
//};

#define	 E_ADV_UNKNOWN	            0
#define	 E_ADV_INCOMPATIBLE         1
#define  E_ADV_TOOMANYADV           2
#define  E_ADV_WRONGIDXNUMBER       3
#define  E_ADV_WRONGAXENUMBER       4

#define  W_ADV_UNKNOWN              0

/******************************************************************************/

int mem_attach_list7(void);
int err_attach_list7(void);
int warn_attach_list7(void);
int err_warn_attach_lists7(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error7(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,7)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning7(err_num,fn_name) ev_warn(__FILE__,err_num,__LINE__,fn_name,7)

#endif


#ifndef MEF_ADV_1D_H
#define MEF_ADV_1D_H



#include "matrix.h"

#include "functions_structs.h"

/*------------------------------------------------------------------------- */

typedef struct {
   
// FUN_1D Fun[1][1][NBMAX_ADV_1D_FUNCTIONS];   /* nb_ADV_ADV_fun+1 functions      */

   Real tps;
   
} ADV_1D ;



#define Adv1D_FREE(MyAdv)       ( Adv1D_free(MyAdv),       (MyAdv)=(ADV_1D *)NULL )

ADV_1D *Adv1D_get(void);

int     Adv1D_free(ADV_1D  *Adv);

ADV_1D *Adv1D_setFunction          ( ADV_1D* Adv, int ref_e, int axe1 , int axe2 , int axe3 ,FUN_TYPE type, void* phi, void* clientdata);

ADV_1D *Adv1D_setCFunction         ( ADV_1D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUNC_1D phi);
ADV_1D *Adv1D_setCFunctionTransient( ADV_1D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUNC_2D phi);

ADV_1D *Adv1D_setTps               ( ADV_1D* Adv, Real tps );

Real    Adv1D_evalFunction         ( ADV_1D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , Real x);

#endif



#ifndef MEF_ADV_2D_H
#define MEF_ADV_2D_H


#include "matrix.h"

#include "functions_structs.h"

/*---------------------------------------------------------------------- */


typedef struct {
  
// FUN_2D Fun1[2][2][NBMAX_ADV_2D_FUNCTIONS];      /* nb_ADV_fun+1 functions on the domain */
// FUN_2D Fun2[2][2][NBMAX_ADV_2D_FUNCTIONS];      /* nb_ADV_fun+1 functions on the domain */

   Real tps;

   int  current_selected_axe1;
   int  current_selected_axe2;
   int  current_selected_axe3;
   
} ADV_2D ;            /* to INITIALIZE in "Adv2D_get"           */



#define Adv2D_FREE(MyAdv) ( Adv2D_free(MyAdv), (MyAdv)=(ADV_2D *)NULL )


ADV_2D *Adv2D_get(void);

int     Adv2D_free(ADV_2D  *Adv);

ADV_2D* Adv2D_setFunction          ( ADV_2D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUN_TYPE type, void* phi, void* clientdata);

ADV_2D *Adv2D_setCFunction         ( ADV_2D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUNC_2D phi);
ADV_2D *Adv2D_setCFunctionTransient( ADV_2D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUNC_3D phi);

ADV_2D *Adv2D_setTps               ( ADV_2D* Adv, Real tps );
ADV_2D *Adv2D_setCurrentSelectedAxe( ADV_2D* Adv, int  axe1,  int axe2 , int axe3 );

Real    Adv2D_evalFunction         ( ADV_2D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , Real x, Real y);

#endif



#ifndef MEF_ADV_3D_H
#define MEF_ADV_3D_H

#include "matrix.h"

#include "functions_structs.h"

/*---------------------------------------------------------------------- */

typedef struct {
   
// FUN_3D Fun1[3][3][NBMAX_ADV_3D_FUNCTIONS];  /* nb_ADV_fun+1 functions on the domain  */
// FUN_3D Fun2[3][3][NBMAX_ADV_3D_FUNCTIONS];  /* nb_ADV_fun+1 functions on the domain  */
// FUN_3D Fun3[3][3][NBMAX_ADV_3D_FUNCTIONS];  /* nb_ADV_fun+1 functions on the domain  */

   Real tps;
   
   int  current_selected_axe1;
   int  current_selected_axe2;
   int  current_selected_axe3;
   
} ADV_3D ;            /* to INITIALIZE in "ADV_get"           */




#define Adv3D_FREE(MyAdv) ( Adv3D_free(MyAdv), (MyAdv)=(ADV_3D *)NULL )


ADV_3D *Adv3D_get(void);

int     Adv3D_free(ADV_3D  *Adv);

ADV_3D* Adv3D_setFunction          ( ADV_3D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUN_TYPE type, void* phi, void* clientdata);

ADV_3D *Adv3D_setCFunction         ( ADV_3D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUNC_3D phi);
ADV_3D *Adv3D_setCFunctionTransient( ADV_3D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , FUNC_4D phi);

ADV_3D *Adv3D_setTps               ( ADV_3D* Adv, Real tps );
ADV_3D *Adv3D_setCurrentSelectedAxe( ADV_3D* Adv, int  axe1 , int axe2 , int axe3 );

Real    Adv3D_evalFunction         ( ADV_3D* Adv, int ref_e, int axe1 , int axe2 , int axe3 , Real x, Real y, Real z);

#endif




//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{


#undef  ADV_1D_FREE
#undef  ADV_2D_FREE
#undef  ADV_3D_FREE




void ADV_1D_FREE(ADV_1D* Adv)
{ 
   Adv1D_free(Adv);
   (Adv)=(ADV_1D *)NULL;
}
void ADV_2D_FREE(ADV_2D* Adv)
{ 
   Adv2D_free(Adv);
   (Adv)=(ADV_2D *)NULL;
}
void ADV_3D_FREE(ADV_3D* Adv)
{ 
   Adv3D_free(Adv);
   (Adv)=(ADV_3D *)NULL;
}


ADV_1D* Adv1D_setFunctionPython(ADV_1D* MyAdv, int ref_e, int axe1, int axe2, int axe3, PyObject *pyfunc )
{
   //printf("in <Adv1D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Adv1D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction1D);
   Adv1D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject1D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Adv1D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyAdv;
}

ADV_2D* Adv2D_setFunctionPython(ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3, PyObject *pyfunc )
{
   //printf("in <Adv2D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Adv2D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction2D);
   Adv2D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Adv2D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyAdv;
}

ADV_3D* Adv3D_setFunctionPython(ADV_3D* MyAdv, int ref_e,  int axe1, int axe2, int axe3, PyObject *pyfunc )
{
   //printf("in <Adv3D_setFunctionPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Adv3D_setFunctionPython>,   func = %Ox%lx\n",PythonFunction3D);
   Adv3D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Adv3D_setFunctionPython>, A python function as been successfully set ... \n");
   return MyAdv;
}


ADV_1D* Adv1D_setFunctionTransientPython(ADV_1D* MyAdv, int ref_e,  int axe1, int axe2, int axe3, PyObject *pyfunc )
{
   //printf("in <Adv1D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Adv1D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction2D);
   Adv1D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject2D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Adv1D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyAdv;
}

ADV_2D* Adv2D_setFunctionTransientPython(ADV_2D* MyAdv, int ref_e, int axe1, int axe2, int axe3 , PyObject *pyfunc )
{
   //printf("in <Adv2D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Adv2D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction3D);
   Adv2D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject3D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Adv2D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyAdv;
}

ADV_3D* Adv3D_setFunctionTransientPython(ADV_3D* MyAdv, int ref_e, int axe1, int axe2, int axe3, PyObject *pyfunc )
{
   //printf("in <Adv3D_setFunctionTransientPython>, pyfunc = %Ox%lx\n",pyfunc);
   //printf("in <Adv3D_setFunctionTransientPython>,   func = %Ox%lx\n",PythonFunction4D);
   Adv3D_setFunction(MyAdv, ref_e, axe1, axe2, axe3, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject4D, pyfunc);
   Py_INCREF(pyfunc);
   //printf("in <Adv3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
   return MyAdv;
}

%}
