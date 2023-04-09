#include <unordered_map>
#include <DirectXCollision.h>
#include "mesh.h"
#include "basic_utility.h"
#include "transform_constant_buffer.h"
#include "stage_manager.h"

static drawable_description create_common_drawable_desc(graphics& gfx, const aiMesh& asset_mesh, const std::filesystem::path& model_path, const aiMaterial* const* p_materials, float scale)
{
	drawable_description d_desc;
	std::string root_path = model_path.parent_path().string();
	bool has_specular_map = false;
	bool has_texture_map = false;
	bool has_normal_map = false;
	d_desc.scale = scale;
	d_desc.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	d_desc.blend_params.first = false;
	d_desc.raster_params.first = false;
	d_desc.smplr_params.first = false;
	d_desc.ds_params.first = true;
	d_desc.ds_params.second = { .depth_enabled = true, .s_mode = stencil_mode::off, .ref_val = 1u };
	//common constant buffers
	//check for basic info 
	if (asset_mesh.HasNormals())
		d_desc.vertex_info[(size_t)vertex_layout::type::normal3f] = true;
	if (asset_mesh.HasFaces())
		d_desc.has_indices = true;
	if (asset_mesh.HasPositions())
		d_desc.vertex_info[(size_t)vertex_layout::type::position3f] = true;
	//check if materials
	if (asset_mesh.mMaterialIndex >= 0)
	{
		const auto& material = p_materials[asset_mesh.mMaterialIndex];
		aiString tex_path_ai;
		std::string tex_path;
		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &tex_path_ai) == AI_SUCCESS) // we have a diffuse texture in folder 
		{
			has_texture_map = true;
			tex_path = root_path + "\\textures\\" + get_name_from_path({ tex_path_ai.C_Str() });
			d_desc.tex_params.emplace_back(texture_params(tex_path, (UINT)PS_map_register::texture_m ));
			d_desc.ps_map_list[(size_t)PS_map_register::texture_m] = true;
		}
		if (material->GetTexture(aiTextureType_SPECULAR, 0, &tex_path_ai) == AI_SUCCESS)
		{
			has_specular_map = true;
			tex_path = root_path + "\\textures\\" + get_name_from_path({ tex_path_ai.C_Str() });
			d_desc.tex_params.emplace_back(texture_params(tex_path, (UINT)PS_map_register::specular_m));
			d_desc.ps_map_list[(size_t)PS_map_register::specular_m] = true;
		}
		else
			d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::specular] = true;
		if (material->GetTexture(aiTextureType_NORMALS, 0, &tex_path_ai) == AI_SUCCESS)
		{
			has_normal_map = true;
			tex_path = root_path + "\\textures\\" + get_name_from_path({ tex_path_ai.C_Str() });
			d_desc.tex_params.emplace_back(texture_params(tex_path, (UINT)PS_map_register::normal_m));
			d_desc.ps_map_list[(size_t)PS_map_register::normal_m] = true;
		}
	}
	if (has_specular_map || has_normal_map || has_texture_map)
	{
		d_desc.vertex_info[(size_t)vertex_layout::type::texture2f] = true;
		d_desc.smplr_params.first = true;
		d_desc.smplr_params.second.sampler_filter = D3D11_FILTER_ANISOTROPIC;
		if (has_normal_map)
		{
			d_desc.vertex_info[(size_t)vertex_layout::type::tangent3f] = true;
			d_desc.vertex_info[(size_t)vertex_layout::type::bitangent3f] = true;
		}
	}
	if (!has_texture_map)
		d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::obj_color] = true;
	return d_desc;
}

