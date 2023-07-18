#include "Space.h"

Space::Space()
{
	CameraSettings cs = CameraSettings();
	currentCamera = new Camera(cs);
}

Space::Space(Camera* camera)
{
	this->currentCamera = camera;
}

Space::~Space()
{
	delete currentCamera;
}

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

// Temp function, to be removed with tickrate introduction
void Space::forceStatic()
{
	this->isStatic = true;
}