#include "line.h"
#include "vertex.h"



line::line(drawable_description d_desc_in, std::string name, codex& cdx, graphics& gfx, float pos_x, float pos_y, float pos_z)
	:
	drawable(std::move(d_desc_in), name),
	pos(pos_x, pos_y, pos_z)
{
	auto& normal_comps = owned_techs[technique::normal].get_stage_comps(0u);
	auto& normal_ps = owned_techs[technique::normal].get_stage_ps(0u);
	dc_buf obj_col_cbuf;
	obj_col_cbuf.add<dc_buf::type::float4>("color")(DirectX::XMFLOAT4{ 1.0f, 0.0f, 0.0f, 1.0f });
	normal_ps[PS_cbuf_register::obj_color] = (cbuf_state{ std::move(obj_col_cbuf) });
	normal_comps.emplace_back(cdx.resolve_by_id<PS_dynamic_cbuf>("obj_color_cbuf#" + name, gfx, normal_ps[PS_cbuf_register::obj_color], (size_t)PS_cbuf_register::obj_color));
	normal_comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_Solid" + get_name_ex_ps(d_desc.ps_map_list, d_desc.ps_cbuf_list, {})));
	auto vs = cdx.resolve<vertex_shader>(gfx, "VS_Solid" + get_name_ex_vs(d_desc.vertex_info));

	add_input_layout(cdx.resolve<input_layout>(gfx, d_desc.vbuf.get_layout().get_D3D11_layout(), reinterpret_cast<vertex_shader*>(vs.get())->get_blobp()));
	normal_comps.emplace_back(std::move(vs));

	if(d_desc.ds_params.first)
		normal_comps.emplace_back(cdx.resolve<depth_stencil_state>(gfx, d_desc.ds_params.second));
	add_vertex_buffer(cdx.resolve_by_id<vertex_buffer>("vertex_buffer#" + name, gfx, d_desc.vbuf));
	add_primitive_topology(cdx.resolve<primitive_topology>(gfx, d_desc.topology));
	add_transform_cbuf(cdx.resolve_by_id<transform_c_buf>("transform_c_buf#" + name, gfx, *this));
}

DirectX::XMMATRIX line::get_transform_m() const noexcept
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
}
