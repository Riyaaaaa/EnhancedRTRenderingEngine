struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;

//カラーをそのまま返す
OUT.col = IN.col;

return OUT.col;
}