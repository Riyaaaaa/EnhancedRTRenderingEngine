
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
    float MAP_WIDTH = texsize.x;

    float3 col = weight1.x * txDiffuse.Sample(samLinear, float2(In.tex) + float2(+1.0f / MAP_WIDTH, 0));
    col += weight1.y * (txDiffuse.Sample(samLinear, In.tex + float2(+3.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-3.0f / MAP_WIDTH, 0)));
    col += weight1.z * (txDiffuse.Sample(samLinear, In.tex + float2(+5.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-5.0f / MAP_WIDTH, 0)));
    col += weight1.w * (txDiffuse.Sample(samLinear, In.tex + float2(+7.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-7.0f / MAP_WIDTH, 0)));
    col += weight2.x * (txDiffuse.Sample(samLinear, In.tex + float2(+9.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-9.0f / MAP_WIDTH, 0)));
    col += weight2.y * (txDiffuse.Sample(samLinear, In.tex + float2(+11.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-11.0f / MAP_WIDTH, 0)));
    col += weight2.z * (txDiffuse.Sample(samLinear, In.tex + float2(+13.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-13.0f / MAP_WIDTH, 0)));
    col += weight2.w * (txDiffuse.Sample(samLinear, In.tex + float2(+15.0f / MAP_WIDTH, 0)) + txDiffuse.Sample(samLinear, In.tex + float2(-15.0f / MAP_WIDTH, 0)));

    return float4(col, 1.0f);
}
