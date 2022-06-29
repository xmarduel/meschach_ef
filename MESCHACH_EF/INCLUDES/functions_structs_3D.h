#ifndef MEF_FUNCTIONS_STRUCTS_3D__H
#define MEF_FUNCTIONS_STRUCTS_3D__H

#ifdef __cplusplus
extern "C"
{
#endif

/** \file functions_structs_3D.h
 *
 * Contains the structures describing a "C" or a "Python" function in 3-D
 *
 * -------------------------------------------------------------------- */

#include "MESCHACH/INCLUDES/machine.h"
#include "MESCHACH/INCLUDES/matrix.h"

#include "MESCHACH_EF/INCLUDES/functions_structs.h"


typedef struct FUN_3D_
{
   FUNC_3D phi_xyz;               /**< placeholder for a "C" function  with 3 vars (x,y,z)     */
   FUNC_4D phi_xyzt;              /**< placeholder for a "C" function  with 4 vars (x,y,z,t)   */
   FUNC_3D_PLUS_VOID phi_xyz_v;   /**< placeholder for a "C" function  with 4 vars (x,y,z,  python_obj)  */
   FUNC_4D_PLUS_VOID phi_xyzt_v;  /**< placeholder for a "C" function  with 5 vars (x,y,z,t,python_obj)  */

   void*      clientdata;         /**< hold a pointer to a python object */

   FUN_TYPE type;                 /**< gives back the type of function stored in the object */

   double (*eval)(const struct FUN_3D_ * fun3D , double x, double y, double z,  ... );

} FUN_3D ;         /* to INITIALIZE in "Fun3D_get"          */


typedef double (*FUN_3D_EVAL__FUN_3D_VAARGS)(const FUN_3D * fun3D , double x, double y, double z, ... );


#define FUN_3D_NULL  ((FUN_3D *)NULL)

#define FUN_3D_FREE(Fun) ( Fun3D_free(Fun), (Fun)=(FUN_3D *)NULL )


FUN_3D  *Fun3D_get(void);

int Fun3D_free(FUN_3D *Fun);


FUN_3D * Fun3D_init                   (FUN_3D* Fun);
FUN_3D * Fun3D_setFunction            (FUN_3D* Fun, FUN_TYPE type, void* phi, void* cliendata);

FUN_3D * Fun3D_setCFunction           (FUN_3D* Fun, FUNC_3D  phi);
FUN_3D * Fun3D_setCFunctionTransient  (FUN_3D* Fun, FUNC_4D  phi);

FUN_3D * Fun3D_setLUAFunction            (FUN_3D* Fun, const char* def);
FUN_3D * Fun3D_setLUAFunctionTransient   (FUN_3D* Fun, const char* def);


Real     Fun3D_evalCFunction             (const FUN_3D* Fun, Real x, Real y, Real z          );
Real     Fun3D_evalCFunctionTransient    (const FUN_3D* Fun, Real x, Real y, Real z, Real tps);
Real     Fun3D_evalPyFunction            (const FUN_3D* Fun, Real x, Real y, Real z          );
Real     Fun3D_evalPyFunctionTransient   (const FUN_3D* Fun, Real x, Real y, Real z, Real tps);
Real     Fun3D_evalLUAFunction           (const FUN_3D* Fun, Real x, Real y, Real z           );
Real     Fun3D_evalLUAFunctionTransient  (const FUN_3D* Fun, Real x, Real y, Real z, Real tps);

#ifdef __cplusplus
}
#endif

#endif



/*! \struct FUN_3D
* \brief structure containing the def. of a function in 3-D
*
* The function can be a "C" function (-> clientdata is NULL, while phi_xyz or phi_xyzt stores the real C function) \n
* The function can be a "Python" function (-> clientdata stores the python object, while phi_xyz_v or phi_xyzt_v \n
* (an other "C" function) stores the function "evaluating" the python callable object; \n
* \n
* For example, in a Python program, we will call the function (for a 3D stationnary problem): \n
* Fun3D_setPythonFunction         ( FUN_3D* Fun, python_object); \n
* What happens is that we will store in phi_xyz_v the following "C" function (in a transparent manner for the user): \n
* \n
* static Real FunctionForEvaluatingPythonObject3D(Real x, Real y, Real z, void* clientdata ) \n
* {  \n
*    PyObject *pyfunc, *arglist; \n
*    PyObject *result; \n
*    Real dres = 0; \n
*    \n
*    pyfunc = (PyObject*)clientdata; \n
*    arglist = Py_BuildValue("(ddd)", x,y,z); \n
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
* Fun3D_setCFunction         ( FUN_3D* Fun, FUNC_3D phi); \n
* Fun3D_setCFunctionTransient( FUN_3D* Fun, FUNC_4D phi); \n
* \n
* We set the members of the structure from a "Python" program with the functions \n
* Fun3D_setPythonFunction         ( FUN_3D* Fun, python_object); \n
* Fun3D_setPythonFunctionTransient( FUN_3D* Fun, python_object); \n
*/

