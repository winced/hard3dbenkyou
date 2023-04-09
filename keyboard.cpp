#include "keyboard.h"

bool keyboard::is_key_pressed(unsigned char c) const
{
    return key_states[c];
}
std::optional<keyboard::kbd_event> keyboard::read_key()
{
    if (events.size() > 0u)
    {
        keyboard::kbd_event e = events.front();
        events.pop();
        return e;
    }
    else
        return std::nullopt;
}
void keyboard::clear_event_que()
{
    while (!events.empty())
        events.pop();
}
bool keyboard::is_event_que_empty() const
{
    return events.empty();
}
unsigned char keyboard::read_char()
{
    if (events.size() > 0u)
    {
        unsigned char c = char_que.front();
        events.pop();
        return c;
    }
    else
        return 0;
}
bool keyboard::char_is_empty() const
{
    return char_que.empty();
}
void keyboard::clear_char_que()
{
    while (!char_que.empty())
        char_que.pop();
}
void keyboard::enable_autorepeat()
{
    autorepeat = true;
}
void keyboard::disable_autorepeat()
{
    autorepeat = false;
}
bool keyboard::is_autorepeat() const
{
    return autorepeat;
}
void keyboard::set_event_down(const unsigned char code)
{
    key_states[code] = true;
    events.push(keyboard::kbd_event(keyboard::kbd_event::type::down, code));
    trim_buf(events);
}
void keyboard::set_event_up(const unsigned char code)
{
    key_states[code] = false;
    events.push(keyboard::kbd_event(keyboard::kbd_event::type::up, code));
    trim_buf(events);
}
void keyboard::set_char(const unsigned char code)
{
    char_que.push(code);
    trim_buf(char_que);
}
void keyboard::clear_states()
{
    key_states.reset();
}
template<typename T>
inline void keyboard::trim_buf(std::queue<T>& buf)
{
    while (buf.size() > buffer_size)
        buf.pop();
}
