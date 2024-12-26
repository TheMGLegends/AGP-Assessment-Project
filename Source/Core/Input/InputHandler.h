#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <Windows.h>

#include <Keyboard.h>
#include <Mouse.h>

enum class MouseButton
{
	None = 0,

	LeftMouseButton,
	MiddleMouseButton,
	RightMouseButton
};

enum class ButtonState
{
	None = 0,

	Pressed,
	Held,
	Released
};

struct BindData
{
public:
	BindData() : action(nullptr), buttonState(ButtonState::None) {}
	BindData(std::function<void()> _action, ButtonState _buttonState) : action(_action), buttonState(_buttonState) {}
	~BindData() = default;

	inline void Execute() { action(); }
	inline ButtonState GetButtonState() const { return buttonState; }

private:
	std::function<void()> action;
	ButtonState buttonState;
};

class InputHandler
{
public:
	InputHandler() = delete;
	~InputHandler() = delete;
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

	static bool Initialise(HWND hWnd);
	static void HandleInput();
	static inline void ClearBindings() { keyboardActions.clear(); mouseActions.clear(); }
	static void SetMouseMode(DirectX::Mouse::Mode mouseMode);

	static inline void BindKeyToAction(DirectX::Keyboard::Keys key, BindData bindData) { keyboardActions[key] = bindData; }
	static inline bool GetKey(DirectX::Keyboard::Keys key) { return keyboardState.IsKeyDown(key); }
	static inline bool GetKeyDown(DirectX::Keyboard::Keys key) { return keyboardStateTracker.IsKeyPressed(key); }
	static inline bool GetKeyUp(DirectX::Keyboard::Keys key) { return keyboardStateTracker.IsKeyReleased(key); }
	static inline void ClearKeyBindings() { keyboardActions.clear(); }
	static inline void ClearKeyBinding(DirectX::Keyboard::Keys key) { keyboardActions.erase(key); }

	static inline void BindMouseButtonToAction(MouseButton mouseButton, BindData bindData) { mouseActions[mouseButton] = bindData; }
	static bool GetMouseButton(MouseButton mouseButton);
	static bool GetMouseButtonDown(MouseButton mouseButton);
	static bool GetMouseButtonUp(MouseButton mouseButton);
	static inline void ClearMouseButtonBindings() { mouseActions.clear(); }
	static inline void ClearMouseButtonBinding(MouseButton mouseButton) { mouseActions.erase(mouseButton); }

private:
	static std::unique_ptr<DirectX::Keyboard> keyboard;
	static DirectX::Keyboard::State keyboardState;
	static DirectX::Keyboard::KeyboardStateTracker keyboardStateTracker;
	static std::unordered_map<DirectX::Keyboard::Keys, BindData> keyboardActions;

	static std::unique_ptr<DirectX::Mouse> mouse;
	static DirectX::Mouse::State mouseState;
	static DirectX::Mouse::ButtonStateTracker mouseStateTracker;
	static std::unordered_map<MouseButton, BindData> mouseActions;

	static DirectX::Mouse::Mode currentMouseMode;
};

