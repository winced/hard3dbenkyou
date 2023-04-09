#include "picker.h"

picker::picker(const camera& cam, const graphics& gfx)
	:
	cam(cam),
	gfx(gfx)
{
}

ray picker::get_origin_ray(int x, int y) const noexcept
{
	ray r;
	const unsigned int screen_width = gfx.get_width();
	const unsigned int screen_height = gfx.get_height(); 
	DirectX::XMVECTOR screen_near_v = DirectX::XMVectorSet(x, y, 0.0f, 1.0f);
	DirectX::XMVECTOR screen_far_v = DirectX::XMVectorSet(x, y, 1.0f, 1.0f);
	DirectX::XMStoreFloat3(&r.ray_origin, DirectX::XMVector3Unproject(screen_near_v, 0.0f, 0.0f, screen_width, screen_height, 0.0f, 1.0f, gfx.get_projection(), cam.get_matrix(), DirectX::XMMatrixIdentity()));
	DirectX::XMFLOAT3 far_v;
	DirectX::XMStoreFloat3(&far_v, DirectX::XMVector3Unproject(screen_far_v, 0.0f, 0.0f, screen_width, screen_height, 0.0f, 1.0f, gfx.get_projection(), cam.get_matrix(), DirectX::XMMatrixIdentity()));

	DirectX::XMStoreFloat3(&r.ray_dir, DirectX::XMVector3Normalize({ far_v.x - r.ray_origin.x, far_v.y - r.ray_origin.y, far_v.z - r.ray_origin.z }));
	return r;

}

