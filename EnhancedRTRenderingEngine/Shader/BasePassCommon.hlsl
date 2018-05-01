#define EPSILON 1e-6
#define PI 3.14159265359

#define LIGHT_MAX 4
#define MAX_REFLECTION_MIPLEVEL 10

struct pixcelIn
{
    float4 pos : SV_POSITION;
    float4 posw : POSITION0;
    float4 norw : NORMAL0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
    float2 lightUV : TEXCOORD1;
    float4 shadowCoord : SHADOW_COORD;
};

Texture2D DirectionalShadowMap : register(t0);
TextureCube PointShadowMap : register(t1);
SamplerState ShadowSampler : register(s0);
SamplerState PShadowSampler : register(s1);

TextureCube EnviromentMap : register(t2);
SamplerState EnviromentSampler : register(s2);

Texture2D LightMap : register(t3);
SamplerState LightSampler : register(s3);

struct PointLightParam
{
    float4 pos;
    float4 att;
};

struct MaterialParameters
{
    float3 BaseColor;
    float metallic;
    float roughness;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix DirectionalLightView[LIGHT_MAX];
    matrix DirectionalLightProjection[LIGHT_MAX];
    matrix PointLightView[LIGHT_MAX][6];
    matrix PointLightProjection[LIGHT_MAX];
    float4 DirectionalLights[LIGHT_MAX];
    PointLightParam PLightParams[LIGHT_MAX];
    float numDirectionalLights;
    float numPointLights;
    float4 Eye;
}

cbuffer MaterialBuffer : register(b1)
{
    MaterialParameters materialParameters;
    float UseLightMap;
    float UseEnviromentMap;
}

float3 PointLighting(float3 diffuseColor, float distance, float3 attenuation) {
    return diffuseColor * saturate(1.0f / (attenuation.x + attenuation.y * distance + attenuation.z * distance * distance));
}

float3 DirectionalLighting(float3 diffuseColor) {
    return diffuseColor / PI;
}

float GetVarianceDirectionalShadowFactor(float4 shadowCoord) {
    float w = 1.0f / shadowCoord.w;
    float2 stex = float2((1.0f + shadowCoord.x * w) * 0.5f, (1.0f - shadowCoord.y * w) * 0.5f);
    
    float2 depth = DirectionalShadowMap.Sample(ShadowSampler, stex.xy).xy;

    float depth_sq = depth.x * depth.x;
    float variance = depth.y - depth_sq;
    variance = min(1.0f, max(0.0f, variance + 0.0001f));

    float fragDepth = shadowCoord.z * w;
    float md = fragDepth - depth.x;
    float p = variance / (variance + (md * md));

    return max(p, fragDepth <= depth.x);
}

bool IsVisibleFromDirectionalLight(float4 shadowCoord) {
    float w = 1.0f / shadowCoord.w;
    float2 stex = float2((1.0f + shadowCoord.x * w) * 0.5f, (1.0f - shadowCoord.y * w) * 0.5f);
    float depth = DirectionalShadowMap.Sample(ShadowSampler, stex.xy).x;

    if (shadowCoord.z * w <= depth + 0.005f) {
        return true;
    }
    return false;
}

float GetVariancePointShadowFactor(float3 posw, int index) {
    float3 pointDir = posw - PLightParams[index].pos;
    float3 depth = PointShadowMap.Sample(PShadowSampler, normalize(pointDir));

    float3 absVec = abs(pointDir);
    float z = max(absVec.x, max(absVec.y, absVec.z));
    float fragDepth = 100.0f / (100.0f - 0.10f) - (100.0f * 0.10f) / (100.0f - 0.10f) / z;

    float depth_sq = depth.x * depth.x;
    float variance = depth.y - depth_sq;
    variance = min(1.0f, max(0.0f, variance + 0.0001f));

    float md = fragDepth - depth.x;
    float p = variance / (variance + (md * md));

    return max(p, fragDepth <= depth.x);
}

bool IsVisibleFromPointLight(float3 posw, int index) {
    float3 pointDir = posw - PLightParams[index].pos;
    float depth = PointShadowMap.Sample(PShadowSampler, normalize(pointDir)).x;
    float3 absVec = abs(pointDir);
    float z = max(absVec.x, max(absVec.y, absVec.z));

    float normZComp = 100.0f / (100.0f - 0.10f) - (100.0f * 0.10f) / (100.0f - 0.10f) / z;
    return normZComp <= depth + 0.005f;
}

// Frensel equations approximated by Schlick
float3 FrenselEquations(float3 reflectionCoef, float3 H, float3 V) {
    return (reflectionCoef + (1.0f - reflectionCoef) * pow(1.0 - saturate(dot(V, H)), 5.0));
}

float FrenselEquations(float reflectionCoef, float3 H, float3 V) {
    return (reflectionCoef + (1.0f - reflectionCoef) * pow(1.0 - saturate(dot(V, H)), 5.0));
}


// Microfacet distribution function
// GGX(Throwbridge-Reiz) model
// DGGX(h) = ƒ¿^2 / ƒÎ((nEh)^2(ƒ¿^2 - 1) + 1)^2 
float MicrofacetDistFunc(float roughness, float dotNH) {
    float rough2 = roughness * roughness;
    float d = dotNH * dotNH * (rough2 - 1.0f) + 1.0f;
    return  rough2 / (PI * d * d);
}

// The geometrical attenuation factor
// Smith model
float GeometryAttenuationFactor(float a, float dotNV, float dotNL) {
    float k = a * a * 0.5 + EPSILON;
    float gl = dotNL / (dotNL * (1.0f - k) + k);
    float gv = dotNV / (dotNV * (1.0f - k) + k);
    return gl * gv;
}


float SpecularBRDF(float3 normalizedLightDir, float4 posw, float4 norw, float4 eye, float3 specular, float roughness) {
    float3 N = norw.xyz;
    float3 V = normalize(eye.xyz - posw.xyz);
    float3 L = normalizedLightDir;

    float dotNL = saturate(dot(N, normalizedLightDir));
    float dotNV = saturate(dot(N, V));
    float3 H = normalize(L + V); // half vector
    float dotNH = saturate(dot(N, H));
    float dotVH = saturate(dot(V, H));
    float dotLV = saturate(dot(L, V));
    float a = roughness * roughness;
    
    float D = MicrofacetDistFunc(a, dotNH);
    float G = GeometryAttenuationFactor(a, dotNV, dotNL);
    float F = FrenselEquations(specular, H, V);
    return (F * G * D) / (4.0 * dotNL * dotNV + EPSILON);
}

float3 ReflectionFrensel(float4 posw, float4 norw, float4 eye, float eta, float roughness)
{
    float3 N = norw;
    float3 I = normalize(posw.xyz - eye);
    float3 R = reflect(I, N);
    float3 T = refract(I, N, eta);
    float fresnel = FrenselEquations(pow(eta - 1 / eta + 1, 2), N, I);

    float Mip = MAX_REFLECTION_MIPLEVEL * roughness;

    float3 reflecColor = EnviromentMap.SampleLevel(EnviromentSampler, R, Mip);
    float3 refracColor = EnviromentMap.SampleLevel(EnviromentSampler, T, Mip);

    float3 col = lerp(refracColor, reflecColor, fresnel);

    return col;
}
