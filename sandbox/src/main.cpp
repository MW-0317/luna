#define SOL_ALL_SAFETIES_ON 1
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include <main/luna.h>

#include <thirdparty/lua.h>
#include <thirdparty/sol/sol.hpp>

#include "ArgParser.h"
#include "luawormhole/LuaManager.h"
#include "luawormhole/LuaRender.h"

int main(int argc, char* argv[])
{
	ArgParser arguments = ArgParser(argc, argv);
	std::string luaFile = arguments.getArgument("input");
	std::string filename = arguments.getArgument("output");
	if (luaFile == "")
	{
		std::cout << "Please enter a valid input" << std::endl;
		return -1;
	}
	if (filename == "")
	{
		std::cout << "Please enter a valid output" << std::endl;
		return -1;
	}
	
	sol::state* lua = new sol::state();
	lua->open_libraries(sol::lib::base);

	LuaManager::registerGlobals(lua);
	LuaManager::registerFunctions(lua);

	LuaRender* r = new LuaRender(lua, filename.c_str(), 30.0f, 2.0f,
		LuaManager::WINDOW_WIDTH, LuaManager::WINDOW_HEIGHT);

	auto luaR = lua->script_file(luaFile, [](lua_State* L, sol::protected_function_result pfr) {
		return pfr;
	});
	if (!luaR.valid()) return -1;

	r->run();
	r->save();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	system("pause");
	//return 0;
}