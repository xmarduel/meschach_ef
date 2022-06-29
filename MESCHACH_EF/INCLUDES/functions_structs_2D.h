 #ifndef MEF_FUNCTIONS_STRUCTS_2D__H
#define MEF_FUNCTIONS_STRUCTS_2D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file functions_structs_2D.h
 *
 * Contains the structures describing a "C" or a "Python" function in 2-D
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/machine.h"
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"


typedef struct FUN_2D_
{
   FUNC_2D phi_xy;               /**< placeholder for a "C" function  with 2 vars (x,y)     */
   FUNC_3D phi_xyt;              /**< placeholder for a "C" function  with 3 vars (x,y,t)   */
   FUNC_2D_PLUS_VOID phi_xy_v;   /**< placeholder for a "C" function  with 3 vars (x,y,  python_obj)  */
   FUNC_3D_PLUS_VOID phi_xyt_v;  /**< placeholder for a "C" function  with 4 vars (x,y,t,python_obj)  */

   void*      clientdata;        /**< hold a pointer to a python object */

   FUN_TYPE type;                /**< gives back the type of function stored in the object */

   double (*eval)(const struct FUN_2D_ * fun2D , double x, double y , ... );

} FUN_2D ;         /* to INITIALIZE in "Fun2D_get"          */


typedef double (*FUN_2D_EVAL__FUN_2D_VAARGS)(const FUN_2D * fun2D , double x, double y , ... );


#define FUN_2D_NULL  ((FUN_2D *)NULL)

#define FUN_2D_FREE(Fun) ( Fun2D_free(Fun), (Fun)=(FUN_2D *)NULL )


FUN_2D  *Fun2D_get(void);

int Fun2D_free(FUN_2D *Fun);


FUN_2D * Fun2D_init                  (FUN_2D* Fun);
FUN_2D * Fun2D_setFunction           (FUN_2D* Fun, FUN_TYPE type, void* phi, void* cliendata);

FUN_2D * Fun2D_setCFunction          (FUN_2D* Fun, FUNC_2D  phi);
FUN_2D * Fun2D_setCFunctionTransient (FUN_2D* Fun, FUNC_3D  phi);

FUN_2D * Fun2D_setLUAFunction            (FUN_2D* Fun, const char* def);
FUN_2D * Fun2D_setLUAFunctionTransient   (FUN_2D* Fun, const char* def);


Real     Fun2D_evalCFunction           (const FUN_2D* Fun, Real x, Real y          );
Real     Fun2D_evalCFunctionTransient  (const FUN_2D* Fun, Real x, Real y, Real tps);
Real     Fun2D_evalPyFunction          (const FUN_2D* Fun, Real x, Real y          );
Real     Fun2D_evalPyFunctionTransient (const FUN_2D* Fun, Real x, Real y, Real tps);
Real     Fun2D_evalLUAFunction         (const FUN_2D* Fun, Real x, Real y          );
Real     Fun2D_evalLUAFunctionTransient(const FUN_2D* Fun, Real x, Real y, Real tps);

#ifdef __cplusplus
}
#endif

#endif





/*! \struct FUN_2D
* \brief structure containing the def. of a function in 2-D
*
* The function can be a "C" function (-> clientdata is NULL, while phi_xy or phi_xyt stores the real C function) \n
* The function can be a "Python" function (-> clientdata stores the python object, while phi_xy_v or phi_xyt_v \n
* (an other "C" function) stores the function "evaluating" the python callable object; \n
* \n
* For example, in a Python program, we will call the function (for a 2D stationnary problem): \n
* Fun2D_setPythonFunction         ( FUN_2D* Fun, python_function); \n
* What happens is that we will store in phi_xy_v the following "C" function (in a transparent manner for the user): \n
* \n
* static Real FunctionForEvaluatingPythonObject2D(Real x, Real y, void* clientdata ) \n
* {  \n
*    PyObject *pyfunc, *arglist; \n
*    PyObject *result; \n
*    Real dres = 0; \n
*    \n
*    pyfunc = (PyObject*)clientdata; \n
*    arglist = Py_BuildValue("(dd)", x,y); \n
*    \n
*    if (!PyCallable_Check(pyfunc)) { \n
*       PyErr_SetString(PyExc_TypeError, "Need a function!"); \n
*       return 0.0; \n
*    } \n
*    \n
*    result = PyEval_CallObject(pyfunc, arglist); \n
*     \n
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
* Fun2D_setCFunction         ( FUN_2D* Fun, FUNC_2D phi); \n
* Fun2D_setCFunctionTransient( FUN_2D* Fun, FUNC_3D phi); \n
* \n
* We set the members of the structure from a "Python" program with the functions \n
* Fun2D_setPythonFunction         ( FUN_2D* Fun, python_object); \n
* Fun2D_setPythonFunctionTransient( FUN_2D* Fun, python_object); \n
*/

