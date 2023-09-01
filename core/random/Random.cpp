#include "Random.h"
#include <climits>
#include <cstdlib>
#include <ctime>
#include <iostream>

namespace luna
{
	Random::Random()
	{
		// WHY IS THIS BEING CALLED EVERY FRAME (OR TICK???) ?!?!?
		std::cout << "Random Init" << std::endl;
		//unsigned timeSeed = time(nullptr);

		//rng.seed(timeSeed);
	}

	template<typename T>
	T Random::generateUniformReal(T start, T end)
	{
		std::uniform_real_distribution<T> r(start, end);
		T randomVal = r(randomNumberEngine);
		//rng.seed(randomVal);
		return randomVal;
	}
	template LUNA_API float Random::generateUniformReal<float>(float start, float end);
	template LUNA_API double Random::generateUniformReal<double>(double start, double end);

	template<typename T>
	T Random::generateUniformReal(T end) { return generateUniformReal<T>(0, end); }
	template LUNA_API float Random::generateUniformReal<float>(float end);
	template LUNA_API double Random::generateUniformReal<double>(double end);

	template<typename T>
	T Random::generateUniformReal() { return generateUniformReal<T>(0, 1); }
	template LUNA_API float Random::generateUniformReal<float>();
	template LUNA_API double Random::generateUniformReal<double>();

	template<typename T>
	T Random::generateUniformInt(int start, int end)
	{
		std::uniform_int_distribution<T> r(start, end);
		T randomVal = r(randomNumberEngine);
		//rng.seed(randomVal);
		return randomVal;
	}
	template LUNA_API int Random::generateUniformInt<int>(int start, int end);
	template LUNA_API unsigned int Random::generateUniformInt<unsigned int>(int start, int end);
	template LUNA_API int32_t Random::generateUniformInt<int32_t>(int start, int end);
	template LUNA_API uint32_t Random::generateUniformInt<uint32_t>(int start, int end);

	template<typename T>
	T Random::generateUniformInt(int end) { return generateUniformInt<T>(0, end); }
	template LUNA_API int Random::generateUniformInt<int>(int end);
	template LUNA_API unsigned int Random::generateUniformInt<unsigned int>(int end);
	template LUNA_API int32_t Random::generateUniformInt<int32_t>(int end);
	template LUNA_API uint16_t Random::generateUniformInt<uint16_t>(int end);

	template<typename T>
	T Random::generateUniformInt() { return generateUniformInt<T>(0, 1); }
	template LUNA_API int Random::generateUniformInt<int>();
	template LUNA_API unsigned int Random::generateUniformInt<unsigned int>();
	template LUNA_API int32_t Random::generateUniformInt<int32_t>();
	template LUNA_API uint16_t Random::generateUniformInt<uint16_t>();
}

/*
	template <class _Engine>
    result_type _Eval(_Engine& _Eng, const param_type& _Par0) const {
        return _NRAND(_Eng, _Ty) * (_Par0._Max - _Par0._Min) + _Par0._Min;
    }
*/