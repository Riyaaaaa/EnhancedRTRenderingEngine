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

cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
	float3 DirectionalLight;
	float3 PointLightPos;
	float3 PointLightAtt;
}

float4 main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;
	//OUT.col = IN.col;

float3 dir;
float  len;
float  colD;
float  colA;
float  col;

dir = PointLightPos - IN.posw.xyz;
len = length(dir);
dir = dir / len;
colD = saturate(dot(IN.norw.xyz, dir));
colA = 1.0f;
//colD = 1.0f;
//colA = saturate(1.0f / (PointLightAtt.x + PointLightAtt.y * len + PointLightAtt.z * len * len));
col = colD * colA;

	//OUT.col = ShadowMap.Sample(samLinear, IN.tex);
	//return float4(col, col, col, 1.0f);
return float4(col, col, col, 1.0f);
}