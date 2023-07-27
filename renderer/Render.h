#pragma once
#include "core/Core.h"
#include "core/Engine.h"

#include "core/media/Video.h"

namespace luna
{
	LUNA_API class Render : public Engine
	{
	private:
		bool DEBUG;

		Video* video;

		LUNA_API virtual void mainFrameUpdate(float deltatime) override;
		LUNA_API void createWindow(int width, int height) override;
	public:
		LUNA_API Render(const char* filename, float fps, float seconds, int width, int height, bool debug);
		LUNA_API ~Render();

		void renderFrameUpdate(float deltatime);
		LUNA_API void save();
	};
}

