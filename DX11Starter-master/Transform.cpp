#include "Transform.h"

using namespace DirectX;
Transform::Transform() :
    position(0, 0, 0),
    pitchYawRoll(0, 0, 0),
    scale(0, 0, 0)
{
    //position = XMFLOAT3(0, 0, 0);
    //pitchYawRoll = XMFLOAT3(0, 0, 0);
    //scale = XMFLOAT3(1, 1, 1);
}

void Transform::MoveAbsolute(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
}

void Transform::MoveRelative(float x, float y, float z)
{

}

void Transform::Rotate(float p, float y, float r)
{
    pitchYawRoll.x += p;
    pitchYawRoll.y += y;
    pitchYawRoll.z += r;
}

void Transform::Scale(float x, float y, float z)
{
    scale.x *= x;
    scale.y *= y;
    scale.z *= z;
}

void Transform::SetPosition(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
}

void Transform::SetRotation(float p, float y, float r)
{
    pitchYawRoll.x = p;
    pitchYawRoll.y = y;
    pitchYawRoll.z = r;
}

void Transform::SetScale(float x, float y, float z)
{
    scale.x = x;
    scale.y = y;
    scale.z = z;
}

DirectX::XMFLOAT3 Transform::GetPosition(){ return position;}
DirectX::XMFLOAT3 Transform::GetRotation(){ return pitchYawRoll;}
DirectX::XMFLOAT3 Transform::GetScale(){ return scale;}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
    // Build individual transformation matrices
    //XMMATRIX t = XMMatrixTranslation(position.x, position.y, position.z);
    XMMATRIX t = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
    XMMATRIX r = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
    XMMATRIX s = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

    // Combine into a single world matrix
    XMMATRIX wm = s * r * t;

    // Store it somewhere
    //XMFLOAT4X4
}
