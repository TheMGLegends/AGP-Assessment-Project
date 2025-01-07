#include "Time.h"

using namespace std::chrono;

constexpr float MICRO_TO_SEC = 1000000.0f;

double Time::deltaTime = 0.0f;
double Time::elapsedTime = 0.0f;
float Time::timeScale = 1.0f;
unsigned int Time::frameCount = 0;

time_point<steady_clock> Time::currentTime;
time_point<steady_clock> Time::previousTime = steady_clock::now();

void Time::Tick()
{
	currentTime = steady_clock::now();

	deltaTime = duration_cast<microseconds>(currentTime - previousTime).count() / MICRO_TO_SEC;

	// INFO: Adjust delta time by the time scale before adding it to the elapsed time
	deltaTime *= timeScale;
	elapsedTime += deltaTime;

	previousTime = currentTime;

	frameCount++;
}
