cbuffer ExternalData : register(b0)
{
    float4 colorTint; // 4-component float vector
    float time;
}

// Struct representing the data we expect to receive from earlier pipeline stages
struct VertexToPixel
{
    float4 screenPosition : SV_POSITION;
    float2 uv : TEXCOORD0; // Use TEXCOORD0 for UV
};

float random(float2 s)
{
    return frac(sin(dot(s, float2(12.9898, 78.233))) * 43758.5453123);
}
#define PI 3.14

float3 CalculateColor(float u, float v, float time, float2 uv)
{
    
    const int numWaves = 2;
    float speed[numWaves] = { 7.0f, 10.0f};
    float freq[numWaves] = { 3.0f, 2.0f};
    float amp[numWaves] = { 2.0f, 0.75f};
    float glow[numWaves] = { 1.0f, 0.5f};
    float intensities[numWaves] = { 5.0f, 20.0f};
    float3 colors[numWaves] = {
        float3(1.0f, 0.1f, 0.1f),
        float3(0.1f, 1.0f, 0.1f)
    };

    float3 total = 0.0f;
    for (int i = 0; i < numWaves; i++)
    {
        // Calculate a sine wave based on the U tex coord, frequency, time, and amplitude
        float f = freq[i] * 2.0 * PI ; // Multiple of 2pi for wrapping
        float s = sin(u * f + time * speed[i] + sin(time)) * amp[i] + random(uv);

        // How far from the sine wave are we?
        float dist = 1.0 - saturate(pow(abs(v - s), glow[i]));
        total += colors[i] * dist * intensities[i];
    }

    return total;
}

float4 main(VertexToPixel input) : SV_TARGET
{
    float v = (input.uv.y * -2 + 1) * 10.0;
    float3 result = CalculateColor(input.uv.x, v, time, input.uv);
    return (float4(result, 1) + colorTint) / 2;
}
