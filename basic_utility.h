#pragma once
#include "better_windows.h"
#include <string>
#include <cmath>
#include <algorithm>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template<typename T>
T wrap_angle(T theta)
{
    constexpr T twoPi = (T)2 * (T)PI_D;
    const T mod = fmod(theta, twoPi);
    if (mod > (T)PI_D)
    {
        return mod - twoPi;
    }
    else if (mod < (T)PI_D)
    {
        return mod + twoPi;
    }
    return mod;
}


inline std::wstring convert_multibyte_to_wide(const std::string& as)
{
    // deal with trivial case of empty string
    if (as.empty())    return std::wstring();

    // determine required length of new string
    size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

    // construct new string of required length
    std::wstring ret(reqLength, L'\0');

    // convert old string to new string
    ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

    // return new string ( compiler should optimize this away )
    return ret;
}

inline std::string get_name_from_path(std::string& path)
{
    std::string::iterator name_itr;
    for (auto i = path.begin(); i != path.end(); ++i)
    {
        if (*i == '\\')
            name_itr = ++i;
    }
    std::string temp;
    for (; name_itr != path.end(); ++name_itr)
        temp.push_back(*name_itr);
    return temp;
}
inline std::string get_name_from_path(std::string&& path)
{
    std::string::iterator name_itr = path.begin();
    for (auto i = path.begin(); i != path.end(); ++i)
    {
        if (*i == '\\')
            name_itr = ++i;
    }
    std::string temp;
    for (; name_itr != path.end(); ++name_itr)
        temp.push_back(*name_itr);
    return temp;
}

inline std::string get_root_path(std::string& path)
{
    std::string::iterator new_end;
    for (auto i = path.begin(); i != path.end(); ++i)
    {
        if (*i == '\\')
            new_end = ++i;
    }
    std::string temp;
    for (auto i = path.begin(); i != new_end; ++i)
        temp.push_back(*i);
    return temp;
}