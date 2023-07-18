//#define SOL_ALL_SAFETIES_ON 1

#include <main/luna.h>

#include "ArgParser.h"

#include <thirdparty/lua.h>
#include <thirdparty/sol/sol.hpp>

class LuaRender : public luna::Render
{
public:
	LuaRender(const char* filename, float fps, float seconds, int width, int height)
		: Render(filename, fps, seconds, width, height)
	{}

	void frameUpdate(float deltatime) override
	{
		return;
	}
};

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
	
	sol::state lua;
	lua.open_libraries(sol::lib::base);
	lua.script_file(luaFile);
	auto WINDOW_WIDTH_V = lua["WINDOW_WIDTH"];
	auto WINDOW_HEIGHT_V = lua["WINDOW_HEIGHT"];
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;
	if (WINDOW_WIDTH_V.valid()) WINDOW_WIDTH = WINDOW_WIDTH_V;
	if (WINDOW_HEIGHT_V.valid()) WINDOW_HEIGHT = WINDOW_HEIGHT_V;
	//luna::WINDOW_WIDTH, luna::WINDOW_HEIGHT
	LuaRender* r = new LuaRender(filename.c_str(), 30.0f, 2.0f,
		WINDOW_WIDTH, WINDOW_HEIGHT);
	r->run();
	r->save();

	//system("pause");
	return 0;
}