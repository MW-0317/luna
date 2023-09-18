#pragma once

#include <main/luna.h>

#include <thirdparty/lua.h>
#include <thirdparty/sol/sol.hpp>

class LuaRender : public luna::Render
{
private:
	sol::state* lua;
public:
	LuaRender(sol::state* lua, const char* filename, float fps, float seconds,
		int width, int height);

	void frameUpdate(luna::FrameProps fp) override;
	void tickUpdate(luna::TickProps tp) override;
};

