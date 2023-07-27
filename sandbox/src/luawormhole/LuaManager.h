#pragma once

#include <main/luna.h>

#include <thirdparty/lua.h>
#include <thirdparty/sol/sol.hpp>

#include "LuaRender.h"

class LuaManager
{
public:
	static int WINDOW_WIDTH;
	static int WINDOW_HEIGHT;

	static LuaRender* luaRender;

	static void setLuaRender(LuaRender* luaRender);
	static void registerGlobals(sol::state* lua);
	static void registerFunctions(sol::state* lua);

	template<typename T>
	static void setGlobal(sol::state* lua, const char* name, T* callbackVar, T defaultVal);
};

