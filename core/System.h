#pragma once
#include "core/Core.h"
#include "random/Random.h"

#include "Globals.h"

namespace luna
{
	class Random;
	class Engine;
	class Space;
	class Camera;
	class Object;
	struct Interval
	{
		float deltatime;
		Random* rng;
		Engine* engine;
		Space* space;
		Camera* camera;
		Object* object;

		int width;
		int height;
	};

	struct Frame : public Interval
	{
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct Tick : public Interval
	{

	};

	class System
	{
	public:
		LUNA_API virtual void frameUpdate(Frame frame) {}
		LUNA_API virtual void tickUpdate(Tick tick) {}
	};
}