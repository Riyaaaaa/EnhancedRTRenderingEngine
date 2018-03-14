
struct pixcelIn
{
    float4 pos : SV_POSITION;
    float2 tex : TEXCOORD0;
    float2 viewpos : TEXCOORD1;
};

Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    float time;
    float2 size;
}

float4 main(pixcelIn In) : SV_Target
{
    float t = time / 12.;
float2 viewpos = (In.viewpos + 1.0f) * 0.5f;
    float2 r = size, o = viewpos.xy * size - r / 2.;
    o = float2(length(o) * 0.4 / r.y - .06, atan2(o.y, o.x));

    float tmp1 = t + o.y + cos(o.x) * cos(t);

    float4 s = 0.07 * cos(1.5 * float4(0,1,2,3) + float4(tmp1, tmp1, tmp1, tmp1)) * (clamp(cos(t + 3.14 / 2.) + 0.5, 0.7, 1.0)),
    e = s.yzwx,
    f = max(o.xxxx - s,e - o.xxxx);
    return dot(clamp(f*r.y,0.,1.), 112.*(s - e)) * (s - float4(0.1, 0.1, 0.1, 0.1));
}
