
struct vertexIn
{
    float3 pos : POSITION0;
    float2 tex : TEXCOORD0;
    float3 nor : NORMAL0;
    float4 col : COLOR0;
};

struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 depthcol: COLOR0;
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

VertexOut main(vertexIn IN)
{
    VertexOut OUT;

    float4 pos = float4(IN.pos, 1.0f);

    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);
    OUT.pos = pos;

    float z = pos.z / pos.w;
    OUT.depthcol = float4(z, z * z, 0.0f, 1.0f);

    return OUT;
}

