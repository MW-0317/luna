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
	public:
		LUNA_API Engine(int width, int height);
		~Engine();

		LUNA_API int run();
		virtual void mainFrameUpdate(float deltatime);
		LUNA_API virtual void frameUpdate(float deltatime);
	};
}

#endif // LUNA_ENGINE_H