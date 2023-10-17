#pragma once

#include "Globals.h"
#include "Object.h"

namespace luna
{
	enum CameraType
	{
		Orthographic,
		Perspective
	};

	struct CameraSettings
	{
		float width, height;

		glm::vec3 position = glm::vec3(0.0f, 0.0f, 1.0f);
		glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

		float fov = 45.0f;
		float speed = 2.5f;
		float sensitivity = 0.1f;
		CameraType cameraType;

		GLFWwindow* window = nullptr;

		bool movement = true;
	};

	class Camera : public Object
	{
	protected:
		CameraSettings cameraSettings;
		glm::mat4 view, proj;
		double lastX = 0.0f, lastY = 0.0f;
		double pitch = 0.0f, yaw = 0.0f;

	public:
		Camera(CameraSettings cameraSettings);
		~Camera();

		void processInput(float deltaframe);

		glm::vec3 getPosition();
		glm::vec3 getForwardVector();
		glm::vec3 getUpwardVector();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjectionMatrix();
		float getSpeed();

		void setSize(int width, int height);
		void setPosition(glm::vec3 position);
		void setSpeed(float speed);

		bool isWindow();
	};
}