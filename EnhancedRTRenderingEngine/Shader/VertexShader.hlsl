
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

	OUT.pos = mul(IN.pos, World);		//���[���h�ϊ�
	OUT.pos = mul(OUT.pos, View);		//�r���[�ϊ�
	OUT.pos = mul(OUT.pos, Projection);	//�����ˉe�ϊ�
	OUT.col = IN.col;
	OUT.tex = IN.tex;

	return OUT;
}