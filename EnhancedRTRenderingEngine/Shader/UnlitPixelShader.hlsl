#include "BasePassCommon.hlsl"

Texture2D TextureMap : register(t10);
SamplerState samLinear : register(s10);

float4 main(pixcelIn IN) : SV_Target
{
    return TextureMap.Sample(samLinear, IN.tex);
}
