
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
	matrix World;
	matrix View;
	matrix Projection;
	float3 Light;
}

cbuffer ObjectBuffer : register(b1)
{
	matrix LocalTransMatrix;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;
	float3 nor;
	float col;

	float4 pos = float4(IN.pos, 1.0f);

	pos = mul(pos, LocalTransMatrix);
	pos = mul(pos, World);
	pos = mul(pos, View);
	pos = mul(pos, Projection);

	nor = mul(float4(IN.nor, 1.0f), LocalTransMatrix).xyz;
	nor = normalize(nor);

	OUT.pos = pos;
	OUT.col = saturate(dot(nor, Light)) * 0.5f + 0.5f;

	return OUT;
}

