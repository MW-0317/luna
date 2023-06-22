#include "Space.h"

Space::Space() : currentCamera() {}

Space::Space(Camera* camera)
{
	this->currentCamera = camera;
}

Space::~Space() {}

void Space::updateSpace()
{
	float currentframe = glfwGetTime();
	deltaframe = currentframe - lastframe;
	lastframe = currentframe;
}

Camera* Space::getCamera()
{
	return this->currentCamera;
}

float Space::getDelta()
{
	return this->deltaframe;
}