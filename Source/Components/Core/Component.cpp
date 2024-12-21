#include "Component.h"

using namespace DirectX::SimpleMath;

unsigned int Component::componentCount = 0;

Component::Component(GameObject* _gameObject) : position(Vector3::Zero), offset(Vector3::Zero), 
												isActive(true), canHaveMultiple(false), 
												gameObject(_gameObject), componentID(componentCount++)
{
}

Component::~Component()
{
}
