#pragma once
#include "Camera.h"

class Space
{
private:
	Camera* currentCamera;
	float deltaframe = 0.0f;
	float lastframe = 0.0f;

	int width, height, length;

public:
	Space();
	Space(Camera* camera);
	~Space();

	void updateSpace();

	Camera* getCamera();
	float getDelta();
};