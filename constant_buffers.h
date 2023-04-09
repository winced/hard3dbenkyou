#pragma once
#include "dx_component.h"
#include "exeption.h"

template<typename C>
class constant_buffer : public dx_component
{
public:
	static std::string get_id(const C& consts, UINT slot)
	{
		return "constant_buffer#" + std::string(typeid(C).name());
	}
	void update(const C& consts, graphics& gfx)
	{
		HRESULT hr;
		D3D11_MAPPED_SUBRESOURCE mapped_resource;
		if (hr = get_context(gfx)->Map(p_constant_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &mapped_resource) != S_OK)
			throw windows_sdk_shippai(__LINE__, __FILE__, hr);
		memcpy(mapped_resource.pData, &consts, sizeof(consts));
		get_context(gfx)->Unmap(p_constant_buffer.Get(), 0u);
	}
	constant_buffer(const C& consts, UINT slot, graphics& gfx)
		:
		slot(slot)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(consts);
		cbd.StructureByteStride = 0u;

		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &consts;
		if (hr = get_device(gfx)->CreateBuffer(&cbd, &csd, &p_constant_buffer) != S_OK)
			throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	}
	constant_buffer(UINT slot, graphics& gfx)
		:
		slot(slot)
	{
		HRESULT hr;

		D3D11_BUFFER_DESC cbd;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
		if ((hr = get_device(gfx)->CreateBuffer(&cbd, nullptr, &p_constant_buffer)) != S_OK)
			throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	}
protected:
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_constant_buffer;
};

template<typename C>
class vertex_constant_buffer : public constant_buffer<C>
{
	using constant_buffer<C>::p_constant_buffer;
	using constant_buffer<C>::slot;
	using dx_component::get_context;
public:
	using constant_buffer<C>::constant_buffer;
	void set(graphics& gfx) noexcept override
	{
		get_context(gfx)->VSSetConstantBuffers(slot, 1u, p_constant_buffer.GetAddressOf());
	}
};

template<typename C>
class pixel_constant_buffer : public constant_buffer<C>
{
	using constant_buffer<C>::p_constant_buffer;
	using constant_buffer<C>::slot;
	using dx_component::get_context;
public:
	using constant_buffer<C>::constant_buffer;
	void set(graphics& gfx) noexcept override
	{
		get_context(gfx)->PSSetConstantBuffers(slot, 1u, p_constant_buffer.GetAddressOf());
	}
};