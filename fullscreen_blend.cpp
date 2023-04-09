#include "fullscreen_blend.h"
#include "stage_manager.h"

fullscreen_blender::fullscreen_blender(graphics& gfx, codex& cdx, const fullscreen_blender_description& desc, technique t)
	:
	blend_desc(desc),
	tech(t)
{
	element_vertex_buffer vbuf{ vertex_layout{}.append(vertex_layout::type::position3f) };
	vbuf.emplace_back(DirectX::XMFLOAT3{ -1.0f,  1.0f, 0.0f });
	vbuf.emplace_back(DirectX::XMFLOAT3{ 1.0f,  1.0f, 0.0f });
	vbuf.emplace_back(DirectX::XMFLOAT3{ -1.0f, -1.0f, 0.0f });
	vbuf.emplace_back(DirectX::XMFLOAT3{ 1.0f, -1.0f, 0.0f });
	std::vector<unsigned short> indices = { 2, 0, 1, 1, 3, 2 };
	comp_ptrs.emplace_back(cdx.resolve<vertex_shader>(gfx, blend_desc.VS));
	comp_ptrs.emplace_back(cdx.resolve<input_layout>(gfx, vbuf.get_layout().get_D3D11_layout(), reinterpret_cast<vertex_shader*>(comp_ptrs.back().get())->get_blobp()));
	comp_ptrs.emplace_back(cdx.resolve<pixel_shader>(gfx, blend_desc.PS));
	comp_ptrs.emplace_back(cdx.resolve_by_id<vertex_buffer>("FullBlend_vertex_buffer#", gfx, vbuf));
	comp_ptrs.emplace_back(cdx.resolve_by_id<index_buffer>("FullBlend_vertex_buffer#", gfx, indices));
	comp_ptrs.emplace_back(cdx.resolve<primitive_topology>(gfx, blend_desc.topology));
	comp_ptrs.emplace_back(cdx.resolve<blender>(gfx, blend_desc.blend_params.second));
	comp_ptrs.emplace_back(cdx.resolve<sampler>(gfx, blend_desc.smplr_params.second));
	comp_ptrs.emplace_back(cdx.resolve<depth_stencil_state>(gfx, blend_desc.ds_params.second));
}

size_t fullscreen_blender::get_index_count() const
{
	return 6u;
}

void fullscreen_blender::add_to_manager(stage_manager& mangr)
{
	mangr.add_fs_blender(this, tech);
}

void fullscreen_blender::set(graphics& gfx) const noexcept
{
	for (const auto c : comp_ptrs)
	{
		c->set(gfx);
	}
}
