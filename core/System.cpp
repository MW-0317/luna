#include "System.h"
#include "Object.h"

namespace luna
{
	System::System()
	{

	}

	void System::addSystem(System* system)
	{
		systems.push_back(system);
	}

	void System::tickSystems(Tick tick)
	{
		int size = systems.size();
		for (int i = 0; i < size; i++)
			systems[i]->tickUpdate(tick);
	}

	void System::frameSystems(Frame frame)
	{
		int size = systems.size();
		for (int i = 0; i < size; i++)
			systems[i]->frameUpdate(frame);
	}
}