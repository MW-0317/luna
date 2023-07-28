#pragma once
#include "core/Core.h"
namespace luna
{
	class System
	{
	public:
		LUNA_API virtual void frameUpdate(float deltatime) {}
		LUNA_API virtual void tickUpdate(float deltatime) {}
	};
}