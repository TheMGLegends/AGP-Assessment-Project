#pragma once

#include <chrono>

class Time
{
public:
	static void Tick();

	static inline const float GetDeltaTime() { return static_cast<float>(deltaTime); }
	static inline const float GetElapsedTime() { return static_cast<float>(elapsedTime); }

	static inline void SetTimeScale(const float _timeScale) { timeScale = _timeScale; }
	static inline const float GetTimeScale() { return timeScale; }

private:
	static double deltaTime;
	static double elapsedTime;
	static float timeScale;

	static std::chrono::time_point<std::chrono::steady_clock> currentTime;
	static std::chrono::time_point<std::chrono::steady_clock> previousTime;
};

