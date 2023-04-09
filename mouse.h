#pragma once
#include "Vec2.h"
#include <vector>
#include <queue>
#include <optional>


//incomplete
// TODO: 
// no mousewheel or middle click 
// mousewheel also should support finer detail scrolling 
// doesn't support exiting the client region
// watch chili tutorial
class mouse
{
	friend class window;
public:
	class mouse_event
	{
	public:
		enum class type { Ldown, Lup, Rdown, Rup, move, enter, leave, invalid };
	public:
		mouse_event(mouse::mouse_event::type t, const mouse& parent)
			:
			pos(parent.mouse_pos),
			left_down(parent.left_down),
			right_down(parent.right_down),
			t(t)
		{}
		type get_type() const noexcept
		{
			return t;
		}
		vec2<int> get_pos() const noexcept
		{
			return pos;
		}
	private:
		bool left_down;
		bool right_down;
		type t;
		vec2<int> pos;
	};
public:
	bool is_right_pressed() const;
	bool is_left_pressed() const;
	bool in_window() const;
	void enable_raw() noexcept;
	void disable_raw() noexcept;
	bool get_raw_enabled() const noexcept;
	vec2<int> get_pos() const;
	std::optional<mouse::mouse_event> read_event();
	std::optional<vec2<int>> read_raw();
	std::optional<vec2<int>> get_raw_delta();
private:
	void add_event_que(mouse::mouse_event::type t, vec2<int> pos);
	void add_event_que(mouse::mouse_event::type t);
	void set_left_pressed(bool pressed);
	void set_right_pressed(bool pressed);
	void set_pos(vec2<int> npos);
	void set_mouse_leave();
	void set_mouse_enter();
	void set_mouse_move(int x, int y);
	void set_raw_delta(int x, int y); 
	void trim_raw_que();
	void trim_que();
	void clear_que();
private: 
	bool is_in_window = false;
	bool left_down = false;
	bool right_down = false;
	bool raw_enabled = false;
	unsigned int max_que_len = 16u;
	vec2<int> mouse_pos;
	std::queue<mouse_event> event_que;
	std::queue<vec2<int>> raw_que;
};