#include "LightingPSMain.hlsl"

float4 main(pixcelIn IN) : SV_Target
{
    IN.col = float4(materialParameters.BaseColor, 1.0f);
    return ps_main(IN);
}
