#include "BasePassCommon.hlsl"

float4 main(pixcelIn IN) : SV_Target
{
float3 col = IN.col.xyz + Lighting(IN.posw, IN.norw);
float spCol = Specular(DirectionalLight, IN.posw, IN.norw, Eye);
col += float3(spCol, spCol, spCol);
Shadowing(IN.shadowCoord, col);

//float threshold = ShadowMap.SampleCmpLevelZero(ShadowMap, IN.shadowCoord.xy, IN.shadowCoord.z)
//float3 shadowColor = lerp(float(0.25f, 0.25f, 0.25f), float3(1.0f, 1.0f, 1.0f), threshold);
//col = (col - float3(0.95f, 0.0f, 0.0f)) * 20.0f;
//return IN.shadowCoord;
//return float4(1.0f, 0.0f, 0.0f, 1.0f);
return float4(col, 1.0f);
}
