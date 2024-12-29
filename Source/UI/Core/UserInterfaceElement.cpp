#include "UserInterfaceElement.h"

#include "../../Assets/AssetHandler.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

UserInterfaceElement::UserInterfaceElement(std::string fontName, Vector2 _position, XMVECTORF32 _colour) : position(_position), colour(_colour), 
																										   text(""), isActive(true)
{
	font = AssetHandler::GetFont(fontName);
}

UserInterfaceElement::~UserInterfaceElement()
{
}

void UserInterfaceElement::Draw(SpriteBatch* spriteBatch)
{
	font->DrawString(spriteBatch, text.c_str(), position, colour, 0.0f, Vector2::Zero, Vector2::One);
}
