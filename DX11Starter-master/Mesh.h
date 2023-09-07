#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <wrl/client.h> 

class Mesh
{
public:
	Mesh(Vertex* vertices, int vertexCount, unsigned int* indices, int indexCount,
		Microsoft::WRL::ComPtr<ID3D11Device> device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> context);
	~Mesh();

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
	void Draw(float deltaTime, float totalTime);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	int indexCount;
};

