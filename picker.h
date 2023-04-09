#pragma once
#include "graphics.h"
#include "camera.h"

struct ray
{
	DirectX::XMFLOAT3 ray_origin;
	DirectX::XMFLOAT3 ray_dir;
};

class picker
{
public:
	picker(const camera& cam, const graphics& gfx);
	ray get_origin_ray(int x, int y) const noexcept;
private:
	const camera& cam;
	const graphics& gfx;
};