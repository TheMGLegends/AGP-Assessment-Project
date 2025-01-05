#pragma once

#include "../Core/Component.h"

#include <d3d11.h>
#include <DirectXColors.h>
#include <Effects.h>
#include <functional>
#include <memory>
#include <PrimitiveBatch.h>
#include <VertexTypes.h>
#include <wrl.h>

class Collider : public Component
{
public:
	enum class Type
	{
		None = 0,

		Box,
		Sphere
	};

public:
	Collider(GameObject* _gameObject);

	virtual void DrawWireframe(ID3D11DeviceContext* deviceContext) {}

	inline Collider::Type GetColliderType() const { return colliderType; }

	inline void SetColour(DirectX::XMVECTORF32 _colour) { colour = _colour; }

	inline void SetIsTrigger(bool _isTrigger) { isTrigger = _isTrigger; }
	inline bool GetIsTrigger() const { return isTrigger; }

	inline void ExecuteOnCollision(std::shared_ptr<Collider> other) { OnCollision(other); }
	inline void ExecuteOnTrigger(std::shared_ptr<Collider> other) { OnTrigger(other); }

	static HRESULT InitialiseWireframes(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	static DirectX::BasicEffect* GetBatchEffect() { return batchEffect.get(); }

protected:
	static std::unique_ptr<DirectX::BasicEffect> batchEffect;
	static std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>> primitiveBatch;
	static Microsoft::WRL::ComPtr<ID3D11InputLayout> batchInputLayout;

protected:
	Collider::Type colliderType;
	DirectX::XMVECTORF32 colour;

private:
	bool isTrigger;

	std::function<void(std::shared_ptr<Collider>)> OnCollision;
	std::function<void(std::shared_ptr<Collider>)> OnTrigger;
};

