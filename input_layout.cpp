#include "input_layout.h"



//UNORM  defines the way it converts our data into the datatypes in the shaders. So it normalizes our integer values into the floating point numbers.  

input_layout::input_layout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_descriptions, ID3DBlob* vertex_shader_source, graphics& gfx)
{
	HRESULT hr;
	if ((hr = get_device(gfx)->CreateInputLayout(input_descriptions.data(), (UINT)(input_descriptions.size()), vertex_shader_source->GetBufferPointer(), vertex_shader_source->GetBufferSize(), &p_input_layout)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	
}
std::string input_layout::get_id(const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_descriptions, ID3DBlob*)
{
	std::string id; 
	for (const auto& desc : input_descriptions)
	{
		id.append(std::to_string(desc.Format));
		id.append(desc.SemanticName);
	}
	return id;
}
void input_layout::set(graphics& gfx)
{
	get_context(gfx)->IASetInputLayout(p_input_layout.Get());
}
