
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////

#ifndef MEF_PARAMETERS_H
#define MEF_PARAMETERS_H

#include "all_params_json_config.h"

#define MY_LIST8       8

#define  NB_ERRS8      3
#define  NB_WARNS8     1

//static  char *my_err_mesg8[NB_ERRS8] =
//{
//   "unknown json error",                  /* 0 */
//   "json missing object error",           /* 1 */
//   "json object without value error",     /* 2 */
//};

//static char *my_warn_mesg8[NB_WARNS8] =
//{
//   "unknown json warning"	, 			      /* 0 */
//};

#define	E_JSON_UNKNOWN           0
#define	E_JSON_MISSING_OBJECT    1
#define  E_JSON_NULL_OBJECT_VALUE 2

#define  W_JSON_UNKNOWN           0

/******************************************************************************/

int err_attach_list8(void);
int warn_attach_list8(void);
int err_warn_attach_lists8(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error8(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,8)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning8(err_num,fn_name) ev_warn(__FILE__,err_num,__LINE__,fn_name,8)



#define NBPARAMS  5

typedef enum
{
   AXEe_X = 0 ,
   AXEe_Y = 1 ,
   AXEe_Z = 2

} AXES_IDX ;


PARAMS*  Params_get();

PARAMS*  Params_get_staticparam(int i);
PARAMS*  Params_set_staticparam(PARAMS* p, int i);

PARAMS*  Params_setup_from_file(const char* input_file);
PARAMS*  Params_append_from_string(PARAMS* p, const char* text);

//////////////////////////////////////////////////////// set void * typemap as input
#ifdef SWIGPYTHON

%typemap(in) void * {

   int ok = 1;
  
   switch( Params_get_valuetype(arg2, arg3) ) /* check that we send a good type value */
   {
      case TYPEd_STRING: if (PyUnicode_Check($input)) {
	                         PyObject* pStrObj  = PyUnicode_AsUTF8String($input);
                            $1 = PyBytes_AsString(pStrObj);
	                         printf("--> %s %s \t %s \n", arg2 , arg3 , ((char*)$1));
                         }
                         else { ok = 0;
                         }
                         break;

      case TYPEd_REAL:   if (PyFloat_Check($input)) {
                            // OLD $1 = &(PyFloat_AS_DOUBLE($input));
                            // NEW ---
                            double d = PyFloat_AS_DOUBLE($input);
                            $1 = &d;
                            // --- NEW
	                         printf("--> %s %s \t %e \n", arg2 , arg3 , *((double*)$1));
	                      }
                         else { ok = 0;
                         }
                         break;

      case TYPEd_INT:    if (PyLong_Check($input)) {
                            long along = PyLong_AS_LONG($input);
                            $1 = (void*)&along;
                            printf("--> %s %s \t %d \n", arg2 , arg3 , *((int*)$1));
	                      }
                         else { ok = 0;
                         }
                         break;

      case TYPEd_FUN_1D: {
                         FUN_1D *fun = Fun1D_get();
                         Fun1D_setFunctionPython(fun, $input);
                         $1 = fun;
                         printf("--> %s %s \t 0x%lx (function) \n", arg2 , arg3, (long)($input));
	                 }
                         break;

      case TYPEd_FUN_2D: {
                         FUN_2D *fun = Fun2D_get();
                         Fun2D_setFunctionPython(fun, $input);
                         $1 = fun;
                         printf("--> %s %s \t 0x%lx (function) \n", arg2 , arg3, (long)($input));
	                 }
                         break;
                         
      case TYPEd_FUN_3D: {
                         FUN_3D *fun = Fun3D_get();
                         Fun3D_setFunctionPython(fun, $input);
                         $1 = fun;
                         printf("--> %s %s \t 0x%lx (function) \n", arg2 , arg3, (long)($input));
	                 }
                         break;


      case TYPEd_REAL_VECTOR:
			if (PyList_Check($input))
			{
			    int dim = PyList_Size($input);
			    VEC *v = v_get(dim);
			    int k;
			    for (k=0; k<dim; k++)
			    {
					 PyObject *o = PyList_GetItem($input, k);
				    if (!PyFloat_Check(o))
				    {
					    ok = 0;
					    break;
				    }
				    v->ve[k] = PyFloat_AS_DOUBLE(o);
			    }
				$1 = v;
				 printf("--> %s %s \t %p \n", arg2 , arg3 , v);
				v_output(v);
			}
		   else
			{
			    ok = 0;
			}
		
		   break;
			
      case TYPEd_INT_VECTOR:
			if (PyList_Check($input))
			{
			   int dim = PyList_Size($input);
			   IVEC *iv = iv_get(dim);
			   int k;
			   for (k=0; k<dim; k++)
			   {
				   PyObject *o = PyList_GetItem($input, k);
			      if (!PyInt_Check(o))
				   {
					   ok = 0;
					   break;
				   }
				   //iv->ive[k] = PyInt_AS_LONG(o);
               iv->ive[k] = PyLong_AS_LONG(o);
			   }
			   printf("--> %s %s \t %p \n", arg2 , arg3 , iv);
				iv_output(iv);
				$1 = iv;
			}
		   else
			{
			   ok = 0;
			}
			break;
		
		case TYPEd_UNDEFINED:
      default:
			printf("--> %s %s \t %s \n", arg2 , arg3 , "TYPEd_UNDEFINED !!! ");
         ok = 0;
         break;
   }

   if ( ok == 0 ) /* wrong type value sent */
   {
	printf("send something wrong <%s - %s> .... \n", arg2, arg3);
	PyErr_SetString(PyExc_TypeError, "Need something else !!!!!!!!!");
	return NULL;
   }
}

#endif

// this typemap is usued for the following 3 functions:
void  Params_set_oneparam (PARAMS* p, const char* category, const char* field,                     void*  val);
void  Params_set_oneparam1(PARAMS* p, const char* category, const char* field, int idx1,           void*  val);
void  Params_set_oneparam2(PARAMS* p, const char* category, const char* field, int idx1, int idx2, void*  val);



///////////////////////////////////////////////////////// set void * typemap for output
#ifdef SWIGPYTHON

%typemap(out) void * {

   char msg[128];
   
   switch( Params_get_valuetype(arg2, arg3) )
   {
      case TYPEd_STRING: $result = PyString_FromString((char*)($1)); break;
      case TYPEd_REAL:   $result = PyFloat_FromDouble(*(Real*)($1)); break;
      case TYPEd_INT:    $result = PyLong_FromLong((long)(*(int*)($1))); break;
      case TYPEd_REAL_VECTOR:   $result = SWIG_NewPointerObj((void *) $1, SWIGTYPE_p_VEC, 0); break;
      //case TYPEd_INT_VECTOR:    $result = SWIG_NewPointerObj((void *) $1, SWIGTYPE_p_IVEC, 0); break; // actually not used
      case TYPEd_FUN_1D: $result = SWIG_NewPointerObj((void *) $1, SWIGTYPE_p_FUN_1D, 0); break;
      case TYPEd_FUN_2D: $result = SWIG_NewPointerObj((void *) $1, SWIGTYPE_p_FUN_2D, 0); break;
      case TYPEd_FUN_3D: $result = SWIG_NewPointerObj((void *) $1, SWIGTYPE_p_FUN_3D, 0); break;
      case TYPEd_UNDEFINED: /* return when arg2 and arg3 do not exist */
         {
         snprintf(msg, 128, "In SWIG_INTERFACE_M_EF_params : category = %s and field = %s are unknown \n", arg2, arg3);
         msg[127] = '\0';
         PyErr_SetString(PyExc_RuntimeError, msg); return NULL;
         }
         break;
      default:
         {
         snprintf(msg, 128, "In SWIG_INTERFACE_M_EF_params : category = %s and field = %s : type unknown \n", arg2, arg3);
         msg[127] = '\0';
         PyErr_SetString(PyExc_RuntimeError, msg); return NULL;
         }
         break;
   }
}

#endif

// this typemap is usued for the following 3 functions:
void* Params_get_oneparam (const PARAMS* p, const char* category, const char* field);
void* Params_get_oneparam1(const PARAMS* p, const char* category, const char* field, int idx1);
void* Params_get_oneparam2(const PARAMS* p, const char* category, const char* field, int idx1, int idx2);


#endif

//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%typemap(in)  PyObject * {
   if (!PyDict_Check($input))
   {
      PyErr_SetString(PyExc_TypeError, "2nd argument: need a dictionary");
      return NULL;
   }
   $1 = $input;
}


%inline %{

#undef  PARAMS_FREE

void PARAMS_FREE(PARAMS* p)
{
	Params_free(p);
	(p) = (PARAMS*)NULL;
}

   
%}




