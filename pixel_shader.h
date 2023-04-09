#pragma once
#include "dx_component.h"


class pixel_shader : public dx_component
{
public:
	pixel_shader(const std::string& path, graphics& gfx);
	pixel_shader(ID3DBlob* plob, graphics& gfx);
	void set(graphics& gfx) override;
	static std::string get_id(const std::string& path);
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> p_pixel_shader;
};

