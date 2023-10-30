#pragma once

#include <memory>

#include <wrl/client.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"

class Entity
{
public:
	Entity(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);

	std::shared_ptr<Mesh> GetMesh();
	Transform* GetTransform();
	std::shared_ptr<Material> GetMaterial();
	void SetMaterial(std::shared_ptr<Material> material);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
		std::shared_ptr<Camera> camera);

private:
	std::shared_ptr<Material> material;
	Transform transform;
	std::shared_ptr<Mesh> mesh;
};

