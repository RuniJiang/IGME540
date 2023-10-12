#include "Entity.h"
#include "BufferStructs.h"

using namespace DirectX;

Entity::Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material)
{ 
	this->mesh = mesh;
	this->material = material;
}
std::shared_ptr<Mesh> Entity::GetMesh() { return mesh;}
Transform* Entity::GetTransform() {	return &transform ;}

std::shared_ptr<Material> Entity::GetMaterial()
{
	return material;
}

void Entity::SetMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

void Entity::Draw(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
	Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer,
	std::shared_ptr<Camera> camera)
{
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();

		VertexShaderExternalData vsData;
		vsData.colorTint = XMFLOAT4(1.0f, 0.5f, 0.5f, 1.0f);
		vsData.worldMatrix = transform.GetWorldMatrix();
		vsData.viewMatrix = camera->GetView();
		vsData.projMatrix = camera->GetProj();

		D3D11_MAPPED_SUBRESOURCE mappedBuffer = {};
		context->Map(vsConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedBuffer);
		memcpy(mappedBuffer.pData, &vsData, sizeof(vsData));
		context->Unmap(vsConstantBuffer.Get(), 0);

		context->VSSetConstantBuffers(
			0, // Which slot (register) to bind the buffer to?
			1, // How many are we activating? Can do multiple at once
			vsConstantBuffer.GetAddressOf()); // Array of buffers (or the address of one)

		mesh->Draw(context);
}
