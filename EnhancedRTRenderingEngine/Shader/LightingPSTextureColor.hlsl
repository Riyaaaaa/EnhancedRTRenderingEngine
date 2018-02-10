#include "BasePassCommon.hlsl"

float4 main(pixcelIn IN) : SV_Target
{
	float3 col = Lighting(IN.posw, IN.norw) + TextureMap.Sample(samLinear, IN.tex.xy);
	Shadowing(IN.shadowCoord, col);
	return float4(col, 1.0f);
}
