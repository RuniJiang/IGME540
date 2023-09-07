#pragma once

#include <d3d11.h>
#include "Vertex.h"
#include <wrl/client.h> 

class Mesh
{
public:
	Mesh(
		Vertex* vertices, // a pointer to vertex objects
		int vertexCount,  // An integer specifying the number of vertices in the vertex array
		unsigned int* indices, // a pointer to unsigned integers for the indices
		int indexCount, //integer specifying the number of indices in the index array
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

