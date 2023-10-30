#include "Entity.h"

using namespace DirectX;

Entity::Entity(
	std::shared_ptr<Mesh> mesh, 
	std::shared_ptr<Material> material)
	:
	mesh(mesh),
	material(material)
{ }
std::shared_ptr<Mesh> Entity::GetMesh() { return mesh;}
Transform* Entity::GetTransform() {	return &transform ;}

std::shared_ptr<Material> Entity::GetMaterial(){ return material;}
void Entity::SetMaterial(std::shared_ptr<Material> material) { this->material = material; }

void Entity::Draw(
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
	std::shared_ptr<Camera> camera)
{

		material->GetVertexShader()->SetShader();
		material->GetPixelShader()->SetShader();

		std::shared_ptr<SimpleVertexShader> vs = material->GetVertexShader();

		vs->SetMatrix4x4("world", transform.GetWorldMatrix()); // // Strings here MUST match variable
		vs->SetMatrix4x4("worldInvTranspose", transform.GetWorldInverseTransposeMatrix());
		vs->SetMatrix4x4("view", camera->GetView()); // names in your
		vs->SetMatrix4x4("proj", camera->GetProj()); // shader�s cbuffer

		vs->CopyAllBufferData();

		std::shared_ptr<SimplePixelShader> ps = material->GetPixelShader();
		ps->SetFloat4("colorTint", material->GetColorTint()); 
		ps->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
		ps->SetFloat("roughness", material->GetRoughness());

		ps->CopyAllBufferData();

		mesh->Draw(context);
}
