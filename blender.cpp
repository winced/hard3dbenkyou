#include "blender.h"


blender::blender(bool blend_enabled, graphics& gfx)
{
	D3D11_BLEND_DESC blend_desc = {};
	if (blend_enabled)
	{
		blend_desc.RenderTarget[0].BlendEnable = true;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		//add operation adds the source and the destionation 
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD; 
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		//these blend the alpha values together 
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		//not using a mask, for special effects. Like writing only to the green or blue channel 
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}
	else
	{
		blend_desc.RenderTarget->BlendEnable = false;
		blend_desc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	}


	get_device(gfx)->CreateBlendState(&blend_desc, &p_blender_state);
}

blender::blender(const blender_params& params, graphics& gfx)
{
	*this = { params.blend_enabled, gfx };
}

void blender::set(graphics& gfx) noexcept 
{
	get_context(gfx)->OMSetBlendState(p_blender_state.Get(), nullptr, 0xffffffff);
}

std::string blender::get_id(bool blend_enabled)
{
	return "blender#" + std::to_string(blend_enabled);
}

std::string blender::get_id(const blender_params& params)
{
	return get_id(params.blend_enabled);
}
