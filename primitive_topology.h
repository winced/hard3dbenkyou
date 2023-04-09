#pragma once
#include "dx_component.h"


class primitive_topology : public dx_component
{
public:
	primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology, graphics& gfx);
	void set(graphics& gfx) override;
	static std::string get_id(D3D11_PRIMITIVE_TOPOLOGY topology);
private:
	D3D11_PRIMITIVE_TOPOLOGY topology; 
};

