
struct vertexIn
{
    float3 pos : POSITION0;
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

cbuffer ObjectBuffer : register(b1)
{
    matrix LocalTransMatrix;
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    float4 pos = float4(IN.pos, 1.0f);
    pos = mul(pos, LocalTransMatrix);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    OUT.pos = pos;
    OUT.col = IN.col;
    OUT.tex = IN.tex;

    return OUT;
}

