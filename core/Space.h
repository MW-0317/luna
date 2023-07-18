#pragma once
#include "Camera.h"

class Space
{
private:
	Camera* currentCamera;
	float deltaframe = 0.0f;
	float lastframe = 0.0f;

	int width, height, length;

	// Temp, to be removed with tickrate introduction
	bool isStatic;

public:
	Space();
	Space(Camera* camera);
	~Space();

	void updateSpace();

	Camera* getCamera();
	float getDelta();

	// Temp function, to be removed with tickrate introduction
	void forceStatic();
};