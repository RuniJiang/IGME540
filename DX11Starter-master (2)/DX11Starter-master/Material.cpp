#include "Material.h"

Material::Material(
    DirectX::XMFLOAT4 colorTint,
    float roughness,
    std::shared_ptr<SimpleVertexShader> vs, 
    std::shared_ptr<SimplePixelShader> ps)
    :
    colorTint(colorTint),
    roughness(roughness),
    vs(vs),
    ps(ps)
{
}

DirectX::XMFLOAT4 Material::GetColorTint(){ return colorTint;}
float Material::GetRoughness(){ return roughness;}
std::shared_ptr<SimpleVertexShader> Material::GetVertexShader(){ return vs;}
std::shared_ptr<SimplePixelShader> Material::GetPixelShader(){ return ps;}

void Material::SetColorTint(DirectX::XMFLOAT4 colorTint){ this->colorTint = colorTint;}
void Material::SetRoughness(float roughness) { this->roughness = roughness; }
void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> vs) { this->vs = vs; }
void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> ps) { this->ps = ps; }

