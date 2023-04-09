#pragma once
#include "better_windows.h"
#include "exeption.h"
#include <d3d11.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <sstream>
#include <d3dcompiler.h>
#include <cmath> 
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")



class dx_component; 

class graphics
{
	friend class picker;
	friend dx_component;
public:
	graphics(HWND hWnd, int width, int height);
	~graphics();
	graphics(const graphics& other) = delete;
	graphics& operator=(const graphics& other) = delete;
	float get_width() const;
	float get_height() const; 
	void enable_imgui();
	void disable_imgui();
	void end_frame();
	void begin_frame(float red, float green, float blue) noexcept;
	void draw_indexed(UINT index_count);
	void draw_unindexed();
	void draw_unindexed(unsigned int vertex_count);
	void set_camera(DirectX::FXMMATRIX cam);
	void draw_indexed(size_t n_indices);
	void set_swapchain_render_target();
	void set_swapchain_render_target(ID3D11DepthStencilView* p_ds_view);
	void clear_swapchain_render_target(DirectX::XMFLOAT4 color);
	DirectX::XMMATRIX get_camera() const;
	DirectX::XMMATRIX get_projection() const;
	void set_projection(DirectX::FXMMATRIX new_projection);
private:
	float width;
	float height;
	bool imgui_state = false;
	DirectX::XMMATRIX projection; 
	DirectX::XMMATRIX camera;
	Microsoft::WRL::ComPtr<ID3D11Device> p_device = nullptr;
	Microsoft::WRL::ComPtr<IDXGISwapChain> p_swap = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> p_context = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> p_rt_view = nullptr;
	D3D_FEATURE_LEVEL feature_level;
};