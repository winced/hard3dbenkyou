#include "func_u.hlsl"
#include "light_u.hlsl"
#include "cam_u.hlsl"
#include "backface_cbuf.hlsl"

//TODO: make a cbuf for material constants 
float4 main(float3 pixel_world_pos : POSITION, float3 n : NORMAL, float3 col : COLOR) : SV_TARGET
{
	n = normalize(n);
	if(backface_enabled)
    {
        n = back_face_normal(n, cam_pos, pixel_world_pos);
    }
    float specular_intensity = calc_specular_intensity(cam_pos, light_pos, light_specular_intensity, pixel_world_pos, n, float3(1.0f, 1.0f, 1.0f), 60.0f);
    float3 diffuse = calc_diffuse(light_diffuse_intensity, light_diffuse_color, light_pos, pixel_world_pos, n, dist_c_a, dist_c_b, dist_c_c);
    // specular is not affected by the color of the object, since it is just the reflection of the light. Whici is also why it needs to be affected by the diffuse
    return float4(col * (ambient + diffuse) + (diffuse * specular_intensity), 1.0f);
}