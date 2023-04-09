#pragma once
#include <vector>
#include <string>

#include "better_windows.h"
#include "graphics.h"
#include "technique.h"



class dx_component
{
public:
	virtual void set(graphics& gfx) = 0;
protected:
	ID3D11DeviceContext* get_context(graphics& gfx) const 
	{
		return gfx.p_context.Get();
	}
	ID3D11Device* get_device(graphics& gfx) const
	{
		return gfx.p_device.Get();
	}
	IDXGISwapChain* get_swap_chain(graphics& gfx) const
	{
		return gfx.p_swap.Get();
	}
};