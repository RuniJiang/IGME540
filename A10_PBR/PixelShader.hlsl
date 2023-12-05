#include "ShaderIncludes.hlsli"


cbuffer ExternalData : register(b0)
{
	float4 colorTint;       // 4-component float vector
	float3 cameraPosition;  
	float roughness;

	Light lights[5];
}


//Texture2D SurfaceTexture	: register(t0); // Textures use "t" registers
//SamplerState BasicSampler : register(s0); // "s" registers for samplers
//Texture2D SpecularMap		: register(t1);
//Texture2D NormalMap		: register(t2); 

Texture2D Albedo				: register(t0);
Texture2D NormalMap				: register(t1);
Texture2D RoughnessMap			: register(t2);
Texture2D MetalnessMap				: register(t3);
SamplerState BasicSampler		: register(s0);


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
	input.tangent = normalize(input.tangent);

	input.normal = NormalMapping(NormalMap, BasicSampler, input.uv, input.normal, input.tangent);

	float roughness = RoughnessMap.Sample(BasicSampler, input.uv).r;
	float metalness = MetalnessMap.Sample(BasicSampler, input.uv).r;
	//float roughness = 0.9;
	//float metalness = 0;
	float3 albedoColor = pow(Albedo.Sample(BasicSampler, input.uv).rgb, 2.2f);

	float3 surfaceColor = albedoColor;
	surfaceColor *= (float3)colorTint;
	float3 finalResult = float3(0,0,0) * surfaceColor;

	float3 specularColor = lerp(F0_NON_METAL, albedoColor.rgb, metalness);

	for (int i = 0; i < 5; i++)
	{
		Light light = lights[i];
		light.Direction = normalize(light.Direction);

		switch (lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			finalResult += DirLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
			break;

		case LIGHT_TYPE_POINT:
			finalResult += PointLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
			break;

		case LIGHT_TYPE_SPOT:
			break;
		}

	}
	return float4(pow(finalResult, 1.0f / 2.2f), 1);
}