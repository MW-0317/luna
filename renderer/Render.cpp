#include "Render.h"
#include "core/Globals.h"

namespace luna
{
	Render::Render(const char* filename, float fps, float seconds, int width, int height) 
		: Engine(width, height)
	{
		video = new Video(filename, fps, seconds, width, height);
	}

	Render::~Render()
	{
		video->free();
	}

	void Render::mainFrameUpdate(float deltatime)
	{
		frameUpdate(deltatime);
		renderFrameUpdate(deltatime);
	}

	void Render::renderFrameUpdate(float deltatime)
	{
		// POSSIBLE OPTIMIZATION:
		// Do the converting process between the 
		// readpixels space and the frame space
		// in video->encodeFrame(pixels, size);
		int size = width * height * 4;
		uint8_t* pixels = (uint8_t*)malloc(sizeof(uint8_t) * size);
		glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8, pixels);
		video->encodeFrame(pixels, size);
		if (video->isFull())
			glfwSetWindowShouldClose(window, true);
	}

	void Render::save()
	{
		video->save();
		video->free();
	}
}
