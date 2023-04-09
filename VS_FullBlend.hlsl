#include "rt_info_cbuf.hlsl"

struct vs_out
{
	float4 pos : SV_POSITION;
	float2 uv_pos : UV_POSITION; 
};


vs_out main( float2 pos : POSITION ) 
{
	vs_out output;
    output.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
    output.uv_pos = float2((pos.x + 1.0f) / 2.0f, (pos.y + 1.0f) / 2.0f);
	return output;
}