#include "Globals.h"

namespace luna
{
	int WINDOW_WIDTH = 800;
	int WINDOW_HEIGHT = 600;
	bool IMGUI_INIT = false;

	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		WINDOW_WIDTH = width;
		WINDOW_HEIGHT = height;
		glViewport(0, 0, width, height);
	}
}