
#define LIGHT_MAX 4

struct vertexIn
{
    float3 pos : POSITION0;
    float2 tex : TEXCOORD0;
    float3 nor : NORMAL0;
    float4 col : COLOR0;
};

struct vertexOut
{
    float4 pos : SV_POSITION;
    float4 posw : POSITION0;
    float4 norw : NORMAL0;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
    float4 shadowCoord : SHADOW_COORD;
};

struct PointLightParam
{
    float4 pos;
    float4 att;
};

cbuffer ConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix DirectionalLightView[LIGHT_MAX];
    matrix DirectionalLightProjection[LIGHT_MAX];
    matrix PointLightView[LIGHT_MAX][6];
    matrix PointLightProjection[LIGHT_MAX];
    float4 DirectionalLights[LIGHT_MAX];
    PointLightParam PLightParams[LIGHT_MAX];
    float numDirectionalLights;
    float numPointLights;
    float4 Eye;
}

cbuffer ObjectBuffer : register(b1)
{
    matrix World;
    matrix NormalWorld;
}

vertexOut main(vertexIn IN)
{
    vertexOut OUT;

    float3 nor;
    float4 pos = float4(IN.pos, 1.0f);

    OUT.posw = mul(pos, World);
    pos = mul(OUT.posw, View);
    pos = mul(pos, Projection);
    nor = mul(float4(IN.nor, 1.0f), NormalWorld).xyz;
    nor = normalize(nor);

    OUT.pos = pos;
    OUT.norw = float4(nor, 1.0f);

    OUT.col = IN.col;
    OUT.tex = IN.tex;
    OUT.shadowCoord = mul(mul(OUT.posw, DirectionalLightView[0]), DirectionalLightProjection[0]);

    return OUT;
}

