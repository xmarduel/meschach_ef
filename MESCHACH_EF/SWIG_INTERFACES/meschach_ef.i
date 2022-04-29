
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  
 
%module meschach_ef 

%{
#undef HAVE_PROTOTYPES

#include "MESCHACH/INCLUDES/err.h"
#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/zmatrix.h"
#include "MESCHACH/INCLUDES/zmatrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/iter.h"
#include "MESCHACH/INCLUDES/matlab.h"

#include "MESCHACH_ADDS/INCLUDES/iter_adds.h"
#include "MESCHACH_ADDS/INCLUDES/matrix_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse_adds.h"
#include "MESCHACH_ADDS/INCLUDES/sparse2_adds.h"
#include "MESCHACH_ADDS/INCLUDES/spmat2formats.h"
#include "MESCHACH_ADDS/INCLUDES/control.h"
#include "MESCHACH_ADDS/INCLUDES/gpskmesh.h"

#include "MESCHACH_EF/INCLUDES/functions_definitions.h"
#include "MESCHACH_EF/INCLUDES/all_params.h"
#include "MESCHACH_EF/INCLUDES/all_params_datas.h"
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/functions_structs_1D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs_2D.h"
#include "MESCHACH_EF/INCLUDES/functions_structs_3D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_1D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_2D.h"
#include "MESCHACH_EF/INCLUDES/boundary_conditions_3D.h"
#include "MESCHACH_EF/INCLUDES/rhs.h"
#include "MESCHACH_EF/INCLUDES/rhs_1D.h"
#include "MESCHACH_EF/INCLUDES/rhs_2D.h"
#include "MESCHACH_EF/INCLUDES/rhs_3D.h"
#include "MESCHACH_EF/INCLUDES/adv.h"
#include "MESCHACH_EF/INCLUDES/adv_1D.h"
#include "MESCHACH_EF/INCLUDES/adv_2D.h"
#include "MESCHACH_EF/INCLUDES/adv_3D.h"
#include "MESCHACH_EF/INCLUDES/finite_elements.h"
#include "MESCHACH_EF/INCLUDES/finite_elements_1D.h"
#include "MESCHACH_EF/INCLUDES/finite_elements_2D.h"
#include "MESCHACH_EF/INCLUDES/finite_elements_3D.h"
#include "MESCHACH_EF/INCLUDES/geometry.h"
#include "MESCHACH_EF/INCLUDES/geometry_1D.h"
#include "MESCHACH_EF/INCLUDES/geometry_2D.h"
#include "MESCHACH_EF/INCLUDES/geometry_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_1D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_2D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_3D.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_geoms.h"
#include "MESCHACH_EF/INCLUDES/graphics_output_mtxpattern.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_1D.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_2D.h"
#include "MESCHACH_EF/INCLUDES/assemblage_objects_3D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_1D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_2D.h"
#include "MESCHACH_EF/INCLUDES/apply_boundaryconditions_3D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_1D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_2D.h"
#include "MESCHACH_EF/INCLUDES/miscellaneous_3D.h"
#include "MESCHACH_EF/INCLUDES/algo_stokes.h"
#include "MESCHACH_EF/INCLUDES/algo_navierstokes.h"
#include "MESCHACH_EF/INCLUDES/problems_solvers.h"
   
   static Real FunctionForEvaluatingPythonObject1D(Real x, PyObject* pyfunc)
   {
      PyObject *arglist;
      PyObject *result;
      Real dres = 0;
      
      arglist = Py_BuildValue("(d)", x);
      
      if (!PyCallable_Check(pyfunc)) {
         PyErr_SetString(PyExc_TypeError, "Need a function!");
         return 0.0;
      }
      
      /**/
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure();
      /**/
      
      result = PyEval_CallObject(pyfunc, arglist);
      
      /**/
      PyGILState_Release(gstate);
      /**/
      
      Py_DECREF(arglist);
      
      if (result)
      {
         dres = PyFloat_AsDouble(result);
      }
      
      Py_XDECREF(result);
      
      return dres;
   }
   
   static Real FunctionForEvaluatingPythonObject2D(Real x, Real y, PyObject* pyfunc)
   {
      PyObject *arglist;
      PyObject *result;
      Real dres = 0;
      
      arglist = Py_BuildValue("(dd)", x,y);
      
      if (!PyCallable_Check(pyfunc)) {
         PyErr_SetString(PyExc_TypeError, "Need a function!");
         return 0.0;
      }
      
      /**/
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure();
      /**/
      
      result = PyEval_CallObject(pyfunc, arglist);
      
      /**/
      PyGILState_Release(gstate);
      /**/
      
      Py_DECREF(arglist);
      
      if (result)
      {
         dres = PyFloat_AsDouble(result);
      }
      
      Py_XDECREF(result);
      
      return dres;
   }
   
   static Real FunctionForEvaluatingPythonObject3D(Real x, Real y, Real z, PyObject* pyfunc)
   {
      PyObject *arglist;
      PyObject *result;
      Real dres = 0;
      
      arglist = Py_BuildValue("(ddd)", x,y,z);
      
      if (!PyCallable_Check(pyfunc)) {
         PyErr_SetString(PyExc_TypeError, "Need a function!");
         return 0.0;
      }
      
      /**/
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure();
      /**/
      
      result = PyEval_CallObject(pyfunc, arglist);
      
      /**/
      PyGILState_Release(gstate);
      /**/
      
      Py_DECREF(arglist);
      
      if (result)
      {
         dres = PyFloat_AsDouble(result);
      }
      
      Py_XDECREF(result);
      
      return dres;
   }
   
   static Real FunctionForEvaluatingPythonObject4D(Real x, Real y, Real z, Real t, PyObject* pyfunc)
   {
      PyObject *arglist;
      PyObject *result;
      Real dres = 0;
      
      arglist = Py_BuildValue("(dddd)", x,y,z,t);
      
      if (!PyCallable_Check(pyfunc)) {
         PyErr_SetString(PyExc_TypeError, "Need a function!");
         return 0.0;
      }
      
      /**/
      PyGILState_STATE gstate;
      gstate = PyGILState_Ensure();
      /**/
      
      result = PyEval_CallObject(pyfunc, arglist);
      
      /**/
      PyGILState_Release(gstate);
      /**/
      
      Py_DECREF(arglist);
      
      if (result)
      {
         dres = PyFloat_AsDouble(result);
      }
      
      Py_XDECREF(result);
      
      return dres;
   }

   static void * exception_catcher(int exception_status)
   {
      int list_number = list_number__get();
      
      switch(list_number)
      {
         case 0:
         {
            switch(exception_status)
            {
               case E_SIZES:        PyErr_SetString(PyExc_IndexError  ,"???"); break;
               case E_BOUNDS:       PyErr_SetString(PyExc_IndexError  ,"???"); break;
               case E_MEM:          PyErr_SetString(PyExc_MemoryError ,"???"); break;
               case E_SING:         PyErr_SetString(PyExc_ValueError  ,"???"); break;
               case E_POSDEF:       PyErr_SetString(PyExc_ValueError  ,"???"); break;
               case E_FORMAT:       PyErr_SetString(PyExc_IndexError  ,"???"); break;
               case E_INPUT:        PyErr_SetString(PyExc_IndexError  ,"???"); break;
               case E_NULL:         PyErr_SetString(PyExc_MemoryError ,"???"); break;
               case E_SQUARE:       PyErr_SetString(PyExc_ValueError  ,"???"); break;
               case E_RANGE:        PyErr_SetString(PyExc_IndexError  ,"???"); break;
               case E_INSITU2:      PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_INSITU:       PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_ITER:         PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_CONV:         PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_START:        PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_SIGNAL:       PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_INTERN:       PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_SHARED_VECS:  PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_NEG:          PyErr_SetString(PyExc_ValueError  ,"???"); break;
               case E_OVERWRITE:    PyErr_SetString(PyExc_RuntimeError,"???"); break;
               case E_EOF:          PyErr_SetString(PyExc_EOFError    ,"file error"); break;
                  
               case -1/*E_UNKNOWN=0*/:
               default: 		 PyErr_SetString(PyExc_RuntimeError,"unknown error");
            }
         } break;
            
            
         case MY_LIST2:
         {
            if (exception_status < NB_ERRS2 )
            {
               PyErr_SetString(PyExc_RuntimeError,my_err_mesg2[exception_status]);
            }
            else
            {
               PyErr_SetString(PyExc_RuntimeError,"???");
            }
         } break;
            
         case MY_LIST3:
         {
            if (exception_status < NB_ERRS3 )
            {
               PyErr_SetString(PyExc_RuntimeError,my_err_mesg3[exception_status]);
            }
            else
            {
               PyErr_SetString(PyExc_RuntimeError,"???");
            }
         } break;
            
         case MY_LIST4:
         {
            if (exception_status < NB_ERRS4 )
            {
               PyErr_SetString(PyExc_RuntimeError,my_err_mesg4[exception_status]);
            }
            else
            {
               PyErr_SetString(PyExc_RuntimeError,"???");
            }
         } break;
            
         case MY_LIST5:
         {
            if (exception_status < NB_ERRS5 )
            {
               PyErr_SetString(PyExc_RuntimeError,my_err_mesg5[exception_status]);
            }
            else
            {
               PyErr_SetString(PyExc_RuntimeError,"???");
            }
         } break;
            
         case MY_LIST6:
         {
            if (exception_status < NB_ERRS6 )
            {
               PyErr_SetString(PyExc_RuntimeError,my_err_mesg6[exception_status]);
            }
            else
            {
               PyErr_SetString(PyExc_RuntimeError,"???");
            }
         } break;
            
         case MY_LIST7:
         {
            if (exception_status < NB_ERRS7 )
            {
               PyErr_SetString(PyExc_RuntimeError,my_err_mesg7[exception_status]);
            }
            else
            {
               PyErr_SetString(PyExc_RuntimeError,"???");
            }
         } break;
            
         default:
         {
            PyErr_SetString(PyExc_RuntimeError,"LIST UNKNOWN");
         } break;
            
      }
      
      return NULL;
   }

%}


