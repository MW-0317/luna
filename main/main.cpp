#include <iostream>

#include <thirdparty/glad/glad.h>
#include <thirdparty/GLFW/glfw3.h>
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>
#include <thirdparty/glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb/stb_image.h"

#include "main.h"
#include "main_time.h"

#include "core/Camera.h"
#include "core/Space.h"
#include "core/Mesh.h"
#include "core/Shader.h"

int main()
{
	stbi_set_flip_vertically_on_load(true);

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "luna", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "MAIN::WINDOW_FAILED" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "MAIN::GLAD_INIT_FAILED" << std::endl;
		return -1;
	}

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	CameraSettings cSet;
	Camera* cam = new Camera2D(cSet);
	Space s = Space(cam);

	while (!glfwWindowShouldClose(window))
	{
		s.updateSpace();
		s.getCamera()->processInput(window, s.getDelta());

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}