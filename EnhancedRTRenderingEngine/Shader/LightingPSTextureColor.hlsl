#include "BasePassCommon.hlsl"

float4 main(pixcelIn IN) : SV_Target
{
	float3 col =  TextureMap.Sample(samLinear, IN.tex.xy).xyz + Lighting(IN.posw, IN.norw);
	float spCol = Specular(DirectionalLight, IN.posw, IN.norw, Eye);;
	col += float3(spCol, spCol, spCol);
	Shadowing(IN.shadowCoord, col);
	return float4(col, 1.0f);
}
