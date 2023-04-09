// Vc = vertex color
// C = constant color (from cbuf)
// N = normal map
// S = specular map
// T = texture map 


//pixel constant buffers 
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


cbuffer obj_color_cbuf : register(b1)
{
    float4 obj_color;
};

cbuffer camera_pos_cbuf : register(b2)
{
    float3 cam_pos;
};

cbuffer specular_cbuf : register(b3)
{
    float obj_specular_intensity;
    float obj_specular_power;
    float padding[2];
};
//vertex constant buffers

cbuffer transformation_matrix : register(b0)
{
    float4x4 model;
    float4x4 model_view_projection;
};

//textures

SamplerState smplr : register(s0);

Texture2D tex : register(t0);

Texture2D specular_map : register(t1);

Texture2D normal_map : register(t2);
