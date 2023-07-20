#include "LuaRender.h"

LuaRender::LuaRender(sol::state* lua, const char* filename, float fps, float seconds, 
	int width, int height)
	: Render(filename, fps, seconds, width, height)
{
	this->lua = lua;
}

void LuaRender::frameUpdate(float deltatime)
{
	// Call lua update() if it exists
	auto update = (*lua)["update"];
	if (update.valid()) update();
	return;
}