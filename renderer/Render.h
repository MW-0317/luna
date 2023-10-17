#pragma once
#include "core/Core.h"
#include "core/Engine.h"

#include "core/media/Video.h"

namespace luna
{
	class LUNA_API Render : public Engine
	{
	private:
		bool DEBUG;

		Video* video;
		const char* filename;
		float fps;
		float seconds;
		
		void createWindow(int width, int height) override;
	public:
		Render();
		Render(const char* filename, float fps, float seconds, int width, int height, bool debug);
		~Render();

		void init() override;

		void frameUpdate(Frame frame) override;

		void save();

		void setFilename(const char* filename);
		void setFPS(float fps);
		void setSeconds(float seconds);
	};
}

