#include "Space.h"

using namespace luna;

void Space::init()
{
	objects = std::vector<Object>();
}

Space::Space()
{
	CameraSettings cs = CameraSettings();
	cs.width = luna::WINDOW_WIDTH;
	cs.height = luna::WINDOW_HEIGHT;
	cs.cameraType = CameraType::Orthographic;
	currentCamera = new Camera(cs);
	init();
}

Space::Space(Camera* camera)
{
	this->currentCamera = camera;
	init();
}

Space::~Space()
{
	delete currentCamera;
}

void Space::frameUpdate()
{
	float currentframe = glfwGetTime();
	deltaframe = currentframe - lastframe;
	lastframe = currentframe;

	this->draw();
}

bool Space::tickUpdate()
{
	float currenttick = glfwGetTime();
	deltatick = currenttick - lasttick;

	bool isTickable = deltatick > INV_TPS;
	if (isTickable)
	{
		lasttick = currenttick;
		if (currentCamera->isWindow()) currentCamera->processInput(deltatick);
	}

	return isTickable;
}

float Space::getDeltaTick()
{
	return this->deltatick;
}

Camera* Space::getCamera()
{
	return this->currentCamera;
}

float Space::getDelta()
{
	return this->deltaframe;
}

void Space::draw()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i].draw(currentCamera->getViewMatrix(), currentCamera->getProjectionMatrix());
	}
}

void Space::addObject(Object object)
{
	objects.push_back(object);
}

// Temp function, to be removed with tickrate introduction
void Space::forceStatic()
{
	this->isStatic = true;
}