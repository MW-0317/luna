#pragma once
#ifndef LUNA_ENGINE_H
#define LUNA_ENGINE_H

#include "core/Core.h"
#include "core/Space.h"

namespace luna
{
	LUNA_API class Engine
	{
	protected:
		GLFWwindow* window;
		Space* space;

		int width, height;

		bool testWindow = true;

		LUNA_API virtual void init(int width, int height);
	private:
		LUNA_API virtual void createWindow(int width, int height);
	public:
		LUNA_API Engine(int width, int height);
		Engine();
		~Engine();

		LUNA_API int run();
		virtual void mainFrameUpdate(float deltatime);
		LUNA_API virtual void frameUpdate(float deltatime);

		LUNA_API static void clearColor(float r, float g, float b, float a);
	};
}

#endif // LUNA_ENGINE_H