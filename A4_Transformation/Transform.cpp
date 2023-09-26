#include "Transform.h"

using namespace DirectX;
Transform::Transform() :
    position(0, 0, 0),
    pitchYawRoll(0, 0, 0),
    scale(1, 1, 1)
{
    matrixDirty = false;
    XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&worldInverseTranspose, XMMatrixIdentity());
}

void Transform::MoveAbsolute(float x, float y, float z)
{
    position.x += x;
    position.y += y;
    position.z += z;
    matrixDirty = true;
}

void Transform::MoveAbsolute(DirectX::XMFLOAT3 position)
{
    this->position.x += position.x;
    this->position.y += position.y;
    this->position.z += position.z;
    matrixDirty = true;
}
//
//void Transform::MoveRelative(float x, float y, float z)
//{
//
//}

void Transform::Rotate(float p, float y, float r)
{
    pitchYawRoll.x += p;
    pitchYawRoll.y += y;
    pitchYawRoll.z += r;
    matrixDirty = true;
}

void Transform::Rotate(DirectX::XMFLOAT3 rotation)
{
    pitchYawRoll.x += rotation.x;
    pitchYawRoll.y += rotation.y;
    pitchYawRoll.z += rotation.z;
    matrixDirty = true;
}

void Transform::Scale(float x, float y, float z)
{
    scale.x *= x;
    scale.y *= y;
    scale.z *= z;
    matrixDirty = true;
}

void Transform::Scale(DirectX::XMFLOAT3 scale)
{
    this->scale.x *= scale.x;
    this->scale.y *= scale.y;
    this->scale.z *= scale.z;
    matrixDirty = true;
}

void Transform::SetPosition(float x, float y, float z)
{
    position.x = x;
    position.y = y;
    position.z = z;
    matrixDirty = true;
}

void Transform::SetPosition(DirectX::XMFLOAT3 position)
{
    this->position.x = position.x;
    this->position.y = position.y;
    this->position.z = position.z;
    matrixDirty = true;
}

void Transform::SetRotation(float p, float y, float r)
{
    pitchYawRoll.x = p;
    pitchYawRoll.y = y;
    pitchYawRoll.z = r;
    matrixDirty = true;
}

void Transform::SetRotation(DirectX::XMFLOAT3 rotation)
{
    pitchYawRoll.x = rotation.x;
    pitchYawRoll.y = rotation.y;
    pitchYawRoll.z = rotation.z;
    matrixDirty = true;
}

void Transform::SetScale(float x, float y, float z)
{
    scale.x = x;
    scale.y = y;
    scale.z = z;
    matrixDirty = true;
}

void Transform::SetScale(DirectX::XMFLOAT3 scale)
{
    this->scale.x = scale.x;
    this->scale.y = scale.y;
    this->scale.z = scale.z;
    matrixDirty = true;
}

DirectX::XMFLOAT3 Transform::GetPosition(){ return position;}
DirectX::XMFLOAT3 Transform::GetRotation(){ return pitchYawRoll;}
DirectX::XMFLOAT3 Transform::GetScale(){ return scale;}

DirectX::XMFLOAT4X4 Transform::GetWorldMatrix()
{
    UpdateWorldMatrix();
    return worldMatrix;
}

DirectX::XMFLOAT4X4 Transform::GetWorldInverseTransposeMatrix()
{
    UpdateWorldMatrix();
    return worldInverseTranspose;
}

void Transform::UpdateWorldMatrix()
{
    if(!matrixDirty)
    {
        return; 
    }
    // Build individual transformation matrices
    //XMMATRIX t = XMMatrixTranslation(position.x, position.y, position.z);
    XMMATRIX t = XMMatrixTranslationFromVector(XMLoadFloat3(&position));
    XMMATRIX r = XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&pitchYawRoll));
    XMMATRIX s = XMMatrixScalingFromVector(XMLoadFloat3(&scale));

    // Combine into a single world matrix
    XMMATRIX wm = s * r * t;

    XMStoreFloat4x4(&worldMatrix, wm);
    XMStoreFloat4x4(&worldInverseTranspose,
        XMMatrixInverse(0, XMMatrixTranspose(wm)));

    matrixDirty = false;
}
