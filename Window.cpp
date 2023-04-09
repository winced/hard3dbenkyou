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
#include "Window.h"
#include "imgui_impl_win32.h"
#include <sstream>

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// Window Class Stuff
window::window_class window::window_class::wndClass;
window::window_class::window_class() noexcept
	:
	hInst( GetModuleHandle( nullptr ) )
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof( wc );
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = handle_msg_setup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetInstance();
	wc.hIcon = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	wc.hCursor = nullptr;
	wc.hbrBackground = nullptr;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = GetName();
	wc.hIconSm = static_cast<HICON>(LoadImage(GetInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 32, 32, 0));
	RegisterClassEx( &wc );
	RAWINPUTDEVICE rdev;
	rdev.usUsagePage = 0x01; // mouse usage
	rdev.usUsage = 0x02; // mouse usage
	rdev.hwndTarget = nullptr;
	rdev.dwFlags = 0;
	if (RegisterRawInputDevices(&rdev, 1u, sizeof(rdev)) == FALSE)
		throw windows_sdk_shippai(__LINE__, __FILE__, 0);
}
window::window_class::~window_class()
{
	UnregisterClass( wndClassName,GetInstance() );
}
const char* window::window_class::GetName() noexcept
{
	return wndClassName;
}
HINSTANCE window::window_class::GetInstance() noexcept
{
	return wndClass.hInst;
}
// Window Stuff
window::window( int width,int height,const char* name) 
	:
	width(width),
	height(height)
{
	// calculate window size based on desired client region size
	RECT wr;
	wr.left = 100;
	wr.right = width + wr.left;
	wr.top = 100;
	wr.bottom = height + wr.top;
	// adjustes the rect to include the size of the non client region based on the options provided 
	AdjustWindowRect( &wr,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,FALSE );
	
	//throw window::window_shippai(__LINE__, __FILE__, ERROR_ARENA_TRASHED);
	// create window & get hWnd
	hWnd = CreateWindow(window_class::GetName(),name,WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,CW_USEDEFAULT,CW_USEDEFAULT,
						wr.right - wr.left,wr.bottom - wr.top, nullptr, nullptr, window_class::GetInstance(), this);
	if (hWnd == nullptr)
		throw windows_sdk_shippai(__LINE__, __FILE__, GetLastError());
	HICON icon0;
	ShowWindow( hWnd,SW_SHOWDEFAULT );
	ImGui_ImplWin32_Init(hWnd);
	graphics_c = std::make_unique<graphics>(hWnd, width, height);
}
window::~window()
{
	ImGui_ImplWin32_Shutdown();
	DestroyWindow( hWnd );
}
void window::set_title(const std::string& new_title)
{
	SetWindowText(hWnd, new_title.c_str());
}
std::optional<int> window::process_messages()
{
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}
graphics& window::get_graphics_c()
{
	return *graphics_c;
}
void window::disable_cursor()
{
	hide_cursor();
	disable_imgui_input();
	confine_cursor();
	cursor_hidden = true;
}
void window::enable_cursor()
{
	show_cursor();
	enable_imgui_input();
	free_cursor();
	cursor_hidden = false;
}
bool window::cursor_disabled()
{
	return cursor_hidden;
}
void window::hide_cursor()
{
	while (::ShowCursor(FALSE) >= 0);
}
void window::show_cursor()
{
	while (::ShowCursor(TRUE) < 0);
}
void window::disable_imgui_input()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
	imgui_input_disabled = true;
}
void window::enable_imgui_input()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
	imgui_input_disabled = false;
}
LRESULT CALLBACK window::handle_msg_setup( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ) noexcept
{
	// use create parameter passed in from CreateWindow() to store window class pointer at WinAPI side
	// WM_NCCREATE is sent when the window is created, its lParam is a pointer to a createstruct, which contains information about the window (also includes given userdata)
	if( msg == WM_NCCREATE )
	{
		// extract ptr to window class from creation data
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
		window* const p_wnd = static_cast<window*>(pCreate->lpCreateParams);

		// set WinAPI-managed user data to store ptr to window class
		SetWindowLongPtr( hWnd,GWLP_USERDATA,reinterpret_cast<LONG_PTR>(p_wnd) );

		// set message proc to normal (non-setup) handler now that setup is finished
		SetWindowLongPtr( hWnd,GWLP_WNDPROC,reinterpret_cast<LONG_PTR>(&window::handle_msg_thunk) );

		// forward message to window class handler
		return p_wnd->handle_msg( hWnd, msg, wParam, lParam );
	}
	// if we get a message before the WM_NCCREATE message, handle with default handler
	return DefWindowProc( hWnd,msg,wParam,lParam );
}
LRESULT CALLBACK window::handle_msg_thunk( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	// get the pointer to the window class and call the message handler 
	window* const pWnd = reinterpret_cast<window*>(GetWindowLongPtr( hWnd,GWLP_USERDATA ));
	return pWnd->handle_msg( hWnd,msg,wParam,lParam );
}
LRESULT window::handle_msg( HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam ) noexcept
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
		//keyboard
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		if (!(lParam & 0x40000000) || kbd.is_autorepeat())
			kbd.set_event_down(static_cast<unsigned int>(wParam));
		break;
	case WM_KEYUP:
	case WM_SYSKEYUP:
		kbd.set_event_up(static_cast<unsigned int>(wParam));
		break;
	case WM_CHAR:
		kbd.set_char(static_cast<unsigned int>(wParam));
		break;
	case WM_KILLFOCUS:
		kbd.clear_states();
		break;
		//mouse
	case WM_MOUSEMOVE:
		POINTS pt = MAKEPOINTS(lParam);
		if (cursor_hidden)
		{
			if (!mouse_c.in_window()) // we recorded a 
			{
				SetCapture(hWnd);
				mouse_c.set_mouse_enter();
				hide_cursor();
			}
			break;
		}
		if (pt.x >= 0 && pt.x < width && pt.y >= 0 && pt.y < height)
		{
			mouse_c.set_mouse_move(pt.x, pt.y);
			if (!mouse_c.in_window())
			{
				SetCapture(hWnd);
				mouse_c.set_mouse_enter();
			}
		}
		else
		{
			if (wParam & (MK_LBUTTON | MK_RBUTTON))
			{
				mouse_c.set_mouse_move(pt.x, pt.y);
			}
			// button up -> release capture / log event for leaving
			else
			{
				ReleaseCapture();
				mouse_c.set_mouse_leave();
			}
		}
		break;
	case WM_LBUTTONDOWN:
		SetForegroundWindow(hWnd);
		if (cursor_hidden)
		{
			confine_cursor();
			hide_cursor();
		}
		// stifle this mouse message if imgui wants to capture
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		mouse_c.set_left_pressed(true);
		break;
	case WM_RBUTTONDOWN:
		SetForegroundWindow(hWnd);
		if (cursor_hidden)
		{
			confine_cursor();
			hide_cursor();
		}
		// stifle this mouse message if imgui wants to capture
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		mouse_c.set_right_pressed(true);
		break;
	case WM_LBUTTONUP:
		SetForegroundWindow(hWnd);
		if (cursor_hidden)
		{
			confine_cursor();
			hide_cursor();
		}
		// stifle this mouse message if imgui wants to capture
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS ptl = MAKEPOINTS(lParam);
		if (ptl.x < 0 || ptl.x >= width || ptl.y < 0 || ptl.y >= height)
		{
			ReleaseCapture();
			mouse_c.set_mouse_leave();
		}
		mouse_c.set_left_pressed(false);
		break;
	case WM_RBUTTONUP:
		SetForegroundWindow(hWnd);
		if (cursor_hidden)
		{
			confine_cursor();
			hide_cursor();
		}
		// stifle this mouse message if imgui wants to capture
		if (ImGui::GetIO().WantCaptureMouse)
		{
			break;
		}
		const POINTS ptr = MAKEPOINTS(lParam);
		if (ptr.x < 0 || ptr.x >= width || ptr.y < 0 || ptr.y >= height)
		{
			ReleaseCapture();
			mouse_c.set_mouse_leave();
		}
		mouse_c.set_right_pressed(false);
		break;
	case WM_INPUT:
		UINT size = 0;
		if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, nullptr, &size, sizeof(RAWINPUTHEADER)) == 1)
			break;
		raw_buffer.resize(size);
		GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, raw_buffer.data(), &size, sizeof(RAWINPUTHEADER));
		auto& r_input = reinterpret_cast<const RAWINPUT&>(*raw_buffer.data());
		if (r_input.header.dwType == RIM_TYPEMOUSE && mouse_c.get_raw_enabled() && (r_input.data.mouse.lLastX != 0 || r_input.data.mouse.lLastY != 0))
		{
			mouse_c.set_raw_delta(r_input.data.mouse.lLastX, r_input.data.mouse.lLastY);
		}
		break;
	}

	return DefWindowProc( hWnd,msg,wParam,lParam );
}
void window::confine_cursor() noexcept
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	MapWindowPoints(hWnd, nullptr, reinterpret_cast<POINT*>(&rect), 2);
	ClipCursor(&rect);
}
void window::free_cursor() noexcept
{
	ClipCursor(nullptr);
}
// io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear / overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard
