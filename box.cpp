#include "box.h"


box::box(float width, const std::string& id, codex& cdx, graphics& gfx)
{

}

DirectX::XMMATRIX box::get_transform_m() const noexcept
{
	return DirectX::XMMatrixIdentity();
}

void box::update(float dt) 
{
}
