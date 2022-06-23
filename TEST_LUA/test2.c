#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

int main(int argc, char ** argv) {

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    /*
    if (luaL_dofile(L, "./script4.lua") == LUA_OK) {
        lua_pop(L, lua_gettop(L));
    }
    */

    /*
    if (luaL_dostring(L, "my_function = function(x,y) return math.cos(x) + x*y end") == LUA_OK) {
        lua_pop(L, lua_gettop(L));
    }
    */

    //char fundef[256] = "my_function = function(x,y) return x*y end";
    
    char fundef[256];
    char funspec[256] = "x*y + math.cos(x/4)";

    sprintf(fundef, "my_function = function(x,y) return %s end", funspec);


    //printf("fundef = \"%s\"\n", fundef);

    if (luaL_dostring(L, fundef) == LUA_OK) {
        lua_pop(L, lua_gettop(L));
    } else {
        printf("zut: lua_dostring\n");
    }

    // Put the function to be called onto the stack
    lua_getglobal(L, "my_function");
    lua_pushnumber(L, 3.141592653589793);  // first argument
    lua_pushnumber(L, 0.0);  // second argument

    // Execute my_function with 2 arguments and 1 return value
    if (lua_pcall(L, 2, 1, 0) == LUA_OK) {

        // Check if the return is an integer
        if (lua_isnumber(L, -1)) {

            // Convert the return value to integer
            double result = lua_tonumber(L, -1);

            // Pop the return value
            lua_pop(L, 1);
            printf("Result: %lf\n", result);
        }
        // Remove the function from the stack
        lua_pop(L, lua_gettop(L));
    } else {
        printf("zut: lua_pcall\n");
    }

    lua_close(L);
    return 0;
}
