
/////////////////////////////////////////////////////////
//
// interface file for MESCHACH_EF
//
/////////////////////////////////////////////////////////  
 
#ifndef MEF_FUNCTIONS_STRUCTS__H
#define MEF_FUNCTIONS_STRUCTS__H


#define MY_LIST5   5

#define TYPE_FUN_1D   0
#define TYPE_FUN_2D   1
#define TYPE_FUN_3D   2

#define NB_STRUCTS5   3

typedef Real (*FUNC_1D)(Real);
typedef Real (*FUNC_2D)(Real,Real);
typedef Real (*FUNC_3D)(Real,Real,Real);
typedef Real (*FUNC_4D)(Real,Real,Real,Real);

//typedef Real (*FUNC_1D_PLUS_VOID)(Real,                void*);
//typedef Real (*FUNC_2D_PLUS_VOID)(Real,Real,           void*);
//typedef Real (*FUNC_3D_PLUS_VOID)(Real,Real,Real,      void*);
//typedef Real (*FUNC_4D_PLUS_VOID)(Real,Real,Real,Real, void*);

typedef enum
{
   FUN_UNDEFINED      = -1,  /**< flag indicates that the function is not yet set */

   FUN_C_STATIONNARY  =  0,  /**< function depends only on space variable */
   FUN_C_TRANSIENT    =  1,  /**< function depends on t + space variables */
   FUN_PY_STATIONNARY =  2,  /**< function depends on t + space variables */
   FUN_PY_TRANSIENT   =  3   /**< function depends on t + space variables */
   
} FUN_TYPE;



//typedef struct
//{
//   FUNC_1D phi_x;               /**< phi for dim 1                      */
//   FUNC_2D phi_xt;              /**< phi for dim 1 with time            */
//   FUNC_1D_PLUS_VOID phi_x_v;   /**< phi for dim 1 plus void            */
//   FUNC_2D_PLUS_VOID phi_xt_v;  /**< phi for dim 1 plus_void with time  */
//   
//   void*      clientdata;       /**< hold a pointer to a python object */
//   
//   FUN_TYPE type;               /**< gives back the type of function stored im the object */
//  
//} FUN_1D ;         /* to INITIALIZE in "Fun1D_get"          */


//typedef struct
//{
//   FUNC_2D phi_xy;               /**< phi for dim 2                      */
//   FUNC_3D phi_xyt;              /**< phi for dim 2 with time            */
//   FUNC_2D_PLUS_VOID phi_xy_v;   /**< phi for dim 2 plus void            */
//   FUNC_3D_PLUS_VOID phi_xyt_v;  /**< phi for dim 2 plus_void with time  */
//   
//   void*      clientdata;        /**< hold a pointer to a python object */
//   
//   FUN_TYPE type;                /**< gives back the type of function stored im the object */
//
//} FUN_2D ;         /* to INITIALIZE in "Fun2D_get"          */


//typedef struct
//{
//   FUNC_3D phi_xyz;               /**< phi for dim 3                      */
//   FUNC_4D phi_xyzt;              /**< phi for dim 3 with time            */
//   FUNC_3D_PLUS_VOID phi_xyz_v;   /**< phi for dim 3 plus void            */
//   FUNC_4D_PLUS_VOID phi_xyzt_v;  /**< phi for dim 3 plus_void with time  */
//   
//   void*      clientdata;         /**< hold a pointer to a python object */
//   
//   FUN_TYPE type;                 /**< gives back the type of function stored im the object */
//
//} FUN_3D ;         /* to INITIALIZE in "Fun3D_get"          */






//#define FUN_1D_NULL  ((FUN_1D *)NULL)
//#define FUN_2D_NULL  ((FUN_2D *)NULL)
//#define FUN_3D_NULL  ((FUN_3D *)NULL) 

//#define FUN_1D_FREE(Fun) ( Fun1D_free(Fun), (Fun)=(FUN_1D *)NULL )
//#define FUN_2D_FREE(Fun) ( Fun2D_free(Fun), (Fun)=(FUN_2D *)NULL )
//#define FUN_3D_FREE(Fun) ( Fun3D_free(Fun), (Fun)=(FUN_3D *)NULL )

