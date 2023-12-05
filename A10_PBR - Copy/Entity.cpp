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

	material->PrepareMaterial(&transform, camera);
	mesh->Draw(context);
}
