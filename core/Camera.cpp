#include "Camera.h"

#include <iostream>
#include <glm/gtx/string_cast.hpp>

namespace luna
{
	Camera::Camera(CameraSettings cameraSettings)
		: Object(mesh, shader, cameraSettings.position, glm::vec3(1.0f), glm::vec3(0.0f))
	{
		this->cameraSettings = cameraSettings;
		glm::mat4 view = glm::mat4(1.0f);
		view = glm::lookAt(
			cameraSettings.position,
			cameraSettings.position + cameraSettings.front,
			cameraSettings.up
		);
		//view = glm::translate(view, -cameraSettings.position);

		if (cameraSettings.cameraType == CameraType::Orthographic)
		{
			proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 100.0f);
		}
		else if (cameraSettings.cameraType == CameraType::Perspective)
		{
			proj = glm::perspective(glm::radians(cameraSettings.fov),
				cameraSettings.width / cameraSettings.height, 0.1f, 100.0f);
		}
		else
		{
			proj = glm::mat4(1.0f);
		}

		if (!cameraSettings.movement)
			return;
		glfwSetInputMode(cameraSettings.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(cameraSettings.window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		glfwGetCursorPos(cameraSettings.window, &lastX, &lastY);
	}

	Camera::~Camera() {}

	void Camera::processInput(float deltatime)
	{
		if (glfwGetKey(cameraSettings.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetInputMode(cameraSettings.window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		if (!(glfwGetInputMode(cameraSettings.window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED))
			return;

		float frameCameraSpeed = this->getSpeed() * deltatime;

		if (glfwGetKey(cameraSettings.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
			frameCameraSpeed *= 2;

		if (glfwGetKey(cameraSettings.window, GLFW_KEY_D) == GLFW_PRESS)
			this->cameraSettings.position += glm::normalize(
				glm::cross(this->cameraSettings.front,
					this->cameraSettings.up)) * frameCameraSpeed;
		if (glfwGetKey(cameraSettings.window, GLFW_KEY_A) == GLFW_PRESS)
			this->cameraSettings.position -= glm::normalize(
				glm::cross(this->cameraSettings.front,
					this->cameraSettings.up)) * frameCameraSpeed;

		if (glfwGetKey(cameraSettings.window, GLFW_KEY_SPACE) == GLFW_PRESS)
			this->cameraSettings.position += frameCameraSpeed * this->cameraSettings.up;
		if (glfwGetKey(cameraSettings.window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			this->cameraSettings.position -= frameCameraSpeed * this->cameraSettings.up;

		if (cameraSettings.cameraType != CameraType::Perspective)
			return;

		if (glfwGetKey(cameraSettings.window, GLFW_KEY_W) == GLFW_PRESS)
			this->cameraSettings.position += glm::normalize(
				cameraSettings.front
			) * frameCameraSpeed;
		if (glfwGetKey(cameraSettings.window, GLFW_KEY_S) == GLFW_PRESS)
			this->cameraSettings.position -= glm::normalize(
				cameraSettings.front
			) * frameCameraSpeed;

		double currX, currY;
		glfwGetCursorPos(cameraSettings.window, &currX, &currY);
		double diffX = currX - lastX;
		double diffY = currY - lastY;
		lastX = currX;
		lastY = currY;
		diffX *= cameraSettings.sensitivity;
		diffY *= cameraSettings.sensitivity;
		yaw += diffX;
		pitch += diffY;
		pitch = glm::clamp(pitch, (double)-90.0f, (double)90.0f);

		glm::vec3 dir;
		dir.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		dir.y = sin(glm::radians(-pitch));
		dir.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		this->cameraSettings.front = glm::normalize(dir);
	}

	glm::vec3 Camera::getPosition()
	{
		return this->cameraSettings.position;
	}

	glm::vec3 Camera::getForwardVector()
	{
		return this->cameraSettings.front;
	}

	glm::vec3 Camera::getUpwardVector()
	{
		return this->cameraSettings.up;
	}

	glm::mat4 Camera::getViewMatrix()
	{
		view = glm::lookAt(
			cameraSettings.position,
			cameraSettings.position + cameraSettings.front,
			cameraSettings.up
		);
		//view = glm::translate(view, -cameraSettings.position);
		return view;
	}

	glm::mat4 Camera::getProjectionMatrix()
	{
		return proj;
	}

	float Camera::getSpeed()
	{
		return this->cameraSettings.speed;
	}

	void Camera::setPosition(glm::vec3 position)
	{
		this->cameraSettings.position = position;
	}

	void Camera::setSpeed(float speed)
	{
		this->cameraSettings.speed = speed;
	}

	bool Camera::isWindow()
	{
		return cameraSettings.window != nullptr;
	}
}