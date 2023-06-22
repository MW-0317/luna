#include "Camera.h"

Camera::Camera(CameraSettings cameraSettings)
{
	this->cameraSettings = cameraSettings;
}

glm::vec3 Camera::getPosition()
{
	return this->cameraSettings.position;
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

void Camera2D::processInput(GLFWwindow* window, float deltaframe)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float frameCameraSpeed = this->getSpeed() * deltaframe;

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		frameCameraSpeed *= 2;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->cameraSettings.position += glm::normalize(
			glm::cross(this->cameraSettings.front,
				this->cameraSettings.up) * frameCameraSpeed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->cameraSettings.position -= glm::normalize(
			glm::cross(this->cameraSettings.front,
				this->cameraSettings.up) * frameCameraSpeed);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->cameraSettings.position += frameCameraSpeed * this->cameraSettings.up;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		this->cameraSettings.position -= frameCameraSpeed * this->cameraSettings.up;
}