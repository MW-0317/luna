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
		int windowHeight = 600;
		int windowWidth = 800;
		GLFWwindow* window;

		Space* space;

		bool testWindow = true;
	public:
		LUNA_API Engine();
		~Engine();

		LUNA_API int run();
		void frameUpdate();
	};
}

#endif // LUNA_ENGINE_H