#pragma once
#ifndef CAMERA_H

#include <thirdparty/GLFW/glfw3.h>
#include <thirdparty/glm/glm.hpp>

struct CameraSettings
{
	glm::vec3 position	= glm::vec3(0.0f);
	glm::vec3 front		= glm::vec3(0.0f);
	glm::vec3 up		= glm::vec3(0.0f);

	float fov			= 45.0f;
	float speed			= 2.5f;
};

class Camera
{
protected:
	CameraSettings cameraSettings;

public:
	Camera(CameraSettings cameraSettings);
	~Camera();

	virtual void processInput(GLFWwindow* window, float deltaframe) = 0;

	glm::vec3 getPosition();
	float getSpeed();

	void setPosition(glm::vec3 position);
	void setSpeed(float speed);
};

class Camera2D : Camera
{
	void processInput(GLFWwindow* window, float deltaframe);
};

class Camera3D : Camera
{
	void processInput(GLFWwindow* window, float deltaframe);
};

#endif // !CAMERA_H