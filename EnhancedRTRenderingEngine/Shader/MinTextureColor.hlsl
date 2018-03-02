struct pixcelIn
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

#define epsilon 1e-2

float4 main(pixcelIn IN) : SV_Target
{
    float3 col = txDiffuse.Sample(samLinear, IN.tex);
    return float4((col.xxx - float3(0.9f, 0.9f, 0.9f)) * 10.0f, 1.0f);
    return float4((1.0 - col.x) < epsilon ? 0.0f : 1.0f, 1.0f, 1.0f, 1.0f);
}
