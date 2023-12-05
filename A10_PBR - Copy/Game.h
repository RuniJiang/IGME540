#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "Entity.h"
#include "Camera.h"
#include "Material.h"
#include "SimpleShader.h"
#include "Light.h"
#include "Sky.h"

#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory>
#include <vector>


class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	//Helper Function
	void UIUpdate(float deltaTime);

private:

	// Initialization helper methods - feel free to customize, combine, remove, etc.
	void LoadShaders(); 
	void CreateGeometry();
	void CreateCameras();
	void CreateShadowMap();
	void RenderShaowMap();

	void CameraUI(std::shared_ptr<Camera> cam);
	void LightUI(Light& light, int index);

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//     Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr

	// Buffers to hold actual geometry data
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;


	// Shaders and shader-related constructs
	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimplePixelShader> customPS;
	std::shared_ptr<SimplePixelShader> customPS2;
	std::shared_ptr<SimpleVertexShader> skyVS;
	std::shared_ptr<SimplePixelShader> skyPS;
	std::shared_ptr<SimpleVertexShader> shadowVS;

	// Shadow
	int shadowMapResolution;
	float shadowProjectionSize;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> shadowDSV;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shadowSRV;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> shadowRasterizer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> shadowSampler;
	DirectX::XMFLOAT4X4 shadowViewMatrix;
	DirectX::XMFLOAT4X4 shadowProjectionMatrix;


	//Meshes
	std::vector < std::shared_ptr<Mesh>> meshes;
	std::vector < std::shared_ptr<Entity>> objects;
	std::vector < std::shared_ptr<Material>> materials;

	float speed;
	float speed1;
	float speed2;

	DirectX::XMFLOAT4 colorTint;
	DirectX::XMFLOAT3 ambientColor;

	bool showImGuiDemo;

	std::vector<std::shared_ptr<Camera>> cameras;
	std::shared_ptr<Camera> activedCamera;

	std::shared_ptr<Sky> sky;

	std::vector<Light> lights;
};

