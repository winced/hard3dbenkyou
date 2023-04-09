#include "func_u.hlsl"
#include "cam_u.hlsl"
#include "light_u.hlsl"
#include "backface_cbuf.hlsl"

#include "obj_color_cbuf.hlsl"
#include "specular_cbuf.hlsl"

float4 main(float3 pixel_world_pos : POSITION, float3 n : NORMAL) : SV_TARGET
{
// multiply * operator in hlsl is hadamard 
// to dot, call dot 
// saturate, saturates a vector/scalar from 0.0f to 1.0f
// these functions are called intrinsic functions 
    normalize(n);
    float specular_intensity = calc_specular_intensity(cam_pos, light_pos, light_specular_intensity, pixel_world_pos, n, float3(obj_specular_intensity, obj_specular_intensity, obj_specular_intensity), obj_specular_power);
    float3 diffuse = calc_diffuse(light_diffuse_intensity, light_diffuse_color, light_pos, pixel_world_pos, n, dist_c_a, dist_c_b, dist_c_c);
// specular is not affected by the color of the object, since it is just the reflection of the light. Which is also why it needs to be affected by the diffuse
    return float4(saturate(obj_color.rgb * (ambient + diffuse) + (diffuse * specular_intensity)), 1.0f);
}