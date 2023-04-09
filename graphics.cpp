#include "graphics.h"
#include <vector>


graphics::graphics(HWND hWnd, int width, int height)
	:
	width(width),
	height(height)
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0; 
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// For debuging messages use D3D11_CREATE_DEVICE_DEBUG as a flag for creatign the device and swapchain. 
	// comptr overrides the & operator to release the interface it is pointing to at the moment and return the address of the underlying pointer.
	// so to get just the address without releasing call comptr.get_addressof();
	if((hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, &sd, &p_swap, &p_device, &feature_level, &p_context)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	Microsoft::WRL::ComPtr<ID3D11Resource> p_backbuffer = nullptr;
	if((hr = p_swap->GetBuffer(0, __uuidof(ID3D11Resource), &p_backbuffer)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);

	if((hr = p_device->CreateRenderTargetView(p_backbuffer.Get(), nullptr, &p_rt_view)) != S_OK)
		throw windows_sdk_shippai(__LINE__, __FILE__, hr);     

	D3D11_VIEWPORT viewport_desc;
	viewport_desc.TopLeftX = 0;
	viewport_desc.TopLeftY = 0;
	viewport_desc.Width = width;
	viewport_desc.Height = height;
	viewport_desc.MaxDepth = 1.0f;
	viewport_desc.MinDepth = 0.0f;
	p_context->RSSetViewports(1u, &viewport_desc);

	ImGui_ImplDX11_Init(p_device.Get(), p_context.Get());
}

graphics::~graphics()
{
	ImGui_ImplDX11_Shutdown();
}

float graphics::get_width() const
{
	return width;
}

float graphics::get_height() const
{
	return height;
}

void graphics::enable_imgui()
{
	imgui_state = true;
}

void graphics::disable_imgui()
{
	imgui_state = false;
}

//flips the back and front buffers 
void graphics::end_frame()
{
	if (imgui_state)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()); 
	}
	HRESULT hr;
	hr = p_swap->Present(1u, 0u);
	if (hr != S_OK && hr != 0X087A0001)
	{
		if (hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			throw windows_sdk_shippai(__LINE__, __FILE__, p_device->GetDeviceRemovedReason());
		}
		else
			throw windows_sdk_shippai(__LINE__, __FILE__, hr);
	}
}
void graphics::begin_frame(float red, float green, float blue) noexcept
{
	if (imgui_state)
	{
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}
	const float color[] = { red, green, blue, 1.0f };
	//p_context->ClearRenderTargetView(p_rtv_view.Get(), color);
	//p_context->ClearDepthStencilView(p_ds_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}
void graphics::draw_indexed(UINT index_count)
{
	p_context->DrawIndexed(index_count, 0u, 0u);
}
void graphics::draw_unindexed()
{
	p_context->DrawAuto();
}
void graphics::draw_unindexed(unsigned int vertex_count)
{
	p_context->Draw(vertex_count, 0u);
}
void graphics::set_camera(DirectX::FXMMATRIX cam)
{
	camera = cam;
}
void graphics::draw_indexed(size_t n_indices)
{
	p_context->DrawIndexed(n_indices, 0, 0);
}
void graphics::set_swapchain_render_target()
{
	p_context->OMSetRenderTargets(1u, p_rt_view.GetAddressOf(), nullptr);
}
void graphics::set_swapchain_render_target(ID3D11DepthStencilView* p_ds_view)
{
	p_context->OMSetRenderTargets(1u, p_rt_view.GetAddressOf(), p_ds_view);
}
void graphics::clear_swapchain_render_target(DirectX::XMFLOAT4 color)
{
	float col_arr[4] = { color.x, color.y, color.z, color.w };
	p_context->ClearRenderTargetView(p_rt_view.Get(), col_arr);
}
DirectX::XMMATRIX graphics::get_camera() const
{
	return camera;
}
DirectX::XMMATRIX graphics::get_projection() const
{
	return projection;
}

void graphics::set_projection(DirectX::FXMMATRIX new_projection)
{
	projection = new_projection;
}


