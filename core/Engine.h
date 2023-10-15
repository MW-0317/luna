#pragma once
#ifndef LUNA_ENGINE_H
#define LUNA_ENGINE_H

#include "core/Core.h"
#include "core/Space.h"
#include "core/System.h"

namespace luna
{
	class LUNA_API Engine : public virtual System
	{
	protected:
		bool debug;

		GLFWwindow* window;
		std::vector<Space*> spaces;

		int width, height;

		bool exampleWindow = true;

		virtual void init(int width, int height);
	private:
		float const MAX_TPS = 60;
		float const INV_TPS = 1 / MAX_TPS;

		virtual void createWindow(int width, int height);
	public:
		Engine(int width, int height);
		Engine();
		~Engine();

		void enableDebug() { this->debug = true; }
		void disableDebug() { this->debug = false; }

		int run();
		virtual void mainFrameUpdate(Frame frame);
		virtual void frameUpdate(Frame frame) override;
		// Deltatime should be near zero, however could fluctuate.
		void mainTickUpdate(Tick tick);
		virtual void tickUpdate(Tick tick) override;

		Space* getSpace(int index);

		static void clearColor(float r, float g, float b, float a);
	};
}

#endif // LUNA_ENGINE_H