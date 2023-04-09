#pragma once
#include "dx_component.h"


struct blender_params
{
	bool blend_enabled;
};

class blender : public dx_component
{
public:
	blender(bool blend_enabled, graphics& gfx);
	blender(const blender_params& params, graphics& gfx);
	void set(graphics& gfx) noexcept override;
	static std::string get_id(bool blend_enabled);
	static std::string get_id(const blender_params& params);
private:
	Microsoft::WRL::ComPtr<ID3D11BlendState> p_blender_state;
};