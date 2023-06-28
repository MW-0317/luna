#pragma once
#include "core/Core.h"
#include "core/Engine.h"

namespace luna
{
	LUNA_API class Render : public Engine
	{
	public:
		Render();
		~Render();

		int run();
	};
}

