#pragma once
#include "graphics.h"
#include "constant_buffers.h"

struct camera_position_cbuf
{
	DirectX::XMFLOAT3 pos;
	float padding;
};
class camera
{
public:
	camera(graphics& gfx, const DirectX::XMFLOAT3& pos);
	DirectX::XMMATRIX get_matrix() const;
	void show_imgui_control_window();
	void reset();
	void set(graphics& gfx);
	void rotate(float dx, float dy) noexcept;
	void translate(DirectX::XMFLOAT3 translation) noexcept;
	DirectX::XMFLOAT3 get_pos() const noexcept; 
private:
private:
	std::unique_ptr<pixel_constant_buffer<camera_position_cbuf>> pixel_cbuf_p;
	camera_position_cbuf pos_cbuf;
	DirectX::XMFLOAT3 camera_pos;
	float yaw;
	float pitch;
	static constexpr float travel_speed = 12.0f;
	static constexpr float rotation_speed = 0.001f;
};