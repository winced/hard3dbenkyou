#pragma once
#include <memory>
#include <optional>
#include <DirectXMath.h>
#include "basic_utility.h"
#include "basic_geometry.h"

#include "codex.h"
#include "dx_component_base.h"
#include "picker.h"



// struct that will define the supported capabilities and the default options of a drawable 
struct drawable_description
{
	bool has_indices = true;
	float scale = 1.0f;
	std::array<bool, vertex_layout::type::n_vertex_types> vertex_info = { false };
	std::array<bool, (size_t)PS_cbuf_register::n_PS_cbuf> ps_cbuf_list = { false };
	std::array<bool, (size_t)VS_cbuf_register::n_VS_cbuf> vs_cbuf_list = { false };
	std::array<bool, (size_t)PS_map_register::n_PS_maps> ps_map_list = { false };
	//vertex info 
	element_vertex_buffer vbuf;
	std::vector<unsigned short> indices; 
	//depth_stencil_params depths_params; 
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	std::pair<bool, rasterizer_params> raster_params = { false, {.disable_backface_culling = false} };
	std::pair<bool, sampler_params> smplr_params = { false, {.sampler_filter = D3D11_FILTER_ANISOTROPIC } };
	std::pair<bool, blender_params> blend_params = { false, {.blend_enabled = false } };
	std::pair<bool, ds_state_params> ds_params = { false, {.depth_enabled = true, .s_mode = stencil_mode::off, .ref_val = 1u} };
	std::vector<texture_params> tex_params;
};

// each drawable is going to have its own state, which describes how its going to fit into technique tempaltes
// the state describes which maps it uses, and what shaders it can support 
// so with that information you can create the technique from a template 
class drawable
{
public:
	class waza
	{
	public:
		waza() = default;
		std::vector<std::shared_ptr<dx_component>>& get_stage_comps(size_t index)
		{
			if (stage_comps.size() < index+1)
				stage_comps.resize(index+1);
			return stage_comps[index];
		}
		std::unordered_map<PS_cbuf_register, cbuf_state>& get_stage_ps(size_t index)
		{
			if (PS_cbufs.size() < index+1)
				PS_cbufs.resize(index+1);
			return PS_cbufs[index];
		}
		std::unordered_map<VS_cbuf_register, cbuf_state>& get_stage_vs(size_t index)
		{
			if (VS_cbufs.size() < index+1)
				VS_cbufs.resize(index+1);
			return VS_cbufs[index];
		}
		void set_comps(graphics& gfx, size_t index)
		{
			for (const auto& c : stage_comps[index])
			{
				c->set(gfx);
			}
		}
		size_t get_n_stages()
		{
			return stage_comps.size();
		}
	private:
		std::vector<std::vector<std::shared_ptr<dx_component>>> stage_comps; // also add the fullscreen blend shit to these comps?! YES 
		std::vector<std::unordered_map<PS_cbuf_register, cbuf_state>> PS_cbufs; // these should be in the order that they appear in the register 
		std::vector<std::unordered_map<VS_cbuf_register, cbuf_state>> VS_cbufs;
		//maybe a vector to display what cbufs we have? 
	};
	drawable() = default;
	drawable(const std::string& name);
	drawable(drawable_description d_desc, std::string name);
	drawable(drawable_description d_desc, std::vector<std::shared_ptr<dx_component>> normal_comps);
	drawable(const drawable&) = delete;
	virtual ~drawable() = default;
	virtual DirectX::XMMATRIX get_transform_m() const noexcept;
	void add_vertex_buffer(std::shared_ptr<dx_component> vertex_buf);
	void add_primitive_topology(std::shared_ptr<dx_component> prim_top);
	void add_index_buffer(std::shared_ptr<dx_component> index_buf);
	void add_transform_cbuf(std::shared_ptr<dx_component> transform_cbuf);
	void add_input_layout(std::shared_ptr<dx_component> input_lay);
	void set_default(graphics& gfx);
	void add_technique(graphics& gfx, codex& cdx, technique t);
	void set_stage_comps(graphics& gfx, technique tech, size_t index)
	{
		owned_techs.at(tech).set_comps(gfx, index);
	}
	size_t get_n_stages(technique t)
	{
		return owned_techs[t].get_n_stages();
	}
	const std::string& get_name() const
	{
		return name;
	}
	size_t get_index_count() const;
	void add_to_manager(class stage_manager& mangr);
	bool has_tech(technique t) const noexcept;
	bool has_indices() const noexcept;
	unsigned int get_vertex_count() const noexcept; 
protected:
	std::string name; 
	drawable_description d_desc;
	std::shared_ptr<dx_component> p_index_buffer;
	std::shared_ptr<dx_component> p_vertex_buffer;
	std::shared_ptr<dx_component> p_primitive_topology;
	std::shared_ptr<dx_component> p_transform_cbuf; 
	std::shared_ptr<dx_component> p_input_layout;
	std::unordered_map<technique, waza> owned_techs;
};