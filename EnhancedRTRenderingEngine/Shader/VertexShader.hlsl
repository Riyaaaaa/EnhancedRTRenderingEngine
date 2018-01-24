//“ü—Í—p
struct vertexIn
{
	float4 pos : POSITION;
	float4 col : COLOR0;
};

//o—Í—p
struct vertexOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

vertexOut main(vertexIn IN)
{
	vertexOut OUT;

	OUT.pos = IN.pos;
	OUT.col = IN.col;

	return OUT;
}