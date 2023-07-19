#pragma once

#include <thirdparty/lua.h>
#include <thirdparty/sol/sol.hpp>

class LuaManager
{
public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;

	static void registerGlobals(sol::state* lua);

	template<typename T>
	static void setGlobal(sol::state* lua, const char* name, T* callbackVar, T defaultVal);
};

