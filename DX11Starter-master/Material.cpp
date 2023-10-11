#include "Material.h"

Material::Material(
    DirectX::XMFLOAT2 colorTint,
    std::shared_ptr<SimpleVertexShader> vs, 
    std::shared_ptr<SimplePixelShader> ps)
{
}

DirectX::XMFLOAT3 Material::GetColorTint()
{
    return colorTint;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
    return vs;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
    return ps;
}
