/////////////////////////////////////////////////////////
//
// interface file for MESCHACHPLUS-SWIG
//
///////////////////////////////////////////////////////// 

/* 
 * ---------------
 * $Author: xavier $
 * $Date: 2004/07/23 09:00:28 $
 * $Revision: 1.2 $
 * -------------------------------------------------------------------- */
 
%module libmeschach_threaded

%{
#undef HAVE_PROTOTYPES

#include <machine.h>
#include <err.h>
#include <matrix.h>
#include <matrix2.h>
#include <zmatrix.h>
#include <zmatrix2.h>
#include <sparse.h>
#include <sparse2.h>
#include <iter.h>
#include <matlab.h>

#include <machine_threaded.h>
%}

typedef	unsigned int	u_int;
typedef double          Real;

/////////////////////////////////////////////////////////
// TYPEMAP : allow to write to file opened from python
%typemap(python,in) FILE * {
if (!PyFile_Check($input)) {
	PyErr_SetString(PyExc_TypeError, "Need a file!");
	return NULL;
	}
$1 = PyFile_AsFile($input);
}


//////////////////////////////////////////////////////////

%inline %{

static void * exception_catcher(int exception_status)
{
   switch(exception_status)
   {
   case E_SIZES:        PyErr_SetString(PyExc_IndexError  ,"???"); return NULL;
   case E_BOUNDS:       PyErr_SetString(PyExc_IndexError  ,"???"); return NULL;
   case E_MEM:          PyErr_SetString(PyExc_MemoryError ,"???"); return NULL;
   case E_SING:         PyErr_SetString(PyExc_ValueError  ,"???"); return NULL;
   case E_POSDEF:       PyErr_SetString(PyExc_ValueError  ,"???"); return NULL;
   case E_FORMAT:       PyErr_SetString(PyExc_IndexError  ,"???"); return NULL;
   case E_INPUT:        PyErr_SetString(PyExc_IndexError  ,"???"); return NULL;
   case E_NULL:         PyErr_SetString(PyExc_MemoryError ,"???"); return NULL;
   case E_SQUARE:       PyErr_SetString(PyExc_ValueError  ,"???"); return NULL;
   case E_RANGE:        PyErr_SetString(PyExc_IndexError  ,"???"); return NULL;
   case E_INSITU2:      PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_INSITU:       PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_ITER:         PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_CONV:         PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_START:        PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_SIGNAL:       PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_INTERN:       PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_SHARED_VECS:  PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_NEG:          PyErr_SetString(PyExc_ValueError  ,"???"); return NULL;
   case E_OVERWRITE:    PyErr_SetString(PyExc_RuntimeError,"???"); return NULL;
   case E_EOF:          PyErr_SetString(PyExc_EOFError    ,"file error"); return NULL; 

   case -1/*E_UNKNOWN=0*/:
   default: 			 PyErr_SetString(PyExc_RuntimeError,"unknown error"); return NULL;
   }
   return NULL;
}

%} // inline


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

%include ./MESCHACH_THREADED/SWIG_INTERFACES/SWIG_INTERFACE_M_THREADED_machine.i

