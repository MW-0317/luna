#pragma once

#include "Globals.h"

enum CameraType
{
	Orthographic,
	Perspective
};

struct CameraSettings
{
	glm::vec3 position	= glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 front		= glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up		= glm::vec3(0.0f, 1.0f, 0.0f);
	float width, height;

	float fov			= 45.0f;
	float speed			= 2.5f;
	CameraType cameraType;

	GLFWwindow* window = nullptr;
};

class Camera
{
protected:
	CameraSettings cameraSettings;
	glm::mat4 view, proj;

public:
	Camera(CameraSettings cameraSettings);
	~Camera();

	void processInput(float deltaframe);

	glm::vec3 getPosition();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();
	float getSpeed();

	void setPosition(glm::vec3 position);
	void setSpeed(float speed);

	bool isWindow();
};