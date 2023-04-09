#include "func_u.hlsl"
#include "light_u.hlsl"
#include "cam_u.hlsl"
#include "backface_cbuf.hlsl"
#include "obj_color_cbuf.hlsl"
#include "specular_cbuf.hlsl"


float4 main(float3 pixel_world_pos : POSITION, float3 n : NORMAL) : SV_TARGET
{
	// multiply * operator in hlsl is hadamard 
	// to dot, call dot 
	// saturate, saturates a vector/scalar from 0.0f to 1.0f
	// these functions are called intrinsic functions 
	n = normalize(n);
    if(backface_enabled)
    {
        n = back_face_normal(n, cam_pos, pixel_world_pos);
    }
    float specular_intensity = calc_specular_intensity(cam_pos, light_pos, light_specular_intensity, pixel_world_pos, n, float3(obj_specular_intensity, obj_specular_intensity, obj_specular_intensity), obj_specular_power);
	float3 diffuse = calc_diffuse(light_diffuse_intensity, light_diffuse_color, light_pos, pixel_world_pos, n, dist_c_a, dist_c_b, dist_c_c);
	return float4(saturate(obj_color.rgb * (ambient + diffuse) + (diffuse * specular_intensity)), 1.0f);
}