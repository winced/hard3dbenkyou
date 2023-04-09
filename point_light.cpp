#include "point_light.h"

point_light::point_light(const DirectX::XMFLOAT3& pos, codex& cdx, graphics& gfx)
{
	reset();
	light_info.light_pos = pos;
	position_const_buffer = std::make_unique<pixel_constant_buffer<point_light_cbuf>>(light_info, 0u, gfx);
	sphere_p = std::make_unique<sphere>(0.5f, "point_light", DirectX::XMFLOAT4{1.0f, 1.0f, 1.0f, 1.0f}, true, cdx, gfx);
}

void point_light::reset()
{
	light_info =
	{
		{0.0f, 90.0f, 0.0f},
		{0.03f, 0.03f, 0.03f},
		{1.0f, 1.0f, 1.0f},
		{30.0f},
		{0.0075f},
		{0.045f},
		{1.0f},
		{1.0f}
	};
}

void point_light::show_imgui_control_window(graphics& gfx)
{
	if (ImGui::Begin("Light"))
	{
		ImGui::Text("Position");
		ImGui::SliderFloat("X", &light_info.light_pos.x, -160.0f, 160.0f, "%.1f");
		ImGui::SliderFloat("Y", &light_info.light_pos.y, -160.0f, 160.0f, "%.1f");
		ImGui::SliderFloat("Z", &light_info.light_pos.z, -160.0f, 160.0f, "%.1f");
		ImGui::SliderFloat("dist_c_a", &light_info.dist_c_a, 0.0f, 0.3f, "%.6f");
		ImGui::SliderFloat("dist_c_b", &light_info.dist_c_b, 0.0f, 0.3f, "%.6f");
		ImGui::SliderFloat("dist_c_c", &light_info.dist_c_c, 0.1f, 1.0f, "%.6f");
		ImGui::SliderFloat("diffuse intensity", &light_info.diffuse_intensity, 0.0f, 100.0f, "%.6f");
		ImGui::SliderFloat("specular intensity", &light_info.specular_intensity, 0.0f, 100.0f, "%.6f");
	}
	ImGui::End();
}

void point_light::add_to_manager(stage_manager& mangr)
{
	mangr.add_drawable(sphere_p.get(), technique::normal);
}

void point_light::set(graphics& gfx)
{
	sphere_p->set_pos(light_info.light_pos);
	position_const_buffer->update(light_info, gfx);
	position_const_buffer->set(gfx);
}
