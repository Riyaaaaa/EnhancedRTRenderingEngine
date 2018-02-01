
struct vertexIn
{
	float3 pos : POSITION0;
	float2 tex : TEXCOORD0;
	float3 nor : NORMAL0;
};

struct vertexOut
{
	float4 pos : SV_POSITION;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix View;
	matrix Projection;
}

cbuffer ObjectBuffer : register(b1)
{
	matrix World;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	float4 pos = float4(IN.pos, 1.0f);

	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);

	OUT.pos = pos;

	return OUT;
}

