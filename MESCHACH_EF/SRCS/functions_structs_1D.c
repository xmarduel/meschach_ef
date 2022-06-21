
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

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

FUN_1D *Fun1D_get(void)
{
   FUN_1D *Fun;
   
   if ( (Fun=NEW(FUN_1D)) == (FUN_1D *)NULL )
   {
      error(E_MEM, "Fun1D_get");
   }
   else if (mem_info_is_on())
   {
      mem_bytes_list(TYPE_FUN_1D, 0, sizeof(FUN_1D), MY_LIST5);
      mem_numvar_list(TYPE_FUN_1D, 1, MY_LIST5);
   }
   
   Fun1D_init(Fun);

   return  Fun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int Fun1D_free(FUN_1D *Fun)
{
   
   if ( Fun == FUN_1D_NULL )
   {
      return EXIT_FAILURE;
   }
   else
   {
      if (mem_info_is_on())
      {
         mem_bytes_list(TYPE_FUN_1D, sizeof(FUN_1D), 0, MY_LIST5);
         mem_numvar_list(TYPE_FUN_1D, -1, MY_LIST5);
      }
      
      free(Fun);  
      
      return EXIT_SUCCESS;
   }
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_1D *Fun1D_init ( FUN_1D *MyFun)
{
   if ( MyFun == NULL ) error(E_NULL, "Fun1D_init");

   MyFun->clientdata   = NULL;
   
   MyFun->phi_x        = NULL;
   MyFun->phi_xt       = NULL;
   MyFun->phi_x_v      = NULL;
   MyFun->phi_xt_v     = NULL;
      
   MyFun->type         = FUN_UNDEFINED;

   MyFun->eval         = NULL;
   
   return MyFun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_1D * Fun1D_setFunction(FUN_1D* MyFun, FUN_TYPE type, void* phi, void* clientdata)
{
   if ( MyFun == NULL ) error(E_NULL, "Fun1D_setFunction");
         
   /* set type */
   MyFun->type = type;

   /* assign members */
   switch(type)
   {
      case FUN_C_STATIONNARY:  MyFun->phi_x      = (FUNC_1D)phi;
                               MyFun->eval       = (FUN_1D_EVAL__FUN_1D_VAARGS)Fun1D_evalCFunction;
                               break;
      case FUN_C_TRANSIENT:    MyFun->phi_xt     = (FUNC_2D)phi;
                               MyFun->eval       = (FUN_1D_EVAL__FUN_1D_VAARGS)Fun1D_evalCFunctionTransient;
                               break;
      case FUN_PY_STATIONNARY: MyFun->phi_x_v    = (FUNC_1D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_1D_EVAL__FUN_1D_VAARGS)Fun1D_evalPyFunction;
                               break;
      case FUN_PY_TRANSIENT:   MyFun->phi_xt_v   = (FUNC_2D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_1D_EVAL__FUN_1D_VAARGS)Fun1D_evalPyFunctionTransient;
                               break;
	  case FUN_LUA_STATIONNARY: MyFun->phi_x_v    = (FUNC_1D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_1D_EVAL__FUN_1D_VAARGS)Fun1D_evalLUAFunction;
                               break;
      case FUN_LUA_TRANSIENT:  MyFun->phi_xt_v   = (FUNC_2D_PLUS_VOID)phi;
                               MyFun->clientdata = clientdata;
                               MyFun->eval       = (FUN_1D_EVAL__FUN_1D_VAARGS)Fun1D_evalLUAFunctionTransient;
                               break;

      default: error5(E_FUN_WRONGTYPE, "Fun1D_setFunction");
   }
   
   return  MyFun;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_1D * Fun1D_setCFunction(FUN_1D* MyFun, FUNC_1D phi)
{
   return Fun1D_setFunction( MyFun, FUN_C_STATIONNARY, phi , NULL);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_1D * Fun1D_setCFunctionTransient(FUN_1D* MyFun, FUNC_2D phi)
{
   return Fun1D_setFunction(MyFun, FUN_C_TRANSIENT, phi, NULL);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_1D * Fun1D_setLUAFunction(FUN_1D* Fun, const char* def) 
{
#ifdef HAVE_LUA
	lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    char* fundef[256];
	snprintf(fundef, 256, "fun = function(x)  return %s  end", def);
	
    if (luaL_dostring(L, fundef == LUA_OK) {
        lua_pop(L, lua_gettop(L));
    }
	
    Fun1D_setFunction(Fun, FUN_LUA_STATIONNARY, FunctionForEvaluatingLuaFunction1D, L);	
#endif
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

FUN_1D * Fun1D_setLUAFunctionTransient(FUN_1D* Fun, const char* def)
{
#ifdef HAVE_LUA
	lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    char* fundef[256];
	snprintf(fundef, 256, "fun = function(x, t)  return %s  end", def);
	
    if (luaL_dostring(L, fundef == LUA_OK) {
        lua_pop(L, lua_gettop(L));
    }
	
    Fun1D_setFunction(Fun, FUN_LUA_TRANSIENT, FunctionForEvaluatingLuaFunction2D, L);	
#endif
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun1D_evalCFunction     (const FUN_1D* MyFun, Real x)
{
   Real res = 0.0;
   
   if ( MyFun        == NULL ) error(E_NULL, "Fun1D_evalCFunction");
   if ( MyFun->phi_x == NULL ) error(E_NULL, "Fun1D_evalCFunction");

   /* eval function */
   res = MyFun->phi_x(x);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun1D_evalPyFunction     (const FUN_1D* MyFun, Real x)
{
   Real res = 0.0;

   if ( MyFun              == NULL ) error(E_NULL, "Fun1D_evalPyFunction");
   if ( MyFun->phi_x_v     == NULL ) error(E_NULL, "Fun1D_evalPyFunction");
   if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun1D_evalPyFunction");

   /* eval function */
   res = MyFun->phi_x_v(x, MyFun->clientdata);

   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun1D_evalCFunctionTransient  (const FUN_1D* MyFun, Real x, Real tps)
{
   Real res = 0.0;
   
   if ( MyFun         == NULL ) error(E_NULL, "Fun1D_evalFunctionTransient");
   if ( MyFun->phi_xt == NULL ) error(E_NULL, "Fun1D_evalFunctionTransient");

   /* eval function */   
   res = MyFun->phi_xt(x, tps);
         
   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real Fun1D_evalPyFunctionTransient     (const FUN_1D* MyFun, Real x, Real tps)
{
   Real res = 0.0;

   if ( MyFun              == NULL ) error(E_NULL, "Fun1D_evalPyFunctionTransient");
   if ( MyFun->phi_xt_v    == NULL ) error(E_NULL, "Fun1D_evalPyFunctionTransient");
   if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun1D_evalPyFunctionTransient");

   /* eval function */
   res = MyFun->phi_xt_v(x, tps, MyFun->clientdata);
   
   return res;
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

Real     Fun1D_evalLUAFunction           (const FUN_1D* Fun, Real x)
{  
#ifdef HAVE_LUA
    if ( MyFun              == NULL ) error(E_NULL, "Fun1D_evalLUAFunction");
    if ( MyFun->phi_x_v     == NULL ) error(E_NULL, "Fun1D_evalLUAFunction");
    if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun1D_evalLUAFunction");

	return MyFun->phi_xt_v(x, MyFun->clientdata);
#else
	return 0.0
#endif
}

Real     Fun1D_evalLUAFunctionTransient  (const FUN_1D* Fun, Real x, Real tps)
{
#ifdef HAVE_LUA
    if ( MyFun              == NULL ) error(E_NULL, "Fun1D_evalLUAFunctionTransient");
    if ( MyFun->phi_xt_v    == NULL ) error(E_NULL, "Fun1D_evalLUAFunctionTransient");
    if ( MyFun->clientdata  == NULL ) error(E_NULL, "Fun1D_evalLUAFunctionTransient");

	return MyFun->phi_xt_v(x, t, MyFun->clientdata);
#else
	return 0.0
#endif
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

// https://lucasklassmann.com/blog/2019-02-02-how-to-embeddeding-lua-in-c/

Real FunctionForEvaluatingLuaFunction1D(Real x, void *L)
{
	Real result = 0.0;

#ifdef HAVE_LUA
    lua_State *L = (lua_State *)LL;

    // Put the function to be called onto the stack
    lua_getglobal(L, "fun");
    lua_pushdouble(L, x);  // first argument

    // Execute my_function with 2 arguments and 1 return value
    if (lua_pcall(L, 1, 1, 0) == LUA_OK) {

        // Check if the return is an integer
        if (lua_isdouble(L, -1)) {

            // Convert the return value to integer
            result = lua_todouble(L, -1);

            // Pop the return value
            lua_pop(L, 1);
            printf("Result: %d\n", result);
        }
        // Remove the function from the stack
        lua_pop(L, lua_gettop(L));
    }
#endif

	return result;
}

Real FunctionForEvaluatingLuaFunction2D(Real x, Real t, void *LL)
{
	Real result = 0.0;

#ifdef HAVE_LUA
    lua_State *L = (lua_State *)LL;
	
    // Put the function to be called onto the stack
    lua_getglobal(L, "fun");
    lua_pushdouble(L, x);  // first argument
    lua_pushdouble(L, t);  // second argument

    // Execute my_function with 2 arguments and 1 return value
    if (lua_pcall(L, 2, 1, 0) == LUA_OK) {

        // Check if the return is an integer
        if (lua_isdouble(L, -1)) {

            // Convert the return value to integer
            result = lua_todouble(L, -1);

            // Pop the return value
            lua_pop(L, 1);
            printf("Result: %d\n", result);
        }
        // Remove the function from the stack
        lua_pop(L, lua_gettop(L));
    }
#endif

	return result;
}

