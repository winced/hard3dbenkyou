#include "depth_stencil_state.h"
/*
typedef enum D3D11_STENCIL_OP {
  D3D11_STENCIL_OP_KEEP = 1, //keep stencil data
  D3D11_STENCIL_OP_ZERO = 2, //put stencil data to zero 
  D3D11_STENCIL_OP_REPLACE = 3, //set the stencil data to refrence value by calling OMSetDepthStencilState 
  D3D11_STENCIL_OP_INCR_SAT = 4, //increment by one (clamps (stays att 255 if max))
  D3D11_STENCIL_OP_DECR_SAT = 5, //decreemtn by one (clamps)
  D3D11_STENCIL_OP_INVERT = 6, // inverts the stencil data 
  D3D11_STENCIL_OP_INCR = 7,  // increment (wraps)
  D3D11_STENCIL_OP_DECR = 8 // decrement (wraps)
} ;

// REFRENCE VALUE THAT IS USED IN THE COMPARISONS IS PASSED WHEN SETTING THE STENCIL STATE
typedef enum D3D11_COMPARISON_FUNC { // defines when the refrence value passes or fail the stencil test 
  D3D11_COMPARISON_NEVER = 1, // always fails 
  D3D11_COMPARISON_LESS = 2, // if refrence value is less than the buffer value 
  D3D11_COMPARISON_EQUAL = 3, // if refrence value is equal to the buffer value 
  D3D11_COMPARISON_LESS_EQUAL = 4,
  D3D11_COMPARISON_GREATER = 5,
  D3D11_COMPARISON_NOT_EQUAL = 6,
  D3D11_COMPARISON_GREATER_EQUAL = 7,
  D3D11_COMPARISON_ALWAYS = 8
*/
depth_stencil_state::depth_stencil_state(bool depth_enabled, stencil_mode s_mode, UINT stencil_ref_val, graphics& gfx)
	:
	stencil_ref_val(stencil_ref_val)
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC ds_desc = {};
	ds_desc.DepthEnable = depth_enabled;
	ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
	// a portion of the depth buffer that can be modified by the depth data (can be either 1 (can write) or 0 (cant write)) 
	ds_desc.DepthWriteMask = (D3D11_DEPTH_WRITE_MASK)depth_enabled;

	if (s_mode == stencil_mode::mask)
	{
		ds_desc.StencilEnable = true;
		ds_desc.StencilReadMask = 0xFF;
		ds_desc.StencilWriteMask = 0xFF;
		ds_desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		ds_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		ds_desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		ds_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	}
	else if (s_mode == stencil_mode::write)
	{
		ds_desc.StencilEnable = true;
		ds_desc.StencilReadMask = 0xFF;
		ds_desc.StencilWriteMask = 0xFF;
		ds_desc.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;
		ds_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_INCR_SAT;
		ds_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

		ds_desc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;
		ds_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		ds_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	}
	else
	{
		ds_desc.StencilEnable = false;
	}
	if ((hr = get_device(gfx)->CreateDepthStencilState(&ds_desc, &p_ds_state)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

depth_stencil_state::depth_stencil_state(const ds_state_params& params, graphics& gfx)
{
	*this = { params.depth_enabled, params.s_mode, params.ref_val, gfx };
}

std::string depth_stencil_state::get_id(bool depth_enabled, stencil_mode s_mode, UINT stencil_ref_val)
{
	return "depth_stencil buffer#: " + std::to_string(depth_enabled) + std::to_string((int)s_mode) + std::to_string(stencil_ref_val);
}

std::string depth_stencil_state::get_id(const ds_state_params& params)
{
	return get_id(params.depth_enabled, params.s_mode, params.ref_val);
}

void depth_stencil_state::set(graphics& gfx) noexcept
{
	get_context(gfx)->OMSetDepthStencilState(p_ds_state.Get(), stencil_ref_val); 
}










/*	//depth buffer schenanigans 
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depth_stencil_state;
	D3D11_DEPTH_STENCIL_DESC ds_desc = {};
	ds_desc.DepthEnable = TRUE;
	ds_desc.DepthFunc = D3D11_COMPARISON_LESS;
	ds_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	if ((hr = p_device->CreateDepthStencilState(&ds_desc, &depth_stencil_state)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	p_context->OMSetDepthStencilState(depth_stencil_state.Get(), 1u);
	//depth stencil texture
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_depth_stencil_texture;
	D3D11_TEXTURE2D_DESC texture_desc = {};
	texture_desc.Width = 1280;
	texture_desc.Height = 960;
	texture_desc.MipLevels = 1u;
	texture_desc.ArraySize = 1u;
	texture_desc.Format = DXGI_FORMAT_D32_FLOAT;
	texture_desc.SampleDesc.Count = 1u;
	texture_desc.SampleDesc.Quality = 0u;
	texture_desc.Usage = D3D11_USAGE_DEFAULT;
	texture_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	if ((hr = p_device->CreateTexture2D(&texture_desc, nullptr, &p_depth_stencil_texture)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsview_desc = {};
	dsview_desc.Format = DXGI_FORMAT_D32_FLOAT;
	dsview_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsview_desc.Texture2D.MipSlice = 0u;  

	if ((hr = p_device->CreateDepthStencilView(p_depth_stencil_texture.Get(), &dsview_desc, &p_dsview)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);*/