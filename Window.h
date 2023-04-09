/******************************************************************************************
*	Chili Direct3D Engine																  *
*	Copyright 2018 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of Chili Direct3D Engine.											  *
*																						  *
*	Chili Direct3D Engine is free software: you can redistribute it and/or modify		  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili Direct3D Engine is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili Direct3D Engine.  If not, see <http://www.gnu.org/licenses/>.    *
******************************************************************************************/
#pragma once
#include <optional>
#include <vector>
#include <algorithm>
#include <memory>

#include "better_windows.h"
#include "exeption.h"
#include "resource.h"
#include "Vec2.h"
#include "mouse.h"
#include "keyboard.h"
#include "graphics.h"


class window
{
public:
	window(int width, int height, const char* name);
	~window();
	window(const window&) = delete;
	window& operator=(const window&) = delete;
	void set_title(const std::string& new_title);
	static std::optional<int> process_messages();
	graphics& get_graphics_c();
	void disable_cursor();
	void enable_cursor();
	bool cursor_disabled();
private:
	class window_class
	{
	public:
		static const char* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;
	private:
		window_class() noexcept;
		~window_class();
		window_class( const window_class& ) = delete;
		window_class& operator=( const window_class& ) = delete;
		static constexpr const char* wndClassName = "Direct3D benkyou";
		static window_class wndClass;
		HINSTANCE hInst;
	};
private:
	// can pass static functions because they behave the same way as free functions
	static LRESULT CALLBACK handle_msg_setup( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	static LRESULT CALLBACK handle_msg_thunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	// cant pass a memberfunction to windows API 
	LRESULT handle_msg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept;
	void confine_cursor() noexcept;
	void free_cursor() noexcept;
	void hide_cursor();
	void show_cursor();
	void disable_imgui_input();
	void enable_imgui_input();
private:
	bool cursor_hidden = false; 
	bool imgui_input_disabled = false;
	int width;
	int height;
	HWND hWnd;
	std::unique_ptr<graphics> graphics_c;
	std::vector<BYTE> raw_buffer;
public:
	keyboard kbd;
	mouse mouse_c;
};


