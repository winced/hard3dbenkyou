#include "drawable.h"
#include "stage_manager.h"
#include "technique_factory.h"


drawable::drawable(const std::string& name)
    :
    name(name)
{
}

drawable::drawable(drawable_description d_desc, std::string name)
    :
    d_desc(std::move(d_desc)),
    name(name)
{
}

drawable::drawable(drawable_description d_desc, std::vector<std::shared_ptr<dx_component>> normal_comps)
    :
    d_desc(d_desc)
{
    owned_techs[technique::normal].get_stage_comps(1u) = std::move(normal_comps);
}

DirectX::XMMATRIX drawable::get_transform_m() const noexcept
{
    return DirectX::XMMATRIX();
}

void drawable::add_vertex_buffer(std::shared_ptr<dx_component> vertex_buf)
{
    p_vertex_buffer = std::move(vertex_buf);
}

void drawable::add_primitive_topology(std::shared_ptr<dx_component> prim_top)
{
    p_primitive_topology = std::move(prim_top);
}
void drawable::add_index_buffer(std::shared_ptr<dx_component> index_buf)
{
    p_index_buffer = std::move(index_buf);
}

void drawable::add_transform_cbuf(std::shared_ptr<dx_component> transform_cbuf)
{
    p_transform_cbuf = std::move(transform_cbuf);
}

void drawable::add_input_layout(std::shared_ptr<dx_component> input_lay)
{
    p_input_layout = std::move(input_lay);
}

void drawable::set_default(graphics& gfx)
{
    if(d_desc.has_indices)
        p_index_buffer->set(gfx);
    p_vertex_buffer->set(gfx);
    p_primitive_topology->set(gfx);
    p_transform_cbuf->set(gfx);
    p_input_layout->set(gfx);
}

void drawable::add_technique(graphics& gfx, codex& cdx, technique t)
{
    const auto stage_arr = get_tech_stages(t);
    const auto descs = technique_factory::create_stage_descs(d_desc, t);
    for (int i = 0; i < descs.size(); i++)
    {
        owned_techs[t].get_stage_comps(i) = technique_factory::create_stage_comps(gfx, cdx, *this, descs[i], owned_techs[t].get_stage_ps(i), owned_techs[t].get_stage_vs(i), std::to_string(i));
    }
}
size_t drawable::get_index_count() const
{
    return dynamic_cast<index_buffer*>(p_index_buffer.get())->get_count();
}

void drawable::add_to_manager(stage_manager& mangr)
{
    for (const auto& p : owned_techs)
    {
        mangr.add_drawable(this, p.first);
    }
}

bool drawable::has_tech(technique t) const noexcept
{
    return owned_techs.contains(t);
}

bool drawable::has_indices() const noexcept
{
    return d_desc.has_indices;
}

unsigned int drawable::get_vertex_count() const noexcept
{
    return d_desc.vbuf.size();
}
