
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_CPGPLOT
//
///////////////////////////////////////////////////////// 

%module meschach_cpgplot

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

#include "MESCHACH_ADDS/INCLUDES/queue.h"

#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_svqueue.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_stationnary.h"
#include "MESCHACH_CPGPLOT/INCLUDES/graphics1D_pgplot_transient.h"

   static void * exception_catch(int exception_status)
   {
      int list_number = list_number__get();
      
      switch(list_number)
      {
         case 0:
         {
            switch(exception_status)
            {
               case E_SIZES:        PyErr_SetString(PyExc_IndexError  , "???"); break;
               case E_BOUNDS:       PyErr_SetString(PyExc_IndexError  , "???"); break;
               case E_MEM:          PyErr_SetString(PyExc_MemoryError , "???"); break;
               case E_SING:         PyErr_SetString(PyExc_ValueError  , "???"); break;
               case E_POSDEF:       PyErr_SetString(PyExc_ValueError  , "???"); break;
               case E_FORMAT:       PyErr_SetString(PyExc_IndexError  , "???"); break;
               case E_INPUT:        PyErr_SetString(PyExc_IndexError  , "???"); break;
               case E_NULL:         PyErr_SetString(PyExc_MemoryError , "???"); break;
               case E_SQUARE:       PyErr_SetString(PyExc_ValueError  , "???"); break;
               case E_RANGE:        PyErr_SetString(PyExc_IndexError  , "???"); break;
               case E_INSITU2:      PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_INSITU:       PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_ITER:         PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_CONV:         PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_START:        PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_SIGNAL:       PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_INTERN:       PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_SHARED_VECS:  PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_NEG:          PyErr_SetString(PyExc_ValueError  , "???"); break;
               case E_OVERWRITE:    PyErr_SetString(PyExc_RuntimeError, "???"); break;
               case E_EOF:          PyErr_SetString(PyExc_EOFError    , "file error"); break;
                  
               case -1/*E_UNKNOWN=0*/:
               default: 		      PyErr_SetString(PyExc_RuntimeError, "unknown error");
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

//////////////////////////////////////////////////////////

%exception {

	int exception_status = 0;
	
	if ( (exception_status=setjmp(*jmp_buf__get())) == 0 )
	{
		$action
	}
	else
	{
		return exception_catch(exception_status);
	}
}

//////////////////////////////////////////////////////////////

%include SWIG_INTERFACE_M_CPGPLOT_WRAPPER.i

