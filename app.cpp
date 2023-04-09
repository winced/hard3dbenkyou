#include <memory>
#include "app.h"

app::app()
	:
	wnd(1280, 960, "Direct3D benkyou"),
	lantern({ 0.0f, 90.0f, 0.0f }, cdx, wnd.get_graphics_c()),
	cam(wnd.get_graphics_c(), {0.0f, 0.0f, -20.0f}),
	sponza(wnd.get_graphics_c(), cdx, "C:\\Users\\Arska\\source\\repos\\HardW3\\hw3d\\models\\sponza\\sponza.obj", 0.2f),
	test_ball0(5.0f, "test_ball0", DirectX::XMFLOAT4(0.5f, 0.4f, 0.7f, 1.0f), false, cdx, wnd.get_graphics_c()),
	pickr(cam, wnd.get_graphics_c()),
	bldr(wnd.get_graphics_c(), cdx, technique_factory::create_outline_blend_desc(), technique::outline)
{
	p_depth_stencil_buffer = std::make_shared<depth_stencil_buffer>(wnd.get_graphics_c(), wnd.get_graphics_c().get_width(), wnd.get_graphics_c().get_height());
	p_render_target = std::make_shared<render_target>(wnd.get_graphics_c(), wnd.get_graphics_c().get_width(), wnd.get_graphics_c().get_height());
	mangr.init_ptrs(p_render_target, p_depth_stencil_buffer);
	std::mt19937 rng(std::random_device{}());
	wnd.get_graphics_c().enable_imgui();
	wnd.get_graphics_c().set_projection(DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 4000.0f));
	test_ball0.add_to_manager(mangr);
	sponza.add_to_manager(mangr);
	lantern.add_to_manager(mangr);
	bldr.add_to_manager(mangr);
}

int app::go()
{
	while (true)
	{

		// exit_code is an optional and it converts to true if it has a value, 
		// so if we get WM_QUIT the process_messages function assigns the exit code to the returned optional, 
		// which gets assined to exit_code and it converts to true 
		if (const auto exit_code = window::process_messages())
		{
			return *exit_code;
		}
		do_frame();
	}
	
}

void app::do_frame()
{
	p_render_target->clear(wnd.get_graphics_c(), {0.0f,0.0f,0.0f, 0.0f});
	wnd.get_graphics_c().clear_swapchain_render_target({ 0.0f,0.0f,0.0f, 1.0f });
	p_depth_stencil_buffer->clear(wnd.get_graphics_c());
	const auto dt = tokei.get_frame_time() * speed_factor;
	wnd.get_graphics_c().begin_frame(0.f, 0.0f, 0.0f);
	wnd.get_graphics_c().set_camera(cam.get_matrix());
	cam.show_imgui_control_window();
	cam.set(wnd.get_graphics_c());
	lantern.set(wnd.get_graphics_c());
	


	mangr.draw(wnd.get_graphics_c(), technique::normal);
	//mangr.draw(wnd.get_graphics_c(), technique::outline);

	while (const auto e = wnd.mouse_c.read_event())
	{
		if (e->get_type() == mouse::mouse_event::type::Rdown)
		{
			const auto pos = e->get_pos();
			ray r = pickr.get_origin_ray(pos.x, pos.y);
			DirectX::XMFLOAT3 pos0 = r.ray_origin;
			DirectX::XMFLOAT3 pos1(1000 * r.ray_dir.x + r.ray_origin.x, 1000 * r.ray_dir.y + r.ray_origin.y, 1000 * r.ray_dir.z + r.ray_origin.z);
			
			p_lines.push_back(std::make_unique<line>(technique_factory::create_line_desc(pos0.x, pos0.y, pos0.z, pos1.x, pos1.y, pos1.z), "line" + std::to_string(p_lines.size()), cdx, wnd.get_graphics_c(), 0.0f, 0.0f, 0.0f));
			p_lines.back()->add_to_manager(mangr);
			auto result = sponza.pick(pickr.get_origin_ray(pos.x, pos.y));
			if (result.first >= 0)
			{
				if (!result.second->has_tech(technique::outline))
				{
					result.second->add_technique(wnd.get_graphics_c(), cdx, technique::outline);
				}
			}
		}
	}

	if (wnd.kbd.is_key_pressed(VK_SHIFT))
		speed_factor = base_speed * 2;
	else
		speed_factor = base_speed;
	while (const auto e = wnd.kbd.read_key() )
	{
		if (!e->is_down())
			continue;
		switch (e->get_code())
		{
		case VK_ESCAPE:
			if (!wnd.cursor_disabled())
			{
				wnd.disable_cursor();
				wnd.mouse_c.enable_raw();
			}
			else 
			{
				wnd.enable_cursor();
				wnd.mouse_c.disable_raw();
			}
			break;
		}
	}
	if (wnd.cursor_disabled())
	{
		if (wnd.kbd.is_key_pressed('W'))
		{
			cam.translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.is_key_pressed('A'))
		{
			cam.translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.is_key_pressed('S'))
		{
			cam.translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.is_key_pressed('D'))
		{
			cam.translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.is_key_pressed('R'))
		{
			cam.translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.is_key_pressed('F'))
		{
			cam.translate({ 0.0f,-dt,0.0f });
		}
	}

	while (const auto delta = wnd.mouse_c.read_raw())
	{
		if (wnd.cursor_disabled())
		{
			cam.rotate((float)delta->x, (float)delta->y);
		}
	}
	if (ImGui::Begin("fps"))
	{
		ImGui::Text("fps: %.3f", ImGui::GetIO().Framerate);
	}
	ImGui::End();
	lantern.show_imgui_control_window(wnd.get_graphics_c());
	wnd.get_graphics_c().end_frame();
}
