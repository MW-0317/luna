#include "LuaManager.h"

int LuaManager::WINDOW_WIDTH;
int LuaManager::WINDOW_HEIGHT;

void LuaManager::registerGlobals(sol::state* lua)
{
	setGlobal(lua, "WINDOW_WIDTH", &WINDOW_WIDTH, 800);
	setGlobal(lua, "WINDOW_HEIGHT", &WINDOW_HEIGHT, 600);
}

void LuaManager::registerFunctions(sol::state* lua)
{
	(*lua)["clearColor"] = &LuaRender::clearColor;
}

template<typename T>
void LuaManager::setGlobal(sol::state* lua, const char* name, T* callbackVar, T defaultVal)
{
	auto globalVal = (*lua)[name];
	if (globalVal.valid()) *callbackVar = globalVal;
	else *callbackVar = defaultVal;
}