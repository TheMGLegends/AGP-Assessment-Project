#pragma once

#include <d3d11.h>
#include <DirectXColors.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <string>

class UserInterfaceElement
{
public:
	UserInterfaceElement(std::string fontName, DirectX::SimpleMath::Vector2 _position, DirectX::XMVECTORF32 _colour = DirectX::Colors::White);
	virtual ~UserInterfaceElement();

	virtual void Update(float deltaTime) {}
	void Draw(DirectX::SpriteBatch* spriteBatch);

	inline void SetPosition(DirectX::SimpleMath::Vector2 _position) { position = _position; }
	inline const DirectX::SimpleMath::Vector2& GetPosition() const { return position; }

	inline void SetScale(DirectX::SimpleMath::Vector2 _scale) { scale = _scale; }

	inline void SetColour(DirectX::XMVECTORF32 _colour) { colour = _colour; }
	inline const DirectX::XMVECTORF32& GetColour() const { return colour; }

	inline void SetText(std::string _text) { text = _text; }
	inline const std::string& GetText() { return text; }

	inline void SetActive(bool _isActive) { isActive = _isActive; }
	inline bool GetIsActive() const { return isActive; }

private:
	DirectX::SpriteFont* font;
	DirectX::SimpleMath::Vector2 position;
	DirectX::SimpleMath::Vector2 scale;
	DirectX::XMVECTORF32 colour;
	std::string text;
	bool isActive;
};

