struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

Texture2D ShadowMap : register(t1);
SamplerState samLinear : register(s0);

struct PointLightParam
{
	float3 pos;
	float3 att;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
	float3 DirectionalLight;
	PointLightParam PLightParam;
}

float4 main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;
	//OUT.col = IN.col;

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

	//OUT.col = ShadowMap.Sample(samLinear, IN.tex);
return float4(col, 1.0f);
}