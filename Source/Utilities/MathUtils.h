#pragma once

#include <cstdlib>

namespace MathUtils
{
	inline float RandomRange(float min, float max)
	{
		float range = max - min;
		return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / range));
	}

	inline int RandomRange(int min, int max)
	{
		int range = max - min;
		return min + rand() % range;
	}
}