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

	// Space
	luna_sol.set_function("createDebugLines",
		[]()
		{
			luaRender->getSpace(0)->createDebugLines();
		});

	luna_sol.set_function("setCameraFOV",
		[](float fov)
		{
			luaRender->getSpace(0)->getCamera()->setFOV(fov);
		});

	luna_sol.set_function("setCameraPosition",
		[](float x, float y, float z)
		{
			glm::vec3 pos = glm::vec3(x, y, z);
			luaRender->getSpace(0)->getCamera()->setPosition(pos);
		});

	luna_sol.set_function("setCameraLookAt", 
		[](float x, float y, float z)
		{
			luna::Camera* camera = luaRender->getSpace(0)->getCamera();
			glm::vec3 lookAt = glm::vec3(x, y, z);
			glm::vec3 pos = camera->getPosition();
			camera->setForwardVector(lookAt - pos);
		});
	
	// Objects
	luna_sol.new_usertype<luna::Object>("object",
		"createCube",			&luna::Object::createCube,
		"createSphere",			&luna::Object::createSphere,
		"setTexture",			&luna::Object::setTexture,
		"setCubeMapTexture",	&luna::Object::setCubeMap,
		"setRotation",			sol::resolve<void(float, float, float)>(
			&luna::Object::setRotation),
		"setPosition",			sol::resolve<void(float, float, float)>(
			&luna::Object::setPosition)
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
	if (globalVal.valid()) *callbackVar = globalVal;
	else *callbackVar = defaultVal;
}