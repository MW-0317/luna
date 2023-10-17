#include <main/luna.h>
#include "LuaManager.h"

int LuaManager::WINDOW_WIDTH;
int LuaManager::WINDOW_HEIGHT;

LuaRender* LuaManager::luaRender;

void LuaManager::setLuaRender(LuaRender* luaRender)
{
	LuaManager::luaRender = luaRender;
}

void LuaManager::registerGlobals(sol::state* lua)
{
	setGlobal(lua, "WINDOW_WIDTH", &WINDOW_WIDTH, 800);
	setGlobal(lua, "WINDOW_HEIGHT", &WINDOW_HEIGHT, 600);
}

void LuaManager::registerFunctions(sol::state* lua)
{
	// Namespacing
	auto luna_sol = (*lua)["luna"].get_or_create<sol::table>();

	// Engine
	luna_sol.new_usertype<luna::System>("system");

	luna_sol.set_function("clearColor",
		&LuaRender::clearColor);

	luna_sol.set_function("setSize",
		[](float width, float height)
		{

		});

	// Space
	luna_sol.set_function("createDebugLines",
		[]()
		{
			luaRender->getSpace(0)->createDebugLines();
		});
	
	// Objects
	luna_sol.new_usertype<luna::Object>("object",
		"createCube", &luna::Object::createCube,
		"setTexture", &luna::Object::setTexture,
		"setRotation", sol::resolve<void(float, float, float)>(&luna::Object::setRotation)
	);

	/*
	luna_sol.set_function("addObject", 
		sol::resolve<void(luna::Object)>(&luna::Space::addSystem),
		luaRender->getSpace(0));
		*/

	luna_sol.set_function("addObject",
		[](luna::Object& obj) 
		{
			luaRender->getSpace(0)->addSystem(&obj);
		});
	
}

template<typename T>
void LuaManager::setGlobal(sol::state* lua, const char* name, T* callbackVar, T defaultVal)
{
	auto globalVal = (*lua)[name];
	std::cout << globalVal.valid() << std::endl;
	if (globalVal.valid()) *callbackVar = globalVal;
	else *callbackVar = defaultVal;
}