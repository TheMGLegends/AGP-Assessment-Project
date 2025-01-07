#pragma once

class IObserver
{
public:
	virtual void OnNotifyIsFreeCamChange(bool isFreeCam) {}
	virtual void OnNotifyScoreChange(int score) {}
	virtual void OnNotifyWindowSizeChanged(int width, int height) {}
};

