#pragma once
#ifndef LUNA_ENGINE_H
#define LUNA_ENGINE_H

#include "core/Core.h"
#include "core/Space.h"
#include "core/System.h"

#include "logs/ImguiLogs.h"

namespace luna
{
	LUNA_API class Engine : public virtual System
	{
	protected:
		bool debug;

		GLFWwindow* window;
		Space* space;

		int width, height;

		bool exampleWindow = true;

		LUNA_API virtual void init(int width, int height);
	private:
		LUNA_API virtual void createWindow(int width, int height);
	public:
		DefaultLog log;
		Overlay oLog;

		LUNA_API Engine(int width, int height);
		Engine();
		~Engine();

		LUNA_API void enableDebug() { this->debug = true; }
		LUNA_API void disableDebug() { this->debug = false; }

		LUNA_API int run();
		virtual void mainFrameUpdate(FrameProps fp);
		LUNA_API virtual void frameUpdate(FrameProps fp) override;
		// Deltatime should be near zero, however could fluctuate.
		LUNA_API virtual void tickUpdate(TickProps tp) override;

		LUNA_API Space* getSpace();

		LUNA_API static void clearColor(float r, float g, float b, float a);
	};
}

#endif // LUNA_ENGINE_H