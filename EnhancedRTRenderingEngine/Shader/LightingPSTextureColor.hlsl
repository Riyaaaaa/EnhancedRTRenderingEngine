#include "LightingPSMain.hlsl"

Texture2D TextureMap : register(t10);
SamplerState samLinear : register(s10);

float4 main(pixcelIn IN) : SV_Target
{
    IN.col = TextureMap.Sample(samLinear, float2(IN.tex.x, 1.0 - IN.tex.y));
    return ps_main(IN);
}
