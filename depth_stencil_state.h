#pragma once
#include "dx_component.h"

enum class stencil_mode { mask, write, off };

struct ds_state_params
{
	bool depth_enabled = true;
	stencil_mode s_mode = stencil_mode::off;
	UINT ref_val = 1u;
};

class depth_stencil_state : public dx_component
{
public:
	depth_stencil_state(bool depth_enabled, stencil_mode s_mode, UINT stencil_ref_val, graphics& gfx);
	depth_stencil_state(const ds_state_params& params, graphics& gfx);
	static std::string get_id(bool depth_enabled, stencil_mode s_mode, UINT stencil_ref_val);
	static std::string get_id(const ds_state_params& params);
	void set(graphics& gfx) noexcept override;
private:
	UINT stencil_ref_val;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> p_ds_state; 
};