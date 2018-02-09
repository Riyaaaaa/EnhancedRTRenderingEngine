
struct vertexIn
{
    float3 pos : POSITION0;
    float4 col : COLOR0;
};

struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    OUT.pos = float4(IN.pos, 1.0f);
    OUT.col = IN.col;

    return OUT;
}