FUN_1D  *Fun1D_get(void);
FUN_2D  *Fun2D_get(void);
FUN_3D  *Fun3D_get(void);

int   Fun1D_free(FUN_1D *Fun);
int   Fun2D_free(FUN_2D *Fun);
int   Fun3D_free(FUN_3D *Fun);


FUN_1D * Fun1D_init                   ( FUN_1D* Fun);
FUN_1D * Fun1D_setCFunction           ( FUN_1D* Fun, FUNC_1D  phi);
FUN_1D * Fun1D_setCFunctionTransient  ( FUN_1D* Fun, FUNC_2D  phi);
Real     Fun1D_evalCFunction          ( FUN_1D* Fun, Real x            );
Real     Fun1D_evalCFunctionTransient ( FUN_1D* Fun, Real x, Real tps  );
Real     Fun1D_evalPyFunction         ( FUN_1D* Fun, Real x            );
Real     Fun1D_evalPyFunctionTransient( FUN_1D* Fun, Real x, Real tps  );

FUN_2D * Fun2D_init                   ( FUN_2D* Fun);
FUN_2D * Fun2D_setCFunction           ( FUN_2D* Fun, FUNC_2D  phi);
FUN_2D * Fun2D_setCFunctionTransient  ( FUN_2D* Fun, FUNC_3D  phi);
Real     Fun2D_evalCFunction          ( FUN_2D* Fun, Real x, Real y             );
Real     Fun2D_evalCFunctionTransient ( FUN_2D* Fun, Real x, Real y , Real tps  );
Real     Fun2D_evalPyFunction         ( FUN_2D* Fun, Real x, Real y             );
Real     Fun2D_evalPyFunctionTransient( FUN_2D* Fun, Real x, Real y , Real tps  );

FUN_3D * Fun3D_init                   ( FUN_3D* Fun);
FUN_3D * Fun3D_setCFunction           ( FUN_3D* Fun, FUNC_3D  phi);
FUN_3D * Fun3D_setCFunctionTransient  ( FUN_3D* Fun, FUNC_4D  phi);
Real     Fun3D_evalCFunction          ( FUN_3D* Fun, Real x , Real y , Real z            );
Real     Fun3D_evalCFunctionTransient ( FUN_3D* Fun, Real x , Real y , Real z , Real tps );
Real     Fun3D_evalPyFunction         ( FUN_3D* Fun, Real x , Real y , Real z            );
Real     Fun3D_evalPyFunctionTransient( FUN_3D* Fun, Real x , Real y , Real z , Real tps );


/*****************************************************************************/
//static char *my_names5[3] = { "FUN_1D" ,
//                              "FUN_2D" ,
//                              "FUN_3D" };

//static int (*my_frees5[3])(void*) = { (int(*)(void*))Fun1D_free ,
//                                      (int(*)(void*))Fun2D_free ,
//                                      (int(*)(void*))Fun3D_free };


//static MEM_ARRAY my_tnums5[3]; /* initialised to zeros */

/* ------------------------------------------------------------------- */

#define  NB_ERRS5                  4
#define  NB_WARNS5                 1

//static  char *my_err_mesg5[NB_ERRS5] =
//{
//   "fun type is not set (stationnaire or transient)",  /* 0 */
//   "fun_struct contains transient func",               /* 1 */
//   "fun_struct contains stationnary func",             /* 2 */
//   "fun_struct has a wrong type"                       /* 3 */
//};

//static char *my_warn_mesg5[NB_WARNS5] = 
//{
//   "unknown fun_struc warning"            /* 0 */
//};


#define	 E_FUN_UNKNOWN	           0
#define	 E_FUN_NOTATRANSIENTFUNC   1
#define  E_FUN_NOTASTATIONNARYFUNC 2
#define  E_FUN_WRONGTYPE           3

#define  W_FUN_UNKNOWN             0

/******************************************************************************/

int mem_attach_list5(void);
int err_attach_list5(void);
int warn_attach_list5(void);
int err_warn_attach_lists5(void);

/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 error5(err_num,fn_name) ev_err(__FILE__,err_num,__LINE__,fn_name,5)
/* error(E_TYPE,"myfunc") raises error type E_TYPE for function my_func() */
#define	 warning5(err_num,fn_name) ev_warn(__FILE__,err_num,__LINE__,fn_name,5)


