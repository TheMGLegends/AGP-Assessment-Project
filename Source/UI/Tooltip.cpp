#include "Tooltip.h"

using namespace DirectX::SimpleMath;

Tooltip::~Tooltip()
{
}

void Tooltip::Update(float deltaTime)
{
	HWND hWnd = GetActiveWindow();

	if (hWnd == nullptr)
		return;

	RECT clientRect;
	GetClientRect(hWnd, &clientRect);

	// INFO: Set position top right corner of window
	SetPosition(Vector2(static_cast<float>(clientRect.right - 205), 0.0f));
}
