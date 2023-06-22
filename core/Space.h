#pragma once
#ifndef SPACE_H
#define SPACE_H

#include "Camera.h"

class Space
{
private:
	Camera* currentCamera;
	float deltaframe = 0.0f;
	float lastframe = 0.0f;

public:
	Space();
	Space(Camera* camera);
	~Space();

	void updateSpace();

	Camera* getCamera();
	float getDelta();
};

#endif // !SPACE_H
