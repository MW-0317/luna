#pragma once
#include "core/Core.h"
#include "random/Random.h"

#include "Globals.h"
#include "Shader.h"

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

	class LUNA_API System
	{
	protected:
		std::vector<System*> systems;
	public:
		System();

		virtual void draw(Frame frame) {}

		virtual void frameUpdate(Frame frame) {}
		virtual void tickUpdate(Tick tick) {}

		void addSystem(System* system);
		void addSystem(System system);
		void tickSystems(Tick tick);
		void frameSystems(Frame frame);
	};
}