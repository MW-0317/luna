#include "Camera.h"

#include <iostream>

Camera::Camera(CameraSettings cameraSettings)
{
	this->cameraSettings = cameraSettings;

	view = glm::lookAt(
		cameraSettings.position,
		cameraSettings.position + cameraSettings.front,
		cameraSettings.up
	);
	view = glm::translate(view, -cameraSettings.position);

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
}

Camera::~Camera(){}

void Camera::processInput(float deltatime)
{
	if (glfwGetKey(cameraSettings.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(cameraSettings.window, true);

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
}

glm::vec3 Camera::getPosition()
{
	return this->cameraSettings.position;
}

glm::mat4 Camera::getViewMatrix()
{
	view = glm::lookAt(
		cameraSettings.position,
		cameraSettings.position + cameraSettings.front,
		cameraSettings.up
	);
	view = glm::translate(view, -cameraSettings.position);
	std::cout << cameraSettings.position[0] << std::endl;
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