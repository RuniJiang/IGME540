#include "ShaderIncludes.hlsli"
// Define a constant buffer
// - Each buffer needs an identifier
// ExternalData is used to signify to the programmer the intent of this cbuffer
//           | 
//			 |  register tells the shader which slot we're referring to when accessing the var in cbuffer
//           |           |
//           V           V  all constant buffers are bound to b registers, 0 is an index (not  arbitrary) 
cbuffer ExternalData : register(b0)
{

	matrix world;
	matrix worldInvTranspose;
	matrix view;
	matrix proj;

	matrix shadowView;
	matrix shadowProjection;
}


// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// 
// - Input is exactly one vertex worth of data (defined by a struct)
// - Output is a single struct of data to pass down the pipeline
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
VertexToPixel main( VertexShaderInput input )
{
	// Set up output struct
	VertexToPixel output;

	// Mutiply the three matrices together first
	matrix wvp = mul(proj, mul(view, world));
	output.screenPosition = mul(wvp, float4(input.localPosition, 1.0f));
	
	output.uv = input.uv;
	output.normal = normalize(mul((float3x3)worldInvTranspose, input.normal));
	output.tangent = normalize(mul((float3x3)worldInvTranspose, input.tangent));
	output.worldPosition = mul(world, float4(input.localPosition, 1)).xyz;


	matrix shadowWVP = mul(shadowProjection, mul(shadowView, world));
	output.shadowMapPos = mul(shadowWVP, float4(input.localPosition, 1.0f));

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}