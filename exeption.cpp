#include "exeption.h"
#include <sstream>

shippai::shippai(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{}
const char* shippai::what() const noexcept
{
	std::stringstream ss;
	ss << "file: " << file << '\n' << "line: " << line << '\n';
	what_buffer = ss.str();
	return what_buffer.c_str();
}
const char* shippai::get_type() const noexcept
{
	return "fumei na shippai desu";
}
const std::string& shippai::get_file() const noexcept
{
	return file;
}
const std::string shippai::get_oring_string() const noexcept
{
	std::stringstream ss;
	ss << "file: " << file << '\n' << "line: " << line << '\n';
	auto s = ss.str();
	return ss.str();
}

windows_sdk_shippai::windows_sdk_shippai(int line, const char* file, HRESULT hr) noexcept
	:
	shippai(line, file),
	hr(hr)
{}
const char* windows_sdk_shippai::what() const noexcept
{
	std::stringstream ss;
	ss << shippai::get_oring_string();
	ss << translate_error_code(hr) << "error code: " << get_error_code() << '\n';
	what_buffer = ss.str();
	return what_buffer.c_str();
}
const char* windows_sdk_shippai::get_type() const noexcept
{
	return "graphical shippai desu";
}
std::string windows_sdk_shippai::translate_error_code(HRESULT hr)
{
	char* p_msg_buf = nullptr;
	DWORD n_msg_len = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&p_msg_buf), 0, nullptr);
	if (n_msg_len == 0)
		return "unidentified error code";
	std::string error_string = p_msg_buf;
	// windows allocates memory for the buffer, so we need to copy and free that memory 
	LocalFree(p_msg_buf);
	return error_string;
}
HRESULT windows_sdk_shippai::get_error_code() const noexcept
{
	return hr;
}
std::string windows_sdk_shippai::get_error_string() const noexcept
{
	return  translate_error_code(hr);
}
