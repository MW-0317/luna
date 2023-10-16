#pragma once
#ifndef LUNA_ENGINE_H
#define LUNA_ENGINE_H

#include "core/Core.h"
#include "core/Space.h"
#include "core/System.h"

namespace luna
{
	LUNA_API class Engine : public virtual System
	{
	protected:
		bool debug;

		GLFWwindow* window;
		std::vector<Space*> spaces;

		int width, height;

		bool exampleWindow = true;

		LUNA_API virtual void init(int width, int height);
	private:
		float const MAX_TPS = 60;
		float const INV_TPS = 1 / MAX_TPS;

		LUNA_API virtual void createWindow(int width, int height);
	public:
		LUNA_API Engine(int width, int height);
		Engine();
		~Engine();

		LUNA_API void enableDebug() { this->debug = true; }
		LUNA_API void disableDebug() { this->debug = false; }

		LUNA_API int run();
		virtual void mainFrameUpdate(Frame frame);
		LUNA_API virtual void frameUpdate(Frame frame) override;
		// Deltatime should be near zero, however could fluctuate.
		LUNA_API void mainTickUpdate(Tick tick);
		LUNA_API virtual void tickUpdate(Tick tick) override;

		LUNA_API Space* getSpace(int index);

		LUNA_API static void clearColor(float r, float g, float b, float a);
	};
}

#endif // LUNA_ENGINE_H