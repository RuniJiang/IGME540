#pragma once
#include "Transform.h"

class Camera
{
public:
	Camera(float x, float y, float z,
		float moveSpeed,
		float mouseLookSpeed,
		float fov,
		float aspectRation);

	~Camera();

	// Update methods
	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float fov, float aspectRatio);

	Transform* GetTransform();
	XMFLOAT4X4 GetView();
	XMFLOAT4X4 GetProj();

private:
	// Matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 prjectionMatrix;

	Transform transform;

	float moveSpeed;
	float mouseLookSpeed;
};

