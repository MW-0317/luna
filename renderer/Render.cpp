#include "Render.h"
#include "core/Globals.h"

namespace luna
{
	void Render::init()
	{
		if (DEBUG)
		{
			video = nullptr;
			Engine::init();
			return;
		}
		if (type == RenderType::VIDEO)
			video = new Video(filename, fps, seconds, width, height);
		else if (type == RenderType::IMAGE)
			;

		Engine::init();
	}

	Render::Render() {}

	Render::Render(const char* filename, 
		float fps, 
		float seconds, 
		int width, 
		int height, 
		bool debug = false)
	{
		DEBUG = debug;
		this->fps = fps;
		this->seconds = seconds;
		this->width = width;
		this->height = height;
		this->filename = filename;
		init();
	}

	Render::~Render()
	{
		if (!DEBUG)
			video->free();
	}

	void Render::frameUpdate(Frame frame)
	{		
		if (DEBUG) return;
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
		if (DEBUG) return;
		video->save();
		video->free();
	}

	void Render::setFilename(const char* filename)
	{
		this->filename = filename;
	}

	void Render::setFPS(float fps)
	{
		this->fps = fps;
	}

	void Render::setSeconds(float seconds)
	{
		this->seconds = seconds;
	}

	void Render::createWindow(int width, int height)
	{
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		if (!DEBUG)
			glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

		this->window = glfwCreateWindow(width, height, "lunaR", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "MAIN::WINDOW_FAILED" << std::endl;
			std::cout << "MAIN::WINDOW_WIDTH::" << width << " "
				<< "MAIN::WINDOW_HEIGHT::" << height << std::endl;
			glfwTerminate();
			abort();
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
