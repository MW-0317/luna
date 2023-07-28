#pragma once
#include "Core.h"
#include "Globals.h"
#include "Camera.h"
#include "Object.h"
#include "System.h"
#include <vector>

using namespace luna;

class Space
{
private:
	float const MAX_TPS = 60;
	float const INV_TPS = 1 / MAX_TPS;

	Camera* currentCamera;
	std::vector<Camera> cameras;

	float deltaframe = 0.0f;
	float lastframe = 0.0f;

	float deltatick = 0.0f;
	float lasttick = 0.0f;

	int width, height, length;

	std::vector<Object> objects;
	std::vector<System> systems;

public:
	void init();
	Space();
	Space(Camera* camera);
	~Space();

	void frameUpdate();
	// Returns true if a tick can be processed outside of the Space class.
	bool tickUpdate();
	float getDeltaTick();

	Camera* getCamera();
	float getDelta();

	void draw();

	LUNA_API void addObject(Object object);
	LUNA_API void addSystem(System system);
};