#include "LuaRender.h"

LuaRender::LuaRender(sol::state* lua, const char* filename, float fps, float seconds, 
	int width, int height)
	: Render(filename, fps, seconds, width, height, true)
{
	this->lua = lua;
}

void LuaRender::frameUpdate(luna::FrameProps fp)
{
	// Call lua update() if it exists
	auto update = (*lua)["update"];
	if (update.valid()) update();
	return;
}

void LuaRender::tickUpdate(luna::TickProps tp)
{
	auto tickUpdate = (*lua)["tickUpdate"];
	if (tickUpdate.valid()) tickUpdate();
	return;
}