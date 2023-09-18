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

void Space::frameUpdate(RenderProps renderProps)
{
	float currentframe = glfwGetTime();
	deltaframe = currentframe - lastframe;
	lastframe = currentframe;

	renderProps.space = this;
	renderProps.camera = currentCamera;
	renderProps.deltatime = deltaframe;
	this->draw(renderProps);

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

void Space::draw(RenderProps renderProps)
{
	renderProps.view = this->currentCamera->getViewMatrix();
	renderProps.proj = this->currentCamera->getProjectionMatrix();

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw(renderProps);
	}

	for (int i = 0; i < cells.size(); i++)
	{
		cells[i]->draw(renderProps, cells[i]->getBasicShader());
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

void Space::addCell(Cell* cell)
{
	cells.push_back(cell);
}

// TODO
void Space::createDebugLines()
{
	std::vector<Line*> lines =
	{
		new Line({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) })
	};

	for (int i = 0; i < lines.size(); i++)
	{
		cells.push_back(lines[i]);
	}
}