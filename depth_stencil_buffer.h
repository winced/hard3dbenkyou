#pragma once
#include "dx_component.h"

struct ds_buffer_params
{
	bool stencil_enabled = true;
	unsigned int width;
	unsigned int height; 
};

class depth_stencil_buffer : dx_component
{
public:
	depth_stencil_buffer(graphics& gfx, unsigned int width, unsigned int height, bool stencil_enabled = true);
	depth_stencil_buffer(graphics& gfx, ds_buffer_params ds_params);
	//sets the depth stencil as an input texture 
	void set(graphics& gfx) override; 
	void clear(graphics& gfx) const; 
	void load_z_buffer(graphics& gfx) const;
	float get_z_value(unsigned int x, unsigned int y, graphics& gfx) const;
	ID3D11DepthStencilView* get_ds_view();
private:
	unsigned int width;
	unsigned int height; 
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> p_ds_view = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> p_resource_view = nullptr;
};