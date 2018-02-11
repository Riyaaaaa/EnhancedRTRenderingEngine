
struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 shadowCoord : SHADOW_COORD;
};

Texture2D TextureMap : register(t0);
Texture2D ShadowMap : register(t1);
SamplerState samLinear : register(s0);

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
	colD = saturate(dot(norw.xyz, dir));
	colA = saturate(1.0f / (PLightParam.att.x + PLightParam.att.y * len + PLightParam.att.z * len * len));

	return colD * colA;
}

void Shadowing(float4 shadowCoord, inout float3 col) {
	float w = 1.0f / shadowCoord.w;
	float2 stex = float2((1.0f + shadowCoord.x * w) * 0.5f, (1.0f - shadowCoord.y * w) * 0.5f);
	float depth = ShadowMap.Sample(samLinear, stex.xy).x;

	if (shadowCoord.z * w > depth + 0.0005f) {
		col = col * 0.5f;
	}
}

float Specular(float4 lightDir, float4 posw, float4 norw, float4 eye) {
	float3 H = normalize(normalize(lightDir.xyz) + normalize(eye.xyz - posw.xyz));
	return dot(norw.xyz, H);
}
