
struct vertexIn
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 nor : NORMAL0;
};

struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
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

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);

	nor = mul(float4(IN.nor, 1.0f), World).xyz;
	nor = normalize(nor);

	float3 dir;
	float  len;
	float  colD;
	float  colA;
	float  col;

	dir = PointLightPos - pos.xyz;
	len = length(dir);
	dir = dir / len;
	colD = saturate(dot(normalize(pos.xyz), dir));
	colA = saturate(1.0f / (PointLightAtt.x + PointLightAtt.y * len + PointLightAtt.z * len * len));
	col = colD * colA;

	OUT.pos = pos;
	OUT.col = (saturate(dot(nor, DirectionalLight)) * 0.5f + 0.5f) + col;

	return OUT;
}

