#include "ShaderIncludes.hlsli"

cbuffer ExternalData : register(b0)
{
	float4 colorTint; // 4-component float vector
	float3 cameraPosition;
	float roughness;
	float3 ambient;
	Light directionalLight1;
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	input.normal = normalize(input.normal);
	float3 toLight = normalize(-directionalLight1.Direction);
	float3 diffuseAmount = saturate(Diffuse(input.normal, toLight));
	float3 resultColor = (diffuseAmount * directionalLight1.Color * colorTint) +
		(ambient * colorTint);


	return float4(resultColor, 1);
}