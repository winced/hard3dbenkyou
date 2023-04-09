#include "primitive_topology.h"

primitive_topology::primitive_topology(D3D11_PRIMITIVE_TOPOLOGY topology, graphics& gfx)
	:
	topology(topology)
{
}

void primitive_topology::set(graphics& gfx)
{
	get_context(gfx)->IASetPrimitiveTopology(topology);
}

std::string primitive_topology::get_id(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	return ("primitive_topology#" + std::to_string(topology));
}
