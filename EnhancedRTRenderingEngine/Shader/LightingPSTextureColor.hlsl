#include "LightingPSMain.hlsl"

Texture2D TextureMap : register(t1);
SamplerState samLinear : register(s1);

float4 main(pixcelIn IN) : SV_Target
{
    IN.col = TextureMap.Sample(samLinear, IN.tex.xy);
    return ps_main(IN);
}
