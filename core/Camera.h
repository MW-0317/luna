#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <thirdparty/GLFW/glfw3.h>
#include <thirdparty/glm/glm.hpp>

struct CameraSettings
{
	glm::vec3 position	= glm::vec3(0.0f);
	glm::vec3 front		= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up		= glm::vec3(0.0f, 1.0f, 0.0f);

	float fov			= 45.0f;
	float speed			= 2.5f;
};

class Camera
{
protected:
	CameraSettings cameraSettings;

public:
	Camera();
	Camera(CameraSettings cameraSettings);
	~Camera();

	virtual void processInput(GLFWwindow* window, float deltaframe) = 0;

	glm::vec3 getPosition();
	float getSpeed();

	void setPosition(glm::vec3 position);
	void setSpeed(float speed);
};

class Camera2D : public Camera
{
public:
	Camera2D(CameraSettings cameraSettings);
	~Camera2D();
	void processInput(GLFWwindow* window, float deltaframe);
};

class Camera3D : public Camera
{
public:
	void processInput(GLFWwindow* window, float deltaframe);
};

#endif // !CAMERA_H