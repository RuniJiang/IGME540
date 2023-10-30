#ifndef __GGP_SHADER_INCLUDES__SHADERHELPER // Each .hlsli file needs a unique identifier!
#define __GGP_SHADER_INCLUDES__SHADERHELPER

#define MAX_SPECULAR_EXPONENT 256.0f
#define LIGHT_TYPE_DIRECTIONAL 0
#define LIGHT_TYPE_POINT 1
#define LIGHT_TYPE_SPOT 2


struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 localPosition	: POSITION;     // XYZ position
	float3 normal           : NORMAL;
	float2 uv               : TEXCOORD;
};

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 screenPosition	: SV_POSITION;	// XYZW position (System Value Position)
	float2 uv : TEXCOORD;
    float3 normal : NORMARL;
    float3 worldPosition : POSITIONT;
};

// ---------------------------------------------------------------------------------- //
// ---------------------------------- LIGHTS ---------------------------------------- //
struct Light
{
    int Type; // Which kind of light? 0, 1 or 2 (see above)
    float3 Direction; // Directional and Spot lights need a direction
    float Range; // Point and Spot lights have a max range for attenuation
    float3 Position; // Point and Spot lights have a position in space
    float Intensity; // All lights need an intensity
    float3 Color; // All lights need a color
    float SpotFalloff; // Spot lights need a value to define their “cone” size
    float3 Padding; // Purposefully padding to hit the 16-byte boundary

};


float Diffuse(float3 normal, float3 dirToLight)
{
    return saturate(dot(normal, dirToLight));
}

float Specular(float3 normal, float3 dirToLight, float roughness, float3 cameraPosition, float3 worldPosition)
{
	// Specular
	float specExponent;
	if (roughness > 0.05)
	{
		specExponent = (1.0f - roughness) * MAX_SPECULAR_EXPONENT;
		
	}
	else
	{
		specExponent = 0;
	}

	float3 V = normalize(cameraPosition - worldPosition);
	float3 R = reflect(-dirToLight, normal);
	float spec = pow(saturate(dot(R, V)), specExponent);
	return spec;
}

float Attenuate(Light light, float3 worldPos)
{
	float dist = distance(light.Position, worldPos);
	float att = saturate(1.0f - (dist * dist / (light.Range * light.Range)));
	return att * att;
}

// LIGHT HELPER FUNCTIONS

// DIRECTIONAL LIGHTS
float3 DirectionalLight(Light light, float3 normal, float3 cameraPosition, float3 worldPosition, float roughness, float3 colorTint)
{
    float3 toLight = normalize(-light.Direction);
	
    float diffuse = Diffuse(normal, toLight);
    float specular = Specular(normal, toLight, roughness, cameraPosition, worldPosition);
	
    return (diffuse * colorTint + specular) * light.Intensity * light.Color;
}

float3 PointLight(Light light, float3 normal, float3 cameraPosition, float3 worldPosition, float roughness, float3 colorTint)
{
    float3 toLight = normalize(light.Position - worldPosition);
	
    float attenuate = Attenuate(light, worldPosition);
    float diffuse = Diffuse(normal, toLight);
    float specular = Specular(normal, toLight, roughness, cameraPosition, worldPosition);
	
    return (diffuse * colorTint + specular) * attenuate * light.Intensity * light.Color;
}

#endif