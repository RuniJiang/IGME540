#include "Camera.h"

using namespace DirectX;
Camera::Camera(
    float x, float y, float z, 
    float moveSpeed,
    float mouseLookSpeed, 
    float fov, 
    float aspectRatio,
    float nearClipDis,
    float farClipDis,
    bool isPerspective)
    :
    moveSpeed(moveSpeed),
    mouseLookSpeed(mouseLookSpeed),
    aspectRatio(aspectRatio),
    fov(fov),
    nearClipDis(nearClipDis),
    farClipDis(farClipDis),
    isPerspective(isPerspective),
    orthographicWidth(2)
{
    // set up initial position
    transform.SetPosition(x, y, z);

    // Set up matriices
    UpdateViewMatrix();
    UpdateProjectionMatrix(aspectRatio);
}

Camera::~Camera()
{
}

void Camera::Update(float dt)
{

    Input& input = Input::GetInstance();
    float speed = moveSpeed;

    if (input.KeyDown(VK_SHIFT))
    {
        speed *= 3;
    }
    if (input.KeyDown(VK_CONTROL))
    {
        speed *= 0.5;
    }
    if (input.KeyDown('W'))
    {
        transform.MoveRelative(0, 0, speed * dt);
    }
    if (input.KeyDown('S'))
    {
        transform.MoveRelative(0, 0, -speed * dt);
    }
    if (input.KeyDown('A'))
    {
        transform.MoveRelative(-speed * dt, 0, 0);
    }
    if (input.KeyDown('D'))
    {
        transform.MoveRelative(speed * dt, 0, 0);
    }
    if (input.KeyDown(VK_SPACE))
    {
        transform.MoveAbsolute(0, speed * dt, 0);
    }
    if (input.KeyDown('X'))
    {
        transform.MoveAbsolute(0, -speed * dt, 0);
    }


    if (input.MouseLeftDown())
    {
        int cursorMovementX = input.GetMouseXDelta();
        int cursorMovementY = input.GetMouseYDelta();

        transform.Rotate(cursorMovementY * dt, cursorMovementX * dt, 0);

        XMFLOAT3 rot = transform.GetRotation();
        if (rot.x > XM_PIDIV2) rot.x = XM_PIDIV2;
        if (rot.x < -XM_PIDIV2) rot.x = -XM_PIDIV2;
        transform.SetRotation(rot);
    }

    UpdateViewMatrix();
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

void Camera::UpdateProjectionMatrix(float aspectRatio)
{
    XMMATRIX proj;
    if (isPerspective)
    {
        proj = XMMatrixPerspectiveFovLH(
            fov,
            aspectRatio,
            nearClipDis,       // Near clip distance 0.01f
            farClipDis     // Far clip distance 1000.0f
        );
    }
    else // CameraProjectionType::ORTHOGRAPHIC
    {
        proj = XMMatrixOrthographicLH(
            orthographicWidth,	// Projection width (in world units)
            orthographicWidth / aspectRatio,// Projection height (in world units)
            nearClipDis,			// Near clip plane distance 
            farClipDis);			// Far clip plane distance
    }


    XMStoreFloat4x4(&prjectionMatrix, proj);
}

Transform* Camera::GetTransform(){ return &transform; }
XMFLOAT4X4 Camera::GetView(){ return viewMatrix;}
XMFLOAT4X4 Camera::GetProj(){ return prjectionMatrix;}
float Camera::GetFov(){ return fov;}
float Camera::GetNearClipDis(){ return nearClipDis;}
float Camera::GetFarClipDis(){ return farClipDis;}
bool Camera::GetisPerspective(){ return isPerspective;}

float Camera::GetOrthographicWidth()
{
    return orthographicWidth;
}

void Camera::SetFov(float fov)
{
    this->fov = fov;
    UpdateProjectionMatrix(aspectRatio);
}

void Camera::SetNearClipDis(float nearClipDis)
{
    this->nearClipDis = nearClipDis;
    UpdateProjectionMatrix(aspectRatio);
}

void Camera::SetFarClipDis(float farClipDis)
{
    this->farClipDis = farClipDis;
    UpdateProjectionMatrix(aspectRatio);
}

void Camera::SetisPerspective(bool isPerspective)
{
    this->isPerspective = isPerspective;
    UpdateProjectionMatrix(aspectRatio);
}

void Camera::SetOrthographicWidth(float orthographicWidth)
{
    this->orthographicWidth = orthographicWidth;
    UpdateProjectionMatrix(aspectRatio);
}
