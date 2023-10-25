#include "ShaderIncludes.hlsli"


cbuffer ExternalData : register(b0)
{
	float4 colorTint;       // 4-component float vector
	float3 cameraPosition;  
	float roughness;
	float3 ambient;

	Light lights[5];
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
	float3 finalResult = ambient * (float3)colorTint;

	for (int i = 0; i < 5; i++)
	{
		Light light = lights[i];
		light.Direction = normalize(light.Direction);
		float spec;

		switch (lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			finalResult += DirectionalLight(light, input.normal, cameraPosition, input.worldPosition, roughness, (float3)colorTint);
			break;

		case LIGHT_TYPE_POINT:
			finalResult += PointLight(light, input.normal, cameraPosition, input.worldPosition, roughness, (float3)colorTint);
			break;

		case LIGHT_TYPE_SPOT:
			break;
		}

	}

	return float4(finalResult, 1);
}