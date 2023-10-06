#include "Space.h"
#include "Engine.h"

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
	cs.movement = false;
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

void Space::frameUpdate(Frame frame)
{
	frame.space = this;
	frame.camera = currentCamera;
	this->draw(frame);

	for (int i = 0; i < systems.size(); i++)
	{
		frame.rng = this->getRandom();
		systems[i]->frameUpdate(frame);
	}
}

void Space::tickUpdate(Tick tick)
{
	if (currentCamera->isWindow()) currentCamera->processInput(tick.deltatime);
	for (int i = 0; i < systems.size(); i++)
	{
		Tick tick;
		tick.deltatime = deltaframe;
		tick.rng = this->getRandom();
		systems[i]->tickUpdate(tick);
	}
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

void Space::draw(Frame frame)
{
	frame.space	= this;
	frame.camera	= this->currentCamera;
	frame.view	= this->currentCamera->getViewMatrix();
	frame.proj	= this->currentCamera->getProjectionMatrix();

	glm::vec3 pos = this->currentCamera->getPosition();
	frame.engine->oLog.update("x", pos.x);
	frame.engine->oLog.update("y", pos.y);
	frame.engine->oLog.update("z", pos.z);

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw(frame);
	}

	for (int i = 0; i < primitives.size(); i++)
	{
		primitives[i]->draw(frame, primitives[i]->getBasicShader());
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

void Space::addPrimitive(Primitive* primitive)
{
	primitives.push_back(primitive);
}

void Space::createDebugLines()
{
	std::vector<Line*> lines =
	{
		new Line(
					{ 
						glm::vec3(0.0f, 0.0f, 0.0f), 
						glm::vec3(1.0f, 0.0f, 0.0f),
						glm::vec3(0.0f, 1.0f, 0.0f),
						glm::vec3(0.0f, 0.0f, 1.0f)
					},
					{
						0, 1, 0, 2, 0, 3
					}
				)
	};



	for (int i = 0; i < lines.size(); i++)
	{
		primitives.push_back(lines[i]);
	}
}