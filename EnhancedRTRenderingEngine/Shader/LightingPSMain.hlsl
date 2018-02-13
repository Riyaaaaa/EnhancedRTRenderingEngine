#include "BasePassCommon.hlsl"

float4 ps_main(pixcelIn IN) : SV_Target
{
    float3 albedo = IN.col.xyz;

    float3 diffuseColor = lerp(albedo, float3(0.0f, 0.0f, 0.0f), materialParameters.metallic);
    float3 specularColor = lerp(float3(0.04f, 0.04f, 0.04f), albedo, materialParameters.metallic);

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;

    int i = 0;
    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numDirectionalLights) break;
        diffuse += albedo * DirectionalLighting(DirectionalLights[i].xyz, IN.norw.xyz);
        specular += SpecularBRDF(DirectionalLights[i], IN.posw, IN.norw, Eye, specularColor, materialParameters.roughness);
    }

    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numPointLights) break;
        diffuse += albedo * PointLighting(PLightParams[i], IN.posw.xyz, IN.norw.xyz);
        specular += SpecularBRDF(IN.posw - PLightParams[i].pos, IN.posw, IN.norw, Eye, specularColor, materialParameters.roughness);
    }

    float3 col = saturate(diffuse + specular);
    Shadowing(IN.shadowCoord, col);
    return float4(col, 1.0f);
}
