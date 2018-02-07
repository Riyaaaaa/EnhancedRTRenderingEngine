struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float2 shadowCoord : SHADOW_COORD;
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

dir = PLightParam.pos - IN.posw.xyz;
len = length(dir);
dir = dir / len;
colD = saturate(dot(IN.norw.xyz, dir));
colA = saturate(1.0f / (PLightParam.att.x + PLightParam.att.y * len + PLightParam.att.z * len * len));
col = colD * colA + IN.col.xyz;
//col = ShadowMap.Sample(samLinear, IN.shadowCoord);
return float4(col, 1.0f);
}