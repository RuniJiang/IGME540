
cbuffer ExternalData : register(b0)
{
	float4 colorTint; // 4-component float vector
}

// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 screenPosition : SV_POSITION;
	float2 uv : TEXCOORD;
};

// https://gamedev.stackexchange.com/questions/32681/random-number-hlsl
float rand_1_05(in float2 uv)
{
	float2 noise = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
	return abs(noise.x + noise.y) * 0.5;
}

float2 rand_2_10(in float2 uv) {
	float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
	float noiseY = sqrt(1 - noiseX * noiseX);
	return float2(noiseX, noiseY);
}

float2 rand_2_0004(in float2 uv)
{
	float noiseX = (frac(sin(dot(uv, float2(12.9898, 78.233))) * 43758.5453));
	float noiseY = (frac(sin(dot(uv, float2(12.9898, 78.233) * 2.0)) * 43758.5453));
	return float2(noiseX, noiseY) * 0.004;
}
float random(float2 s)
{
	return frac(sin(dot(s, float2(12.9898, 78.233))) * 43758.5453123);
}


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// ------------------------------------------aw--------------
float4 main(VertexToPixel input) : SV_TARGET
{
	return (float4(0, rand_2_10(input.uv), 1) + colorTint) / 2; // Returns purple (or another color of your choice)
}