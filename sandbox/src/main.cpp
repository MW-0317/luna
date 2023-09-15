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
	/*
	LuaRender* r = new LuaRender(lua, filename.c_str(), 30.0f, 2.0f,
		LuaManager::WINDOW_WIDTH, LuaManager::WINDOW_HEIGHT);

	LuaManager::setLuaRender(r);
	LuaManager::registerFunctions(lua);

	auto luaR = lua->script_file(luaFile, [](lua_State* L, sol::protected_function_result pfr) {
		return pfr;
	});
	if (!luaR.valid()) return -1;

	r->run();
	r->save();
	*/

	/*
	Random r = Random();
	while (true)
	{
		double randomVal = r.generateUniformReal<double>();
		std::cout << randomVal << std::endl;
	}
	*/

	// FOR DEBUG REASONS ONLY
	/*
	Engine* e = new Engine(LuaManager::WINDOW_WIDTH, LuaManager::WINDOW_HEIGHT);
	Engine::clearColor(0.5f, 0.5f, 0.5f, 1.0f);
	//e->getSpace()->addObject(Object::createSquare());
	ParticleSystemProps psp;
	psp.mesh = Mesh::createSquare();
	//psp.shader = Shader::getDefaultShader();
	psp.shader = Shader::getBillboardShader();
	psp.position = glm::vec3(0.0f);
	psp.scale = glm::vec3(1.0f);
	ParticleSystem* p = new ParticleSystem(psp);
	Particle particle;
	particle.sizeBegin = 1.0f;
	particle.sizeEnd = 0.0f;
	particle.velocity = glm::vec3(0.0f, 0.001f, 0.0f);
	p->addParticle(particle);
	ParticleSpawner ps;
	ps.count = 1000;
	ps.sizeBegin = 0.1f;
	ps.sizeEnd = 0.0f;
	ps.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	ps.velocityVariation = glm::vec3(20.0f);
	ps.acceleration = glm::vec3(-50.0f, 50.0f, 0.0f);
	p->createSpawner(ps);
	e->getSpace()->addSystem(p);
	e->getSpace()->addObject(p);
	e->run();
	*/

	// Todo for engine
	/*
		- Must add createCube and createSphere for meshes and objects.
		- Must add a .obj parser since it seems easiest to parse.
			- Can later add a 3d model parser, but still want to write
				the .obj parser.
		- Must create own shader code that defaults to some basic shader.
			- Shader environment that mounts on top of GLSL.
		- Initalization of OpenGL should exist outside the creation
			of the engine/render/game.
	*/
	
	// Todo for erosion/dissintegration effect
	/*
		Implement better Textures
			- Allow the inclusion of as many textures into a shader as possible.
			- Give meshes a texture array and generate the appropriate buffers
				based on the array index (keeping in mind the max is 16 textures).
			- Need to figure out how to render transparent/opaque test given a noise
				texture.
		Figure out how to send information from the shader back to the gpu
			(to get screen space coordinates).
			- I'm assuming this is done with some buffer that can be written to.
			- Need to figure out how to send the information to spawn a
				particle once. I don't want the particle to spawn every frame
				after the object has dissappeared.
		Convert screen space coordinates to world space coordinates.
			- This should be trivial (I know I'll regret this wording)
			since I know where the camera and object are positioned, 
			however difference in space along the object could trip me up
			since I only know the difference from the objects center.
			- Could also use some depth buffer?
		Spawn particle
	*/

	/*
		Steps for Dissintegration effect
		1. Use and send noise texture to the shader.
		2. Create SSBO (shader storage buffer object) and send to shader.
		3. Send the deltatime and current game time to the shader.
			(Maybe start using a UBO (uniform buffer object) to send
			large amounts of data)
		4. Compare the game time with modulo against the noise texture.
			4a. If the texture is less than the game time, remove the fragment.
		5. If the current fragment will be removed next frame, by adding
			deltatime to the current time, send the pixel coordinates to
			the SSBO.
		6. Somehow read the SSBO back in C++ and use the depth buffer to
			extract 3d coordinates from the camera normal.
			6a. Need to figure out reading the SSBO in C++ and using
			a depth buffer.
	*/

	// ENGINE MUST BE CALLED BEFORE ALL OTHER FUNCTIONS TO INITIALIZE
	// OpenGL functions. This must be changed in later versions.
	Engine* e = new Engine(LuaManager::WINDOW_WIDTH, LuaManager::WINDOW_HEIGHT);
	Object square = Object::createSquare();
	std::vector<const char*> paths = {
		"./resources/textures/default.png",
		"./resources/textures/Milky 5 - 128x128.png"
	};
	Engine::clearColor(0.5f, 0.5f, 0.5f, 1.0f);
	e->getSpace()->addObject(&square);
	e->run();

	


	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();
	system("pause");
	//return 0;
}