#pragma once

#include <vector>

class IObserver;

class Subject
{
public:
	Subject();
	virtual ~Subject();

	void AddObserver(IObserver* observer);
	void RemoveObserver(IObserver* observer);

protected:
	void NotifyIsFreeCamChanged(bool isFreeCam);
	void NotifyScoreChanged(int score);

private:
	std::vector<IObserver*> observers;
};

