#pragma once

#include <memory>
#include <DirectXmath.h>
#include "SimpleShader.h"

class Material
{
public:

	Material(DirectX::XMFLOAT2 colorTint,
		std::shared_ptr<SimpleVertexShader> vs,
		std::shared_ptr<SimplePixelShader> ps);

	DirectX::XMFLOAT3 GetColorTint();
	std::shared_ptr<SimpleVertexShader> GetVertexShader();
	std::shared_ptr<SimplePixelShader> GetPixelShader();

private:
	
	DirectX::XMFLOAT3 colorTint;
	std::shared_ptr<SimpleVertexShader> vs;
	std::shared_ptr<SimplePixelShader> ps;
};

