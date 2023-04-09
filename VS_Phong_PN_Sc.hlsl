#include "transform_matrix.hlsli"
#include "scale_cbuf.hlsl"

struct VS_OUT
{
    float3 world_pos : POSITION;
    float3 normal : NORMAL;
    float4 pos : SV_POSITION;
};



VS_OUT main(float3 pos : POSITION, float3 n : NORMAL)
{
	// to not translate normals, we need to cast the transformation matrix to a 3x3 
	// so we just rotate the normal, by the objects rotation matrix 
    VS_OUT vso;
    vso.normal = mul(n, (float3x3) model);
    vso.world_pos = (float3) mul(float4(pos * scale, 1.0f), model);
    vso.pos = mul(float4(pos * scale, 1.0f), model_view_projection);
    return vso;
}