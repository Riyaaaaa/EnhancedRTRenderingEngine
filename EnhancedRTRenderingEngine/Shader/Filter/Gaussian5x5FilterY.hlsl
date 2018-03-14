
struct pixcelIn
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float4 weight1;
    float4 weight2;
    float2 texsize;
}

float4 main(pixcelIn In) : SV_Target
{
    float MAP_HEIGHT = texsize.y;

    float3 col = weight1.x * txDiffuse.Sample(samLinear, float2(In.tex));
    col += weight1.y * (txDiffuse.Sample(samLinear, In.tex + float2(0, +2.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -2.0f / MAP_HEIGHT)));
    col += weight1.z * (txDiffuse.Sample(samLinear, In.tex + float2(0, +4.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -4.0f / MAP_HEIGHT)));
    col += weight1.w * (txDiffuse.Sample(samLinear, In.tex + float2(0, +6.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -6.0f / MAP_HEIGHT)));
    col += weight2.x * (txDiffuse.Sample(samLinear, In.tex + float2(0, +8.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -8.0f / MAP_HEIGHT)));
    col += weight2.y * (txDiffuse.Sample(samLinear, In.tex + float2(0, +10.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -10.0f / MAP_HEIGHT)));
    col += weight2.z * (txDiffuse.Sample(samLinear, In.tex + float2(0, +12.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -12.0f / MAP_HEIGHT)));
    col += weight2.w * (txDiffuse.Sample(samLinear, In.tex + float2(0, +14.0f / MAP_HEIGHT)) + txDiffuse.Sample(samLinear, In.tex + float2(0, -14.0f / MAP_HEIGHT)));

    return float4(col, 1.0f);
}
