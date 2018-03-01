#include "BasePassCommon.hlsl"

float4 main(pixcelIn IN) : SV_Target
{
    return float4(IN.tex.x, IN.tex.y, 0, 1);
}