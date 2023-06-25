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
#include "core/Object.h"
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
	glClearColor(1.0f, 0.5f, 1.0f, 1.0f);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		 0.5f,  0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f
	};

	unsigned int indices[] = {
		1, 3, 0,
		1, 3, 2
	};

	CameraSettings cSet;
	Camera* cam = new Camera2D(cSet);
	Space s = Space(cam);

	Shader shader = Shader(
		"shaders/basic.vs",
		"shaders/basic.fs"
	);

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		s.updateSpace();
		s.getCamera()->processInput(window, s.getDelta());

		shader.use();

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		
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