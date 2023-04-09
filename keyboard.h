#pragma once
#include <bitset>
#include <queue>
#include <optional>


class keyboard
{
	friend class window;
public:
	class kbd_event
	{
	public:
		enum class type{up, down, invalid};
		kbd_event(type t, unsigned char c)
			:
			c(c),
			t(t)
		{}
		kbd_event()
			:
			t(keyboard::kbd_event::type::invalid)
		{}
		char get_code() const
		{
			return c;
		}
		type get_type() const
		{
			return t;
		}
		bool is_up() const 
		{
			return t == keyboard::kbd_event::type::up;
		}
		bool is_down() const
		{
			return t == keyboard::kbd_event::type::down;
		}
		bool is_invalid() const
		{
			return t == keyboard::kbd_event::type::invalid;
		}
	private:
		type t;
		unsigned char c;
	};
public:
	keyboard() = default;
	keyboard(const keyboard& other) = delete;
	keyboard& operator=(const keyboard& other) = delete;
	bool is_key_pressed(unsigned char c) const;
	std::optional<keyboard::kbd_event> read_key();
	void clear_event_que();
	bool is_event_que_empty() const;
	//char
	unsigned char read_char();
	bool char_is_empty() const;
	void clear_char_que();
	//autorepeat
	void enable_autorepeat();
	void disable_autorepeat();
	bool is_autorepeat() const;
private:
	void set_event_down(const unsigned char code);
	void set_event_up(const unsigned char code);
	void set_char(const unsigned char code);
	template<typename T>
	void trim_buf(std::queue<T>& buf);
	void clear_states();
private:
	bool autorepeat = false;
	static constexpr unsigned int n_keys = 256u;
	static constexpr unsigned int buffer_size = 16u;
	std::bitset<n_keys> key_states;
	std::queue<kbd_event> events;
	std::queue<char> char_que;
};


