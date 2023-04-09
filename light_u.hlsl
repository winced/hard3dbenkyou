
cbuffer light_cbuf : register(b0)
{
	float3 light_pos;
	float3 ambient;
	float3 light_diffuse_color;
	float light_diffuse_intensity;
	float dist_c_a;
	float dist_c_b;
	float dist_c_c;
	float light_specular_intensity;
};
