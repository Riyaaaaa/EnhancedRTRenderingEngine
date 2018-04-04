
struct vertexIn
{
    float3 pos : POSITION0;
    float4 col : COLOR0;
    float thickness : THICKNESS;
};

struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float thickness : THICKNESS;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
}

cbuffer ObjectBuffer : register(b1)
{
    matrix World;
    matrix NormalWorld;
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    float4 pos = float4(IN.pos, 1.0f);
    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    OUT.pos = pos;
    OUT.col = IN.col;
    OUT.thickness = IN.thickness;

    return OUT;
}

