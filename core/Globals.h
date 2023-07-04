#pragma once
#include "Core.h"
#include "includes.h"
namespace luna
{
	LUNA_API extern int WINDOW_WIDTH;
	LUNA_API extern int WINDOW_HEIGHT;
	extern bool IMGUI_INIT;
	
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);

	LUNA_API int run();
}