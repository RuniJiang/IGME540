#pragma once

#include <memory>

#include <wrl/client.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"

class Entity
{
public:
	Entity(std::shared_ptr<Mesh> mesh);

	std::shared_ptr<Mesh> GetMesh();
	Transform* GetTransform();
	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, 
		Microsoft::WRL::ComPtr<ID3D11Buffer> vsConstantBuffer,
		std::shared_ptr<Camera> camera);

private:
	Transform transform;
	std::shared_ptr<Mesh> mesh;
};

