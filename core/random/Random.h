#pragma once

#include <random>

#include "../Core.h"

namespace luna
{
	class Random
	{
	protected:
		std::mt19937_64 randomNumberEngine;
		//std::random_device rd;

	public:
		LUNA_API Random();

		//std::uniform_real_distribution<float> uniform;

		template<typename T>
		LUNA_API T generateUniformReal(T start, T end);
		template<typename T>
		LUNA_API T generateUniformReal(T end);
		template<typename T>
		LUNA_API T generateUniformReal();

		template<typename T>
		T generateUniformInt(int start, int end);
		template<typename T>
		LUNA_API T generateUniformInt(int end);
		template<typename T>
		LUNA_API T generateUniformInt();
	};

}
