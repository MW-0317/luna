extern "C"
{
	#include <lua/lua.h>
	#include <lua/lauxlib.h>
	#include <lua/lualib.h>
}

#ifdef _WIN64
#pragma comment(lib, "liblua54.a")
#endif