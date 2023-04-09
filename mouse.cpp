#include "mouse.h"

bool mouse::is_right_pressed() const
{
    return right_down;
}
bool mouse::is_left_pressed() const
{
    return left_down;
}
bool mouse::in_window() const
{
    return is_in_window;
}
void mouse::enable_raw() noexcept
{
    raw_enabled = true;
}
void mouse::disable_raw() noexcept
{
    raw_enabled = false;
}
bool mouse::get_raw_enabled() const noexcept
{
    return raw_enabled;
}
vec2<int> mouse::get_pos() const
{
    return mouse_pos;
}
void mouse::set_mouse_leave()
{
    is_in_window = false;
    event_que.push(mouse::mouse_event(mouse::mouse_event::type::leave, *this));
    trim_que();
}
void mouse::set_mouse_enter()
{   
    is_in_window = true;
    event_que.push(mouse::mouse_event(mouse::mouse_event::type::enter, *this));
    trim_que();
}
void mouse::set_mouse_move(int x, int y)
{
    mouse_pos = { x, y };
    event_que.push(mouse::mouse_event(mouse::mouse_event::type::move, *this));
    trim_que();
}
void mouse::set_raw_delta(int x, int y)
{
    raw_que.push({ x, y });
    trim_raw_que();
}
void mouse::trim_raw_que()
{
    while (raw_que.size() > max_que_len)
        raw_que.pop();
}
void mouse::trim_que()
{
    while (event_que.size() > max_que_len)
        event_que.pop();
}
void mouse::clear_que()
{
    while (!event_que.empty())
        event_que.pop();
}
std::optional<mouse::mouse_event> mouse::read_event()
{
    if (!event_que.empty())
    {
        mouse::mouse_event e = event_que.front();
        event_que.pop();
        return e;
    }
    else
        return std::nullopt;

}
std::optional<vec2<int>> mouse::read_raw()
{
    if (!raw_que.empty())
    {
        auto e = raw_que.front();
        raw_que.pop();
        return e;
    }
    else
        return std::nullopt;
}
std::optional<vec2<int>> mouse::get_raw_delta()
{
    if (raw_que.empty())
        return std::nullopt;
    const vec2<int> rd = raw_que.front();
    raw_que.pop();
    return rd;
}
void mouse::add_event_que(mouse::mouse_event::type t, vec2<int> pos)
{
    mouse_pos = pos;
    event_que.push(mouse::mouse_event(t, *this));
    trim_que();
}
void mouse::add_event_que(mouse::mouse_event::type t)
{
    event_que.push(mouse::mouse_event(t, *this));
    trim_que();
}
void mouse::set_left_pressed(bool pressed) 
{
    left_down = pressed;
    event_que.push(mouse_event(mouse::mouse_event::type::Ldown, *this));
    trim_que();
}
void mouse::set_right_pressed(bool pressed)
{
    right_down = pressed;
    event_que.push(mouse_event(mouse::mouse_event::type::Rdown, *this));
    trim_que();
}
void mouse::set_pos(vec2<int> npos)
{
    mouse_pos = npos;
}
