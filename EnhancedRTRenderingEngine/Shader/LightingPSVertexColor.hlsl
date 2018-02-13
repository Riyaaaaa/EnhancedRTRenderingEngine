#include "BasePassCommon.hlsl"

float4 main(pixcelIn IN) : SV_Target
{
    float3 emissive = IN.col.xyz;
    float diffuse = Lighting(IN.posw, IN.norw);
    float specular = SpecularBRDF(DirectionalLight, IN.posw, IN.norw, Eye, 0.5f, 0.2f);
    float3 col = emissive + float3(diffuse, diffuse, diffuse) + float3(specular, specular, specular);
    Shadowing(IN.shadowCoord, col);
    return float4(col, 1.0f);
}
