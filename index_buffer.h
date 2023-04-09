#pragma once
#include "dx_component.h"


class index_buffer : public dx_component
{
public:
	index_buffer(const std::vector<unsigned short>& indices, D3D11_BUFFER_DESC& bd, graphics& gfx);
	index_buffer(const std::vector<unsigned short>& indices, graphics& gfx);
	UINT get_count() const;
	void set(graphics& gfx) override;
private:
	UINT n_indicies;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_index_buffer;
};