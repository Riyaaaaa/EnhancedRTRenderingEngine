#include "BasePassCommon.hlsl"

#define USE_VSM 1
#define ENABLE_SHADING_DIRECT_LIGHT 0

float4 ps_main(pixcelIn IN) : SV_Target
{
    float3 albedo = IN.col.xyz;

    float3 diffuseColor = lerp(albedo, float3(0.0f, 0.0f, 0.0f), materialParameters.metallic);
    float3 specularCoef = lerp(float3(0.04f, 0.04f, 0.04f), albedo, materialParameters.metallic);
    float3 specularColor = EnviromentMap.Sample(EnviromentSampler, reflect(Eye, IN.norw));

    float3 diffuse = 0.0f;
    float3 specular = 0.0f;

#if ENABLE_SHADING_DIRECT_LIGHT
    // direct lighting
    int i = 0;
    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numDirectionalLights) break;
#if USE_VSM
        float shadowFactor = GetVarianceDirectionalShadowFactor(IN.shadowCoord);
#else
        float shadowFactor = IsVisibleFromDirectionalLight(IN.shadowCoord);
#endif
        if (shadowFactor > 0.0f) {
            float irradiance = saturate(dot(IN.norw.xyz, -DirectionalLights[i].xyz)) * shadowFactor;
            diffuse += irradiance * DirectionalLighting(diffuseColor);
            specular += irradiance * SpecularBRDF(-DirectionalLights[i], IN.posw, IN.norw, Eye, specularCoef, materialParameters.roughness);
        }
    }

    for (i = 0; i < LIGHT_MAX; i++) {
        if (i >= numPointLights) break;
#if USE_VSM
        float shadowFactor = GetVariancePointShadowFactor(IN.posw.xyz, i);
#else
        float shadowFactor = IsVisibleFromPointLight(IN.posw.xyz, i);
#endif
        if (shadowFactor > 0.0f) {
            float3 dir = PLightParams[i].pos.xyz - IN.posw.xyz;
            float len = length(dir);
            float irradiance = saturate(dot(IN.norw.xyz, dir / len)) * shadowFactor;
            diffuse += irradiance * PointLighting(diffuseColor, len, PLightParams[i].att);
            specular += irradiance * SpecularBRDF(dir / len, IN.posw, IN.norw, Eye, specularCoef, materialParameters.roughness);
        }
    }
#endif

    if (UseEnviromentMap > EPSILON) {
        specular += ReflectionFrensel(IN.posw, IN.norw, Eye, 0.2f, materialParameters.roughness * materialParameters.roughness) * materialParameters.metallic;
    }
    float3 col = saturate(diffuse + specular);

    if (UseLightMap > EPSILON) {
        col += diffuseColor * LightMap.Sample(LightSampler, IN.lightUV);
    }

    return float4(col, 1.0f);
}
