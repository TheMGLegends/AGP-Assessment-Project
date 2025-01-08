#include "InputHandler.h"

#include "../../Utilities/Debugging/DebugUtils.h"

using namespace DebugUtils;
using namespace DirectX;

std::unique_ptr<Keyboard> InputHandler::keyboard;
Keyboard::State InputHandler::keyboardState;
Keyboard::KeyboardStateTracker InputHandler::keyboardStateTracker;
std::unordered_multimap<Keyboard::Keys, BindData> InputHandler::keyboardActions;

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
		LogError("InputHandler::Initialise(): Failed to create the keyboard/mouse!");
		return false;
	}

	// INFO: Set mouse properties
	mouse->SetWindow(hWnd);
	mouse->SetMode(Mouse::Mode::MODE_ABSOLUTE);

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

void InputHandler::SetMouseMode(DirectX::Mouse::Mode mouseMode)
{
	if (mouseMode == mouseState.positionMode)
		return;
	
	mouse->SetMode(mouseMode);

	if (mouseMode == Mouse::Mode::MODE_ABSOLUTE)
		ShowCursor(TRUE);
	else if (mouseMode == Mouse::Mode::MODE_RELATIVE)
		ShowCursor(FALSE);
}

void InputHandler::ClearKeyBinding(DirectX::Keyboard::Keys key, ButtonState buttonState)
{
	auto range = keyboardActions.equal_range(key);

	for (auto& it = range.first; it != range.second; ++it)
	{
		if (it->second.GetButtonState() == buttonState)
		{
			keyboardActions.erase(it);
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

bool InputHandler::IsMouseInsideWindow(HWND hWnd)
{
	POINT mousePosition;
	GetCursorPos(&mousePosition);

	ScreenToClient(hWnd, &mousePosition);

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	return PtInRect(&clientRect, mousePosition);
}
