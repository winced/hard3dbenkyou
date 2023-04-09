

float calc_specular_intensity(uniform float3 cam_pos_u, uniform float3 light_pos_u, float light_specular_intensity_u, float3 pixel_world_pos_u, float3 n_u, float3 specular_color_u, float specular_power_u)
{
	float3 to_cam_vector_u = cam_pos_u - pixel_world_pos_u;
	float3 to_light_vector_u = light_pos_u - pixel_world_pos_u;
	float3 reflection_vector_u = 2 * dot(to_light_vector_u, n_u) * n_u - to_light_vector_u;
	return specular_color_u * light_specular_intensity_u * pow(max(0.0f, dot(normalize(to_cam_vector_u), normalize(reflection_vector_u))), specular_power_u);
}


float3 calc_diffuse(uniform float light_diffuse_intensity_u, uniform float3 light_diffuse_color_u, uniform float3 light_pos_u, float3 pixel_world_pos_u, float3 n_u, float dist_c_a_u, float dist_c_b_u, float dist_c_c_u)
{
	float3 to_light_vector_u = light_pos_u - pixel_world_pos_u;
	float distance_to_light_u = length(to_light_vector_u);
	float3 to_light_vector_n_u = to_light_vector_u / distance_to_light_u;
	float distance_scale_u = 1.0f / (dist_c_a_u * distance_to_light_u * distance_to_light_u + dist_c_b_u * distance_to_light_u + dist_c_c_u);
	//calculate diffuse intensity and multiply by the diffuse color to get the scale the diffuse color by the intensity
	return light_diffuse_color_u * (light_diffuse_intensity_u * (max(dot(to_light_vector_n_u, n_u), 0.0f) * (distance_scale_u)));
}

float3 map_normal(uniform sampler smplr_u, uniform Texture2D normal_map_u, float3 n_u, float2 tex_cord_u, float3 tangent_u, float3 bitangent_u)
{
	const float3x3 tan_mat_u = float3x3(normalize(tangent_u), normalize(bitangent_u), normalize(n_u));
	n_u = normal_map_u.Sample(smplr_u, tex_cord_u).xyz;
	n_u.x = n_u.x * 2.0f - 1.0f;
	n_u.y = n_u.y * 2.0f - 1.0f;
	n_u.z = (n_u.z * 2.0f - 1.0f);
	n_u = mul(n_u, tan_mat_u);
	return n_u;
}

float3 back_face_normal(float3 n_u, float3 cam_pos_u, float3 pixel_pos_u)
{
    float3 vec_to_p_u = cam_pos_u - pixel_pos_u;
    return dot(n_u, -vec_to_p_u) >= 0.0f ? -n_u : n_u;
}

