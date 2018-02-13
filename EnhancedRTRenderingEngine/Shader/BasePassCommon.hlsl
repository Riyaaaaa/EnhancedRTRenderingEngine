#define EPSILON 1e-6
#define PI 3.14159265359

struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 shadowCoord : SHADOW_COORD;
};

Texture2D ShadowMap : register(t0);
SamplerState ShadowSampler : register(s0);

struct PointLightParam
{
	float4 pos;
	float4 att;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
	matrix Shadow;
	float4 DirectionalLight;
	PointLightParam PLightParam;
	float4 Eye;
}

float Lighting(float3 posw, float3 norw) {
	float3 dir;
	float  len;
	float  colD;
	float  colA;
	float3  col;

	dir = PLightParam.pos.xyz - posw.xyz;
	len = length(dir);
	dir = dir / len;
	
    // point light
    colD = saturate(dot(norw.xyz, dir));
	colA = saturate(1.0f / (PLightParam.att.x + PLightParam.att.y * len + PLightParam.att.z * len * len));
    
    //directional light
    float colB = saturate(dot(norw, -DirectionalLight.xyz);

	return saturate(colD * colA + colB);
}

void Shadowing(float4 shadowCoord, inout float3 col) {
	float w = 1.0f / shadowCoord.w;
	float2 stex = float2((1.0f + shadowCoord.x * w) * 0.5f, (1.0f - shadowCoord.y * w) * 0.5f);
	float depth = ShadowMap.Sample(ShadowSampler, stex.xy).x;

	if (shadowCoord.z * w > depth + 0.0005f) {
		col = col * 0.5f;
	}
}


// Frensel equations approximated by Schlick
float3 FrenselEquations(float3 reflectionCoef, float3 H, float V) {
    return (reflectionCoef + (1.0f - reflectionCoef) * pow(1.0 - saturate(dot(V, H)), 5.0));
}


// Microfacet distribution function
// GGX(Throwbridge-Reiz) model
// DGGX(h) = Éø^2 / ÉŒ((nÅEh)^2(Éø^2 - 1) + 1)^2 
float MicrofacetDistFunc(float roughness, float3 dotNH) {
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


float SpecularBRDF(float4 lightDir, float4 posw, float4 norw, float4 eye, float specular, float roughness) {
    float3 N = norw.xyz;
    float3 V = normalize(eye.xyz - posw.xyz);
    float3 L = normalize(-lightDir.xyz);

    float dotNL = saturate(dot(N, L));
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
