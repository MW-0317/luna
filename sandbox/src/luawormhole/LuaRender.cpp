#include "LuaRender.h"

LuaRender::LuaRender(sol::state* lua, const char* filename, float fps, float seconds, 
	int width, int height)
	: Render(filename, fps, seconds, width, height, false)
{
	this->lua = lua;
}

void LuaRender::frameUpdate(luna::Frame frame)
{
	// Call lua update() if it exists
	auto update = (*lua)["update"];
	if (update.valid()) update();
	luna::Render::frameUpdate(frame);
}

void LuaRender::tickUpdate(luna::Tick tick)
{
	auto tickUpdate = (*lua)["tickUpdate"];
	if (tickUpdate.valid()) tickUpdate();
}