// for now all the default construction is in the mesh constructor 
// I can make different constructors in the futrue that take in the drawable description if I want 
mesh::mesh(graphics& gfx, codex& cdx, const aiMesh& asset_mesh, const std::filesystem::path& model_path, const aiMaterial* const* p_materials, float scale)
	:
	drawable(create_common_drawable_desc(gfx, asset_mesh, model_path, p_materials, scale), asset_mesh.mName.C_Str())
{
	auto& normal_comps = owned_techs[technique::normal].get_stage_comps(0u);
	auto& normal_ps = owned_techs[technique::normal].get_stage_ps(0u);
	std::vector<std::pair<PS_cbuf_register, dc_buf>> cbufs;
	std::vector<dc_buf> testc;
	vertex_layout v_layout;
	for (size_t i = 0; i < d_desc.vertex_info.size(); i++)
	{
		if (d_desc.vertex_info[i])
			v_layout.append(vertex_layout::type(i));
	}
	d_desc.vbuf = element_vertex_buffer(std::move(v_layout));
	for (int i = 0; i < asset_mesh.mNumVertices; i++)
	{
		if (d_desc.vertex_info[vertex_layout::type::position3f])
			d_desc.vbuf.emplace_back_single(0u, DirectX::XMFLOAT3{ asset_mesh.mVertices[i].x * d_desc.scale, asset_mesh.mVertices[i].y * d_desc.scale ,asset_mesh.mVertices[i].z * d_desc.scale });
		if (d_desc.vertex_info[vertex_layout::type::normal3f]) // we dont set normals if it only has a normal map 
			d_desc.vbuf.emplace_back_single(1u, *reinterpret_cast<DirectX::XMFLOAT3*>(&asset_mesh.mNormals[i]));
		if (d_desc.vertex_info[vertex_layout::type::texture2f])
			d_desc.vbuf.emplace_back_single(2u, *reinterpret_cast<DirectX::XMFLOAT2*>(&asset_mesh.mTextureCoords[0][i]));
		if (d_desc.vertex_info[vertex_layout::type::tangent3f])
			d_desc.vbuf.emplace_back_single(3u, *reinterpret_cast<DirectX::XMFLOAT3*>(&asset_mesh.mTangents[i]));
		if (d_desc.vertex_info[vertex_layout::type::bitangent3f])
			d_desc.vbuf.emplace_back_single(4u, *reinterpret_cast<DirectX::XMFLOAT3*>(&asset_mesh.mBitangents[i]));
	}
	d_desc.indices.reserve((size_t)asset_mesh.mNumFaces * 3u);
	for (int i = 0; i < asset_mesh.mNumFaces; i++)
	{
		for (int n = 0; n < asset_mesh.mFaces[i].mNumIndices; n++)
		{
			d_desc.indices.push_back(asset_mesh.mFaces[i].mIndices[n]);
		}
	}
	//add depth_stencil, rasterizer, sampler and blender if they are required 
	if(d_desc.ds_params.first)
		normal_comps.emplace_back(cdx.resolve<depth_stencil_state>(gfx, d_desc.ds_params.second));
	if(d_desc.raster_params.first)
		normal_comps.emplace_back(cdx.resolve<rasterizer>(gfx, d_desc.raster_params.second));
	if (d_desc.smplr_params.first) 
		normal_comps.emplace_back(cdx.resolve<sampler>(gfx, d_desc.smplr_params.second));
	if(d_desc.blend_params.first)
		normal_comps.emplace_back(cdx.resolve<blender>(gfx, d_desc.blend_params.second));
	for (auto& t_params : d_desc.tex_params)
	{
		normal_comps.emplace_back(cdx.resolve<texture>(gfx, t_params));
	}
	if (d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::obj_color])
	{
		normal_ps[PS_cbuf_register::obj_color].get_buffer().add<dc_buf::type::float4>("color")(DirectX::XMFLOAT4{ 1.0f, 1.0f, 1.0f, 1.0f });
		normal_comps.push_back(cdx.resolve_by_id<PS_dynamic_cbuf>("obj_color_cbuf" + name, gfx, normal_ps[PS_cbuf_register::obj_color], (size_t)PS_cbuf_register::obj_color));
	}
	if (d_desc.ps_cbuf_list[(size_t)PS_cbuf_register::specular])
	{
		const auto& material = p_materials[asset_mesh.mMaterialIndex];
		float specular_power;
		float specular_intensity = 0.8f;
		material->Get(AI_MATKEY_SHININESS, specular_power);

		normal_ps[PS_cbuf_register::specular].get_buffer().add<dc_buf::type::float1>("specular")(std::move(specular_intensity));
		normal_ps[PS_cbuf_register::specular].get_buffer().add<dc_buf::type::float1>("power")(std::move(specular_power));
		normal_comps.push_back(cdx.resolve_by_id<PS_dynamic_cbuf>("specular_cbuf" + name, gfx, normal_ps[PS_cbuf_register::specular], (size_t)PS_cbuf_register::specular));
	}
	std::shared_ptr<dx_component> vs;
	if (d_desc.vertex_info[(size_t)vertex_layout::type::normal3f] || d_desc.ps_map_list[(size_t)PS_map_register::normal_m]) // if we got normals, we are going to use phong shading
	{
		normal_comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_Phong" + get_name_ex_ps(d_desc.ps_map_list, d_desc.ps_cbuf_list, d_desc.vertex_info)));
		vs = cdx.resolve<vertex_shader>(gfx, "VS_Phong" + get_name_ex_vs(d_desc.vertex_info));
	}
	else
	{
		// solid ps dont take any vertex parameters so pass empty {}
		normal_comps.emplace_back(cdx.resolve<pixel_shader>(gfx, "PS_Solid" + get_name_ex_ps(d_desc.ps_map_list, d_desc.ps_cbuf_list, {})));
		vs = cdx.resolve<vertex_shader>(gfx, "VS_Solid" + get_name_ex_vs(d_desc.vertex_info));
	}
	add_input_layout(cdx.resolve<input_layout>(gfx, d_desc.vbuf.get_layout().get_D3D11_layout(), reinterpret_cast<vertex_shader*>(vs.get())->get_blobp()));
	normal_comps.emplace_back(std::move(vs));



	add_index_buffer(cdx.resolve_by_id<index_buffer>("index_buffer#" + name, gfx, d_desc.indices));
	add_vertex_buffer(cdx.resolve_by_id<vertex_buffer>("vertex_buffer#" + name, gfx, d_desc.vbuf));
	add_primitive_topology(cdx.resolve<primitive_topology>(gfx, d_desc.topology));
	add_transform_cbuf(cdx.resolve_by_id<transform_c_buf>("transform_c_buf#" + name, gfx, *this));

	
}
DirectX::XMMATRIX mesh::get_transform_m() const noexcept
{
	return DirectX::XMLoadFloat4x4(&m_transform);
}

