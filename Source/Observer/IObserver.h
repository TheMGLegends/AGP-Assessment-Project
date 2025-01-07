#pragma once

class IObserver
{
public:
	virtual void OnNotifyIsFreeCamChange(bool isFreeCam) {}
	virtual void OnNotifyScoreChange(int score) {}
};

