#pragma once
#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <stdio.h>

#include "thirdparty/imgui/imgui.h"
#include "thirdparty/imgui/imgui_impl_glfw.h"
#include "thirdparty/imgui/imgui_impl_opengl3.h"
#include "thirdparty/imgui/imgui_impl_win32.h"
#include <thirdparty/glad/glad.h>
#include <thirdparty/GLFW/glfw3.h>
#include <thirdparty/glm/glm.hpp>
#include <thirdparty/glm/gtc/matrix_transform.hpp>
#include <thirdparty/glm/gtc/type_ptr.hpp>
#include <thirdparty/glm/gtx/string_cast.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "thirdparty/stb/stb_image.h"

#include "main_time.h"

#include "core/Camera.h"
#include "core/Space.h"
#include "core/Object.h"
#include "core/Shader.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

namespace luna
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height);
	void processInput(GLFWwindow* window);

	__declspec(dllexport) int run();
}
#endif // MAIN_H
