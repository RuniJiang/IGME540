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
Texture2D MetalnessMap			: register(t3);

Texture2D ShadowMap : register(t4);

SamplerState BasicSampler		: register(s0);
SamplerComparisonState ShadowSampler : register(s1); 

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
	// Perform the perspective divide (divide by W) ourselves
	input.shadowMapPos /= input.shadowMapPos.w;
	// Convert the normalized device coordinates to UVs for sampling
	float2 shadowUV = input.shadowMapPos.xy * 0.5f + 0.5f;
	shadowUV.y = 1 - shadowUV.y; // Flip the Y
	// Grab the distances we need: light-to-pixel and closest-surface
	float distToLight = input.shadowMapPos.z;
	// Get a ratio of comparison results using SampleCmpLevelZero()
	float shadowAmount = ShadowMap.SampleCmpLevelZero(
		ShadowSampler,
		shadowUV,
		distToLight).r;
	//// For testing, just return black where there are shadows.
	//if (distShadowMap < distToLight)
	//return float4(0, 0, 0, 1);

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
			
			float3 lightResult = DirLightPBR(light, input.normal, input.worldPosition, cameraPosition, roughness, metalness, surfaceColor.rgb, specularColor);
			// If this is the first light, apply the shadowing result
			if (i == 0)
			{
				lightResult *= shadowAmount;
			}
			// Add this light's result to the total light for this pixel
			finalResult += lightResult;
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