#pragma once
#include "dc_buf.h"
#include "dx_component.h"
#include <optional>



class dynamic_cbuf : public dx_component
{
public:
	dynamic_cbuf(cbuf_state& parent, UINT slot, graphics& gfx);
	void update(const dc_buf& dcbuf, graphics& gfx);
protected:
	tech_stage stg;
	cbuf_state& parent; 
	UINT slot;
	Microsoft::WRL::ComPtr<ID3D11Buffer> p_constant_buffer;
};
class PS_dynamic_cbuf : public dynamic_cbuf
{
public:
	PS_dynamic_cbuf(cbuf_state& parent, UINT slot, graphics& gfx);
	void set(graphics& gfx) override;
private:
};

class VS_dynamic_cbuf : public dynamic_cbuf
{
public:
	VS_dynamic_cbuf(cbuf_state& parent, UINT slot, graphics& gfx);
	void set(graphics& gfx) override;
private:
};


