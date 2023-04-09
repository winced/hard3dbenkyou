#pragma once
#include <vector>
#include <DirectXMath.h>
#include <map>
#include <string>
#include <unordered_map>
#include <memory>

#include "better_windows.h"



class dc_buf
{
	friend class element;
public:
	enum type
	{
		float1,
		float2,
		float3,
		float4,
		int1,
		bool1,
		struct_t,
		float4x4,
		float3x3
	};
	template<dc_buf::type>
	struct type_map
	{};
	template<>
	struct type_map<float1>
	{
		using sys_type = float;
		static constexpr int size = sizeof(float);
	};
	template<>
	struct type_map<float2>
	{
		using sys_type = DirectX::XMFLOAT2;
		static constexpr int size = sizeof(DirectX::XMFLOAT2);
	};
	template<>
	struct type_map<float3>
	{
		using sys_type = DirectX::XMFLOAT3;
		static constexpr int size = sizeof(DirectX::XMFLOAT3) + sizeof(float); // needs to have padding
	};
	template<>
	struct type_map<float4>
	{
		using sys_type = DirectX::XMFLOAT4;
		static constexpr int size = sizeof(DirectX::XMFLOAT4);
	};
	template<>
	struct type_map<int1>
	{
		using sys_type = int;
		static constexpr int size = sizeof(int);
	};
	template<>
	struct type_map<bool1>
	{
		using sys_type = bool;
		static constexpr int size = sizeof(int);
	};
	template<>
	struct type_map<struct_t>
	{
		using sys_type = struct { };
		static constexpr int size = 0;
	};
	template<>
	struct type_map<float3x3>
	{
		using sys_type = DirectX::XMFLOAT3X3;
		static constexpr int size = sizeof(DirectX::XMFLOAT3X3);
	};
	template<>
	struct type_map<float4x4>
	{
		using sys_type = DirectX::XMFLOAT4X4;
		static constexpr int size = sizeof(DirectX::XMFLOAT4X4);
	};
	static constexpr size_t type_size(type t)
	{
		switch (t)
		{
		case type::float1:
			return type_map<float1>::size;
			break;
		case type::float2:
			return type_map<float2>::size;
			break;
		case type::float3:
			return type_map<float3>::size;
			break;
		case type::int1:
			return type_map<int1>::size;
			break;
		case type::struct_t:
			return type_map<struct_t>::size;
			break;
		case type::bool1:
			return type_map<bool1>::size;
			break;
		case type::float3x3:
			return type_map<float3x3>::size;
			break;
		case type::float4x4:
			return type_map<float4x4>::size;
			break;
		}
	}
	class layout
	{
	public:
		template<typename dc_buf::type T>
		layout& add(const std::string& name)
		{
			type_map.insert({ name, T });
			size += dc_buf::type_map<T>::size;
			return *this;
		}
		dc_buf::type operator[](const std::string& name)
		{
			return type_map.at(name);
		}
		size_t get_size() const
		{
			return size;
		}
	private:
		std::map<std::string, dc_buf::type> type_map;
		size_t size = 0;
	};
	class element
	{
		friend dc_buf;
	public:
		element(dc_buf& parent_ref, size_t index, dc_buf::type t, const std::string& e_name)
			:
			p_ref(parent_ref),
			index(index),
			elem_type(t),
			e_name(e_name)
		{}
		size_t get_csize() const // get the size of the children in bytes
		{
			//this will return 0 if no elements have been added.
			size_t size = type_size(elem_type); // get the size of direct children
			for (const auto& e : edata_map)
			{
				size += e.second->get_csize(); // get the size of all children further  down 
			}
			return size;
		}
		size_t get_index() const
		{
			return index;
		}
		template<dc_buf::type et>
		dc_buf::element& add(const std::string& name)
		{
			if (edata_map.find(name) != edata_map.end())
				return *edata_map.at(name);
			const size_t s_size = get_csize();
			elay.add<et>(name);
			p_ref.shift(index + s_size, type_map<et>::size, e_name); //maybe works
			if (type_map<et>::size < p_ref.padding)
			{
				p_ref.shift_by(index + s_size, type_map<et>::size);
			}
			else
			{
				p_ref.data.insert(p_ref.data.begin() + index + s_size, type_map<et>::size, 0);
				if (p_ref.data.size() % 16 != 0)
				{
					auto rem = (16u - p_ref.data.size() % 16u);
					p_ref.padding += rem;
					p_ref.data.resize(p_ref.data.size() + rem);
				}
			}
			return *(edata_map[name] = std::make_unique<element>(p_ref, index + s_size, et, name));
		}
		template<typename T>
		void operator()(T&& val)
		{
			p_ref.set(index, std::forward<T>(val));
		}
		auto& operator[](const std::string& name)
		{
			return *edata_map.at(name);
		}
		template<type et>
		auto& get()
		{
			return *reinterpret_cast<type_map<et>::sys_type*>(p_ref.data.data() + index);
		}
		template<typename act_type>
		auto& get()
		{
			return *reinterpret_cast<act_type*>(p_ref.data.data() + index);
		}
	private:
		void shift_by(size_t offset)
		{
			index += offset;
			for (auto& e : edata_map)
			{
				e.second->shift_by(offset);
			}
		}
	private:
		const dc_buf::type elem_type;
		std::unordered_map<std::string, std::unique_ptr<element>> edata_map; // map to all the elements in the struct, index from the beginning of the struct 
		dc_buf& p_ref;
		layout elay;
		size_t index;
		const std::string e_name;
	};
public:
	dc_buf() = default;
	dc_buf(dc_buf& other) = delete;
	dc_buf operator=(dc_buf& other) = delete;
	dc_buf(dc_buf&& other) noexcept
		:
		padding(std::move(other.padding)),
		blay(std::move(other.blay)),
		data(std::move(other.data)),
		data_map(std::move(other.data_map))
	{}
	dc_buf& operator=(dc_buf&& other) noexcept
	{
		padding = std::move(other.padding);
		blay = std::move(other.blay);
		data = std::move(other.data);
		data_map = std::move(other.data_map);
		return *this;
	}
	size_t get_size() const
	{
		return data.size();
	}
	template<dc_buf::type et>
	dc_buf::element& add(const std::string& name)
	{
		if (data_map.find(name) != data_map.end())
			return *data_map.at(name);
		blay.add<et>(name);
		size_t type_size = type_map<et>::size;
		size_t elem_pos;
		if (type_size < padding)
		{
			//there is space, no need for resize
			elem_pos = data.size() - padding;
			padding -= type_size;
			
		}
		else
		{
			elem_pos = data.size();
			size_t new_size = data.size() + type_size;
			if (new_size % 16u != 0)
			{
				auto rem = 16u - (new_size % 16u);
				padding += rem;
				new_size += rem;
			}
			data.resize(new_size);
		}
		return *(data_map[name] = std::make_unique<element>(*this, elem_pos, et, name));
		
	}
	template<typename T>
	void set(const std::string& name, T&& val)
	{
		(*reinterpret_cast<T*>(data.data() + data_map[name]->get_index())) = val;
	}
	template<typename T>
	void set(size_t index, T&& val)
	{
		(*reinterpret_cast<T*>(data.data() + index)) = val;
	}
	auto& at(const std::string& name)
	{
		return *data_map.at(name);
	}
	auto& operator[](const std::string& name)
	{
		return *data_map.at(name);
	}
	char* get_data_ptr()
	{
		return data.data();
	}
	const char* get_data_ptr() const
	{
		return data.data();
	}
private:
	std::vector<char>& get_data()
	{
		return data;
	}
	void shift(size_t index, size_t offset, const std::string& caller)
	{
		for (auto i = data_map.begin(); i != data_map.end(); ++i)
		{
			if (i->first == caller)
				continue;
			if (i->second->get_index() >= index)
				i->second->index += offset;
		}
	}
	void shift_by(int index, size_t count)// excpects that it has the size to do this
	{
		for (int end_index = data.size() - padding - 1; end_index >= index; end_index--)
		{
			data[end_index + count] = data[end_index];
		}
		for (int i = index; i < index + count; i++)
		{
			data[i] = 0;
		}
		padding -= count;
	}
private:
	size_t padding = 0;
	static constexpr unsigned int alloc_size = 16u;
	layout blay;
	std::vector<char> data;
	std::unordered_map<std::string, std::unique_ptr<element>> data_map;
};

class cbuf_state
{
public:
	cbuf_state() = default;
	cbuf_state(const cbuf_state& other) = delete;
	cbuf_state& operator=(const cbuf_state& other) = delete;
	cbuf_state(cbuf_state&& other) noexcept
	{
		update_state = other.update_state;
		buffer = std::move(other.buffer);
	}
	cbuf_state& operator=(cbuf_state&& other) noexcept
	{
		update_state = other.update_state;
		buffer = std::move(other.buffer);
		return *this;
	}
	cbuf_state(dc_buf&& buffer)
		:
		buffer(std::move(buffer))
	{}
	cbuf_state(bool state, dc_buf&& buffer)
		:
		buffer(std::move(buffer)),
		update_state(state)
	{}
	void enable_update()
	{
		update_state = true;
	}
	void disable_update()
	{
		update_state = false; 
	}
	bool get_update_state() const
	{
		return update_state; 
	}
	dc_buf& get_buffer()
	{
		return buffer;
	}
private:
	bool update_state = false;
	dc_buf buffer;
};

