#include "LuaRender.h"

LuaRender::LuaRender() : Render()
{
	lua = nullptr;
}

LuaRender::LuaRender(sol::state* lua, const char* filename, float fps, float seconds, 
	int width, int height)
	: Render(filename, fps, seconds, width, height, false)
{
	this->lua = lua;
}

void LuaRender::frameUpdate(luna::Frame frame)
{
	// Call lua update() if it exists
	sol::protected_function update = (*lua)["update"];
	sol::protected_function_result result;
	if (update.valid()) result = update(frame.deltatime);
	luna::Render::frameUpdate(frame);
}

void LuaRender::tickUpdate(luna::Tick tick)
{
	sol::protected_function tickUpdate = (*lua)["tickUpdate"];
	if (tickUpdate.valid()) tickUpdate(tick.deltatime);
	luna::Render::tickUpdate(tick);
}

void LuaRender::setLua(sol::state* lua)
{
	this->lua = lua;
}