#pragma once
#include "Core.h"
#include "Globals.h"
#include "Camera.h"
#include "Object.h"
#include "System.h"
#include "random/Random.h"
#include <vector>

namespace luna
{
	class Space
	{
	private:
		Random* rng;

		float const MAX_TPS = 60;
		float const INV_TPS = 1 / MAX_TPS;

		Camera* currentCamera;
		std::vector<Camera> cameras;

		float deltaframe = 0.0f;
		float lastframe = 0.0f;

		float deltatick = 0.0f;
		float lasttick = 0.0f;

		int width, height, length;

		std::vector<Object*>	objects;
		std::vector<System*>	systems;
		std::vector<Cell*>		cells;
	public:
		void init();
		Space();
		Space(Camera* camera);
		~Space();

		void frameUpdate(RenderProps renderProps);
		// Returns true if a tick can be processed outside of the Space class.
		bool tickUpdate();
		float getDeltaTick();

		Camera* getCamera();
		float getDelta();
		Random* getRandom();

		void draw(RenderProps renderProps);

		LUNA_API void addObject(Object* object);
		LUNA_API void addSystem(System* system);
		LUNA_API void addCell(Cell* cell);

		LUNA_API void createDebugLines();
	};
}