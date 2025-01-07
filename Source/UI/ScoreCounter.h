#pragma once

#include "Core/UserInterfaceElement.h"
#include "../Observer/IObserver.h"

class ScoreCounter : public UserInterfaceElement, public IObserver
{
public:
	using UserInterfaceElement::UserInterfaceElement;
	virtual ~ScoreCounter() override;

public:
	virtual void OnNotifyScoreChange(int score) override;

private:
	int score;
};

