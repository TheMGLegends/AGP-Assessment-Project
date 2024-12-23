#pragma once

#include <chrono>

class Time
{
public:
	Time() = delete;
	~Time() = delete;
	Time(const Time&) = delete;
	Time& operator=(const Time&) = delete;

	static void Tick();

	static inline float GetDeltaTime() { return static_cast<float>(deltaTime); }
	static inline float GetElapsedTime() { return static_cast<float>(elapsedTime); }

	static inline void SetTimeScale(const float _timeScale) { timeScale = _timeScale; }
	static inline float GetTimeScale() { return timeScale; }

private:
	static double deltaTime;
	static double elapsedTime;
	static float timeScale;

	static std::chrono::time_point<std::chrono::steady_clock> currentTime;
	static std::chrono::time_point<std::chrono::steady_clock> previousTime;
};

