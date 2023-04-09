#pragma once
#include <sstream>
#include "timer.h"
#include "box.h"
#include "imgui_manager.h"
#include "camera.h"
#include "sphere.h"
#include "line.h"
#include "point_light.h"
#include "mesh.h"
#include "codex.h"
#include "Window.h"
#include "technique_factory.h"
#include "stage_manager.h"


class app
{
public:
	app();
	int go();
private:
	void do_frame();
private:
	codex cdx;
	imgui_manager imgui;
	window wnd;
	model sponza;
	sphere test_ball0;
	point_light lantern;
	timer tokei;
	camera cam;
	stage_manager mangr; 
	picker pickr; 
	std::vector<std::unique_ptr<line>> p_lines; 
	std::shared_ptr<render_target> p_render_target;
	std::shared_ptr<depth_stencil_buffer> p_depth_stencil_buffer;
	std::unique_ptr<viewport> p_viewport; 
	fullscreen_blender bldr; 
	bool input_dis = false;
	float speed_factor = 3.0f;
	const float base_speed = 5.0f;
};