#include "depth_stencil_buffer.h"


depth_stencil_buffer::depth_stencil_buffer(graphics& gfx, unsigned int width, unsigned int height, bool stencil_enabled)
	:
	width(width),
	height(height)
{
	HRESULT hr;
	ID3D11Texture2D* ds_texture = nullptr;
	D3D11_TEXTURE2D_DESC depth_stencil_tex_desc;
	depth_stencil_tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depth_stencil_tex_desc.ArraySize = 1;
	depth_stencil_tex_desc.MipLevels = 1;
	depth_stencil_tex_desc.Usage = D3D11_USAGE_DEFAULT;
	depth_stencil_tex_desc.SampleDesc.Count = 1;
	depth_stencil_tex_desc.SampleDesc.Quality = 0;
	depth_stencil_tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depth_stencil_tex_desc.MiscFlags = 0;
	depth_stencil_tex_desc.CPUAccessFlags = 0;
	depth_stencil_tex_desc.Width = gfx.get_width();
	depth_stencil_tex_desc.Height = gfx.get_height();
	if ((hr = get_device(gfx)->CreateTexture2D(&depth_stencil_tex_desc, nullptr, &ds_texture)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC ds_view_desc;
	ds_view_desc.Flags = 0u;
	ds_view_desc.Format = depth_stencil_tex_desc.Format;
	ds_view_desc.Texture2D.MipSlice = 0u;
	ds_view_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	if ((hr = get_device(gfx)->CreateDepthStencilView(ds_texture, &ds_view_desc, &p_ds_view)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	D3D11_SHADER_RESOURCE_VIEW_DESC resource_desc;
	resource_desc.Format = depth_stencil_tex_desc.Format;
	resource_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resource_desc.Texture2D.MipLevels = 1;
	resource_desc.Texture2D.MostDetailedMip = 0u;

	//if ((hr = get_device(gfx)->CreateShaderResourceView(ds_texture, &resource_desc, &p_resource_view)) != S_OK)
	//	throw windows_sdk_shippai(__LINE__, __FILE__, hr);

}
depth_stencil_buffer::depth_stencil_buffer(graphics& gfx, ds_buffer_params ds_params)
{
	*this = depth_stencil_buffer(gfx, ds_params.width, ds_params.height, ds_params.stencil_enabled);
}
void depth_stencil_buffer::set(graphics& gfx)
{
	get_context(gfx)->PSSetShaderResources((UINT)PS_map_register::depth_stencil_m, 1u, p_resource_view.GetAddressOf());
}
void depth_stencil_buffer::clear(graphics& gfx) const
{
	get_context(gfx)->ClearDepthStencilView(p_ds_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}
float depth_stencil_buffer::get_z_value( unsigned int x, unsigned int y, graphics& gfx) const
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D11Resource> p_res;
	p_ds_view->GetResource(&p_res);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_tex_res;
	p_res.As(&p_tex_res);
	D3D11_TEXTURE2D_DESC tex_desc;
	p_tex_res->GetDesc(&tex_desc);
	tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tex_desc.Usage = D3D11_USAGE_STAGING;
	tex_desc.BindFlags = 0;  // dont bind to pipeline 
	tex_desc.MipLevels = 1;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_stg_tex;
	if ((hr = get_device(gfx)->CreateTexture2D(&tex_desc, nullptr, &p_stg_tex)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	// copy from the actual texture to the staging texture (because the actual texture does not have read flags for cpu) 
	get_context(gfx)->CopyResource(p_stg_tex.Get(), p_tex_res.Get());
	D3D11_MAPPED_SUBRESOURCE sub_res = {};
	get_context(gfx)->Map(p_stg_tex.Get(), 0, D3D11_MAP_READ, 0, &sub_res);
	auto p_tex_src = reinterpret_cast<char*>(sub_res.pData);
	p_tex_src += (size_t)y * sub_res.RowPitch + x;
	unsigned int uint24_max = 0xffffff;

	unsigned int value = ((unsigned int)*(p_tex_src + 2) | ((unsigned int)*(p_tex_src + 1) << 8) | ((unsigned int)*(p_tex_src) << 16)) & uint24_max;
	float depth_val = float(value) / (float)uint24_max;
	get_context(gfx)->Unmap(p_stg_tex.Get(), 0);
	return depth_val;
}
ID3D11DepthStencilView* depth_stencil_buffer::get_ds_view()
{
	return p_ds_view.Get();
}
void depth_stencil_buffer::load_z_buffer(graphics& gfx) const
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D11Resource> p_res;
	p_ds_view->GetResource(&p_res);
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_tex_res;
	p_res.As(&p_tex_res);
	D3D11_TEXTURE2D_DESC tex_desc;
	p_tex_res->GetDesc(&tex_desc);
	tex_desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	tex_desc.Usage = D3D11_USAGE_STAGING;
	tex_desc.BindFlags = 0;  // dont bind to pipeline 
	tex_desc.MipLevels = 1;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> p_stg_tex;
	if ((hr = get_device(gfx)->CreateTexture2D(&tex_desc, nullptr, &p_stg_tex)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	// copy from the actual texture to the staging texture (because the actual texture does not have read flags for cpu) 
	get_context(gfx)->CopyResource(p_stg_tex.Get(), p_tex_res.Get());
	D3D11_MAPPED_SUBRESOURCE sub_res = {};
	get_context(gfx)->Map(p_stg_tex.Get(), 0, D3D11_MAP_READ, 0, &sub_res);
	auto p_tex_src = reinterpret_cast<char*>(sub_res.pData);
	unsigned int uint24_max = 0xffffff;
	for (int ty = 0; ty < height; ty++)
	{
		const auto src_row = (p_tex_src + ty * sub_res.RowPitch);
		for (int tx = 0; tx < width; tx++)
		{

			unsigned int value =
				((unsigned int)*(src_row + tx * sizeof(char) * 4 + 2) |
					((unsigned int)*(src_row + tx * sizeof(char) * 4 + 1) << 8) |
					((unsigned int)*(src_row + tx * sizeof(char) * 4) << 16)) & uint24_max;
			float fval = float(value) / (float)uint24_max;
		}
	}
	get_context(gfx)->Unmap(p_stg_tex.Get(), 0);
}


