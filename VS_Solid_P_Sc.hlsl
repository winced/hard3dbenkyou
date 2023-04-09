#include "transform_matrix.hlsli"
#include "scale_cbuf.hlsl"

float4 main(float3 pos : POSITION) : SV_POSITION
{
    return mul(float4(pos * scale, 1.0f), model_view_projection);
}