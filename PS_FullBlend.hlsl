#include "rt_info_cbuf.hlsl"
Texture2D offscreen : register(t3);

sampler smplr : register(s0);

static const int radius = 3;

static const int divisor = 4 * radius * radius;

float4 main(float2 uv_pos : UV_Position) : SV_TARGET
{
    float dx = 1.0f / width;
    float dy = 1.0f / height;
    float3 max_color = { 0.0f, 0.0f, 0.0f};
    float alpha = 0.0f; 
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            float4 tex_col = offscreen.Sample(smplr, float2(uv_pos.x + x * dx, uv_pos.y + y * dy));
            max_color = max(tex_col.rgb, max_color);
            alpha += tex_col.a;
        }
    }
       
    return float4(max_color, alpha/divisor);
}