%include meschach_general.i

//////////////////////////////////////////////////////////////


%exception {

	int exception_status = 0;
	
	if ((exception_status=setjmp(*jmp_buf__get())) == 0)
	{
		$action
	}
	else
	{
		return exception_catcher(exception_status);
	}
}
/////////////////////////////////////////////////////////////////

%include SWIG_INTERFACE_M_EF_finite_element.i
%include SWIG_INTERFACE_M_EF_functions_defs.i
%include SWIG_INTERFACE_M_EF_functions_structs.i
%include SWIG_INTERFACE_M_EF_boundary_conditions.i
%include SWIG_INTERFACE_M_EF_rhs.i
%include SWIG_INTERFACE_M_EF_adv.i
%include SWIG_INTERFACE_M_EF_assemblage.i
%include SWIG_INTERFACE_M_EF_apply_bc.i
%include SWIG_INTERFACE_M_EF_geometry.i
%include SWIG_INTERFACE_M_EF_graphics.i
%include SWIG_INTERFACE_M_EF_problem_solvers.i
%include SWIG_INTERFACE_M_EF_miscellaneous.i
%include SWIG_INTERFACE_M_EF_algo_stokes.i
%include SWIG_INTERFACE_M_EF_algo_navierstokes.i
%include SWIG_INTERFACE_M_EF_params.i
