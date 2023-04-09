#pragma once
#include "drawable.h"
#include <random>

class box : public drawable
{
public:
	box(float width, const std::string& id, codex& cdx, graphics& gfx);
	DirectX::XMMATRIX get_transform_m() const noexcept override;
	void update(float dt);
private:
	// positional
	float r;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float theta;
	float phi;
	float chi;
	// speed (delta/s)
	float droll;
	float dpitch;
	float dyaw;
	float dtheta;
	float dphi;
	float dchi;
};