#include "Camera.h"

using namespace DirectX;
Camera::Camera(
    float x, float y, float z, 
    float moveSpeed,
    float mouseLookSpeed, 
    float fov, 
    float aspectRatio)
    :
    moveSpeed(moveSpeed),
    mouseLookSpeed(mouseLookSpeed)
{
    // set up initial position
    transform.SetPosition(x, y, z);

    // Set up matriices
    UpdateViewMatrix();
    UpdateProjectionMatrix(fov, aspectRatio);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{
}

void Camera::UpdateViewMatrix()
{
    // Grab the transform data we'll need
    XMFLOAT3 pos = transform.GetPosition();
    XMFLOAT3 fwd = transform.GetForward();

    XMMATRIX view = XMMatrixLookToLH(
        XMLoadFloat3(&pos),
        XMLoadFloat3(&fwd),
        XMVectorSet(0, 1, 0, 0)
    );

    XMStoreFloat4x4(&viewMatrix, view);
}

void Camera::UpdateProjectionMatrix(float fov, float aspectRatio)
{
    XMMATRIX proj = XMMatrixPerspectiveFovLH(
        fov,
        aspectRatio,
        0.01f,       // Near clip distance
        1000.0f      // Far clip distance
    );

    XMStoreFloat4x4(&prjectionMatrix, proj);
}

Transform* Camera::GetTransform()
{
    return nullptr;
}

XMFLOAT4X4 Camera::GetView()
{
    return viewMatrix;
}

XMFLOAT4X4 Camera::GetProj()
{
    return prjectionMatrix;
}
