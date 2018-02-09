struct pixcelIn
{
    float4 pos : SV_POSITION;
    float4 col : COLOR;
};

float4 main(pixcelIn IN) : SV_Target
{
    return IN.col;
}