struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 shadowCoord : SHADOW_COORD;
};

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
}

float4 main(pixcelIn IN) : SV_Target
{
float3 dir;
float  len;
float  colD;
float  colA;
float3  col;

dir = PLightParam.pos.xyz - IN.posw.xyz;
len = length(dir);
dir = dir / len;
colD = saturate(dot(IN.norw.xyz, dir));
colA = saturate(1.0f / (PLightParam.att.x + PLightParam.att.y * len + PLightParam.att.z * len * len));
col = colD * colA + IN.col.xyz;

float w = 1.0f / IN.shadowCoord.w;
float2 stex = float2((1.0f + IN.shadowCoord.x * w) * 0.5f, (1.0f - IN.shadowCoord.y * w) * 0.5f);
float depth = ShadowMap.Sample(samLinear, stex.xy).x;

if (IN.shadowCoord.z * w > depth + 0.0005f) {
	col = col * 0.5f;
}

//float threshold = ShadowMap.SampleCmpLevelZero(ShadowMap, IN.shadowCoord.xy, IN.shadowCoord.z)
//float3 shadowColor = lerp(float(0.25f, 0.25f, 0.25f), float3(1.0f, 1.0f, 1.0f), threshold);
//col = (col - float3(0.95f, 0.0f, 0.0f)) * 20.0f;
//return IN.shadowCoord;
return float4(col, 1.0f);
}
