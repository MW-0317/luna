#pragma once

#include <main/luna.h>

#include <thirdparty/lua.h>
#include <thirdparty/sol/sol.hpp>

class LuaRender : public luna::Render
{
private:
	sol::state* lua;
public:
	LuaRender();
	LuaRender(sol::state* lua, const char* filename, float fps, float seconds,
		int width, int height);

	void frameUpdate(luna::Frame frame) override;
	void tickUpdate(luna::Tick tick) override;

	void setLua(sol::state* lua);
};

