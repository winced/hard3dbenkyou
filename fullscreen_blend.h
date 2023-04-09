#pragma once
#include "dx_component_base.h"
#include "drawable.h"

struct fullscreen_blender_description
{
	std::array<bool, (size_t)PS_map_register::n_PS_maps> ps_map_list = { false };
	//depth_stencil_params depths_params; 
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::pair<bool, sampler_params> smplr_params = { false, {.sampler_filter = D3D11_FILTER_ANISOTROPIC } };
	std::pair<bool, blender_params> blend_params = { false, {.blend_enabled = false } };
	std::pair<bool, ds_state_params> ds_params = { false, {.depth_enabled = true, .s_mode = stencil_mode::off, .ref_val = 1u} };
	std::string PS;
	std::string VS;
	std::vector<texture_params> tex_params;
};

class fullscreen_blender 
{
public:
	fullscreen_blender() = default;
	fullscreen_blender(graphics& gfx, codex& cdx, const fullscreen_blender_description& desc, technique t);
	size_t get_index_count() const;
	void set(graphics& gfx) const noexcept;
	void add_to_manager(class stage_manager& mangr);
private:
	technique tech; 
	fullscreen_blender_description blend_desc;
	std::vector<std::shared_ptr<dx_component>> comp_ptrs; 
};