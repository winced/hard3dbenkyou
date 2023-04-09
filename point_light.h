#pragma once
#include <DirectXMath.h>
#include "sphere.h"
#include "stage_manager.h"


struct point_light_cbuf
{
	alignas(16)DirectX::XMFLOAT3 light_pos;
	alignas(16)DirectX::XMFLOAT3 ambient;
	alignas(16)DirectX::XMFLOAT3 diffuse_color;
	float diffuse_intensity;
	float dist_c_a;
	float dist_c_b;
	float dist_c_c;
	float specular_intensity;
};
class point_light
{
public:
	point_light(const DirectX::XMFLOAT3& pos, codex& cdx, graphics& gfx);
	void reset();
	void show_imgui_control_window(graphics& gfx);
	void add_to_manager(stage_manager& mangr);
	void set(graphics& gfx);
private:
	std::unique_ptr<pixel_constant_buffer<point_light_cbuf>> position_const_buffer;
	std::unique_ptr<sphere> sphere_p;
	point_light_cbuf light_info;
};