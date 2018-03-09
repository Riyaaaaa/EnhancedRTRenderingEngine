#include "BasePassCommon.hlsl"

TextureCube TextureMap : register(t10);
SamplerState samLinear : register(s10);

float4 main(pixcelIn IN) : SV_Target
{
    return TextureMap.Sample(samLinear, IN.posw.xyz);
}
