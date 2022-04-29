/////////////////////////////////////////////////////////
//
// interface file for MESCHACH
//
///////////////////////////////////////////////////////// 

%module meschach

%include cpointer.i
%pointer_functions(int, intp);

//%include ptrlang.i

%{
#undef HAVE_PROTOTYPES

#include "MESCHACH/INCLUDES/machine.h"
#include "MESCHACH/INCLUDES/err.h"
#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/matrix2.h"
#include "MESCHACH/INCLUDES/zmatrix.h"
#include "MESCHACH/INCLUDES/zmatrix2.h"
#include "MESCHACH/INCLUDES/sparse.h"
#include "MESCHACH/INCLUDES/sparse2.h"
#include "MESCHACH/INCLUDES/iter.h"
#include "MESCHACH/INCLUDES/matlab.h"

typedef int (*FUNC_FREE)(void*);

   static void * exception_catcher(int exception_status)
   {
      switch(exception_status)
      {
         case E_SIZES:        PyErr_SetString(PyExc_IndexError  , "???"); return NULL;
         case E_BOUNDS:       PyErr_SetString(PyExc_IndexError  , "???"); return NULL;
         case E_MEM:          PyErr_SetString(PyExc_MemoryError , "???"); return NULL;
         case E_SING:         PyErr_SetString(PyExc_ValueError  , "???"); return NULL;
         case E_POSDEF:       PyErr_SetString(PyExc_ValueError  , "???"); return NULL;
         case E_FORMAT:       PyErr_SetString(PyExc_IndexError  , "???"); return NULL;
         case E_INPUT:        PyErr_SetString(PyExc_IndexError  , "???"); return NULL;
         case E_NULL:         PyErr_SetString(PyExc_MemoryError , "???"); return NULL;
         case E_SQUARE:       PyErr_SetString(PyExc_ValueError  , "???"); return NULL;
         case E_RANGE:        PyErr_SetString(PyExc_IndexError  , "???"); return NULL;
         case E_INSITU2:      PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_INSITU:       PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_ITER:         PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_CONV:         PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_START:        PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_SIGNAL:       PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_INTERN:       PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_SHARED_VECS:  PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_NEG:          PyErr_SetString(PyExc_ValueError  , "???"); return NULL;
         case E_OVERWRITE:    PyErr_SetString(PyExc_RuntimeError, "???"); return NULL;
         case E_EOF:          PyErr_SetString(PyExc_EOFError    , "file error"); return NULL;
            
         case -1 /*E_UNKNOWN=0*/:
         default: 			   PyErr_SetString(PyExc_RuntimeError,"unknown error"); return NULL;
      }
      return NULL;
   }
   
%}

%include meschach_general.i

//////////////////////////////////////////////////////////
#define atry if ((exception_status=setjmp(*restart)) == 0)
#define acatch(val) else if (exception_status == val )
#define afinally else


%exception {

	int exception_status = 0;
        
	if ( (exception_status=setjmp(*jmp_buf__get())) == 0 )
	{
		$action
	}
	else
   {
		return exception_catcher(exception_status);
   }
}
//////////////////////////////////////////////////////////////

%include SWIG_INTERFACE_M_M_meminfo.i
%include SWIG_INTERFACE_M_M_err.i
%include SWIG_INTERFACE_M_M_matrix.i
%include SWIG_INTERFACE_M_M_matrix2.i 
%include SWIG_INTERFACE_M_M_iter.i
%include SWIG_INTERFACE_M_M_machine.i
%include SWIG_INTERFACE_M_M_matlab.i
%include SWIG_INTERFACE_M_M_sparse.i
%include SWIG_INTERFACE_M_M_sparse2.i
%include SWIG_INTERFACE_M_M_zmatrix.i
%include SWIG_INTERFACE_M_M_zmatrix2.i 
