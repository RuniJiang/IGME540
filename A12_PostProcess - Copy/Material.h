#pragma once

#include <memory>
#include <DirectXmath.h>
#include "SimpleShader.h"
#include "Camera.h"
#include "Transform.h"

class Material
{
public:

	Material(DirectX::XMFLOAT4 colorTint,
		float roughness,
		std::shared_ptr<SimpleVertexShader> vs,
		std::shared_ptr<SimplePixelShader> ps);

	DirectX::XMFLOAT4 GetColorTint();
	float GetRoughness();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();

	void SetColorTint(DirectX::XMFLOAT4 colorTint);
	void SetRoughness(float roughness);
	void SetVertexShader(std::shared_ptr<SimpleVertexShader> vs);
	void SetPixelShader(std::shared_ptr<SimplePixelShader> ps);

	void AddTextureSRV(std::string name, Microsoft::WRL::ComPtr <ID3D11ShaderResourceView> srv);
	void AddSampler(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler);

	void PrepareMaterial(Transform* transform, std::shared_ptr<Camera> camera);

private:
	
	DirectX::XMFLOAT4 colorTint;
	float roughness;
	std::shared_ptr<SimpleVertexShader> vs;
	std::shared_ptr<SimplePixelShader> ps;

	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> textureSRVs;
	std::unordered_map<std::string, Microsoft::WRL::ComPtr<ID3D11SamplerState>> samplers;
};

