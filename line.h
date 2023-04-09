#pragma once
#include "drawable.h"
#include "transform_constant_buffer.h"


class line : public drawable
{
public:
	line(drawable_description d_desc_in, std::string name, codex& cdx, graphics& gfx, float pos_x, float pos_y, float pos_z);
	DirectX::XMMATRIX get_transform_m() const noexcept override;
private:
	DirectX::XMFLOAT3 pos;
};