#include "dynamic_cbuffer.h"
#include "drawable.h"


dynamic_cbuf::dynamic_cbuf(cbuf_state& parent, UINT slot, graphics& gfx)
	:
	parent(parent),
	slot(slot),
	stg(stg)
{
	HRESULT hr;

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = parent.get_buffer().get_size();
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = parent.get_buffer().get_data_ptr();
	if ((hr = get_device(gfx)->CreateBuffer(&cbd, &csd, &p_constant_buffer)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
}

void dynamic_cbuf::update(const dc_buf& dcbuf, graphics& gfx)
{
	HRESULT hr;
	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	if ((hr = get_context(gfx)->Map(p_constant_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_resource)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	memcpy(mapped_resource.pData, dcbuf.get_data_ptr(), dcbuf.get_size());
	get_context(gfx)->Unmap(p_constant_buffer.Get(), 0u);
}

PS_dynamic_cbuf::PS_dynamic_cbuf(cbuf_state& parent, UINT slot, graphics& gfx)
	:
	dynamic_cbuf(parent, slot, gfx)
{}

void PS_dynamic_cbuf::set(graphics& gfx)
{
	if (parent.get_update_state())
	{
		update(parent.get_buffer(), gfx);
		parent.disable_update();
	}
	get_context(gfx)->PSSetConstantBuffers(slot, 1u, p_constant_buffer.GetAddressOf());
}

VS_dynamic_cbuf::VS_dynamic_cbuf(cbuf_state& parent, UINT slot, graphics& gfx)
	:
	dynamic_cbuf(parent, slot, gfx)
{
}

void VS_dynamic_cbuf::set(graphics& gfx)
{
	if (parent.get_update_state())
	{
		update(parent.get_buffer(), gfx);
		parent.disable_update();
	}
	get_context(gfx)->VSSetConstantBuffers(slot, 1u, p_constant_buffer.GetAddressOf());
}