#endif


//////////////////////////////////////////////////////////////////////
////  MACROS /////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

%inline %{


#undef  FUN_1D_FREE
#undef  FUN_2D_FREE
#undef  FUN_3D_FREE


void FUN_1D_FREE(FUN_1D* fun)
{ 
   Fun1D_free(fun);
   (fun)=(FUN_1D *)NULL;
}
void FUN_2D_FREE(FUN_2D* fun)
{ 
   Fun2D_free(fun);
   (fun)=(FUN_2D *)NULL;
}
void FUN_3D_FREE(FUN_3D* fun)
{ 
   Fun3D_free(fun);
   (fun)=(FUN_3D *)NULL;
}


int Fun1D_setFunctionPython(FUN_1D* Fun, PyObject *pyfunc)
{
	//printf("in <Fun1D_setFunctionPython>, pyfunc @ 0x%lx\n",pyfunc);
	//printf("in <Fun1D_setFunctionPython>,   func @ 0x%lx\n",PythonFunction1D);
	Fun1D_setFunction(Fun, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject1D, pyfunc);
	Py_INCREF(pyfunc);
	//printf("in <Fun1D_setFunctionPython>, A python function as been successfully set ... \n");
	return 0;
}
int Fun2D_setFunctionPython(FUN_2D* Fun, PyObject *pyfunc)
{
	//printf("in <Fun2D_setFunctionPython>, pyfunc @ 0x%lx\n",pyfunc);
	//printf("in <Fun2D_setFunctionPython>,   func @ 0x%lx\n",PythonFunction2D);
	Fun2D_setFunction(Fun, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject2D, pyfunc);
	Py_INCREF(pyfunc);
	//printf("in <Fun2D_setFunctionPython>, A python function as been successfully set ... \n");
	return 0;
}
int Fun3D_setFunctionPython(FUN_3D* Fun, PyObject *pyfunc)
{
	//printf("in <Fun3D_setFunctionPython>, pyfunc @ 0x%lx\n",pyfunc);
	//printf("in <Fun3D_setFunctionPython>,   func @ 0x%lx\n",PythonFunction3D);
	Fun3D_setFunction(Fun, FUN_PY_STATIONNARY, FunctionForEvaluatingPythonObject3D, pyfunc);
	Py_INCREF(pyfunc);
	//printf("in <Fun3D_setFunctionPython>, A python function as been successfully set ... \n");
	return 0;
}


int Fun1D_setFunctionTransientPython(FUN_1D* Fun, PyObject *pyfunc)
{
	//printf("in <Fun1D_setFunctionTransientPython>, pyfunc @ 0x%lx\n",pyfunc);
	//printf("in <Fun1D_setFunctionTransientPython>,   func @ 0x%lx\n",PythonFunction2D);
	Fun1D_setFunction(Fun, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject2D, pyfunc);
	Py_INCREF(pyfunc);
	//printf("in <Fun1D_setFunctionTransientPython>, A python function as been successfully set ... \n");
	return 0;
}
int Fun2D_setFunctionTransientPython(FUN_2D* Fun, PyObject *pyfunc)
{
	//printf("in <Fun2D_setFunctionTransientPython>, pyfunc @ 0x%lx\n",pyfunc);
	//printf("in <Fun2D_setFunctionTransientPython>,   func @ 0x%lx\n",PythonFunction3D);
	Fun2D_setFunction(Fun, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject3D, pyfunc);
	Py_INCREF(pyfunc);
	//printf("in <Fun2D_setFunctionTransientPython>, A python function as been successfully set ... \n");
	return 0;
}
int Fun3D_setFunctionTransientPython(FUN_3D* Fun, PyObject *pyfunc)
{
	//printf("in <Fun3D_setFunctionTransientPython>, pyfunc @ 0x%lx\n",pyfunc);
	//printf("in <Fun3D_setFunctionTransientPython>,   func @ 0x%lx\n",PythonFunction4D);
	Fun3D_setFunction(Fun, FUN_PY_TRANSIENT, FunctionForEvaluatingPythonObject4D, pyfunc);
	Py_INCREF(pyfunc);
	//printf("in <Fun3D_setFunctionTransientPython>, A python function as been successfully set ... \n");
	return 0;
}

%}