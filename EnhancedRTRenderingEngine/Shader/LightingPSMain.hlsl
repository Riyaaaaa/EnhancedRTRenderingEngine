#include "BasePassCommon.hlsl"

float4 ps_main(pixcelIn IN) : SV_Target
{
    float3 albedo = IN.col.xyz;

    float3 diffuseColor = lerp(albedo, float3(0.0f, 0.0f, 0.0f), materialParameters.metallic);
    float3 specularCoef = lerp(float3(0.04f, 0.04f, 0.04f), albedo, materialParameters.metallic);
    float3 specularColor = EnviromentMap.Sample(EnviromentSampler, reflect(Eye, IN.norw));

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;

    // direct lighting
    int i = 0;
    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numDirectionalLights) break;
        if (IsVisibleFromDirectionalLight(IN.shadowCoord)) {
            float irradiance = saturate(dot(IN.norw.xyz, -DirectionalLights[i].xyz));
            diffuse += irradiance * DirectionalLighting(diffuseColor);
            specular += irradiance * SpecularBRDF(DirectionalLights[i], IN.posw, IN.norw, Eye, specularCoef, materialParameters.roughness);
        }
    }

    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numPointLights) break;
        if (IsVisibleFromPointLight(IN.posw.xyz, i)) {
            float3 dir = PLightParams[i].pos.xyz - IN.posw.xyz;
            float len = length(dir);
            float irradiance = saturate(dot(IN.norw.xyz, dir / len)); 
            diffuse += irradiance * PointLighting(diffuseColor, len, PLightParams[i].att);
            specular += irradiance * SpecularBRDF(dir / len, IN.posw, IN.norw, Eye, specularCoef, materialParameters.roughness);
        }
    }

    float3 col = saturate(diffuse + specular);
    return float4(col, 1.0f);
}
