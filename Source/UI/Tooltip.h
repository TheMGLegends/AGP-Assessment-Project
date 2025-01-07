#pragma once

#include "Core/UserInterfaceElement.h"

class Tooltip : public UserInterfaceElement
{
public:
	using UserInterfaceElement::UserInterfaceElement;
	virtual ~Tooltip() override;

	virtual void Update(float deltaTime) override;
};

