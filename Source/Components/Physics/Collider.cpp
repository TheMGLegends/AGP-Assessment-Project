#include "Collider.h"

#include "../../Game/GameObjects/Core/GameObject.h"
#include "../../Utilities/Debugging/DebugUtils.h"

using namespace DebugUtils;
using namespace DirectX;
using namespace Microsoft::WRL;

std::unique_ptr<BasicEffect> Collider::batchEffect = nullptr;
std::unique_ptr<PrimitiveBatch<VertexPositionColor>> Collider::primitiveBatch = nullptr;
ComPtr<ID3D11InputLayout> Collider::batchInputLayout = nullptr;

Collider::Collider(GameObject* _gameObject) : Component(_gameObject), colliderType(Type::None), colour(Colors::LimeGreen), isTrigger(false)
{
	// INFO: Bind callbacks to the game object functions
	if (GameObject* gameObject = GetGameObject())
	{
		OnCollision = std::bind(&GameObject::OnCollision, gameObject, std::placeholders::_1);
		OnTrigger = std::bind(&GameObject::OnTrigger, gameObject, std::placeholders::_1);
	}
}

HRESULT Collider::InitialiseWireframes(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	HRESULT hResult = { S_OK };

	batchEffect = std::make_unique<BasicEffect>(device);
	batchEffect->SetVertexColorEnabled(true);
	primitiveBatch = std::make_unique<PrimitiveBatch<VertexPositionColor>>(deviceContext);

	const void* vertexShaderBytecode = nullptr;
	size_t vertexShaderBytecodeLength = 0;
	batchEffect->GetVertexShaderBytecode(&vertexShaderBytecode, &vertexShaderBytecodeLength);

	hResult = device->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, 
										vertexShaderBytecode, vertexShaderBytecodeLength, &batchInputLayout);

	if (FAILED(hResult))
	{
		LogError("Collider::InitialiseWireframes(): Failed to create input layout!");
		return hResult;
	}

	return S_OK;
}
