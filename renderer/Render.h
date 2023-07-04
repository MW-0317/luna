#pragma once
#include "core/Core.h"
#include "core/Engine.h"

#include "core/media/Video.h"

namespace luna
{
	LUNA_API class Render : public Engine
	{
	private:
		Video* video;
	public:
		LUNA_API Render(const char* filename, float fps, float seconds, int width, int height);
		~Render();

		void mainFrameUpdate(float deltatime) override;
		void renderFrameUpdate(float deltatime);
		LUNA_API void save();
	};
}

