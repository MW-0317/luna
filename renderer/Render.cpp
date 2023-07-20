#include "Render.h"
#include "core/Globals.h"

namespace luna
{
	Render::Render(const char* filename, float fps, float seconds, int width, int height)
	{
		init(width, height);
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
		free(pixels);
	}

	void Render::save()
	{
		video->save();
		video->free();
	}

	void Render::createWindow(int width, int height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		this->window = glfwCreateWindow(width, height, "luna", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "MAIN::WINDOW_FAILED" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		// glfwSwapInterval(1); // Enables vsync, add to Game::Game()

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "MAIN::GLAD_INIT_FAILED" << std::endl;
			return;
		}
	}
}
