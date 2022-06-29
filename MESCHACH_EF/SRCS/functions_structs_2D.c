
/*
 * easy administration for the structures "functions"
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"   /* for axes enumerations */
#include "MESCHACH_EF/INCLUDES/functions_structs.h"

#ifdef HAVE_LUA
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_2D *Fun2D_get(void)
{
   FUN_2D *Fun;

   if ( (Fun=NEW(FUN_2D)) == (FUN_2D *)NULL )
   {
      error(E_MEM, "Fun2D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_FUN_2D, 0, sizeof(FUN_2D), MY_LIST5);
      mem_numvar_list(TYPE_FUN_2D, 1, MY_LIST5);
   }

   Fun2D_init(Fun);

   return  Fun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int Fun2D_free(FUN_2D *Fun)
{

   if ( Fun == FUN_2D_NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_FUN_2D, sizeof(FUN_2D), 0, MY_LIST5);
         mem_numvar_list(TYPE_FUN_2D, -1, MY_LIST5);
      }

      free(Fun);

      return EXIT_SUCCESS;
   }
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_2D *Fun2D_init(FUN_2D *MyFun)
{
   if ( MyFun == NULL ) error(E_NULL, "Fun2D_init");

   MyFun->clientdata    = NULL;

   MyFun->phi_xy        = NULL;
   MyFun->phi_xyt       = NULL;
   MyFun->phi_xy_v      = NULL;
   MyFun->phi_xyt_v     = NULL;

   MyFun->type          = FUN_UNDEFINED;

   MyFun->eval          = NULL;

   return MyFun;
}

/* ------------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------------ */

FUN_2D * Fun2D_setFunction(FUN_2D* MyFun, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyFun == NULL ) error(E_NULL, "Fun2D_setFunction");

   /* set type */
   MyFun->type = type;

   /* assign members */
   switch(type)
   {
      case FUN_C_STATIONNARY:  MyFun->phi_xy     = (FUNC_2D)phi;
                               MyFun->eval       = (FUN_2D_EVAL__FUN_2D_VAARGS)Fun2D_evalCFunction;
                               break;
      case FUN_C_TRANSIENT:    MyFun->phi_xyt    = (FUNC_3D)phi;
                               MyFun->eval       = (FUN_2D_EVAL__FUN_2D_VAARGS)Fun2D_evalCFunctionTransient;
                               break;
      case FUN_PY_STATIONNARY: MyFun->phi_xy_v   = (FUNC_2D_PLUS_VOID)phi;
                               MyFun->eval       = (FUN_2D_EVAL__FUN_2D_VAARGS)Fun2D_evalPyFunction;
                               MyFun->clientdata = clientdata;
                               break;
      case FUN_PY_TRANSIENT:   MyFun->phi_xyt_v  = (FUNC_3D_PLUS_VOID)phi;
                               MyFun->eval       = (FUN_2D_EVAL__FUN_2D_VAARGS)Fun2D_evalPyFunctionTransient;
                               MyFun->clientdata = clientdata;
                               break;
      case FUN_LUA_STATIONNARY: MyFun->phi_xy_v  = (FUNC_2D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_2D_EVAL__FUN_2D_VAARGS)Fun2D_evalLUAFunction;
                               break;
      case FUN_LUA_TRANSIENT:  MyFun->phi_xyt_v  = (FUNC_3D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_2D_EVAL__FUN_2D_VAARGS)Fun2D_evalLUAFunctionTransient;
                               break;
      default: error5(E_FUN_WRONGTYPE, "Fun2D_setFunction");
   }

   return  MyFun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_2D * Fun2D_setCFunction(FUN_2D* MyFun, FUNC_2D phi)
{
   return Fun2D_setFunction(MyFun, FUN_C_STATIONNARY, phi, NULL);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_2D * Fun2D_setCFunctionTransient( FUN_2D* MyFun, FUNC_3D phi)
{
   return Fun2D_setFunction(MyFun, FUN_C_TRANSIENT, phi, NULL);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_2D * Fun2D_setLUAFunction(FUN_2D* Fun, const char* def)
{
#ifdef HAVE_LUA
   lua_State *L = make_lua_interpreter(def, "2D");

   return Fun2D_setFunction(Fun, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction2D, L);
#else
   return Fun;
#endif
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_2D * Fun2D_setLUAFunctionTransient(FUN_2D* Fun, const char* def)
{
#ifdef HAVE_LUA
   lua_State *L = make_lua_interpreter(def, "2D_TR");

   return Fun2D_setFunction(Fun, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction3D, L);
#else
   return Fun;
#endif
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun2D_evalCFunction     (const FUN_2D* MyFun, Real x, Real y)
{
   Real res = 0.0;

   if ( MyFun         == NULL ) error(E_NULL, "Fun2D_evalCFunction");
   if ( MyFun->phi_xy == NULL ) error(E_NULL, "Fun2D_evalCFunction");

   /* eval function */
   res = MyFun->phi_xy(x, y);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun2D_evalPyFunction     (const FUN_2D* MyFun, Real x, Real y)
{
   Real res = 0.0;

   if ( MyFun              == NULL ) error(E_NULL, "Fun2D_evalPyFunction");
   if ( MyFun->phi_xy_v    == NULL ) error(E_NULL, "Fun2D_evalPyFunction");
   if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun2D_evalPyFunction");

   /* eval function */
   res = MyFun->phi_xy_v(x, y, MyFun->clientdata);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun2D_evalLUAFunction     (const FUN_2D* MyFun, Real x, Real y)
{
#ifdef HAVE_LUA
   if ( MyFun              == NULL ) error(E_NULL, "Fun2D_evalLUAFunction");
   if ( MyFun->phi_xy_v    == NULL ) error(E_NULL, "Fun2D_evalLUAFunction");
   if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun2D_evalLUAFunction");

   return MyFun->phi_xy_v(x, y, MyFun->clientdata);
#else
   return 0.0;
#endif
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun2D_evalCFunctionTransient  (const FUN_2D* MyFun, Real x, Real y, Real tps)
{
   Real res = 0.0;

   if ( MyFun          == NULL ) error(E_NULL, "Fun2D_evalCFunctionTransient");
   if ( MyFun->phi_xyt == NULL ) error(E_NULL, "Fun2D_evalCFunctionTransient");

   /* eval function */
   res = MyFun->phi_xyt(x, y, tps);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun2D_evalPyFunctionTransient     (const FUN_2D* MyFun, Real x, Real y, Real tps)
{
   Real res = 0.0;

   if ( MyFun              == NULL ) error(E_NULL, "Fun2D_evalPyFunctionTransient");
   if ( MyFun->phi_xyt_v   == NULL ) error(E_NULL, "Fun2D_evalPyFunctionTransient");
   if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun2D_evalPyFunctionTransient");

   /* eval function */
   res = MyFun->phi_xyt_v(x, y, tps, MyFun->clientdata);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun2D_evalLUAFunctionTransient  (const FUN_2D* MyFun, Real x, Real y, Real tps)
{
#ifdef HAVE_LUA
   if ( MyFun              == NULL ) error(E_NULL, "Fun2D_evalLUAFunctionTransient");
   if ( MyFun->phi_xyt_v   == NULL ) error(E_NULL, "Fun2D_evalLUAFunctionTransient");
   if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun2D_evalLUAFunctionTransient");

   return MyFun->phi_xyt_v(x, y, tps, MyFun->clientdata);
#else
   return 0.0;
#endif
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */
