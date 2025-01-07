#pragma once

#include "Core/UserInterfaceElement.h"

class FPSCounter : public UserInterfaceElement
{
public:
	using UserInterfaceElement::UserInterfaceElement;
	virtual ~FPSCounter() override;

	virtual void Update(float deltaTime) override;

private:
	float updateFPSTime;
};

