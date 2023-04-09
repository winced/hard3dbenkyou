	#pragma once
#include <vector>
#include <memory>
#include <filesystem>
#include <string>
#include <assimp/Importer.hpp>      
#include <assimp/scene.h>           
#include <assimp/postprocess.h>

#include "drawable.h"
#include "surface.h"




class mesh : public drawable
{
public:
	mesh(graphics& gfx, codex& cdx, const aiMesh& asset_mesh, const std::filesystem::path& model_path, const aiMaterial* const* p_materials, float scale);
	DirectX::XMMATRIX get_transform_m() const noexcept override;
	std::pair<float, drawable*> pick(const ray& ray_in);
	void add_to_manager(class stage_manager& mangr, DirectX::FXMMATRIX built_transform);
private:
	mutable DirectX::XMFLOAT4X4 m_transform;
};

class node
{
	friend class model;
public:
	node(UINT id, const std::string& name, const DirectX::XMMATRIX& transformation, std::vector<mesh*> meshPtrs);
	void set_applied_transform(DirectX::FXMMATRIX new_mat);
	void show_tree(node*& p_selected) const;
	UINT get_id() const;
	const std::string& get_name() const;
	void set_to_origin();
	std::pair<float, drawable*> pick(const ray& ray_in);
	void add_to_manager(class stage_manager& mangr, DirectX::FXMMATRIX built_transform) const;
private:
	void add_child(std::unique_ptr<node> p_child);
private:
	UINT id;
	std::string name; 
	std::vector<mesh*> p_meshes; // model owns the meshes with unique_ptr, duplicates in the nodes cant own with unique_ptr 
	std::vector<std::unique_ptr<node>> p_cnodes;
	DirectX::XMFLOAT4X4 n_transform; // offset from the root node
	DirectX::XMFLOAT4X4 applied_transform; // transform applied to this node (for example a tire rotating in a car, where the tire is a node and the car is the model) 
};

class model
{
public:
	model(graphics& gfx, codex& cdx, const std::string& filename, float scale);
	~model(); // have to declare and define in cpp file to make the unique ptr work with an incomplete type defined in the cpp file 
	void add_to_manager(class stage_manager& mangr);
	std::pair<float, drawable*> pick(const ray& ray_in);
	void show_imgui_control_window();
private:
	static std::unique_ptr<mesh> parse_mesh(graphics& gfx, codex& cdx, const aiMesh& asset_mesh, const std::filesystem::path& model_path, const aiMaterial* const* p_materials, float scale = 1.0f);
	std::unique_ptr<node> parse_node(UINT& id, const aiNode& asset_node);
private:
	std::string model_path;
	std::unique_ptr<node> p_rnode;	
	std::vector<std::unique_ptr<mesh>> mesh_ptrs;
	std::vector<const aiTexture*> texture_ptrs;
	std::unique_ptr<class model_control_window> p_window;
};