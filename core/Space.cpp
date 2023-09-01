#include "Space.h"

using namespace luna;

void Space::init()
{
	objects = std::vector<Object*>();
	rng = new Random();
}

Space::Space()
{
	CameraSettings cs = CameraSettings();
	cs.width = luna::WINDOW_WIDTH;
	cs.height = luna::WINDOW_HEIGHT;
	cs.cameraType = CameraType::Perspective;
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

	for (int i = 0; i < systems.size(); i++)
	{
		FrameProps fp;
		fp.deltatime = deltaframe;
		fp.rng = this->getRandom();
		systems[i]->frameUpdate(fp);
	}
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
		for (int i = 0; i < systems.size(); i++)
		{
			TickProps tp;
			tp.deltatime = deltaframe;
			tp.rng = this->getRandom();
			systems[i]->tickUpdate(tp);
		}
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

Random* Space::getRandom()
{
	return this->rng;
}

void Space::draw()
{
	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->getShader()->setMat4("view", this->currentCamera->getViewMatrix());
		objects[i]->getShader()->setMat4("projection", this->currentCamera->getProjectionMatrix());
		objects[i]->draw();
	}
}

void Space::addObject(Object* object)
{
	objects.push_back(object);
}

void Space::addSystem(System* system)
{
	systems.push_back(system);
}