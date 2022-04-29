
//
//
//

%nodefault; /* no constructor/destructor */ 

typedef unsigned int 	u_int;
typedef double          Real;

//////////////////////////////////////////////////////////////
// TYPEMAP : allow to write to file opened from python
%typemap(in) FILE * {
if (!PyFile_Check($input)) {
	PyErr_SetString(PyExc_TypeError, "Need a file!");
	return NULL;
	}
$1 = PyFile_AsFile($input);
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