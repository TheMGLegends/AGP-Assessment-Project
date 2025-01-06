#pragma once

#include "../Core/Component.h"

class Transform : public Component
{
public:
	Transform(GameObject* _gameObject);
	virtual ~Transform() override;

	DirectX::XMMATRIX GetWorldMatrix() const;
	DirectX::XMMATRIX GetWorldMatrix(const DirectX::SimpleMath::Vector3& additionalScale) const;

	DirectX::SimpleMath::Vector3 GetForwardVector() const;
	DirectX::SimpleMath::Vector3 GetRightVector() const;
	DirectX::SimpleMath::Vector3 GetUpVector() const;

	inline void SetRotation(const DirectX::SimpleMath::Quaternion& _rotation) { rotation = _rotation; }
	inline const DirectX::SimpleMath::Quaternion& GetRotation() const { return rotation; }

	inline void SetScale(const DirectX::SimpleMath::Vector3& _scale) { scale = _scale; }
	inline const DirectX::SimpleMath::Vector3& GetScale() const { return scale; }

	void Rotate(const DirectX::SimpleMath::Vector3& eulerRotation);

	/// @brief Translates the position of the transform, making sure to update the previous position first
	void Translate(const DirectX::SimpleMath::Vector3& translation);

private:
	DirectX::SimpleMath::Quaternion rotation;
	DirectX::SimpleMath::Vector3 scale;
};

