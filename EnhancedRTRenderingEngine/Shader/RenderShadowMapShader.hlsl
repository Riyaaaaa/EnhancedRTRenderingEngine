struct VertexOut
{
    float4 pos : SV_POSITION;
    float4 depthcol: COLOR0;
};

float4 main(VertexOut IN) : SV_Target
{
    return IN.depthcol;
}