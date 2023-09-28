#pragma once
//#include <direct.h>
#include "Transform.h"
#include "Input.h"

class Camera
{
public:
	Camera(float x, float y, float z,
		float moveSpeed,
		float mouseLookSpeed,
		float fov,
		float aspectRation,
		float nearClipDis,
		float farClipDis,
		bool isPerspective);

	~Camera();

	// Update methods
	void Update(float dt);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	Transform* GetTransform();
	DirectX::XMFLOAT4X4 GetView();
	DirectX::XMFLOAT4X4 GetProj();

	float GetFov();
	float GetNearClipDis();
	float GetFarClipDis();
	bool GetisPerspective();
	float GetOrthographicWidth();

	void SetFov(float fov);
	void SetNearClipDis(float nearClipDis);
	void SetFarClipDis(float farClipDis);
	void SetisPerspective(bool isPerspetive);
	void SetOrthographicWidth(float orthoganalWidth);

private:
	// Matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 prjectionMatrix;

	Transform transform;

	float moveSpeed;
	float mouseLookSpeed;
	float aspectRatio;
	float fov;
	float nearClipDis;
	float farClipDis;

	bool isPerspective;

	float orthographicWidth;
};

