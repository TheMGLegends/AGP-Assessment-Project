#include "MathUtils.h"

#include <cstdlib>

float MathUtils::RandomRange(float min, float max)
{
	float range = max - min;
	return min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / range));
}

int MathUtils::RandomRange(int min, int max)
{
	int range = max - min;
	return min + rand() % range;
}
