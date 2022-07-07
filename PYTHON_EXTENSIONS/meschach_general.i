
//
//

%nodefault; /* no constructor/destructor */

typedef unsigned int 	u_int;
typedef double          Real;
typedef double          real;

%include "typemaps.i"

//////////////////////////////////////////////////////////////
// TYPEMAP : allow to write to file opened from python
%typemap(in) FILE * {
   // PYTHON-2.7 was easy:
//if (!PyFile_Check($input)) {
//	PyErr_SetString(PyExc_TypeError, "Need a file!");
//	return NULL;
//}

   // PYTHON-3.x is not so easy

   if ( PyLong_Check($input) == 1 ) {
      printf("--> PyLong_Check($input) = %d : IS INTEGER \n", PyLong_Check($input));
      PyErr_SetString(PyExc_TypeError, "Need a file (not an INTEGER)!");
      return NULL;
   }
   if ( PyFloat_Check($input) == 1 ) {
      printf("--> PyFloat_Check($input) = %d : IS FLOAT \n", PyFloat_Check($input));
      PyErr_SetString(PyExc_TypeError, "Need a file (not a FLOAT)!");
      return NULL;
   }
   if ( PyString_Check($input) == 1 ) {
      printf("--> PyString_Check($input) = %d : IS STRING \n", PyString_Check($input));
      PyErr_SetString(PyExc_TypeError, "Need a file (not a STRING)!");
      return NULL;
   }

   int fd = PyObject_AsFileDescriptor($input);
   if ( fd < 0 ) {
         PyErr_SetString(PyExc_TypeError, "Need a file!");
         return NULL;
   }

   if ( strstr("$symname", "input") != NULL) {
      //printf("--> FILE* typemap <r>\n");
      $1 = fdopen(fd, "r"); // a FILE * to read
   }
   else {
      //printf("--> FILE* typemap <w>\n");
      $1 = fdopen(fd, "w"); // a FILE *, but we need a "flush" so that it works
   }
}

// force writing to FILE* before returning to python
%typemap(freearg) FILE * {
   if ( strstr("$symname", "output") != NULL) {
      fflush(arg1);
   }
}

//////////////////////////////////////////////////////////////
// TYPEMAP : allow to write to file opened from python
%typemap(in) char * {
if (!PyString_Check($input)) {
	PyErr_SetString(PyExc_TypeError, "Need a string!");
	return NULL;
	}
$1 = PyString_AsString($input);
}
//////////////////////////////////////////////////////////////
// TYPEMAP :
%typemap(in) PyObject * {
if (!PyCallable_Check($input)) {
	PyErr_SetString(PyExc_TypeError, "Need a function!");
	return NULL;
	}
$1 = $input;
}
//////////////////////////////////////////////////////////////