std::pair<float, drawable*> mesh::pick(const ray& ray_in)
{
	float distance; 
	for (size_t i = 0; i < d_desc.indices.size(); i+=3)
	{
		DirectX::XMFLOAT3 vert0 = d_desc.vbuf[d_desc.indices[i]].element<vertex_layout::type::position3f>();
		DirectX::XMFLOAT3 vert1 = d_desc.vbuf[d_desc.indices[i+1]].element<vertex_layout::type::position3f>();
		DirectX::XMFLOAT3 vert2 = d_desc.vbuf[d_desc.indices[i+2]].element<vertex_layout::type::position3f>();
		if(DirectX::TriangleTests::Intersects(DirectX::XMLoadFloat3(&ray_in.ray_origin), DirectX::XMLoadFloat3(&ray_in.ray_dir), DirectX::XMLoadFloat3(&vert0), DirectX::XMLoadFloat3(&vert1), DirectX::XMLoadFloat3(&vert2), distance))
		{
			return std::pair<float, drawable*>(distance, this);
		}
	}
	return { -1, nullptr };
}
void mesh::add_to_manager(stage_manager& mangr, DirectX::FXMMATRIX built_transform)
{
	DirectX::XMStoreFloat4x4(&m_transform, built_transform);
	drawable::add_to_manager(mangr);
}
node::node(UINT id, const std::string& name, const DirectX::XMMATRIX& transformation, std::vector<mesh*> mesh_ptrs)
	:
	p_meshes(std::move(mesh_ptrs)),
	name(name),
	id(id)
{
	DirectX::XMStoreFloat4x4(&n_transform, transformation);
	DirectX::XMStoreFloat4x4(&applied_transform, DirectX::XMMatrixIdentity());
}
void node::set_applied_transform(DirectX::FXMMATRIX new_mat)
{
	DirectX::XMStoreFloat4x4(&applied_transform, new_mat);
}
void node::show_tree(node*& p_selected) const 
{
	// if there is no selected node, set selectedId to an impossible value
	const int selectedId = (p_selected == nullptr) ? -1 : p_selected->get_id();
	// build up flags for current node
	const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
		| ((get_id() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
		| ((p_cnodes.size() == 0) ? ImGuiTreeNodeFlags_Leaf : 0);
	// render this node
	const auto expanded = ImGui::TreeNodeEx((void*)(intptr_t)get_id(), node_flags, name.c_str());
	// processing for selecting node
	if (ImGui::IsItemClicked())
	{
		p_selected = const_cast<node*>(this);
	}
	// recursive rendering of open node's children
	if (expanded)
	{
		for (const auto& pChild : p_cnodes)
		{
			pChild->show_tree(p_selected);
		}
		ImGui::TreePop();
	}
}
UINT node::get_id() const
{
	return id;
}
const std::string& node::get_name() const
{
	return name;
}

std::pair<float, drawable*> node::pick(const ray& ray_in)
{
	std::pair<float, drawable*> result = {-1, nullptr};
	for (auto& m : p_meshes)
	{
		const auto m_res = m->pick(ray_in);
		if (m_res.first != -1 && (m_res.first < result.first || result.first == -1))
			result = m_res;
	}
	for (auto& n : p_cnodes)
	{
		const auto m_res = n->pick(ray_in);
		if (m_res.first != -1 && m_res.first < result.first || result.first == -1)
			result = m_res;
	}
	return result;
}
void node::set_to_origin()
{
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR rotation;
	DirectX::XMVECTOR translation;
	DirectX::XMMatrixDecompose(&scale, &rotation, &translation, DirectX::XMLoadFloat4x4(&n_transform));
	DirectX::XMStoreFloat4x4(&applied_transform, DirectX::XMLoadFloat4x4(&applied_transform) * DirectX::XMMatrixTranslationFromVector(DirectX::XMQuaternionInverse(translation)));
}
void node::add_to_manager(stage_manager& mangr, DirectX::FXMMATRIX built_transform) const
{
	const auto bm = DirectX::XMLoadFloat4x4(&applied_transform) * DirectX::XMLoadFloat4x4(&n_transform) * built_transform;

	for (const auto pm : p_meshes)
	{
		pm->add_to_manager(mangr, bm);
	}
	for (const auto& pc : p_cnodes)
	{
		pc->add_to_manager(mangr, bm);
	}
}
void node::add_child(std::unique_ptr<node> p_child)
{
	p_cnodes.push_back(std::move(p_child));
}
class model_control_window
{
public:
	void show_window(const char* window_name, const node& root)
	{
		window_name = window_name ? window_name : "Model";

		int nodeIndexTracker = 0;
		if (ImGui::Begin(window_name))
		{
			ImGui::Columns(2, nullptr, true);
			root.show_tree(p_selected_node);

			ImGui::NextColumn();
			if (p_selected_node != nullptr)
			{
				auto& transform = node_transform_map[p_selected_node->get_id()]; // creates one if one does not exist (with 0 values as default in the struct) 
				ImGui::Text("Orientation");
				ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
				ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
				ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);
				ImGui::Text("Position");
				ImGui::SliderFloat("X", &transform.x, -200.0f, 200.0f);
				ImGui::SliderFloat("Y", &transform.y, -200.0f, 200.0f);
				ImGui::SliderFloat("Z", &transform.z, -200.0f, 200.0f);
				if(ImGui::Button("put to origin"))
				{
					transform.x = 0.0f;
					transform.y = 0.0f;
					transform.z = 0.0f;
					p_selected_node->set_to_origin();
				}
			}
		}
		ImGui::End();
	}
	DirectX::XMMATRIX get_transform() const noexcept
	{
		const auto& transform = node_transform_map.at(p_selected_node->get_id());
		return DirectX::XMMatrixRotationRollPitchYaw(transform.roll, transform.pitch, transform.yaw) *DirectX::XMMatrixTranslation(transform.x, transform.y, transform.z);
	}
	node* get_selected_node() const
	{
		return p_selected_node;
	}
private:
	struct transformation_info
	{
		float roll = 0.0f;
		float pitch = 0.0f;
		float yaw = 0.0f;
		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;
	};
private:
	UINT selected_id;
	node* p_selected_node = nullptr;
	std::unordered_map<UINT, transformation_info> node_transform_map;
};
model::model(graphics& gfx, codex& cdx, const std::string& filename, float scale)
	:
	p_window(std::make_unique<model_control_window>()),
	model_path(filename)
{
	Assimp::Importer asset_import;
	const aiScene* p_scene = asset_import.ReadFile(filename,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals|
		aiProcess_CalcTangentSpace);
	for (int i = 0; i < p_scene->mNumTextures; i++)
	{
		texture_ptrs.push_back(p_scene->mTextures[i]);
	}
	for (int i = 0; i < p_scene->mNumMeshes; i++)
	{
		// we are passing in a const pointer to const materials, so we need to make the function take: const aiMaterial* const* ptr (the second const defines that its a const pointer)
		mesh_ptrs.emplace_back(parse_mesh(gfx, cdx, *p_scene->mMeshes[i], model_path, p_scene->mMaterials, scale));
	}
	UINT temp_id = 0;
	p_rnode = parse_node(temp_id, *p_scene->mRootNode);

}
model::~model()
{
}
void model::show_imgui_control_window()
{
	p_window->show_window("test window", *p_rnode);
}

std::pair<float, drawable*> model::pick(const ray& ray_in)
{
	return p_rnode->pick(ray_in);
}

std::unique_ptr<mesh> model::parse_mesh(graphics& gfx, codex& cdx, const aiMesh& asset_mesh, const std::filesystem::path& model_path, const aiMaterial* const* p_materials, float scale)
{
	return std::make_unique<mesh>(gfx, cdx, asset_mesh, model_path, p_materials, scale);
}
std::unique_ptr<node> model::parse_node(UINT& id, const aiNode& asset_node)
{

	auto transform = DirectX::XMMatrixTranspose(DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&asset_node.mTransformation)));
	std::vector<mesh*> p_nodemeshes; 
	p_nodemeshes.reserve(asset_node.mNumMeshes);
	for (int i = 0; i < asset_node.mNumMeshes; i++)
	{
		p_nodemeshes.push_back(mesh_ptrs[asset_node.mMeshes[i]].get());
	}
	std::unique_ptr<node> cur_node = std::make_unique<node>(id++, asset_node.mName.C_Str(), transform, std::move(p_nodemeshes));
	for (size_t i = 0; i < asset_node.mNumChildren; i++)
	{
		cur_node->add_child(parse_node(id, *asset_node.mChildren[i]));
	}
	return cur_node;

}
void model::add_to_manager(stage_manager& mangr)
{
	if (auto node = p_window->get_selected_node())
	{
		node->set_applied_transform(p_window->get_transform());
	}
	p_rnode->add_to_manager(mangr, DirectX::XMMatrixIdentity());
}
	