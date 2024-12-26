#include "InputHandler.h"

#include <iostream>

using namespace DirectX;

std::unique_ptr<Keyboard> InputHandler::keyboard;
Keyboard::State InputHandler::keyboardState;
Keyboard::KeyboardStateTracker InputHandler::keyboardStateTracker;
std::unordered_map<Keyboard::Keys, BindData> InputHandler::keyboardActions;

std::unique_ptr<Mouse> InputHandler::mouse;
Mouse::State InputHandler::mouseState;
Mouse::ButtonStateTracker InputHandler::mouseStateTracker;
std::unordered_map<MouseButton, BindData> InputHandler::mouseActions;

bool InputHandler::Initialise(HWND hWnd)
{
	// INFO: Create a keyboard and mouse
	keyboard = std::make_unique<Keyboard>();
	mouse = std::make_unique<Mouse>();

	if (!keyboard || !mouse)
	{
		std::cout << "InputHandler::Initialise(): Failed to create the keyboard and mouse!" << std::endl;
		return false;
	}

	// INFO: Set mouse properties
	mouse->SetWindow(hWnd);
	mouse->SetMode(Mouse::MODE_RELATIVE); // Relative mode is used for first person camera

	return true;
}

void InputHandler::HandleInput()
{
	keyboardState = keyboard->GetState();
	keyboardStateTracker.Update(keyboardState);

	mouseState = mouse->GetState();
	mouseStateTracker.Update(mouseState);

	// INFO: Execute all bound actions for the keyboard
	for (auto& action : keyboardActions)
	{
		switch (action.second.GetButtonState())
		{
		case ButtonState::Pressed:
			if (GetKeyDown(action.first))
				action.second.Execute();
			break;
		case ButtonState::Held:
			if (GetKey(action.first))
				action.second.Execute();
			break;
		case ButtonState::Released:
			if (GetKeyUp(action.first))
				action.second.Execute();
			break;
		case ButtonState::None:
		default:
			break;
		}
	}

	// INFO: Execute all bound actions for the mouse
	for (auto& action : mouseActions)
	{
		switch (action.second.GetButtonState())
		{
		case ButtonState::Pressed:
			if (GetMouseButtonDown(action.first))
				action.second.Execute();
			break;
		case ButtonState::Held:
			if (GetMouseButton(action.first))
				action.second.Execute();
			break;
		case ButtonState::Released:
			if (GetMouseButtonUp(action.first))
				action.second.Execute();
			break;
		case ButtonState::None:
		default:
			break;
		}
	}
}

bool InputHandler::GetMouseButton(MouseButton mouseButton)
{
	switch (mouseButton)
	{
	case MouseButton::LeftMouseButton:
		return mouseStateTracker.leftButton == mouseStateTracker.HELD;
	case MouseButton::MiddleMouseButton:
		return mouseStateTracker.middleButton == mouseStateTracker.HELD;
	case MouseButton::RightMouseButton:
		return mouseStateTracker.rightButton == mouseStateTracker.HELD;
	case MouseButton::None:
	default:
		return false;
	}
}

bool InputHandler::GetMouseButtonDown(MouseButton mouseButton)
{
	switch (mouseButton)
	{
	case MouseButton::LeftMouseButton:
		return mouseStateTracker.leftButton == mouseStateTracker.PRESSED;
	case MouseButton::MiddleMouseButton:
		return mouseStateTracker.middleButton == mouseStateTracker.PRESSED;
	case MouseButton::RightMouseButton:
		return mouseStateTracker.rightButton == mouseStateTracker.PRESSED;
	case MouseButton::None:
	default:
		return false;
	}
}

bool InputHandler::GetMouseButtonUp(MouseButton mouseButton)
{
	switch (mouseButton)
	{
	case MouseButton::LeftMouseButton:
		return mouseStateTracker.leftButton == mouseStateTracker.RELEASED;
	case MouseButton::MiddleMouseButton:
		return mouseStateTracker.middleButton == mouseStateTracker.RELEASED;
	case MouseButton::RightMouseButton:
		return mouseStateTracker.rightButton == mouseStateTracker.RELEASED;
	case MouseButton::None:
	default:
		return false;
	}
}
