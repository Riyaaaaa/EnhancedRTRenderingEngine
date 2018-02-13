#include "BasePassCommon.hlsl"

float4 ps_main(pixcelIn IN) : SV_Target
{
    float3 emissive = IN.col.xyz;
    float3 diffuse = 0.0f;
    float3 specular = 0.0f;

    float specularParam = 0.5f;
    float roughness = 0.2f;

    int i = 0;
    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numDirectionalLights) break;
        diffuse += DirectionalLighting(DirectionalLights[i].xyz, IN.norw.xyz);
        specular += SpecularBRDF(DirectionalLights[i], IN.posw, IN.norw, Eye, specularParam, roughness);
    }

    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numPointLights) break;
        diffuse += PointLighting(PLightParams[i], IN.posw.xyz, IN.norw.xyz);
        specular += SpecularBRDF(IN.posw - PLightParams[i].pos, IN.posw, IN.norw, Eye, specularParam, roughness);
    }

    float3 col = emissive * diffuse + specular;
    Shadowing(IN.shadowCoord, col);
    return float4(col, 1.0f);
}
