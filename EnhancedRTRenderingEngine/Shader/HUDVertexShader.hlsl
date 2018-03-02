struct vertexIn
{
    float3 pos : POSITION0;
    float2 tex : TEXCOORD0;
};

struct vertexOut
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
};

cbuffer ObjectBuffer : register(b1)
{
    matrix World;
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    OUT.pos = float4(IN.pos, 1.0f);
    OUT.pos = mul(OUT.pos, World);

    OUT.tex = IN.tex;

    return OUT;
}