
struct vertexIn
{
	float4 pos : POSITION0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
};

cbuffer ConstantBuffer : register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
}

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	OUT.pos = mul(IN.pos, World);		//ワールド変換
	OUT.pos = mul(OUT.pos, View);		//ビュー変換
	OUT.pos = mul(OUT.pos, Projection);	//透視射影変換
	OUT.col = IN.col;
	OUT.tex = IN.tex;

	return OUT;
}