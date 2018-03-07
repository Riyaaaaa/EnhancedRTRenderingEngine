#include "BasePassCommon.hlsl"

float4 ps_main(pixcelIn IN) : SV_Target
{
    float3 albedo = IN.col.xyz;

    float3 diffuseColor = lerp(albedo, float3(0.0f, 0.0f, 0.0f), materialParameters.metallic);
    float3 specularColor = lerp(float3(0.04f, 0.04f, 0.04f), albedo, materialParameters.metallic);

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;

    // direct lighting
    int i = 0;
    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numDirectionalLights) break;
        if (IsVisibleFromLight(IN.shadowCoord)) {
            diffuse += diffuseColor * DirectionalLighting(DirectionalLights[i].xyz, IN.norw.xyz) * (1.0f / PI);
            specular += SpecularBRDF(DirectionalLights[i], IN.posw, IN.norw, Eye, specularColor, materialParameters.roughness);
        }
    }

    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numPointLights) break;
        float4 pointDir = IN.posw - PLightParams[i].pos;
        float depth = PointShadowMap.Sample(PShadowSampler, normalize(pointDir)).x;

        float4 absVec = abs(pointDir);
        float z = max(absVec.x, max(absVec.y, absVec.z));
        float normZComp = (100.0f + 0.10f) / (100.0f - 0.10f) - (2 * 100.0f * 0.10f) / (100.0f - 0.10f) / z;
        if ((normZComp + 1.0) * 0.5 <= depth + 0.005f) {
            diffuse += diffuseColor * PointLighting(PLightParams[i], IN.posw.xyz, IN.norw.xyz);
            specular += SpecularBRDF(IN.posw - PLightParams[i].pos, IN.posw, IN.norw, Eye, specularColor, materialParameters.roughness);
        }
    }

    float3 col = saturate(diffuse + specular);
    return float4(col, 1.0f);
}
