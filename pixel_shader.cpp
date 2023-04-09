#include "pixel_shader.h"
#include "basic_utility.h"
#include <d3dcompiler.h>

pixel_shader::pixel_shader(const std::string& path, graphics& gfx)
{
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3DBlob> p_blob;
	if ((hr = D3DReadFileToBlob(convert_multibyte_to_wide(path).c_str(), &p_blob)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	if ((hr = get_device(gfx)->CreatePixelShader(p_blob->GetBufferPointer(), p_blob->GetBufferSize(), nullptr, &p_pixel_shader)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	
}

pixel_shader::pixel_shader(ID3DBlob* plob, graphics& gfx)
{
	HRESULT hr;
	if ((hr = get_device(gfx)->CreatePixelShader(plob->GetBufferPointer(), plob->GetBufferSize(), nullptr, &p_pixel_shader)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

void pixel_shader::set(graphics& gfx)
{
	get_context(gfx)->PSSetShader(p_pixel_shader.Get(), nullptr, 0u);
}

std::string pixel_shader::get_id(const std::string& path)
{
	return "pixel_shader#" + path;
}
