#pragma once
#include "dx_component.h"


class input_layout : public dx_component
{
public:
	input_layout(const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_descriptions, ID3DBlob* vertex_shader_source, graphics& gfx);
	static std::string get_id(const std::vector<D3D11_INPUT_ELEMENT_DESC>& input_descriptions, ID3DBlob*);
	void set(graphics& gfx) override;
private:
	Microsoft::WRL::ComPtr<ID3D11InputLayout> p_input_layout;
};

