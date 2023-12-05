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

void Material::AddTextureSRV(std::string shaderName, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
    textureSRVs.insert({ shaderName, srv });
}

void Material::AddSampler(std::string samplerName, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
    samplers.insert({samplerName, sampler});
}

void Material::PrepareMaterial(Transform* transform, std::shared_ptr<Camera> camera)
{
    // Shader
    vs->SetShader();
    ps->SetShader();

    // Vertex Shader
    vs->SetMatrix4x4("world", transform->GetWorldMatrix()); // // Strings here MUST match variable
    vs->SetMatrix4x4("worldInvTranspose", transform->GetWorldInverseTransposeMatrix());
    vs->SetMatrix4x4("view", camera->GetView()); // names in your
    vs->SetMatrix4x4("proj", camera->GetProj()); // shader�s cbuffer
    vs->CopyAllBufferData();

    // Pixel Shader
    ps->SetFloat4("colorTint", GetColorTint());
    ps->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
    ps->SetFloat("roughness", GetRoughness());
    ps->CopyAllBufferData();

    for (auto& texture : textureSRVs) { ps->SetShaderResourceView(texture.first.c_str(), texture.second.Get()); }
    for (auto& sampler : samplers) { ps->SetSamplerState(sampler.first.c_str(), sampler.second.Get()); }
}


