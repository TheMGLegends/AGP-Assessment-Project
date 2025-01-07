#include "Subject.h"

#include "IObserver.h"

Subject::Subject()
{
}

Subject::~Subject()
{
}

void Subject::AddObserver(IObserver* observer)
{
	observers.push_back(observer);
}

void Subject::RemoveObserver(IObserver* observer)
{
	observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
}

void Subject::NotifyIsFreeCamChanged(bool isFreeCam)
{
	for (size_t i = 0; i < observers.size(); i++)
	{
		observers[i]->OnNotifyIsFreeCamChange(isFreeCam);
	}
}

void Subject::NotifyScoreChanged(int score)
{
	for (size_t i = 0; i < observers.size(); i++)
	{
		observers[i]->OnNotifyScoreChange(score);
	}
}

void Subject::NotifyWindowSizeChanged(int width, int height)
{
	for (size_t i = 0; i < observers.size(); i++)
	{
		observers[i]->OnNotifyWindowSizeChanged(width, height);
	}
}
