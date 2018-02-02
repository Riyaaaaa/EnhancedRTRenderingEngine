struct pixcelIn
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 tex : TEXCOORD0;
};

Texture2D ShadowMap : register(t1);
SamplerState samLinear : register(s0);

float4 main(pixcelIn IN) : SV_Target
{
	pixcelIn OUT;
	OUT.col = ShadowMap.Sample(samLinear, IN.tex);
	return float4(OUT.col.z, OUT.col.z, OUT.col.z, OUT.col.z);
}