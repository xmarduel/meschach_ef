#ifndef MEF_FUNCTIONS_STRUCTS_1D__H
#define MEF_FUNCTIONS_STRUCTS_1D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file functions_structs_1D.h
 *
 * Contains the structures describing a "C" or a "Python" function in 1-D
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/machine.h"
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"



typedef struct FUN_1D_
{
   FUNC_1D phi_x;               /**< placeholder for a "C" function  with 1 var  (x)         */
   FUNC_2D phi_xt;              /**< placeholder for a "C" function  with 2 vars (x,t)       */
   FUNC_1D_PLUS_VOID phi_x_v;   /**< placeholder for a "C" function  with 2 vars (x,   obj)  */
   FUNC_2D_PLUS_VOID phi_xt_v;  /**< placeholder for a "C" function  with 3 vars (x,t, obj)  */
   
   void*      clientdata;       /**< hold a pointer to a python object or lua state */
   
   FUN_TYPE type;               /**< gives back the type of function stored in the object */

   double (*eval)(const struct FUN_1D_ * fun1D , double x,  ... );
  
} FUN_1D ;         /* to INITIALIZE in "Fun1D_get"          */


typedef double (*FUN_1D_EVAL__FUN_1D_VAARGS)(const FUN_1D * fun1D , double x,  ... );


#define FUN_1D_NULL  ((FUN_1D *)NULL) 

#define FUN_1D_FREE(Fun) ( Fun1D_free(Fun), (Fun)=(FUN_1D *)NULL )


FUN_1D  *Fun1D_get(void);

int Fun1D_free(FUN_1D *Fun);




FUN_1D * Fun1D_init                     (FUN_1D* Fun);
FUN_1D * Fun1D_setFunction              (FUN_1D* Fun, FUN_TYPE type, void* phi, void* cliendata);

Real     Fun1D_evalCFunction            (const FUN_1D* Fun, Real x);
Real     Fun1D_evalPyFunction           (const FUN_1D* Fun, Real x);
Real     Fun1D_evalLUAFunction          (const FUN_1D* Fun, Real x);
   
Real     Fun1D_evalCFunctionTransient   (const FUN_1D* Fun, Real x, Real tps);
Real     Fun1D_evalPyFunctionTransient  (const FUN_1D* Fun, Real x, Real tps);
Real     Fun1D_evalLUAFunctionTransient (const FUN_1D* Fun, Real x, Real tps);

FUN_1D * Fun1D_setCFunction             (FUN_1D* Fun, FUNC_1D  phi);
FUN_1D * Fun1D_setCFunctionTransient    (FUN_1D* Fun, FUNC_2D  phi);

FUN_1D * Fun1D_setLUAFunction           (FUN_1D* Fun, const char* def);
FUN_1D * Fun1D_setLUAFunctionTransient  (FUN_1D* Fun, const char* def);
   
/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif

#endif



/*! \struct FUN_1D
* \brief structure containing the def. of a function in 1-D
*
* The function can be a "C" function (-> clientdata is NULL, while phi_xy or phi_xyt stores the real C function) \n
* The function can be a "Python" function (-> clientdata stores the python object, while phi_x_v or phi_xt_v \n
* (an other "C" function) stores the function "evaluating" the python callable object; \n
* \n
* For example, in a Python program, we will call the function (for a 1D stationnary problem): \n
* Fun1D_setPythonFunction         ( FUN_1D* Fun, python_function); \n
* What happens is that we will store in phi_x_v the following "C" function (in a transparent manner for the user): \n
* \n 
* static Real FunctionForEvaluatingPythonObject1D(Real x, void* clientdata ) \n
* {  \n
*    PyObject *pyfunc, *arglist; \n
*    PyObject *result; \n
*    Real dres = 0; \n
*    \n
*    pyfunc = (PyObject*)clientdata; \n
*    arglist = Py_BuildValue("(d)", x); \n
*    \n
*    if (!PyCallable_Check(pyfunc)) { \n
*       PyErr_SetString(PyExc_TypeError, "Need a function!"); \n
*       return 0.0; \n
*    } \n
*    \n
*    result = PyEval_CallObject(pyfunc, arglist); \n
*    \n
*    Py_DECREF(arglist); \n
*    if (result) \n
*    { \n
*       dres = PyFloat_AsDouble(result); \n
*    } \n
*    Py_XDECREF(result); \n
*    \n
*    return dres; \n
*  } \n
* \n
* \n
* For example, in a Python program, we will call the function (for a 1D transient problem): \n
* Fun1D_setPythonTransientFunction         ( FUN_1D* Fun, python_function); \n
* What happens is that we will store in phi_xt_v the following "C" function (in a transparent manner for the user): \n
* \n
* static Real FunctionForEvaluatingPythonObject1D(Real x, Real t, void* clientdata ) \n
* {  \n
*    PyObject *pyfunc, *arglist; \n
*    PyObject *result; \n
*    Real dres = 0; \n
*    \n
*    pyfunc = (PyObject*)clientdata; \n
*    arglist = Py_BuildValue("(dd)", x, t); \n
*    \n
*    if (!PyCallable_Check(pyfunc)) { \n
*       PyErr_SetString(PyExc_TypeError, "Need a function!"); \n
*       return 0.0; \n
*    } \n
*    \n
*    result = PyEval_CallObject(pyfunc, arglist); \n
*    \n
*    Py_DECREF(arglist); \n
*    if (result) \n
*    { \n
*       dres = PyFloat_AsDouble(result); \n
*    } \n
*    Py_XDECREF(result); \n
*    \n
*    return dres; \n
*  } \n
* \n
* \n
* and we will store in void* clientdata the PyObject function. \n
* \n 
* \n
* We set the members of the structure from a "C" program with the functions \n
* Fun1D_setCFunction         ( FUN_1D* Fun, FUNC_1D   phi); \n
* Fun1D_setCFunctionTransient( FUN_1D* Fun, FUNC_2D   phi); \n
* \n
* We set the members of the structure from a "Python" program with the functions \n
* Fun1D_setPythonFunction         ( FUN_1D* Fun, python_object); \n
* Fun1D_setPythonFunctionTransient( FUN_1D* Fun, python_object); \n
*/

