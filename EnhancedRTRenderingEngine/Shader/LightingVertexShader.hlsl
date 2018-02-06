
struct vertexIn
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 nor : NORMAL0;
};

struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 posw : POSITION0;
	float4 norw : NORMAL0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
	float3 DirectionalLight;
	float3 PointLightPos;
	float3 PointLightAtt;
}

cbuffer ObjectBuffer : register(b1)
{
	matrix World;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;
	float3 nor;

	float4 pos = float4(IN.pos, 1.0f);

	OUT.posw = mul(pos, World);
	pos = mul(OUT.posw, View);
	pos = mul(pos, Projection);

	nor = mul(float4(IN.nor, 1.0f), World).xyz;
	nor = normalize(nor);

	OUT.pos = pos;
	OUT.norw = float4(nor, 1.0f);
	OUT.col = saturate(dot(nor, -DirectionalLight)) * 0.5f + 0.5f;
	OUT.tex = pos.xy;

	return OUT;
}

