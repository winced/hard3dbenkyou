#pragma once
#include <unordered_map>
#include <string>
#include <utility>
#include <tuple>
#include "dx_component.h"

class codex
{
public:
	template<typename component_type, typename... args>
	std::shared_ptr<dx_component> resolve(graphics& gfx, args&&... arguments)
	{
		std::string id = component_type::get_id(std::forward<args>(arguments)...);
		if (component_pool.find(id) == component_pool.end())
			component_pool[id] = std::make_shared<component_type>(component_type(std::forward<args>(arguments)..., gfx));
		return component_pool.at(id);
	}
	template<typename component_type, typename... args>
	std::shared_ptr<dx_component> resolve_by_id(const std::string& id, graphics& gfx, args&&... arguments)
	{
		if (component_pool.find(id) == component_pool.end())
			component_pool[id] = std::make_shared<component_type>(component_type(std::forward<args>(arguments)..., gfx));
		return component_pool.at(id);
	}
private:
	static std::unordered_map<std::string, std::shared_ptr<dx_component>> component_pool;
};

