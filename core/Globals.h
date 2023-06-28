#pragma once
#include "Core.h"
#include "includes.h"
namespace luna
{
	extern void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	extern void processInput(GLFWwindow* window);

	LUNA_API int run();
}