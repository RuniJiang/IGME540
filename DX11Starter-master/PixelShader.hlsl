#include "ShaderIncludes.hlsli"


cbuffer ExternalData : register(b0)
{
	float4 colorTint; // 4-component float vector
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
	float3 finalResult = ambient * colorTint;

	for (int i = 0; i < 5; i++)
	{
		// Grab this light and normalize the direction (just in case)
		Light light = lights[i];
		float3 toLight;
		float3 diffuseAmount;
		float spec;
		// Run the correct lighting calculation based on the light's type
		switch (lights[i].Type)
		{
		case LIGHT_TYPE_DIRECTIONAL:
			toLight = normalize(-light.Direction);

			// Diffuse
			diffuseAmount = saturate(Diffuse(input.normal, toLight));

			// Specular
			spec = Specular(input.normal, toLight, roughness, cameraPosition, input.worldPosition);
			finalResult += (diffuseAmount * colorTint + spec) * lights[i].Intensity * lights[i].Color;
			break;

		case LIGHT_TYPE_POINT:
			toLight = normalize(light.Position - input.worldPosition);

			// Calculate the light amounts
			float atten = Attenuate(light, input.worldPosition);
			// Diffuse
			diffuseAmount = Diffuse(input.normal, toLight);
			// Specular
			spec = Specular(input.normal, toLight, roughness, cameraPosition, input.worldPosition);

			finalResult += (diffuseAmount * colorTint + spec) * atten * lights[i].Intensity * lights[i].Color;
			break;

		case LIGHT_TYPE_SPOT:
			break;
		}

	}

	return float4(finalResult, 1);
}