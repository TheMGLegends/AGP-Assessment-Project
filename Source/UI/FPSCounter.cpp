#include "FPSCounter.h"

#include <cmath>

#include "../Core/Time/Time.h"

FPSCounter::~FPSCounter()
{
}

void FPSCounter::Update(float deltaTime)
{
	updateFPSTime += deltaTime;

	if (updateFPSTime >= 1.0f)
	{
		SetText("FPS: " + std::to_string(static_cast<int>(Time::GetFrameCount())));
		Time::SetFrameCount(0);
		updateFPSTime = 0.0f;
	}
}
