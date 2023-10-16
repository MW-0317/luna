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
	class LUNA_API Space : public System
	{
	private:
		Random* rng;

		Camera* currentCamera;
		std::vector<Camera> cameras;

		float deltaframe = 0.0f;
		float lastframe = 0.0f;

		float deltatick = 0.0f;
		float lasttick = 0.0f;

		int width, height, length;

		//std::vector<Object*>	objects;
		std::vector<Primitive*>	primitives;
	public:
		void init();
		Space();
		Space(Camera* camera);
		~Space();

		void frameUpdate(Frame frame) override;
		// Returns true if a tick can be processed outside of the Space class.
		void tickUpdate(Tick tick) override;

		Camera* getCamera();
		float getDelta();
		Random* getRandom();

		void draw(Frame frame);

		//LUNA_API void addObject(Object* object);
		void addPrimitive(Primitive* primitive);

		void createDebugLines();
	};
}