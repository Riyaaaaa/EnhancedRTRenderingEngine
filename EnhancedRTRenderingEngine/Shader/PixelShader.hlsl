struct pixcelIn
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
    float2 tex : TEXCOORD0;
};

float4 main(pixcelIn IN) : SV_Target
{
    return IN.col;
}