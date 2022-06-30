
/*
 * easy administration for the structures "functions"
 *
 */

#include "MESCHACH/INCLUDES/matrix.h"
#include "MESCHACH/INCLUDES/sparse.h"

#include "MESCHACH_EF/INCLUDES/all_params.h"   /* for axes enumerations */
#include "MESCHACH_EF/INCLUDES/functions_structs.h"
#include "MESCHACH_EF/INCLUDES/functions_definitions.h"

#ifdef HAVE_LUA
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#endif

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int mem_attach_list5(void)
{
   return mem_attach_list(MY_LIST5, NB_STRUCTS5, my_names5, my_frees5, my_tnums5);
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int err_attach_list5(void)
{
   return err_list_attach(MY_LIST5, NB_ERRS5, my_err_mesg5 );
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int warn_attach_list5(void)
{
   return warn_list_attach(MY_LIST5, NB_WARNS5, my_warn_mesg5 );
}

/* ------------------------------------------------------------------------------------------------ */
/* ------------------------------------------------------------------------------------------------ */

int err_warn_attach_lists5(void)
{
   err_attach_list5();
   warn_attach_list5();

   return 1;
}

/*-----------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------*/

// https://lucasklassmann.com/blog/2019-02-02-how-to-embeddeding-lua-in-c/

Real FunctionForEvaluatingLuaFunction1D(Real x, void *LL)
{
   Real result = 0.0;

#ifdef HAVE_LUA
   lua_State *L = (lua_State *)LL;

   // Put the function to be called onto the stack
   lua_getglobal(L, "fun");
   lua_pushnumber(L, x);  // first argument

   // Execute my_function with 2 arguments and 1 return value
   if (lua_pcall(L, 1, 1, 0) == LUA_OK)
   {
      // Check if the return is an integer
      if (lua_isnumber(L, -1))
      {
         // Convert the return value to integer
         result = lua_tonumber(L, -1);

         // Pop the return value
         lua_pop(L, 1);
         //printf("Result: %lf\n", result);
      }
      // Remove the function from the stack
      lua_pop(L, lua_gettop(L));
   }
#endif

   return result;
}

Real FunctionForEvaluatingLuaFunction2D(Real x, Real y, void *LL)
{
   Real result = 0.0;

#ifdef HAVE_LUA
   lua_State *L = (lua_State *)LL;

   // Put the function to be called onto the stack
   lua_getglobal(L, "fun");
   lua_pushnumber(L, x);  // first argument
   lua_pushnumber(L, y);  // second argument

   // Execute my_function with 2 arguments and 1 return value
   if (lua_pcall(L, 2, 1, 0) == LUA_OK)
   {
      // Check if the return is an integer
      if (lua_isnumber(L, -1))
      {
         // Convert the return value to integer
         result = lua_tonumber(L, -1);

         // Pop the return value
         lua_pop(L, 1);
         //printf("Result: %lf\n", result);
      }
      // Remove the function from the stack
      lua_pop(L, lua_gettop(L));
   }
#endif

   return result;
}

Real FunctionForEvaluatingLuaFunction3D(Real x, Real y, Real z, void *LL)
{
   Real result = 0.0;

#ifdef HAVE_LUA
   lua_State *L = (lua_State *)LL;

   // Put the function to be called onto the stack
   lua_getglobal(L, "fun");
   lua_pushnumber(L, x);  // first argument
   lua_pushnumber(L, y);  // second argument
   lua_pushnumber(L, z);  // third argument

   // Execute my_function with 3 arguments and 1 return value
   if (lua_pcall(L, 3, 1, 0) == LUA_OK)
   {
      // Check if the return is an integer
      if (lua_isnumber(L, -1))
      {
         // Convert the return value to integer
         result = lua_tonumber(L, -1);

         // Pop the return value
         lua_pop(L, 1);
         //printf("Result: %lf\n", result);
      }
      // Remove the function from the stack
      lua_pop(L, lua_gettop(L));
   }
#endif

   return result;
}

Real FunctionForEvaluatingLuaFunction4D(Real x, Real y, Real z, Real t, void *LL)
{
   Real result = 0.0;

#ifdef HAVE_LUA
   lua_State *L = (lua_State *)LL;

   // Put the function to be called onto the stack
   lua_getglobal(L, "fun");
   lua_pushnumber(L, x);  // first argument
   lua_pushnumber(L, y);  // second argument
   lua_pushnumber(L, z);  // third argument
   lua_pushnumber(L, t);  // fourth argument

   // Execute my_function with 4 arguments and 1 return value
   if (lua_pcall(L, 4, 1, 0) == LUA_OK)
   {
      // Check if the return is an integer
      if (lua_isnumber(L, -1))
      {
         // Convert the return value to integer
         result = lua_tonumber(L, -1);

         // Pop the return value
         lua_pop(L, 1);
         //printf("Result: %d\n", result);
      }
      // Remove the function from the stack
      lua_pop(L, lua_gettop(L));
   }
#endif

   return result;
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------

void * make_lua_interpreter(const char* def, const char *fun_type)
{
#ifdef HAVE_LUA
   lua_State *L = luaL_newstate();
   luaL_openlibs(L);

   char fundef[256];

   if (strcmp(fun_type, "1D") == 0) {
      snprintf(fundef, 256, "fun = function(x)  return %s  end", def);
   }
   if (strcmp(fun_type, "2D") == 0) {
      snprintf(fundef, 256, "fun = function(x, y)  return %s  end", def);
   }
   if (strcmp(fun_type, "3D") == 0) {
      snprintf(fundef, 256, "fun = function(x, y, z)  return %s  end", def);
   }
   if (strcmp(fun_type, "1D_TR") == 0) {
      snprintf(fundef, 256, "fun = function(x, t)  return %s  end", def);
   }
   if (strcmp(fun_type, "2D_TR") == 0) {
      snprintf(fundef, 256, "fun = function(x, y, t)  return %s  end", def);
   }
   if (strcmp(fun_type, "3D_TR") == 0) {
      snprintf(fundef, 256, "fun = function(x, y, z, t)  return %s  end", def);
   }

   if (luaL_dostring(L, fundef) == LUA_OK)
   {
      lua_pop(L, lua_gettop(L));
   }

   return L;
#else
   return NULL;
#endif
}

// ------------------------------------------------------------------------
// ------------------------------------------------------------------------
