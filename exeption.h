#pragma once
#include "better_windows.h"
#include <exception>
#include <string>


class shippai : public std::exception
{
public:
	shippai(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* get_type() const noexcept;
	const std::string& get_file() const noexcept;
	const std::string get_oring_string() const noexcept;
private:
	int line;
	std::string file;
protected:
	mutable std::string what_buffer;
};

class windows_sdk_shippai : public shippai
{
public:
	windows_sdk_shippai(int line, const char* file, HRESULT hr) noexcept;
	const char* what() const noexcept override;
	const char* get_type() const noexcept override;
	static std::string translate_error_code(HRESULT hr);
	HRESULT get_error_code() const noexcept;
	std::string get_error_string() const noexcept;
private:
	HRESULT hr;
};