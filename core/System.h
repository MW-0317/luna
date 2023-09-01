#pragma once
#include "core/Core.h"
#include "random/Random.h"

namespace luna
{
	struct FrameProps
	{
		float deltatime;
		Random* rng;
	};

	struct TickProps
	{
		float deltatime;
		Random* rng;
	};

	class System
	{
	public:
		LUNA_API virtual void frameUpdate(FrameProps fp) {}
		LUNA_API virtual void tickUpdate(TickProps tp) {}
	};
}