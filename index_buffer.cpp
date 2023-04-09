#include "index_buffer.h"

index_buffer::index_buffer(const std::vector<unsigned short>& indices, D3D11_BUFFER_DESC& bd, graphics& gfx)
	:
	n_indicies(UINT(indices.size()))
{
	HRESULT hr;
	D3D11_SUBRESOURCE_DATA sd_i = {};
	sd_i.pSysMem = indices.data();
	if ((hr = get_device(gfx)->CreateBuffer(&bd, &sd_i, &p_index_buffer)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	
}

index_buffer::index_buffer(const std::vector<unsigned short>& indices, graphics& gfx)
	:
	n_indicies(UINT(indices.size()))
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = UINT(n_indicies * sizeof(unsigned short));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA resource_data = {};
	resource_data.pSysMem = indices.data();
	if ((hr = get_device(gfx)->CreateBuffer(&bd, &resource_data, &p_index_buffer)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

UINT index_buffer::get_count() const
{
	return n_indicies;
}

void index_buffer::set(graphics& gfx)
{
	get_context(gfx)->IASetIndexBuffer(p_index_buffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}
